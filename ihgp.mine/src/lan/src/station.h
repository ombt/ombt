#ifndef __STATION_H
#define __STATION_H
/* class for work stations */

/* headers */
#include "returns.h"
#include "item.h"
#include "exponential.h"
#include "uniform.h"
#include "event.h"
#include "cable.h"

/* declare class */
class StationSender;
class StationReceiver;

/* states for work station  */
enum StationState {
	StationIdle,
	StationTransmitting,
	StationReceiving
};

/* size of LAN address */
#define AddressSize 6

/* station name */
#define UnknownStation "000000"

/* maximum times to attempt back-off */
#define DefaultMaxBackOff 16

/* maximum time to wait for a backoff */
#define MaxBackOffInterval 10

/* maximum size of call waiting queue */
#define DefaultCallWaitingQueueSize 10

/* size of destination address */
#define DestAddrSize AddressSize
#define DestAddrOffset 0

/* size of source address */
#define SrcAddrSize AddressSize
#define SrcAddrOffset (DestAddrOffset + DestAddrSize)

/* size of type field */
#define TypeSize 2
#define TypeOffset (SrcAddrOffset + SrcAddrSize)

/* size of data field */
#define MinimumDataSize 46
#define MaximumDataSize 1500
#define DataOffset (TypeOffset + TypeSize)

/* size of CRC sum */
#define CrcSize sizeof(long)
#define CrcOffset(DataSize) (DataOffset + (DataSize))

/* size of frame */
#define MinimumMsgSize \
	(DestAddrSize + SrcAddrSize + TypeSize + MinimumDataSize + CrcSize)
#define MaximumMsgSize \
	(DestAddrSize + SrcAddrSize + TypeSize + MaximumDataSize + CrcSize)
#define MsgSize(DataSize) \
	(DestAddrSize + SrcAddrSize + TypeSize + (DataSize) + CrcSize)

/* bits in a byte */
#define BitsPerByte 8

/* speed of light in medium in km/sec */
#define SpeedOfLight 200000.0

/* time in microseconds for an EM signal to travel 2.5 km in a medium */
#define EndToEndTime ((2.5/SpeedOfLight)*1.0e6)

/* worst case time in microseconds for detecting a collision */
#define WorstCaseTime (2.0*EndToEndTime)

/* slot time in microseconds for backoffs */
#define SlotTime WorstCaseTime

/* preamble duration in microseconds */ 
#define PreambleDuration WorstCaseTime

/* time to transmit data */
#define TransmissionDuration EndToEndTime

/* class for a LAN work station transmitter */
class StationSender: public Item {
	friend int process(StationSender *);
public:
	/* constructor and destructor */
	StationSender();
	StationSender(double, char *, 
		int = MinimumMsgSize, int = MaximumMsgSize,
		int = DefaultCallWaitingQueueSize, int = DefaultMaxBackOff);
	~StationSender();

	/* initialization */
	int init();
	int init(double, char *, int, int, int, int);
	int deleteAll();

	/* generate next call origination */
	long nextCall();

	/* generate backoff time interval */
	long backOffTime(int);

	/* queue and dequeue for cable */
	Item *enqueue(Item *);
	Item *dequeue();
	int isEmpty();
	int isFull();

	/* generate size of broadcast message */
	inline msgSize() { return((((long)msgSizeDist.randomSample())/4)*4); }

	/* other functions */
	inline int getStatus() { return(status); }

private:
	/* do not allow these */
	StationSender(const StationSender &);
	StationSender &operator=(const StationSender &);

public:
	/* internal data */
	int status;

	/* sender FSM state */
	StationState state; 

	/* next event type */
	Event event;

	/* station address */
	char address[AddressSize+1];

	/* call origination rate */
	double callRate;

	/* interarrival-time distribution */
	Exponential interArrivalDist;

	/* number of backoff attempts */
	int maxBackOff;

	/* message size distribution */
	Uniform msgSizeDist;

	/* call queue waiting for station */
	int maxCallsInQueue;
	int callsInQueue;
	Queue queue;

public:
	/* total times */
	long totalTimeInCallWaitingQueue;
	long totalTimeInCableQueue;
	long totalCallDuration;
	long totalBackOffTime;
	long totalTransmissionTime;

	/* measurements */
	long callsOriginated;
	long callsQueuedForStation;
	long callsQueuedForCable;
	long callsQueueIsFull;
	long callsCollided;
	long callsInitiallyBlocked;
	long callsFailedTransmission;
};

/* class for a LAN work station receiver */
class StationReceiver: public Item {
	friend int process(StationReceiver *);
public:
	/* constructor and destructor */
	StationReceiver();
	StationReceiver(char *);
	~StationReceiver();

	/* initialization */
	int init(char * = (char *)UnknownStation);
	int deleteAll();

	/* other functions */
	inline int getStatus() { return(status); }

private:
	/* do not allow these */
	StationReceiver(const StationReceiver &);
	StationReceiver &operator=(const StationReceiver &);

public:
	/* internal data */
	int status;

	/* receiver FSM state */
	StationState state; 

	/* next event type */
	Event event;

	/* station address */
	char address[AddressSize+1];

public:
	/* measurements */
	long callsTerminated;
	long callsCorrupted;
};

/* list of stations connected to LAN */
class StationList {
public:
	/* lists of stations */
	struct StationSenderList {
		StationSender *station;
		char address[AddressSize+1];
	};
	struct StationReceiverList {
		StationReceiver *station;
		char address[AddressSize+1];
	};

public:
	/* initialize lists */
	static void initLists();

	/* store stations in lists */
	static int registerSender(char *, StationSender *);
	static int unregisterSender(char *);
	static int registerReceiver(char *, StationReceiver *);
	static int unregisterReceiver(char *);

public:
	/* list of sending stations */
	static StationSenderList senders[MaxNumberOfStations];
	static int numberOfSenders;

	/* list of receiving stations */
	static StationReceiverList receivers[MaxNumberOfStations];
	static int numberOfReceivers;
};
#endif
