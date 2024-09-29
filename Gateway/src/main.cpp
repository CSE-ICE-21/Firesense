#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 15
#define dio0 2

int counter = 10000;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  Serial.println("LoRa Initialization Completed!");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  // LoRa.print("C = "); // Any prefix to the counter value.
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

delay(random(1000, 2000));;
}