#include <Arduino.h>

const int buzzer = 33;
int vibpin;
int push;
int push2;

void setup()
{
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(vibpin, INPUT);
}

void loop()
{
  push = analogRead(4);
  vibpin = analogRead(32);
  push2 = analogRead(21);

  Serial.print("pressure value = ");
  Serial.println(push);
  
  if (push < 4000)
  {
    Serial.print("Sensor Value: ");
    Serial.println(vibpin);
    noTone(buzzer);
  }
  else if (push > 4000 && vibpin > 1000)
  {
    tone(buzzer, 500);
    Serial.print("Sensor Value: ");
    Serial.println(vibpin);
  }
  else
  {
    Serial.print("Sensor Value: ");
    Serial.println(vibpin);
    noTone(buzzer);
  }
  delay(100); // Small delay
}
