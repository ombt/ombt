scenario origination originationstate {
timers {
	maxcallduration 30 ;
	waitfor200duration 10;
}
state inactive {
	timer origination {
		send invite {
			hdrs {
				From:
				To: 
				...
			}
			sdp {
				...
			}
		}
		starttimer waitfor200invite waitfor200duration;
		next waitfor200invite;
	}
	timer default {
		next inactive;
	}
	msg default {
		next inactive;
	}
}
state waitfor200invite {
	timer waitfor200invite {
		send cancel {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor200cancel waitfor200duration;
		next waitfor200cancel;
	}
	timer default {
		next waitfor200invite;
	}
	msg 100 invite {
		starttimer waitfor200invite waitfor200duration;
		next waitfor200invite;
	}
	msg 180 invite {
		starttimer waitfor200invite waitfor200duration;
		next waitfor200invite;
	}
	msg 183 invite {
		starttimer waitfor200invite waitfor200duration;
		next waitfor200invite;
	}
	msg 200 invite {
		send ack {
			hdrs {
				From:
				To: 
				...
			}
			sdp {
				...
			}
		}
		starttimer talking maxcallduration;
		next talking;
	}
	msg 486 invite {
		send ack {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg 302 invite {
		send ack {
			hdrs {
				From:
				To: 
				...
			}
		}
		queueredirect;
		idle;
	}
	msg 503 invite {
		send ack {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg cancel {
		send 200 cancel {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg bye {
		send 200 cancel {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg default {
		next waitfor200invite;
	}
}
state talking {
	timer talking {
		send bye {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor200bye waitfor200duration;
		next waitfor200bye;
	}
	timer default {
		next talking;
	}
	msg bye {
		send 200 bye {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg invite {
		queuereinvite;
		idle;
	}
	msg subscribe {
		send notify {
			hdrs {
				From:
				To: 
				...
			}
		}
		next talking;
	}
	msg 200 notify {
		next talking;
	}
	msg default {
		next talking;
	}
}
state waitfor200bye {
	timer waitfor200bye {
		idle;
	}
	timer default {
		next waitfor200bye;
	}
	msg 200 bye {
		idle;
	}
	msg bye {
		send 200 bye {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg default {
		next waitfor200bye;
	}
}
state waitfor200cancel {
	timer waitfor200cancel {
		idle;
	}
	timer default {
		next waitfor200cancel;
	}
	msg 200 cancel {
		idle;
	}
	msg default {
		next waitfor200cancel;
	}
}
state default {
	timer default {
		idle;
	}
	msg default {
		idle;
	}
}
}
