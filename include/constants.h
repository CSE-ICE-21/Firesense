//Uncomment below line if it is the gateway.Otherwise keep it commented out.
// #define GATEWAY

//Lora transceiver module and sensor pins
#define CS 5 
#define DIO0 GPIO_NUM_2 // Dont define just with numbers, use GPIO_NUM_X
#define SENSOR_INPUT GPIO_NUM_4 // Dont define just with numbers, use GPIO_NUM_X
#define SENSORSWITCH 32 // Switching the Sensor on and off.

//general parameters
#ifdef GATEWAY
#define LISTENING_INT_LOW 5000 // in milliseconds
#define LISTENING_INT_HIGH 6000 // in milliseconds
#else
#define LISTENING_INT_LOW 2000 // in milliseconds
#define LISTENING_INT_HIGH 5000 // in milliseconds
#endif
#define TEMP_CHECK_INTERVAL 0.5 // in minutes
#define WAITBEFORERESEND 10 // in seconds
#define RESETINTERVAL 15 // in minutes

#ifdef GATEWAY
//gsm
#define GPRS_APN "dialogbb" // Access Point Name
#define SMS_NUMBER "+13137621438" // Phone number to send SMS

//mqtt
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_DEVICE_ID "firesense_hazards"
#define DATA_RECEPTION_TOPIC "firesense_hazards/reciever"
#define DATA_SENDING_TOPIC "firesense_hazards/sender"

#define TINY_GSM_MODEM_SIM900 // Modem is SIM900
#define SerialMon Serial // Define Serial monitor
#define SerialAT Serial2 // Define Serial for AT commands
// #define DUMP_AT_COMMANDS // Dump AT commands to Serial for debugging.
#define TINY_GSM_DEBUG SerialMon // Debug stream
#define GSM_PIN "" // SIM card PIN (leave empty if not defined)
#define TINY_GSM_USE_GPRS true // Use GPRS for internet connection
#define MAX_GSM_CONNECTION_REATTEMPTS 3 // Maximum number of reattempts to connect to GSM network. then will send another sms saying the system is down.
#define MAX_MQTT_CONNECTION_REATTEMPTS 3 // Maximum number of reattempts to connect to MQTT server. then will send another sms saying the system is down.
#endif