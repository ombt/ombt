/* main program for simulating a lan data-link layer */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <errno.h>

/* other headers */
#include "returns.h"
#include "debug.h"
#include "station.h"
#include "call.h"
#include "cable.h"
#include "event.h"
#include "item.h"

/* random number generator seed */
long seed;

/* current simulation time */
long currentTime = 0;
long maxTime = 1;

/* queue for storing events */
Queue eventQueue;

/* work station information */
int numberOfStations = 0;
StationSender **senders;
StationReceiver **receivers;

/* cable information */
Cable *cable;

/* dummy type for msgs */
unsigned char DummyType[TypeSize] = { 0xff, 0xff };

#ifdef CRC32
/* CRC-32 polynomial */
#define CRCDIVISOR1 ((unsigned long)0x82408edb)
#define CRCDIVISOR2 ((unsigned long)0x1)
#define HIGHESTBIT ((unsigned long)0x80000000)
#endif
 
/* initializations for general system */
int
initSystem()
{
	/* get seed for random number generator */
	if (isatty(0)) printf("enter random number generator seed: ");
	scanf("%ld", &seed);
	if (seed == 0) seed = time((long *)0) | 1L;
	srand48(seed);

	/* get maximum time for simulation */
	double tmp;
	if (isatty(0)) printf("enter maximum simulation time (secs): ");
	scanf("%lf", &tmp);
	maxTime = (long)(tmp*1.0e6);	/* convert to microseconds */

	/* all done */
	return(OK);
}

/* initialize LAN work stations */
int
initStations()
{
	/* get number of stations */
	if (isatty(0)) printf("enter number of work stations: ");
	scanf("%d", &numberOfStations);
	senders = new StationSender *[numberOfStations];
	if (senders == (StationSender **)0)
	{
		errno = ENOMEM;
		ERROR("unable to malloc sender stations.", errno);
		return(NOTOK);
	}
	receivers = new StationReceiver *[numberOfStations];
	if (receivers == (StationReceiver **)0)
	{
		errno = ENOMEM;
		ERROR("unable to malloc receiver stations.", errno);
		return(NOTOK);
	}

	/* get data for each station */
	double rate;
	char address[BUFSIZ];
	int qsize;
	int minmsgsz, maxmsgsz;
	int maxbackoff;
	for (int station = 0; station < numberOfStations; station++)
	{
		/* get station address */
		if (isatty(0)) 
			printf("enter station [%d] address: ", 
				station);
		scanf("%s", address);
		address[AddressSize] = 0;

		/* get station call rate */
		if (isatty(0)) 
			printf("enter station [%d] call rate (calls/sec): ", 
				station);
		scanf("%lf", &rate);

		/* get station call waiting queue size */
		if (isatty(0)) 
			printf("enter station [%d] call waiting queue size: ", 
				station);
		scanf("%d", &qsize);
	
		/* get minimum and maximum msg size */
		if (isatty(0)) 
			printf("enter station [%d] minimum data size (bytes): ",
				station);
		scanf("%d", &minmsgsz);
		if (isatty(0)) 
			printf("enter station [%d] maximum data size (bytes): ",
				station);
		scanf("%d", &maxmsgsz);
	
		/* get maximum back off attempts */
		if (isatty(0)) 
			printf("enter station [%d] maximum backoffs: ",
				station);
		scanf("%d", &maxbackoff);
	
		/* create sender station, convert calls/sec to calls/microsec */
		senders[station] = new StationSender(rate/1.0e6, 
			address, minmsgsz, maxmsgsz, qsize, maxbackoff);
		if (senders[station] == (StationSender *)0)
		{
			errno = ENOMEM;
			ERROR("unable to malloc a sender station.", errno);
			return(NOTOK);
		}
		if ((errno = senders[station]->getStatus()) != OK)
		{
			ERROR("sender station constructor failed.", errno);
			return(NOTOK);
		}

		/* create receiver station */
		receivers[station] = new StationReceiver(address);
		if (receivers[station] == (StationReceiver *)0)
		{
			errno = ENOMEM;
			ERROR("unable to malloc a receiver station.", errno);
			return(NOTOK);
		}
		if ((errno = receivers[station]->getStatus()) != OK)
		{
			ERROR("receiver station constructor failed.", errno);
			return(NOTOK);
		}
	}
	return(OK);
}

/* initialize LAN cable */
int
initCable()
{
	/* get cable error rate */
	double rate;
	if (isatty(0)) printf("enter cable error rate (errors per bit): ");
	scanf("%lf", &rate);

	/* get bps for cable */
	long bpus;
	if (isatty(0)) printf("enter transmission delay (bits/microsecond): ");
	scanf("%ld", &bpus);

	/* create a cable, set bit errors per frame */
	cable = new Cable(rate*MaximumMsgSize*BitsPerByte, bpus);
	if (cable == (Cable *)0)
	{
		errno = ENOMEM;
		ERROR("unable to malloc cable.", errno);
		return(NOTOK);
	}
	if ((errno = cable->getStatus()) != OK)
	{
		ERROR("cable constructor failed.", errno);
		return(NOTOK);
	}
	return(OK);
}

/* generate first calls on LAN */
int
firstCalls()
{
	/* construct call originations for all stations */
	for (int station = 0; station < numberOfStations; station++)
	{
		/* originate a call */
		Call *newCall = new Call(senders[station]);
		if (newCall == (Call *)0)
		{
			errno = ENOMEM;
			ERROR("ENOMEM for a new call.", errno);
			return(NOTOK);
		}
		if ((errno = newCall->getStatus()) != OK)
		{
			errno = ENOMEM;
			ERROR("new call constructor failed.", errno);
			return(NOTOK);
		}

		/* get time for first call */
		newCall->priority = senders[station]->nextCall();
		
		/* queue up the event */
		eventQueue.priorityEnq(newCall);
		if ((errno = eventQueue.getStatus()) != OK)
		{
			ERROR("queuing event failed.", errno);
			return(NOTOK);
		}
	}
	return(OK);
}

/* print call statistics */
void
printCallData()
{
	long totalCallsOriginated = 0;
	long totalCallsQueuedForStation = 0;
	long totalCallsQueueIsFull = 0;
	long totalCallsQueuedForCable = 0;
	long totalCallsInitiallyBlocked = 0;
	long totalCallsCollided = 0;
	long totalCallsFailedTransmission = 0;
	long totalCallsTerminated = 0;
	long totalCallsCorrupted = 0;

	/* print calls originated, terminated, etc. */
	printf("\n");
	printf("Number of Stations            : %ld\n", 
		numberOfStations);
	printf("Actual Simulation Time        : %ld usecs\n", 
		currentTime);
	printf("Maximum Simulation Time       : %ld usecs\n", 
		maxTime);
	printf("Calls Transmitted Over Cable  : %ld\n", 
		cable->totalCallsTransmitted);
	printf("Calls Queued Waiting For Cable: %ld\n", 
		cable->totalCallsQueued);
	printf("Calls Actually Corrupted      : %ld\n", 
		cable->totalCallsCorrupted);
	printf("Bits Transmitted Over Cable   : %ld\n", 
		cable->totalBitsTransmitted);
	printf("Transmission Rate             : %.3f Mbps\n",
		(double)cable->totalBitsTransmitted/(double)currentTime);
	printf("\n"
"Station  Orig   QPass  QFail  QCable  Block  Coll   Fail   Term   Corr \n"
"         Calls  Calls  Calls  Calls   Calls  Calls  Calls  Calls  Calls\n"
"-----------------------------------------------------------------------\n");
	int station;
	for (station = 0; station < numberOfStations; station++)
	{
		printf("%-7s %6ld %6ld %6ld %7ld %6ld %6ld %6ld %6ld %6ld\n",
			senders[station]->address,
			senders[station]->callsOriginated,
			senders[station]->callsQueuedForStation,
			senders[station]->callsQueueIsFull,
			senders[station]->callsQueuedForCable,
			senders[station]->callsInitiallyBlocked,
			senders[station]->callsCollided,
			senders[station]->callsFailedTransmission,
			receivers[station]->callsTerminated,
			receivers[station]->callsCorrupted
		);

		/* add totals */
		totalCallsOriginated += 
			senders[station]->callsOriginated;
		totalCallsQueuedForStation += 
			senders[station]->callsQueuedForStation;
		totalCallsQueueIsFull += 
			senders[station]->callsQueueIsFull;
		totalCallsQueuedForCable += 
			senders[station]->callsQueuedForCable;
		totalCallsInitiallyBlocked += 
			senders[station]->callsInitiallyBlocked;
		totalCallsCollided += 
			senders[station]->callsCollided;
		totalCallsFailedTransmission += 
			senders[station]->callsFailedTransmission;
		totalCallsTerminated += 
			receivers[station]->callsTerminated;
		totalCallsCorrupted += 
			receivers[station]->callsCorrupted;
	}
	printf(
"-----------------------------------------------------------------------\n");
	printf("%-7s %6ld %6ld %6ld %7ld %6ld %6ld %6ld %6ld %6ld\n",
		"Totals",
		totalCallsOriginated,
		totalCallsQueuedForStation,
		totalCallsQueueIsFull,
		totalCallsQueuedForCable,
		totalCallsInitiallyBlocked,
		totalCallsCollided,
		totalCallsFailedTransmission,
		totalCallsTerminated,
		totalCallsCorrupted
	);
	printf("\n"
"Station Avg     Avg    Avg      Avg    Avg    Inter  \n"
"        Station Cable  Back Off Trans  Call   Arrival\n"
"        Q Time  Q Time Time     Time   Time   Time   \n"
"        (usec)  (usec) (usec)   (usec) (usec) (usec) \n"
"-----------------------------------------------------\n");
	for (station = 0; station < numberOfStations; station++)
	{
		long callStarted = 
			senders[station]->callsOriginated - 
			senders[station]->callsQueueIsFull;
		printf("%-7s %7ld %6ld %8ld %6ld %6ld %7ld\n",
		senders[station]->address,
		senders[station]->totalTimeInCallWaitingQueue/callStarted,
		senders[station]->totalTimeInCableQueue/callStarted,
		senders[station]->totalBackOffTime/callStarted,
		senders[station]->totalTransmissionTime/callStarted,
		senders[station]->totalCallDuration/callStarted,
		currentTime/senders[station]->callsOriginated
		);
	}
	return;
}

#ifdef CRC32

/* get the next bit in a buffer */
unsigned long
nextBit(unsigned long *buf, int nbuf)
{
	static int maxbyte, byte, bit;
	static unsigned long *pbuf;
	if (buf != (unsigned long *)0)
	{
		pbuf = (unsigned long *)buf;
		maxbyte = nbuf;
		byte = 0;
		bit = 31;
		return((pbuf[byte] & (1L << bit--)) ? 1L : 0L);
	}
	if (bit < 0)
	{
		byte++;
		bit = 31;
	}
	if (byte >= maxbyte)
	{
		return((unsigned long)NOTOK);
	}
	return((pbuf[byte] & (1L << bit--)) ? 1L : 0L);
}

/* calculate CRC */
unsigned long
crc32(unsigned long *buf, int nbuf)
{
	unsigned long d1, d2, bit;
	bit = nextBit(buf, nbuf);
	d1 = bit << 31;
	for (int ibit = 30; ibit >= 0; ibit--)
	{
		bit = nextBit((unsigned long *)0, nbuf);
		d1 |= (bit << ibit);
	}
	while ((d2 = nextBit((unsigned long *)0, nbuf)) != NOTOK)
	{
		if ((d1 & HIGHESTBIT) == HIGHESTBIT)
		{
			d1 ^= CRCDIVISOR1;
			d2 ^= CRCDIVISOR2;
		}
		d1 <<= 1;
		d1 += d2;
	}
	return(d1);
}

/* calculate CRC value for a msg */
unsigned long
calculateCRC(char *mbuf, int mbufsz)
{
	return(crc32((unsigned long *)mbuf, mbufsz));
}

#else

/* calculate CRC value for a msg */
unsigned long
calculateCRC(char *mbuf, int mbufsz)
{
	register unsigned long sum = 0;
	for (char *pmbuf = mbuf; pmbuf < (mbuf + mbufsz); pmbuf++)
	{
		register int c = *pmbuf;
		if (sum & 01)
			sum = (sum >> 1) + 0x8000;
		else
			sum >>= 1;
		sum += c;
		sum &= 0xffff;
	}
	return(sum);
}
#endif

/* generate a new msg */
int
generateMsg(char *mbuf, int &mbufsz, StationSender *sender)
{
	/* get size of data */
	int dataSize = (int) (sender->msgSize());
	mbufsz = MsgSize(dataSize);

	/* get destination address, do it randomly for now */
	int inum, idest;
	for (inum = 1; inum <= 20; inum++)
	{
		idest = (int)(numberOfStations*drand48());
		if (strncmp(receivers[idest]->address, 
			    sender->address, AddressSize) != 0)
		{
			/* found a destination */
			break;
		}
	}
	if (inum > 20)
	{
		/* send msg to first receiver in list by default */
		ERROR("using default address to generate msg !!!", EINVAL);
		memcpy(mbuf, receivers[0]->address, DestAddrSize);
	}
	else
	{
		/* send msg to receiver */
		memcpy(mbuf+DestAddrOffset, receivers[idest]->address, 
		       DestAddrSize);
	}

	/* store source address */
	memcpy(mbuf+SrcAddrOffset, sender->address, SrcAddrSize);

	/* store type */
	memcpy(mbuf+TypeOffset, DummyType, TypeSize);

	/* store data */
	for (int idata = DataOffset; idata < CrcOffset(dataSize); idata++)
	{
		mbuf[idata] = 0xff;
	}

	/* calculate and store CRC value */
#ifdef CRC32
	memset(mbuf+CrcOffset(dataSize), 0, CrcSize);
	unsigned long crc = calculateCRC(mbuf, mbufsz);
	memcpy(mbuf+CrcOffset(dataSize), (char *)&crc, CrcSize);
#else
	unsigned long crc = calculateCRC(mbuf, mbufsz-CrcSize);
	memcpy(mbuf+CrcOffset(dataSize), (char *)&crc, CrcSize);
#endif

	/* all done */
	return(OK);
}

/* process sender station events */
int
process(StationSender *sender)
{
	errno = EINVAL;
	ERRORS("process sender station not used.", sender->address, errno);
	return(NOTOK);
}

/* receiver idle state event processing */
int
processReceiverIdle(StationReceiver *receiver)
{
	/* check event type */
	switch (receiver->event)
	{
	case IncomingMsg:
	{
		/* check size of message */
		if (cable->messageSize < MinimumMsgSize)
		{
			/* ignore message */
			return(OK);
		}

		/* check destination address */
		if (strncmp(cable->message + DestAddrOffset, 
			    receiver->address, DestAddrSize) != 0)
		{
			/* not for this station */
			return(OK);
		}

		/* increment number of calls received by this station */
		receiver->callsTerminated++;

		/* check CRC value */
#ifdef CRC32
		unsigned long crc =
			calculateCRC(cable->message, cable->messageSize);

		/* check if we have a corrupted frame */
		if (crc != 0) 
			receiver->callsCorrupted++;
#else
		unsigned long crc = 
			calculateCRC(cable->message, cable->messageSize-CrcSize);
		unsigned long mcrc;
		memcpy((char *)&mcrc, 
		       cable->message + cable->messageSize - CrcSize,
		       CrcSize);

		/* check if we have a corrupted frame */
		if (crc != mcrc) 
			receiver->callsCorrupted++;
#endif
		return(OK);
	}
	default:
		errno = EINVAL;
		ERRORI("unexpected event in receiver idle state.",
			receiver->event, errno);
		return(NOTOK);
	}
}

/* process receiver station events */
int
process(StationReceiver *receiver)
{
	/* check receiver state */
	switch (receiver->state)
	{
	case StationIdle:
		return(processReceiverIdle(receiver));
	default:
		errno = EINVAL;
		ERRORI("unknown receiver state.", receiver->state, errno);
		return(NOTOK);
	}
}

/* retry event processing */
int
processRetry(Call *call)
{
	/* check if maximum back offs was exceeded */
	if (call->backOff >= call->station->maxBackOff)
	{
		/* increment counters */
		call->station->callsFailedTransmission++;

		/* check if call waiting queue is empty */
		if ( ! call->station->isEmpty())
		{
			/* remove a call from call waiting queue */
			Call *qcall = (Call *)call->station->dequeue();
			if (qcall == (Call *)0)
			{
				errno = EINVAL;
				ERRORS("dequeuing a call waiting call failed.", 
					call->station->address, errno);
				return(NOTOK);
			}
			qcall->priority = currentTime;
			qcall->station->totalTimeInCallWaitingQueue +=
				currentTime - qcall->queuedForStationStart;

			/* schedule call */
			eventQueue.priorityEnq(qcall);
			if ((errno = eventQueue.getStatus()) != OK)
			{
				ERRORS("queuing of call waiting call failed.", 
					qcall->station->address, errno);
				return(NOTOK);
			}

			/*
			 * although the station is idle at this point, leave 
			 * station state as transmitting to block any call 
			 * originations. we want the dequeued call to get the 
			 * station next, not a new originating call.
			 */
			call->station->state = StationTransmitting;
		}
		else
		{
			/* idle station */
			call->station->state = StationIdle;
		}

		/* call dies */
		call->station->totalCallDuration +=
			currentTime - call->callStart;
		delete call;
	}
	else
	{
		/* schedule a back off event */
		call->state = BackOff;
		call->event = BackOffEnds;
		call->priority = currentTime + 
			call->station->backOffTime(call->backOff);
		call->backOffStart = currentTime;
		eventQueue.priorityEnq(call);
		if ((errno = eventQueue.getStatus()) != OK)
		{
			ERRORS("queuing of back off ends event failed.", 
				call->station->address, errno);
			return(NOTOK);
		}
	}
	return(OK);
}

/* start transmission event processing */
int
processStartTransmission(Call *call)
{
	/* check cable state */
	switch (cable->state)
	{
	case Idle:
		/* start preamble for this call */
		call->state = StartPreamble;
		call->event = PreambleEnds;
		call->priority = currentTime + PreambleDuration;

		/* switch cable to preamble */
		cable->state = Preamble;

		/* queue up the event */
		eventQueue.priorityEnq(call);
		if ((errno = eventQueue.getStatus()) != OK)
		{
			ERRORS("queuing end of preamble event failed.", 
				call->station->address, errno);
			return(NOTOK);
		}
		return(OK);

	case Preamble:
		/* collision, increment counters */
		if (call->backOff == 0) 
			call->station->callsInitiallyBlocked++;
		call->station->callsCollided++;
		call->backOff++;

		/* change cable state to jammed */
		cable->state = Jammed;

		/* retry transmission */
		call->state = Retry;
		return(processRetry(call));

	case Jammed:
		/* collision, increment counters */
		if (call->backOff == 0) 
			call->station->callsInitiallyBlocked++;
		call->station->callsCollided++;
		call->backOff++;

		/* retry transmission */
		call->state = Retry;
		return(processRetry(call));

	case Transmitting:
		/* wait for cable to idle */
		call->state = QueueForCable;
		call->event = CableIsIdle;
		call->queuedForCableStart = currentTime;
		call->station->callsQueuedForCable++;
		cable->enqueue(call);
		if ((errno = cable->getStatus()) != OK)
		{
			ERRORS("queuing for cable failed.", 
				call->station->address, errno);
			return(NOTOK);
		}
		return(OK);

	default:
		errno = EINVAL;
		ERRORI("unexpected event in start transmission state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* start call event processing */
int
processStartCall(Call *call)
{
	/* increment counter and mark start of call */
	call->station->callsOriginated++;
	call->callStart = call->priority;

	/* schedule next call origination */
	Call *newCall = new Call(call->station);
	if (newCall == (Call *)0)
	{
		errno = ENOMEM;
		ERROR("ENOMEM for a new call.", errno);
		return(NOTOK);
	}
	if ((errno = newCall->getStatus()) != OK)
	{
		errno = ENOMEM;
		ERROR("new call constructor failed.", errno);
		return(NOTOK);
	}

	/* get time for next call */
	newCall->priority = currentTime + call->station->nextCall();
		
	/* queue up the event */
	eventQueue.priorityEnq(newCall);
	if ((errno = eventQueue.getStatus()) != OK)
	{
		ERROR("queuing event failed.", errno);
		return(NOTOK);
	}

	/* check event type */
	switch (call->event)
	{
	case CallOrigination:
		/* check condition of station */
		switch (call->station->state)
		{
		case StationIdle:
			/* start transmission */
			call->state = StartTransmission;
			call->station->state = StationTransmitting;
			return(processStartTransmission(call));

		case StationTransmitting:
			/* check if call waiting queue is full */
			if (call->station->isFull())
			{
				/* increment counter and kill call */
				call->station->callsQueueIsFull++;
				delete call;
				return(OK);
			}

			/* increment queue for station counter */
			call->station->callsQueuedForStation++;

			/* queue for station to idle */
			call->state = QueueForStation;
			call->event = StationIsIdle;
			call->queuedForStationStart = currentTime;
			call->station->enqueue(call);
			if ((errno = call->station->getStatus()) != OK)
			{
				ERRORS("queuing for station failed.",
					call->station->address, errno);
				return(NOTOK);
			}
			return(OK);

		default:
			errno = EINVAL;
			ERRORI("unexpected sender station state.",
				call->station->state, errno);
			return(NOTOK);
		}

	default:
		errno = EINVAL;
		ERRORI("unexpected event in start call state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* queue for station event processing */
int
processQueueForStation(Call *call)
{
	/* check event */
	switch (call->event)
	{
	case StationIsIdle:
		/* start transmission */
		call->state = StartTransmission;
		call->station->state = StationTransmitting;
		return(processStartTransmission(call));

	default:
		errno = EINVAL;
		ERRORI("unexpected event in queue for station state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* queue for cable event processing */
int
processQueueForCable(Call *call)
{
	/* check event */
	switch (call->event)
	{
	case CableIsIdle:
		/* start transmission */
		call->state = StartTransmission;
		return(processStartTransmission(call));

	default:
		errno = EINVAL;
		ERRORI("unexpected event in queue for cable state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* start preamble event processing */
int
processStartPreamble(Call *call)
{
	int msgBufferSize;
	char msgBuffer[2*MaximumMsgSize];

	/* check event */
	switch (call->event)
	{
	case PreambleEnds:
	{
		/* check status of cable */
		switch (cable->state)
		{
		case Preamble:
		{
			/* start sending msg over cable */
			call->state = StartMsg;

			/* generate msg buffer */
			if (generateMsg(msgBuffer, msgBufferSize, 
				call->station) != OK)
			{
				ERROR("unable to generate a msg.", errno);
				return(NOTOK);
			}

			/* set cable state to transmitting */
			cable->state = Transmitting;
			call->transmissionStart = currentTime;

			/* send msg on cable */
			cable->sendMessage(msgBuffer, msgBufferSize);
			if ((errno = cable->getStatus()) != OK)
			{
				ERROR("unable to send msg.", errno);
				return(NOTOK);
			}

			/* schedule all readers */
			long maxTran = 0;
			for (int istation = 0; istation < numberOfStations; 
				istation++)
			{
				/* schedule reader */
				receivers[istation]->state = StationIdle;
				receivers[istation]->event = IncomingMsg;
				receivers[istation]->priority = 
					currentTime + 
					TransmissionDuration*drand48() + 
					(msgBufferSize*BitsPerByte)/cable->bpus;
				if (receivers[istation]->priority > maxTran)
					maxTran = receivers[istation]->priority;

				/* queue up reader */
				eventQueue.priorityEnq(receivers[istation]);
				if ((errno = eventQueue.getStatus()) != OK)
				{
					ERRORS("queuing income msg failed.", 
					receivers[istation]->address, errno);
					return(NOTOK);
				}
			}

			/* queue end of message event */
			call->event = MsgEnds;
			call->priority = maxTran + 1;
			eventQueue.priorityEnq(call);
			if ((errno = eventQueue.getStatus()) != OK)
			{
				ERRORS("queuing end of msg failed.", 
					call->station->address, errno);
				return(NOTOK);
			}
			return(OK);
		}

		case Jammed:
		{
			/* a collision was detected idle cable */
			cable->state = Idle;

			/* increment counters */
			call->station->callsCollided++;
			call->backOff++;

			/* reschedule this sender with a jammer event */
			call->state = Retry;
			call->event = Jammer;
			eventQueue.priorityEnq(call);
			if ((errno = eventQueue.getStatus()) != OK)
			{
				ERRORS("queuing jammer event failed.", 
					call->station->address, errno);
				return(NOTOK);
			}
			return(OK);

		}
		default:
			errno = EINVAL;
			ERRORI("unexpected cable state in start preamble state.",
				cable->state, errno);
			return(NOTOK);
		}
	}

	default:
		errno = EINVAL;
		ERRORI("unexpected event in start preamble state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* back off event processing */
int
processBackOff(Call *call)
{
	/* check event */
	switch (call->event)
	{
	case BackOffEnds:
		/* start transmission */
		call->state = StartTransmission;
		call->station->totalBackOffTime += 
			currentTime - call->backOffStart;
		return(processStartTransmission(call));

	default:
		errno = EINVAL;
		ERRORI("unexpected event in back off state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* start message event processing */
int
processStartMsg(Call *call)
{
	/* check event */
	switch (call->event)
	{
	case MsgEnds:
		/* end of transmission, idle cable */
		cable->deleteMessage();
		cable->state = Idle;

		/* schedule all senders in cable queue */
		Call *pcall;
		while ( ! cable->isEmpty())
		{
			/* schedule all stations */
			pcall = (Call *)cable->dequeue();
			pcall->priority = currentTime;
			eventQueue.priorityEnq(pcall);
			if ((errno = eventQueue.getStatus()) != OK)
			{
				ERRORS("queuing cable is idle event failed.", 
					pcall->station->address, errno);
				return(NOTOK);
			}
			pcall->station->totalTimeInCableQueue +=
				currentTime - pcall->queuedForCableStart;
		}

		/* check if call waiting queue is empty */
		if ( ! call->station->isEmpty())
		{
			/* remove a call from call waiting queue */
			Call *qcall = (Call *)call->station->dequeue();
			if (qcall == (Call *)0)
			{
				errno = EINVAL;
				ERRORS("dequeuing a call waiting call failed.", 
					call->station->address, errno);
				return(NOTOK);
			}
			qcall->priority = currentTime;
			qcall->station->totalTimeInCallWaitingQueue +=
				currentTime - qcall->queuedForStationStart;

			/* schedule call */
			eventQueue.priorityEnq(qcall);
			if ((errno = eventQueue.getStatus()) != OK)
			{
				ERRORS("queuing of call waiting call failed.", 
					qcall->station->address, errno);
				return(NOTOK);
			}

			/*
			 * although the station is idle at this point, leave 
			 * station state as transmitting to block any call 
			 * originations. we want the dequeued call to get the 
			 * station next, not a new originating call.
			 */
			call->station->state = StationTransmitting;
		}
		else
		{
			/* idle station */
			call->station->state = StationIdle;
		}

		/* call dies */
		call->station->totalCallDuration +=
			currentTime - call->callStart;
		call->station->totalTransmissionTime +=
			currentTime - call->transmissionStart;
		delete call;
		return(OK);

	default:
		errno = EINVAL;
		ERRORI("unexpected event in start msg state.",
			call->event, errno);
		return(NOTOK);
	}
}

/* call dies event processing */
int
processCallDies(Call *call)
{
	errno = EINVAL;
	ERRORS("process call dies state is not used.",
		call->station->address, errno);
	return(NOTOK);
}

/* process call events */
int
process(Call *call)
{
	/* check call state */
	switch (call->state)
	{
	case StartCall:
		return(processStartCall(call));
	case QueueForStation:
		return(processQueueForStation(call));
	case StartTransmission:
		return(processStartTransmission(call));
	case QueueForCable:
		return(processQueueForCable(call));
	case StartPreamble:
		return(processStartPreamble(call));
	case Retry:
		return(processRetry(call));
	case BackOff:
		return(processBackOff(call));
	case StartMsg:
		return(processStartMsg(call));
	case CallDies:
		return(processCallDies(call));
	default:
		errno = EINVAL;
		ERRORI("unknown call state.", call->state, errno);
		return(NOTOK);
	}
}

#if 0
/* signal handler */
void
interrupt(int, int, sigcontext *, char *)
{
	printCallData();
	exit(2);
}
#endif

/* main entry point for simulator */
submain(int, char **)
{
	/* preliminary initializations */
	if (initSystem() != OK)
	{
		ERROR("initSystem failed.", errno);
		return(2);
	}

	/* initialize LAN cable */
	if (initCable() != OK)
	{
		ERROR("initCable failed.", errno);
		return(2);
	}

	/* initialize LAN stations */
	if (initStations() != OK)
	{
		ERROR("initStations failed.", errno);
		return(2);
	}

	/* create first call events */
	if (firstCalls() != OK)
	{
		ERROR("firstCalls failed.", errno);
		return(2);
	}

#if 0
	/* declare interrupt handler */
	(void) signal(SIGINT, interrupt);
#endif

	/* start event executive */
	while ( ! eventQueue.isEmpty() && currentTime < maxTime)
	{
		/* get next event in queue */
		Item *item = eventQueue.priorityDeq();
		if (item == (Item *)0)
		{
			errno = ENOMEM;
			ERROR("priority dequeue failed.", errno);
			return(2);
		}

		/* update simulation clock */
		currentTime = item->getPriority();

		/* handle event */
		switch (item->getType())
		{
		case SenderType:
			if (process((StationSender *)item) != OK)
			{
				ERROR("processing of event failed.", errno);
				return(2);
			}
			break;
		case ReceiverType:
			if (process((StationReceiver *)item) != OK)
			{
				ERROR("processing of event failed.", errno);
				return(2);
			}
			break;
		case CallerType:
			if (process((Call *)item) != OK)
			{
				ERROR("processing of event failed.", errno);
				return(2);
			}
			break;
		case UnknownType:
		default:
			errno = EINVAL;
			ERROR("unknown item type.", errno);
			return(2);
		}
	}

	/* print statistics */
	printCallData();

	/* all done */
	return(0);
}

/* main function */
main(int argc, char **argv)
{
	exit(submain(argc, argv));
}

