#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#include <OOCSI.h>                              // include OOCSI library

// connecting ESP to wifi
const char* ssid = "iotroam";                   // SSID of your Wifi network
const char* password = "PencilCase123";         // Password of Wifi network

const char* OOCSIName = "pencilcase_sender";    // name for connecting with OOCSI 
const char* hostserver = "oocsi.id.tue.nl";     // the adress of the OOCSI server

OOCSI oocsi = OOCSI();                          // connection to oocsi

// pin numbers
const int buzzPin = 33;                         // the number of the buzzer pin
int vibrationPin = 32;                          // the number of the vibration sensor pin
int pressurePinSelf_D4 = 35;                    // the number of the pressure pin that controls soundoutput recorded by this device
int pressurePinOther_9C = 34;                   // the number of the pressure pin that controls soundoutput recorded by 9C
int pressurePinOther_Third = 39;                // the number of the pressure pin that controls soundoutput recorded by third device
int ledPin_9C = 25;                             // green LED, the number of the led pin representing 9C
int ledPin_Third = 27;                          // yellow LED, the number of the led pin representing third device

// variables
int pressureStateSelf_D4 = 0;                   // pressure state of pressurePinSelf_D4
int pressureStateOther_Third = 0;               // pressure state of pressurePinOther_Third
int pressureStateOther_9C = 0;                  // pressure state of pressurePinOther_9C
int pressureStateListening_Third = 0;           // receiving pressure state of third device
int pressureStateListening_9C = 0;              // receiving pressure state of 9C

int vibrationStateReceive_9C = 0;               // vibration state of vibration data this code receives from 9C
int vibrationStateReceive_Third = 0;            // vibration state of vibration data this code receives from third device
int vibrationStateSend = 0;                     // vibration state of vibration data this code sends, vibrationPin

int clickWeb = 0;

void setup() {
  Serial.begin(115200);

  // pinmode identification for sensor pins
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin_9C, OUTPUT);
  pinMode(ledPin_Third, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(pressurePinSelf_D4, INPUT);
  pinMode(pressurePinOther_9C, INPUT);  
  pinMode(pressurePinOther_Third, INPUT);  


  // connect wifi and OOCSI to the server
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);

  // subscribe to the channels
  Serial.println("subscribing to pencilcaseCommunication2");
  oocsi.subscribe("pencilcaseCommunication2");

  Serial.println("subscribing to pencilcaseCommunication3");
  oocsi.subscribe("pencilcaseCommunication3");
  
  Serial.println("subscribing to pencilsenderweb");
  oocsi.subscribe("pencilsenderweb");
  
  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {

  // connecting sensor data to right variable name
  pressureStateSelf_D4 = analogRead(pressurePinSelf_D4);
  pressureStateOther_9C = analogRead(pressurePinOther_9C);
  pressureStateOther_Third = analogRead(pressurePinOther_Third);
  vibrationStateSend = digitalRead(vibrationPin);


  // create a new message in pencilcaseCommunication1
  oocsi.newMessage("pencilcaseCommunication1");

  // Adding data to message and sending it
  oocsi.addInt("vib_D4", vibrationStateSend);
  oocsi.addInt("D4_listen_3", pressureStateOther_Third);
  oocsi.addInt("D4_listen_9C", pressureStateOther_9C);
  oocsi.sendMessage();

  // print data send to and received by oocsi in serial monitor
  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateReceive_9C);
  Serial.print("Sensor Value Third: ");
  Serial.println(vibrationStateReceive_Third);
  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening receive pressure Third: ");
  Serial.println(pressureStateListening_Third);
  Serial.print("Sensor Value listening receive pressure 9C: ");
  Serial.println(pressureStateListening_9C);
  Serial.print("Sensor Value pressure self D4: ");
  Serial.println(pressureStateSelf_D4); 

  if (clickWeb == 1){
    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);

    tone(buzzPin, 1500);
    delay(750);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(750);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(750);
    noTone(buzzPin);
    delay(250);

    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);
    tone(buzzPin, 1500);
    delay(250);
    noTone(buzzPin);
    delay(250);
    
    clickWeb=0;
  }
 if (clickWeb == 0){
  noTone(buzzPin);
 }
  
  // buzzer control
  // pencils in pencilcase, no buzzer
  if (pressureStateSelf_D4 < 4000 && pressureStateOther_9C < 4000 && pressureStateOther_Third <4000) {
    noTone(buzzPin);
  }
  
  // if all 3 pencils are out and vibration is detected in all devices, buzz  loud
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_9C > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 1000);
    delay(500);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  
  // if at least two pencils for listening are removed and vibration is detected by both, buzz 
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_9C > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1) {
    tone(buzzPin, 500);
    delay(500);
  }
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 500);
    delay(500);
  }
  else if (pressureStateOther_9C > 4000 && pressureStateOther_Third > 4000 && vibrationStateReceive_9C == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 500);
    delay(500);
  }
 
  // if at least 1 pencil for listening is removed and a vibration is detected by the pencil, buzz soft
  else if (( pressureStateOther_9C > 4000 && vibrationStateReceive_9C == 1)||( pressureStateOther_Third > 4000 && vibrationStateReceive_Third == 1)||(pressureStateSelf_D4 && vibrationStateSend == 1)) {
    tone(buzzPin, 250);
    delay(500);
  }
  
  // all other cases, no buzz sound
  else {
    noTone(buzzPin);
  }



  // if 9C is listening to you, light their representitive LED
  if (pressureStateListening_9C > 4000) {
    digitalWrite(ledPin_9C, HIGH);
  } 
  if (pressureStateListening_9C < 4000){
    digitalWrite(ledPin_9C, LOW);
  }

  // if third device is listening to you, light their representitive LED
  if (pressureStateListening_Third > 4000) {
    digitalWrite(ledPin_Third, HIGH);
  }
  if (pressureStateListening_Third < 4000) {
    digitalWrite(ledPin_Third, LOW);
  }
  
 
  delay(500);
  oocsi.check();      // oocsi checks for new messages
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {

  vibrationStateReceive_9C = oocsi.getInt("vib_9C", 0);           // incoming vibration data of 9C gets called vibrationStateReceive_9C
  pressureStateListening_9C = oocsi.getInt("d9C_listen_D4", pressureStateListening_9C);    // incoming pressure data of 9C gets called pressureStateListening_9C

  vibrationStateReceive_Third = oocsi.getInt("vib_3", 0);         // incoming vibration data of third device gets called vibrationStateReceive_Third
  pressureStateListening_Third = oocsi.getInt("d3_listen_D4", pressureStateListening_Third);  // incoming pressure data of third device gets called pressureStateListening_Third

  clickWeb = oocsi.getInt("send", 0);
}
