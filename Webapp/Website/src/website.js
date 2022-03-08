function submit() {
    console.log("Submit Clicked!");

    //Read in all inputs
    let theTemperature = document.getElementById('Temperature').value;
    let theFeed = document.getElementById('Feed').value;

    let LightOnHour =       document.getElementById('LightOnHour').value;
    let LightOnMinute =     document.getElementById('LightOnMinute').value;
    let LightOnAMPM =       document.getElementById('LightOnAMPM').value;

    let LightOffHour =      document.getElementById('LightOffHour').value;
    let LightOffMinute =    document.getElementById('LightOffMinute').value;
    let LightOffAMPM =      document.getElementById('LightOffAMPM').value;

    //Construct LightOn and LightOff time strings

    //First take into account AM/PM
    if(LightOnAMPM == 1) LightOnHour = parseInt(LightOnHour) + 12;
    if(LightOffAMPM == 1) LightOffHour = parseInt(LightOffHour) + 12;

    //Now construct the string
    let LightOnTime = LightOnHour + ":" + LightOnMinute;
    let LightOffTime = LightOffHour + ":" + LightOffMinute;

    //Prepare the URL to fetch
    let theURL = "/Temperature/" + theTemperature + "/Feed/" + theFeed + "/on/" + LightOnTime + "/off/" + LightOffTime;

    //For offline testing, print theURL
    console.log(theURL);

    //Now fetch the URL. There will be no response, this is to communicate webpage info to arduino
    fetch(theURL);

}

async function update() {

    //Fetch /update/ to retrieve information from arduino
    let theURL = "/update";

    //For offline testing, print theURL
    console.log(theURL);

    //Fetch the path and parse it as a JSON file (this will need to be reflected in the server response)
    let response = await fetch(theURL).then(response => response.json());

    let theTemperature = response['currentTemperature'];
    console.log(theTemperature);

    let theCurrentTemperatureOutput = document.getElementById('#the-temperature');
    theCurrentTemperatureOutput.textContent = theTemperature;

    //If we decide to display anything else, we will need to add it here

}

//Setup the page to update every 5 seconds after the webpage has loaded
document.addEventListener('DOMContentLoaded', function() {
    //This code will execute once the webpage has loaded
    setInterval( function() {
        //This code will execute every 5 seconds
        update();
    }, 5000);

    const temperatureInput = document.querySelector('#Temperature');
    const temperatureDisplay = document.querySelector('#temp-current');
    temperatureInput.addEventListener('input', (e) => {
        temperatureDisplay.innerHTML = e.target.value;
    });

    const feedInput = document.querySelector('#Feed');
    const feedDisplay = document.querySelector('#feed-current');
    feedInput.addEventListener('input', (e) => {
        feedDisplay.innerHTML = e.target.value;
    });
 }, false);
