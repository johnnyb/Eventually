#include "EvtContextManager.h"

EvtContextManager::EvtContextManager()
{
    pushContext(&_defaultContext);
}

void EvtContextManager::addListener(IEvtListener *lstn)
{
    currentContext()->addListener(lstn);
}

void EvtContextManager::removeListener(IEvtListener *lstn)
{
    currentContext()->removeListener(lstn);
}

IEvtContext *EvtContextManager::currentContext()
{
    return _contextStack[_contextOffset - 1];
}

void EvtContextManager::pushContext(IEvtContext *context)
{
    context->reset();
    _contextStack[_contextOffset] = context;
    _contextOffset++;
}

void EvtContextManager::resetContext()
{
    IEvtContext *current = currentContext();
    current->reset();
}

IEvtContext *EvtContextManager::popContext()
{
    _contextOffset--;
    return currentContext();
}

void EvtContextManager::loopIteration()
{
    currentContext()->loopIteration();
}

void EvtContextManager::reset()
{
    _contextOffset = 1;
    currentContext()->reset();
}

void EvtContextManager::manageListeners(bool manage)
{
    currentContext()->manageListeners(manage);
}

byte EvtContextManager::listenerCount()
{
    return currentContext()->listenerCount();
}