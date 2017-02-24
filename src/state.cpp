#include "state.h"
#include "main.h"

State state;

void statusLed_changeState(State s);

void state_change(State s)
{
    Serial.print("State changing: ");
    Serial.println(s);

    statusLed_changeState(s);

    switch(s)
    {
        case State::NotifyingTimeout:
            break;

        case State::ButtonPressing:
            break;

        case State::ButtonPressed:
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
