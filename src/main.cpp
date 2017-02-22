#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "secrets.h"

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    // setting up Station AP
    WiFi.begin(ssid, pass);

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
}


void printWifiStatus() {
    static int dbgCounter = 0;

    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.print(dbgCounter++);
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}



void loop()
{
    printWifiStatus();
    delay(1000);
}