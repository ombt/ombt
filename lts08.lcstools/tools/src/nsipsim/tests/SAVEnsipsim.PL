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
	branch => "z9hG4bk",
	delta => 10,
	minmsgsize => 10,
	logfile => "",
	mysipip => "127.0.0.1",
	mysipport => 5060,
	myrtpip => "127.0.0.1",
	myrtpport => 8000,
	myrtcpport => 8001,
	maxforwards => 70,
);
#
%dn2data = (
	# nothing here
);
#
%dndata = (
	DEFAULT => {
		dn => undef,
		dntype => "individual",
		mindn => 0,
		maxdn => 0,
		dnstate => "INACTIVE",
		registrationstate => "INACTIVE",
		originationstate => "INACTIVE",
		terminationstate => "INACTIVE",
		registrar => "registrar",
		registration => "register",
		origination => "origination",
		termination => "termination",
		ipaddress => "127.0.0.1",
		sipip => "127.0.0.1",
		sipport => 5060,
		rtpip => "127.0.0.1",
		rtpport => 8000,
		rtcpip => "127.0.0.1",
		rtcpport => 8001,
		expires => 600,
		interregistertime => 120,
		maxcallduration => 60,
		waitingforackduration => 5,
		waitingfor200duration => 5,
		waitingfor202duration => 5,
		prerefercallduration => 5,
	},
);
#
%dialogs = (
	DEFAULT => {
		dn => undef,
		cseq => undef,
		timertag => undef,
		handler => undef,
		contact => undef,
	},
);
#
use subs qw(printit);
use subs qw(dprint);
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
#
sub dprint {
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
sub dumphdrs {
	my ($phdrs) = @_;
	printit "\nHeader Fields:\n";
	foreach $key (sort keys %{$phdrs}) {
		if (defined($phdrs->{$key})) {
			printit "$key = $phdrs->{$key}\n";
		} else {
			printit "$key NOT DEFINED.\n";
		}
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
			$dndata{$dn}{dn} = $dn;
			$dndata{$dn}{dntype} = "individual";
		} elsif ($key =~ /^mindn/) {
			$dn = $value;
			$dndata{$dn} = dclone($dndata{DEFAULT});
			$dndata{$dn}{dn} = $dn;
			$dndata{$dn}{mindn} = $dn;
			$dndata{$dn}{dntype} = "range";
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
	# initialize mapping of DNs to DNs in dndata table. remember, ranges
	# will map only to one DN in dndata.
	#
	foreach $dn (keys %dndata) {
		if ($dndata{$dn}{dntype} eq "individual") {
			$dn2data{$dn} = $dn;
		} elsif ($dndata{$dn}{"dntype"} eq "range") {
			my $mindn = $dndata{$dn}{"mindn"};
			my $maxdn = $dndata{$dn}{"maxdn"};
			if ($mindn < 0 || $maxdn < 0) {
				printit "readconfigfile: min DN or max DN < 0.\n";
				exit 2;
			}
			if ($mindn > $maxdn) {
				printit "readconfigfile: min DN > max DN.\n";
				exit 2;
			}
			for (my $dn2 = $mindn; $dn2 <= $maxdn; $dn2 += 1) {
				$dn2data{$dn2} = "$dn";
			}
		} else {
			printit "readconfigfile: invalid DN type - $dndata{$dn}{dntype}.\n";
			exit 2;
		}
	}
	#
	return;
}
#
sub dumpfld {
	my ($fldname, $phdrs) = @_;
	my $maxi = $#{$phdrs->{$fldname}};
	for (my $i = 0; $i <= $maxi; ${i}++) {
		printit "$fldname\[$i\] = $phdrs->{$fldname}[$i]\n";
	}
}
#
sub dn2mysipuri {
	my ($dn) = @_;
	my $myip = $parameters{mysipip};
	my $myport = $parameters{mysipport};
	my $mycontact = "sip:${dn}\@${myip}";
	return($mycontact);
}
#
sub dn2contact {
	my ($dn) = @_;
	printit "dn2contact: dn=$dn\n";
	#
	my $ddn = $dn2data{$dn}
	#
	my $ip = $dndata{$ddn}{sipip};
	my $port = $dndata{$ddn}{sipport};
	my $contact = "sip:${dn}\@${ip}:${port}";
	#
	printit "dn2contact: contact=$contact\n";
	return($contact);
}
#
sub getCseqNo {
	my ($cseq) = @_;
	$cseq =~ /^[\s]*([0-9]+)[\s]+/;
	return($1);
}
#
sub makerequesturi {
	my ($callid, $field) = @_;
	#
	my $from = $dialogs{$callid}{$field};
	$from =~ /^.*sip:([^@]+)@([0-9\.]+).*$/;
	#
	my $dn = $1;
	my $ip = $2;
	#
	my $requesturi = "sip:${dn}\@${ip}";
	return($requesturi);
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
	my $ddn = $dn2data{$dn}
	#
	$$psdp = "";
	$$psdp .= "v=0\n";
	$$psdp .= "o=$dn 0 $dndata{$ddn}{rtpport} IN IP4 $dndata{$ddn}{rtpip}\n";
	$$psdp .= "s=-\n";
	$$psdp .= "c=IN IP4 $dndata{$ddn}{rtpip}\n";
	$$psdp .= "t=0 0\n";
	$$psdp .= "m=audio $dndata{$ddn}{rtpport} RTP/AVP 0\n";
	$$psdp .= "a=rtpmap:0 pcmu/$dndata{$ddn}{rtpport}\n";
}
#
sub parseSipMsg {
	my ($rawmsg, $prawhdr, $prawdata, $phdrs, $pdatablocks) = @_;
	#
	# separate the hdr from the payload.
	#
	($$prawhdr, $$prawdata) = split /[\r\n]{4,4}/, $rawmsg, 2;
	return unless (defined($$prawhdr));
	#
	# split fields in header. each header field is 
	# separated by CRLF. each header in turn is separated
	# by commas first, then semicolons. also, headers can 
	# be repeated. each repeated header field can be combined
	# into one header field separated by commas. each header field
	# can contain parameters which are separated by semicolons.
	#
	my @fldhdrs = split /[\r\n]+/, $$prawhdr;
	#
	# get the type of msg
	#
	my ($fld1, $fld2) = split /\s+/, $fldhdrs[0];
	$phdrs->{msgtype} = $fld1;
	if ($phdrs->{msgtype} =~ /^SIP\/2.0/) {
		$phdrs->{msgtype} = "RETURNCODE";
		$phdrs->{returncode} = $fld2;
	}
	#
	# split the remaining flds. combine multiple lines
	# into one line.
	#
	my $nflds = scalar @fldhdrs;
	for (my $ifld = 1; $ifld < $nflds; ${ifld}++) {
		my ($fldname, $fldvalue) = 
			split /\s*:\s*/, $fldhdrs[$ifld], 2;
		if (!exists($phdrs->{$fldname})) {
			$phdrs->{$fldname} = $fldvalue;
		} else {
			$phdrs->{$fldname} .= ",".$fldvalue;
		}
	}
	#
	# get data specific to msg type.
	#
	my $dummy = undef;
	if ($phdrs->{msgtype} =~ /^REGISTER/) {
		$dummy = $phdrs->{"To"};
		$dummy =~ /^.*[<]*sip:([0-9]+)@.*$/;
		$dummy = $1;
		if (defined($dummy)) {
			$phdrs->{"dn"} = $dummy;
		}
		my $callid = $phdrs->{"Call-ID"};
		if (!defined($dialogs{$callid})) {
			# first time, create dialog
			my $ddn = $dn2data{$phdrs->{"dn"}};
			$dialogs{$callid}{dn} = $phdrs->{"dn"};
			$dialogs{$callid}{contact} = $phdrs->{"Contact"};
			$dialogs{$callid}{mycontact} = 
				dn2contact($phdrs->{"dn"});
			$dialogs{$callid}{cseq} = 
				getCseqNo($phdrs->{"CSeq"});
			$dialogs{$callid}{handler} = $dndata{$ddn}{registrar};
			$dialogs{$callid}{"timertag"} = 0;
			$dialogs{$callid}{"mytag"} = int(rand(1000000000));
			$dialogs{$callid}{"mytag"} .= "mytag";
			# save additional fields when timers expire and
			# we need to send msgs.
			$dialogs{$callid}{from} = $phdrs->{"From"};
			$dialogs{$callid}{to} = $phdrs->{"To"};
			$dialogs{$callid}{via} = $phdrs->{"Via"};
			$dialogs{$callid}{maxforwards} = 
				$phdrs->{"Max-Forwards"};
			$dialogs{$callid}{requesturi} = 
				makerequesturi($callid, "contact");
		}
	} elsif ($phdrs->{msgtype} =~ /^INVITE/) {
		$dummy = $phdrs->{"To"};
		$dummy =~ /^.*[<]*sip:([0-9]+)@.*$/;
		$dummy = $1;
		if (defined($dummy)) {
			$phdrs->{"dn"} = $dummy;
		}
		my $callid = $phdrs->{"Call-ID"};
		if (!defined($dialogs{$callid})) {
			# first time, create dialog
			my $ddn = $dn2data{$phdrs->{"dn"}};
			$dialogs{$callid}{dn} = $phdrs->{"dn"};
			$dialogs{$callid}{contact} = $phdrs->{"Contact"};
			$dialogs{$callid}{mycontact} = 
				dn2contact($phdrs->{"dn"});
			$dialogs{$callid}{cseq} = 
				getCseqNo($phdrs->{"CSeq"});
			$dialogs{$callid}{handler} = $dndata{$ddn}{termination};
			$dialogs{$callid}{"timertag"} = 0;
			$dialogs{$callid}{"mytag"} = int(rand(1000000000));
			$dialogs{$callid}{"mytag"} .= "mytag";
			# save additional fields when timers expire and
			# we need to send msgs.
			$dialogs{$callid}{from} = $phdrs->{"From"};
			$dialogs{$callid}{to} = $phdrs->{"To"};
			$dialogs{$callid}{via} = $phdrs->{"Via"};
			$dialogs{$callid}{maxforwards} = 
				$phdrs->{"Max-Forwards"};
			$dialogs{$callid}{requesturi} = 
				makerequesturi($callid, "contact");
		}
	}
	#
	# any data blocks?
	#
	if ($phdrs->{"Content-Length"} == 0) {
		# no data
		return;
	}
	#
	# split blocks
	#
	if ($phdrs->{"Content-Type"} =~ /^multipart\/mixed/) {
		$phdrs->{"Content-Type"} =~ 
			/^multipart\/mixed;boundary=\"([^\"]+)\"/;
		my $dbdelimiter = $1;
		die "delimiter not found in Content-Type = mixed." unless 
			defined($dbdelimiter);
		@{$pdatablocks} = split /${dbdelimiter}/, $$prawdata;
	} else {
		# only one data block.
		@{$pdatablocks} = [ $$prawdata ];
		# check if we have an SDP block
		if ($phdrs->{"Content-Type"} =~ /application\/sdp/) {
			@{$phdrs->{sdp}} = [];
			@{$phdrs->{sdp}[0]} = split /[\r\n]+/, $$prawdata;
			dumpSDP($phdrs->{sdp}[0]);
		}
	}
	return;
}
#
sub changeDnState {
	my ($dn, $dnstate, $newstate) = @_;
	my $ddn = $dn2data{$phdrs->{"dn"}};
	printit("\nOLD $dnstate STATE FOR $dn: $dndata{$ddn}{$dnstate}\n");
	$dndata{$dn}{$dnstate} = $newstate;
	printit("NEW $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
#
sub dumpDnState {
	my ($dn, $dnstate) = @_;
	printit("\nCURRENT $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
#
sub isMyTimer {
	my ($callid, $ptimer) = @_;
	#
	my $dialogtag = $dialogs{$callid}{"timertag"};
	my $timertag = $ptimer->{"tag"};
	#
	if ($dialogtag != $timertag) {
		# it is not my timer
		printit "\nTimer label: ".$ptimer->{"label"}."\n";
		printit "Ignoring old timer (tag=$timertag).\n";
		printit "(callid=$callid).\n";
		return(0);
	} else {
		# it is my timer
		return(1);
	}
}
#
########################################################################
#
# utility functions to make msgs
#
sub starttimer {
	my ($callid, $delta, $label) = @_;
	#
	printit "\nstarttimer:\n";
	printit "\tcallid=${callid}\n";
	printit "\tlabel=${label}\n";
	printit "\tdelta=${delta}\n";
	my $endtime = time() + $delta;
	printit "\tendtime=$endtime\n";
	#
	my $pnewtimer = ();
	$pnewtimer->{"Call-ID"} = $callid;
	$pnewtimer->{"tag"} = $endtime;
	$pnewtimer->{"label"} = $label;
	#
	$dialogs{$callid}{"timertag"} = $endtime;
	#
	foreach $key (keys %${pnewtimer}) {
		printit "$key = $pnewtimer->{$key}\n";
	}
	#
	mypqueue::enqueue($pq, $endtime, $pnewtimer);
}
#
sub makepaddr {
	my ($contact, $callid) = @_;
	#
	my $dcontact = $dialogs{$callid}{$contact};
	$dcontact =~ /^.*<*sip:([^@]+)@([^:]+):([0-9]+).*$/;
	#
	my $dn = $1;
	my $ip = $2;
	my $port = $3;
	#
	my $ipaddr = gethostbyname($ip);
	defined($ipaddr) or die "gethostbyname: $!";
	my $paddr = sockaddr_in($port, $ipaddr);
	defined($paddr) or die "getprotobyname: $!";
	#
	return($paddr);
}
#
sub sendRegister {
	my ($callid, $contact) = @_;
	#
	return;
}
#
sub sendRegister200OK {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
	$ackmsg .= "Expires: ".$dndata{$dn}{"expires"}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub sendInvite180Ringing {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 180 Ringing\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}.";tag=".$dialogs{$callid}{mytag}."\r\n";
	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub sendInvite200OK {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}.";tag=".$dialogs{$callid}{mytag}."\r\n";
	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	printit "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub sendBye {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	# flip To and From fields in the BYE msg.
	#
	my $ackmsg = "BYE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{to}.";tag=".$dialogs{$callid}{mytag}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{from}."\r\n";
	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." BYE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub sendAck {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	printit "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub sendCancel {
	my ($callid, $contact) = @_;
	#
	return;
}
#
sub sendInvite {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "INVITE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{from}.";tag=".$dialogs{$callid}{mytag}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{to}."\r\n";
	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." INVITE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	printit "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
sub send200OK {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	return;
}
#
##########################################################################
#
# high-level msg handlers
#
sub register {
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry register: Call-ID = <$callid>\n";
	printit "Timer Flag for $callid = <$timerflag>\n";
	#
	my $dn = $dialogs{$callid}{dn};
	my $regstate = $dndata{$dn}{registrationstate};
	dumpDnState($dn, "registrationstate");
	#
	# check for registrations
	#
	if ($origstate =~ /^INACTIVE$/) {
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
			changeDnState($dn, "registration", "WAITFOR200");
		} else {
			printit "\nregistration INACTIVE:\n";
			printit "Unexpected messge or timer.\n";
			printit "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# timed out, schedule another register attempt.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "registration", "INACTIVE");
			queueReg($dn, $dndata{$dn}{interregistertime});
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			if ($phdrs->{returncode} == 200) {
				starttimer($callid, $dndata{$dn}{"expires"},
					"REGISTRATION EXPIRES");
				changeDnState($dn, "registrationstate", "REGISTERED");
			} else {
				# registration denied. try again.
				delete $dialogs{$callid};
				changeDnState($dn, "registration", "INACTIVE");
				queueReg($dn, $dndata{$dn}{interregistertime});
			}
		} else {
			printit "\nregistration WAITFOR200 warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^REGISTERED$/) {
		if ($timerflag) {
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
			changeDnState($dn, "registration", "WAITFOR200");
		} else {
			printit "\nregistration TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} else {
		printit "\nregistration - unknown state $origstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(callid=$callid).\n";
	}
}
#
sub registrar {
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry registrar: Call-ID = <$callid>\n";
	printit "Timer Flag for $callid = <$timerflag>\n";
	#
	my $dn = $dialogs{$callid}{dn};
	my $regstate = $dndata{$dn}{registrationstate};
	dumpDnState($dn, "registrationstate");
	#
	if ($timerflag) {
		if (!isMyTimer($callid, $ptimer)) {
			$ptimer = undef;
			return;
		}
		#
		$ptimer = undef;
		$dialogs{$callid}{"timertag"} = 0;
		changeDnState($dn, "registrationstate", "UNREGISTER");
	} elsif ($phdrs->{msgtype} =~ /^REGISTER$/) {
		sendRegister200OK($callid, $phdrs, "contact");
		starttimer($callid, $dndata{$dn}{"expires"}, "REGISTRATION TIME OUT");
		changeDnState($dn, "registrationstate", "REGISTER");
	} else {
		printit "\nregistrar REGISTER warning:\n";
		printit "Unexpected msg $phdrs->{msgtype}.\n";
		printit "(callid=$callid).\n";
	}
	return;
}
#
sub origination {
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry origination: Call-ID = <$callid>\n";
	printit "Timer Flag for $callid = <$timerflag>\n";
	#
	my $dn = $dialogs{$callid}{dn};
	my $origstate = $dndata{$dn}{originationstate};
	dumpDnState($dn, "originationstate");
	#
	# check for originations
	#
	if ($origstate =~ /^INACTIVE$/) {
		# only call orig timers are allowed.
		if ($timerflag == 2) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			sendInvite($callid, "contact");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "originationstate", "WAITFOR200");
		} else {
			printit "\norigination INACTIVE:\n";
			printit "Unexpected messge or timer.\n";
			printit "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendCancel($callid, "mycontact");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 CANCEL");
			changeDnState($dn, "originationstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			if ($phdrs->{returncode} == 100) {
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "originationstate", "WAITFOR200");
			} elsif ($phdrs->{returncode} == 180) {
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "originationstate", "WAITFOR200");
			} elsif ($phdrs->{returncode} == 183) {
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "originationstate", "WAITFOR200");
			} elsif ($phdrs->{returncode} == 200) {
				sendAck($callid, $phdrs, "contact");
				starttimer($callid, 
					$dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKING");
			} else {
				# call ends.
				delete $dialogs{$callid};
				changeDnState($dn, "originationstate", "INACTIVE");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} else {
			printit "\norigination WAITFOR200 warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} else {
			printit "\norigination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} else {
			printit "\norigination WAITFOR200BYE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "originationstate", "INACTIVE");
		} else {
			printit "\norigination WAITFOR200CANCEL warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} else {
		printit "\norigination - unknown state $origstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(callid=$callid).\n";
	}
	return;
}
#
sub termination {
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry termination: Call-ID = <$callid>\n";
	printit "Timer Flag for $callid = <$timerflag>\n";
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			printit "\ntermination INACTIVE warning:\n";
			printit "Unexpected timer.\n";
			printit "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			sendInvite180Ringing($callid, $phdrs, "contact");
			sendInvite200OK($callid, $phdrs, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} else {
			printit "\ntermination INACTIVE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFORACK warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFOR200BYE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} else {
		printit "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(callid=$callid).\n";
	}
	return;
}
#
sub terminationrefer {
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer) = @_;
	#
	printit "\nEntry termination: Call-ID = <$callid>\n";
	printit "Timer Flag for $callid = <$timerflag>\n";
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			printit "\ntermination INACTIVE warning:\n";
			printit "Unexpected timer.\n";
			printit "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			sendInvite180Ringing($callid, $phdrs, "contact");
			sendInvite200OK($callid, $phdrs, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} else {
			printit "\ntermination INACTIVE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			starttimer($callid, $dndata{$dn}{"prerefercallduration"},
				"TALKING PRE-REFER CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKINGBEFOREREFER");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFORACK warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKINGBEFOREREFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendRefer($callid, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor202duration"},
				"WAIT FOR 202 REFER");
			changeDnState($dn, "terminationstate", "WAITFOR202REFER");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKINGBEFOREREFER warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFOR202REFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "mycontact");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			if ($phdrs->{returncode} == 202) {
				# got OK response
				starttimer($callid, $dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "terminationstate", "TALKING");
			} else {
				# not what we wanted, end call.
				$dialogs{$callid}{cseq} += 1;
				sendBye($callid, "mycontact");
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 BYE");
				changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\norigination WAITFOR202REFER warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			# respond and stay in same state. timer already is active.
			send200OK($callid, $phdrs, "contact");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination TALKING warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$ptimer = undef;
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			# call ends.
			delete $dialogs{$callid};
			changeDnState($dn, "terminationstate", "INACTIVE");
		} else {
			printit "\ntermination WAITFOR200BYE warning:\n";
			printit "Unexpected msg $phdrs->{msgtype}.\n";
			printit "(callid=$callid).\n";
		}
	} else {
		printit "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			printit "Unexpected timer.\n";
			$ptimer = undef;
		}
		printit "(callid=$callid).\n";
	}
	return;
}
#
##########################################################################
#
# general msg handlers
#
sub queueOrig {
	my ($odn, $tdn, $delta) = @_;
	#
	# verify both DNs exist
	#
	my $ok = 1;
	if (!exists($dndata{$odn})) {
		printit "Unknown Orig DN $odn.\n";
		$ok = 0;
	}
	if (!exists($dndata{$tdn})) {
		printit "Unknown Term DN $tdn.\n";
		$ok = 0;
	}
	if (!$ok) {
		printit "returning without starting call.\n";
		return;
	}
	#
	# create call id
	#
	my $callid = int(rand(1000000000));
	$callid = "${callid}\@$parameters{mysipip}";
	#
	# create a dialog
	#
	$dialogs{$callid}{dn} = $odn;
	$dialogs{$callid}{origdn} = $odn;
	$dialogs{$callid}{termdn} = $tdn;
	$dialogs{$callid}{handler} = $dndata{$odn}{origination};
	$dialogs{$callid}{contact} = dn2contact($tdn);
	$dialogs{$callid}{mycontact} = dn2contact($odn);
	$dialogs{$callid}{cseq} = int(rand(1000000000));
	$dialogs{$callid}{"timertag"} = 0;
	$dialogs{$callid}{"mytag"} = int(rand(1000000000));
	$dialogs{$callid}{"mytag"} .= "mytag";
	$dialogs{$callid}{maxforwards} = $parameters{"maxforwards"};
	#
	my $odnip = $dndata{$odn}{sipip};
	my $odnport = $dndata{$odn}{sipport};
	$dialogs{$callid}{"from"} = "sip:${odn}\@${odnip}:${odnport}";
	#
	my $tdnip = $dndata{$tdn}{sipip};
	my $tdnport = $dndata{$tdn}{sipport};
	$dialogs{$callid}{"requesturi"} = "sip:${tdn}\@${tdnip}:${tdnport}";
	$dialogs{$callid}{"to"} = "sip:${tdn}\@${tdnip}:${tdnport}";
	#
	# queue up a timer request
	#
	starttimer($callid, $delta, "ORIGINATION");
	return;
}
#
sub queueReg {
	my ($rdn, $delta) = @_;
	#
}
#
$previousorigdn = undef;
$previoustermdn = undef;
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
	} elsif ($line =~ /^w/) {
		printit "\n$line\n";
	} elsif ($line =~ /^D/) {
		$debugflag = !$debugflag;
		if ($debugflag) {
			printit "Debugging is ON\n";
		} else {
			printit "Debugging is OFF\n";
		}
	} elsif ($line =~ /^l[\s]+([^\s]+)/) {
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
	} elsif ($line =~ /^l[\s]*$/) {
		printit "\nListing DNs:\n";
		foreach my $dn (keys %dndata) {
			printit "DN $dn\n";
			printit "\tDN type           => $dndata{$dn}{dntype}\n";
			printit "\tregistrationstate => $dndata{$dn}{registrationstate}\n";
			printit "\toriginationstate  => $dndata{$dn}{originationstate}\n";
			printit "\tterminationstate  => $dndata{$dn}{terminationstate}\n";
		}
	} elsif ($line =~ /^d[\s]*$/) {
		printit "\nListing Dialogs:\n";
		foreach my $callid (keys %dialogs) {
			printit "Call-ID $callid\n";
		}
	} elsif ($line =~ /^r[\s]+([^\s]+)/) {
		my $regdn = $1;
		printit "\nRegistering DN $regdn.\n";
		queueReg($regdn, 0);
	} elsif ($line =~ /^o[\s]*$/) {
		if (defined($previousorigdn) && defined($previoustermdn)) {
			printit "\nDN $previousorigdn calling DN $previoustermdn.\n";
			queueOrig($previousorigdn, $previoustermdn, 0);
		} else {
			printit "Previous orig/term DNs not defined.\n";
		}
	} elsif ($line =~ /^o[\s]+([^\s]+)[\s]+([^\s]+)/) {
		my $origdn = $previousorigdn = $1;
		my $termdn = $previoustermdn = $2;
		printit "\nDN $origdn calling DN $termdn.\n";
		queueOrig($origdn, $termdn, 0);
	} elsif ($line =~ /^\?/) {
		printit "\nhelp text:\n";
		printit "\t'?' - help text\n";
		printit "\t'q' - quit\n";
		printit "\t'w text' - write 'text' to stdout/logfile.\n";
		printit "\t'l' - list DNs.\n";
		printit "\t'd' - list active dialogs.\n";
		printit "\t'D' - toggle debugging.\n";
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
	#
	my $msglen = length($rawmsg);
	printit "\nRTP msg received: length=$msglen\n";
	#
	return;
}
#
sub handleRTCP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	#
	my $msglen = length($rawmsg);
	printit "\nRTCP msg received: length=$msglen\n";
	#
	return;
}
#
sub handleTimer {
	my ($current_time, $ptimer) = @_;
	#
	printit "\nTimer expired: \n";
	# check if call id data exits
	if (!defined($ptimer->{"Call-ID"})) {
		printit "\nNo Call-ID defined in timer.\n";
		$ptimer = undef;
		return;
	}
	my $callid = $ptimer->{"Call-ID"};
	if (!defined($dialogs{$callid})) {
		printit "\nNo timer dialog data for $callid defined.\n";
		$ptimer = undef;
		return;
	}
	#
	my $timertype = 0;
	if ($ptimer->{"label"} =~ /^ORIGINATION$/) {
		# call origination event
		$timertype = 2;
	} elsif ($ptimer->{"label"} =~ /^REGISTRATION$/) {
		# registration event
		$timertype = 3;
	} else {
		# regular timer
		$timertype = 1;
	}
	#
	my $handler = $dialogs{$callid}{handler};
	&{${handler}}($callid, $timertype, $current_time, 
		undef, undef, undef, $ptimer);
	return;
}
#
sub handleSIP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	#
	my $msglen = length($rawmsg);
	if ($msglen < $parameters{minmsgsize}) {
		# garbage msg, skip it ...
		return;
	}
	printit "\nSIP msg received: length=$msglen\n";
	dumpIncomingMsg($recvpaddr, $rawmsg);
	#
	my %hdrs = ();
	my @datablocks = ();
	my $rawhdr = undef;
	my $rawdata = undef;
	#
	parseSipMsg($rawmsg, \$rawhdr, \$rawdata, \%hdrs, \@datablocks);
	dumphdrs(\%hdrs);
	#
	my $callid = undef;
	if (defined($hdrs{"Call-ID"}) && 
	    defined($dialogs{$hdrs{"Call-ID"}})) {
		$callid = $hdrs{"Call-ID"};
	} else {
		printit "\nNo dialog data found.\n";
		return;
	}
	if (!defined($dialogs{$callid}{handler})) {
		printit "\nNo handler for $callid defined.\n";
		return;
	}
	my $handler = $dialogs{$callid}{handler};
	&{${handler}}($callid, 0, $current_time, $recvpaddr, 
			\@datablocks, \%hdrs, undef);
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
#
dumpparams();
dumpdndata();
#
# set up socket connections
#
my $proto = getprotobyname('udp');
defined($proto) or die "getprotobyname: $!";
#
socket(SIP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
my $ipaddr = gethostbyname($parameters{mysipip});
defined($ipaddr) or die "gethostbyname: $!";
my $paddr = sockaddr_in($parameters{mysipport}, $ipaddr);
defined($paddr) or die "getprotobyname: $!";
bind(SIP, $paddr) or die "bind: $!";
#
socket(RTP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
my $rtp_ipaddr = gethostbyname($parameters{myrtpip});
defined($ipaddr) or die "gethostbyname: $!";
my $rtp_paddr = sockaddr_in($parameters{myrtpport}, $rtp_ipaddr);
defined($rtp_paddr) or die "getprotobyname: $!";
bind(RTP, $rtp_paddr) or die "bind: $!";
#
socket(RTCP, PF_INET, SOCK_DGRAM, $proto) or die "socket: $!";
my $rtcp_paddr = sockaddr_in($parameters{myrtcpport}, $rtp_ipaddr);
defined($rtcp_paddr) or die "getprotobyname: $!";
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
# we don't want to I/O block. if no input, just return.
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
my $mydelta;
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
		dprint "default delta = $mydelta.\n";
	} elsif (mypqueue::front($pq, \$key, \$pdata) == 1) {
		# sleep until next scheduled event.
		$mydelta = $key - $current_time;
		$mydelta = 0 if ($mydelta < 0);
		dprint "delta = $mydelta.\n";
	} else {
		printit "ERROR: event queue corruption, unable to calculate delta !!!\n";
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
	dprint "(currtime, etime, totaletime)=($current_time,$elapsed_time,$total_elapsed_time)\n";
	#
	# handle any expired timers
	#
	while (!mypqueue::isEmpty($pq)) {
		if ((mypqueue::front($pq, \$key, \$pdata) == 0) ||
		    ($key > $current_time)) {
			dprint "last (key, current_time)=($key,$current_time)\n";
			last;
		} 
		dprint "found one (key, current_time)=($key,$current_time)\n";
		mypqueue::dequeue($pq, \$key, \$pdata);
		#
		dprint "\nexpired timer:\n";
		dprint "\tcallid = ".$pdata->{"Call-ID"}."\n";
		dprint "\ttag    = ".$pdata->{tag}."\n";
		handleTimer($current_time, $pdata);
	}
	#
	# handle any input from user.
	#
	if (vec($rout, fileno(STDIN), 1)) {
		handleInput(\$done);
	}
	#
	# handle any msgs from sockets: RTP, RTCP, SIP.
	#
	if (vec($rout, fileno(SIP), 1)) {
		while (defined($recvpaddr = recv(SIP, my $msg, 8*1024, 0))) {
			handleSIP($current_time, $recvpaddr, $msg);
		}
	}
	if (vec($rout, fileno(RTCP), 1)) {
		while (defined($recvpaddr = recv(RTCP, my $msg, 8*1024, 0))) {
			handleRTCP($current_time, $recvpaddr, $msg);
		}
	}
	if (vec($rout, fileno(RTP), 1)) {
		while (defined($recvpaddr = recv(RTP, my $msg, 8*1024, 0))) {
			handleRTP($current_time, $recvpaddr, $msg);
		}
	}
}
#
close(SIP);
close(RTP);
close(RTCP);
close(LOGFILE) if (exists($parameters{logfile}));
#
exit 0;
