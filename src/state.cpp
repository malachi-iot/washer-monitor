#include "state.h"
#include "main.h"

State state;

void statusLed_changeState(State s);
void wiggle_set_detector_timeout();
void mqtt_send_log(const char* s);

void state_change(State s)
{
    Serial.print("State changing: ");
    Serial.println(s);

    // global state is changed FIRST so that callbacks/context-specific code
    // (such as statusLed_blink_event)
    // has access to up-to-date state
    state = s;

    statusLed_changeState(s);

    switch(s)
    {
        case State::NotifyingTimeout:
            break;

        case State::ButtonPressing:
            if(state == State::Detecting || state == State::DetectedWiggle)
            {
                // button was pressed in the middle of a cycle
            }
            break;

        case State::ButtonPressed:
            s = State::Detecting;
            state_change(s);
            return;

        case State::Detecting:
            mqtt_send_log("Detection mode begins");
            wiggle_set_detector_timeout();
            break;
    }

    Serial.println("State changed");
}


void state_setup()
{

}


void state_loop()
{

}
