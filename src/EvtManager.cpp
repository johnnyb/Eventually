#include "EvtManager.h"

EvtManager::EvtManager()
{
    contextStack = new EvtContext[EVENTUALLY_MAX_CONTEXTS];
    contextStack[contextOffset].setupContext();
}

void EvtManager::addListener(EvtListener *lstn)
{
    contextStack[contextOffset].addListener(lstn);
}

void EvtManager::removeListener(EvtListener *lstn)
{
    contextStack[contextOffset].removeListener(lstn);
}

EvtContext *EvtManager::currentContext()
{
    return &contextStack[contextOffset];
}

EvtContext *EvtManager::pushContext()
{
    contextOffset++;
    contextStack[contextOffset].setupContext();
    return &contextStack[contextOffset];
}

EvtContext *EvtManager::resetContext()
{
    contextStack[contextOffset].setupContext();
    return &contextStack[contextOffset];
}

EvtContext *EvtManager::popContext()
{
    contextOffset--;
    return &contextStack[contextOffset];
}

void EvtManager::loopIteration()
{
    contextStack[contextOffset].loopIteration();
}