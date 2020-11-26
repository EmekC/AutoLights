#include <Arduino.h>
// ota updates
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// wifi
#define OTA_SSID "Cohensoon"
#define OTA_TOKEN "0506966260"

void initWiFi();
void handleOTA();

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
#define TIME_OFFSET 60000 // 1 min sec

void setup() {
  Serial.begin(115200);

  initSensor();

  initRelay();

  initWiFi();
}

void loop() {
  handleOTA();
  
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

// OTA STUFF
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(OTA_SSID, OTA_TOKEN);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });

  ArduinoOTA.begin();
  Serial.println("Ready");
}

void handleOTA() {
  ArduinoOTA.handle();
}

