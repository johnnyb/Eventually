#include "Eventually.h"

#include <Arduino.h>

const byte IDLE = 0;
const byte PENDING = 1;
const byte IN_PROGRESS = 2;

void wakeUp()
{
    if (stateMachine.currentState() == IDLE)
    {
        stateMachine.setState(PENDING);
    }
}

bool idle()
{
    Serial.println("Idling...");
    digitalWrite(13, LOW);
    delay(500);
    return true;
}

bool pending()
{
    Serial.println("Pending...");
    return true;
}

bool inProgress()
{
    Serial.println("In progress...");
    digitalWrite(13, HIGH);
    delay(500);
    return true;
}

EvtManager mgr;
StateMachineListener stateMachine;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    pinMode(13, OUTPUT);

    stateMachine.setState(IDLE);

    stateMachine.addState(IDLE, (EvtAction)idle), PENDING);
    stateMachine.addState(PENDING, (EvtAction)pending), IN_PROGRESS);
    stateMachine.addState(IN_PROGRESS, (EvtAction)inProgress), IDLE);

    mgr.addListener(&stateMachine);

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);

    Serial.println("Setup complete, continuing...");
}

USE_EVENTUALLY_LOOP(mgr)