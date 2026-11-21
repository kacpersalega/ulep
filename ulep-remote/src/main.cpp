#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>

// Receiver mac address
uint8_t broadcastAddress[] = {0xac, 0xa7, 0x04, 0x2c, 0x4c, 0xe8};

// Remote mac address
uint8_t remoteAddress[] = {};

// Must match the receiver structure.
typedef struct struct_message {
  bool left_engine;
  bool right_engine;
}

void setup()
{

}
 
void loop()
{

}