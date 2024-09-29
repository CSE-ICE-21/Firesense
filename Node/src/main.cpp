#include <functions.h>
#include <LoRa.h>
#include <globals.h>
#include <constants.h>

void setup() {
  setupSerial();
  initLora();
  deviceID = generateRandomID();
}

void loop() {
  listenForPackets(LISTENING_INTERVAL);
  sendPacket(deviceID);
}

