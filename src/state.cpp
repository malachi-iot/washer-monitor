#include "state.h"
#include "main.h"

State state;

void statusLed_changeState(State s);
void wiggle_set_detector_timeout();
void mqtt_send_log(const char* s);
void mqtt_send_status(const char* status);

void state_change(State s)
{
#ifdef DEBUG
    Serial.print("State changing from: ");
    Serial.println(s);
#endif

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

        case State::Detected:
            // revert right back again, we don't want to restart detecting process
            s = State::Detecting;
            break;

        case State::Detecting:
            mqtt_send_log("Detection mode begins");
            wiggle_set_detector_timeout();
            break;
    }

    // global state is changed FIRST so that callbacks/context-specific code
    // (such as statusLed_blink_event)
    // has access to up-to-date state
    state = s;

    Serial.print("State changed: ");
    Serial.println(s);
}


void state_setup()
{
    state_change(State::Idle);
}


void state_loop()
{

}
