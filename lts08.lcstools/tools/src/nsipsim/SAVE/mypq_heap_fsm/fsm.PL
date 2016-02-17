#
##########################################################################
#
# high-level msg handlers
#
sub registration_INACTIVE_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# only registration event timers are allowed
	if ($timerflag == 3) {
		# check if my timer
		if (!isMyTimer($callid, $ptimer)) {
			$ptimer = undef;
			return;
		}
		# send a register msg
		$dialogs{$callid}{cseq} += 1;
		sendRegister($callid, "mycontact");
		starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 REGISTER");
		changeDnState($dn, $handler, "WAITFOR200");
	} else {
		vprint "\n${handler} ${state}:\n";
		vprint "Unexpected timer.\n";
		vprint "(callid=$callid).\n";
	}
	$ptimer = undef;
}
#
sub registration_INACTIVE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub registration_WAITFOR200_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		$ptimer = undef;
		return;
	}
	# timed out, schedule another register attempt.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	queueReg($dn, $dndata{$dn}{interregistertime});
}
#
sub registration_WAITFOR200_RETURNCODE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	my $label = "";
	removetimer($callid);
	if ($phdrs->{returncode} == 200) {
		starttimer($callid, $dndata{$dn}{"expires"},
			"REGISTRATION EXPIRES");
		changeDnState($dn, $handler, "REGISTERED");
	} else {
		# registration denied. try again.
		delete $dialogs{$callid};
		changeDnState($dn, $handler, "INACTIVE");
		queueReg($dn, $dndata{$dn}{interregistertime});
	}
}
#
sub registration_WAITFOR200_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
		},
sub registration_REGISTERED_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		$ptimer = undef;
		return;
	}
	# send a re-register msg
	$dialogs{$callid}{cseq} += 1;
	sendRegister($callid, "mycontact");
	starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 REGISTER");
	changeDnState($dn, $handler, "WAITFOR200");
}
#
sub registration_REGISTERED_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
		},
sub registration_DEFAULT_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state}:\n";
	vprint "Unexpected timer.\n";
	vprint "(callid=$callid).\n";
	$ptimer = undef;
}
#
sub registration_DEFAULT_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub registrar_DEFAULT_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	if (!isMyTimer($callid, $ptimer)) {
		$ptimer = undef;
		return;
	}
	#
	$ptimer = undef;
	$dialogs{$callid}{"timertag"} = 0;
	changeDnState($dn, $handler, "UNREGISTER");
}
#
sub registrar_DEFAULT_REGISTER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	sendRegister200OK($callid, $phdrs, "contact");
	removetimer($callid);
	starttimer($callid, $dndata{$dn}{"expires"}, "REGISTRATION TIME OUT");
	changeDnState($dn, $handler, "REGISTER");
}
#
sub registrar_DEFAULT_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
		}
	},
sub origination_INACTIVE_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# only call orig timers are allowed.
	if ($timerflag == 2) {
		# check if my timer
		if (!isMyTimer($callid, $ptimer)) {
			dumptimer($timerflag, $ptimer);
			incrstats("origination","invalidtimers-1");
			$ptimer = undef;
			return;
		}
		# start up a call, send an INVITE
		$dialogs{$callid}{cseq} += 1;
		sendInvite($callid, "contact");
		incrstats("origination","started");
		incrstats("origination","sendInvite");
		starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 INVITE");
		changeDnState($dn, $handler, "WAITFOR200");
	} else {
		dumptimer($timerflag, $ptimer);
		incrstats("origination","invalidtimers-2");
		#
		vprint "\n${handler} ${state}:\n";
		vprint "Unexpected timer.\n";
		vprint "(callid=$callid).\n";
		$ptimer = undef;
	}
}
#
sub origination_INACTIVE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-1");
	incrstats("origination-unexpectedmsgs-1", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub origination_WAITFOR200_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("origination","invalidtimers-3");
		if ($timerflag == 2) {
			incrstats("origination","notreadyfororig");
		}
		$ptimer = undef;
		return;
	}
	# cancel call.
	$ptimer = undef;
	$dialogs{$callid}{cseq} += 1;
	sendCancel($callid, "contact");
	incrstats("origination","sendCancel");
	incrstats("origination","timedout-1");
	starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
		"WAIT FOR 200 CANCEL");
	changeDnState($dn, $handler, "WAITFOR200CANCEL");
}
#
sub origination_WAITFOR200_RETURNCODE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	my $label = "";
	# now we have the far-end URI.
	$dialogs{$callid}{peer} = $phdrs->{"To"};
	#
	if ($phdrs->{returncode} == 100) {
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 INVITE (after 100)");
		changeDnState($dn, $handler, "WAITFOR200");
		incrstats("origination","received100Invite");
	} elsif ($phdrs->{returncode} == 180) {
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 INVITE (after 180)");
		changeDnState($dn, $handler, "WAITFOR200");
		incrstats("origination","received180Invite");
	} elsif ($phdrs->{returncode} == 183) {
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
			"WAIT FOR 200 INVITE (after 183)");
		changeDnState($dn, $handler, "WAITFOR200");
		incrstats("origination","received183Invite");
	} elsif ($phdrs->{returncode} == 200) {
		sendAck($callid, $phdrs, "contact");
		incrstats("origination","sendAck");
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"maxcallduration"},
			"TALKING MAX CALL LENGTH");
		changeDnState($dn, $handler, "TALKING");
		incrstats("origination","stable");
		incrstats("origination","received200Invite");
	} elsif ($phdrs->{returncode} == 486) {
		incrstats("origination", "received486busy");
		sendAck($callid, $phdrs, "contact");
		incrstats("origination","send486Ack");
		# call ends.
		removetimer($callid);
		delete $dialogs{$callid};
		changeDnState($dn, $handler, "INACTIVE");
		incrstats("origination", "inactive");
	} elsif ($phdrs->{returncode} == 503) {
		incrstats("origination", "received503serviceunavailable");
		sendAck($callid, $phdrs, "contact");
		incrstats("origination","send503Ack");
		# call ends.
		removetimer($callid);
		delete $dialogs{$callid};
		changeDnState($dn, $handler, "INACTIVE");
		incrstats("origination", "inactive");
	} else {
		# call ends.
		removetimer($callid);
		delete $dialogs{$callid};
		changeDnState($dn, $handler, "INACTIVE");
		incrstats("origination", "inactive");
	}
}
#
sub origination_WAITFOR200_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
sub origination_WAITFOR200_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# other side is gone.
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_WAITFOR200_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-2");
	incrstats("origination-unexpectedmsgs-2", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
		},
sub origination_TALKING_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("origination","invalidtimers-4");
		if ($timerflag == 2) {
			incrstats("origination","notreadyfororig");
		}
		$ptimer = undef;
		return;
	}
	# call ends, send bye.
	$dialogs{$callid}{cseq} += 1;
	sendBye($callid, "contact");
	incrstats("origination","sendBye");
	incrstats("origination","timedout-2");
	#
	starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
		"WAIT FOR 200 BYE");
	changeDnState($dn, $handler, "WAITFOR200BYE");
}
#
sub origination_TALKING_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# onhook received
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_TALKING_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_TALKING_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-3");
	incrstats("origination-unexpectedmsgs-3", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub origination_WAITFOR200BYE_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("origination","invalidtimers-5");
		if ($timerflag == 2) {
			incrstats("origination","notreadyfororig");
		}
		$ptimer = undef;
		return;
	}
	# call ends.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
	incrstats("origination","timedout-3");
}
#
sub origination_WAITFOR200BYE_RETURNCODE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# call ends. the return code should be
	# 200OK, but really we are ending the call
	# whether it is or not.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_WAITFOR200BYE_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_WAITFOR200BYE_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# onhook received
	send200OK($callid, $phdrs, "contact");
	incrstats("origination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
}
#
sub origination_WAITFOR200BYE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-4");
	incrstats("origination-unexpectedmsgs-4", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub origination_WAITFOR200CANCEL_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("origination","invalidtimers-6");
		if ($timerflag == 2) {
			incrstats("origination","notreadyfororig");
		}
		$ptimer = undef;
		return;
	}
	# call ends.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
	incrstats("origination","timedout-4");
}
#
sub origination_WAITFOR200CANCEL_RETURNCODE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# call ends. the return code should be
	# 200OK, but really we are ending the call
	# whether it is or not.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("origination","inactive");
	incrstats("origination","waitfor200cancel-received200OK");
}
#
sub origination_WAITFOR200CANCEL_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-5");
	incrstats("origination-unexpectedmsgs-5", 
			$phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub origination_DEFAULT_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","invalidtimers-7");
	if ($timerflag == 2) {
		incrstats("origination","notreadyfororig");
	}
	#
	vprint "\n${handler} ${state}:\n";
	vprint "Unexpected timer.\n";
	vprint "(callid=$callid).\n";
	$ptimer = undef;
}
#
sub origination_DEFAULT_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("origination","unexpectedmsgs-6");
	incrstats("origination-unexpectedmsgs-6", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_INACTIVE_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","invalidtimers-1");
	vprint "\n${handler} ${state}:\n";
	vprint "Unexpected timer.\n";
	vprint "(callid=$callid).\n";
	$ptimer = undef;
}
#
sub termination_INACTIVE_INVITE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","started");
	sendInvite180Ringing($callid, $phdrs, "contact");
	incrstats("termination","sendInvite180Ringing");
	#
	if ($dndata{$dn}{"ringingduration"} > 0) {
		# wait for ringing
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"ringingduration"}, 
			"RINGING");
		changeDnState($dn, $handler, "RINGING");
		$dialogs{$callid}{lastphdrs} = $phdrs;
	} else {
		# just send 200OK immediately
		sendInvite200OK($callid, $phdrs, "contact");
		incrstats("termination","sendInvite200OK");
		#
		removetimer($callid);
		starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
			"WAIT FOR ACK");
		changeDnState($dn, $handler, "WAITFORACK");
	}
}
#
sub termination_INACTIVE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-1");
	incrstats("termination-unexpectedmsgs-1", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_RINGING_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("termination","invalidtimers-6");
		$ptimer = undef;
		return;
	}
	$phdrs = $dialogs{$callid}{lastphdrs};
	sendInvite200OK($callid, $phdrs, "contact");
	incrstats("termination","sendInvite200OK");
	#
	starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
		"WAIT FOR ACK");
	changeDnState($dn, $handler, "WAITFORACK");
}
#
sub termination_RINGING_INVITE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# busy, send 486
	send486Busy($callid, $phdrs, "contact");
	incrstats("termination","send486Busy");
	# wait for 486 ACK.
	removetimer($callid);
	changeDnState($dn, $handler, "WAITFOR486ACK");
	starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
		"WAIT FOR 486 ACK");
	incrstats("termination","waitfor486ack");
}
#
sub termination_RINGING_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
}
#
sub termination_RINGING_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_RINGING_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-2");
	incrstats("termination-unexpectedmsgs-2", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_WAITFORACK_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("termination","invalidtimers-2");
		$ptimer = undef;
		return;
	}
	# call ends.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
	incrstats("termination","timedout-1");
}
#
sub termination_WAITFORACK_INVITE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# busy, send 486
	send486Busy($callid, $phdrs, "contact");
	incrstats("termination","send486Busy");
	# wait for 486 ACK.
	removetimer($callid);
	changeDnState($dn, $handler, "WAITFOR486ACK");
	starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
		"WAIT FOR 486 ACK");
	incrstats("termination","waitfor486ack");
}
#
sub termination_WAITFORACK_ACK {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	removetimer($callid);
	starttimer($callid, $dndata{$dn}{"maxcallduration"},
		"TALKING MAX CALL LENGTH");
	changeDnState($dn, $handler, "TALKING");
	incrstats("termination","stable");
}
#
sub termination_WAITFORACK_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFORACK_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFORACK_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-3");
	incrstats("termination-unexpectedmsgs-3", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_TALKING_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("termination","invalidtimers-3");
		$ptimer = undef;
		return;
	}
	# call ends, send bye.
	$dialogs{$callid}{cseq} += 1;
	sendBye($callid, "contact");
	incrstats("termination","sendBye");
	#
	starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
		"WAIT FOR 200 BYE");
	changeDnState($dn, $handler, "WAITFOR200BYE");
	incrstats("termination","timedout-2");
}
#
sub termination_TALKING_INVITE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# busy, send 486
	send486Busy($callid, $phdrs, "contact");
	incrstats("termination","send486Busy");
	# wait for 486 ACK.
	removetimer($callid);
	changeDnState($dn, $handler, "WAITFOR486ACK");
	starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
		"WAIT FOR 486 ACK");
	incrstats("termination","waitfor486ack");
}
#
sub termination_TALKING_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# onhook received
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_TALKING_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_TALKING_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-4");
	incrstats("termination-unexpectedmsgs-4", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_WAITFOR200BYE_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("termination","invalidtimers-4");
		$ptimer = undef;
		return;
	}
	# call ends.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
	incrstats("termination","timedout-3");
}
#
sub termination_WAITFOR200BYE_INVITE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# busy, send 486
	send486Busy($callid, $phdrs, "contact");
	incrstats("termination","send486Busy");
	# wait for 486 ACK.
	removetimer($callid);
	changeDnState($dn, $handler, "WAITFOR486ACK");
	starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
		"WAIT FOR 486 ACK");
	incrstats("termination","waitfor486ack");
}
#
sub termination_WAITFOR200BYE_RETURNCODE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# call ends. the return code should be
	# 200OK, but really we are ending the call
	# whether it is or not.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFOR200BYE_CANCEL {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# last request was canceled. end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OKCancel");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFOR200BYE_BYE {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# end call.
	send200OK($callid, $phdrs, "contact");
	incrstats("termination","send200OK");
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFOR200BYE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-5");
	incrstats("termination-unexpectedmsgs-5", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
		},
sub termination_WAITFOR486ACK_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# check if my timer
	if (!isMyTimer($callid, $ptimer)) {
		incrstats("termination","invalidtimers-7");
		$ptimer = undef;
		return;
	}
	# call ends.
	$ptimer = undef;
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
	incrstats("termination","timedout-5");
}
#
sub termination_WAITFOR200BYE_ACK {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	# call ends.
	removetimer($callid);
	delete $dialogs{$callid};
	changeDnState($dn, $handler, "INACTIVE");
	incrstats("termination","inactive");
}
#
sub termination_WAITFOR200BYE_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-6");
	incrstats("termination-unexpectedmsgs-6", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
sub termination_DEFAULT_TIMER {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","invalidtimers-5");
	vprint "\n${handler} ${state}:\n";
	vprint "Unexpected timer.\n";
	vprint "(callid=$callid).\n";
	$ptimer = undef;
}
#
sub termination_DEFAULT_DEFAULT {
	my ($dn, $state, $handler, $callid, $timerflag, $current_time, 
		$recvpaddr, $pdatablocks, $phdrs, $ptimer) = @_;
	#
	incrstats("termination","unexpectedmsgs-7");
	incrstats("termination-unexpectedmsgs-7", $phdrs->{msgtype});
	#
	vprint "\n${handler} ${state} warning:\n";
	vprint "Unexpected msg $phdrs->{msgtype}.\n";
	vprint "(callid=$callid).\n";
}
#
# FSM contains call-backs for different states and events (msgs, timers).
#
%fsm = (
	registration => {
		INACTIVE => {
			TIMER => \&registration_INACTIVE_TIMER,
			DEFAULT => \&registration_INACTIVE_DEFAULT,
		},
		WAITFOR200 => {
			TIMER => \&registration_WAITFOR200_TIMER,
			RETURNCODE => \&registration_WAITFOR200_TIMER,
			DEFAULT => \&registration_WAITFOR200_TIMER,
		},
		REGISTERED => {
			TIMER => \&registration_REGISTERED_TIMER,
			DEFAULT => \&registration_REGISTERED_TIMER,
		},
		DEFAULT => {
			TIMER => \&registration_DEFAULT_TIMER,
			DEFAULT => \&registration_DEFAULT_DEFAULT,
		}
	},
	registrar => {
		DEFAULT => {
			TIMER => \&registrar_DEFAULT_TIMER,
			REGISTER => \&registrar_DEFAULT_REGISTER,
			DEFAULT => \&registrar_DEFAULT_DEFAULT,
		}
	},
	origination => {
		INACTIVE => {
			TIMER => \&origination_INACTIVE_TIMER,
			DEFAULT => \&origination_INACTIVE_DEFAULT,
		},
		WAITFOR200 => {
			TIMER => \&origination_WAITFOR200_TIMER,
			RETURNCODE => \&origination_WAITFOR200_RETURNCODE,
			CANCEL => \&origination_WAITFOR200_CANCEL,
			BYE => \&origination_WAITFOR200_BYE,
			DEFAULT => \&origination_WAITFOR200_DEFAULT,
		},
		TALKING => {
			TIMER => \&origination_TALKING_TIMER,
			BYE => \&origination_TALKING_BYE,
			CANCEL => \&origination_TALKING_CANCEL,
			DEFAULT => \&origination_TALKING_DEFAULT,
		},
		WAITFOR200BYE => {
			TIMER => \&origination_WAITFOR200BYE_TIMER,
			RETURNCODE => \&origination_WAITFOR200BYE_RETURNCODE,
			CANCEL => \&origination_WAITFOR200BYE_CANCEL,
			BYE => \&origination_WAITFOR200BYE_BYE,
			DEFAULT => \&origination_WAITFOR200BYE_DEFAULT,
		},
		WAITFOR200CANCEL => {
			TIMER => \&origination_WAITFOR200CANCEL_TIMER,
			RETURNCODE => \&origination_WAITFOR200CANCEL_RETURNCODE,
			DEFAULT => \&origination_WAITFOR200CANCEL_DEFAULT,
		},
		DEFAULT => {
			TIMER => \&origination_DEFAULT_TIMER
			DEFAULT => \&origination_DEFAULT_DEFAULT
		},
	},
	termination => {
		INACTIVE => {
			TIMER => termination_INACTIVE_TIMER,
			INVITE => termination_INACTIVE_INVITE,
			DEFAULT => termination_INACTIVE_DEFAULT,
		},
		RINGING => {
			TIMER => termination_RINGING_TIMER,
			INVITE => termination_RINGING_INVITE,
			CANCEL => termination_RINGING_CANCEL,
			BYE => termination_RINGING_BYE,
			DEFAULT => termination_RINGING_DEFAULT,
		},
		WAITFORACK => {
			TIMER => termination_WAITFORACK_TIMER,
			INVITE => termination_WAITFORACK_INVITE,
			ACK => termination_WAITFORACK_ACK,
			CANCEL => termination_WAITFORACK_CANCEL,
			BYE => termination_WAITFORACK_BYE,
			DEFAULT => termination_WAITFORACK_DEFAULT,
		},
		TALKING => {
			TIMER => termination_TALKING_TIMER,
			INVITE => termination_TALKING_INVITE,
			BYE => termination_TALKING_BYE,
			CANCEL => termination_TALKING_CANCEL,
			DEFAULT => termination_TALKING_DEFAULT,
		},
		WAITFOR200BYE => {
			TIMER => termination_WAITFOR200BYE_TIMER,
			INVITE => termination_WAITFOR200BYE_INVITE,
			RETURNCODE => termination_WAITFOR200BYE_RETURNCODE,
			CANCEL => termination_WAITFOR200BYE_CANCEL,
			BYE => termination_WAITFOR200BYE_BYE,
			DEFAULT => termination_WAITFOR200BYE_DEFAULT,
		},
		WAITFOR486ACK => {
			TIMER => termination_WAITFOR486ACK_TIMER,
			ACK => termination_WAITFOR486ACK_ACK,
			DEFAULT => termination_WAITFOR486ACK_DEFAULT,
		},
		DEFAULT => {
			TIMER => termination_DEFAULT_TIMER,
			DEFAULT => termination_DEFAULT_DEFAULT,
		},
	},
);
