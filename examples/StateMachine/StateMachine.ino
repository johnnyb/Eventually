#include "Eventually.h"

#include <Arduino.h>

const byte IDLE = 0;
const byte PENDING = 1;
const byte IN_PROGRESS = 2;

volatile byte state = IDLE;
volatile byte *pState = &state;

void wakeUp()
{
    if (state == IDLE)
    {
        state = PENDING;
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
    state = IN_PROGRESS;
    return true;
}

bool inProgress()
{
    Serial.println("In progress...");
    state = IDLE;
    digitalWrite(13, HIGH);
    delay(500);
    return true;
}

EvtManager mgr;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    pinMode(13, OUTPUT);
    state = IDLE;

    mgr.addListener(new EvtByteListener(pState, IDLE, (EvtAction)idle));
    mgr.addListener(new EvtByteListener(pState, PENDING, (EvtAction)pending));
    mgr.addListener(new EvtByteListener(pState, IN_PROGRESS, (EvtAction)inProgress));

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);

    Serial.println("Setup complete, continuing...");
}

USE_EVENTUALLY_LOOP(mgr)