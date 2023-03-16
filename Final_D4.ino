#include <ESP32Tone.h>
#include <OOCSI.h>

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
int pressurePinSelf_D4 = 35;     // the number of the pressure pin
int pressurePinOther_D4 = 34;    // the number of the pressure pin
int ledPin = 25;

int pressureStateSelf_D4 = 0;
int pressureStateOther_D4 = 0;
int pressureStateOther_9C = 0;

int vibrationStateReceive = 0;
int vibrationStateSend = 0;


void setup() {
  Serial.begin(115200);

  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(pressurePinSelf_D4, INPUT);
  pinMode(pressurePinOther_D4, INPUT);

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

  pressureStateSelf_D4 = analogRead(pressurePinSelf_D4);
  pressureStateOther_D4 = analogRead(pressurePinOther_D4);
  vibrationStateSend = digitalRead(vibrationPin);


  // create a new message
  oocsi.newMessage("pencilcaseCommunication1");

  //Constante loop versturen en ontvangen vibratie waardes
  oocsi.addInt("vibration_device1", vibrationStateSend);
  oocsi.addInt("listeningPressure_device1", pressureStateOther_D4);
  oocsi.sendMessage();

  Serial.print("Sensor Value 9C: ");
  Serial.println(vibrationStateReceive);
  Serial.print("Sensor Value D4: ");
  Serial.println(vibrationStateSend);

  Serial.print("Sensor Value listening pressure 9C: ");
  Serial.println(pressureStateOther_9C);
  Serial.print("Sensor Value listening pressure D4: ");
  Serial.println(pressureStateOther_D4);

  // offstate
  if (pressureStateSelf_D4 < 4000 && pressureStateOther_D4 < 4000) {
    noTone(buzzPin);
  }

  // onstate with vibration
  else if (pressureStateSelf_D4 > 4000 && pressureStateOther_D4 > 4000 && vibrationStateSend == 1 && vibrationStateReceive == 1) {
    tone(buzzPin, 1000);
    delay(1000);
  }
  else if (pressureStateSelf_D4 > 4000 && vibrationStateSend == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }

  else if ( pressureStateOther_D4 > 4000 && vibrationStateReceive == 1) {
    tone(buzzPin, 500);
    delay(1000);
  }

  else {
    noTone(buzzPin);
  }

  if (pressureStateOther_9C > 4000) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(500);
  oocsi.check();
}

// function which OOCSI calls when an OOCSI message is received
void processOOCSI() {

  vibrationStateReceive = oocsi.getInt("vibration_device2", 0);
  pressureStateOther_9C = oocsi.getInt("listeningPressure_device2", 0);  

}
