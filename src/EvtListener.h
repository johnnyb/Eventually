/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtListener_h
#define EvtListener_h

#include "EvtAction.h"
#include "IEvtListener.h"

class IEvtContext;

class EvtListener : public IEvtListener
{
public:
    virtual void reset() override;
    virtual bool isEventTriggered() override;
    virtual bool performTriggerAction(IEvtContext *) override;

    virtual void disable() override;
    virtual void enable() override;
    virtual ~EvtListener() override;

protected:
    EvtAction _triggerAction;
    bool _enabled = true;
};

#endif