/* definitions for LAN station */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

/* other headers */
#include "returns.h"
#include "debug.h"
#include "station.h"

/* static symbols */
StationList::StationSenderList StationList::senders[MaxNumberOfStations];
StationList::StationReceiverList StationList::receivers[MaxNumberOfStations];
int StationList::numberOfSenders = 0;
int StationList::numberOfReceivers = 0;

/* constructors and destructor */
StationSender::StationSender(): Item(SenderType)
{
	init();
}

StationSender::StationSender(double rate, char *addr, 
	int minmsgsz, int maxmsgsz, int cwqsize, int maxbackoff): 
	Item(SenderType)
{
	init(rate, addr, minmsgsz, maxmsgsz, cwqsize, maxbackoff);
}

StationSender::~StationSender()
{
	deleteAll();
}

/* initialization */
int
StationSender::init()
{
	/* default sender FSM state */
	state = StationIdle;

	/* next event */
	event = NoEvent;

	/* default station address */
	strcpy(address, UnknownStation);

	/* rate of calls per second */
	callRate = 1.0;

	/* exponential arrival distribution */
	interArrivalDist.init(1.0/callRate);

	/* backoff maximum */
	maxBackOff = DefaultMaxBackOff;

	/* uniform message size distribution */
	msgSizeDist.init(MinimumMsgSize, MaximumMsgSize);

	/* call waiting queue */
	maxCallsInQueue = DefaultCallWaitingQueueSize;
	callsInQueue = 0;
	queue.init();

	/* call measurements */
	callsOriginated = 0;
	callsQueuedForStation = 0;
	callsQueuedForCable = 0;
	callsFailedTransmission = 0;
	callsQueueIsFull = 0;
	callsCollided = 0;
	callsInitiallyBlocked = 0;

	/* total times */
	totalTimeInCallWaitingQueue = 0;
	totalTimeInCableQueue = 0;
	totalCallDuration = 0;
	totalBackOffTime = 0;
	totalTransmissionTime = 0;

	/* all done */
	status = OK;
	return(OK);
}

int
StationSender::init(double rate, char *addr, int minmsgsz, int maxmsgsz, 
	int cwqsize, int maxbackoff)
{
	/* default sender FSM state */
	state = StationIdle;

	/* next event */
	event = NoEvent;

	/* default station address */
	strcpy(address, addr);

	/* rate of calls per second */
	callRate = rate;

	/* exponential arrival distribution */
	interArrivalDist.init(1.0/rate);

	/* backoff maximum */
	maxBackOff = maxbackoff;

	/* uniform message size distribution */
	msgSizeDist.init(minmsgsz, maxmsgsz);

	/* call waiting queue */
	maxCallsInQueue = cwqsize;
	callsInQueue = 0;
	queue.init();

	/* call measurements */
	callsOriginated = 0;
	callsQueuedForStation = 0;
	callsQueuedForCable = 0;
	callsFailedTransmission = 0;
	callsQueueIsFull = 0;
	callsCollided = 0;
	callsInitiallyBlocked = 0;

	/* total queue times */
	totalTimeInCallWaitingQueue = 0;
	totalTimeInCableQueue = 0;
	totalCallDuration = 0;
	totalBackOffTime = 0;
	totalTransmissionTime = 0;

	/* register station address */
	if (strcmp(address, UnknownStation) != 0)
	{
		status = StationList::registerSender(address, this);
		if (status != OK)
		{
			ERROR("failed to register sender.", status);
			return(NOTOK);
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
StationSender::deleteAll()
{
	/* unregister station address with LAN */
	if (strcmp(address, UnknownStation) != 0)
	{
		status = StationList::unregisterSender(address);
		if (status != OK)
		{
			ERROR("failed to unregister sender.", status);
			return(NOTOK);
		}
	}
	queue.deleteAll();
	status = OK;
	return(NOTOK);
}

/* generate a call load, get inter-arrival time */
long
StationSender::nextCall()
{
	return((long)interArrivalDist.randomSample());
}

/* generate backoff duration */
long
StationSender::backOffTime(int backOff)
{
	/* get range of times */
	double max;
	if (backOff < MaxBackOffInterval)
		max = pow(2.0, (double)backOff)-1.0;
	else
		max = pow(2.0, (double)MaxBackOffInterval)-1.0;
	Uniform dist(0.0, max);
	if ((status = dist.getStatus()) != OK)
	{
		ERROR("backoff uniform constructor failed.", status);
		return(-1);
	}

	/* return with backoff time */
	status = OK;
	return((long)(dist.randomSample()*SlotTime));
}

/* mantain call waiting queue for station */
Item *
StationSender::enqueue(Item *item)
{
	if (isFull())
	{
		status = EINVAL;
		ERROR("call waiting queue is full.", status);
		return((Item *)0);
	}
	callsInQueue++;
	return(queue.enqueue(item));
}

Item *
StationSender::dequeue()
{
	if (isEmpty())
	{
		status = EINVAL;
		ERROR("call waiting queue is empty.", status);
		return((Item *)0);
	}
	callsInQueue--;
	return(queue.dequeue());
}

int 
StationSender::isEmpty()
{
	return(queue.isEmpty());
}

int 
StationSender::isFull()
{
	return(callsInQueue >= maxCallsInQueue);
}

/* constructors and destructor */
StationReceiver::StationReceiver(): Item(ReceiverType)
{
	init();
}

StationReceiver::StationReceiver(char *addr): Item(ReceiverType)
{
	init(addr);
}

StationReceiver::~StationReceiver()
{
	deleteAll();
}

/* initialization */
int
StationReceiver::init(char *addr)
{
	/* default receiver FSM state */
	state = StationIdle;

	/* next event */
	event = NoEvent;

	/* default station address */
	strcpy(address, addr);

	/* call measurements */
	callsTerminated = 0;
	callsCorrupted = 0;

	/* register station address */
	if (strcmp(address, UnknownStation) != 0)
	{
		status = StationList::registerReceiver(address, this);
		if (status != OK)
		{
			ERROR("failed to register receiver.", status);
			return(NOTOK);
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
StationReceiver::deleteAll()
{
	/* unregister station address */
	if (strcmp(address, UnknownStation) != 0)
	{
		status = StationList::unregisterReceiver(address);
		if (status != OK)
		{
			ERROR("failed to unregister receiver.", status);
			return(NOTOK);
		}
	}
	status = OK;
	return(NOTOK);
}

/* initialize station lists */
void
StationList::initLists()
{
	/* list of sending stations */
	numberOfSenders = 0;
	memset((void *)senders, 0, 
		MaxNumberOfStations*sizeof(StationSender *));

	/* list of receiving stations */
	numberOfReceivers = 0;
	memset((void *)receivers, 0, 
		MaxNumberOfStations*sizeof(StationReceiver *));

	/* all done */
	return;
}

/* register a station sender */
int
StationList::registerSender(char *addr, StationSender *station)
{
	/* check parameters */
	if ((addr == (char *)0) || (station == (StationSender *)0))
	{
		ERROR("null address or station.", EINVAL);
		return(NOTOK);
	}

	/* check if list is full */
	if (numberOfSenders >= MaxNumberOfStations)
	{
		ERROR("too many senders.", EINVAL);
		return(NOTOK);
	}

	/* store new sender */
	strcpy(senders[numberOfSenders].address, addr);
	senders[numberOfSenders].station = station;
	numberOfSenders++;

	/* all done */
	return(OK);
}

/* unregister a station sender */
int
StationList::unregisterSender(char *addr)
{
	/* check parameter */
	if (addr == (char *)0)
	{
		ERROR("null address.", EINVAL);
		return(NOTOK);
	}

	/* find station and remove it from list */
	int istation;
	for (istation = 0; istation < numberOfSenders; istation++)
	{
		if (strcmp(senders[istation].address, addr) == 0) break;
	}

	/* check if station was found */
	if (istation >= numberOfSenders)
	{
		/* station was not found */
		ERRORS("station not found.", addr, EINVAL);
		return(NOTOK);
	}

	/* delete station, and move other stations down */
	for (--numberOfSenders; istation < numberOfSenders; istation++)
	{
		strcpy(senders[istation].address, senders[istation+1].address);
		senders[istation].station =  senders[istation+1].station;
	}

	/* all done */
	return(OK);
}

/* register a station receiver */
int
StationList::registerReceiver(char *addr, StationReceiver *station)
{
	/* check parameters */
	if ((addr == (char *)0) || (station == (StationReceiver *)0))
	{
		ERROR("null address or station.", EINVAL);
		return(NOTOK);
	}

	/* check if list is full */
	if (numberOfReceivers >= MaxNumberOfStations)
	{
		ERROR("too many receivers.", EINVAL);
		return(NOTOK);
	}

	/* store new receiver */
	strcpy(receivers[numberOfReceivers].address, addr);
	receivers[numberOfReceivers].station = station;
	numberOfReceivers++;

	/* all done */
	return(OK);
}

/* unregister a station receiver */
int
StationList::unregisterReceiver(char *addr)
{
	/* check parameter */
	if (addr == (char *)0)
	{
		ERROR("null address.", EINVAL);
		return(NOTOK);
	}

	/* find station and remove it from list */
	int istation;
	for (istation = 0; istation < numberOfReceivers; istation++)
	{
		if (strcmp(receivers[istation].address, addr) == 0) break;
	}

	/* check if station was found */
	if (istation >= numberOfReceivers)
	{
		/* station was not found */
		ERRORS("station not found.", addr, EINVAL);
		return(NOTOK);
	}

	/* delete station, and move other stations down */
	for (--numberOfReceivers; istation < numberOfReceivers; istation++)
	{
		strcpy(receivers[istation].address, 
			receivers[istation+1].address);
		receivers[istation].station =  receivers[istation+1].station;
	}

	/* all done */
	return(OK);
}
