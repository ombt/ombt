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
#include "call.h"
#include "station.h"

/* constructors and destructor */
Call::Call(): Item(CallerType)
{
	init();
}

Call::Call(StationSender *sender): Item(CallerType)
{
	init(sender);
}

Call::~Call()
{
	deleteAll();
}

/* initialization */
int
Call::init(StationSender *sender)
{
	/* default call state */
	state = StartCall;

	/* next event */
	event = CallOrigination;

	/* station where call originates */
	station = sender;

	/* number of back offs */
	backOff = 0;

	/* start times */
	queuedForStationStart = 0;
	queuedForCableStart = 0;
	transmissionStart = 0;
	backOffStart = 0;
	callStart = 0;

	/* all done */
	status = OK;
	return(OK);
}

int
Call::deleteAll()
{
	/* nothing for now */
	status = OK;
	return(NOTOK);
}

