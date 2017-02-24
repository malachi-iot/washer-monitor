#include "main.h"
#include "state.h"

const int LED_PIN = BUILTIN_LED;

void statusLed_setup()
{
    pinMode(LED_PIN, OUTPUT);
}

int ledNotifyTimer = -1;
bool ledOn = false;

inline void statusLed(bool on)
{
    digitalWrite(LED_PIN, on ? LOW : HIGH);
}


inline void toggleLed()
{
    statusLed(ledOn = !ledOn);
}


void statusLed_blink_event()
{
    uint32_t timeout = 0;

    toggleLed();

    switch(state)
    {
        case State::Detecting:
            timeout = ledOn ? 3000 : 500;
            break;

        case State::NotifyingTimeout:
            timeout = 1000;
            break;

        case State::NotifyingManual:
            timeout = 500;
            break;

        default:
            return;
    }

    ledNotifyTimer = timer.setTimeout(timeout, statusLed_blink_event);
}

static void clear_event()
{
    if(ledNotifyTimer != -1)
        timer.deleteTimer(ledNotifyTimer);
}

// statusLed responder to system state change
void statusLed_changeState(State s)
{
    clear_event();

    switch(s)
    {
        case State::Detecting:
        case State::NotifyingManual:
        case State::NotifyingTimeout:
            statusLed_blink_event();
            break;

        case State::ButtonPressing:
            statusLed(true);
            break;

        case State::ButtonPressed:
            statusLed(false);
            break;

        default:
            break;
    }
}


void statusLed_loop()
{

}
