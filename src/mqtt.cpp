#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include "main.h"

WiFiClient net;
MQTTClient client;

uint32_t getTotalWigglesInLast60Seconds();

void mqtt_setup()
{
    // Wow, only QoS 0 supported.... weak!!
    // At least there's a good reason for this:
    // https://github.com/256dpi/arduino-mqtt/issues/49
    client.begin("192.168.2.11", net);
}


void mqtt_send_log(const char* msg)
{
    client.publish("marguerita/washer-monitor/log", msg);
}


void mqtt_send_status(const char* status)
{
    client.publish("marguerita/washer-monitor/status", status);
}


void mqtt_send_event()
{
    static uint32_t counter = 0;
    char buf[64];

    sprintf(buf, "Wiggles [last 60s]: %lu", getTotalWigglesInLast60Seconds());

    if(client.connected())
    {
        client.publish("hello", buf);

        // reschedule our next call
        timer.setTimeout(5000, mqtt_send_event);
    }
}

bool mqtt_attempting_connection = false;

void connect_event()
{
    Serial.println("MQTT: Connecting...");

    if(!client.connect("esp8266"))
    {
        mqtt_attempting_connection = true;
        // try again in one second
        timer.setTimeout(1000, connect_event);
    }
    else
    {
        Serial.println("MQTT: Connected");

        mqtt_attempting_connection = false;
        // got a connection, kick off test send event
        mqtt_send_log("Starting up.  Compiled @ " __DATE__ ", " __TIME__);
        mqtt_send_event();
    }
}


void mqtt_loop()
{
    client.loop();
    // kick off connect event when
    // a) we have no connection AND
    // b) we aren't already attempting to connect
    if(!client.connected() && !mqtt_attempting_connection)
        connect_event();
}


void messageReceived(String topic, String payload, char * bytes, unsigned int length)
{
    Serial.print("incoming: ");
    Serial.print(topic);
    Serial.print(" - ");
    Serial.print(payload);
    Serial.println();
}
