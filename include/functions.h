#include <Arduino.h>
#include <constants.h>
//utils
void setupSerial();
void initLora();
String generateRandomID();
bool validateID(String id);
void runFunction(int state_number);
void sensorSetup();
String getDeviceID();

// int readAnalogSensor(int pin, int numSamples = 10);
bool readDigitalSensor(int pin);

// lora
String listenForPackets(unsigned long lowerbound, unsigned long upperbound);
#ifdef GATEWAY
void sendSystemMessage(String message);
#else
void sendPacket( String message);

// Program
void systemReset(uint64_t resetInterval); // Reset the system after a certain time.
#endif

#ifdef GATEWAY
//gsm and mqtt
void GsmAndMqttSetup();
void GsmConnect();
boolean mqttConnect();
void sendMqttMessage(String Payload);
void sendSms(String Payload);
#endif