scenario registrar registrationstate {
state inactive {
	msg register {
		send 200 register {
			hdrs {
				From:
				To: 
				...
			}
		}
		next inactive;
	}
	timer default {
		next inactive;
	}
	msg default {
		next inactive;
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
