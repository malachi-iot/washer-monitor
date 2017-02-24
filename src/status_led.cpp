#include "main.h"
#include "state.h"

//const int LED_PIN = BUILTIN_LED;
const int LED_PIN = 2;

void statusLed_setup()
{
    pinMode(LED_PIN, OUTPUT);
}

int ledNotifyTimer = -1;
bool ledOn = false;

inline void statusLed(bool on)
{
    digitalWrite(LED_PIN, on ? LOW : HIGH);
#ifdef DEBUG
    Serial.print("Changing led: ");
    Serial.println(on ? "ON" : "OFF");
#endif
}


inline void toggleLed()
{
    statusLed(ledOn = !ledOn);
}

// a bit of a kludge since SimpleTimer can't pass contexts around
// UNUSED at this time
struct
{
    State state;
} statusLed_blink_event_context;

void statusLed_blink_event()
{
    //State state = statusLed_blink_event_context.state;

    uint32_t timeout = 0;

#ifdef DEBUG
    Serial.print("Blink event: ");
    Serial.println(state);
#endif

    toggleLed();

    switch(state)
    {
        case State::Idle:
            timeout = ledOn ? 50 : 10000;
            break;

        case State::Detecting:
            timeout = ledOn ? 5000 : 250;
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

#ifdef DEBUG
    Serial.print("Blink timer event scheduled: ");
    Serial.println(ledNotifyTimer);
#endif
}

static void clear_event()
{
    if(ledNotifyTimer != -1)
    {
#ifdef DEBUG
        Serial.print("Removing timer: ");
        Serial.println(ledNotifyTimer);
#endif

        timer.deleteTimer(ledNotifyTimer);
    }
}

// statusLed responder to system state change
void statusLed_changeState(State s)
{
    clear_event();

    switch(s)
    {
        case State::Idle:
        case State::Detecting:
        case State::NotifyingManual:
        case State::NotifyingTimeout:
            //statusLed_blink_event_context.state = s;
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
