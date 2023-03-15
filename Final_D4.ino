#include "OOCSI.h"

// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "iotroam";
// Password of your Wifi network.
const char* password = "PencilCase123";  

// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "pencilcase_sender";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

OOCSI oocsi = OOCSI();

const int buzzPin = 33;          // the number of the buzzer pin
int vibrationPin = 32;           // the number of the vibration sensor pin
int pressurePin = 4;             // the number of the pressure pin 
int pressurePinOther = 34;       // the number of the pressure pin 

int pressureState = 0;
int pressureStateOther = 0;

int vibrationStateReceive = 0;
int vibrationStateSend = 0;

//int volume;

void setup() {
  Serial.begin(115200);

  pinMode(buzzPin, OUTPUT);
  pinMode(pressurePin, INPUT);
  pinMode(vibrationPin, INPUT);  
  pinMode(pressurePinOther, INPUT);  

  // setting up OOCSI. processOOCSI is the name of the fucntion to call when receiving messages, can be a random function name
  // connect wifi and OOCSI to the server
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);

  // subscribe to a channel
  Serial.println("subscribing to pencilcaseCommunication2");
  oocsi.subscribe("pencilcaseCommunication2");

  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {
  
  pressureState = analogRead(pressurePin);
  pressureStateOther = analogRead(pressurePinOther);
  vibrationStateSend = analogRead(vibrationPin);
  

  // create a new message
  oocsi.newMessage("pencilcaseCommunication1");

  //Constante loop versturen en ontvangen vibratie waardes
  oocsi.addInt("vibration_device1", vibrationStateSend);
  oocsi.sendMessage();
     
 // vibrationStateReceive = oocsi.getInt("vibration_device2", 0); is al gedefineerd in processoocsi

  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateReceive);
  Serial.print("Sensor Value C9: ");
  Serial.println(vibrationStateSend);
  
  // offstate 
  if (pressureState < 4000 && pressureStateOther < 4000){ 
      noTone(buzzPin);
  }
  
  // onstate with vibration 
  else if (pressureState > 4000 && pressureStateOther < 4000 && vibrationStateSend < 100){
    tone(buzzPin, 100);
  }
  else if (pressureState < 4000 && pressureStateOther > 4000 && vibrationStateReceive < 100){
    tone(buzzPin, 100);
  }
  else if (pressureState < 4000 && pressureStateOther > 4000 && vibrationStateSend < 100 && vibrationStateReceive < 100){
    tone(buzzPin, 300);
  } 
  else{
      noTone(buzzPin);
  }
  delay(1000);
  oocsi.check();
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {
   
   vibrationStateReceive = oocsi.getInt("vibration_device2", 0);
   
  // use this to print out the raw message that was received
  oocsi.printMessage();
}
