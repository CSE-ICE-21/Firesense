#include <globals.h>
#include <functions.h>
#include <constants.h>

void _initialSetup()
{
    uint64_t wakeup_pins = (1ULL << DIO0) | (1ULL << SENSOR_INPUT); // Bitmask for the pins that will trigger the wakeup
    // Enable EXT1 wakeup for both pins, wake up when either pin goes HIGH
    esp_sleep_enable_ext1_wakeup(wakeup_pins, ESP_EXT1_WAKEUP_ANY_HIGH);
    Serial.println("Entering deep sleep...");
    system_state = 1;
    esp_deep_sleep_start();
}

void _respond()
{
    Serial.println("Wake up reason: " + String(esp_sleep_get_wakeup_cause()));

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1)
    {
        uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status(); // Get wake-up status
        if (wakeup_pin_mask != 0)
        { // Ensure the wake-up cause was EXT1
            if (wakeup_pin_mask & (1ULL << SENSOR_INPUT))
            {
                Serial.println("GPIO 2 caused the wakeup.");
                sendPacket("sdeviceID");
                // Go back to deep sleep.
                system_state = 0;
                esp_deep_sleep_start();
            }
            if (wakeup_pin_mask & (1ULL << DIO0))
            {
                Serial.println("GPIO 13 caused the wakeup.");
                listenForPackets(LISTENING_INTERVAL);
            }
        }
    }
}

void runFunction(int state_number)
{
    void (*functionArray[])() = {_initialSetup, _respond};
    functionArray[state_number]();
}
