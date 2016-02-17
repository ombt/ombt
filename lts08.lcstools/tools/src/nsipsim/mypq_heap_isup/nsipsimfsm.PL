%fsm = (
	origination => {
		INACTIVE => {
		},
		WAITFOR200 => {
		},
		TALKING => {
		},
		WAITFOR200BYE => {
		},
		WAITFOR200CANCEL => {
		},
		DEFAULT => {
		},
	},
	originationabandon => {
	},
	originationisup => {
	},
	originationprack => {
	},
	originationrefer => {
		INACTIVE => {
		},
		WAITFOR200 => {
		},
		TALKINGBEFOREREFER => {
		},
		WAITFOR202REFER => {
		},
		TALKING => {
		},
		WAITFOR200BYE => {
		},
		WAITFOR200CANCEL => {
		},
		DEFAULT => {
		},
	},
	originationreinvite => {
	},
	originationreqannc => {
		INACTIVE => {
		},
		WAITFOR200 => {
		},
		TALKING => {
		},
		WAITFOR200BYE => {
		},
		WAITFOR200CANCEL => {
		},
		DEFAULT => {
		},
	},
	originationupdate => {
	},
	registrar => {
		INACTIVE => {
		},
		DEFAULT => {
		}
	},
	register => {
		INACTIVE => {
		},
		WAITFOR200 => {
		},
		REGISTERED => {
		},
		DEFAULT => {
		}
	},
	termination => {
		INACTIVE => {
		},
		RINGING => {
		},
		WAITFORACK => {
		},
		TALKING => {
		}
		WAITFOR200BYE => {
		}
		WAITFOR486ACK => {
		}
		DEFAULT => {
		}
	},
	termination_busy => {
	},
	termination_redirect => {
	},
	termination_return => {
	},
	terminationisup => {
	},
	terminationprack => {
	},
	terminationrefer => {
		INACTIVE => {
		},
		RINGING => {
		},
		WAITFORACK => {
		},
		TALKINGBEFOREREFER => {
		},
		WAITFOR202REFER => {
		}
		TALKING => {
		}
		WAITFOR200BYE => {
		}
		DEFAULT => {
		}
	},
	terminationreinvite => {
	},
	terminationreqannc => {
	},
	terminationsubscribe => {
	},
	terminationupdate => {
	},
);
#
########################################################################
#
