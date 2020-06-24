/* class for LAN cable */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>

/* other headers */
#include "returns.h"
#include "debug.h"
#include "cable.h"

/* local definitions */
#define BitsInByte 8

/* constructors and destructor */
Cable::Cable()
{
	init();
}

Cable::Cable(double errs, long mybpus)
{
	init(errs, mybpus);
}

Cable::~Cable()
{
	deleteAll();
}

/* initialization routines */
int
Cable::init(double errs, long mybpus)
{
	/* default cable state */
	state = Idle;

	/* no event for now */
	event = NoEvent;

	/* no message send for now */
	message = (char *)0;
	messageSize = 0;

	/* mean error bit rate for messages */
	meanErrorRate = errs;

	/* poisson error distribution */
	errorDist.init(errs);

	/* transmission delay in bits per microsecond */
	bpus = mybpus;

	/* processes waiting for an idle cable */
	queue.init();

	/* measurements */
	totalCallsTransmitted = 0;
	totalCallsQueued = 0;
	totalBitsTransmitted = 0;
	totalCallsCorrupted = 0;

	/* all done */
	status = OK;
	return(OK);
}

int
Cable::deleteAll()
{
	/* idle station */
	state = Idle;
	event = NoEvent;

	/* delete message buffer */
	if (message != (char *)0) delete message;
	message = (char *)0;
	messageSize = 0;

	/* initialize error rate */
	meanErrorRate = 0.0;
	errorDist.init(0.0);

	/* transmission delay in bits/microsecond */
	bpus = 1;

	/* clean out any processes in queue */
	queue.deleteAll();

	/* zap measurements */
	totalCallsTransmitted = 0;
	totalCallsQueued = 0;
	totalBitsTransmitted = 0;
	totalCallsCorrupted = 0;

	/* all done */
	status = OK;
	return(OK);
}

/* send a message */
int
Cable::sendMessage(char *msg, int msgsz)
{
	/* store message */
	state = Transmitting;
	message = new char[msgsz];
	if (message == (char *)0)
	{
		state = Idle;
		status = ENOMEM;
		ERROR("ENOMEM for msg.", status);
		return(NOTOK);
	}
	memcpy(message, msg, msgsz);
	messageSize = msgsz;
	totalCallsTransmitted++;
	totalBitsTransmitted += messageSize*BitsInByte;

	/* generate random errors in message */
	errorsInMessage();

	/* all done */
	status = OK;
	return(OK);
}

/* get a message */
int
Cable::receiveMessage(char *msg, int &msgsz)
{
	/* check if cable is busy */
	if (state != Transmitting)
	{
		status = EINVAL;
		ERROR("Cable is idle.", status);
		return(NOTOK);
	}

	/* get message */
	memcpy(msg, message, messageSize);
	msgsz = messageSize;

	/* idle resources */
	delete message;
	message = (char *)0;
	messageSize = 0;

	/* all done */
	status = OK;
	return(OK);
}

/* generate errors in message */
int
Cable::errorsInMessage()
{
	/* check message */
	if (message == (char *)0)
	{
		status = EINVAL;
		ERROR("Message buffer is null.", status);
		return(NOTOK);
	}

	/* determine number of errors in message */
	int nerrs = (int)(errorDist.randomSample()+0.5);
	if (nerrs <= 0)
	{
		/* no errors to generate */
		status = OK;
		return(OK);
	}

	/* actual number of calls corrupted */
	totalCallsCorrupted++;

	/* create those little errors in message */
	for (int ierrs = 1; ierrs <= nerrs; ierrs++)
	{
		/* get the byte and bit to mangle */
		int byte = (int)((messageSize-1)*drand48());
		int bit = (int)(7.99*drand48());
		message[byte] ^= 1 << bit;
	}

	/* all done */
	status = OK;
	return(NOTOK);
}

/* delete message buffer */
void
Cable::deleteMessage()
{
	delete message;
	messageSize = 0;
	status = OK;
	return;
}
