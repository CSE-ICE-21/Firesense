#include <Arduino.h>
#include <functions.h>
#include <constants.h>
#include <LoRa.h>


void setupSerial() { // Function to setup serial communication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
}

void initLora(){
      //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initialization Completed!");
}



// Generation of a random 6-character ID (alphanumeric: 0-9, A-F) and verification of it.
String generateRandomID() {
  String id = "";
  
  // Generate first 4 characters (random alphanumeric: 0-9, A-F)
  for (int i = 0; i < 4; i++) {
    if (random(0, 2) == 0) {
      id += (char)random('0', '9' + 1);  // Random digit
    } else {
      id += (char)random('A', 'F' + 1);  // Random uppercase letter (A-F)
    }
  }

  // Calculate checksum for verification (using XOR and modulo to keep it simple)
  char checksum = 0;
  for (int i = 0; i < 4; i++) {
    checksum ^= id[i];  // XOR of ASCII values
  }

  // Convert checksum into 2 hex characters (e.g., 0-9, A-F)
  char checksum1 = 'A' + (checksum % 6);  // Convert to 'A'-'F'
  char checksum2 = '0' + (checksum % 10); // Convert to '0'-'9'

  // Append checksum to the ID
  id += checksum1;
  id += checksum2;

  return id;
}

bool validateID(String id) {
  // Check if ID is exactly 6 characters
  if (id.length() != 6) {
    return false;
  }

  // Check first 4 characters are alphanumeric (0-9, A-F)
  for (int i = 0; i < 4; i++) {
    if (!(isDigit(id[i]) || (id[i] >= 'A' && id[i] <= 'F'))) {
      return false;
    }
  }

  // Recalculate checksum of the first 4 characters
  char checksum = 0;
  for (int i = 0; i < 4; i++) {
    checksum ^= id[i];  // XOR of ASCII values
  }

  // Validate the last 2 characters against the calculated checksum
  char checksum1 = 'A' + (checksum % 6);  // Convert to 'A'-'F'
  char checksum2 = '0' + (checksum % 10); // Convert to '0'-'9'

  // Check if the last two characters match the calculated checksum
  if (id[4] == checksum1 && id[5] == checksum2) {
    return true;  // ID is valid
  }

  return false;  // ID is invalid
}



//deep sleep function
void deepSleepESP() {
  Serial.println("Going to sleep");
 uint64_t sleepTime = SLEEP_AFTER_ACK * 60 * 1000000; // 10 minutes
  esp_sleep_enable_timer_wakeup(sleepTime);
  esp_deep_sleep_start();
}