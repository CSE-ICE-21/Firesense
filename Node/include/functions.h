#include <Arduino.h>

//utils
void setupSerial();
void initLora();
String generateRandomID();
bool validateID(String id);
void runFunction(int state_number);
void sensorSetup();

// lora
void listenForPackets(unsigned long loopDuration);
void sendPacket( String message);