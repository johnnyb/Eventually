/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtAction_h
#define EvtAction_h

class EvtContext;
class EvtListener;

typedef bool (*EvtAction)(EvtListener *, EvtContext *);

#endif