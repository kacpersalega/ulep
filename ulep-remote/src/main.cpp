#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Receiver MAC address
uint8_t boatMacAddress[] = {0xAC, 0xA7, 0x04, 0x2C, 0x4C, 0xE8};

// Button pins
const int LEFT_BTN_PIN = D2;
const int RIGHT_BTN_PIN = D6;

// Must match the receiver structure
typedef struct struct_message {
  bool leftMotor;
  bool rightMotor;
} struct_message;

struct_message sendData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);

  pinMode(LEFT_BTN_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, boatMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (digitalRead(LEFT_BTN_PIN) == LOW) {
    sendData.leftMotor = true;
  } else {
    sendData.leftMotor = false;
  }

  if (digitalRead(RIGHT_BTN_PIN) == LOW) {
    sendData.rightMotor = true;
  } else {
    sendData.rightMotor = false;
  }

  esp_now_send(boatMacAddress, (uint8_t *) &sendData, sizeof(sendData));
  delay(50);
}