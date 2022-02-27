from ECE16Lib.CircularList import CircularList
import ECE16Lib.DSP as filt
import numpy as np
import glob
import pickle

# The GMM Import
from sklearn.mixture import GaussianMixture as GMM

"""
A class to enable a simple heart rate monitor
"""
class HRMonitor:
  """
  Encapsulated class attributes (with default values)
  """
  __hr = 0           # the current heart rate
  __time = None      # CircularList containing the time vector
  __ppg = None       # CircularList containing the raw signal
  __filtered = None  # CircularList containing filtered signal
  __num_samples = 0  # The length of data maintained
  __new_samples = 0  # How many new samples exist to process
  __fs = 0           # Sampling rate in Hz
  __thresh = 0.6     # Threshold from Tutorial 2
  __directory = "./data" #Directory used to get all data
  __gmm = None

  """
  Initialize the class instance
  """
  def __init__(self, num_samples, fs, times=[], data=[]):
    self.__hr = 0
    self.__num_samples = num_samples
    self.__fs = fs
    self.__time = CircularList(data, num_samples)
    self.__ppg = CircularList(data, num_samples)
    self.__filtered = CircularList([], num_samples)

  """
  Add new samples to the data buffer
  Handles both integers and vectors!
  """
  def add(self, t, x):
      if isinstance(t, np.ndarray):
          t = t.tolist()
      if isinstance(x, np.ndarray):
          x = x.tolist()

      if isinstance(x, int):
          self.__new_samples += 1
      else:
          self.__new_samples += len(x)

      self.__time.add(t)
      self.__ppg.add(x)

  """
  Get Subjects from given data directory, based on given naming convention
  """
  def get_subjects(self, directory):
      filepaths = glob.glob(directory + "\\*")
      return [filepath.split("\\")[-1] for filepath in filepaths]

  """
  Get Data from the name of a data file
  """
  def get_data(self, directory, subject, trial, fs):
      search_key = "%s/%s/%s_%02d_*.csv" % (directory, subject, subject, trial)
      filepath = glob.glob(search_key)[0]
      t, ppg = np.loadtxt(filepath, delimiter=',', unpack=True)
      t = (t - t[0]) / 1e3
      hr = self.get_hr(filepath, len(ppg), fs)

      fs_est = self.estimate_fs(t)
      if (fs_est < fs - 1 or fs_est > fs):
          print("Bad data! FS=%.2f. Consider discarding: %s" % (fs_est, filepath))

      return t, ppg, hr, fs_est

  """
  Gets HR from filename
  """
  def get_hr(self, filepath, num_samples, fs):
      count = int(filepath.split("_")[-1].split(".")[0])
      seconds = num_samples / fs
      return count / seconds * 60  # 60s in a minute

  """
  Gets sampling frequency from time input
  """
  def estimate_fs(self, times):
      return 1 / np.mean(np.diff(times))

  """
  Train GMM model from all offline data
  """
  def train(self):
      subjects = self.get_subjects(self.__directory)
      train_data = np.array([])
      for subject in subjects:
          for trial in range(1, 11):
              t, ppg, hr, fs_est = self.get_data(self.__directory, subject, trial, self.__fs)
              train_data = np.append(train_data, self.process_training(ppg))


      # Train the GMM
      train_data = train_data.reshape(-1, 1)  # convert from (N,1) to (N,) vector
      self.__gmm = GMM(n_components=2).fit(train_data)

  """
  Unpickles the gmm file to create the gmm as opposed to training everytime
  """
  def unpickle(self):
      self.__gmm = pickle.load( open("gmm.pickle", "rb") )

  """
  Predicts the heartrate based on labels output of GMM
  """
  def estimate_hr(self, labels, num_samples, fs):
      peaks = np.diff(labels, prepend=0) == 1
      count = sum(peaks)
      seconds = num_samples / fs
      hr = count / seconds * 60  # 60s in a minute
      return hr, peaks

  """
  Predicts the estimated heartrate using the GMM
  """
  def predict(self):
      # Grab only the new samples into a NumPy array
      x = np.array(self.__ppg[-self.__new_samples:])

      #Filter the new signal and add to classes filtered data
      x = self.process_training(x)
      self.__filtered.add(x.tolist())

      labels = self.__gmm.predict(x.reshape(-1, 1))

      heartrate, _ = self.estimate_hr(labels, len(x), self.__fs)
      self.__hr = heartrate
      return heartrate, self.__filtered


  """
  Process all offline data using same filtering methods as process()
  """
  def process_training(self, x):
      x = filt.detrend(x, 25)
      x = filt.moving_average(x, 5)
      x = filt.gradient(x)
      x = filt.moving_average(x, 5)
      return filt.normalize(x)

  """
  Determine if recent collection is a good sample
  takes in as input the indices of the peaks
  """

  def analyze_peaks(self, signal):
      array = np.array(signal)
      differences = np.diff(array)
      time_between_sample = .02
      for elem in differences:
          if elem * time_between_sample < .3:
              return False
      return True


  """
  Calculate adaptive threshold 20% above average signal
  takes in as input the filtered signal
  """
  def calculate_thresh(self, signal):
      array = np.array(signal)
      max = np.max(array)
      mean = np.mean(array)

      self.__thresh = mean + .2*(max - mean)


  """
  Compute the average heart rate over the peaks
  """
  def compute_heart_rate(self, peaks):
    t = np.array(self.__time)
    if len(peaks) > 1 and np.mean(np.diff(t[peaks])) != 0:
        hr = 60 / np.mean(np.diff(t[peaks])) * 1000
        print(hr)
        return hr
    else:
        return 0


  """
  Process the new data to update step count
  """
  def process(self):
      # Grab only the new samples into a NumPy array
      x = np.array(self.__ppg[-self.__new_samples:])

      # Filter the signal (feel free to customize!)
      x = filt.detrend(x, 25)
      x = filt.moving_average(x, 5)
      x = filt.gradient(x)
      x = filt.normalize(x)

      self.calculate_thresh(x)

      # Store the filtered data
      self.__filtered.add(x.tolist())

      # Find the peaks in the filtered data
      _, peaks = filt.count_peaks(x, self.__thresh , 1)

      # Update the step count and reset the new sample count
      self.__hr = self.compute_heart_rate(peaks)
      self.__new_samples = 0

      # Return the heart rate, peak locations, and filtered data
      return self.__hr, peaks, np.array(self.__filtered)


  """
  Clear the data buffers and step count
  """
  def reset(self):
      self.__steps = 0
      self.__time.clear()
      self.__ppg.clear()
      self.__filtered = np.zeros(self.__num_samples)
