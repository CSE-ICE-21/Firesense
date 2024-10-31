#include <constants.h>
#ifdef GATEWAY
#include <constants.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <constants.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
PubSubClient mqtt(client);

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    SerialMon.print("Message arrived [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    SerialMon.write(payload, len);
    SerialMon.println();

    //   // Only proceed if incoming message's topic matches
    //   if (String(topic) == DATA_RECEPTION_TOPIC) {
    //     //Do whatever wanted
    //   }
}

boolean mqttConnect()
{
    SerialMon.print("Connecting to ");
    SerialMon.print(MQTT_SERVER);

    // Connect to MQTT Broker
    boolean status = mqtt.connect(MQTT_DEVICE_ID);

    if (status == false)
    {
        SerialMon.println(" MQTT Connection failed");
        return false;
    }
    SerialMon.println(" success");
    // mqtt.publish(DATA_SENDING_TOPIC, "Initial testing hazard message");
    mqtt.subscribe(DATA_RECEPTION_TOPIC);
    return mqtt.connected();
}

bool GsmAndMqttSetup()
{
    // Set console baud rate
    SerialMon.begin(115200);
    SerialAT.begin(115200, SERIAL_8N1, 16, 17);
    delay(10);

    SerialMon.println("Wait...");

    modem.setBaud(115200);
    delay(6000);
    // Restart takes quite some time
    SerialMon.println("Initializing modem...");
    modem.restart();
    // modem.init();

    String modemInfo = modem.getModemInfo();
    SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
    // Unlock your SIM card with a PIN if needed
    if (GSM_PIN && modem.getSimStatus() != 3)
    {
        modem.simUnlock(GSM_PIN);
    }
#endif

    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork())
    {
        SerialMon.println(" failed waiting for network");
        delay(10000);
        return false;
    }
    SerialMon.println(" successfully waited for network");

    if (modem.isNetworkConnected())
    {
        SerialMon.println("Network connected succesfully!");
        return true;
    }
    else{
        SerialMon.println("Network connection failed!");
        return false;
    }
}

bool GsmConnect(){
    #if TINY_GSM_USE_GPRS
    // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to :"));
    SerialMon.print(GPRS_APN);
    if (!modem.gprsConnect(GPRS_APN))
    {
        SerialMon.println("GPRS Connection attempt failed!");
        delay(2000);
        return false;
    }
    SerialMon.println("GPRS Connection succesful!");

    if (modem.isGprsConnected())
    {
        SerialMon.println("GPRS connected");
    }
#endif

    // MQTT Broker setup
    mqtt.setServer(MQTT_SERVER, 1883);
    mqtt.setCallback(mqttCallback);
    return true;
}

void sendMqttMessage(String Payload)
{
    if (mqtt.publish(DATA_SENDING_TOPIC, (("Fire Hazard : " + Payload).c_str())),2)
    {
        Serial.println("Message sent to MQTT Broker");
    }
    else
    {
        Serial.println("Message sending failed");
    }
}

void sendSms(String Payload)
{
    String message = "Firesense\nWildfire Hazard!\nWildfire detected on Node : " + Payload + ".";
    if (modem.sendSMS(SMS_NUMBER, message.c_str()))
    {
        Serial.println("SMS sent successfully");
    }
    else
    {
        Serial.println("SMS sending failed");
    }
}
#endif