#include "main.h"
#include "state.h"
#include "state/button.h"

using namespace FactUtilEmbedded::state;

extern ButtonWithTimer button;

//#define STATUS_LED_ON_HIGH

//const int LED_PIN = BUILTIN_LED;
const int LED_PIN = 2;

void statusLed_setup()
{
    pinMode(LED_PIN, OUTPUT);
}

int ledNotifyTimer = -1;
bool ledOn = false;

void statusLed(bool on)
{
#ifdef STATUS_LED_ON_HIGH
    digitalWrite(LED_PIN, on ? HIGH : LOW);
#else
    digitalWrite(LED_PIN, on ? LOW : HIGH);
#endif

#ifdef DEBUG
    Serial.print("Changing led: ");
    Serial.println(on ? "ON" : "OFF");
#endif
}


inline void toggleLed()
{
    statusLed(ledOn = !ledOn);
}


void statusLed_blink_event()
{
    // This logic ensures that a long button press won't get its LED feedback
    // shut off - except for NotifyingManual which is a special case
    // where we want blink to take over while button is still pressed
    if(button.isPressed() && state != State::NotifyingManual)
    {
        // check a little later when button isn't pressed and leave LED alone
        ledNotifyTimer = timer.setTimeout(500, statusLed_blink_event);
        return;
    }

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

// Not well tested, and won't work well anyway because we spend almost no time in "Detected" state
#ifdef DEBUG
        case State::Detected:
            timeout = 20;
            break;
#endif

        case State::Detecting:
            timeout = ledOn ? 5000 : 250;
            break;

        case State::NotifyingTimeout:
            timeout = 1000;
            break;

        case State::NotifyingManual:
            timeout = 250;
            break;

        default:
            // FIX: kludgey, toggle LED back again since we aren't handling an actual event after
            // all
            toggleLed();
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
            // would do this as a direct call, but want some cycles to pass
            // so that global state can get set (since we can't easily manage
            // context/global state with this event)
            timer.setTimeout(10, statusLed_blink_event);
            break;

        default:
            break;
    }
}


void statusLed_loop()
{

}
