#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

const int LEFT_PWM_PIN  = D3;
const int LEFT_DIR_PIN  = D4;
const int RIGHT_PWM_PIN = D5;
const int RIGHT_DIR_PIN = D6;

const uint8_t MAX_PWM = 255;
const uint8_t MIN_PWM = 0;

// Must match the sender's struct EXACTLY
typedef struct struct_message {
  bool leftMotor;
  bool rightMotor;
} struct_message;

struct_message receivedData;
unsigned long lastRecvTime = 0;

void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *incomingData, int len) {
  if (len == sizeof(receivedData)) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    lastRecvTime = millis();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LEFT_PWM_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);

  analogWrite(LEFT_PWM_PIN, MIN_PWM);
  analogWrite(RIGHT_PWM_PIN, MIN_PWM);

  digitalWrite(LEFT_DIR_PIN, HIGH);
  digitalWrite(RIGHT_DIR_PIN, HIGH);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (millis() - lastRecvTime > 500) {
    receivedData.leftMotor = false;
    receivedData.rightMotor = false;
  }

  if (receivedData.leftMotor) {
    analogWrite(LEFT_PWM_PIN, MAX_PWM);
  } else {
    analogWrite(LEFT_PWM_PIN, MIN_PWM);
  }

  if (receivedData.rightMotor) {
    analogWrite(RIGHT_PWM_PIN, MAX_PWM);
  } else {
    analogWrite(RIGHT_PWM_PIN, MIN_PWM);
  }
}