#include "Eventually.h"

#include <Arduino.h>

const byte IDLE = 0;
const byte PENDING = 1;
const byte IN_PROGRESS = 2;

byte state = IDLE;
byte *pState = &state;

bool idle()
{
    Serial.println("Idling...");
    *pState = PENDING;
    digitalWrite(13, LOW);
    delay(500);
    return true;
}

bool pending()
{
    Serial.println("Pending...");
    *pState = IN_PROGRESS;
    return true;
}

bool inProgress()
{
    Serial.println("In progress...");
    *pState = IDLE;
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
    *pState = IDLE;

    mgr.addListener(new EvtByteListener(pState, IDLE, ALWAYS, (EvtAction)idle));
    mgr.addListener(new EvtByteListener(pState, PENDING, ALWAYS, (EvtAction)pending));
    mgr.addListener(new EvtByteListener(pState, IN_PROGRESS, ALWAYS, (EvtAction)inProgress));

    Serial.println("Setup complete, continuing...");
}

void loop()
{
    Serial.print(".");
    mgr.loopIteration();
}