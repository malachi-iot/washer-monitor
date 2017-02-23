#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SimpleTimer.h>

#include "secrets.h"

const int SENSOR_PIN = 13;

void wiggleDetector();
void printWifiStatus();
void printWiggleStatus();


void mqtt_setup();
void mqtt_loop();

extern volatile int wigglesDetected;

SimpleTimer timer;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    // setting up Station AP
    WiFi.begin(ssid, pass);

    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), wiggleDetector, RISING);

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

    mqtt_setup();
}


void printWifiStatus() {
    static int dbgCounter = 0;

    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.print(dbgCounter++);
    Serial.println(WiFi.SSID());

    //digitalWrite(BUILTIN_LED, dbgCounter % 2 == 0 ? LOW : HIGH);

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}





void loop()
{
    digitalWrite(BUILTIN_LED, wigglesDetected > 0 ? LOW : HIGH); // ON / OFF

    timer.run();

    mqtt_loop();
}