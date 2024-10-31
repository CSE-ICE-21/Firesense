#include <functions.h>
#include <constants.h>
#include <LoRa.h>
#include <Preferences.h>
#include <nvs_flash.h> // Include the NVS flash library for manual NVS initialization

void setupSerial()
{ // Function to setup serial communication
  Serial.begin(115200);
  while (!Serial)
    ;
}

void sensorSetup()
{
  pinMode(SENSOR_INPUT, INPUT);  // Sensor Input
  pinMode(SENSORSWITCH, OUTPUT); // Sensor Switch
}

void initLora()
{
  LoRa.setPins(CS, -1, DIO0); // Reset pin is not used. Connect LoRa module Reset pin to the MCU Reset pin.
  // replace the LoRa.begin(---E-) argument with your location's frequency
  // 433E6 for Asia
  while (!LoRa.begin(433E6))
  {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initialization Completed!");
}

// Function to check Preferences for deviceID and generate if not found
String getDeviceID()
{
  // Initialize the NVS (Non-Volatile Storage) system (Usually this should be handled by the preferences.h, but there was a error and hence done manually.)
  if (nvs_flash_init() == ESP_ERR_NVS_NO_FREE_PAGES || nvs_flash_init() == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // If NVS partition was truncated or a new version was found, erase it and try again
    nvs_flash_erase();
    nvs_flash_init();
  }
  Preferences preferences;                 // Create a Preferences object
  preferences.begin("device_info", false); // Start preferences in read-write mode
  String device_Identifier = preferences.getString("deviceID", "NULL");

  if (device_Identifier == "NULL")
  {
    device_Identifier = generateRandomID();
    preferences.putString("deviceID", device_Identifier);
    ;
  }

  preferences.end(); // Close preferences
  return device_Identifier;
}

// Generation of a random 6-character ID (alphanumeric: 0-9, A-F) and verification of it.
String generateRandomID()
{
  String id = "";

  // Generate first 4 characters (random alphanumeric: 0-9, A-F)
  for (int i = 0; i < 4; i++)
  {
    if (random(0, 2) == 0)
    {
      id += (char)random('0', '9' + 1); // Random digit
    }
    else
    {
      id += (char)random('A', 'F' + 1); // Random uppercase letter (A-F)
    }
  }

  // Calculate checksum for verification (using XOR and modulo)
  char checksum = 0;
  for (int i = 0; i < 4; i++)
  {
    checksum ^= id[i]; // XOR of ASCII values
  }

  // Convert checksum into 2 hex characters (e.g., 0-9, A-F)
  char checksum1 = 'A' + (checksum % 6);  // Convert to 'A'-'F'
  char checksum2 = '0' + (checksum % 10); // Convert to '0'-'9'

  // Append checksum to the ID
  id += checksum1;
  id += checksum2;

  return id;
}

bool validateID(String id)
{
  // Check if ID is exactly 6 characters
  if (id.length() != 6)
  {
    return false;
  }

  // Check first 4 characters are alphanumeric (0-9, A-F)
  for (int i = 0; i < 4; i++)
  {
    if (!(isDigit(id[i]) || (id[i] >= 'A' && id[i] <= 'F')))
    {
      return false;
    }
  }

  // Recalculate checksum of the first 4 characters
  char checksum = 0;
  for (int i = 0; i < 4; i++)
  {
    checksum ^= id[i]; // XOR of ASCII values
  }

  // Validate the last 2 characters against the calculated checksum
  char checksum1 = 'A' + (checksum % 6);  // Convert to 'A'-'F'
  char checksum2 = '0' + (checksum % 10); // Convert to '0'-'9'

  // Check if the last two characters match the calculated checksum
  if (id[4] == checksum1 && id[5] == checksum2)
  {
    return true; // ID is valid
  }

  return false; // ID is invalid
}

// Read the digital sensor with debouncing
bool readDigitalSensor(int pin)
{
  const unsigned long debounceDelay = 50;    // 50 ms debounce time
  unsigned long lastDebounceTime = millis(); // Start the debounce timer
  int lastStableState = digitalRead(pin);    // Get the initial state

  while (millis() - lastDebounceTime < debounceDelay) // Wait for the debounce time
  {
    int currentState = digitalRead(pin); // Read the current state
    if (currentState != lastStableState) // If the state changes, reset the timer
    {
      lastDebounceTime = millis();
      lastStableState = currentState;
    }
  }

  // Return the stable state after debounce delay
  return lastStableState;
}

// Read the analog sensor with averaging and oversampling
// int readAnalogSensor(int pin, int numSamples = 10) {
//     long total = 0;

//     // Take multiple samples and average them
//     for (int i = 0; i < numSamples; i++) {
//         total += analogRead(pin);
//         delay(2); // Small delay to stabilize readings
//     }

//     return total / numSamples;  // Return the average reading
// }