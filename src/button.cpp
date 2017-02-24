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


// ms we started button press, for long press detection
// (timer class is neat but overkill for this one)
uint32_t buttonPressStart;

void button_loop()
{
    debouncer.update();
    int value = debouncer.read();

    if(state == State::ButtonPressing || state == State::ButtonLongPressing)
    {
        if(value == HIGH) // button released if value == HIGH
            state_change(State::ButtonPressed);
        else if(state != State::ButtonLongPressing && millis() > buttonPressStart + 5000) // check for 5s timeout
            state_change(State::ButtonLongPressing);
    }
    else if(value == LOW)
    {
        buttonPressStart = millis();
        state_change(State::ButtonPressing);
    }
}
