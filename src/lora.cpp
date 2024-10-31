#include <LoRa.h>
#include <functions.h>
#include <globals.h>
#include <constants.h>

// Function to listen for incoming LoRa packets.
String listenForPackets(unsigned long lowerbound, unsigned long upperbound)
{
    unsigned long startTime = millis(); // Record the start time
    while (millis() - startTime < random(lowerbound, upperbound))
    { // Run for loopDuration milliseconds
        int packetSize = LoRa.parsePacket();
        if (packetSize > 0)
        {
            // Received a packet
            Serial.print("Received packet: ");
            String LoRaData;
            while (LoRa.available())
            {
                LoRaData = LoRa.readString(); // Read the packet data as a string
                Serial.println(LoRaData);
            }
            if (validateID(LoRaData))
            {
                return LoRaData;
            }
#ifndef GATEWAY
            else if (LoRaData == "IMMEDIATE_RESET")
            {
                systemReset(RESETINTERVAL); // Reset the system immediately
            }
#endif
        }
    }
    Serial.println("No any valid packet received");
    return "NULL";
}

#ifdef GATEWAY
void sendSystemMessage(String message)
{
    while (true)
    {
        LoRa.beginPacket();

        LoRa.print(message); // Send the device ID as the packet content
        Serial.println("Sending packet: " + message);

        if (LoRa.endPacket() == 1)
        {
            Serial.println("Packet sent successfully");
            String reply = listenForPackets(LISTENING_INT_LOW, LISTENING_INT_HIGH);
            if (reply == "NULL")
            {
                Serial.println("All neighbours notified to reset.");
                break;
            }
        }
        else
        {
            Serial.println("Packet failed to send");
            delay(WAITBEFORERESEND * random(1000, 1500)); // Wait before resending
        }
    }
}
#else
void sendPacket(String message)
{
    while (true)
    {
        LoRa.beginPacket();

        LoRa.print(message); // Send the device ID as the packet content
        Serial.println("Sending packet: " + message);

        if (LoRa.endPacket() == 1)
        {
            Serial.println("Packet sent successfully");
            String reply = listenForPackets(LISTENING_INT_LOW, LISTENING_INT_HIGH);
            if (validateID(reply) && message == reply) //  "NULL" return will be invalid.
            {
                Serial.println(reply + "- Recieved Acknowledgement");
                return;
            }
            else if (validateID(reply) && message != reply && random(0, 2)) // This randomisation is to avoid the livelock situation occur when conflicting nodes which was sending two different messages(which is already a livelock) both switch to each others message to acknowledge each other.
            {
                Serial.println(reply + "- Sent an Acknowledgement for another message!");
                sendPacket(reply);
            }
        }
        else
        {
            Serial.println("Packet failed to send");
            delay(WAITBEFORERESEND * random(1000, 1500)); // Wait before resending
        }
    }
}
#endif