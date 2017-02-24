#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "secrets.h"
#include "main.h"

void printWifiStatus();
void printWiggleStatus();

void wiggleDetector_setup();

void ota_setup();
void ota_loop();

void button_setup();
void button_loop();

void mqtt_setup();
void mqtt_loop();

void statusLed_setup();
void statusLed_loop();

void state_setup();
void state_loop();

extern volatile int wigglesDetected;

SimpleTimer timer;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    // setting up Station AP
    WiFi.begin(ssid, pass);

    pinMode(BUILTIN_LED, OUTPUT);

    // Wait for connect to AP
    Serial.print("[Connecting]");
    Serial.print(ssid);
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        tries++;
        if (tries > 30) {
            break;
        }
    }

    printWifiStatus();

    // every second, report wiggle status over serial line
    timer.setInterval(1000, printWiggleStatus);

    wiggleDetector_setup();
    mqtt_setup();
    ota_setup();
    button_setup();
    statusLed_setup();
    state_setup();
}


void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}





void loop()
{
    digitalWrite(BUILTIN_LED, wigglesDetected > 0 ? LOW : HIGH); // ON / OFF

    timer.run();

    mqtt_loop();
    ota_loop();
    button_loop();
}
