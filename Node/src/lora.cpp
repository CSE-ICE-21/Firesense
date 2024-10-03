#include <LoRa.h>
#include <functions.h>
#include <globals.h>

String deviceID;

void listenForPackets(unsigned long loopDuration)
{
    unsigned long startTime = millis(); // Record the start time
    while (millis() - startTime < loopDuration)
    { // Run for loopDuration milliseconds
        int packetSize = LoRa.parsePacket();
        if (packetSize > 0)
        {
            // Received a packet
            Serial.print("Received packet: ");

            while (LoRa.available())
            {
                String LoRaData = LoRa.readString(); // Read the packet data as a string
                Serial.print(LoRaData + " - ");

                if (validateID(LoRaData))
                {
                    if (deviceID == LoRaData)
                    {
                        Serial.println("Recieved Acknowledgement");
                        esp_deep_sleep_start();
                    }
                    else
                    {
                        Serial.println("Verified");
                        sendPacket(LoRaData);
                        Serial.println("ReBroadcasted - " + LoRaData);
                    }
                }
                else
                {
                    Serial.println("Invalid");
                }
            }
        }
    }
}

void sendPacket(String message)
{
    LoRa.beginPacket();

    LoRa.print(message); // Send the device ID as the packet content
    Serial.println("Sending packet: " + message);

    if (LoRa.endPacket() == 1)
    {
        Serial.println("Packet sent successfully");
    }
    else
    {
        Serial.println("Packet failed to send");
    }
}
