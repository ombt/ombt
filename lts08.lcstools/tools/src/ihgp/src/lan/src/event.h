#ifndef __EVENT_H
#define __EVENT_H
/* event types */
enum Event {
	CallOrigination,
	StationIsIdle,
	CableIsIdle,
	PreambleEnds,
	Jammer,
	MsgEnds,
	BackOffEnds,
	IncomingMsg,
	NoEvent
};
#endif
