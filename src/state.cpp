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
            if(state == State::Detecting || state == State::DetectedWiggle)
            {
                // button was pressed in the middle of a cycle
            }
            break;

        case State::ButtonPressed:
            //s = State::Detecting;
            //statusLed_changeState(s);
            break;
    }

    state = s;

    Serial.println("State changed");
}


void state_setup()
{

}


void state_loop()
{

}
