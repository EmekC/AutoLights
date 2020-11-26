#include <Arduino.h>
// relay
#define RELAY_PIN 5 //D1
void initRelay();
void turnOnRelay();
void turnOffRelay();

// sensor
#define SENSOR_PIN 13 //D7
int pirResult = 0;
void initSensor();
void onSensorDetect();

// variables
unsigned oldMillis = 0;

#define LIGHT_ON_TIME 3 * 60000
#define TIME_OFFSET 10000 // 10 sec

void setup() {
  initSensor();

  initRelay();
}

void loop() {
  pirResult = digitalRead(SENSOR_PIN);
  if (pirResult == HIGH) {
    oldMillis = millis();
    turnOnRelay();
    delay(LIGHT_ON_TIME);
  }

  if ((millis() - LIGHT_ON_TIME + TIME_OFFSET) > oldMillis) {
    turnOffRelay();
  }
}

void initSensor() {
  pinMode(SENSOR_PIN, INPUT);
}

void initRelay() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void turnOnRelay() {
  digitalWrite(RELAY_PIN, LOW);
}

void turnOffRelay() {
  digitalWrite(RELAY_PIN, HIGH);
}