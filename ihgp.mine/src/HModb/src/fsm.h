#ifndef __FSM_H
#define __FSM_H
// definitions for finite state machine class 

// unix headers
#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"

// definitions
#define DefaultFASize 128

// finite state machine class
template <class InputType> class FSM {
public:
	// constructor and destructor
	FSM();
	FSM(const FSM &);
	~FSM();

	// operations
	int insertState(int);
	int removeState(int);
	int insertTransition(int, int, const InputType &);
	int removeTransition(int, int, const InputType &);

	// get transition for a given input
	int move(int, const InputType &, int &);

	FA &operator=(const FA &);

protected:
	int status;
	Set inputSet;
	FAstateList *fa;
};

#endif
