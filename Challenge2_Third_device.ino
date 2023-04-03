#include <OOCSI.h>                                    // include OOCSI library

// connecting ESP to wifi
const char* ssid = "iotroam";                         // SSID of your Wifi network
const char* password = "OverMeNec";                   // Password of Wifi network

const char* OOCSIName = "pencilcase_third_device";    // name for connecting with OOCSI 
const char* hostserver = "oocsi.id.tue.nl";           // the adress of the OOCSI server

OOCSI oocsi = OOCSI();                                // connection to oocsi

// pin numbers
const int buzzPin = 33;                               // the number of the buzzer pin
int vibrationPin = 32;                                // the number of the vibration sensor pin
int pressurePinSelf_Third = 35;                       // the number of the pressure pin that controls soundoutput recorded by this device
int pressurePinOther_D4 = 34;                         // the number of the pressure pin that controls soundoutput recorded by D4
int pressurePinOther_9C = 39;                         // the number of the pressure pin that controls soundoutput recorded by 9C
int ledPin_9C = 25;                                   // green LED, the number of the led pin representing 9C 
int ledPin_D4 = 27;                                   // red LED, the number of the led pin representing D4 

// variables
int pressureStateSelf_Third = 0;                      // pressure state of pressurePinSelf_Third
int pressureStateOther_D4 = 0;                        // pressure state of pressurePinOther_D4
int pressureStateOther_9C = 0;                        // pressure state of pressurePinOther_9C
int pressureStateListening_D4 = 0;                    // receiving pressure state of D4
int pressureStateListening_9C = 0;                    // receiving pressure state of 9C

int vibrationStateReceive_D4 = 0;                     // vibration state of vibration data this code receives from D4
int vibrationStateReceive_9C = 0;                     // vibration state of vibration data this code receives from 9C
int vibrationStateSend = 0;                           // vibration state of vibration data this code sends, vibrationPin


void setup() {
  Serial.begin(115200);

  // pinmode identification for sensor pins
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin_D4, OUTPUT);  
  pinMode(ledPin_9C, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(pressurePinSelf_Third, INPUT);
  pinMode(pressurePinOther_9C, INPUT);
  pinMode(pressurePinOther_D4, INPUT);  

  // connect wifi and OOCSI to the server
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);

  // subscribe to the channels
  Serial.println("subscribing to pencilcaseCommunication1");
  oocsi.subscribe("pencilcaseCommunication1");
  
  Serial.println("subscribing to pencilcaseCommunication2");
  oocsi.subscribe("pencilcaseCommunication2");

  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {
  
  // connecting sensor data to right variable name
  pressureStateSelf_Third = analogRead(pressurePinSelf_Third);
  pressureStateOther_D4 = analogRead(pressurePinOther_D4);
  pressureStateOther_9C = analogRead(pressurePinOther_9C);
  vibrationStateSend = digitalRead(vibrationPin);


  // create a new message in pencilcaseCommunication3
  oocsi.newMessage("pencilcaseCommunication3");

  // Adding data to message and sending it
  oocsi.addInt("vib_3", vibrationStateSend);
  oocsi.addInt("3_listen_D4", pressureStateOther_D4);
  oocsi.addInt("3_listen_9C", pressureStateOther_9C);
  oocsi.sendMessage();

  // print data send to and received by oocsi in serial monitor
  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateReceive_9C);
  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateReceive_D4);
  Serial.print("Sensor Value Third: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening receiving pressure D4: ");
  Serial.println(pressureStateListening_D4);
  Serial.print("Sensor Value listening receive pressure 9C: ");
  Serial.println(pressureStateListening_9C);
  Serial.print("Sensor Value pressure self Third: ");
  Serial.println(pressureStateSelf_Third); 


  // buzzer control
  // pencils in pencilcase, no buzzer
  if (pressureStateSelf_Third < 4000 && pressureStateOther_D4 < 4000 && pressureStateOther_9C < 4000) {
    noTone(buzzPin);
  }

  // if all 3 pencils are out and vibration is detected in all devices, buzz loud
  else if (pressureStateSelf_Third > 4000 && pressureStateOther_D4 > 4000 && pressureStateOther_9C > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1 && vibrationStateReceive_D4 == 1) {
    tone(buzzPin, 1000);
    delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }

// if at least two pencils for listening are removed and vibration is detected by both, buzz 
  else if (pressureStateSelf_Third > 4000 && pressureStateOther_D4 > 4000 && vibrationStateSend == 1 && vibrationStateReceive_D4 == 1) {
  tone(buzzPin, 500);
  delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  else if (pressureStateSelf_Third > 4000 && pressureStateOther_9C > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1) {
  tone(buzzPin, 500);
  delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  else if (pressureStateOther_9C > 4000 && pressureStateOther_D4 > 4000 && vibrationStateReceive_9C == 1 && vibrationStateReceive_D4 == 1) {
  tone(buzzPin, 500);
  delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  
  // if at least 1 pencil for listening is removed and a vibration is detected by the pencil, buzz soft
  else if ((pressureStateSelf_Third > 4000 && vibrationStateSend == 1)||(pressureStateOther_9C > 4000 && vibrationStateReceive_9C)||(pressureStateOther_D4 > 4000 && vibrationStateReceive_D4)) {
    tone(buzzPin, 250);
    delay(1000);
  }

  // all other cases, no buzz sound
  else {
    noTone(buzzPin);
  }




  // if D4 is listening to you, light their representitive LED
  if (pressureStateListening_D4 > 4000) {
    digitalWrite(ledPin_D4, HIGH);
    //delay(1000);
  } 
  if (pressureStateListening_D4 < 4000) {
    digitalWrite(ledPin_D4, LOW);
  }

  // if 9C is listening to you, light their representitive LED
  if (pressureStateListening_9C > 4000) {
    digitalWrite(ledPin_9C, HIGH);
  } 
  if (pressureStateListening_9C < 4000) {
    digitalWrite(ledPin_9C, LOW);
  }

     
  delay(500);
  oocsi.check();      // oocsi checks for new messages
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {

  vibrationStateReceive_9C = oocsi.getInt("vib_9C", 0);           // incoming vibration data of 9C gets called vibrationStateReceive_9C
  pressureStateListening_9C = oocsi.getInt("9C_listen_3",0);      // incoming pressure data of 9C gets called pressureStateOther_9C

  vibrationStateReceive_D4 = oocsi.getInt("vib_D4", 0);           // incoming vibration data of D4 gets called vibrationStateReceive_D4
  pressureStateListening_D4 = oocsi.getInt("D4_listen_3",0);      // incoming pressure data of D4 gets called pressureStateOther_D4

}
