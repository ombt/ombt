#ifndef __CALL_H
#define __CALL_H
/* class for calls */

/* headers */
#include "returns.h"
#include "item.h"
#include "event.h"
#include "station.h"

/* declare class */
class Call;

/* states for work station  */
enum CallState {
	StartCall,
	QueueForStation,
	StartTransmission,
	QueueForCable,
	StartPreamble,
	Retry,
	BackOff,
	StartMsg,
	CallDies
};

/* class for a call */
class Call: public Item {
	friend int process(Call *);

public:
	/* constructor and destructor */
	Call();
	Call(StationSender *);
	~Call();

	/* initialization */
	int init(StationSender * = 0);
	int deleteAll();

	/* other functions */
	inline int getStatus() { return(status); }

private:
	/* do not allow these */
	Call(const Call &);
	Call &operator=(const Call &);

public:
	/* internal data */
	int status;

	/* call state */
	CallState state; 

	/* next event type */
	Event event;

	/* back off counter */
	int backOff;

	/* sender station */
	StationSender *station;

	/* start times */
	long queuedForStationStart;
	long queuedForCableStart;
	long transmissionStart;
	long backOffStart;
	long callStart;
};
#endif

