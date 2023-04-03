#include "OOCSI.h"                             // include OOCSI library

// connecting ESP to wifi
const char* ssid = "iotroam";                  // SSID of your Wifi network
const char* password = "PencilCase321";        // Password of Wifi network

const char* OOCSIName = "pencilcase_receiver";  // name for connecting with OOCSI 
const char* hostserver = "oocsi.id.tue.nl";     // the adress of the OOCSI server

OOCSI oocsi = OOCSI();                          // connection to oocsi

// pin numbers
const int buzzPin = 33;                         // the number of the buzzer pin
int vibrationPin = 32;                          // the number of the vibration sensor pin
int pressurePinSelf_9C = 35;                    // the number of the pressure pin that controls soundoutput recorded by this device
int pressurePinOther_D4 = 34;                   // the number of the pressure pin that controls soundoutput recorded by D4
int pressurePinOther_Third = 39;                // the number of the pressure pin that controls soundoutput recorded by third device
int ledPin_D4 = 25;                             // red LED, the number of the led pin representing D4
int ledPin_Third = 27;                          // yellow LED, the number of the led pin representing third device

// variables
int pressureStateSelf_9C = 0;                   // pressure state of pressurePinSelf_9C
int pressureStateOther_D4 = 0;                  // pressure state of pressurePinOther_D4
int pressureStateOther_Third = 0;               // pressure state of pressurePinOther_Third
int pressureStateListening_D4 = 0;              // receiving pressure state of D4
int pressureStateListening_Third = 0;           // receiving pressure state of 9C

int vibrationStateReceive_D4 = 0;               // vibration state of vibration data this code receives from D4
int vibrationStateReceive_Third = 0;            // vibration state of vibration data this code receives from third device
int vibrationStateSend = 0;                     // vibration state of vibration data this code sends, vibrationPin


void setup() {
  Serial.begin(115200);

  // pinmode identification for sensor pins
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin_D4, OUTPUT);
  pinMode(ledPin_Third, OUTPUT);
  pinMode(vibrationPin, INPUT); 
  pinMode(pressurePinSelf_9C, INPUT); 
  pinMode(pressurePinOther_D4, INPUT);  
  pinMode(pressurePinOther_Third, INPUT);  
  
  // connect wifi and OOCSI to the server
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);

  // subscribe to the channels
  Serial.println("subscribing to pencilcaseCommunicationl");
  oocsi.subscribe("pencilcaseCommunication1");

  Serial.println("subscribing to pencilcaseCommunication3");
  oocsi.subscribe("pencilcaseCommunication3");

  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {

  // connecting sensor data to right variable name
  pressureStateSelf_9C = analogRead(pressurePinSelf_9C);
  pressureStateOther_D4 = analogRead(pressurePinOther_D4);
  pressureStateOther_Third = analogRead(pressurePinOther_Third);
  vibrationStateSend = digitalRead(vibrationPin);
 

  // create a new message in pencilcaseCommunication2
  oocsi.newMessage("pencilcaseCommunication2");

  // Adding data to message and sending it
  oocsi.addInt("vib_9C", vibrationStateSend);
  oocsi.addInt("9C_listen_3", pressureStateOther_Third);
  oocsi.addInt("9C_listen_D4", pressureStateOther_D4);
  oocsi.sendMessage();

  // print data send to and received by oocsi in serial monitor
  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateReceive_D4);
  Serial.print("Sensor Value Third: ");
  Serial.println(vibrationStateReceive_Third);
  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening receiving pressure D4: ");
  Serial.println(pressureStateListening_D4);
  Serial.print("Sensor Value listening receive pressure Third: ");
  Serial.println(pressureStateListening_Third);
  Serial.print("Sensor Value pressure self 9C: ");
  Serial.println(pressureStateSelf_9C); 


  //buzzer control
  // pencils in pencilcase, no buzzer
  if (pressureStateSelf_9C < 4000 && pressureStateOther_D4 < 4000 && pressureStateOther_Third < 4000){                                                          
      noTone(buzzPin);
  }
  
    // if all 3 pencils are out and vibration is detected in all devices, buzz loud
  else if (pressureStateSelf_9C > 4000 && pressureStateOther_D4 > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_D4 == 1 && vibrationStateReceive_Third == 1){   
      tone(buzzPin, 1000);
      delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  
  // if at least two pencils for listening are removed and vibration is detected by both, buzz 
  else if (pressureStateSelf_9C > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
  else if (pressureStateSelf_9C > 4000 && pressureStateOther_D4 > 4000 && vibrationStateSend == 1 && vibrationStateReceive_D4 == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
  else if (pressureStateOther_Third > 4000 && pressureStateOther_D4 > 4000 && vibrationStateReceive_Third == 1 && vibrationStateReceive_D4 == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
  // if at least 1 pencil for listening is removed and a vibration is detected by the pencil, buzz soft  else if (( pressureStateOther_D4 > 4000 && vibrationStateReceive_D4 == 1)||( pressureStateOther_Third > 4000 && vibrationStateReceive_Third == 1)||(pressureStateSelf_9C && vibrationStateSend == 1)) {
    tone(buzzPin, 250);
    delay(1000);
  }
  
  // all other cases, no buzz sound
  else{
      noTone(buzzPin);
  }



  // if D4 is listening to you, light their representitive LED
  if (pressureStateListening_D4 > 4000) {
    digitalWrite(ledPin_D4, HIGH);
  } 
    if (pressureStateListening_D4 < 4000) {
    digitalWrite(ledPin_D4, LOW);
  }

  // if third device is listening to you, light their representitive LED
  if (pressureStateListening_Third > 4000) {
    digitalWrite(ledPin_Third, HIGH);
  }
  if (pressureStateListening_Third < 4000) {
    digitalWrite(ledPin_Third, LOW);
  }
  
  delay(500);
  oocsi.check();    // oocsi checks for new messages
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {
   
   vibrationStateReceive_D4 = oocsi.getInt("vib_D4", 0);           // incoming vibration data of D4 gets called vibrationStateReceive_D4
   pressureStateListening_D4 = oocsi.getInt("D4_listen_9C", 0);    // incoming pressure data of D4 gets called pressureStateListening_D4

   vibrationStateReceive_Third = oocsi.getInt("vib_3", 0);         // incoming vibration data of third device gets called vibrationStateReceive_Third
   pressureStateListening_Third = oocsi.getInt("3_listen_9C", 0);  // incoming pressure data of third device gets called pressureStateListening_Third

}
