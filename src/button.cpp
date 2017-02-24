#include <Arduino.h>
#include <Bounce2.h> // debouncer

#include "main.h"
#include "state.h"
#include "state/button.h"

using namespace FactUtilEmbedded::state;

const int BUTTON_PIN = 0;

Bounce debouncer;

ButtonWithTimer button;

void button_setup()
{
    pinMode(BUTTON_PIN, INPUT);
    debouncer.attach(BUTTON_PIN);
    debouncer.interval(5); // in ms
}

void statusLed(bool on);


void button_loop()
{
    debouncer.update();
    int value = debouncer.read();

    button.update(value == LOW);
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
            if(state != State::NotifyingManual && button.elapsedSinceInitialPress() > 3000)
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
    }
}
