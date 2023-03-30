#include <OOCSI.h>                  // include OOCSI library

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
int pressurePinOther_9C = 34;                   // the number of the pressure pin that controls soundoutput recorded by the other device
int pressurePinOther_Third = 39;
int ledPin_9C = 25;                     // green, the number of the led pin
int ledPin_Third = 27;                  // yellow, the number of the led pin

// variables
int pressureStateSelf_D4 = 0;                   // pressure state of pressurePinSelf_D4
int pressureStateOther_Third = 0;                  // pressure state of pressurePinOther_D4
int pressureStateOther_9C = 0;                  // pressure state of pressurePinOther_9C
int pressureStateListening_Third = 0;                  // pressure state of pressurePinOther_D4
int pressureStateListening_9C = 0;                  // pressure state of pressurePinOther_9C

int vibrationStateReceive_9C = 0;                  // vibration state of vibration data this code receives
int vibrationStateReceive_Third = 0;
int vibrationStateSend = 0;                     // vibration state of vibration data this code sends, vibrationPin


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

  // subscribe to a channel
  Serial.println("subscribing to pencilcaseCommunication2");
  oocsi.subscribe("pencilcaseCommunication2");

  Serial.println("subscribing to pencilcaseCommunication3");
  oocsi.subscribe("pencilcaseCommunication3");
  
  // check if we are in the client list
  Serial.print("is ");
  Serial.print(OOCSIName);
  Serial.print(" a client? --> ");
  Serial.println(oocsi.containsClient(OOCSIName));
}

void loop() {

  pressureStateSelf_D4 = analogRead(pressurePinSelf_D4);
  pressureStateOther_9C = analogRead(pressurePinOther_9C);
  pressureStateOther_Third = analogRead(pressurePinOther_Third);
  vibrationStateSend = digitalRead(vibrationPin);


  // create a new message in pencilcaseCommunication
  oocsi.newMessage("pencilcaseCommunication1");

  // Adding data to message and sending it
  oocsi.addInt("vibration_device1", vibrationStateSend);
  oocsi.addInt("device1_listening_to_Third", pressureStateOther_Third);
  oocsi.addInt("device1_listening_to_9C", pressureStateOther_9C);
  oocsi.sendMessage();

  // print data send to and received by oocsi in serial monitor
  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateReceive_9C);
  Serial.print("Sensor Value Third: ");
  Serial.println(vibrationStateReceive_Third);
  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening sending pressure Third: ");
  Serial.println(pressureStateListening_Third);
  Serial.print("Sensor Value listening sending pressure 9C: ");
  Serial.println(pressureStateListening_9C);
  Serial.print("Sensor Value pressure self D4: ");
  Serial.println(pressureStateSelf_D4); 


  // buzzer control
  // pencils in pencilcase, no buzzer
  if (pressureStateSelf_D4 < 4000 && pressureStateOther_9C < 4000 && pressureStateOther_Third <4000) {
    noTone(buzzPin);
  }
  
  // if all 3 pencils are out and vibration is detected in both devices, buzz loud
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_9C > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 1000);
    delay(1000);                    // this delay is added so that the buzzer plays for at least 1 second
  }
  
  // if the pencil for listening to own vibration is removed and a vibration is detected by own device, buzz
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_9C > 4000 && vibrationStateSend == 1 && vibrationStateReceive_9C == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_Third > 4000 && vibrationStateSend == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
  else if (pressureStateOther_9C > 4000 && pressureStateOther_Third > 4000 && vibrationStateReceive_9C == 1 && vibrationStateReceive_Third == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }
 
  // if the pencil for listening to other vibration is removed and a vibration is detected by other device, buzz
  else if (( pressureStateOther_9C > 4000 && vibrationStateReceive_9C == 1)||( pressureStateOther_Third > 4000 && vibrationStateReceive_Third == 1)||(pressureStateSelf_D4 && vibrationStateSend == 1)) {
    tone(buzzPin, 250);
    delay(1000);
  }
  
  // all other cases, no buzz sound
  else {
    noTone(buzzPin);
  }

  // if the other device is listening to you, show light
  if (pressureStateListening_9C > 4000) {
    digitalWrite(ledPin_9C, HIGH);
    //delay(1000);
  } 
  if (pressureStateListening_9C < 4000){
    digitalWrite(ledPin_9C, LOW);
  }
  if (pressureStateListening_Third > 4000) {
    digitalWrite(ledPin_Third, HIGH);
    //delay(1000);
  }
  if (pressureStateListening_Third < 4000) {
    digitalWrite(ledPin_Third, LOW);
  }
  
 
  delay(500);
  oocsi.check();      // oocsi checks for new messages
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {

  vibrationStateReceive_9C = oocsi.getInt("vibration_device2", 0);           // incoming vibration data gets called vibrationStateReceive
  pressureStateListening_9C = oocsi.getInt("device2_listening_to_D4", 0);   // incoming pressure data gets called pressureStateOther_9C

  vibrationStateReceive_Third = oocsi.getInt("vibration_device3", 0);           // incoming vibration data gets called vibrationStateReceive
  pressureStateListening_Third = oocsi.getInt("device3_listening_to_D4", 0);   // incoming pressure data gets called pressureStateOther_9C

}
