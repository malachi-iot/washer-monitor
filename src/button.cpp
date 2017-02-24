#include <Arduino.h>
#include <Bounce2.h> // debouncer

#include "main.h"
#include "state.h"

const int BUTTON_PIN = 0;

Bounce debouncer;


void button_setup()
{
    pinMode(BUTTON_PIN, INPUT);
    debouncer.attach(BUTTON_PIN);
    debouncer.interval(5); // in ms
}


void button_loop()
{
    debouncer.update();
    int value = debouncer.read();

    //delay(500);
    //Serial.printf("Debouncer value = %d\r\n", value);

    if(state == State::ButtonPressing)
    {
        if(value == HIGH)
            state_change(State::ButtonPressed);
    }
    else if(value == LOW)
    {
        state_change(State::ButtonPressing);
    }
}
