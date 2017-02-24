#include "state.h"
#include "main.h"

State state;

const int LED_PIN = 13;


void state_change(State s)
{
    switch(s)
    {
        case State::NotifyingTimeout:
            break;
    }
}


void state_setup()
{

}


void state_loop()
{

}
