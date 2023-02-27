#include "EvtContext.h"

EvtContext::EvtContext()
{
}

void EvtContext::manageListeners(bool manage)
{
    _managesListeners = manage;
}

void EvtContext::loopIteration()
{
    for (byte i = 0; i < _listenerCount; i++)
    {
        if (_listeners[i])
        {
            if (_listeners[i]->isEventTriggered())
            {
                if (_listeners[i]->performTriggerAction(this))
                {
                    return;
                }
            }
        }
    }
}

void EvtContext::reset()
{
    for (byte i = 0; i < _listenerCount; i++)
    {
        if (_listeners[i])
        {
            if (_managesListeners)
            {
                delete _listeners[i];
            }
            _listeners[i] = 0;
        }
    }

    _listenerCount = 0;
}

void EvtContext::addListener(IEvtListener *lstn)
{
    for (byte i = 0; i < _listenerCount; i++)
    {
        if (_listeners[i] == 0)
        {
            lstn->reset();
            _listeners[i] = lstn;
            return;
        }
    }

    lstn->reset();
    _listeners[_listenerCount] = lstn;
    _listenerCount++;
}

void EvtContext::removeListener(IEvtListener *lstn)
{
    for (byte i = 0; i < _listenerCount; i++)
    {
        if (_listeners[i] == lstn)
        {
            if (_managesListeners)
            {
                delete lstn;
            }
            _listeners[i] = 0;

            if (i == _listenerCount - 1)
            {
                _listenerCount--;
            }
        }
    }
}

byte EvtContext::listenerCount()
{
    byte listenerCount = 0;
    
    for (byte i = 0; i < _listenerCount; i++)
    {
        if (_listeners[i])
        {
            listenerCount++;
        }
    }

    return listenerCount;
}