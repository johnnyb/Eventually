#ifndef EvtAction_h
#define EvtAction_h

class EvtContext;
class EvtListener;

typedef bool (*EvtAction)(EvtListener *, EvtContext *);

#endif