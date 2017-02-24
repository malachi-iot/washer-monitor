#include "state.h"
#include "main.h"

State state;

void statusLed_changeState(State s);
void wiggle_set_detector_timeout();
void mqtt_send_log(const char* s);
void mqtt_send_status(const char* status);

void state_change(State s)
{
    State previousState = state;

    Serial.print("State changing: ");
    Serial.println(s);

    statusLed_changeState(s);

    switch(s)
    {
        case State::NotifyingManual:
            mqtt_send_status("MANUAL ALERT");
            break;

        case State::NotifyingTimeout:
            // get here when enough time has passed without wiggle threshold being exceeded
            mqtt_send_status("DONE");
            break;

        case State::ButtonPressing:
            break;

        case State::ButtonLongPressing:
            state_change(State::NotifyingManual);
            return;

        case State::ButtonPressed:
            /*
            if(previousState == State::Detecting || previousState == State::DetectedWiggle)
            {
                // button was pressed in the middle of a cycle, which means abort
                state_change(State::Idle);
                return;
            }
            else if(previousState == State::NotifyingTimeout || previousState == State::NotifyingManual)
            {
                // button was pressed during alert mode, which means acknowledge
                state_change(State::Idle);
                return;
            } */

            state_change(State::Detecting);
            return;

        case State::Detecting:
            mqtt_send_log("Detection mode begins");
            wiggle_set_detector_timeout();
            break;
    }

    // global state is changed FIRST so that callbacks/context-specific code
    // (such as statusLed_blink_event)
    // has access to up-to-date state
    state = s;

    Serial.println("State changed");
}


void state_setup()
{

}


void state_loop()
{

}
