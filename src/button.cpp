#include <Arduino.h>
#include <Bounce2.h> // debouncer

#include "main.h"
#include "state.h"
#include "button.h"

const int BUTTON_PIN = 0;

Bounce debouncer;

Button::State buttonState;

ButtonWithTimer button;

void button_setup()
{
    pinMode(BUTTON_PIN, INPUT);
    debouncer.attach(BUTTON_PIN);
    debouncer.interval(5); // in ms
}


void Button::loop(bool pressed)
{
    if(isPressed())
    {
        if(state == PRESSED) state = PRESSING;

        if(!pressed)
        {
            state = RELEASED;
        }
    }
    else if(pressed)
    {
        state = PRESSED;
    }
    else // if not pressed, and state was not pressing, then this is a 2nd non-pressed
        // event and resets back to Idle
    {
        state = IDLE;
    }
}

void ButtonWithTimer::loop(bool pressed)
{
    Button::loop(pressed);
    if(getState() == PRESSED)
    {
        buttonInitialPressTimestamp = millis();
    }
}

void statusLed(bool on);


void button_loop()
{
    debouncer.update();
    int value = debouncer.read();

    button.loop(value == LOW);
    switch(button.getState())
    {
        case Button::PRESSED:
            // since normal button release doesn't exit manual mode, we have to check here
            // and clear manual notify state
            if(state == State::NotifyingManual)
                state_change(State::NotifiedManual);

            statusLed(true);

            break;

        case Button::PRESSING:
        {
            if(state != State::NotifyingManual && button.elapsedSinceInitialPress() > 5000)
                state_change(State::NotifyingManual);

            break;
        }

        case Button::RELEASED:
            // Notify manual mode is a special situation where button
            // release doesn't do anything
            if(state != State::NotifyingManual)
            {
                statusLed(false);

                // If idle, then begin our normal detection cycle
                if(state == State::Idle)
                    state_change(State::Detecting);
                // if NOT idle, then we are in the middle/end of something,
                // such as Notify or Detect.  For these situations, go back to Idle
                else
                    state_change(State::Idle);
            }
            break;

        case Button::IDLE:
            break;
    }
}
