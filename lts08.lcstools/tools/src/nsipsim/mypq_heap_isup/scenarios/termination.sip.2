scenario termination terminationstate {
timers {
	maxcalldurarion 30;
	ringingduration 5;
	waitfor200duration 10;
	waitforackduration 10;
}
state inactive {
	timer default {
		next inactive;
	}
	msg invite {
		send 180 invite {
			hdrs {
				From:
				To: 
				...
			}
			sdp {
				...
			}
		}
		send 183 invite {
			hdrs {
				From:
				To: 
				...
			}
			sdp {
				...
			}
		}
		starttimer ringing ringingduration;
		next ringing;
	}
	msg default {
		next inactive;
	}
}
state ringing {
	timer ringing {
		send 200 invite {
			hdrs {
				From:
				To: 
				...
			}
			sdp {
				...
			}
		}
		starttimer waitforack waitforackduration;
		next waitforack;
	}
	timer default {
		next ringing;
	}
	msg invite {
		send 486 invite {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor486ack waitforackduration;
		next waitfor486ack;
	}
	msg ack {
		next ringing;
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
		send 200 bye {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg info {
		send 200 info {
			hdrs {
				From:
				To: 
				...
			}
		}
		next ringing;
	}
	msg update {
		send 200 update {
			hdrs {
				From:
				To: 
				...
			}
		}
		next ringing;
	}
	msg default {
		next ringing;
	}
}
state waitforack {
	timer waitforack {
		idle;
	}
	timer default {
		next waitforack;
	}
	msg invite {
		send 486 invite {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor486ack waitforackduration;
		next waitfor486ack;
	}
	msg ack {
		starttimer talking maxcalldurarion;
		next talking;
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
		send 200 bye {
			hdrs {
				From:
				To: 
				...
			}
		}
		idle;
	}
	msg info {
		send 200 info {
			hdrs {
				From:
				To: 
				...
			}
		}
		next waitforack;
	}
	msg update {
		send 200 update {
			hdrs {
				From:
				To: 
				...
			}
		}
		next waitforack;
	}
	msg default {
		next waitforack;
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
	msg info {
		send 200 info {
			hdrs {
				From:
				To: 
				...
			}
		}
		next talking;
	}
	msg update {
		send 200 update {
			hdrs {
				From:
				To: 
				...
			}
		}
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
	msg invite {
		send 486 invite {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor486ack waitforackduration;
		next waitfor486ack;
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
	msg info {
		send 200 info {
			hdrs {
				From:
				To: 
				...
			}
		}
		next waitfor200bye;
	}
	msg default {
		next waitfor200bye;
	}
}
state waitfor486ack {
	timer waitfor486ack {
		idle;
	}
	timer default {
		next waitfor486ack;
	}
	msg ack {
		idle;
	}
	msg info {
		send 200 info {
			hdrs {
				From:
				To: 
				...
			}
		}
		next waitfor486ack;
	}
	msg default {
		next waitfor486ack;
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
