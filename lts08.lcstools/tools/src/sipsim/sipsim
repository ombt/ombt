#!/opt/exp/bin/perl -w
#
# SIP simulator.
#
########################################################################
#
# path for finding modules
#
push @INC, "/home/lcstools/tools/bin";
#
# modules
#
use Fcntl;
use Storable qw(dclone);
use IO::Socket;
use Getopt::Std;
#
use mypqueue;
#
#########################################################################
#
# global data
#
$debugflag = 0;
$logflag = 0;
#
# any parameter or dndata must have a default value defined below. if not,
# when it is detected, the simulator will report it and exit.
#
%parameters = (
	delta => 10,
	minmsgsize => 10,
	logfile => "",
	myip => "127.0.0.1",
	myport => 5060,
	myrtpip => "127.0.0.1",
	myrtpport => 8000,
	myrtcpport => 8001,
);
#
%dndata = (
	DEFAULT => {
		dn => undef,
		callid => undef,
		expiresform => InExpires,
		expires => 600,
		returncode => 200,
		dnstate => "INACTIVE",
		registrationstate => "INACTIVE",
		originationstate => "INACTIVE",
		terminationstate => "INACTIVE",
		registration => "register",
		registrar => "registrar",
		origination => "origination",
		termination => "termination",
		ipaddress => "127.0.0.1",
		port => "5060",
		fromtag => "from",
		totag => "to",
		branch => "z9hG4bk",
		totagcounter => 0,
		fromtagcounter => 0,
		maxcallduration => 60,
		waitingforackduration => 5,
		waitingfor200duration => 5,
		requesturi => undef,
		waitingfor202duration => 5,
		prerefercallduration => 5,
		referto => undef,
	},
);
#
%dialogs = (
	DEFAULT => {
		dn => undef,
		cseq => undef,
		tag => undef,
		handler => undef,
	},
);
#
use subs qw(printit);
use subs qw(printdebug);
#
########################################################################
#
# utility functions to make msgs
#
sub starttimer {
	my ($did, $endtime, $label) = @_;
	#
	printit "\nstarttimer:\n";
	printit "\tdid=$did\n";
	printit "\tendtime=$endtime\n";
	printit "\tlabel=$label\n";
	#
	my $pnewtimer = ();
	$pnewtimer->{"dialogid"} = $did;
	$pnewtimer->{"tag"} = $endtime;
	$pnewtimer->{"label"} = $label;
	#
	foreach $key (keys %${pnewtimer}) {
		printit "$key = $pnewtimer->{$key}\n";
	}
	#
	mypqueue::enqueue($pq, $endtime, $pnewtimer);
}
#
sub makeRegister200OK {
	my ($dn, $phdrs, $packmsg) = @_;
	#
	$$packmsg = "SIP/2.0 200 OK\r\n";
	$$packmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$$packmsg .= $parameters{myip}."\r\n";
	$$packmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$$packmsg .= "To: ".$phdrs->{"To"}."\r\n";
	$$packmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$$packmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	my $where = ${$dndata{$dn}}{"expiresform"};
	if ($where eq "inexpires") {
		$$packmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
		$$packmsg .= "Expires: ";
		$$packmsg .= $dndata{$dn}{"expires"}."\r\n";
	} else {
		$$packmsg .= "Contact: ".$phdrs->{"Contact"};
		$$packmsg .= ";expires=";
		$$packmsg .= $dndata{$dn}{"expires"}."\r\n";
	}
	$$packmsg .= "Content-Length: 0\r\n";
	$$packmsg .= "\r\n";
}
#
sub makeInvite180Ringing {
	my ($did, $phdrs, $packmsg) = @_;
	#
	my $from = "From: ".$phdrs->{"From"}."\r\n";
	my $callid = "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	my $cseq = "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	my $via = "Via: ".$phdrs->{"Via"}."\r\n";
	my $to = "To: ".$phdrs->{"To"}.";tag=".$dialogs{$did}{totag}."\r\n";
	my $contact = "Contact: ".$dialogs{$did}{dn}."\r\n";
	#
	$$packmsg = "SIP/2.0 180 Ringing\r\n";
	$$packmsg .= $via;
	$$packmsg .= $from;
	$$packmsg .= $to;
	$$packmsg .= $callid;
	$$packmsg .= $cseq;
	$$packmsg .= $contact;
	$$packmsg .= "Content-Length: 0\r\n";
	$$packmsg .= "\r\n";
}
#
sub makeInvite200OK {
	my ($did, $phdrs, $packmsg) = @_;
	#
	my $from = "From: ".$phdrs->{"From"}."\r\n";
	my $callid = "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	my $cseq = "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	my $via = "Via: ".$phdrs->{"Via"}."\r\n";
	my $to = "To: ".$phdrs->{"To"}.";tag=".$dialogs{$did}{totag}."\r\n";
	my $contact = "Contact: ".$dialogs{$did}{dn}."\r\n";
	#
	$$packmsg = "SIP/2.0 200 OK\r\n";
	$$packmsg .= $via;
	$$packmsg .= $from;
	$$packmsg .= $to;
	$$packmsg .= $callid;
	$$packmsg .= $cseq;
	$$packmsg .= $contact;
	#
	my $dn = $dialogs{$did}{dn};
	my $sdp = "";
	makeSDP($dn, \$sdp);
	printit "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$$packmsg .= "Content-Length: $sdplen\r\n";
	$$packmsg .= "\r\n";
	$$packmsg .= "$sdp";
}
#
sub makeBye {
	my ($did, $phdrs, $packmsg) = @_;
	#
	# flip To and From fields in the BYE msg.
	#
	my $from = "From: ".$phdrs->{"To"}."\r\n";
	my $callid = "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	my $cseq = "CSeq: ".$phdrs->{"nextcseq"}." BYE\r\n";
	my $via = "Via: ".$phdrs->{"Via"}."\r\n";
	my $to = "To: ".$phdrs->{"From"}."\r\n";
	my $maxforwards = "Max-Forwards: ".$phdrs->{"Max-Forwards"}."\r\n";
	my $contact = "Contact: ".$dialogs{$did}{dn}."\r\n";
	#
	my $dn = $dialogs{$did}{dn};
	$$packmsg = "BYE ".${dndata{$dn}}{requesturi}." SIP/2.0\r\n";
	$$packmsg .= $via;
	$$packmsg .= $from;
	$$packmsg .= $to;
	$$packmsg .= $callid;
	$$packmsg .= $cseq;
	# $$packmsg .= $contact;
	$$packmsg .= $maxforwards;
	$$packmsg .= "Content-Length: 0\r\n";
	$$packmsg .= "\r\n";
}
#
sub makeRefer {
	my ($did, $phdrs, $packmsg) = @_;
	#
	# flip To and From fields in the REFER msg.
	#
	my $dn = $dialogs{$did}{dn};
	#
	my $from = "From: ".$phdrs->{"To"}."\r\n";
	my $callid = "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	my $cseq = "CSeq: ".$phdrs->{"nextcseq"}." REFER\r\n";
	my $via = "Via: ".$phdrs->{"Via"}."\r\n";
	my $to = "To: ".$phdrs->{"From"}."\r\n";
	my $contact = "Contact: ".${dn}."\r\n";
	my $referto = "Refer-To: ".$dndata{$dn}{referto}."\r\n";
	#
	$$packmsg = "REFER ".${dndata{$dn}}{requesturi}." SIP/2.0\r\n";
	$$packmsg .= $via;
	$$packmsg .= $from;
	$$packmsg .= $to;
	$$packmsg .= $callid;
	$$packmsg .= $cseq;
	$$packmsg .= $referto;
	$$packmsg .= $contact;
	$$packmsg .= "Content-Length: 0\r\n";
	$$packmsg .= "\r\n";
}
#
sub make200OK {
	my ($did, $phdrs, $packmsg) = @_;
	#
	my $from = "From: ".$phdrs->{"From"}."\r\n";
	my $callid = "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	my $cseq = "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	my $via = "Via: ".$phdrs->{"Via"}."\r\n";
	my $to = "To: ".$phdrs->{"To"}."\r\n";
	my $contact = "Contact: ".$dialogs{$did}{dn}."\r\n";
	#
	$$packmsg = "SIP/2.0 200 OK\r\n";
	$$packmsg .= $via;
	$$packmsg .= $from;
	$$packmsg .= $to;
	$$packmsg .= $callid;
	$$packmsg .= $cseq;
	$$packmsg .= $contact;
	$$packmsg .= "Content-Length: 0\r\n";
	$$packmsg .= "\r\n";
}
#
##########################################################################
#
# msg handlers
#
sub register {
	my ($did, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry register: DID = <$did>\n";
	printit "Timer Flag for $did = <$timerflag>\n";
}
#
sub registrar {
	my ($did, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry registrar: DID = <$did>\n";
	printit "Timer Flag for $did = <$timerflag>\n";
	#
	my $regstate = $dndata{$did}{registrationstate};
	#
	if ($regstate =~ /INACTIVE/) {
		# first time to register
		if ($timerflag) {
			printit "\nregistrar regstate=INACTIVE warning:\n";
			printit "Unexpected timer.\n";
			printit "(did=$did).\n";
			#
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /REGISTER/) {
			my $ackmsg = undef;
			makeRegister200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			#
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + $dndata{$did}{"expires"};
			starttimer($did, $endtime, "(1) REGISTRATION TIME OUT");
			#
			$dialogs{$did}{dn} = $did;
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "registrar";
			#
			my $cseq = $phdrs->{"CSeq"};
			$cseq =~ /^[\s]*([0-9]+)[\s]+/;
			$dialogs{$did}{nextcseq} = $1 + 1;
			#
			# $dndata{$did}{registrationstate} = "REGISTER";
			changeDnState($did, "registrationstate", "REGISTER");
		} else {
			printit "\nregistrar INACTIVE error:\n";
			printit "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			printit "(did=$did).\n";
		}
		return;
	} 
	#
	# now treat cases after the first registration.
	#
	if ($timerflag) {
		my $dialogtag = $dialogs{$did}{"tag"};
		my $timertag = $ptimer->{"tag"};
		if ($dialogtag != $timertag) {
			printit "\nregistrar REGISTER warning:\n";
			printit "Ignoring old timer (tag=$timertag).\n";
			printit "(did=$did).\n";
			$ptimer = undef;
			return;
		}
		#
		printit "\nregistrar REGISTER warning:\n";
		printit "Registration timer expired. (tag=$timertag).\n";
		printit "(did=$did).\n";
		#
		$ptimer = undef;
		$dialogs{$did}{"tag"} = 0;
		$dialogs{$did}{handler} = "registrar";
		# $dndata{$did}{registrationstate} = "UNREGISTER";
		changeDnState($did, "registrationstate", "UNREGISTER");
	} elsif ($phdrs->{msgtype} =~ /^REGISTER/) {
		my $ackmsg = undef;
		makeRegister200OK($did, $phdrs, \$ackmsg);
		dumpOutgoingMsg($recvpaddr, $ackmsg);
		#
		defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
			die "send: $!";
		#
		my $endtime = time() + $dndata{$did}{"expires"};
		starttimer($did, $endtime, "(2) REGISTRATION TIME OUT");
		#
		$dialogs{$did}{dn} = $did;
		$dialogs{$did}{tag} = $endtime;
		$dialogs{$did}{handler} = "registrar";
		#
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$dialogs{$did}{nextcseq} = $1 + 1;
		#
		# $dndata{$did}{registrationstate} = "REGISTER";
		changeDnState($did, "registrationstate", "REGISTER");
	} else {
		printit "\nregistrar REGISTER warning:\n";
		printit "Unexpected msg $phdrs->{msgtype}.\n";
		printit "(did=$did).\n";
	}
	return;
}
#
sub origination {
	my ($did, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry origination: DID = <$did>\n";
	printit "Timer Flag for $did = <$timerflag>\n";
	#
}
#
sub termination {
	my ($did, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry termination: DID = <$did>\n";
	printit "Timer Flag for $did = <$timerflag>\n";
	#
	# if the msgtype is INVITE, then the dialog id is a
	# dn and we have to look in dndata to get the state.
	# if this is the first INVITE, then the state is
	# is INACTIVE and we need to create a dialog. however,
	# if the state in dndata is NOT INACTIVE, then we 
	# we have a call going already and the INVITE is 
	# is a second or more INVITE. we will ignore these
	# msgs for now. also, if the msg is NOT an INVITE
	# then the dialog id is real and we need to look
	# in dialogs to get the dn.
	#
	if (!$timerflag && $phdrs->{msgtype} =~ /INVITE/) {
		# dialog id is dn.
		my $termstate = $dndata{$did}{terminationstate};
		if (!($termstate =~ /INACTIVE/)) {
			printit "\ntermination dnstate=$termstate warning:\n";
			printit "unexpected INVITE. ignoring it.\n";
			printit "(did=$did).\n";
			return;
		}
		#
		# we have a first INVITE. process it. construct
		# the dialog id for subsequent msgs in this session.
		#
		my $totag = $dndata{$did}{totag};
		my $fromtag = $phdrs->{"From"};
		$fromtag =~ /^.*;tag=(.*)$/;
		$fromtag = $1;
		my $callid = $phdrs->{"Call-ID"};
		my $dialogid = $totag.$callid.$fromtag;
		$phdrs->{"dialogid"} = $dialogid;
		#
		$dialogs{$dialogid}{dn} = $did;
		$dialogs{$dialogid}{totag} = $totag;
		$dialogs{$dialogid}{fromtag} = $fromtag;
		$dialogs{$dialogid}{callid} = $callid;
		$dialogs{$dialogid}{recvpaddr} = dclone(\$recvpaddr);
		savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
				$phdrs, \$recvpaddr);
		#
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$dialogs{$dialogid}{nextcseq} = $1 + 1;
		#
		my $ackmsg = undef;
		makeInvite180Ringing($dialogid, $phdrs, \$ackmsg);
		dumpOutgoingMsg($recvpaddr, $ackmsg);
		defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
			die "send: $!";
		#
		$ackmsg = undef;
		makeInvite200OK($dialogid, $phdrs, \$ackmsg);
		dumpOutgoingMsg($recvpaddr, $ackmsg);
		defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
			die "send: $!";
		#
		my $endtime = time() + $dndata{$did}{"waitingforackduration"};
		starttimer($dialogid, $endtime, "INVITE WAIT FOR ACK");
		#
		$dialogs{$dialogid}{tag} = $endtime;
		$dialogs{$dialogid}{handler} = "termination";
		# $dndata{$did}{terminationstate} = "WAITFORACK";
		changeDnState($did, "terminationstate", "WAITFORACK");
		return;
	}
	#
	# we have a real dialog id now.
	#
	if (!defined($dialogs{$did})) {
		printit "\ntermination - no dialog data for $did defined.\n";
		return;
	}
	#
	my $dn = $dialogs{$did}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	#
	if ($termstate =~ /WAITFORACK/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination WAITFORACK warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
					$phdrs, \$recvpaddr);
			
			my $endtime = time() + $dndata{$dn}{"maxcallduration"};
			starttimer($did, $endtime, "TALKING MAX CALL LENGTH");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "termination";
			# $dndata{$dn}{terminationstate} = "TALKING";
			changeDnState($dn, "terminationstate", "TALKING");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFORACK warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /TALKING/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination TALKING warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			my $ackmsg = undef;
			$phdrs = $dialogs{$did}{lastmsg}{hdrs};
			makeBye($did, $phdrs, \$ackmsg);
			# remember, timers have NO recvpaddr.
			my $precvpaddr = $dialogs{$did}{lastmsg}{recvpaddr};
			dumpOutgoingMsg($$precvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $$precvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + 
				$dndata{$dn}{"waitingfor200duration"};
			starttimer($did, $endtime, "BYE WAIT FOR 200 OK");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "termination";
			# $dndata{$dn}{terminationstate} = "WAITFOR200BYE";
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination WAITFOR200BYE warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFOR200BYE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} else {
		printit "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(did=$did).\n";
	}
}
#
sub changeDnState {
	my ($dn, $dnstate, $newstate) = @_;
	printit("OLD $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
	$dndata{$dn}{$dnstate} = $newstate;
	printit("NEW $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
sub dumpDnState {
	my ($dn, $dnstate) = @_;
	printit("CURRENT $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
#
sub terminationrefer {
	my ($did, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry terminationrefer: DID = <$did>\n";
	printit "Timer Flag for $did = <$timerflag>\n";
	#
	# if the msgtype is INVITE, then the dialog id is a
	# dn and we have to look in dndata to get the state.
	# if this is the first INVITE, then the state is
	# is INACTIVE and we need to create a dialog. however,
	# if the state in dndata is NOT INACTIVE, then we 
	# we have a call going already and the INVITE is 
	# is a second or more INVITE. we will ignore these
	# msgs for now. also, if the msg is NOT an INVITE
	# then the dialog id is real and we need to look
	# in dialogs to get the dn.
	#
	if (!$timerflag && $phdrs->{msgtype} =~ /INVITE/) {
		# dialog id is dn.
		my $termstate = $dndata{$did}{terminationstate};
		dumpDnState($did, "terminationstate");
		if (!($termstate =~ /INACTIVE/)) {
			printit "\ntermination dnstate=$termstate warning:\n";
			printit "unexpected INVITE. ignoring it.\n";
			printit "(did=$did).\n";
			return;
		}
		#
		# we have a first INVITE. process it. construct
		# the dialog id for subsequent msgs in this session.
		#
		my $totag = $dndata{$did}{totag};
		my $fromtag = $phdrs->{"From"};
		$fromtag =~ /^.*;tag=(.*)$/;
		$fromtag = $1;
		my $callid = $phdrs->{"Call-ID"};
		my $dialogid = $totag.$callid.$fromtag;
		$phdrs->{"dialogid"} = $dialogid;
		#
		$dialogs{$dialogid}{dn} = $did;
		$dialogs{$dialogid}{totag} = $totag;
		$dialogs{$dialogid}{fromtag} = $fromtag;
		$dialogs{$dialogid}{callid} = $callid;
		$dialogs{$dialogid}{recvpaddr} = dclone(\$recvpaddr);
		savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
				$phdrs, \$recvpaddr);
		#
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$dialogs{$dialogid}{nextcseq} = $1 + 1;
		#
		my $ackmsg = undef;
		makeInvite180Ringing($dialogid, $phdrs, \$ackmsg);
		dumpOutgoingMsg($recvpaddr, $ackmsg);
		defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
			die "send: $!";
		#
		makeInvite200OK($dialogid, $phdrs, \$ackmsg);
		dumpOutgoingMsg($recvpaddr, $ackmsg);
		defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
			die "send: $!";
		#
		my $endtime = time() + $dndata{$did}{"waitingforackduration"};
		starttimer($dialogid, $endtime, "INVITE WAIT FOR ACK");
		#
		$dialogs{$dialogid}{tag} = $endtime;
		$dialogs{$dialogid}{handler} = "terminationrefer";
		# $dndata{$did}{terminationstate} = "WAITFORACK";
		changeDnState($did, "terminationstate", "WAITFORACK");
		return;
	}
	#
	# we have a real dialog id now.
	#
	if (!defined($dialogs{$did})) {
		printit "\ntermination - no dialog data for $did defined.\n";
		return;
	}
	#
	my $dn = $dialogs{$did}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /WAITFORACK/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination WAITFORACK warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
					$phdrs, \$recvpaddr);
			
			my $endtime = time() + $dndata{$dn}{"prerefercallduration"};
			starttimer($did, $endtime, "TALKING PRE-REFER CALL LENGTH");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "terminationrefer";
			# $dndata{$dn}{terminationstate} = "TALKINGBEFOREREFER";
			changeDnState($dn, "terminationstate", "TALKINGBEFOREREFER");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFORACK warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /TALKINGBEFOREREFER/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination TALKINGBEFOREREFER warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# first part of call is over, send refer.
			$phdrs = $dialogs{$did}{lastmsg}{hdrs};
			#
			my $ackmsg = undef;
			makeRefer($did, $phdrs, \$ackmsg);
			# remember, timers have NO recvpaddr.
			my $precvpaddr = $dialogs{$did}{lastmsg}{recvpaddr};
			dumpOutgoingMsg($$precvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $$precvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + 
				$dndata{$dn}{"waitingfor202duration"};
			starttimer($did, $endtime, "REFER WAIT FOR 202 OK");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "terminationrefer";
			# $dndata{$dn}{terminationstate} = "WAITFOR202REFER";
			changeDnState($dn, "terminationstate", "WAITFOR202REFER");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /WAITFOR202REFER/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination WAITFOR202REFER warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			my $ackmsg = undef;
			$phdrs = $dialogs{$did}{lastmsg}{hdrs};
			$dialogs{$did}{nextcseq} += 1;
			$phdrs->{"nextcseq"} = $dialogs{$did}{nextcseq};
			makeBye($did, $phdrs, \$ackmsg);
			# remember, timers have NO recvpaddr.
			my $precvpaddr = $dialogs{$did}{lastmsg}{recvpaddr};
			dumpOutgoingMsg($$precvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $$precvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + 
				$dndata{$dn}{"waitingfor200duration"};
			starttimer($did, $endtime, "BYE WAIT FOR 200 OK");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "terminationrefer";
			# $dndata{$dn}{terminationstate} = "WAITFOR200BYE";
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
					$phdrs, \$recvpaddr);
			#
			my $endtime = time() + $dndata{$dn}{"maxcallduration"};
			starttimer($did, $endtime, "TALKING CALL LENGTH");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "terminationrefer";
			# $dndata{$dn}{terminationstate} = "TALKING";
			changeDnState($dn, "terminationstate", "TALKING");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFOR202REFER warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /TALKING/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination TALKING warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			my $ackmsg = undef;
			$phdrs = $dialogs{$did}{lastmsg}{hdrs};
			$dialogs{$did}{nextcseq} += 1;
			$phdrs->{"nextcseq"} = $dialogs{$did}{nextcseq};
			makeBye($did, $phdrs, \$ackmsg);
			# remember, timers have NO recvpaddr.
			my $precvpaddr = $dialogs{$did}{lastmsg}{recvpaddr};
			dumpOutgoingMsg($$precvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $$precvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + 
				$dndata{$dn}{"waitingfor200duration"};
			starttimer($did, $endtime, "BYE WAIT FOR 200 OK");
			#
			$dialogs{$did}{tag} = $endtime;
			$dialogs{$did}{handler} = "terminationrefer";
			# $dndata{$dn}{terminationstate} = "WAITFOR200BYE";
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			#
			# respond to each NOTIFY, but don't change 
			# state. also, there already is a timer, so we
			# don't need start another timer.
			#
			savelastmsg($did, \$rawmsg, \$rawhdr, \$rawdata, 
					$phdrs, \$recvpaddr);
			#
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			my $dialogtag = $dialogs{$did}{"tag"};
			my $timertag = $ptimer->{"tag"};
			if ($dialogtag != $timertag) {
				printit "\ntermination WAITFOR200BYE warning:\n";
				printit "not my timer (tag=$timertag).\n";
				printit "(did=$did).\n";
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			my $ackmsg = undef;
			make200OK($did, $phdrs, \$ackmsg);
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SIP, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			# call ends.
			delete $dialogs{$did};
			# $dndata{$dn}{terminationstate} = "INACTIVE";
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFOR200BYE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(did=$did).\n";
		}
	} else {
		printit "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(did=$did).\n";
	}
}
#
########################################################################
#
# functions
#
sub usage {
	printit "\n";
	printit "usage: sipsim [-?x] [-l logfile] [configfile]\n";
	printit "\n";
	printit "if a configfile is not given, then the default config\n";
	printit "file 'sipsimcfg' is read from the current directory.\n";
}
#
sub savelastmsg {
	my ($did, $prawmsg, $prawhdr, $prawdata, $phdrs, $precvpaddr) = @_;
	#
	$dialogs{$did}{lastmsg} = undef;
	#
	if (defined($prawmsg)) {
		$dialogs{$did}{lastmsg}{rawmsg} = dclone($prawmsg);
	} else {
		$dialogs{$did}{lastmsg}{rawmsg} = undef;
	}
	if (defined($prawhdr)) {
		$dialogs{$did}{lastmsg}{rawhdr} = dclone($prawhdr);
	} else {
		$dialogs{$did}{lastmsg}{rawhdr} = undef;
	}
	if (defined($prawdata)) {
		$dialogs{$did}{lastmsg}{rawdata} = dclone($prawdata);
	} else {
		$dialogs{$did}{lastmsg}{rawdata} = undef;
	}
	if (defined($phdrs)) {
		$dialogs{$did}{lastmsg}{hdrs} = dclone($phdrs);
	} else {
		$dialogs{$did}{lastmsg}{hdrs} = undef;
	}
	if (defined($precvpaddr)) {
		$dialogs{$did}{lastmsg}{recvpaddr} = dclone($precvpaddr);
	} else {
		$dialogs{$did}{lastmsg}{recvpaddr} = undef;
	}
}
#
sub printdebug {
	my ($msg) = @_;
	if ($debugflag) {
		if ($logflag) {
			print LOGFILE ${msg};
		}
		print STDERR ${msg};
	}
}
sub printit {
	my ($msg) = @_;
	#
	if ($logflag) {
		print LOGFILE ${msg};
	}
	print STDERR ${msg};
}
#
sub dumpdndata {
	printit "\n";
	printit "Dumping DN-Data structure:\n";
	foreach $key (sort keys %dndata) {
		printit "dn = $key\n";
		foreach $key2 (sort keys %{$dndata{$key}}) {
			printit "\t$key2 = ";
			if (defined($dndata{$key}{$key2})) {
				printit "$dndata{$key}{$key2}\n";
			} else {
				printit "====>>>> UNDEFINED <<<<====\n";
			}
		}
	}
}
#
sub dumpparams {
	printit "\n";
	printit "Parameters:\n";
	foreach $param (sort keys %parameters) {
		printit "\t$param = $parameters{$param}\n";
	}
}
#
sub readconfigfile {
	my ($configfile) = @_;
	#
	my $nodefaults = 0;
	my $syntaxerrors = 0;
	my $dn = "UNKNOWNDN";
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
				printit "\n";
				printit "skipping record <$crec>.\n";
				printit "not in a 'parameter key value' format.\n";
				$syntaxerrors += 1;
				next;
			}
			if (!exists($parameters{$parameter}))
			{
				$nodefaults += 1;
				printit "\n";
				printit "parameter $parameter has NO default value.\n";
			}
			$parameters{$parameter} = $value;
			next;
		}
		# get name-value pair
		my ($key, $value) = split /\s*=\s*/, $crec, 2;
		if (!defined($key) || !defined($value)) {
			printit "\n";
			printit "skipping record <$crec>.\n";
			printit "not in a 'key=value' format.\n";
			$syntaxerrors += 1;
			next;
		}
		#
		if ($key =~ /^dn/) {
			$dn = $value;
			$dndata{$dn} = dclone($dndata{DEFAULT});
		} else {
			if (!exists($dndata{DEFAULT}{$key}))
			{
				$nodefaults += 1;
				printit "\n";
				printit "field $key has NO default value.\n";
			}
			$dndata{$dn}{$key} = $value;
		}
	}
	close(CONFIGFILE);
	#
	if ($nodefaults) {
		printit "\n";
		printit "$nodefaults field(s) found with NO defaults.\n";
	}
	if ($syntaxerrors) {
		printit "\n";
		printit "$syntaxerrors syntax error(s) found in config file.\n";
	}
	if ($nodefaults || $syntaxerrors) {
		printit "exiting.\n";
		exit 2;
	}
	#
}
#
sub dumpSDP {
	my ($psdp) = @_;
	#
	printit "\n";
	printit "SDP CONTENTS: \n";
	foreach $line (@{${psdp}}) {
		printit "$line\n";
	}
}
#
sub makeSDP {
	my ($dn, $psdp) = @_;
	#
	$$psdp = "";
	$$psdp .= "v=0\r";
	$$psdp .= "o=$dn 0 $parameters{myrtpport} IN IP4 $parameters{myrtpip}\r";
	$$psdp .= "s=-\r";
	$$psdp .= "c=IN IP4 $parameters{myip}\r";
	$$psdp .= "t=0 0\r";
	$$psdp .= "m=audio $parameters{myrtpport} RTP/AVP 0\r";
	$$psdp .= "a-rtpmap:0 pcmu/8000\r";
}
#
sub parseMsg {
	my ($rawmsg, $prawhdr, $prawdata, $phdrs) = @_;
	#
	# separate the hdr from the payload.
	#
	($$prawhdr, $$prawdata) = split /\r\n\r\n/, $rawmsg, 2;
	return unless (defined($$prawhdr));
	#
	# split fields in hdrs. remember there can
	# be multiple entries with the same name. this
	# way things do get overwritten if the field
	# has the same name, for example, the via field. 
	# remember to fix this later if it becomes an issue.
	#
	my @hdrs = split /[\r\n]+/, $$prawhdr;
	$phdrs->{firstline} = shift @hdrs;
	my @dummy = split /\s+/, $phdrs->{firstline};
	$phdrs->{msgtype} = shift @dummy;
	if ($phdrs->{msgtype} =~ /^SIP/) {
		$phdrs->{msgtype} = "RETURNCODE";
		$phdrs->{returncode} = shift @dummy;
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
	if ($phdrs->{msgtype} =~ /REGISTER/) {
		my $dn = $phdrs->{"From"};
		$dn =~ /^[^<]*<([^>]*)>.*$/;
		$phdrs->{"dn"} = $1;
		$phdrs->{"dialogid"} = undef;
		$phdrs->{"dialogid2"} = undef;
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$phdrs->{"nextcseq"} = $1 + 1;
	} elsif ($phdrs->{msgtype} =~ /INVITE/) {
		my $dn = $phdrs->{"To"};
		$dn =~ /^[^<]*<([^>]*)>.*$/;
		$phdrs->{"dn"} = $1;
		$phdrs->{"dialogid2"} = undef;
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$phdrs->{"nextcseq"} = $1 + 1;
	} else {
		# construct a dialog id.
		$phdrs->{"dn"} = "NONE";
		my $fromtag = $phdrs->{"From"};
		$fromtag =~ /^.*;tag=(.*)$/;
		$fromtag = $1;
		my $totag = $phdrs->{"To"};
		$totag =~ /^.*;tag=(.*)$/;
		$totag = $1;
		my $callid = $phdrs->{"Call-ID"};
		my $dialogid = $totag.$callid.$fromtag;
		$phdrs->{"dialogid"} = $dialogid;
		$dialogid = $fromtag.$callid.$totag;
		$phdrs->{"dialogid2"} = $dialogid;
		my $cseq = $phdrs->{"CSeq"};
		$cseq =~ /^[\s]*([0-9]+)[\s]+/;
		$phdrs->{"nextcseq"} = $1 + 1;
	}
	# check if any sdp payload
	if (!exists($phdrs->{"Content-Type"})) {
		# no sdp
		$phdrs->{"SDP"} = undef;
		return;
	}
	# we have sdp
	@{$phdrs->{"SDP"}} = split /[\r\n]+/, $$prawdata;
	dumpSDP($phdrs->{"SDP"});
	return;
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
	printit "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< incoming msg\n";
	printit "FROM IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		printit "line=<$line>\n";
	}
	printit "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< incoming msg\n";
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
	printit ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
	printit "TO IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		printit "line=<$line>\n";
	}
	printit ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
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
	my ($pdone) = @_;
	#
	my $line = <STDIN>;
	if (!defined($line)) {
		return;
	}
	chomp($line);
	#
	if ($line =~ /^q/i) {
		printit "\nQuitting ...\n";
		$$pdone = 1;
	} elsif ($line =~ /^w /i) {
		printit "\n$line\n";
	} elsif ($line =~ /^l[\s]+([^\s]+)/i) {
		my $listdn = $1;
		if (!exists($dndata{$listdn})) {
			printit "DN $listdn not found.\n";
			return;
		}
		printit "\nListing DN $listdn data:\n";
		foreach my $key (keys %{$dndata{$listdn}}) {
			if (defined($dndata{$listdn}{$key})) {
				printit "$key => $dndata{$listdn}{$key}\n";
			} else {
				printit "\$dndata{$listdn}{$key} undefined\n";
			}
		}
	} elsif ($line =~ /^l[\s]*$/i) {
		printit "\nListing DNs:\n";
		foreach my $dn (keys %dndata) {
			printit "DN $dn\n";
			printit "\tregistrationstate => $dndata{$dn}{registrationstate}\n";
			printit "\toriginationstate => $dndata{$dn}{originationstate}\n";
			printit "\tterminationstate => $dndata{$dn}{terminationstate}\n";
		}
	} elsif ($line =~ /^d[\s]*$/i) {
		printit "\nListing Dialogs:\n";
		foreach my $did (keys %dialogs) {
			printit "DID $did\n";
		}
	} elsif ($line =~ /^r[\s]+([^\s]+)/i) {
		my $regdn = $1;
		printit "\nRegistering URI $regdn.\n";
	} elsif ($line =~ /^o[\s]+([^\s]+)[\s]+([^\s]+)/i) {
		my $origdn = $1;
		my $termdn = $2;
		printit "\nURI $origdn calling URI $termdn.\n";
	} elsif ($line =~ /^\?/) {
		printit "\nhelp text:\n";
		printit "\t'?' - help text\n";
		printit "\t'q' - quit\n";
		printit "\t'w text' - write 'text' to stdout/logfile.\n";
		printit "\t'l' - list DNs.\n";
		printit "\t'd' - list active dialogs.\n";
		printit "\t'r dn' - register dn.\n";
		printit "\t'o dn dialdn' - dn calls dialdn.\n";
	} else {
		printit "\nunknown cmd\n";
	}
	return;
}
#
sub handleRTP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	my $msglen = length($rawmsg);
	printit "RTP msg received: length=$msglen\n";
}
#
sub handleRTCP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	my $msglen = length($rawmsg);
	printit "RTCP msg received: length=$msglen\n";
}
#
sub handleEvent {
	my ($current_time, $recvpaddr, $rawmsg, $ptimer) = @_;
	#
	if (defined($ptimer)) {
		my $handler = undef;
		my $timertype = 0;
		my $did = undef;
		# check if we have an origination or registration
		if ($ptimer->{"label"} =~ /CALLORIGINATION/) {
			if (!defined($ptimer->{"dialogid"})) {
				printit "\nNo Dialog ID defined in origination.\n";
				$ptimer = undef;
				return;
			}
			# call origination event
			$timertype = 2;
			$did = $ptimer->{"dialogid"};
			$handler = $dndata{$did}{origination};
		} elsif ($ptimer->{"label"} =~ /ENDPOINTREGISTRATION/) {
			if (!defined($ptimer->{"dialogid"})) {
				printit "\nNo Dialog ID defined in end-point-registration.\n";
				$ptimer = undef;
				return;
			}
			# end-point registration event
			$timertype = 3;
			$did = $ptimer->{"dialogid"};
			$handler = $dndata{$did}{registration};
		} else {
			# we have a regular timer
			if (!defined($ptimer->{"dialogid"})) {
				printit "\nNo Dialog ID defined in timer.\n";
				$ptimer = undef;
				return;
			}
			$did = $ptimer->{"dialogid"};
			if (!defined($dialogs{$did})) {
				printit "\nNo timer dialog data for $did defined.\n";
				$ptimer = undef;
				return;
			}
			# regular timer
			$timertype = 1;
			$handler = $dialogs{$did}{handler};
		}
		#
		&{${handler}}($did, $timertype, $current_time, 
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
		#
		dumpHdrs(\%hdrs);
		#
		# at this point we either have a dialog or we 
		# don't. REGISTER and INVITE msgs are the first
		# msgs in a dialog, so no dialog ID exists. we 
		# need to call the appropriate handler and let 
		# handler create the dialog entry. then, all
		# subsequent msgs will have a dialog id and
		# will be handled using the dialog ID.
		#
		if ($hdrs{msgtype} =~ /REGISTER/) {
			if (!defined($hdrs{"dn"})) {
				printit "\nNo 'dn' defined in REGISTER hdrs.\n";
				return;
			}
			my $dn = $hdrs{"dn"};
			if (!defined($dndata{$dn})) {
				printit "\nNo dndata for $dn defined.\n";
				return;
			}
			if (!defined($dndata{$dn}{registrar}))
			{
				printit "\nregistrar undefined for $dn.\n";
				exit 2;
			}
			my $handler = $dndata{$dn}{registrar};
			&{${handler}}($dn, 0, $current_time, $recvpaddr, 
				$rawmsg, $rawhdr, $rawdata, \%hdrs, undef);
		} elsif ($hdrs{msgtype} =~ /INVITE/) {
			if (!defined($hdrs{"dn"})) {
				printit "\nNo 'dn' defined in INVITE hdrs.\n";
				return;
			}
			my $dn = $hdrs{"dn"};
			if (!defined($dndata{$dn})) {
				printit "\nNo dndata for $dn defined.\n";
				return;
			}
			if (!defined($dndata{$dn}{termination}))
			{
				printit "\ntermination undefined for $dn.\n";
				exit 2;
			}
			my $handler = $dndata{$dn}{termination};
			&{${handler}}($dn, 0, $current_time, $recvpaddr, 
				$rawmsg, $rawhdr, $rawdata, \%hdrs, undef);
		} else {
			my $did = undef;
			if (defined($hdrs{"dialogid"}) && 
			    defined($dialogs{$hdrs{"dialogid"}})) {
				$did = $hdrs{"dialogid"};
				$hdrs{"dialogid2"} = $hdrs{"dialogid"};
			} elsif (defined($hdrs{"dialogid2"}) && 
			    defined($dialogs{$hdrs{"dialogid2"}})) {
				$did = $hdrs{"dialogid2"};
				$hdrs{"dialogid"} = $hdrs{"dialogid2"};
			} else {
				printit "\nNo dialog data found.\n";
				printit "DialogId = $hdrs{dialogid}\n";;
				printit "DialogId2 = $hdrs{dialogid2}\n";;
				return;
			}
			if (!defined($dialogs{$did}{handler})) {
				printit "\nNo handler for $did defined.\n";
				exit 2;
			}
			my $handler = $dialogs{$did}{handler};
			&{${handler}}($did, 0, $current_time, $recvpaddr, 
				$rawmsg, $rawhdr, $rawdata, \%hdrs, undef);
		}
	}
	return;
}
#
#############################################################################
#
printit "\nSIP Simulator:\n";
#
# get cmd line options
#
my %opts;
getopts('?xl:', \%opts);
foreach my $opt (%opts) {
	if ($opt eq "l") {
		$parameters{logfile} = $opts{$opt};
		printit "==>> logfile = $parameters{logfile}\n";
		open(LOGFILE, ">".$parameters{logfile}) or 
			die "can't open $parameters{logfile}: $!";
		LOGFILE->autoflush(1);
		$logflag = 1;
	} elsif ($opt eq "x") {
		$debugflag = 1;
		printit "==>> debugflag = $debugflag\n";
	} elsif ($opt eq "?") {
		usage();
		exit 0;
	}
}
#
# read in config file
#
my ($configfile) = @ARGV;
if (!defined($configfile)) {
	$configfile = "sipsimcfg";
}
readconfigfile($configfile);
dumpparams();
dumpdndata();
#
# set up socket connection
#
my $proto = getprotobyname('udp');
defined($proto) or die "getprotobyname: $!";
socket(SIP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
socket(RTP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
socket(RTCP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
#
my $ipaddr = gethostbyname($parameters{myip});
defined($ipaddr) or die "gethostbyname: $!";
#
my $paddr = sockaddr_in($parameters{myport}, $ipaddr);
defined($paddr) or die "getprotobyname: $!";
my $rtp_paddr = sockaddr_in($parameters{myrtpport}, $ipaddr);
defined($rtp_paddr) or die "getprotobyname: $!";
my $rtcp_paddr = sockaddr_in($parameters{myrtcpport}, $ipaddr);
defined($rtcp_paddr) or die "getprotobyname: $!";
#
bind(SIP, $paddr) or die "bind: $!";
bind(RTP, $rtp_paddr) or die "bind: $!";
bind(RTCP, $rtcp_paddr) or die "bind: $!";
#
my $rin = '';
my $rout = '';
#
vec($rin, fileno(STDIN), 1) = 1;
vec($rin, fileno(SIP), 1) = 1;
vec($rin, fileno(RTP), 1) = 1;
vec($rin, fileno(RTCP), 1) = 1;
#
# we don't want to I/O block, if no input, just return.
#
fcntl(STDIN, F_SETFL, O_NONBLOCK);
fcntl(SIP, F_SETFL, O_NONBLOCK);
fcntl(RTP, F_SETFL, O_NONBLOCK);
fcntl(RTCP, F_SETFL, O_NONBLOCK);
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
# event loop for simulation. 
#
my $mydelta = 1;
my $key;
my $pdata;
#
while ( ! $done) {
	$previous_time = $current_time;
	#
	# determine how long to sleep.
	#
	if (mypqueue::isEmpty($pq)) {
		# no timers, just use default value.
		$mydelta = $parameters{delta};
		printdebug "DEFAULT DELTA = $mydelta ...\n";
	} elsif (mypqueue::front($pq, \$key, \$pdata) == 1) {
		$mydelta = $key - $current_time;
		# if ($mydelta < 0) $mydelta = 0;
		printdebug "CALCULATED DELTA = $mydelta ...\n";
	} else {
		printdebug "ERROR CALCULATING DELTA !!!\n";
		exit 2;
	}
	#
	# wait for input or timer to expire.
	#
	my $nf = select($rout=$rin, undef, undef, $mydelta);
	#
	# update internal clocks
	#
	$current_time = time();
	$elapsed_time = $current_time-$previous_time;
	$total_elapsed_time= $current_time-$start_time;
	#
	printdebug "\n";
	printdebug "delta   = $mydelta\n";
	printdebug "current = $current_time\n";
	printdebug "elapsed = $elapsed_time\n";
	printdebug "telapsed= $total_elapsed_time\n";
	#
	# handle any expired timers
	#
	while (!mypqueue::isEmpty($pq)) {
		if ((mypqueue::front($pq, \$key, \$pdata) == 0) ||
		    ($key > $current_time)) {
			printdebug "last (key, current_time)=($key,$current_time)\n";
			last;
		} 
		printdebug "found one (key, current_time)=($key,$current_time)\n";
		mypqueue::dequeue($pq, \$key, \$pdata);
		#
		printdebug "\nexpired timer:\n";
		printdebug "\tdialogid = $pdata->{dialogid}\n";
		printdebug "\ttag      = $pdata->{tag}\n";
		handleEvent($current_time, undef, undef, $pdata);
	}
	#
	# handle any input from user.
	#
	if (vec($rout, fileno(STDIN), 1)) {
		handleInput(\$done);
	}
	#
	# handle any msgs from socket, ie, SIP msgs.
	#
	if (vec($rout, fileno(SIP), 1)) {
		while (defined($recvpaddr = recv(SIP, my $msg, 8*1024, 0))) {
			handleEvent($current_time, $recvpaddr, $msg, undef);
		}
	}
	if (vec($rout, fileno(RTP), 1)) {
		while (defined($recvpaddr = recv(RTP, my $msg, 8*1024, 0))) {
			handleRTP($current_time, $recvpaddr, $msg);
		}
	}
	if (vec($rout, fileno(RTCP), 1)) {
		while (defined($recvpaddr = recv(RTCP, my $msg, 8*1024, 0))) {
			handleRTCP($current_time, $recvpaddr, $msg);
		}
	}
}
#
close(SIP);
if (exists($parameters{logfile})) {
	close(LOGFILE);
}
#
exit 0;

