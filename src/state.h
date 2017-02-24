#ifndef STATE_H
#define STATE_H

// for our domain, notifing means a) sending an MQTT message and b) flashing the LED
enum State
{
    Idle,
    Detecting,
    NotifyingTimeout,
    NotifyingManual,
    NotifiedManual
};


extern State state;

void state_change(State s);


#endif // STATE_H
