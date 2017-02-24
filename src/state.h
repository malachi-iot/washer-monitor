#ifndef STATE_H
#define STATE_H

// for our domain, notifing means a) sending an MQTT message and b) flashing the LED
enum State
{
    Idle,
    Detecting,// look for wiggles
    Detected, // when wiggles are detected.  NOTE: reverts straight back to Detecting mode
    NotifyingTimeout, // a certain amount of time has passed with no wiggles
    NotifyingManual,
    NotifiedManual
};


extern State state;

void state_change(State s);


#endif // STATE_H
