#!/opt/exp/bin/perl -w
#
# SIP simulator.
#
########################################################################
#
# uses these modules
#
# official modules
#
use Fcntl;
use IO::Socket;
use Getopt::Std;
#
# my modules
#
use mypqueue;
#
#########################################################################
#
# global call-related data
#
$debugflag = 0;
$logflag = 0;
#
$parameters{delta} = 10;
$parameters{minmsgsize} = 10;
$parameters{logfile} = "";
$parameters{myip} = "127.0.0.1";
$parameters{myport} = 5060;
#
%calldata = (
	DUMMY => {
		state => "INACTIVE",
		handler => \&registersuccess,
		Scenario => "registersuccess",
		TAG => 0,
	},
);
#
# available call scenarios. add here they are written. then
# the scenario can be chosen from the cfg file via "Scenario"
# field.
#
%scenarios = (
	"registersuccess" => \&registersuccess,
	"registerfailure" => \&registerfailure,
	"termination" => \&termination,
	"refersuccess" => \&refersuccess,
	"referfailure" => \&referfailure,
);
#
#######################################################################
#
# registersuccess FSM
#
# State		Event	Next State	Actions
#-----------------------------------------------------------------
# INACTIVE	REG	REGISTER	send 200. set reg timer.
# INACTIVE	*	INACTIVE	unexpected event, ignore.
# REGISTER	TIMER	INACTIVE	nothing for now.
# REGISTER	REG	REGISTER	send 200. reset reg timer.
# REGISTER	*	REGISTER	unexpected event, ignore.
#
# registerfailure FSM
#
# State		Event	Next State	Actions
#-----------------------------------------------------------------
# INACTIVE	REG	INACTIVE	send error code.
# INACTIVE	*	INACTIVE	unexpected event, ignore.
#
use subs qw(printit);
#
sub registersuccess {
	my ($dn, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry registersuccess: DN = <$dn>\n";
	#
	if (!exists($calldata{$dn})) {
		printit "\nNo calldata for DN $dn.\n";
		return;
	}
	if (!exists(${$calldata{$dn}}{state})) {
		printit "\nNo calldata state for DN $dn.\n";
		return;
	}
	dumpCallData();
	#
	my $currstate = ${$calldata{$dn}}{state};
	printit "\nCurrent State for $dn = <$currstate>\n";
	printit "Timer Flag for $dn = <$timerflag>\n";
	#
	if ($currstate =~ /^INACTIVE/) {
		if ($timerflag) {
			printit "\nregistersuccess INACTIVE error:\n";
			printit "Unexpected timer.\n";
			printit "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{state} = "INACTIVE";
			${$calldata{$dn}}{"TAG"} = 0;
		} elsif ($phdrs->{MSGTYPE} =~ /REGISTER/) {
			$ackmsg = "SIP/2.0 200 OK\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			if (defined(${$calldata{$dn}}{"ToTag"})) {
				$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
				$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			} else {
				$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
			}
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			my $where = ${$calldata{$dn}}{"ExpiresForm"};
			if ($where eq "InExpires") {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
				$ackmsg .= "Expires: ";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			} else {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"};
				$ackmsg .= ";expires=";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			}
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + ${$calldata{$dn}}{"Expires"};
			# make internal timer a little bit longer to 
			# give the register msg a chance to come in.
			$endtime += 2;
			$newtimer{"DN"} = $dn;
			$newtimer{"TAG"} = $endtime;
			${$calldata{$dn}}{"TAG"} = $endtime;
			mypqueue::enqueue($pq, $endtime, \%newtimer);
			#
			${$calldata{$dn}}{state} = "REGISTER";
		} else {
			printit "\nregistersuccess INACTIVE error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			printit "(dn=$dn).\n";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		}
	} elsif ($currstate =~ /^REGISTER/) {
		if ($timerflag) {
			my $calldatatag = ${$calldata{$dn}}{"TAG"};
			my $timertag = $ptimer->{"TAG"};
			if ($calldatatag != $timertag) {
				printit "\nregistersuccess REGISTER warning:\n";
				printit "Ignoring old timer (tag=$timertag).\n";
				printit "(dn=$dn).\n";
				$ptimer = undef;
				next;
			}
			#
			printit "\nregistersuccess REGISTER warning:\n";
			printit "Registration timer expired. (tag=$timertag).\n";
			printit "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{"TAG"} = 0;
			${$calldata{$dn}}{state} = "INACTIVE";
		} elsif ($phdrs->{MSGTYPE} =~ /^REGISTER/) {
			$ackmsg = "SIP/2.0 200 OK\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			my $where = ${$calldata{$dn}}{"ExpiresForm"};
			if ($where eq "InExpires") {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
				$ackmsg .= "Expires: ";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			} else {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"};
				$ackmsg .= ";expires=";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			}
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + ${$calldata{$dn}}{"Expires"};
			$newtimer{"DN"} = $dn;
			$newtimer{"TAG"} = $endtime;
			${$calldata{$dn}}{"TAG"} = $endtime;
			mypqueue::enqueue($pq, $endtime, \%newtimer);
			#
			${$calldata{$dn}}{state} = "REGISTER";
		} else {
			printit "\nregistersuccess REGISTER error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}.\n";
			printit "(dn=$dn).\n";
			#
			${$calldata{$dn}}{state} = "REGISTER";
		}
	} else {
		printit "\nregistersuccess error:\n";
		printit "Unexpected state $currstate.\n";
		printit "(dn=$dn).\n";
		#
		${$calldata{$dn}}{state} = "INACTIVE";
	}
	return;
}
#
sub registerfailure {
	my ($dn, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry registerfailure: DN = <$dn>\n";
	#
	if (!exists($calldata{$dn})) {
		printit "\nNo calldata for DN $dn.\n";
		return;
	}
	if (!exists(${$calldata{$dn}}{state})) {
		printit "\nNo calldata state for DN $dn.\n";
		return;
	}
	dumpCallData();
	#
	my $currstate = ${$calldata{$dn}}{state};
	printit "\nCurrent State for $dn = <$currstate>\n";
	printit "Timer Flag for $dn = <$timerflag>\n";
	#
	if ($currstate =~ /^INACTIVE/) {
		if ($timerflag) {
			printit "\nregisterfailure INACTIVE error:\n";
			printit "Unexpected timer.\n";
			printit "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{state} = "INACTIVE";
			${$calldata{$dn}}{"TAG"} = 0;
		} elsif ($phdrs->{MSGTYPE} =~ /REGISTER/) {
			my $retcode = ${$calldata{$dn}}{"ReturnCode"};
			$ackmsg = "SIP/2.0 ${retcode} NOTOK\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			my $where = ${$calldata{$dn}}{"ExpiresForm"};
			if ($where eq "InExpires") {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
				$ackmsg .= "Expires: ";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			} else {
				$ackmsg .= "Contact: ".$phdrs->{"Contact"};
				$ackmsg .= ";expires=";
				$ackmsg .= ${$calldata{$dn}}{"Expires"}."\r\n";
			}
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		} else {
			printit "\nregisterfailure INACTIVE error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			printit "(dn=$dn).\n";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		}
	} else {
		printit "\nregisterfailure error:\n";
		printit "Unexpected state $currstate.\n";
		printit "(dn=$dn).\n";
		#
		${$calldata{$dn}}{state} = "INACTIVE";
	}
	return;
}
#
#######################################################################
#
# termination FSM
#
# State		Event	Next State	Actions
#-----------------------------------------------------------------
# INACTIVE	INVITE	RINGING		send 180. start timer.
# INACTIVE	*	INACTIVE	ignore.
#
# RINGING	TIMER	ANSWERED	send 200. start timer.
# RINGING	*	RINGING		ignore.
#
# ANSWERED	ACK	TALKING		set timer.
# ANSWERED	TIMER	INACTIVE	kill call.
# ANSWERED	*	INACTIVE	ignore.
#
# TALKING	BYE	INACTIVE	send 200.
# TALKING 	TIMER	WAITFORACK	send BYE. start timer.
#
# WAITFORACK	ACK	INACTIVE	done.
# WAITFORACK	TIMER	INACTIVE	done.
#
sub termination {
	my ($dn, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry termination: DN = <$dn>\n";
	#
	if (!exists($calldata{$dn})) {
		printit "\nNo calldata for DN $dn.\n";
		return;
	}
	if (!exists(${$calldata{$dn}}{state})) {
		printit "\nNo calldata state for DN $dn.\n";
		return;
	}
	dumpCallData();
	#
	my $currstate = ${$calldata{$dn}}{state};
	printit "\nCurrent State for $dn = <$currstate>\n";
	printit "Timer Flag for $dn = <$timerflag>\n";
	#
	if ($currstate =~ /^INACTIVE/) {
		if ($timerflag) {
			printit "\ntermination INACTIVE error:\n";
			printit "Unexpected timer.\n";
			printit "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{state} = "INACTIVE";
			${$calldata{$dn}}{"TAG"} = 0;
		} elsif ($phdrs->{MSGTYPE} =~ /INVITE/) {
			$ackmsg = "SIP/2.0 180 Ringing\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = 
				time() + ${$calldata{$dn}}{"RingingTimer"};
			$newtimer{"DN"} = $dn;
			$newtimer{"TAG"} = $endtime;
			${$calldata{$dn}}{"TAG"} = $endtime;
			mypqueue::enqueue($pq, $endtime, \%newtimer);
			#
			${$calldata{$dn}}{state} = "RINGING";
		} else {
			printit "\ntermination INACTIVE error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			printit "(dn=$dn).\n";
		}
	} elsif ($currstate =~ /^RINGING/) {
		if ($timerflag) {
			my $calldatatag = ${$calldata{$dn}}{"TAG"};
			my $timertag = $ptimer->{"TAG"};
			if ($calldatatag != $timertag) {
				printit "\ntermination REGISTER warning:\n";
				printit "Ignoring old timer (tag=$timertag).\n";
				printit "(dn=$dn).\n";
				$ptimer = undef;
				next;
			}
			#
			printit "\ntermination RINGING warning:\n";
			printit "Ringing timer expired. (tag=$timertag).\n";
			printit "(dn=$dn).\n";
			#
			$ackmsg = "SIP/2.0 200 OK\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			$ptimer = undef;
			${$calldata{$dn}}{"TAG"} = 0;
			${$calldata{$dn}}{state} = "ANSWERED";
		} else {
			printit "\ntermination RINGING error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}.\n";
			printit "(dn=$dn).\n";
		}
	} elsif ($currstate =~ /^ANSWERED/) {
		if ($timerflag) {
		} elsif ($phdrs->{MSGTYPE} =~ /RETURNCODE/) {
		} else {
			printit "\ntermination ANSWERED error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}.\n";
			printit "(dn=$dn).\n";
		}
	} elsif ($currstate =~ /^TALKING/) {
		if ($timerflag) {
		} elsif ($phdrs->{MSGTYPE} =~ /BYE/) {
		} else {
			printit "\ntermination TALKING error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}.\n";
			printit "(dn=$dn).\n";
		}
	} elsif ($currstate =~ /^WAITFORACK/) {
		if ($timerflag) {
		} elsif ($phdrs->{MSGTYPE} =~ /RETURNCODE/) {
		} else {
			printit "\ntermination WAITFORACK error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}.\n";
			printit "(dn=$dn).\n";
		}
	} else {
		printit "\ntermination error:\n";
		printit "Unexpected state $currstate.\n";
		printit "(dn=$dn).\n";
		#
		${$calldata{$dn}}{state} = "INACTIVE";
	}
	return;
}
#
#######################################################################
#
# refersuccess FSM
#
# State		Event	Next State	Actions
#-----------------------------------------------------------------
# INACTIVE	INVITE	WAITFOR202	send REFER, wait for 202.
# INACTIVE	NOTIFY	INACTIVE	send 200.
# INACTIVE	*	INACTIVE	unexpected event, ignore.
# WAITFOR202	202	INACTIVE	nothing for now.
# WAITFOR202	TIMER	INACTIVE	nothing for now.
# WAITFOR202	*	INACTIVE	unexpected event, ignore.
#
sub refersuccess {
	my ($dn, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry refersuccess: DN = <$dn>\n";
	#
	if (!exists($calldata{$dn})) {
		printit "\nNo calldata for DN $dn.\n";
		return;
	}
	if (!exists(${$calldata{$dn}}{state})) {
		printit "\nNo calldata state for DN $dn.\n";
		return;
	}
	dumpCallData();
	#
	my $currstate = ${$calldata{$dn}}{state};
	printit "\nCurrent State for $dn = <$currstate>\n";
	printit "Timer Flag for $dn = <$timerflag>\n";
	#
	if ($currstate =~ /^INACTIVE/) {
		if ($timerflag) {
			print STDERR "\nrefersuccess INACTIVE error:\n";
			print STDERR "Unexpected timer.\n";
			print STDERR "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{state} = "INACTIVE";
			${$calldata{$dn}}{"TAG"} = 0;
		} elsif ($phdrs->{MSGTYPE} =~ /NOTIFY/) {
			$ackmsg = "SIP/2.0 200 OK\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			my $where = ${$calldata{$dn}}{"ExpiresForm"};
			$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		} elsif ($phdrs->{MSGTYPE} =~ /INVITE/) {
			$ackmsg = $phdrs->{"FIRSTLINE"}."\r\n";
			$ackmsg =~ s/INVITE/REFER/;
			$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
			$ackmsg .= $parameters{myip}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}.";tag=";
			$ackmsg .= ${$calldata{$dn}}{"ToTag"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			$ackmsg .= "Refer-To: ";
			$ackmsg .= ${$calldata{$dn}}{"Refer-To"}."\r\n";
			$ackmsg .= "Max-Forwards: ".$phdrs->{"Max-Forwards"}."\r\n";
			$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = 
				time() + ${$calldata{$dn}}{"WaitFor202Timer"};
			$newtimer{"DN"} = $dn;
			$newtimer{"TAG"} = $endtime;
			${$calldata{$dn}}{"TAG"} = $endtime;
			mypqueue::enqueue($pq, $endtime, \%newtimer);
			#
			${$calldata{$dn}}{state} = "WAITFOR202";
		} else {
			print STDERR "\nrefersuccess INACTIVE error:\n";
			print STDERR "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			print STDERR "(dn=$dn).\n";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		}
	} elsif ($currstate =~ /^WAITFOR202/) {
		if ($timerflag) {
			print STDERR "\nrefersuccess WAITFOR202 error:\n";
			print STDERR "Timer expired.\n";
			print STDERR "(dn=$dn).\n";
			#
			$ptimer = undef;
			${$calldata{$dn}}{state} = "INACTIVE";
			${$calldata{$dn}}{"TAG"} = 0;
		} elsif ($phdrs->{MSGTYPE} =~ /^RETURNCODE/) {
			my $retcode = $phdrs->{RETURNCODE};
			if ($retcode == 202) {
				print STDERR "\nrefersuccess: 202 received.\n";
			} else {
				print STDERR "\nrefersuccess WAITFOR202 error:\n";
				print STDERR "202 NOT received.\n";
				print STDERR "(dn=$dn).\n";
			}
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		} else {
			print STDERR "\nrefersuccess WAITFOR202 error:\n";
			print STDERR "Unexpected msg $phdrs->{MSGTYPE}.\n";
			print STDERR "(dn=$dn).\n";
			#
			${$calldata{$dn}}{state} = "INACTIVE";
		}
	} else {
		print STDERR "\nrefersuccess error:\n";
		print STDERR "Unexpected state $currstate.\n";
		print STDERR "(dn=$dn).\n";
		#
		${$calldata{$dn}}{state} = "INACTIVE";
	}
}
#
sub referfailure {
	print STDERR "\nreferfailure: not supported for now. Exiting.\n";
	exit 2;
}
#
########################################################################
#
# functions
#
sub printit {
	my ($msg) = @_;
	#
	if ($logflag) {
		print LOGFILE ${msg};
	}
	print STDERR ${msg};
}
#
sub dumpCallData {
	if (!$debugflag) {
		return;
	}
	printit "\n";
	printit "Dumping Call-Data structure:\n";
	foreach $key (keys %calldata) {
		printit "key --> <$key>\n";
		foreach $key2 (keys %{$calldata{$key}}) {
			printit "\tkey2 --> <$key2>\n";
			if (defined($calldata{$key}{$key2})) {
				printit "\t\t$calldata{$key}{$key2}\n";
			} else {
				printit "\t\tundefined \$calldata{$key}{$key2}\n";
			}
		}
	}
}
#
sub readconfigfile {
	my ($configfile) = @_;
	#
	my $dn = "";
	open(CONFIGFILE, $configfile) or die "can't open $configfile: $!";
	while (defined($crec = <CONFIGFILE>)) {
		# remove newline
		chomp($crec);
		# skip comments
		if ($crec =~ /^[\s]*#/) {
			next;
		}
		# is it a parameter?
		if ($crec =~ /^parameter/i) {
			my ($dummy, $parameter, $value) = split /\s+/, $crec, 3;
			if (!defined($dummy) || !defined($parameter) ||
			    !defined($value)) {
				next;
			}
			print "$parameter = $value\n";
			$parameters{$parameter} = $value;
			next;
		}
		# get name-value pair
		my ($key, $value) = split /\s*=\s*/, $crec, 2;
		next unless defined($key);
		#
		if ($key =~ /^DN/) {
			printit "\nReading data for DN $value ...\n";
			$dn = $value;
			$calldata{$dn} = {
				state => "INACTIVE",
				handler => \&registersuccess,
				Scenario => "registersuccess",
				TAG => 0,
			};
		} else {
			printit "\$calldata{$dn}{$key} = $value\n";
			${$calldata{$dn}}{$key} = $value;
		}
	}
	close(CONFIGFILE);
	#
	foreach $key (keys %calldata) {
		if (exists $scenarios{${$calldata{$key}}{Scenario}}) {
			${$calldata{$key}}{handler} = 
				$scenarios{${$calldata{$key}}{Scenario}};
		} else {
			printit "\nUnknown scenario <${$calldata{$key}}{Scenario}>.\n";
			exit 2;
		}
	}
	#
	dumpCallData();
}
#
sub parseMsg {
	my ($rawmsg, $prawhdr, $prawdata, $phdrs) = @_;
	#
	($$prawhdr, $$prawdata) = split /\r\n\r\n/, $rawmsg, 2;
	return unless (defined($$prawhdr));
	#
	my @hdrs = split /[\r\n]+/, $$prawhdr;
	$phdrs->{FIRSTLINE} = shift @hdrs;
	my @dummy = split /\s+/, $phdrs->{FIRSTLINE};
	$phdrs->{MSGTYPE} = shift @dummy;
	if ($phdrs->{MSGTYPE} =~ /^SIP/) {
		$phdrs->{MSGTYPE} = "RETURNCODE";
		$phdrs->{RETURNCODE} = shift @dummy;
	}
	#
	foreach $hdr (@hdrs) {
		($key, $value) = split /\s*:\s*/, $hdr, 2;
		$value = "" unless defined($value);
		$phdrs->{$key} = $value;
	}
	#
	# determine address of record depending on MSG type.
	#
	if ($phdrs->{MSGTYPE} =~ /REGISTER/) {
		my $dn = $phdrs->{"To"};
		$dn =~ /^[^<]*<([^>]*)>.*$/;
		$phdrs->{"DN"} = $1;
	} elsif ($phdrs->{MSGTYPE} =~ /INVITE/) {
		my $dn = $phdrs->{"To"};
		$dn =~ /^[^<]*<([^>]*)>.*$/;
		$phdrs->{"DN"} = $1;
	} else {
		# for now, ignore all other msgs
		$phdrs->{"DN"} = "NONE";
	}
}
#
sub dumpIncomingMsg {
	my ($recvpaddr, $msg) = @_;
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	printit "\n";
	printit "incoming msg <<<<<<<<<<<<<<<<<<\n";
	printit "FROM IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		printit "line=<$line>\n";
	}
	printit "incoming msg <<<<<<<<<<<<<<<<<<\n";
}
#
sub dumpOutgoingMsg {
	my ($recvpaddr, $msg) = @_;
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	printit "\n";
	printit ">>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
	printit "TO IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		printit "line=<$line>\n";
	}
	printit ">>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
}
#
sub dumpHdrs {
	my ($phdrs) = @_;
	#
	if (!$debugflag) {
		return;
	}
	#
	printit "\n";
	foreach $key (keys %$phdrs) {
		printit "$key ==> ";
		printit $phdrs->{$key};
		printit "\n";
	}
}
#
sub handleInput {
	my ($done_ref) = @_;
	#
	my $line = <STDIN>;
	if (!defined($line)) {
		return;
	}
	chomp($line);
	#
	if ($line =~ /^[qQ]/) {
		printit "\nQuitting ...\n";
		$$done_ref = 1;
	}
}
#
sub handleEvent {
	my ($current_time, $recvpaddr, $rawmsg, $ptimer) = @_;
	#
	if (defined($ptimer)) {
		if (!defined($ptimer->{"DN"})) {
			printit "\nNo DN defined in timer.\n";
			return;
		}
		if (!defined($calldata{$ptimer->{"DN"}})) {
			my $dn = $ptimer->{"DN"};
			printit "\nNo calldata for $dn defined.\n";
			return;
		}
		#
		my $dn = $ptimer->{"DN"};
		$calldata{$dn}{handler}->($dn, 1, $current_time, 
			undef, undef, undef, undef, undef, $ptimer);
	} else {
		# ignore garbage msgs that are to small.
		if (length($rawmsg) < $parameters{minmsgsize})
		{
			# garbage msg, skip it ...
			return;
		}
		#
		dumpIncomingMsg($recvpaddr, $rawmsg);
		#
		my %hdrs;
		my $rawhdr;
		my $rawdata;
		parseMsg($rawmsg, \$rawhdr, \$rawdata, \%hdrs);
		dumpHdrs(\%hdrs);
		#
		if (!defined($hdrs{"DN"})) {
			printit "\nNo DN defined in msg.\n";
			return;
		}
		if (!defined($calldata{$hdrs{"DN"}})) {
			my $dn = $hdrs{"DN"};
			printit "\nNo calldata for $dn defined.\n";
			return;
		}
		#
		my $dn = $hdrs{"DN"};
		$calldata{$dn}{handler}->($dn, 0, $current_time, 
			$recvpaddr, $rawmsg, $rawhdr, $rawdata, \%hdrs, undef);
	}
	return;
}
#
#############################################################################
#
# get IP address and port from cmd line.
#
printit "\nSIP EndPoint Simulator:\n";
#
# get cmd line options
#
my %opts;
getopts('Dd:l:', \%opts);
foreach my $opt (%opts) {
	if ($opt eq "l") {
		$parameters{logfile} = $opts{$opt};
		printit "==>> logfile = $parameters{logfile}\n";
		open(LOGFILE, ">".$parameters{logfile}) or 
			die "can't open $parameters{logfile}: $!";
		LOGFILE->autoflush(1);
		$logflag = 1;
	} elsif ($opt eq "d") {
		$parameters{delta} = $opts{$opt};
		printit "==>> delta = $parameters{delta}\n";
	} elsif ($opt eq "D") {
		$debugflag = 1;
		printit "==>> debugflag = $debugflag\n";
	}
}
#
# sanity check
#
my ($udpip, $udpport, $configfile) = @ARGV;
if (!defined($udpip) || !defined($udpport) || !defined($configfile)) {
	printit "\nusage: sipsim [-x] [-d delta_in_secs] IP PORT configfile\n";
	exit 2;
} else {
	$parameters{myip} = $udpip;
	$parameters{myport} = $udpport;
	#
	printit "\n";
	printit "IP address : $udpip\n";
	printit "IP port    : $udpport\n";
	printit "Delta(secs): $parameters{delta}\n";
}
#
# get data
#
readconfigfile($configfile);
#
# show new parameters, if any.
#
printit "\n"; 
printit "delta      = $parameters{delta}\n";
printit "minmsgsize = $parameters{minmsgsize}\n";
#
# set up socket connection
#
my $proto = getprotobyname('udp');
defined($proto) or die "getprotobyname: $!";
socket(SOCKET, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
#
my $ipaddr = gethostbyname($udpip);
defined($ipaddr) or die "gethostbyname: $!";
my $paddr = sockaddr_in($udpport, $ipaddr);
defined($paddr) or die "getprotobyname: $!";
#
bind(SOCKET, $paddr) or die "bind: $!";
#
my $rin = '';
my $rout = '';
#
vec($rin, fileno(STDIN), 1) = 1;
vec($rin, fileno(SOCKET), 1) = 1;
#
# we don't want to I/O block, if no input, just return.
#
fcntl(STDIN, F_SETFL, O_NONBLOCK);
fcntl(SOCKET, F_SETFL, O_NONBLOCK);
#
my $done = 0;
my $start_time = time();
my $current_time = $start_time;
my $elapsed_time= 0;
my $previous_time= 0;
my $total_elapsed_time= 0;
#
# priority queue for timer events
#
$pq = mypqueue::makepqueue();
defined($pq) or die "unable to create priority queue.";
#
# event loop for simulation. overkill for now, but it
# will be expanded.
#
while ( ! $done) {
	$previous_time = $current_time;
	#
	# wait for input or timer to expire.
	#
	my $nf = select($rout=$rin, undef, undef, $parameters{delta});
	#
	# update internal clocks
	#
	$current_time = time();
	$elapsed_time = $current_time-$previous_time;
	$total_elapsed_time= $current_time-$start_time;
	#
	printit "\n";
	printit "delta   = $parameters{delta}\n";
	printit "current = $current_time\n";
	printit "elapsed = $elapsed_time\n";
	printit "telapsed= $total_elapsed_time\n";
	#
	# handle any expired timers
	#
	while (!mypqueue::isEmpty($pq)) {
		my $key;
		my $pdata;
		#
		if ((!mypqueue::front($pq, \$key, \$pdata)) ||
		    ($key > $current_time)) {
			last;
		} 
		mypqueue::dequeue($pq, \$key, \$pdata);
		handleEvent($current_time, undef, undef, $pdata);
	}
	#
	# handle any input from user.
	#
	if (vec($rout, fileno(STDIN), 1)) {
		handleInput(\$done);
	}
	#
	# handle any msgs from sockey, ie, SIP msgs.
	#
	if (vec($rout, fileno(SOCKET), 1)) {
		while (defined($recvpaddr = recv(SOCKET, my $msg, 8*1024, 0))) {
			handleEvent($current_time, $recvpaddr, $msg, undef);
		}
	}
}
#
close(SOCKET);
if (exists($parameters{logfile})) {
	close(LOGFILE);
}
#
exit 0;

