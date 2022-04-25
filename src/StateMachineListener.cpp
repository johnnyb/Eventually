#include "StateMachineListener.h"

StateMachineListener::StateMachineListener()
{
}

void StateMachineListener::addState(byte targetState, EvtAction action,
                                    byte successState, byte failureState)
{
    StateAction s;
    s.action = action;
    s.state = targetState;
    s.successState = successState;
    s.failureState = failureState;

    _stateActions[targetState] = s;
}

bool StateMachineListener::performTriggerAction(EvtContext *ctx)
{
    StateAction s = _stateActions[_state];
    bool result = (*s.action)(this, ctx);
    if (result)
    {
        _state = s.successState;
    }
    else
    {
        _state = s.failureState;
    }
    return true;
}

void StateMachineListener::setState(byte state)
{
    _state = state;
}

byte StateMachineListener::currentState()
{
    return _state;
}

void StateMachineListener::setupListener()
{
}