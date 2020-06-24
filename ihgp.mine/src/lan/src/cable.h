#ifndef __CABLE_H
#define __CABLE_H
/* class for LAB cable */

/* headers */
#include "returns.h"
#include "poisson.h"
#include "item.h"
#include "event.h"

/* declare class */
class Cable;

/* cable states */
enum CableState { Idle, Preamble, Jammed, Transmitting };

/* local definitions */
#define MaxNumberOfStations 512

/* class for a LAN cable */
class Cable {
public:
	/* constructors and destructor */
	Cable();
	Cable(double, long);
	~Cable();

	/* queue and dequeue for cable */
	inline Item *enqueue(Item *item) { 
		totalCallsQueued++;
		return(queue.enqueue(item));
	}
	inline Item *dequeue() { 
		return(queue.dequeue());
	}
	inline int isEmpty() {
		return(queue.isEmpty());
	}

	/* sending and receiving msgs */
	int sendMessage(char *, int);
	int receiveMessage(char *, int &);
	void deleteMessage();

	/* other functions */
	int init(double = 0.0, long = 1);
	int deleteAll();
	int errorsInMessage();
	inline int getStatus() { return(status); }
	inline int isIdle() { return(state == Idle); }

private:
	/* do not allow these */
	Cable(const Cable &);
	Cable &operator=(const Cable &);

public:
	/* internal data */
	int status;

	/* cable FSM state */
	CableState state;

	/* next event type */
	Event event;

	/* size of message and buffer */
	int messageSize;
	char *message;

	/* rate of random errors in line */
	double meanErrorRate;

	/* random error distribution */
	Poisson errorDist;

	/* transmission delay in bits/microsecond */
	long bpus;

	/* processes waiting for an idle cable */
	Queue queue;

	/* measurements */
	long totalCallsTransmitted;
	long totalCallsCorrupted;
	long totalCallsQueued;
	long totalBitsTransmitted;
};
#endif
