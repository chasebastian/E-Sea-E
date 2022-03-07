#include <SoftwareSerial.h>

//Initialize Bluetooth pins
#define BLUETOOTH_RX = 2
#define BLUETOOTH_TX = 3

//Initialize Global Bluetooth object, for use in other modules
 SoftwareSerial Ser(BLUETOOTH_RX, BLUETOOTH_TX); // RX | TX 

void setupCommunication() {

  Ser.begin(9600);
  
}

/*
 * Receive a message one character at a time, stopping at a newline ('\n')
 */
String receiveMessage() {
  String message = "";
  if (Ser.available() > 0) {
    while (true) {
      char c = Ser.read();
      if (c == '\n')
        break;
      message += c;
    }
  }
  return message;
}

/*
 * Send a message over the communication protocol. It appears I can print whole strings.
 * TODO: Ask lingpeng if I can send via bluetooth more than one character
 */
void sendMessage(String message) {
  Ser.println(message);
}

/*
 * Appends a "\n" character to the end of an input String if needed.
 * May need to check compatability with Ser.println
 */
 String constructMessage(String message) {

  if(!message.endsWith("\n")) return message.concat("\n");
  else return message;
 }

 
