# keyword "scriptname" "nameofstatevariable"
scenario register registrationstate {
timers {
	waitfor200duration 10;
	interregistertime 60;
	expires 900;
}
state inactive {
	timer register {
		send register {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor200ok waitfor200duration;
		next waitfor200ok;
	}
	timer default {
		next inactive;
	}
	msg default {
		next inactive;
	}
}
state waitfor200ok {
	timer waitfor200ok {
		starttimer register interregistertime;
		next inactive;
	}
	timer default {
		next waitfor200ok;
	}
	msg 200 register {
		starttimer register ${Expires};
		next registered;
	}
	msg default {
		next waitfor200ok;
	}
}
state registered {
	timer register {
		send register {
			hdrs {
				From:
				To: 
				...
			}
		}
		starttimer waitfor200ok waitfor200duration;
		next waitfor200ok;
	}
	timer default {
		next registered;
	}
	msg default {
		next registered;
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
