#include "OOCSI.h"

// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "iotroam";
// Password of your Wifi network.
const char* password = "PencilCase321";  

// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "pencilcase_receiver";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

OOCSI oocsi = OOCSI();

const int buzzPin = 33;          // the number of the buzzer pin
int vibrationPin = 32;           // the number of the vibration sensor pin
int pressurePinSelf_9C = 35;     // the number of the pressure pin 
int pressurePinOther_9C = 34;    // the number of the pressure pin 
int ledPin = 25;

int pressureStateOther_D4 = 0;
int pressureStateSelf_9C = 0;
int pressureStateOther_9C = 0;

int ledState = 0;

int vibrationStateReceive = 0;
int vibrationStateSend = 0;


void setup() {
  Serial.begin(115200);

  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationPin, INPUT); 
  pinMode(pressurePinSelf_9C, INPUT); 
  pinMode(pressurePinOther_9C, INPUT);  

  
  // setting up OOCSI. processOOCSI is the name of the fucntion to call when receiving messages, can be a random function name
  // connect wifi and OOCSI to the server
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);

  // subscribe to a channel
  Serial.println("subscribing to pencilcaseCommunicationl");
  oocsi.subscribe("pencilcaseCommunication1");

  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {
  
  pressureStateSelf_9C = analogRead(pressurePinSelf_9C);
  pressureStateOther_9C = analogRead(pressurePinOther_9C);
  vibrationStateSend = digitalRead(vibrationPin);
 

  // create a new message
  oocsi.newMessage("pencilcaseCommunication2");

  //Constante loop versturen vibratie waardes
  oocsi.addInt("vibration_device2", vibrationStateSend);
  oocsi.addInt("listeningPressure_device2", pressureStateOther_9C);
  oocsi.sendMessage();

  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateReceive);
  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening pressure D4: ");
  Serial.println(pressureStateOther_D4);
  Serial.print("Sensor Value listening pressure 9C: ");
  Serial.println(pressureStateOther_9C);
  
  // offstate 
  if (pressureStateSelf_9C < 4000 && pressureStateOther_9C < 4000){                                                          
      noTone(buzzPin);
  }
  
  // onstate with vibration
  else if (pressureStateSelf_9C > 4000 && pressureStateOther_9C > 4000 && vibrationStateSend == 1 && vibrationStateReceive == 1){   
      tone(buzzPin, 1000);
      delay(1000);
  } 
  else if (pressureStateSelf_9C > 4000 && vibrationStateSend == 1){                    
      tone(buzzPin, 500);
      delay(1000);
   }
  
  else if ( pressureStateOther_9C > 4000 && vibrationStateReceive == 1){                      
      tone(buzzPin, 500);
      delay(1000);
  }

  else{
      noTone(buzzPin);
  }

  if (pressureStateOther_D4 > 4000){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(500);
  oocsi.check();
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {
   
   vibrationStateReceive = oocsi.getInt("vibration_device1", 0);
   pressureStateOther_D4 = oocsi.getInt("listeningPressure_device1", 0);  
   
}
