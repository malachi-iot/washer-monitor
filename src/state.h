#ifndef STATE_H
#define STATE_H

enum State
{
    Idle,
    ButtonPressing,
    ButtonPressed,
    Detecting,
    DetectedWiggle,
    NotifyingTimeout,
    NotifyingManual
};

extern State state;

void state_change(State s);


#endif // STATE_H