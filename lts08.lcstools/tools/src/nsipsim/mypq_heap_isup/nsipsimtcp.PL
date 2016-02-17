#!/opt/exp/bin/perl -w
#!/opt/exp/bin/perl -w -d:DProf
# test
# SIP simulator.
#
########################################################################
#
# path for finding modules
#
use lib "$ENV{SIPSIMHOME}/bin";
use lib "$ENV{SIPSIMHOME}/lib";
#
# modules
#
use Cwd;
use Carp;
use Fcntl;
use Storable qw(dclone);
use IO::Socket;
use Getopt::Std;
use Time::Local;
#
use mytimer;
use mypqueue;
use myisup;
#
#########################################################################
#
# global data
#
$debugflag = 0;
$logflag = 0;
$rtplogflag = 0;
$msglogflag = 0;
$verboseflag = 0;
$rtpverboseflag = 0;
#
# any parameter or dndata must have a default value defined below. if not,
# when it is detected, the simulator will report it and exit.
#
%parameters = (
	branch => "z9hG4bk",
	delta => 10,
	minmsgsize => 10,
	logfile => "",
	rtplogfile => "",
	mysipip => "127.0.0.1",
	mysipport => 5060,
	mytcpsipip => "127.0.0.1",
	mytcpsipport => 5060,
	myrtpip => "127.0.0.1",
	myrtpport => 8000,
	myrtcpport => 8001,
	myadminip => "127.0.0.1",
	myadminport => 10001,
	maxforwards => 70,
	supportrtp => 1,
	interreporttime => 900,
	isupversion => "ansi",
	editor => "vi",
);
#
%dndata = (
	DEFAULT => {
		dn => undef,
		dnstate => "INACTIVE",
		callloadstate => "INACTIVE",
		codec => "pcmu",
		registrationstate => "INACTIVE",
		originationstate => "INACTIVE",
		terminationstate => "INACTIVE",
		announcementstate => "INACTIVE",
		registrationcallid => "",
		originationcallid => "",
		terminationcallid => "",
		announcementcallid => "",
		registrar => "registrar",
		registration => "register",
		origination => "origination",
		termination => "termination",
		announcement => "announcement",
		ipaddress => "127.0.0.1",
		sipip => "127.0.0.1",
		sipport => 5060,
		tcpsipip => "127.0.0.1",
		tcpsipport => 5060,
		rtpip => "127.0.0.1",
		rtpport => 8000,
		rtcpip => "127.0.0.1",
		rtcpport => 8001,
		expires => 600,
		interregistertime => 120,
		maxcallduration => 60,
		waitingforackduration => 10,
		waitingfor200duration => 10,
		waitingfor202duration => 30,
		ringingduration => 1,
		prerefercallduration => 10,
		referto => undef,
		isupoli => "",
		handleorigreinvite => 0,
		handletermreinvite => 0,
		reqanncdn => "__sip-annc",
		reqanncuri => "",
		redirectcontact => "",
		redirectcode => 302,
		returncode => 486,
		prereqannccallduration => 5,
		presubscribecallduration => 5,
		pchargingvector => "",
		invitesubject => "ground control to major tom",
		subscribeevent => "telephone-event;duration=2000",
		isupmsgfile => "isupmsgs.in",
		orig200okbyemsgid => 0b00010000,
		orig200okcancelmsgid => 0b00010000,
		orig200okmsgid => 0b00000000,
		origackmsgid => 0b00000000,
		origbyemsgid => 0b00001100,
		origcancelmsgid => 0b00001100,
		originviteackmsgid => 0b00000000,
		originvitemsgid => 0b00000001,
		orignotifymsgid => 0b00000000,
		term180ringingmsgid => 0b00000110,
		term183ringingmsgid => 0b00101100,
		term200okbyemsgid => 0b00010000,
		term200okcancelmsgid => 0b00010000,
		term200okmsgid => 0b00000000,
		term486busymsgid => 0b00000000,
		termbyemsgid => 0b00001100,
		terminvite200okmsgid => 0b00000000,
		origpost100infomsgid => 0b00000000,
		origpost180infomsgid => 0b00000000,
		origpost183infomsgid => 0b00000000,
		origpost200infomsgid => 0b00000000,
		origpostackinfomsgid => 0b00000000,
		termpost100infomsgid => 0b00000000,
		termpost180infomsgid => 0b00000000,
		termpost183infomsgid => 0b00000000,
		termpost200infomsgid => 0b00000000,
		termpostackinfomsgid => 0b00000000,
		usepassertedidentity => 0,
		passertedidentity => "",

		orig200okbyesdp => "",
		orig200okcancelsdp => "",
		orig200oksdp => "",
		orig200okupdatesdp => "",
		origacksdp => "",
		origbyesdp => "",
		origcancelsdp => "",
		originviteacksdp => "pcma",
		originvitesdp => "pcma",
		orignotifysdp => "",
		term180ringingsdp => "",
		term183ringingsdp => "",
		term200okbyesdp => "",
		term200okcancelsdp => "",
		term200oksdp => "",
		term200okupdatesdp => "",
		term486busysdp => "",
		termbyesdp => "",
		terminvite200oksdp => "pcma",
		origpost100updatesdp => "",
		origpost180updatesdp => "",
		origpost183updatesdp => "",
		origpost200updatesdp => "",
		origpostackupdatesdp => "",
		termpost100updatesdp => "",
		termpost180updatesdp => "",
		termpost183updatesdp => "",
		termpost200updatesdp => "",
		termpostackupdatesdp => "",
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
%statistics = (
	origination => {
		started => 0,
		stable => 0,
		abandoned => 0,
		timedout => 0,
	},
	termination => {
		started => 0,
		stable => 0,
		abandoned => 0,
		timedout => 0,
	},
);
#
%loaddata = (
	state => "idle",
	callspersec => 0.0,
	maxcalls => 0,
	callsstarted => 0,
	holdtime => 0,
	calls => { },
	origdns => [ ],
	currentorigdn => 0,
	origdnsz => 0,
	tag => -1,
);
#
use subs qw(printit);
use subs qw(dprint);
use subs qw(vprint);
#
########################################################################
#
# functions
#
sub usage {
	printit "\n";
	printit "usage: nsipsim [-?xV] [-l logfile] [configfile]\n";
	printit "\n";
	printit "if a configfile is not given, then the default config\n";
	printit "file 'sipsimcfg' is read from the current directory.\n";
}
#
sub dprint {
	my ($msg) = @_;
	if ($debugflag) {
		printit $msg;
	}
}
sub vprint {
	my ($msg) = @_;
	if ($verboseflag) {
		printit $msg;
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
sub logit {
	my ($msg) = @_;
	#
	if ($logflag) {
		print LOGFILE ${msg};
	}
}
#
sub dumpdndata {
	#
	return unless ($verboseflag);
	#
	vprint "\n";
	vprint "Dumping DN-Data structure:\n";
	foreach $key (sort keys %dndata) {
		vprint "dn = $key\n";
		foreach $key2 (sort keys %{$dndata{$key}}) {
			vprint "\t$key2 = ";
			if (defined($dndata{$key}{$key2})) {
				vprint "$dndata{$key}{$key2}\n";
			} else {
				vprint "====>>>> UNDEFINED <<<<====\n";
			}
		}
	}
}
#
sub deleteCallId {
	my ($callid) = @_;
	#
	return unless (defined($dialogs{$callid}));
	return unless (exists($dialogs{$callid}));
	#
	delete $dialogs{$callid};
	#
	return;
}
#
sub dumpparams {
	#
	return unless ($verboseflag);
	#
	vprint "\n";
	vprint "Parameters:\n";
	foreach $param (sort keys %parameters) {
		vprint "\t$param = $parameters{$param}\n";
	}
}
#
sub dumphdrs {
	my ($phdrs) = @_;
	#
	return unless ($verboseflag);
	#
	vprint "\nHeader Fields:\n";
	foreach $key (sort keys %{$phdrs}) {
		if (defined($phdrs->{$key})) {
			vprint "$key = $phdrs->{$key}\n";
		} else {
			vprint "$key NOT DEFINED.\n";
		}
	}
}
#
sub dumpPQ {
	my ($pq) = @_;
	printit "\nPriority Queue Contents:\n";
	$pq->dump();
}
#
sub dumpIncomingMsg {
	my ($recvpaddr, $msg) = @_;
	#
	return unless ($verboseflag);
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	vprint "\n";
	vprint "Current time: ".scalar(localtime())."\n";
	vprint "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< incoming msg\n";
	vprint "FROM IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		vprint "line=<$line>\n";
	}
	vprint "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< incoming msg\n";
}
#
sub dumpOutgoingMsg {
	my ($recvpaddr, $msg, $isupmsg) = @_;
	#
	return unless ($verboseflag);
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	vprint "\n";
	vprint "Current time: ".scalar(localtime())."\n";
	vprint ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
	vprint "TO IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		vprint "line=<$line>\n";
	}
	vprint ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
	if (defined($isupmsg)) {
		vprint ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing ISUP msg\n";
		vprint "TO IP/PORT: $recvaddr\n";
		$isup->decode($parameters{isupversion}, $isupmsg);
		vprint ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> outgoing ISUP msg\n";
	}
}
#
sub dumptimer {
	my ($timerflag, $ptimer) = @_;
	#
	return unless ($verboseflag);
	#
	vprint "\nTimer data:\n";
	vprint "\ttimer flag = ${timerflag}\n";
	vprint "\tcallid = ".$ptimer->{callid}."\n";
	vprint "\texpire = ".$ptimer->{expire}."\n";
	vprint "\tid     = ".$ptimer->{id}."\n";
	vprint "\tlabel  = ".$ptimer->{label}."\n";
}
#
sub incrstats {
	my ($state, $type) = @_;
	#
	if (defined($statistics{$state}{$type})) {
		$statistics{$state}{$type} += 1;
	} else {
		$statistics{$state}{$type} = 1;
	}
}
#
sub bitval {
	my ($v, $lowbit, $highbit) = @_;
	#
	my $v2 = (($v >> $lowbit) << $lowbit);
	my $v3 = (($v2 << (31-$highbit)) >> (31-$highbit));
	my $v4 = ($v3 >> (31-$highbit));
	#
	return($v4);
}
sub rbitval {
	my ($v, $lowbit, $highbit) = @_;
	#
	my $v2 = (($v >> (31-$highbit)) << (31-$highbit));
	my $v3 = (($v2 << $lowbit) >> $lowbit);
	my $v4 = ($v3 >> (31-$highbit));
	#
	return($v4);
}
#
sub readconfigfile {
	my ($configfile) = @_;
	#
	my $dntype = "individual";
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
		# is it a ORIGDN CNT pair?
		if ($crec =~ /^rcall/) {
			my ($dummy, $origdn, $termdn, $cnt) = split /\s+/, $crec, 4;
			if (!defined($dummy) || !defined($origdn) ||
			    !defined($termdn) || !defined($cnt)) {
				printit "\n";
				printit "skipping record <$crec>.\n";
				printit "not in a 'rcall origdn termdn count' format.\n";
				$syntaxerrors += 1;
				next;
			}
			for (my $i=0; $i<$cnt; ${i}++) {
				my $odn = $origdn + $i;
				my $tdn = $termdn + $i;
				$loaddata{calls}{$odn} = $tdn;
				if (!exists($dndata{$odn})) {
					printit "$odn has NO dndata.\n";
					$syntaxerrors += 1;
				}
				if (!exists($dndata{$tdn})) {
					printit "$tdn has NO dndata.\n";
					$syntaxerrors += 1;
				}
			}
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
			$dntype = "individual";
		} elsif ($key =~ /^rdn/) {
			($mindn, $dncnt) = split /\s+/, $value, 2;
			if (!defined($mindn) || !defined($dncnt)) {
				printit "\n";
				printit "skipping record <$crec>.\n";
				printit "not in a 'rdn startdn cnt' format.\n";
				$syntaxerrors += 1;
				next;
			}
			for (my $i=0; $i<$dncnt; ${i}++) {
				$dn = $mindn + $i;
				$dndata{$dn} = dclone($dndata{DEFAULT});
				$dndata{$dn}{dn} = $dn;
			}
			$dntype = "range";
		} else {
			if (!exists($dndata{DEFAULT}{$key}))
			{
				$nodefaults += 1;
				printit "\n";
				printit "field $key has NO default value.\n";
			}
			if ($dntype eq "individual") {
				$dndata{$dn}{$key} = $value;
			} elsif ($dntype eq "range") {
				if ($key eq "tcpsipport") {
					# add a different TCP port for each
					# DN when originating a call from
					# range of DNs.
					for (my $i=0; $i<$dncnt; ${i}++) {
						$dn = $mindn + $i;
						my $tcpport = $value+$i;
						$dndata{$dn}{$key} = $tcpport;
					}
				} else {
					for (my $i=0; $i<$dncnt; ${i}++) {
						$dn = $mindn + $i;
						$dndata{$dn}{$key} = $value;
					}
				}
			} else {
				printit "readconfigfile: invalid dntype: $dntype\n";
				exit 2;
			}
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
	# generate list of originating DNs.
	#
	my $odncnt = scalar(keys %{$loaddata{calls}});
	if ($odncnt > 0) {
		printit "\n$odncnt ORIG-TERM DN pairs read.\n";
		@{$loaddata{origdns}} = sort keys %{$loaddata{calls}};
		$loaddata{origdnsz} = @{$loaddata{origdns}};
	}
	#
	return;
}
#
sub dumpfld {
	my ($fldname, $phdrs) = @_;
	#
	return unless ($verboseflag);
	#
	my $maxi = $#{$phdrs->{$fldname}};
	for (my $i = 0; $i <= $maxi; ${i}++) {
		vprint "$fldname\[$i\] = $phdrs->{$fldname}[$i]\n";
	}
}
#
sub callid2mysipuri {
	my ($callid) = @_;
	my $mydn = $dialogs{$callid}{dn};
	my $myip = $dialogs{$callid}{sipip};
	# my $myport = $dialogs{$callid}{sipport};
	my $mycontact = "sip:${mydn}\@${myip}";
	return($mycontact);
}
#
sub callid2contact {
	my ($callid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $ip = $dialogs{$callid}{sipip};
	my $port = $dialogs{$callid}{sipport};
	my $contact = "sip:${dn}\@${ip}:${port}";
	#
	return($contact);
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
	vprint "\ndn2contact: dn=$dn\n";
	#
	my $ip = $dndata{$dn}{sipip};
	my $port = $dndata{$dn}{sipport};
	my $contact = "sip:${dn}\@${ip}:${port}";
	#
	vprint "dn2contact: contact=$contact\n";
	return($contact);
}
#
sub getCseqNo {
	my ($cseq) = @_;
	$cseq =~ /^[\s]*([0-9]+)[\s]+/;
	return($1);
}
sub getCseqMethod {
	my ($cseq) = @_;
	$cseq =~ /^[\s]*[0-9]+[\s]+([^\s]+)/;
	return($1);
}
#
sub contact2dnipport {
	my ($contact) = @_;
	#
	my $dn = undef;
	my $ip = undef;
	my $port = undef;
	#
	if ($contact =~ /^.*<*sip:([^@]+)@([^:]+):([0-9]+).*$/) {
		$dn = $1;
		$ip = $2;
		$port = $3;
	} elsif ($contact =~ /^.*<*sip:([^:]+):([0-9]+).*$/) {
		$ip = $1;
		$port = $2;
	} elsif ($contact =~ /^.*<*sip:([^@]+)@([0-9\.]+).*$/) {
		$dn = $1;
		$ip = $2;
		$port = 5060;
	} elsif ($contact =~ /^.*<*sip:([0-9\.]+).*$/) {
		$ip = $1;
		$port = 5060;
	} else {
		printit "contact2dnipport: unable to get IP addr from contact !!!\n";
		printit "contact = ${contact}\n";
		confess "contact2dnipport exiting.";
	}
	#
	if ($verboseflag) {
		if (defined($dn)) {
			vprint "contact2dnipport: dn is ... ${dn}\n";
		} else {
			vprint "contact2dnipport: dn is ... undef\n";
		}
		if (defined($ip)) {
			vprint "contact2dnipport: ip is ... ${ip}\n";
		} else {
			vprint "contact2dnipport: ip is ... undef\n";
		}
		if (defined($port)) {
			vprint "contact2dnipport: port is ... ${port}\n";
		} else {
			vprint "contact2dnipport: port is ... undef\n";
		}
	}
	return($dn, $ip, $port);
}
#
sub makerequesturi {
	my ($callid, $field) = @_;
	#
	# try form with DN and IP.
	my $from = $dialogs{$callid}{$field};
	$from =~ /^.*sip:([^@]+)@([0-9\.]+).*$/;
	my $dn = $1;
	my $ip = $2;
	if (defined($dn) && defined($ip)) {
		# found first form.
		my $requesturi = "sip:${dn}\@${ip}";
		return($requesturi);
	} 
	# try without DN, just an IP.
	$from = $dialogs{$callid}{$field};
	$from =~ /^.*sip:([0-9\.]+).*$/;
	$ip = $1;
	my $requesturi = "sip:${ip}";
	return($requesturi);
}
#
sub dumpSDP {
	my ($psdp) = @_;
	#
	return unless ($verboseflag);
	#
	vprint "\n";
	vprint "SDP CONTENTS: \n";
	foreach $line (@{${psdp}}) {
		vprint "$line\n";
	}
}
sub dumpISUP {
	my ($rawisup) = @_;
	#
	return unless ($verboseflag);
	#
	vprint "\n";
	vprint "ISUP CONTENTS: \n";
	my $msglen = length($rawisup);
	printit "\nISUP msg length: $msglen\n";
	#
	my @isupdata = unpack "N*", $rawisup;
	#
	for (my $il=0; $il<scalar(@isupdata); ${il}++) {
		printit "ISUP[${il}] = $isupdata[$il]\n";
	}
}
#
sub makeSDP {
	my ($dn, $psdp) = @_;
	#
	$$psdp = "";
	$$psdp .= "v=0\n";
	$$psdp .= "o=$dn 0 $dndata{$dn}{rtpport} IN IP4 $dndata{$dn}{rtpip}\n";
	$$psdp .= "s=-\n";
	$$psdp .= "c=IN IP4 $dndata{$dn}{rtpip}\n";
	$$psdp .= "t=0 0\n";
	if ($dndata{$dn}{codec} eq "pcma") {
		# a-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 8\n";
		$$psdp .= "a=rtpmap:8 pcma/$dndata{$dn}{rtpport}\n";
	} else {
		# default is mu-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 0\n";
		$$psdp .= "a=rtpmap:0 pcmu/$dndata{$dn}{rtpport}\n";
	}
}
#
sub makeOfferSDP {
	my ($dn, $offercodec, $psdp) = @_;
	#
	$$psdp = "";
	$$psdp .= "v=0\n";
	$$psdp .= "o=$dn 0 $dndata{$dn}{rtpport} IN IP4 $dndata{$dn}{rtpip}\n";
	$$psdp .= "s=-\n";
	$$psdp .= "c=IN IP4 $dndata{$dn}{rtpip}\n";
	$$psdp .= "t=0 0\n";
	if (defined($offercodec) && $offercodec eq "pcma") {
		# a-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 8\n";
		$$psdp .= "a=rtpmap:8 pcma/$dndata{$dn}{rtpport}\n";
	} elsif (defined($offercodec) && $offercodec eq "pcmu") {
		# mu-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 0\n";
		$$psdp .= "a=rtpmap:0 pcmu/$dndata{$dn}{rtpport}\n";
	} elsif ($dndata{$dn}{codec} eq "pcma") {
		# a-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 8\n";
		$$psdp .= "a=rtpmap:8 pcma/$dndata{$dn}{rtpport}\n";
	} else {
		# default is mu-law
		$$psdp .= "m=audio $dndata{$dn}{rtpport} RTP/AVP 0\n";
		$$psdp .= "a=rtpmap:0 pcmu/$dndata{$dn}{rtpport}\n";
	}
}
#
sub parseDataBlock {
	my ($dblock) = @_;
	#
	my $dblklen = length($dblock);
	#
vprint "parseDataBlock: data=<$dblock>\n";
	if ($dblock =~ /Content-Type: *application\/sdp(\r\n)+/g) {
		# sdp data block
		my $offset = pos($dblock);
		vprint "\nFound an SDP data block (length=$dblklen,offset=$offset):\n";
		if ($dblock =~ /Content-Disposition:[^\r\n]*[\r\n]+/g) {
			$offset = pos($dblock);
			vprint "\nFound an SDP data block (length=$dblklen,offset=$offset):\n";
		}
		# strip headers
		$dblock = substr($dblock, $offset);
		$dblklen = length($dblock);
		vprint "\nFound an SDP data block (length=$dblklen):\n";
		# 
		my @sdpdata = unpack "N*", $dblock;
		vprint "SDP Data is: @sdpdata\n";
	} elsif ($dblock =~ /Content-Type: *application\/ISUP[^\r\n]*(\r\n)+/g) {
		# isup data block
		my $offset = pos($dblock);
		vprint "\nFound an ISUP data block (length=$dblklen,offset=$offset):\n";
		if ($dblock =~ /Content-Disposition:[^\r\n]*[\r\n]+/g) {
			$offset = pos($dblock);
			vprint "\nFound an ISUP data block (length=$dblklen,offset=$offset):\n";
		}
		# strip headers
		$dblock = substr($dblock, $offset);
		$dblklen = length($dblock);
		vprint "\nFound an ISUP data block (length=$dblklen):\n";
		# store as a long array for easier processing.
		my @Nisupdata = unpack "N*", $dblock;
		vprint "ISUP Data is (network-order long, 32 bits): @Nisupdata\n";
		my @Hisupdata = unpack "H*", $dblock;
		vprint "ISUP Data is (Hex, high-nibble first): @Hisupdata\n";
		# start decoding msgs
		$isup->decode($parameters{isupversion}, $dblock);
	} else {
		# unknown data block type.
		vprint "\nFound an UNKNOWN data block (length=$dblklen):\n";
	}
}
#
sub parseSipMsg {
	my ($rawmsg, $prawhdr, $prawdata, $phdrs, 
		$pdatablocks, $reinvite, $fileno) = @_;
	#
	# separate the hdr from the payload.
	#
	($$prawhdr, $$prawdata) = split /[\r\n]{4,4}/, $rawmsg, 2;
	return(2) unless (defined($$prawhdr));
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
		$phdrs->{requesturi} = undef;
	} else {
		$phdrs->{requesturi} = $fld2;
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
	# weird problem, "Call-ID" is written as "Call-Id". make sure
	# both versions exist.
	#
	my $ver1callid = "Call-ID";
	my $ver2callid = "Call-Id";
	if (defined($phdrs->{$ver1callid})) {
		$phdrs->{$ver2callid} = $phdrs->{$ver1callid};
	} elsif (defined($phdrs->{$ver2callid})) {
		$phdrs->{$ver1callid} = $phdrs->{$ver2callid};
	} else {
		printit "parseSipMsg: unknown Call-ID Form.\n";
		return(404);
	}
	#
	# get data specific to msg type.
	#
	$phdrs->{"invitefromdn"} = "";
	#
	my $dummy = undef;
	if ($phdrs->{msgtype} =~ /^REGISTER/) {
		$dummy = $phdrs->{"To"};
		$dummy =~ /^.*[<]*sip:([0-9]+)@.*$/;
		$dummy = $1;
		if (defined($dummy)) {
			$phdrs->{"dn"} = $dummy;
		}
		# check if this DN is even known and if it
		# is handled by this simulator.
		if (!defined($dndata{$phdrs->{"dn"}}) ||
		   ($dndata{$phdrs->{"dn"}}{sipip} ne $parameters{mysipip})) {
			printit "parseSipMsg: unknown DN ".$phdrs->{"dn"}."\n";
			return(404);
		}
		#
		my $callid = $phdrs->{"Call-ID"};
		if (!defined($dialogs{$callid})) {
			# save call-id for this DN.
			$dndata{$phdrs->{"dn"}}{registrationcallid} = $callid;
			# first time, create dialog
			$dialogs{$callid}{dn} = $phdrs->{"dn"};
			# get transport type
			if ($phdrs->{"Via"} =~ /\/UDP/) {
				$dialogs{$callid}{transport} = "UDP";
			} elsif ($phdrs->{"Via"} =~ /\/TCP/) {
				$dialogs{$callid}{transport} = "TCP";
			} else {
				die "parseSipMsg: unknown transport in Via";
			}
			$dialogs{$callid}{fileno} = $fileno;
			if ($dialogs{$callid}{transport} eq "TCP") {
				# get local IP/Port.
				my $paddr = getsockname($fd2file[$fileno]);
				my ($port, $ip) = sockaddr_in($paddr);
				$ip = inet_ntoa($ip);
				$dialogs{$callid}{sipip} = $ip;
				$dialogs{$callid}{sipport} = $port;
			} elsif ($dialogs{$callid}{transport} eq "UDP") {
				$dialogs{$callid}{sipip} = 
					$dndata{$
			} else {
				die "parseSipMsg: unknown transport";
			}
			

			#
			$dialogs{$callid}{contact} = $phdrs->{"Contact"};
			$dialogs{$callid}{mycontact} = 
				dn2contact($phdrs->{"dn"});
			$dialogs{$callid}{cseq} = 
				getCseqNo($phdrs->{"CSeq"});
			$dialogs{$callid}{handler} = 
				$dndata{$phdrs->{"dn"}}{registrar};
			$dialogs{$callid}{"timertag"} = 0;
			$dialogs{$callid}{"mytag"} = int(rand(1000000000));
			$dialogs{$callid}{"mytag"} .= "mytag";
			# save additional fields when timers expire and
			# we need to send msgs.
			$dialogs{$callid}{peer} = $phdrs->{"From"};
			$dialogs{$callid}{from} = $phdrs->{"From"};
			$dialogs{$callid}{to} = $phdrs->{"To"};
			# assign the tag at this point.
			$dialogs{$callid}{local} = $phdrs->{"To"};
			$dialogs{$callid}{local} .= ";tag=".$dialogs{$callid}{mytag};
			$dialogs{$callid}{via} = $phdrs->{"Via"};
			$dialogs{$callid}{maxforwards} = 
				$phdrs->{"Max-Forwards"};
			$dialogs{$callid}{requesturi} = 
				makerequesturi($callid, "contact");
			$dialogs{$callid}{pchargingvector} = 
				$dndata{$phdrs->{"dn"}}{pchargingvector};
		}
	} elsif ($phdrs->{msgtype} =~ /^INVITE/) {
		if ($phdrs->{"To"} =~ /__sip-annc/) {
			$dummy = "__sip-annc";
		} else {
			$dummy = $phdrs->{"To"};
			$dummy =~ /^.*[<]*sip:([0-9]+)@.*$/;
			$dummy = $1;
			# we may have a redirect. for the
			# redirect case, the request uri and 
			# To will be different. in this case, we 
			# want to use the DN in the request uri 
			# instead of the DN in the To.
			my $dummy2 = $phdrs->{"requesturi"};
			$dummy2 =~ /^.*[<]*sip:([0-9]+)@.*$/;
			$dummy2 = $1;
			if ($dummy ne $dummy2) {
				$dummy = $dummy2;
			}
		}
		if (defined($dummy)) {
			$phdrs->{"dn"} = $dummy;
		}
		#
		# remove isup-oli field.
		$dummy = $phdrs->{"From"};
		# if ($dummy =~ /^(.*);isup-oli=[^;]*(;.*)$/) {
			# $dummy = $1;
			# $dummy .= $2;
		# }
		# $phdrs->{"From"} = $dummy;
		#
		$dummy =~ /^.*[<]*sip:([0-9]+)@.*$/;
		$dummy = $1;
		if (defined($dummy)) {
			$phdrs->{"invitefromdn"} = $dummy;
		}
		#
		# check if this DN is even known and if it
		# is handled by this simulator.
		#
		if (!defined($dndata{$phdrs->{"dn"}}) ||
		   ($dndata{$phdrs->{"dn"}}{sipip} ne $parameters{mysipip})) {
			printit "parseSipMsg: unknown DN ".$phdrs->{"dn"}."\n";
			return(404);
		}
		#
		my $callid = $phdrs->{"Call-ID"};
		if (!defined($dialogs{$callid})) {
			# save call-id for this DN.
			$dndata{$phdrs->{"dn"}}{terminationcallid} = $callid;
			# first time, create dialog
			$dialogs{$callid}{dn} = $phdrs->{"dn"};
			$dialogs{$callid}{contact} = $phdrs->{"Contact"};
			$dialogs{$callid}{mycontact} = 
				dn2contact($phdrs->{"dn"});
			$dialogs{$callid}{cseq} = 
				getCseqNo($phdrs->{"CSeq"});
			$dialogs{$callid}{handler} = 
				$dndata{$phdrs->{"dn"}}{termination};
			$dialogs{$callid}{"timertag"} = 0;
			$dialogs{$callid}{"mytag"} = int(rand(1000000000));
			$dialogs{$callid}{"mytag"} .= "mytag";
			$dialogs{$callid}{subscribeevent} = 
				$dndata{$phdrs->{"dn"}}{subscribeevent};
			$dialogs{$callid}{expires} = 
				$dndata{$phdrs->{"dn"}}{expires};
			$dialogs{$callid}{redirectcontact} = 
				$dndata{$phdrs->{"dn"}}{redirectcontact};
			$dialogs{$callid}{redirectcode} = 
				$dndata{$phdrs->{"dn"}}{redirectcode};
			$dialogs{$callid}{returncode} = 
				$dndata{$phdrs->{"dn"}}{returncode};
			$dialogs{$callid}{pchargingvector} = 
				$dndata{$phdrs->{"dn"}}{pchargingvector};
			# save additional fields when timers expire and
			# we need to send msgs.
			$dialogs{$callid}{peer} = $phdrs->{"From"};
			$dialogs{$callid}{from} = $phdrs->{"From"};
			$dialogs{$callid}{to} = $phdrs->{"To"};
			# assign the tag at this point.
			$dialogs{$callid}{local} = $phdrs->{"To"};
			if ( ! $reinvite) {
				$dialogs{$callid}{local} .= ";tag=".$dialogs{$callid}{mytag};
			}
			$dialogs{$callid}{via} = $phdrs->{"Via"};
			$dialogs{$callid}{maxforwards} = 
				$phdrs->{"Max-Forwards"};
			$dialogs{$callid}{requesturi} = 
				makerequesturi($callid, "contact");
			# ISUP msgs
			$dialogs{$callid}{useisup} = 0;
			$dialogs{$callid}{isupmsgfile} = 
				$dndata{$phdrs->{"dn"}}{isupmsgfile};
			$dialogs{$callid}{terminvite200okmsgid} =
				$dndata{$phdrs->{"dn"}}{terminvite200okmsgid};
			$dialogs{$callid}{term180ringingmsgid} =
				$dndata{$phdrs->{"dn"}}{term180ringingmsgid};
			$dialogs{$callid}{term183ringingmsgid} =
				$dndata{$phdrs->{"dn"}}{term183ringingmsgid};
			$dialogs{$callid}{term200okbyemsgid} =
				$dndata{$phdrs->{"dn"}}{term200okbyemsgid};
			$dialogs{$callid}{term200okcancelmsgid} =
				$dndata{$phdrs->{"dn"}}{term200okcancelmsgid};
			$dialogs{$callid}{term200okmsgid} =
				$dndata{$phdrs->{"dn"}}{term200okmsgid};
			$dialogs{$callid}{term486busymsgid} =
				$dndata{$phdrs->{"dn"}}{term486busymsgid};
			$dialogs{$callid}{termbyemsgid} =
				$dndata{$phdrs->{"dn"}}{termbyemsgid};
			$dialogs{$callid}{termpost100infomsgid} = 
				$dndata{$phdrs->{"dn"}}{termpost100infomsgid};
			$dialogs{$callid}{termpost180infomsgid} = 
				$dndata{$phdrs->{"dn"}}{termpost180infomsgid};
			$dialogs{$callid}{termpost183infomsgid} = 
				$dndata{$phdrs->{"dn"}}{termpost183infomsgid};
			$dialogs{$callid}{termpost200infomsgid} = 
				$dndata{$phdrs->{"dn"}}{termpost200infomsgid};
			$dialogs{$callid}{termpostackinfomsgid} = 
				$dndata{$phdrs->{"dn"}}{termpostackinfomsgid};
			#
			$dialogs{$callid}{terminvite200oksdp} =
				$dndata{$phdrs->{"dn"}}{terminvite200oksdp};
			$dialogs{$callid}{term180ringingsdp} =
				$dndata{$phdrs->{"dn"}}{term180ringingsdp};
			$dialogs{$callid}{term183ringingsdp} =
				$dndata{$phdrs->{"dn"}}{term183ringingsdp};
			$dialogs{$callid}{term200okbyesdp} =
				$dndata{$phdrs->{"dn"}}{term200okbyesdp};
			$dialogs{$callid}{term200okcancelsdp} =
				$dndata{$phdrs->{"dn"}}{term200okcancelsdp};
			$dialogs{$callid}{term200oksdp} =
				$dndata{$phdrs->{"dn"}}{term200oksdp};
			$dialogs{$callid}{term200okupdatesdp} =
				$dndata{$phdrs->{"dn"}}{term200okupdatesdp};
			$dialogs{$callid}{term486busysdp} =
				$dndata{$phdrs->{"dn"}}{term486busysdp};
			$dialogs{$callid}{termbyesdp} =
				$dndata{$phdrs->{"dn"}}{termbyesdp};
			$dialogs{$callid}{termpost100updatesdp} = 
				$dndata{$phdrs->{"dn"}}{termpost100updatesdp};
			$dialogs{$callid}{termpost180updatesdp} = 
				$dndata{$phdrs->{"dn"}}{termpost180updatesdp};
			$dialogs{$callid}{termpost183updatesdp} = 
				$dndata{$phdrs->{"dn"}}{termpost183updatesdp};
			$dialogs{$callid}{termpost200updatesdp} = 
				$dndata{$phdrs->{"dn"}}{termpost200updatesdp};
			$dialogs{$callid}{termpostackupdatesdp} = 
				$dndata{$phdrs->{"dn"}}{termpostackupdatesdp};
		}
	}
	#
	if ($phdrs->{msgtype} =~ /^RETURNCODE/) {
		# grab the To: URI for future msgs.
		my $callid = $phdrs->{"Call-ID"};
		$dialogs{$callid}{peer} = $phdrs->{"To"};
		#
		my $cseqmeth = undef;
		if (defined($phdrs->{"CSeq"})) {
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
		} 
		if (!defined($cseqmeth)) {
			$cseqmeth = "UNKNOWNMETHOD";
		}
		incrstats("msgs-received", 
			"RETURNCODE".$phdrs->{returncode}.$cseqmeth);
	} else {
		incrstats("msgs-received", $phdrs->{msgtype});
	}
	#
	# any data blocks?
	#
	if ($phdrs->{"Content-Length"} == 0) {
		# no data
		return(0);
	}
	#
	# split blocks
	#
	if ($phdrs->{"Content-Type"} =~ /^multipart\/mixed/) {
		# multiple data blocks, get data block delimiter
		$phdrs->{"Content-Type"} =~ 
			/^multipart\/mixed.*boundary=\"*([^\"]+)\"*/;
		my $dbdelimiter = $1;
		die "delimiter not found in Content-Type = mixed." unless 
			defined($dbdelimiter);
		# split out blocks
		@{$pdatablocks} = split /${dbdelimiter}/, $$prawdata;
		my $ndblks = scalar(@{$pdatablocks});
		@{$pdatablocks} = @{$pdatablocks}[1..($ndblks-2)];
		$ndblks = scalar(@{$pdatablocks});
		vprint "\nFound ${ndblks} blocks in multipart/mixed payload.\n";
		#
		for (my $dblk=0; $dblk<$ndblks; ${dblk}++) {
			parseDataBlock($pdatablocks->[$dblk]);
		}
	} elsif ($phdrs->{"Content-Type"} =~ /application.*sdp/) {
		my $callid = $phdrs->{"Call-ID"};
		$dialogs{$callid}{peersdp} = $$prawdata;
		vprint "\nparseSipMsg: peersdp = ".$dialogs{$callid}{peersdp}."\n";
		@{$pdatablocks} = [ $$prawdata ];
		my $ndblks = scalar(@{$pdatablocks});
		vprint "\nFound ${ndblks} blocks in application/sdp payload.\n";
		#
		for (my $dblk=0; $dblk<$ndblks; ${dblk}++) {
			parseDataBlock($pdatablocks->[$dblk]);
		}
	} else {
		printit "\nWARNING: unknown Content-Type = ";
		printit $phdrs->{"Content-Type"}."\n";
		printit "Setting Content-Length to ZERO.\n";
		$phdrs->{"Content-Length"} = 0;
	}
	return(0);
}
#
sub changeDnState {
	my ($dn, $dnstate, $newstate) = @_;
	#
	my $olddnstate = $dndata{$dn}{$dnstate};
	$dndata{$dn}{$dnstate} = $newstate;
	if ($newstate eq "INACTIVE") {
		$dndata{$dn}{callloadstate} = "INACTIVE";
	}
	return unless ($verboseflag);
	vprint("\nOLD $dnstate STATE FOR $dn: ${olddnstate}\n");
	vprint("NEW $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
#
sub dumpDnState {
	my ($dn, $dnstate) = @_;
	#
	return unless ($verboseflag);
	#
	vprint("\nCURRENT $dnstate STATE FOR $dn: $dndata{$dn}{$dnstate}\n");
}
#
sub isMyTimer {
	my ($callid, $ptimer) = @_;
	#
	my $dialogtag = $dialogs{$callid}{"timertag"};
	my $timertag = $ptimer->{expire};
	#
	my $dialogid = $dialogs{$callid}{"timerid"};
	my $timerid = $ptimer->{id};
	#
	if ($dialogtag != $timertag || $dialogid != $timerid) {
		# it is not my timer
		if ($verboseflag) {
			vprint "\nTimer label: ".$ptimer->{label}."\n";
			vprint "Ignoring old timer (tag=$timertag).\n";
			vprint "(callid=$callid).\n";
		}
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
sub removetimer {
	my ($callid) = @_;
	#
	if (defined($dialogs{$callid}{"ptimer"})) {
		my $ptimer = $dialogs{$callid}{"ptimer"};
		$pq->remove($ptimer);
		$ptimer = undef;
	}
}
#
sub starttimer {
	my ($callid, $delta, $label) = @_;
	#
	my $endtime = time() + $delta;
	my $timerid = int(rand(1000000000));
	if ($delta <= 0) {
		vprint "\n\n\n";
		vprint "starttimer: ZERO LENGTH TIMER FOR CALL-ID $callid.\n";
		vprint "\n\n\n";
	}
	#
	if ($verboseflag) {
		vprint "\nstarttimer:\n";
		vprint "\tcallid=${callid}\n";
		vprint "\tlabel=${label}\n";
		vprint "\tdelta=${delta}\n";
		vprint "\tendtime=$endtime\n";
		vprint "\tid=$timerid\n";
	}
	#
	my $ptimer = "mytimer"->new($callid, $endtime, $timerid, $label);
	#
	$dialogs{$callid}{"timertag"} = $endtime;
	$dialogs{$callid}{"timerid"} = $timerid;
	$dialogs{$callid}{"ptimer"} = $ptimer;
	#
	if ($verboseflag) {
		vprint "\tcallid = ".$ptimer->{callid}."\n";
		vprint "\texpire = ".$ptimer->{expire}."\n";
		vprint "\tid     = ".$ptimer->{id}."\n";
		vprint "\tlabel  = ".$ptimer->{label}."\n";
	}
	#
	$pq->enqueue($ptimer);
	incrstats("general","starttimerenqueuecalled");
}
#
sub makepaddr {
	my ($contact, $callid) = @_;
	#
	my $dn;
	my $ip;
	my $port;
	#
	my $dcontact = $dialogs{$callid}{$contact};
	if ($dcontact =~ /^.*<*sip:([^@]+)@([^:]+):([0-9]+).*$/) {
		$dn = $1;
		$ip = $2;
		$port = $3;
		vprint "makepaddr: found dn,ip,port, no defaults ...\n";
	} elsif ($dcontact =~ /^.*<*sip:([^:]+):([0-9]+).*$/) {
		$ip = $1;
		$port = $2;
		vprint "makepaddr: found ip,port, no defaults ...\n";
	} elsif ($dcontact =~ /^.*<*sip:([^@]+)@([0-9\.]+).*$/) {
		$dn = $1;
		$ip = $2;
		$port = 5060;
		vprint "makepaddr: found dn,ip, default port 5060 ...\n";
	} elsif ($dcontact =~ /^.*<*sip:([0-9\.]+).*$/) {
		$ip = $1;
		$port = 5060;
		vprint "makepaddr: found ip, default port 5060 ...\n";
	} else {
		printit "makepaddr: unable to get IP addr from contact !!!\n";
		printit "contact = ${dcontact}\n";
		confess "makepaddr exiting.";
	}
	#
	my $ipaddr = gethostbyname($ip);
	defined($ipaddr) or die "gethostbyname: $!";
	my $paddr = sockaddr_in($port, $ipaddr);
	defined($paddr) or die "sockaddr_in: $!";
	#
	return($paddr);
}
#
sub sendRegister {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "REGISTER ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." REGISTER\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	$ackmsg .= "Expires: ".$dndata{$dn}{"expires"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "BYE");
	return;
}
#
sub sendRefer {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "REFER ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." REFER\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	$ackmsg .= "Refer-To: ".$dndata{$dn}{referto}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "REFER");
	return;
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
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
	$ackmsg .= "Expires: ".$dndata{$dn}{"expires"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
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
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE180".$cseqmeth);
	return;
}
#
sub sendInvite183Ringing {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 183 Session Progress\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE183".$cseqmeth);
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
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	# dprint "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub sendSubscribe {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "SUBSCRIBE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." SUBSCRIBE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	$ackmsg .= "Event: ".$dialogs{$callid}{subscribeevent}."\r\n";
	$ackmsg .= "Expires: ".$dialogs{$callid}{expires}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "SUBSCRIBE");
	return;
}
#
sub sendNotify {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "NOTIFY ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." NOTIFY\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "NOTIFY");
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
	my $ackmsg = "BYE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." BYE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "BYE");
	return;
}
#
sub sendInviteAck {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	# dprint "OFFER SDP: $sdp\n";
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
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
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
#
sub sendCancel {
	my ($callid, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	# flip To and From fields in the BYE msg.
	#
	my $ackmsg = "CANCEL ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." CANCEL\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "CANCEL");
	return;
}
#
sub sendInvite {
	my ($callid, $contact, $isannc) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "INVITE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	if (length($dialogs{$callid}{"isupoli"}) > 0) {
		$ackmsg .= "From: ".$dialogs{$callid}{localisupoli}."\r\n";
	} else {
		$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	}
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
	if (defined($dialogs{$callid}{invitesubject}) &&
	   (length($dialogs{$callid}{invitesubject}) > 0)) {
		$ackmsg .= "Subject: ".$dialogs{$callid}{invitesubject}."\r\n";
	}
	if ($dialogs{$callid}{usepassertedidentity}) {
		if (length($dialogs{$callid}{passertedidentity}) > 0) {
			$ackmsg .= "P-Asserted-Identity: ".$dialogs{$callid}{passertedidentity}."\r\n";
		} else {
			$ackmsg .= "P-Asserted-Identity: ".$dialogs{$callid}{local}."\r\n";
		}
	}
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." INVITE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $sdp = "";
	if ($isannc) {
		$sdp = $dialogs{$callid}{peersdp};
		vprint "ANNC OFFER PEER SDP: $sdp\n";
	} else {
		makeSDP($dn, \$sdp);
		vprint "OFFER SDP: $sdp\n";
	}
	#
	my $sdplen = length($sdp);
	$ackmsg .= "Content-Type: application/sdp\r\n";
	$ackmsg .= "Content-Length: $sdplen\r\n";
	$ackmsg .= "\r\n";
	$ackmsg .= "$sdp";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "INVITE");
	return;
}
#
sub sendGratuitous200OK {
	# sometime we get BYE or CANCEL msgs after a call
	# dies. this keeps up until the other side times out
	# or until the maximum retries is reached. to shut these
	# msgs up, we take the CANCEL or BYE and return a 200 OK.
	#
	my ($phdrs) = @_;
	#
	if (!(($phdrs->{msgtype} =~ /BYE/) ||
	      ($phdrs->{msgtype} =~ /OPTION/) ||
	      ($phdrs->{msgtype} =~ /CANCEL/))) {
		# not a BYE or CANCEL
		return;
	}
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $contact = $phdrs->{"Contact"};
	return unless defined($contact);
	#
	my ($dn, $ip, $port) = contact2dnipport($phdrs->{"Contact"});
	#
	return unless defined($dn);
	return unless defined($ip);
	return unless defined($port);
	#
	my $ipaddr = gethostbyname($ip);
	return unless defined($ipaddr);
	my $paddr = sockaddr_in($port, $ipaddr);
	return unless defined($paddr);
	#
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "RETURNCODE200".$phdrs->{msgtype}."-GRATUITOUS");
	return;
}
#
sub send404NotFound {
	# unknown DN 
	my ($phdrs) = @_;
	#
	if (!($phdrs->{msgtype} =~ /INVITE/)) {
		# not an INVITE
		return;
	}
	#
	my $ackmsg = "SIP/2.0 404 Not Found\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my ($dn, $ip, $port) = contact2dnipport($phdrs->{"Contact"});
	#
	return unless defined($dn);
	return unless defined($ip);
	return unless defined($port);
	#
	my $ipaddr = gethostbyname($ip);
	return unless defined($ipaddr);
	my $paddr = sockaddr_in($port, $ipaddr);
	return unless defined($paddr);
	#
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "RETURN404NOTFOUND".$phdrs->{msgtype});
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
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub send486Busy {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 486 Busy\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE486".$cseqmeth);
	return;
}
#
sub sendXXXreturn {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 ".$dialogs{$callid}{returncode}." Busy\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE486".$cseqmeth);
	return;
}
#
sub send302redirect {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 ".$dialogs{$callid}{redirectcode}." Moved Temporarily\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{redirectcontact}."\r\n";
	$ackmsg .= "Content-Length: 0\r\n";
	$ackmsg .= "\r\n";
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, undef);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE486".$cseqmeth);
	return;
}
#
##########################################################################
#
# high-level msg handlers
#
sub register {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry register: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
			vprint "\nregistration INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
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
			deleteCallId($callid);
			changeDnState($dn, "registration", "INACTIVE");
			queueReg($dn, $dndata{$dn}{interregistertime});
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			removetimer($callid);
			if ($phdrs->{returncode} == 200) {
				# save expire time returned from registrar.
				$dndata{$dn}{"expires"} = $phdrs->{"Expires"};
				starttimer($callid, $dndata{$dn}{"expires"},
					"REGISTRATION EXPIRES");
				changeDnState($dn, "registrationstate", "REGISTERED");
			} else {
				# registration denied. try again.
				deleteCallId($callid);
				changeDnState($dn, "registration", "INACTIVE");
				queueReg($dn, $dndata{$dn}{interregistertime});
			}
		} else {
			vprint "\nregistration WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
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
			vprint "\nregistration TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
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
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry registrar: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
		removetimer($callid);
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
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry origination: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
				dumptimer($timerflag, $ptimer);
				incrstats("origination","invalidtimers-1");
				$ptimer = undef;
				return;
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			$dialogs{$callid}{invitecseq} = $dialogs{$callid}{cseq};
			sendInvite($callid, "contact", 0);
			incrstats("origination","started");
			incrstats("origination","sendInvite");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "originationstate", "WAITFOR200");
		} else {
			if ($timerflag) {
				dumptimer($timerflag, $ptimer);
				incrstats("origination","invalidtimers-2");
			} else {
				incrstats("origination","unexpectedmsgs-1");
				incrstats("origination-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			}
			vprint "\norigination INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
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
			changeDnState($dn, "originationstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			# now we have the far-end URI.
			$dialogs{$callid}{peer} = $phdrs->{"To"};
			#
			if ($phdrs->{returncode} == 100) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("origination","received100Invite");
			} elsif ($phdrs->{returncode} == 180) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("origination","received180Invite");
			} elsif ($phdrs->{returncode} == 183) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("origination","received183Invite");
			} elsif ($phdrs->{returncode} == 200) {
				sendInviteAck($callid, $phdrs, "contact");
				incrstats("origination","sendAck");
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKING");
				incrstats("origination","stable");
				incrstats("origination","received200Invite");
			} elsif ($phdrs->{returncode} == 486) {
				incrstats("origination", "received486busy");
				sendAck($callid, $phdrs, "contact");
				incrstats("origination","send486Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("origination", "inactive");
			} elsif ($phdrs->{returncode} == 503) {
				incrstats("origination", "received503serviceunavailable");
				sendAck($callid, $phdrs, "contact");
				incrstats("origination","send503Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("origination", "inactive");
			} elsif ($phdrs->{returncode} =~ /^3[0-9][0-9]/) {
				sendAck($callid, $phdrs, "contact");
				incrstats("origination","sendAck");
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("origination", "inactive");
				#
				my ($ntdn, $ntdnip, $ntdnport) = 
					contact2dnipport($phdrs->{"Contact"});
				vprint "\norigination: new tdn is ... $ntdn\n";
				vprint "origination: new ntdnip is ... $ntdnip\n";
				vprint "origination: new ntdnport is ... $ntdnport\n";
				queueOrig($dn, $ntdn, 1, 0, undef, $ntdnip, $ntdnport);
			} else {
				incrstats("origination", "receivedXXXstatus");
				sendAck($callid, $phdrs, "contact");
				incrstats("origination","sendXXXAck");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("origination", "inactive");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} else {
			incrstats("origination","unexpectedmsgs-2");
			incrstats("origination-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\norigination WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
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
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
			incrstats("origination","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} elsif ($phdrs->{msgtype} =~ /SUBSCRIBE/) {
			$dialogs{$callid}{cseq} += 1;
			sendNotify($callid, "contact");
			incrstats("origination","sendNotify");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			if ($phdrs->{returncode} == 200) {
				incrstats("origination", "received200notify");
			} else {
				incrstats("origination", "receivedXXXnotify");
			}
		} else {
			incrstats("origination","unexpectedmsgs-3");
			incrstats("origination-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\norigination TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("origination","invalidtimers-5");
				if ($timerflag == 2) {
					incrstats("origination","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
			incrstats("origination","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
		} else {
			incrstats("origination","unexpectedmsgs-4");
			incrstats("origination-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\norigination WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("origination","invalidtimers-6");
				if ($timerflag == 2) {
					incrstats("origination","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
			incrstats("origination","timedout-4");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("origination","inactive");
			incrstats("origination","waitfor200cancel-received200OK");
		} else {
			incrstats("origination","unexpectedmsgs-5");
			incrstats("origination-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\norigination WAITFOR200CANCEL warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\norigination - unknown state $origstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("origination","invalidtimers-7");
			if ($timerflag == 2) {
				incrstats("origination","notreadyfororig");
			}
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("origination","unexpectedmsgs-6");
			incrstats("origination-unexpectedmsgs-6", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub originationreqannc {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry originationreqannc: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $anncstate = $dndata{$dn}{announcementstate};
	dumpDnState($dn, "announcementstate");
	#
	# check for originations
	#
	if ($anncstate =~ /^INACTIVE$/) {
		# only call orig timers are allowed.
		if ($timerflag == 2) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-1");
				$ptimer = undef;
				return;
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			$dialogs{$callid}{invitecseq} = $dialogs{$callid}{cseq};
			sendInvite($callid, "contact", 1);
			incrstats("originationreqannc","started");
			incrstats("originationreqannc","sendInvite");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "announcementstate", "WAITFOR200");
		} else {
			if ($timerflag) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-2");
			} else {
				incrstats("originationreqannc","unexpectedmsgs-1");
				incrstats("originationreqannc-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			}
			vprint "\noriginationreqannc INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($anncstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-3");
				if ($timerflag == 2) {
					incrstats("originationreqannc","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendCancel($callid, "contact");
			incrstats("originationreqannc","sendCancel");
			incrstats("originationreqannc","timedout-1");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 CANCEL");
			changeDnState($dn, "announcementstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			# now we have the far-end URI.
			$dialogs{$callid}{peer} = $phdrs->{"To"};
			#
			if ($phdrs->{returncode} == 100) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "announcementstate", "WAITFOR200");
				incrstats("originationreqannc","received100Invite");
			} elsif ($phdrs->{returncode} == 180) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "announcementstate", "WAITFOR200");
				incrstats("originationreqannc","received180Invite");
			} elsif ($phdrs->{returncode} == 183) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "announcementstate", "WAITFOR200");
				incrstats("originationreqannc","received183Invite");
			} elsif ($phdrs->{returncode} == 200) {
				sendInviteAck($callid, $phdrs, "contact");
				incrstats("originationreqannc","sendAck");
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "announcementstate", "TALKING");
				incrstats("originationreqannc","stable");
				incrstats("originationreqannc","received200Invite");
			} elsif ($phdrs->{returncode} == 486) {
				incrstats("originationreqannc", "received486busy");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationreqannc","send486Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "announcementstate", "INACTIVE");
				incrstats("originationreqannc", "inactive");
			} elsif ($phdrs->{returncode} == 503) {
				incrstats("originationreqannc", "received503serviceunavailable");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationreqannc","send503Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "announcementstate", "INACTIVE");
				incrstats("originationreqannc", "inactive");
			} else {
				incrstats("originationreqannc", "receivedXXXstatus");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationreqannc","sendXXXAck");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "announcementstate", "INACTIVE");
				incrstats("originationreqannc", "inactive");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} else {
			incrstats("originationreqannc","unexpectedmsgs-2");
			incrstats("originationreqannc-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\noriginationreqannc WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($anncstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-4");
				if ($timerflag == 2) {
					incrstats("originationreqannc","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("originationreqannc","sendBye");
			incrstats("originationreqannc","timedout-2");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "announcementstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
			incrstats("originationreqannc","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /SUBSCRIBE/) {
			$dialogs{$callid}{cseq} += 1;
			sendNotify($callid, $phdrs, "contact");
			incrstats("originationreqannc","sendNotify");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			if ($phdrs->{returncode} == 200) {
				incrstats("originationreqannc", "received200notify");
			} else {
				incrstats("originationreqannc", "receivedXXXnotify");
			}
		} else {
			incrstats("originationreqannc","unexpectedmsgs-3");
			incrstats("originationreqannc-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\noriginationreqannc TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($anncstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-5");
				if ($timerflag == 2) {
					incrstats("originationreqannc","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
			incrstats("originationreqannc","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("originationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
		} else {
			incrstats("originationreqannc","unexpectedmsgs-4");
			incrstats("originationreqannc-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\noriginationreqannc WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($anncstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationreqannc","invalidtimers-6");
				if ($timerflag == 2) {
					incrstats("originationreqannc","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
			incrstats("originationreqannc","timedout-4");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "announcementstate", "INACTIVE");
			incrstats("originationreqannc","inactive");
			incrstats("originationreqannc","waitfor200cancel-received200OK");
		} else {
			incrstats("originationreqannc","unexpectedmsgs-5");
			incrstats("originationreqannc-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\noriginationreqannc WAITFOR200CANCEL warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\noriginationreqannc - unknown state $anncstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("originationreqannc","invalidtimers-7");
			if ($timerflag == 2) {
				incrstats("originationreqannc","notreadyfororig");
			}
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("originationreqannc","unexpectedmsgs-6");
			incrstats("originationreqannc-unexpectedmsgs-6", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub originationrefer {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry originationrefer: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-1");
				$ptimer = undef;
				return;
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			$dialogs{$callid}{invitecseq} = $dialogs{$callid}{cseq};
			sendInvite($callid, "contact", 0);
			incrstats("originationrefer","started");
			incrstats("originationrefer","sendInvite");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "originationstate", "WAITFOR200");
		} else {
			if ($timerflag) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-2");
			} else {
				incrstats("originationrefer","unexpectedmsgs-1");
				incrstats("originationrefer-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			}
			vprint "\noriginationrefer INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-3");
				if ($timerflag == 2) {
					incrstats("originationrefer","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendCancel($callid, "contact");
			incrstats("originationrefer","sendCancel");
			incrstats("originationrefer","timedout-1");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 CANCEL");
			changeDnState($dn, "originationstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			# now we have the far-end URI.
			$dialogs{$callid}{peer} = $phdrs->{"To"};
			#
			if ($phdrs->{returncode} == 100) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationrefer","received100Invite");
			} elsif ($phdrs->{returncode} == 180) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationrefer","received180Invite");
			} elsif ($phdrs->{returncode} == 183) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationrefer","received183Invite");
			} elsif ($phdrs->{returncode} == 200) {
				sendInviteAck($callid, $phdrs, "contact");
				incrstats("originationrefer","sendAck");
				removetimer($callid);
				incrstats("originationrefer","stable");
				incrstats("originationrefer","received200Invite");
				# transfer to wait before REFER
				starttimer($callid, $dndata{$dn}{"prerefercallduration"},
					"TALKING PRE-REFER CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKINGBEFOREREFER");
				incrstats("originationrefer","startingprereferstate");
			} elsif ($phdrs->{returncode} == 486) {
				incrstats("originationrefer", "received486busy");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationrefer","send486Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationrefer", "inactive");
			} elsif ($phdrs->{returncode} == 503) {
				incrstats("originationrefer", "received503serviceunavailable");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationrefer","send503Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationrefer", "inactive");
			} else {
				incrstats("originationrefer", "receivedXXXstatus");
				sendAck($callid, $phdrs, "contact");
				incrstats("originationrefer","sendXXXAck");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationrefer", "inactive");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OKCancel");
			#
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} else {
			incrstats("originationrefer","unexpectedmsgs-2");
			incrstats("originationrefer-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKINGBEFOREREFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendRefer($callid, "contact");
			incrstats("originationrefer","sendRefer");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor202duration"},
				"WAIT FOR 202 REFER");
			changeDnState($dn, "originationstate", "WAITFOR202REFER");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("originationrefer","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "originationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("originationrefer","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} else {
			incrstats("originationrefer","unexpectedmsgs-3");
			incrstats("originationrefer-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer TALKINGBEFOREREFER warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR202REFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-5");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("originationrefer","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
			incrstats("originationrefer","timedout-2");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			if ($phdrs->{returncode} == 202) {
				# got OK response
				removetimer($callid);
				starttimer($callid, $dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKING");
				incrstats("originationrefer","received202refer");
			} else {
				# not what we wanted, end call.
				$dialogs{$callid}{cseq} += 1;
				sendBye($callid, "mycontact");
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 BYE");
				changeDnState($dn, "originationstate", "WAITFOR200BYE");
				incrstats("originationrefer","notreceived202refer");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("origination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} else {
			incrstats("originationrefer","unexpectedmsgs-4");
			incrstats("originationrefer-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer WAITFOR202REFER warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-6");
				if ($timerflag == 2) {
					incrstats("originationrefer","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("originationrefer","sendBye");
			incrstats("originationrefer","timedout-3");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			# respond and stay in same state. timer already is active.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","notifyreceived");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} else {
			incrstats("originationrefer","unexpectedmsgs-5");
			incrstats("originationrefer-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-7");
				if ($timerflag == 2) {
					incrstats("originationrefer","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","timedout-4");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("originationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} else {
			incrstats("originationrefer","unexpectedmsgs-6");
			incrstats("originationrefer-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationrefer","invalidtimers-8");
				if ($timerflag == 2) {
					incrstats("originationrefer","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","waitfor200cancel-received200OK");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationrefer","inactive");
			incrstats("originationrefer","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} else {
			incrstats("originationrefer","unexpectedmsgs-7");
			incrstats("originationrefer-unexpectedmsgs-7", 
					$phdrs->{msgtype});
			vprint "\noriginationrefer WAITFOR200CANCEL warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\noriginationrefer - unknown state $origstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("originationrefer","invalidtimers-9");
			if ($timerflag == 2) {
				incrstats("originationrefer","notreadyfororig");
			}
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("originationrefer","unexpectedmsgs-8");
			incrstats("originationrefer-unexpectedmsgs-8", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub termination {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry termination: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	# if (!$timerflag && $phdrs->{msgtype} =~ /CANCEL/) {
		# printit "MSG CANCEL, STATE ${termstate}\n";
	# }
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination","invalidtimers-1");
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("termination","started");
			sendInvite180Ringing($callid, $phdrs, "contact");
			incrstats("termination","sendInvite180Ringing");
			sendInvite183Ringing($callid, $phdrs, "contact");
			incrstats("termination","sendInvite183Ringing");
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OK($callid, $phdrs, "contact");
				incrstats("termination","sendInvite200OK");
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("termination","unexpectedmsgs-1");
			incrstats("termination-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
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
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("termination","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} else {
			incrstats("termination","unexpectedmsgs-2");
			incrstats("termination-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
			incrstats("termination","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("termination","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			incrstats("termination","stable");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} else {
			incrstats("termination","unexpectedmsgs-3");
			incrstats("termination-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
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
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("termination","timedout-2");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("termination","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("termination","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("termination","inactive");
				incrstats("termination","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} else {
			incrstats("termination","unexpectedmsgs-4");
			incrstats("termination-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\ntermination TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
			incrstats("termination","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("termination","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} else {
			incrstats("termination","unexpectedmsgs-5");
			incrstats("termination-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR486ACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
			incrstats("termination","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
		} else {
			incrstats("termination","unexpectedmsgs-6");
			incrstats("termination-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFOR486BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("termination","unexpectedmsgs-7");
			incrstats("termination-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub terminationrefer {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry termination: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationrefer","invalidtimers-1");
			vprint "\nterminationrefer INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("terminationrefer","started");
			sendInvite180Ringing($callid, $phdrs, "contact");
			incrstats("terminationrefer","sendInvite180Ringing");
			sendInvite183Ringing($callid, $phdrs, "contact");
			incrstats("terminationrefer","sendInvite183Ringing");
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OK($callid, $phdrs, "contact");
				incrstats("terminationrefer","sendInvite200OK");
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("terminationrefer","unexpectedmsgs-1");
			incrstats("terminationrefer-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			$phdrs = $dialogs{$callid}{lastphdrs};
			sendInvite200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","sendInvite200OK");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} else {
			incrstats("terminationrefer","unexpectedmsgs-2");
			incrstats("terminationrefer-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-3");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
			incrstats("terminationrefer","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"prerefercallduration"},
				"TALKING PRE-REFER CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKINGBEFOREREFER");
			incrstats("terminationrefer","stable");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} else {
			incrstats("terminationrefer","unexpectedmsgs-3");
			incrstats("terminationrefer-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\nterminationrefer WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKINGBEFOREREFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendRefer($callid, "contact");
			incrstats("terminationrefer","sendRefer");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor202duration"},
				"WAIT FOR 202 REFER");
			changeDnState($dn, "terminationstate", "WAITFOR202REFER");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} else {
			incrstats("terminationrefer","unexpectedmsgs-4");
			incrstats("terminationrefer-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationrefer TALKINGBEFOREREFER warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFOR202REFER$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-5");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("terminationrefer","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationrefer","timedout-2");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			if ($phdrs->{returncode} == 202) {
				incrstats("terminationrefer","received202refer");
			} else {
				incrstats("terminationrefer","receivedNOT202refer");
			}
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"maxcallduration"}, "TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			# if ($phdrs->{returncode} == 202) {
				# got OK response
				# removetimer($callid);
				# starttimer($callid, $dndata{$dn}{"maxcallduration"}, "TALKING MAX CALL LENGTH");
				# changeDnState($dn, "terminationstate", "TALKING");
				# incrstats("terminationrefer","received202refer");
			# } else {
				# not what we wanted, end call.
				# $dialogs{$callid}{cseq} += 1;
				# sendBye($callid, "mycontact");
				# removetimer($callid);
				# starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, "WAIT FOR 200 BYE");
				# changeDnState($dn, "terminationstate", "WAITFOR200BYE");
				# incrstats("terminationrefer","notreceived202refer");
			# }
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("termination","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} else {
			incrstats("terminationrefer","unexpectedmsgs-5");
			incrstats("terminationrefer-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\nterminationrefer WAITFOR202REFER warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-6");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("terminationrefer","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationrefer","timedout-7");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			# respond and stay in same state. timer already is active.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","notifyreceived");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} else {
			incrstats("terminationrefer","unexpectedmsgs-6");
			incrstats("terminationrefer-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\nterminationrefer TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationrefer","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
			incrstats("terminationrefer","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486Busy($callid, $phdrs, "contact");
				incrstats("terminationrefer","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationrefer","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationrefer","inactive");
				incrstats("terminationrefer","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationrefer","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationrefer","inactive");
		} else {
			incrstats("terminationrefer","unexpectedmsgs-7");
			incrstats("terminationrefer-unexpectedmsgs-7", 
					$phdrs->{msgtype});
			vprint "\nterminationrefer WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\nterminationrefer - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationrefer","invalidtimers-8");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("terminationrefer","unexpectedmsgs-8");
			incrstats("terminationrefer-unexpectedmsgs-8", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub terminationreqannc {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry terminationreqannc: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	my $reqanncdn = $dndata{$dn}{reqanncdn};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationreqannc","invalidtimers-1");
			vprint "\nterminationreqannc INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("terminationreqannc","started");
			sendInvite180Ringing($callid, $phdrs, "contact");
			incrstats("terminationreqannc","sendInvite180Ringing");
			sendInvite183Ringing($callid, $phdrs, "contact");
			incrstats("terminationreqannc","sendInvite183Ringing");
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OK($callid, $phdrs, "contact");
				incrstats("terminationreqannc","sendInvite200OK");
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-1");
			incrstats("terminationreqannc-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationreqannc","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			$phdrs = $dialogs{$callid}{lastphdrs};
			sendInvite200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","sendInvite200OK");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationreqannc","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-2");
			incrstats("terminationreqannc-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationreqannc","invalidtimers-3");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
			incrstats("terminationreqannc","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationreqannc","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"prereqannccallduration"},
				"TALKING PRE-REQUEST-ANNC CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKINGBEFOREREQANNC");
			incrstats("terminationreqannc","stable");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-3");
			incrstats("terminationreqannc-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\nterminationreqannc WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKINGBEFOREREQANNC$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationreqannc","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# send request for an announcement.
			# $dialogs{$callid}{cseq} += 1;
			vprint "\nterminatioreqannc: peersdp = ".$dialogs{$callid}{peersdp}."\n";
			queueOrig($dn, $reqanncdn, 1, 1, $dialogs{$callid}{peersdp}, undef, undef);
			# at this point, just continue to TALKING state.
			# there are no msgs sent to this process for
			# acknowledging the request-annc INVITE.
			incrstats("terminationreqannc","sendRequestAnncInvite");
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			incrstats("terminationreqannc","talking");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationreqannc","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-4");
			incrstats("terminationreqannc-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationreqannc TALKINGBEFOREREQANNC warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationreqannc","invalidtimers-6");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("terminationreqannc","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationreqannc","timedout-7");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			# respond and stay in same state. timer already is active.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","notifyreceived");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-6");
			incrstats("terminationreqannc-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\nterminationreqannc TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationreqannc","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
			incrstats("terminationreqannc","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationreqannc","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationreqannc","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationreqannc","inactive");
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-7");
			incrstats("terminationreqannc-unexpectedmsgs-7", 
					$phdrs->{msgtype});
			vprint "\nterminationreqannc WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\nterminationreqannc - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationreqannc","invalidtimers-8");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("terminationreqannc","unexpectedmsgs-8");
			incrstats("terminationreqannc-unexpectedmsgs-8", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub terminationsubscribe {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry terminationsubscribe: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	my $reqanncdn = $dndata{$dn}{reqanncdn};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationsubscribe","invalidtimers-1");
			vprint "\nterminationsubscribe INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("terminationsubscribe","started");
			sendInvite180Ringing($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","sendInvite180Ringing");
			sendInvite183Ringing($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","sendInvite183Ringing");
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OK($callid, $phdrs, "contact");
				incrstats("terminationsubscribe","sendInvite200OK");
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-1");
			incrstats("terminationsubscribe-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			$phdrs = $dialogs{$callid}{lastphdrs};
			sendInvite200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","sendInvite200OK");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationsubscribe","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-2");
			incrstats("terminationsubscribe-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-3");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
			incrstats("terminationsubscribe","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationsubscribe","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"prereqannccallduration"},
				"TALKING PRE-REQUEST-ANNC CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKINGBEFOREREQANNC");
			incrstats("terminationsubscribe","stable");
			incrstats("terminationsubscribe","prereqannc");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-3");
			incrstats("terminationsubscribe-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\nterminationsubscribe WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKINGBEFOREREQANNC$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# send request for an announcement.
			vprint "\nterminatiosubscribe: peersdp = ".$dialogs{$callid}{peersdp}."\n";
			queueOrig($dn, $reqanncdn, 1, 1, $dialogs{$callid}{peersdp}, undef, undef);
			# now wait before sending SUBSCRIBE msg.
			incrstats("terminationsubscribe","sendRequestAnncInvite");
			starttimer($callid, $dndata{$dn}{"presubscribecallduration"},
				"TALKING PRE-SUBSCRIBE CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKINGBEFORESUBSCRIBE");
			incrstats("terminationsubscribe","presubscribe");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationsubscribe","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-4");
			incrstats("terminationsubscribe-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationsubscribe TALKINGBEFOREREQANNC warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKINGBEFORESUBSCRIBE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# send SUBSCRIBE msg
			$dialogs{$callid}{cseq} += 1;
			sendSubscribe($callid, "contact");
			incrstats("terminationsubscribe","sendBye");
			# at this point, just continue to TALKING state
			# and deal with NOTIFIES there.
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			incrstats("terminationsubscribe","talking");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationsubscribe","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-4");
			incrstats("terminationsubscribe-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationsubscribe TALKINGBEFOREREQANNC warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-6");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendBye($callid, "contact");
			incrstats("terminationsubscribe","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationsubscribe","timedout-7");
		} elsif ($phdrs->{msgtype} =~ /NOTIFY/) {
			# respond and stay in same state. timer already is active.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","notifyreceived");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-6");
			incrstats("terminationsubscribe-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\nterminationsubscribe TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationsubscribe","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
			incrstats("terminationsubscribe","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationsubscribe","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationsubscribe","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OK($callid, $phdrs, "contact");
			incrstats("terminationsubscribe","send200OK");
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-7");
			incrstats("terminationsubscribe-unexpectedmsgs-7", 
					$phdrs->{msgtype});
			vprint "\nterminationsubscribe WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\nterminationsubscribe - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationsubscribe","invalidtimers-8");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("terminationsubscribe","unexpectedmsgs-8");
			incrstats("terminationsubscribe-unexpectedmsgs-8", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub termination_redirect {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry termination_redirect: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_redirect","invalidtimers-1");
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("termination_redirect","started");
			# redirect, send 302
			send302redirect($callid, $phdrs, "contact");
			incrstats("termination_redirect","send302redirect");
			# wait for 302 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR302ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 302 ACK");
			incrstats("termination_redirect","waitfor302ack");
		} else {
			incrstats("termination_redirect","unexpectedmsgs-1");
			incrstats("termination_redirect-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination_redirect INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR302ACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination_redirect","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_redirect","inactive");
			incrstats("termination_redirect","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_redirect","inactive");
		} else {
			incrstats("termination_redirect","unexpectedmsgs-6");
			incrstats("termination_redirect-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_redirect","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("termination_redirect","unexpectedmsgs-7");
			incrstats("termination_redirect-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub termination_busy {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry termination_busy: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_busy","invalidtimers-1");
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("termination_busy","started");
			# busy, send 486
			send486Busy($callid, $phdrs, "contact");
			incrstats("termination_busy","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("termination_busy","waitfor486ack");
		} else {
			incrstats("termination_busy","unexpectedmsgs-1");
			incrstats("termination_busy-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination_busy INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR486ACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination_busy","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_busy","inactive");
			incrstats("termination_busy","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_busy","inactive");
		} else {
			incrstats("termination_busy","unexpectedmsgs-6");
			incrstats("termination_busy-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_busy","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("termination_busy","unexpectedmsgs-7");
			incrstats("termination_busy-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub termination_return {
	# control of what return code to return.
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry termination_return: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_return","invalidtimers-1");
			vprint "\ntermination INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("termination_return","started");
			# send 4XX,5XX, or 6XX return code
			sendXXXreturn($callid, $phdrs, "contact");
			incrstats("termination_return","sendXXXReturn");
			# wait for XXX ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFORXXXACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR XXX ACK");
			incrstats("termination_return","waitforXXXack");
		} else {
			incrstats("termination_return","unexpectedmsgs-1");
			incrstats("termination_return-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\ntermination_return INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFORXXXACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("termination_return","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_return","inactive");
			incrstats("termination_return","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("termination_return","inactive");
		} else {
			incrstats("termination_return","unexpectedmsgs-6");
			incrstats("termination_return-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\ntermination WAITFORXXXACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\ntermination - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("termination_return","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("termination_return","unexpectedmsgs-7");
			incrstats("termination_return-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
##########################################################################3
#
# SIP and UPDATE functions
#
sub sendInvite180RingingUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 180 Ringing\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE180".$cseqmeth);
	return;
}
#
sub sendInvite183RingingUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 183 Session Progress\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE183".$cseqmeth);
	return;
}
#
sub sendInvite200OKUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub sendByeUPDATE {
	my ($callid, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "BYE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." BYE\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "BYE");
	return;
}
#
sub sendInviteAckUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{invitecseq}." ACK\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
#
sub sendAckUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
#
sub sendCancelUPDATE {
	my ($callid, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	# flip To and From fields in the BYE msg.
	#
	my $ackmsg = "CANCEL ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." CANCEL\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "CANCEL");
	return;
}
#
sub sendInviteUPDATE {
	my ($callid, $contact, $isannc, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "INVITE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." INVITE\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $sdp = "";
	if ($isannc) {
		$sdp = $dialogs{$callid}{peersdp};
		vprint "ANNC OFFER PEER SDP: $sdp\n";
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} elsif (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "INVITE");
	return;
}
#
sub send200OKUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub send486BusyUPDATE {
	my ($callid, $phdrs, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 486 Busy\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE486".$cseqmeth);
	return;
}
#
sub sendNotifyUPDATE {
	my ($callid, $contact, $offersdp) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "NOTIFY ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." NOTIFY\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	if (defined($offersdp) && length($offersdp) > 0) {
		my $sdp = "";
		makeOfferSDP($dn, $offersdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "NOTIFY");
	return;
}
#
sub sendUpdate {
	my ($callid, $contact, $updatesdp) = @_;
	#
	if (!defined($updatesdp) || length($updatesdp) <= 0) {
		# no UPDATE to send.
		vprint "\nsendUpdate: no UPDATE msg to send.\n";
		return;
	}
	if (!defined($dialogs{$callid}{$updatesdp}) ||
	     length($dialogs{$callid}{$updatesdp}) <= 0) {
		# no UPDATE to send.
		vprint "\nsendUpdate: no UPDATE msg in data to send.\n";
		return;
	}
	vprint "\nsendUPDATE: UPDATE $updatesdp msg ($dialogs{$callid}{$updatesdp}) to send.\n";
	#
	$dialogs{$callid}{cseq} += 1;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "UPDATE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." UPDATE\r\n";
	$ackmsg .= "Allow: INVITE, ACK, CANCEL, BYE, INFO, OPTIONS, UPDATE\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	if (defined($updatesdp)) {
		my $sdp = "";
		makeOfferSDP($dn, $updatesdp, \$sdp);
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	} else {
		# no SDP to send.
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "INFO");
	return;
}
#
sub originationupdate {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry originationupdate: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-1");
				$ptimer = undef;
				return;
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			$dialogs{$callid}{invitecseq} = $dialogs{$callid}{cseq};
			sendInviteUPDATE($callid, "contact", 0, 
				$dialogs{$callid}{originvitesdp});
			incrstats("originationupdate","started");
			incrstats("originationupdate","sendInvite");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "originationstate", "WAITFOR200");
		} else {
			if ($timerflag) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-2");
			} else {
				incrstats("originationupdate","unexpectedmsgs-1");
				incrstats("originationupdate-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			}
			vprint "\noriginationupdate INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-3");
				if ($timerflag == 2) {
					incrstats("originationupdate","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendCancelUPDATE($callid, "contact",
				$dialogs{$callid}{origcancelsdp});
			incrstats("originationupdate","sendCancel");
			incrstats("originationupdate","timedout-1");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 CANCEL");
			changeDnState($dn, "originationstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			# now we have the far-end URI.
			$dialogs{$callid}{peer} = $phdrs->{"To"};
			#
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("originationupdate", "received200update");
				} else {
					incrstats("originationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			#
			if ($phdrs->{returncode} == 100) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationupdate","received100Invite");
				sendUpdate($callid, "contact", "origpost100updatesdp");
			} elsif ($phdrs->{returncode} == 180) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationupdate","received180Invite");
				sendUpdate($callid, "contact", "origpost180updatesdp");
			} elsif ($phdrs->{returncode} == 183) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationupdate","received183Invite");
				sendUpdate($callid, "contact", "origpost183updatesdp");
			} elsif ($phdrs->{returncode} == 200) {
				# check if the 200 is from an INFO msg and not
				# from the INVITE.
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
				if ($cseqmeth =~ /INFO/) {
					# INFO 200 return.
					incrstats("originationupdate", "received200info");
					return;
				} elsif ($cseqmeth =~ /UPDATE/) {
					# INFO 200 return.
					incrstats("originationupdate", "received200update");
					return;
				}
				# 
				sendUpdate($callid, "contact", "origpost200updatesdp");
				sendInviteAckUPDATE($callid, $phdrs, "contact", 
					$dialogs{$callid}{originviteacksdp});
				incrstats("originationupdate","sendAck");
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKING");
				incrstats("originationupdate","stable");
				incrstats("originationupdate","received200Invite");
				sendUpdate($callid, "contact", "origpostackupdatesdp");
			} elsif ($phdrs->{returncode} == 486) {
				incrstats("originationupdate", "received486busy");
				sendAckUPDATE($callid, $phdrs, "contact", 
					$dialogs{$callid}{origacksdp});
				incrstats("originationupdate","send486Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationupdate", "inactive");
			} elsif ($phdrs->{returncode} == 503) {
				incrstats("originationupdate", "received503serviceunavailable");
				sendAckUPDATE($callid, $phdrs, "contact",
					$dialogs{$callid}{origacksdp});
				incrstats("originationupdate","send503Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationupdate", "inactive");
			} else {
				incrstats("originationupdate", "receivedXXXstatus");
				sendAckUPDATE($callid, $phdrs, "contact",
					$dialogs{$callid}{origacksdp});
				incrstats("originationupdate","sendXXXAck");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationupdate", "inactive");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelsdp});
			incrstats("originationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyesdp});
			incrstats("originationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200oksdp});
			incrstats("originationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okupdatesdp});
			incrstats("originationupdate","send200OK");
		} else {
			incrstats("originationupdate","unexpectedmsgs-2");
			incrstats("originationupdate-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\noriginationupdate WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-4");
				if ($timerflag == 2) {
					incrstats("originationupdate","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendByeUPDATE($callid, "contact",
				$dialogs{$callid}{origbyesdp});
			incrstats("originationupdate","sendBye");
			incrstats("originationupdate","timedout-2");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyesdp});
			incrstats("originationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
			incrstats("originationupdate","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelsdp});
			incrstats("originationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /SUBSCRIBE/) {
			$dialogs{$callid}{cseq} += 1;
			sendNotifyUPDATE($callid, "contact",
				$dialogs{$callid}{orignotifysdp});
			incrstats("originationupdate","sendNotify");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			my $cseqmeth = undef;
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("originationupdate", "received200update");
				} else {
					incrstats("originationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200oksdp});
			incrstats("originationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okupdatesdp});
			incrstats("originationupdate","send200OK");
		} else {
			incrstats("originationupdate","unexpectedmsgs-3");
			incrstats("originationupdate-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\noriginationupdate TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-5");
				if ($timerflag == 2) {
					incrstats("originationupdate","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
			incrstats("originationupdate","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("originationupdate", "received200update");
				} else {
					incrstats("originationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelsdp});
			incrstats("originationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyesdp});
			incrstats("originationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200oksdp});
			incrstats("originationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okupdatesdp});
			incrstats("originationupdate","send200OK");
		} else {
			incrstats("originationupdate","unexpectedmsgs-4");
			incrstats("originationupdate-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\noriginationupdate WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationupdate","invalidtimers-6");
				if ($timerflag == 2) {
					incrstats("originationupdate","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
			incrstats("originationupdate","timedout-4");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("originationupdate", "received200update");
				} else {
					incrstats("originationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationupdate","inactive");
			incrstats("originationupdate","waitfor200cancel-received200OK");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200oksdp});
			incrstats("originationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okupdatesdp});
			incrstats("originationupdate","send200OK");
		} else {
			incrstats("originationupdate","unexpectedmsgs-5");
			incrstats("originationupdate-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\noriginationupdate WAITFOR200CANCEL warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\noriginationupdate - unknown state $origstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("originationupdate","invalidtimers-7");
			if ($timerflag == 2) {
				incrstats("originationupdate","notreadyfororig");
			}
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("originationupdate","unexpectedmsgs-6");
			incrstats("originationupdate-unexpectedmsgs-6", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub terminationupdate {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry terminationupdate: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	# if (!$timerflag && $phdrs->{msgtype} =~ /CANCEL/) {
		# printit "MSG CANCEL, STATE ${termstate}\n";
	# }
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationupdate","invalidtimers-1");
			vprint "\nterminationupdate INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			incrstats("terminationupdate","started");
			sendInvite180RingingUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term180ringingsdp});
			incrstats("terminationupdate","sendInvite180Ringing");
			#
			sendUpdate($callid, "contact", "termpost180updatesdp");
			#	
			sendInvite183RingingUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term183ringingsdp});
			incrstats("terminationupdate","sendInvite183Ringing");
			#
			sendUpdate($callid, "contact", "termpost183updatesdp");
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OKUPDATE($callid, $phdrs, "contact",
					$dialogs{$callid}{terminvite200oksdp});
				incrstats("terminationupdate","sendInvite200OK");
				#
				sendUpdate($callid, "contact", "termpost200updatesdp");
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("terminationupdate","unexpectedmsgs-1");
			incrstats("terminationupdate-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationupdate","invalidtimers-6");
				$ptimer = undef;
				return;
			}
			$phdrs = $dialogs{$callid}{lastphdrs};
			sendInvite200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{terminvite200oksdp});
			incrstats("terminationupdate","sendInvite200OK");
			#
			sendUpdate($callid, "contact", "termpost200updatesdp");
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busysdp});
			incrstats("terminationupdate","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationupdate","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelsdp});
			incrstats("terminationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyesdp});
			incrstats("terminationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200oksdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okupdatesdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("terminationupdate", "received200update");
				} else {
					incrstats("terminationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} else {
			incrstats("terminationupdate","unexpectedmsgs-2");
			incrstats("terminationupdate-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationupdate","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
			incrstats("terminationupdate","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busysdp});
			incrstats("terminationupdate","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationupdate","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			incrstats("terminationupdate","stable");
			#
			sendUpdate($callid, "contact", "termpostackupdatesdp");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelsdp});
			incrstats("terminationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyesdp});
			incrstats("terminationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200oksdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okupdatesdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("terminationupdate", "received200update");
				} else {
					incrstats("terminationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} else {
			incrstats("terminationupdate","unexpectedmsgs-3");
			incrstats("terminationupdate-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationupdate","invalidtimers-3");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendByeUPDATE($callid, "contact",
				$dialogs{$callid}{termbyesdp});
			incrstats("terminationupdate","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationupdate","timedout-2");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486BusyUPDATE($callid, $phdrs, "contact",
					$dialogs{$callid}{term486busysdp});
				incrstats("terminationupdate","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationupdate","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationupdate","inactive");
				incrstats("terminationupdate","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyesdp});
			incrstats("terminationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200oksdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okupdatesdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelsdp});
			incrstats("terminationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("terminationupdate", "received200update");
				} else {
					incrstats("terminationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-received", $retcode);
		} else {
			incrstats("terminationupdate","unexpectedmsgs-4");
			incrstats("terminationupdate-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationupdate","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
			incrstats("terminationupdate","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busysdp});
			incrstats("terminationupdate","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationupdate","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			#
			# check if the 200 is from an INFO msg and not
			# from the BYE or CANCEL.
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("terminationupdate", "received200update");
				} else {
					incrstats("terminationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			}
			if ($phdrs->{returncode} == 200 && 
			    $cseqmeth =~ /INFO/) {
				# INFO 200 return.
				incrstats("terminationupdate", "received200info");
				return;
			}
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelsdp});
			incrstats("terminationupdate","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyesdp});
			incrstats("terminationupdate","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200oksdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okupdatesdp});
			incrstats("terminationupdate","send200OK");
		} else {
			incrstats("terminationupdate","unexpectedmsgs-5");
			incrstats("terminationupdate-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR486ACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationupdate","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
			incrstats("terminationupdate","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationupdate","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200oksdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /UPDATE/) {
			# send back 200 and stay in state
			send200OKUPDATE($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okupdatesdp});
			incrstats("terminationupdate","send200OK");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($cseqmeth =~ /UPDATE/) {
				# UPDATE return code. just stay in the
				# same state.
				my $retcode = $phdrs->{returncode};

				if ($retcode == 200) {
					incrstats("terminationupdate", "received200update");
				} else {
					incrstats("terminationupdate", "RETURNCODE".$retcode.$cseqmeth);
				}
				return;
			} else {
				incrstats("terminationupdate","unexpectedmsgs-6");
				incrstats("terminationupdate-unexpectedmsgs-6", 
						$phdrs->{msgtype});
				vprint "\nterminationupdate RETURNCODE warning:\n";
				vprint "Unexpected msg $phdrs->{msgtype}.\n";
				vprint "(callid=$callid).\n";
			}
		} else {
			incrstats("terminationupdate","unexpectedmsgs-6");
			incrstats("terminationupdate-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\nterminationupdate WAITFOR486BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\nterminationupdate - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationupdate","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("terminationupdate","unexpectedmsgs-7");
			incrstats("terminationupdate-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
##########################################################################
#
# SIP and ISUP functions
#
sub sendInvite180RingingISUP {
	my ($callid, $phdrs, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 180 Ringing\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE180".$cseqmeth);
	return;
}
#
sub sendInvite183RingingISUP {
	my ($callid, $phdrs, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 183 Session Progress\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE183".$cseqmeth);
	return;
}
#
sub sendInvite200OKISUP {
	my ($callid, $phdrs, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{local}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/sdp\r\n";
		$payload .= "Content-Disposition: session; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= "$sdp";
		$payload .= "\r\n--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub sendByeISUP {
	my ($callid, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "BYE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." BYE\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "BYE");
	return;
}
#
sub sendInviteAckISUP {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $sdp = "";
	makeSDP($dn, \$sdp);
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/sdp\r\n";
		$payload .= "Content-Disposition: session; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= "$sdp";
		$payload .= "\r\n--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
#
sub sendAckISUP {
	my ($callid, $phdrs, $contact) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	#
	my $ackmsg = "ACK ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Cseq: ".$dialogs{$callid}{cseq}." ACK\r\n";
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "ACK");
	return;
}
#
sub sendCancelISUP {
	my ($callid, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	# flip To and From fields in the BYE msg.
	#
	my $ackmsg = "CANCEL ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." CANCEL\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "CANCEL");
	return;
}
#
sub sendInviteISUP {
	my ($callid, $contact, $isannc, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "INVITE ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
	if (defined($dialogs{$callid}{invitesubject}) &&
	   (length($dialogs{$callid}{invitesubject}) > 0)) {
		$ackmsg .= "Subject: ".$dialogs{$callid}{invitesubject}."\r\n";
	}
	if ($dialogs{$callid}{usepassertedidentity}) {
		if (length($dialogs{$callid}{passertedidentity}) > 0) {
			$ackmsg .= "P-Asserted-Identity: ".$dialogs{$callid}{passertedidentity}."\r\n";
		}
	}
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." INVITE\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $sdp = "";
	if ($isannc) {
		$sdp = $dialogs{$callid}{peersdp};
		vprint "ANNC OFFER PEER SDP: $sdp\n";
	} else {
		makeSDP($dn, \$sdp);
		vprint "OFFER SDP: $sdp\n";
	}
	#
	vprint "isupmsgid = $isupmsgid \n";
	vprint "useisup =  $dialogs{$callid}{useisup} \n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/sdp\r\n";
		$payload .= "Content-Disposition: session; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= "$sdp";
		$payload .= "\r\n--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		my $sdplen = length($sdp);
		$ackmsg .= "Content-Type: application/sdp\r\n";
		$ackmsg .= "Content-Length: $sdplen\r\n";
		$ackmsg .= "\r\n";
		$ackmsg .= "$sdp";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "INVITE");
	return;
}
#
sub send200OKISUP {
	my ($callid, $phdrs, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 200 OK\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE200".$cseqmeth);
	return;
}
#
sub send486BusyISUP {
	my ($callid, $phdrs, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	#
	my $ackmsg = "SIP/2.0 486 Busy\r\n";
	$ackmsg .= "Via: ".$phdrs->{"Via"}.";received=";
	$ackmsg .= $parameters{mysipip}."\r\n";
	$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
	$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
#	$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "Call-Id: ".$phdrs->{"Call-ID"}."\r\n";
	$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Contact: ".$dialogs{$callid}{mycontact}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	#
	my $cseqmeth = undef;
	if (defined($phdrs->{"CSeq"})) {
		$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
	} 
	if (!defined($cseqmeth)) {
		$cseqmeth = "UNKNOWNMETHOD";
	}
	incrstats("msgs-sent", "RETURNCODE486".$cseqmeth);
	return;
}
#
sub sendNotifyISUP {
	my ($callid, $contact, $isupmsgid) = @_;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "NOTIFY ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." NOTIFY\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "NOTIFY");
	return;
}
#
sub sendInfoISUP {
	my ($callid, $contact, $isupmsgid) = @_;
	#
	if (!defined($isupmsgid) || $isupmsgid <= 0) {
		# no INFO to send.
		vprint "\nsendInfoISUP: no ISUP msg to send.\n";
		return;
	}
	vprint "\nsendInfoISUP: ISUP msg ($isupmsgid) to send.\n";
	#
	$dialogs{$callid}{cseq} += 1;
	#
	my $dn = $dialogs{$callid}{dn};
	my $requesturi = $dialogs{$callid}{requesturi};
	my $brand = int(rand(1000000000));
	my $newbranch = ${parameters}{branch}.$brand;
	#
	my $ackmsg = "INFO ${requesturi} SIP/2.0\r\n";
	$ackmsg .= "Via: SIP/2.0/UDP ".$parameters{mysipip}.":".$parameters{mysipport}.";branch=".${newbranch}."\r\n";
	$ackmsg .= "From: ".$dialogs{$callid}{local}."\r\n";
	$ackmsg .= "To: ".$dialogs{$callid}{peer}."\r\n";
#	$ackmsg .= "Call-ID: ".${callid}."\r\n";
	$ackmsg .= "Call-Id: ".${callid}."\r\n";
	$ackmsg .= "CSeq: ".$dialogs{$callid}{cseq}." INFO\r\n";
	if (defined($dialogs{$callid}{pchargingvector}) &&
	   (length($dialogs{$callid}{pchargingvector}) > 0)) {
		$ackmsg .= "P-Charging-Vector: ".$dialogs{$callid}{pchargingvector}."\r\n";
	}
	$ackmsg .= "Max-Forwards: ".$dialogs{$callid}{maxforwards}."\r\n";
	#
	my $isupmsg = undef;
	my $protocol = $parameters{isupversion};
	if ($dialogs{$callid}{useisup} && $isupmsgid > 0 &&
	    defined($dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid})) {
		# encapsulate ISUP
		$ackmsg .= "Content-Type: multipart/mixed; boundary=Telica-boundary\r\n";
		$ackmsg .= "Content-Length: CONTENTLENGTH\r\n";
		$ackmsg .= "\r\n";
		# create payload
		my $payload = "--Telica-boundary\r\n";
		$payload .= "Content-Type: application/ISUP; version=$protocol\n";
		$payload .= "Content-Disposition: signal; handling=required\r\n";
		$payload .= "\r\n";
		$payload .= $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
		$payload .= "\r\n--Telica-boundary--\r\n";
		my $payloadlen = length($payload);
		$ackmsg .= $payload;
		$ackmsg =~ s/CONTENTLENGTH/$payloadlen/;
		#
		$isupmsg = $dialogs{$callid}{rawmsgs}->{$protocol}{$isupmsgid};
	} else {
		# do not encapsulate ISUP
		$ackmsg .= "Content-Length: 0\r\n";
		$ackmsg .= "\r\n";
	}
	#
	my $paddr = makepaddr($contact, $callid);
	dumpOutgoingMsg($paddr, $ackmsg, $isupmsg);
	#
	defined(send(SIP, $ackmsg, 0, $paddr)) or die "send: $!";
	incrstats("msgs-sent", "INFO");
	return;
}
#
sub originationisup {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry originationisup: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
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
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-1");
				$ptimer = undef;
				return;
			}
			# we have a new call, parse the isup msg file
			# at this point.
			my $isupfile = $dialogs{$callid}{isupmsgfile};
			my $rawmsgs = { };
			my $status = $isup->parseinfile($isupfile, $rawmsgs);
			if (!$status) {
				incrstats("originationisup","isupmsgfileparsingfailed");
				vprint "\noriginationisup INACTIVE:\n";
				vprint "parsing ISUP msg file failed.\n";
				vprint "(callid=$callid).\n";
				vprint "\n\nCALL CONTINUES WITH NO ENCAPSULATED ISUP.\n\n";
				$dialogs{$callid}{rawmsgs} = undef;
				$dialogs{$callid}{useisup} = 0;
				incrstats("originationisup","noencapsulatedisup");
			} else {
				vprint "\n\nCALL CONTINUES WITH ENCAPSULATED ISUP.\n\n";
				incrstats("originationisup","isupmsgfileparsingpassed");
				$dialogs{$callid}{rawmsgs} = $rawmsgs;
				$dialogs{$callid}{useisup} = 1;
				incrstats("originationisup","encapsulatedisup");
			}
			# start up a call, send an INVITE
			$dialogs{$callid}{cseq} += 1;
			$dialogs{$callid}{invitecseq} = $dialogs{$callid}{cseq};
			sendInviteISUP($callid, "contact", 0, 
				$dialogs{$callid}{originvitemsgid});
			incrstats("originationisup","started");
			incrstats("originationisup","sendInvite");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 INVITE");
			changeDnState($dn, "originationstate", "WAITFOR200");
		} else {
			if ($timerflag) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-2");
			} else {
				incrstats("originationisup","unexpectedmsgs-1");
				incrstats("originationisup-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			}
			vprint "\noriginationisup INACTIVE:\n";
			vprint "Unexpected message or timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		}
	} elsif ($origstate =~ /^WAITFOR200$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-3");
				if ($timerflag == 2) {
					incrstats("originationisup","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# cancel call.
			$ptimer = undef;
			$dialogs{$callid}{cseq} += 1;
			sendCancelISUP($callid, "contact",
				$dialogs{$callid}{origcancelmsgid});
			incrstats("originationisup","sendCancel");
			incrstats("originationisup","timedout-1");
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"}, 
				"WAIT FOR 200 CANCEL");
			changeDnState($dn, "originationstate", 
					"WAITFOR200CANCEL");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			my $label = "";
			# now we have the far-end URI.
			$dialogs{$callid}{peer} = $phdrs->{"To"};
			#
			if ($phdrs->{returncode} == 100) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 100)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationisup","received100Invite");
				vprint "\norigpost100infomsgid = $dialogs{$callid}{origpost100infomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{origpost100infomsgid});
			} elsif ($phdrs->{returncode} == 180) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 180)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationisup","received180Invite");
				vprint "\norigpost180infomsgid = $dialogs{$callid}{origpost180infomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{origpost180infomsgid});
			} elsif ($phdrs->{returncode} == 183) {
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingfor200duration"}, 
					"WAIT FOR 200 INVITE (after 183)");
				changeDnState($dn, "originationstate", "WAITFOR200");
				incrstats("originationisup","received183Invite");
				vprint "\norigpost183infomsgid = $dialogs{$callid}{origpost183infomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{origpost183infomsgid});
			} elsif ($phdrs->{returncode} == 200) {
				# check if the 200 is from an INFO msg and not
				# from the INVITE.
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
				if ($cseqmeth =~ /INFO/) {
					# INFO 200 return.
					incrstats("originationisup", "received200info");
					return;
				}
				# 
				vprint "\norigpost200infomsgid = $dialogs{$callid}{origpost200infomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{origpost200infomsgid});
				sendInviteAckISUP($callid, $phdrs, "contact", 
					$dialogs{$callid}{originviteackmsgid});
				incrstats("originationisup","sendAck");
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"maxcallduration"},
					"TALKING MAX CALL LENGTH");
				changeDnState($dn, "originationstate", "TALKING");
				incrstats("originationisup","stable");
				incrstats("originationisup","received200Invite");
				vprint "\norigpostackinfomsgid = $dialogs{$callid}{origpostackinfomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{origpostackinfomsgid});
			} elsif ($phdrs->{returncode} == 486) {
				incrstats("originationisup", "received486busy");
				sendAckISUP($callid, $phdrs, "contact", 
					$dialogs{$callid}{origackmsgid});
				incrstats("originationisup","send486Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationisup", "inactive");
			} elsif ($phdrs->{returncode} == 503) {
				incrstats("originationisup", "received503serviceunavailable");
				sendAckISUP($callid, $phdrs, "contact",
					$dialogs{$callid}{origackmsgid});
				incrstats("originationisup","send503Ack");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationisup", "inactive");
			} else {
				incrstats("originationisup", "receivedXXXstatus");
				sendAckISUP($callid, $phdrs, "contact",
					$dialogs{$callid}{origackmsgid});
				incrstats("originationisup","sendXXXAck");
				# call ends.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "originationstate", "INACTIVE");
				incrstats("originationisup", "inactive");
			}
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelmsgid});
			incrstats("originationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# other side is gone.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyemsgid});
			incrstats("originationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okmsgid});
			incrstats("originationisup","send200OK");
		} else {
			incrstats("originationisup","unexpectedmsgs-2");
			incrstats("originationisup-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\noriginationisup WAITFOR200 warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-4");
				if ($timerflag == 2) {
					incrstats("originationisup","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendByeISUP($callid, "contact",
				$dialogs{$callid}{origbyemsgid});
			incrstats("originationisup","sendBye");
			incrstats("originationisup","timedout-2");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "originationstate", "WAITFOR200BYE");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyemsgid});
			incrstats("originationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif (($dndata{$dn}{"handleorigreinvite"} != 0) && 
			 ($phdrs->{msgtype} =~ /INVITE/)) {
			# we have a RE-INVITE. end this call and
			# treat as a new termination.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
			incrstats("originationisup","reinvitesstarted");
			# call parser for new termination
			handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelmsgid});
			incrstats("originationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /SUBSCRIBE/) {
			$dialogs{$callid}{cseq} += 1;
			sendNotifyISUP($callid, "contact",
				$dialogs{$callid}{orignotifymsgid});
			incrstats("originationisup","sendNotify");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okmsgid});
			incrstats("originationisup","send200OK");
		} else {
			incrstats("originationisup","unexpectedmsgs-3");
			incrstats("originationisup-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\noriginationisup TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200BYE$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-5");
				if ($timerflag == 2) {
					incrstats("originationisup","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
			incrstats("originationisup","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okcancelmsgid});
			incrstats("originationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okbyemsgid});
			incrstats("originationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okmsgid});
			incrstats("originationisup","send200OK");
		} else {
			incrstats("originationisup","unexpectedmsgs-4");
			incrstats("originationisup-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\noriginationisup WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($origstate =~ /^WAITFOR200CANCEL$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("originationisup","invalidtimers-6");
				if ($timerflag == 2) {
					incrstats("originationisup","notreadyfororig");
				}
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
			incrstats("originationisup","timedout-4");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "originationstate", "INACTIVE");
			incrstats("originationisup","inactive");
			incrstats("originationisup","waitfor200cancel-received200OK");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{orig200okmsgid});
			incrstats("originationisup","send200OK");
		} else {
			incrstats("originationisup","unexpectedmsgs-5");
			incrstats("originationisup-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\noriginationisup WAITFOR200CANCEL warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\noriginationisup - unknown state $origstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("originationisup","invalidtimers-7");
			if ($timerflag == 2) {
				incrstats("originationisup","notreadyfororig");
			}
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("originationisup","unexpectedmsgs-6");
			incrstats("originationisup-unexpectedmsgs-6", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
sub terminationisup {
	my ($rawmsg, $callid, $timerflag, $current_time, $recvpaddr, 
	    $pdatablocks, $phdrs, $ptimer, $reinvite) = @_;
	#
	if ($verboseflag) {
		vprint "\nEntry terminationisup: Call-ID = <$callid>\n";
		vprint "Timer Flag for $callid = <$timerflag>\n";
	}
	#
	my $dn = $dialogs{$callid}{dn};
	my $termstate = $dndata{$dn}{terminationstate};
	dumpDnState($dn, "terminationstate");
	#
	# if (!$timerflag && $phdrs->{msgtype} =~ /CANCEL/) {
		# printit "MSG CANCEL, STATE ${termstate}\n";
	# }
	if ($termstate =~ /^INACTIVE$/) {
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationisup","invalidtimers-1");
			vprint "\nterminationisup INACTIVE warning:\n";
			vprint "Unexpected timer.\n";
			vprint "(callid=$callid).\n";
			$ptimer = undef;
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# we have a new call, parse the isup msg file
			# at this point.
			my $isupfile = $dialogs{$callid}{isupmsgfile};
			my $rawmsgs = { };
			my $status = $isup->parseinfile($isupfile, $rawmsgs);
			if (!$status) {
				incrstats("terminationisup","isupmsgfileparsingfailed");
				vprint "\nterminationisup INACTIVE:\n";
				vprint "parsing ISUP msg file failed.\n";
				vprint "(callid=$callid).\n";
				vprint "\n\nCALL CONTINUES WITH NO ENCAPSULATED ISUP.\n\n";
				$dialogs{$callid}{rawmsgs} = undef;
				$dialogs{$callid}{useisup} = 0;
				incrstats("terminationisup","noencapsulatedisup");
			} else {
				vprint "\n\nCALL CONTINUES WITH ENCAPSULATED ISUP.\n\n";
				incrstats("terminationisup","isupmsgfileparsingpassed");
				$dialogs{$callid}{rawmsgs} = $rawmsgs;
				$dialogs{$callid}{useisup} = 1;
				incrstats("terminationisup","encapsulatedisup");
			}
			#
			incrstats("terminationisup","started");
			sendInvite180RingingISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term180ringingmsgid});
			incrstats("terminationisup","sendInvite180Ringing");
			#
			vprint "\ntermpost180infomsgid = $dialogs{$callid}{termpost180infomsgid} \n";
			sendInfoISUP($callid, "contact",
				$dialogs{$callid}{termpost180infomsgid});
			#	
			sendInvite183RingingISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term183ringingmsgid});
			incrstats("terminationisup","sendInvite183Ringing");
			#
			vprint "\ntermpost183infomsgid = $dialogs{$callid}{termpost183infomsgid} \n";
			sendInfoISUP($callid, "contact",
				$dialogs{$callid}{termpost183infomsgid});
			#
			if ($dndata{$dn}{"ringingduration"} > 0) {
				# wait for ringing
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"ringingduration"}, 
					"RINGING");
				changeDnState($dn, "terminationstate", 
					"RINGING");
				$dialogs{$callid}{lastphdrs} = $phdrs;
			} else {
				# just send 200OK immediately
				sendInvite200OKISUP($callid, $phdrs, "contact",
					$dialogs{$callid}{terminvite200okmsgid});
				incrstats("terminationisup","sendInvite200OK");
				#
				vprint "\ntermpost200infomsgid = $dialogs{$callid}{termpost200infomsgid} \n";
				sendInfoISUP($callid, "contact",
					$dialogs{$callid}{termpost200infomsgid});
				#
				removetimer($callid);
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR ACK");
				changeDnState($dn, "terminationstate", 
					"WAITFORACK");
			}
		} else {
			incrstats("terminationisup","unexpectedmsgs-1");
			incrstats("terminationisup-unexpectedmsgs-1", 
					$phdrs->{msgtype});
			vprint "\nterminationisup INACTIVE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^RINGING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationisup","invalidtimers-6");
				$ptimer = undef;
				return;
			}
			$phdrs = $dialogs{$callid}{lastphdrs};
			sendInvite200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{terminvite200okmsgid});
			incrstats("terminationisup","sendInvite200OK");
			#
			vprint "\ntermpost200infomsgid = $dialogs{$callid}{termpost200infomsgid} \n";
			sendInfoISUP($callid, "contact",
				$dialogs{$callid}{termpost200infomsgid});
			#
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR ACK");
			changeDnState($dn, "terminationstate", "WAITFORACK");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busymsgid});
			incrstats("terminationisup","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationisup","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelmsgid});
			incrstats("terminationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyemsgid});
			incrstats("terminationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okmsgid});
			incrstats("terminationisup","send200OK");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} else {
			incrstats("terminationisup","unexpectedmsgs-2");
			incrstats("terminationisup-unexpectedmsgs-2", 
					$phdrs->{msgtype});
			vprint "\nterminationisup WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^WAITFORACK$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationisup","invalidtimers-2");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
			incrstats("terminationisup","timedout-1");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busymsgid});
			incrstats("terminationisup","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationisup","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			removetimer($callid);
			starttimer($callid, $dndata{$dn}{"maxcallduration"},
				"TALKING MAX CALL LENGTH");
			changeDnState($dn, "terminationstate", "TALKING");
			incrstats("terminationisup","stable");
			#
			vprint "\ntermpostackinfomsgid = $dialogs{$callid}{termpostackinfomsgid} \n";
			sendInfoISUP($callid, "contact",
				$dialogs{$callid}{termpostackinfomsgid});
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelmsgid});
			incrstats("terminationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyemsgid});
			incrstats("terminationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okmsgid});
			incrstats("terminationisup","send200OK");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-sent", $retcode);
		} else {
			incrstats("terminationisup","unexpectedmsgs-3");
			incrstats("terminationisup-unexpectedmsgs-3", 
					$phdrs->{msgtype});
			vprint "\nterminationisup WAITFORACK warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /^TALKING$/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationisup","invalidtimers-3");
				$ptimer = undef;
				return;
			}
			# call ends, send bye.
			$dialogs{$callid}{cseq} += 1;
			sendByeISUP($callid, "contact",
				$dialogs{$callid}{termbyemsgid});
			incrstats("terminationisup","sendBye");
			#
			starttimer($callid, $dndata{$dn}{"waitingfor200duration"},
				"WAIT FOR 200 BYE");
			changeDnState($dn, "terminationstate", "WAITFOR200BYE");
			incrstats("terminationisup","timedout-2");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			if ($dndata{$dn}{"handletermreinvite"} == 0)
			{
				# busy, send 486
				send486BusyISUP($callid, $phdrs, "contact",
					$dialogs{$callid}{term486busymsgid});
				incrstats("terminationisup","send486Busy");
				# wait for 486 ACK.
				removetimer($callid);
				changeDnState($dn, "terminationstate", 
					"WAITFOR486ACK");
				starttimer($callid, 
					$dndata{$dn}{"waitingforackduration"}, 
					"WAIT FOR 486 ACK");
				incrstats("terminationisup","waitfor486ack");
			} else {
				# re-invite handling is enabled.
				# end current call and startup a new
				# termination.
				removetimer($callid);
				deleteCallId($callid);
				changeDnState($dn, "terminationstate", 
					"INACTIVE");
				incrstats("terminationisup","inactive");
				incrstats("terminationisup","reinvitesstarted");
				# call parser for new termination
				handleSIP($current_time, $recvpaddr, $rawmsg, 1, $dialogs{$callid}{fd});
			}
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# onhook received
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyemsgid});
			incrstats("terminationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okmsgid});
			incrstats("terminationisup","send200OK");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelmsgid});
			incrstats("terminationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# stay in current state
			my $cseqmeth = undef;
			if (defined($phdrs->{"CSeq"})) {
				$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			} 
			if (!defined($cseqmeth)) {
				$cseqmeth = "UNKNOWNMETHOD";
			}
			my $retcode = "RETURNCODE";
			$retcode .= $cseqmeth;
			$retcode .= $phdrs->{returncode};
			incrstats("msgs-received", $retcode);
		} else {
			incrstats("terminationisup","unexpectedmsgs-4");
			incrstats("terminationisup-unexpectedmsgs-4", 
					$phdrs->{msgtype});
			vprint "\nterminationisup TALKING warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR200BYE/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationisup","invalidtimers-4");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
			incrstats("terminationisup","timedout-3");
		} elsif ($phdrs->{msgtype} =~ /INVITE/) {
			# busy, send 486
			send486BusyISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term486busymsgid});
			incrstats("terminationisup","send486Busy");
			# wait for 486 ACK.
			removetimer($callid);
			changeDnState($dn, "terminationstate", "WAITFOR486ACK");
			starttimer($callid, $dndata{$dn}{"waitingforackduration"}, 
				"WAIT FOR 486 ACK");
			incrstats("terminationisup","waitfor486ack");
		} elsif ($phdrs->{msgtype} =~ /RETURNCODE/) {
			# call ends. the return code should be
			# 200OK, but really we are ending the call
			# whether it is or not.
			#
			# check if the 200 is from an INFO msg and not
			# from the BYE or CANCEL.
			$cseqmeth = getCseqMethod($phdrs->{"CSeq"});
			if ($phdrs->{returncode} == 200 && 
			    $cseqmeth =~ /INFO/) {
				# INFO 200 return.
				incrstats("terminationisup", "received200info");
				return;
			}
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /CANCEL/) {
			# last request was canceled. end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okcancelmsgid});
			incrstats("terminationisup","send200OKCancel");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /BYE/) {
			# end call.
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okbyemsgid});
			incrstats("terminationisup","send200OK");
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okmsgid});
			incrstats("terminationisup","send200OK");
		} else {
			incrstats("terminationisup","unexpectedmsgs-5");
			incrstats("terminationisup-unexpectedmsgs-5", 
					$phdrs->{msgtype});
			vprint "\nterminationisup WAITFOR200BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} elsif ($termstate =~ /WAITFOR486ACK/) {
		if ($timerflag) {
			# check if my timer
			if (!isMyTimer($callid, $ptimer)) {
				dumptimer($timerflag, $ptimer);
				incrstats("terminationisup","invalidtimers-7");
				$ptimer = undef;
				return;
			}
			# call ends.
			$ptimer = undef;
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
			incrstats("terminationisup","timedout-5");
		} elsif ($phdrs->{msgtype} =~ /ACK/) {
			# call ends.
			removetimer($callid);
			deleteCallId($callid);
			changeDnState($dn, "terminationstate", "INACTIVE");
			incrstats("terminationisup","inactive");
		} elsif ($phdrs->{msgtype} =~ /INFO/) {
			# send back 200 and stay in state
			send200OKISUP($callid, $phdrs, "contact",
				$dialogs{$callid}{term200okmsgid});
			incrstats("terminationisup","send200OK");
		} else {
			incrstats("terminationisup","unexpectedmsgs-6");
			incrstats("terminationisup-unexpectedmsgs-6", 
					$phdrs->{msgtype});
			vprint "\nterminationisup WAITFOR486BYE warning:\n";
			vprint "Unexpected msg $phdrs->{msgtype}.\n";
			vprint "(callid=$callid).\n";
		}
	} else {
		vprint "\nterminationisup - unknown state $termstate error:\n";
		if ($timerflag) {
			dumptimer($timerflag, $ptimer);
			incrstats("terminationisup","invalidtimers-5");
			vprint "Unexpected timer.\n";
			$ptimer = undef;
		} else {
			incrstats("terminationisup","unexpectedmsgs-7");
			incrstats("terminationisup-unexpectedmsgs-7", 
					$phdrs->{msgtype});
		}
		vprint "(callid=$callid).\n";
	}
	return;
}
#
##########################################################################
#
# general msg handlers
#
sub haltcallload {
	$loaddata{state} = "idle";
}
#
sub killcallload {
	$loaddata{state} = "idle";
	#
	my @types = ("origination", "termination");
	#
	foreach $type (@types) {
		foreach $callid (keys %dialogs) {
			my $dn = $dialogs{$callid}{dn};
			#
			if ((!defined($dn)) ||
			    (!defined($dndata{$dn})) ||
			    (!defined($dndata{$dn}{"${type}state"})) ||
			    ($dndata{$dn}{"${type}state"} ne "INACTIVE")) {
				next;
			}
			#
			removetimer($callid);
			starttimer($callid, 0, "TALKING MAX CALL LENGTH");
			changeDnState($dn, "${type}state", "TALKING");
			incrstats("${type}", "callkilled");
		}
	}
}
#
sub killcall {
	my ($dn) = @_;
	# does DN exist at this simulator?
	if (!exists($dndata{$dn})) {
		printit "killcall: no DN data found for ${dn}.\n";
		return;
	}
	if (($parameters{mysipip} ne $dndata{$dn}{sipip}) ||
	    ($parameters{mysipport} ne $dndata{$dn}{sipport})) {
		printit "killcall: DN ${dn} not owned by this simulator.\n";
		return;
	}
	# check origination and termination
	my @types = ("origination", "termination");
	foreach $type (@types) {
		if ($dndata{$dn}{"${type}state"} ne "INACTIVE") {
			if ($dndata{$dn}{"${type}callid"} ne "") {
				my $callid = $dndata{$dn}{"${type}callid"};
				if (exists($dialogs{$callid})) {
					removetimer($callid);
					starttimer($callid, 0, 
						"TALKING MAX CALL LENGTH");
					changeDnState($dn, "${type}state", "TALKING");
					incrstats("${type}", "callkilled");
				} else {
					printit "killcall: No dialogs for DN ${dn} ${type} call-id.\n";
				}
			} else {
				printit "killcall: No DN ${dn} ${type} call-id.\n";
			}
		} else {
			printit "killcall: No stable ${type}s found for DN ${dn}.\n";
		}
	}
	# all done
	return;
}
#
sub startcallload {
	my ($cps, $holdtime, $maxcalls) = @_;
	# calculate max simultaneous calls.
	my $ok = 1;
	if ($cps < 1) {
		printit "startcallload: CPS < 1.\n";
		$ok = 0;
	}
	if ($holdtime < 1) {
		printit "startcallload: HOLDTIME < 1.\n";
		$ok = 0;
	}
	if ($maxcalls < 1) {
		printit "startcallload: MAXCALLS < 1.\n";
		$ok = 0;
	}
	if (!$ok) {
		printit "startcallload: call load not started.\n";
		return;
	}
	my $simcalls = $cps*$holdtime;
	if ($simcalls > $loaddata{origdnsz}) {
		printit "Requested number of simultaneous calls > number of ORIG DNs.\n";
		return;
	}
	# check if a call load is already running
	if ($loaddata{state} ne "idle") {
		printit "startcallload: call load already running or halted.\n";
		return;
	}
	#
	$loaddata{callspersec} = $cps;
	$loaddata{holdtime} = $holdtime;
	$loaddata{maxcalls} = $maxcalls;
	$loaddata{callsstarted} = 0;
	# $loaddata{currentorigdn} = 0;
	$loaddata{state} = "running";
	$loaddata{tag} = int(rand(1000000000));
	#
	my $newcalls = 0;
	#
	my $o=0;
	for ( ; $o<$loaddata{origdnsz}; 
		$loaddata{currentorigdn}++, ${o}++) {
		my $i = $loaddata{currentorigdn}%$loaddata{origdnsz};
		my $odn = $loaddata{origdns}[$i];
		my $tdn = $loaddata{calls}{$odn};
		#
		if ($verboseflag) {
			vprint "startcallload: ORID DN - TRYING $odn.\n";
			vprint "startcallload: TERM DN - TRYING $tdn.\n";
		}
		#
		if ($dndata{$odn}{callloadstate} ne "INACTIVE") {
			# skip this orig dn.
			if ($verboseflag) {
				vprint "\tstartcallload: ORID DN - SKIPPING $odn (state=$dndata{$odn}{callloadstate}).\n";
				vprint "\tstartcallload: TERM DN - SKIPPING $tdn.\n";
			}
			next;
		}
		if ($verboseflag) {
			vprint "\tstartcallload: ORID DN - USING $odn (state=$dndata{$odn}{callloadstate}).\n";
			vprint "\tstartcallload: TERM DN - USING $tdn.\n";
		}
		# set max length of call.
		$dndata{$odn}{"maxcallduration"} = $holdtime;
		# we found a dn to start a call.
		queueOrig($odn, $tdn, 1, 0, undef, undef, undef);
		$dndata{$odn}{callloadstate} = "SCHEDULED";
		incrstats("origination","queueOrigCalled");
		# 
		${newcalls}++;
		$loaddata{callsstarted}++;
		#
		if ((${newcalls} >= $cps) || 
		    ($loaddata{callsstarted} >= $loaddata{maxcalls})) {
			# done with starting calls for now.
			last;
		}
	}
	#
	vprint "startcallload: $newcalls new calls queue ($newcalls out of $o).\n";
	# queue up next event to startup a call load.
	starttimer("CALL-LOAD", 2, "CALL-LOAD");
}
#
sub queueOrig {
	my ($odn, $tdn, $delta, $isannc, $peersdp, $newtdnip, $newtdnport) = @_;
	#
	# verify both DNs exist
	#
	my $ok = 1;
	if (!exists($dndata{$odn})) {
		vprint "Unknown Orig DN $odn.\n";
		incrstats("origination","queueOrigUnknownOrigDN");
		$ok = 0;
	}
	if (!exists($dndata{$tdn})) {
		vprint "Unknown Term DN $tdn.\n";
		incrstats("origination","queueOrigUnknownTermDN");
		$ok = 0;
	}
	if (!$ok) {
		vprint "returning without starting call.\n";
		incrstats("origination","queueOrigCallNotStarted");
		return;
	}
	#
	# create call id
	#
	my $callid = int(rand(1000000000));
	$callid = "${callid}${odn}\@$parameters{mysipip}";
	# save call-id for this DN.
	$dndata{$odn}{originationcallid} = $callid;
	#
	# create a dialog
	#
	$dialogs{$callid}{dn} = $odn;
	$dialogs{$callid}{isannc} = $isannc;
	$dialogs{$callid}{origdn} = $odn;
	$dialogs{$callid}{termdn} = $tdn;
	if ($isannc) {
		$dialogs{$callid}{handler} = $dndata{$odn}{announcement};
	} else {
		$dialogs{$callid}{handler} = $dndata{$odn}{origination};
	}
	if (defined($newtdnip) && defined($newtdnport)) {
		$dialogs{$callid}{contact} = 
			"sip:${tdn}\@${newtdnip}:${newtdnport}";
	} else {
		$dialogs{$callid}{contact} = dn2contact($tdn);
	}
	vprint "\nqueueOrig: 'contact' is ... ".$dialogs{$callid}{contact}."\n";
	$dialogs{$callid}{mycontact} = dn2contact($odn);
	$dialogs{$callid}{cseq} = int(rand(1000000000));
	$dialogs{$callid}{"timertag"} = 0;
	$dialogs{$callid}{"mytag"} = int(rand(1000000000));
	$dialogs{$callid}{"mytag"} .= "mytag";
	$dialogs{$callid}{maxforwards} = $parameters{"maxforwards"};
	$dialogs{$callid}{peersdp} = $peersdp;
	$dialogs{$callid}{invitesubject} = $dndata{$odn}{invitesubject};
	$dialogs{$callid}{usepassertedidentity} = 
		$dndata{$odn}{usepassertedidentity};
	$dialogs{$callid}{passertedidentity} = 
		$dndata{$odn}{passertedidentity};
	$dialogs{$callid}{subscribeevent} = $dndata{$odn}{subscribeevent};
	$dialogs{$callid}{isupoli} = $dndata{$odn}{isupoli};
	# ISUP encapsulated msgs
	$dialogs{$callid}{useisup} = 0;
	$dialogs{$callid}{isupmsgfile} = $dndata{$odn}{isupmsgfile};
	$dialogs{$callid}{orig200okbyemsgid} = 
			$dndata{$odn}{orig200okbyemsgid};
	$dialogs{$callid}{orig200okcancelmsgid} = 
			$dndata{$odn}{orig200okcancelmsgid};
	$dialogs{$callid}{orig200okmsgid} = 
			$dndata{$odn}{orig200okmsgid};
	$dialogs{$callid}{origackmsgid} = 
			$dndata{$odn}{origackmsgid};
	$dialogs{$callid}{origbyemsgid} = 
			$dndata{$odn}{origbyemsgid};
	$dialogs{$callid}{origcancelmsgid} = 
			$dndata{$odn}{origcancelmsgid};
	$dialogs{$callid}{originviteackmsgid} = 
			$dndata{$odn}{originviteackmsgid};
	$dialogs{$callid}{originvitemsgid} = 
			$dndata{$odn}{originvitemsgid};
	$dialogs{$callid}{orignotifymsgid} = 
			$dndata{$odn}{orignotifymsgid};
	$dialogs{$callid}{pchargingvector} = 
			$dndata{$odn}{pchargingvector};
	#
	$dialogs{$callid}{origpost100infomsgid} = 
			$dndata{$odn}{origpost100infomsgid};
	$dialogs{$callid}{origpost180infomsgid} = 
			$dndata{$odn}{origpost180infomsgid};
	$dialogs{$callid}{origpost183infomsgid} = 
			$dndata{$odn}{origpost183infomsgid};
	$dialogs{$callid}{origpost200infomsgid} = 
			$dndata{$odn}{origpost200infomsgid};
	$dialogs{$callid}{origpostackinfomsgid} = 
			$dndata{$odn}{origpostackinfomsgid};
	#
	$dialogs{$callid}{orig200okbyesdp} = 
			$dndata{$odn}{orig200okbyesdp};
	$dialogs{$callid}{orig200okcancelsdp} = 
			$dndata{$odn}{orig200okcancelsdp};
	$dialogs{$callid}{orig200oksdp} = 
			$dndata{$odn}{orig200oksdp};
	$dialogs{$callid}{orig200okupdatesdp} = 
			$dndata{$odn}{orig200okupdatesdp};
	$dialogs{$callid}{origacksdp} = 
			$dndata{$odn}{origacksdp};
	$dialogs{$callid}{origbyesdp} = 
			$dndata{$odn}{origbyesdp};
	$dialogs{$callid}{origcancelsdp} = 
			$dndata{$odn}{origcancelsdp};
	$dialogs{$callid}{originviteacksdp} = 
			$dndata{$odn}{originviteacksdp};
	$dialogs{$callid}{originvitesdp} = 
			$dndata{$odn}{originvitesdp};
	$dialogs{$callid}{orignotifysdp} = 
			$dndata{$odn}{orignotifysdp};
	#
	$dialogs{$callid}{origpost100updatesdp} = 
			$dndata{$odn}{origpost100updatesdp};
	$dialogs{$callid}{origpost180updatesdp} = 
			$dndata{$odn}{origpost180updatesdp};
	$dialogs{$callid}{origpost183updatesdp} = 
			$dndata{$odn}{origpost183updatesdp};
	$dialogs{$callid}{origpost200updatesdp} = 
			$dndata{$odn}{origpost200updatesdp};
	$dialogs{$callid}{origpostackupdatesdp} = 
			$dndata{$odn}{origpostackupdatesdp};
	#
	my $odnip = $dndata{$odn}{sipip};
	my $odnport = $dndata{$odn}{sipport};
	$dialogs{$callid}{"from"} = "sip:${odn}\@${odnip}:${odnport}";
	# assign tag here for local dn. 
	if (length($dialogs{$callid}{"isupoli"}) > 0) {
		$dialogs{$callid}{"localisupoli"} = $dialogs{$callid}{"from"};
		$dialogs{$callid}{"localisupoli"} .= 
			";isup-oli=".$dialogs{$callid}{"isupoli"};
		$dialogs{$callid}{"localisupoli"} .= ";tag=".$dialogs{$callid}{"mytag"};
	}
	$dialogs{$callid}{"local"} = $dialogs{$callid}{"from"};
	$dialogs{$callid}{"local"} .= ";tag=".$dialogs{$callid}{"mytag"};
	#
	my $tdnip = $dndata{$tdn}{sipip};
	if (defined($newtdnip)) {
		$tdnip = $newtdnip;
	}
	my $tdnport = $dndata{$tdn}{sipport};
	if (defined($newtdnport)) {
		$tdnport = $newtdnport;
	}
	$dialogs{$callid}{"requesturi"} = "sip:${tdn}\@${tdnip}:${tdnport}";
	if ($isannc) {
		vprint "queueOrig: ADDING ANNC REQUEST URI ...\n";
		vprint "\tBEFORE URI: ".$dialogs{$callid}{"requesturi"}."\n";
		$dialogs{$callid}{"requesturi"} .= $dndata{$odn}{reqanncuri};
		vprint "\tAFTER URI: ".$dialogs{$callid}{"requesturi"}."\n";
	}
	$dialogs{$callid}{"to"} = "sip:${tdn}\@${tdnip}:${tdnport}";
	# default peer has no tag. the response to the INVITE should
	# have the tag.
	$dialogs{$callid}{"peer"} = $dialogs{$callid}{"to"};
	#
	# queue up a timer request
	#
	incrstats("origination","queueOrigStartTimerCalled");
	if ($isannc) {
		incrstats("announcement","queueOrigStartTimerCalled");
	}
	starttimer($callid, $delta, "ORIGINATION");
	return;
}
#
sub queueReg {
	my ($rdn, $delta) = @_;
	#
}
#
sub killclient {
	my ($fd) = @_;
	#
	vec($rin,$fd,1) = 0;
	vec($win,$fd,1) = 0;
	vec($ein,$fd,1) = 0;
	#
	$input[$fd] = '';
	$output[$fd] = '';
	#
	close($fd2file[$fd]);
	#
	$fd2file[$fd] = undef;
	$fd2paddr[$fd] = undef;
	$fd2ip[$fd] = undef;
	$fd2port[$fd] = undef;
	#
	my @cl;
	#
	while ($i = shift @clients) {
		if ($i != $fd) {
			push((@cl), $i);
		}
	}
	@clients = @cl;
	return;
}
#
$previousorigdn = undef;
$previoustermdn = undef;
#
sub parseAndExecInput {
	my ($line, $pdone) = @_;
	#
	if ($line =~ /^q[\s]*$/i) {
		printit "\nQuitting ...\n";
		$$pdone = 1;
	} elsif ($line =~ /^w[\s]*$/) {
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "===============================================================\n";
	} elsif ($line =~ /^w/) {
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "\n$line\n";
	} elsif ($line =~ /^vi/) {
		if ($parameters{logfile} ne "") {
			if (defined($ENV{DISPLAY}) && length($ENV{DISPLAY}) > 0) {
				if ($pid = fork) {
					printit "editting log file $parameters{logfile}.\n";
				} else {
					die "cannot fork: $!" unless defined $pid;
					$SIG{INT} = "IGNORE";
					exec("xterm -e $parameters{editor} $parameters{logfile}");
				}
			} else {
				printit "DISPLAY is not defined or exported.\n";
			}
		} else {
			printit "\nNo Log File defined.\n";
		}
	} elsif ($line =~ /^cls[\s]*$/) {
		printit "\nClear Statistics: \n";
		foreach my $key (sort keys %statistics) {
			printit "${key}:\n";
			foreach my $key2 (sort keys %{$statistics{$key}}) {
				$statistics{$key}{$key2} = 0;
			}
		}
	} elsif ($line =~ /^dp[\s]*$/) {
		printit "\nParameters: \n";
		foreach my $key (sort keys %parameters) {
			printit "\t${key} = $parameters{$key}\n";
		}
	} elsif ($line =~ /^ep[\s]+([^\s]+)[\s]*$/) {
		printit "\nEditting Parameter:\n";
		my $parameter = $1;
		if (!exists($parameters{$parameter})) {
			printit "Parameter ${parameter} not found.\n";
			return;
		}
		my $saveflags = 0;
		fcntl(STDIN, F_GETFL, $saveflags);
		my $newflags = $saveflags & ~O_NONBLOCK;
		fcntl(STDIN, F_SETFL, $newflags);
		if (defined($parameters{$parameter})) {
			printit "\t$parameter => $parameters{$parameter}\n";
		} else {
			printit "\t\$parameters{$parameter} undefined\n";
		}
		printit "enter new value [<CR>=current value, 'NULL'=null value]: ";
		$line = <STDIN>;
		logit($line);
		chomp($line);
		if ($line =~ /^[\s]*$/) {
			printit "keeping current value.\n";
		} elsif ($line =~ /^NULL$/) {
			printit "nulling out current value .\n";
			$parameters{$parameter} = undef;
		} else {
			$parameters{$parameter} = $line;
			printit "\tnew value for $parameter => $parameters{$parameter}\n";
		}
		fcntl(STDIN, F_SETFL, $saveflags);
	} elsif ($line =~ /^ep[\s]*$/) {
		printit "\nEditting Parameters:\n";
		my $saveflags = 0;
		fcntl(STDIN, F_GETFL, $saveflags);
		my $newflags = $saveflags & ~O_NONBLOCK;
		fcntl(STDIN, F_SETFL, $newflags);
		foreach my $key (sort keys %parameters) {
			if (defined($parameters{$key})) {
				printit "\t$key => $parameters{$key}\n";
			} else {
				printit "\t\$parameters{$key} undefined\n";
			}
			printit "enter new value [<CR>=current value, 'NULL'=null value, '>'=done]: ";
			$line = <STDIN>;
			logit($line);
			chomp($line);
			if ($line =~ /^[\s]*$/) {
				printit "keeping current value.\n";
			} elsif ($line =~ /^NULL$/) {
				printit "nulling out current value .\n";
				$parameters{$key} = undef;
			} elsif ($line =~ /^>$/) {
				printit "keeping current value.\n";
				last;
			} else {
				$parameters{$key} = $line;
				printit "\tnew value for $key => $parameters{$key}\n";
			}
		}
		fcntl(STDIN, F_SETFL, $saveflags);
	} elsif ($line =~ /^s[\s]*$/) {
		printit "\nStatistics: \n";
		foreach my $key (sort keys %statistics) {
			printit "${key}:\n";
			foreach my $key2 (sort keys %{$statistics{$key}}) {
				printit "\t${key2} = $statistics{$key}{$key2}\n";
			}
		}
	} elsif ($line =~ /^D[\s]*$/) {
		$debugflag = !$debugflag;
		if ($debugflag) {
			printit "Debugging is ON\n";
		} else {
			printit "Debugging is OFF\n";
		}
	} elsif ($line =~ /^V[\s]*$/) {
		$verboseflag = !$verboseflag;
		if ($verboseflag) {
			printit "Verbose is ON\n";
		} else {
			printit "Verbose is OFF\n";
		}
	} elsif ($line =~ /^VR[\s]*$/) {
		$rtpverboseflag = !$rtpverboseflag;
		if ($rtpverboseflag) {
			printit "RTP Verbose is ON\n";
		} else {
			printit "RTP Verbose is OFF\n";
		}
	} elsif ($line =~ /^cl[\s]*$/) {
		printit "\nListing Call Originator-Terminator Pairs:\n";
		foreach my $key (sort keys %{$loaddata{calls}}) {
			printit "\tORIG $key ==>> TERM $loaddata{calls}{$key}\n";
		}
	} elsif ($line =~ /^cs[\s]*$/) {
		printit "\nListing Call Load Data:\n";
		foreach my $key (sort keys %{loaddata}) {
			printit "\t$key ==>> $loaddata{$key}\n";
		}
	} elsif ($line =~ /^ds[\s]*/) {
		printit "\nDump DN Call-Load Status Counts:\n";
		my %cnts = ();
		foreach my $dn (sort keys %dndata) {

			if (exists($cnts{$dndata{$dn}{callloadstate}})) {
				$cnts{$dndata{$dn}{callloadstate}} += 1;
			} else {
				$cnts{$dndata{$dn}{callloadstate}} = 1;
			}
		}
		foreach my $status (sort keys %cnts) {
			printit "${status}: $cnts{$status}\n";
		}
	} elsif ($line =~ /^e[\s]+([^\s]+)[\s]*$/) {
		my $listdn = $1;
		if (!exists($dndata{$listdn})) {
			printit "DN $listdn not found.\n";
			return;
		}
		printit "\nEditting DN $listdn data:\n";
		my $saveflags = 0;
		fcntl(STDIN, F_GETFL, $saveflags);
		my $newflags = $saveflags & ~O_NONBLOCK;
		fcntl(STDIN, F_SETFL, $newflags);
		foreach my $key (sort keys %{$dndata{$listdn}}) {
			if (defined($dndata{$listdn}{$key})) {
				printit "\t$key => $dndata{$listdn}{$key}\n";
			} else {
				printit "\t\$dndata{$listdn}{$key} undefined\n";
			}
			printit "enter new value [<CR>=current value, 'NULL'=null value, '>'=done]: ";
			$line = <STDIN>;
			logit($line);
			chomp($line);
			if ($line =~ /^[\s]*$/) {
				printit "keeping current value.\n";
			} elsif ($line =~ /^NULL$/) {
				printit "nulling out current value .\n";
				$dndata{$listdn}{$key} = undef;
			} elsif ($line =~ /^>$/) {
				printit "keeping current value.\n";
				last;
			} else {
				$dndata{$listdn}{$key} = $line;
				printit "\tnew value for $key => $dndata{$listdn}{$key}\n";
			}
		}
		fcntl(STDIN, F_SETFL, $saveflags);
	} elsif ($line =~ /^e[\s]+([^\s]+)[\s]+([^\s]+)[\s]*$/) {
		my $listdn = $1;
		my $parameter = $2;
		if (!exists($dndata{$listdn}{$parameter})) {
			printit "DN $listdn parameter ${parameter} not found.\n";
			return;
		}
		printit "\nEditting DN $listdn data:\n";
		my $saveflags = 0;
		fcntl(STDIN, F_GETFL, $saveflags);
		my $newflags = $saveflags & ~O_NONBLOCK;
		fcntl(STDIN, F_SETFL, $newflags);
		if (defined($dndata{$listdn}{$parameter})) {
			printit "\t$parameter => $dndata{$listdn}{$parameter}\n";
		} else {
			printit "\t\$dndata{$listdn}{$parameter} undefined\n";
		}
		printit "enter new value [<CR>=current value, 'NULL'=null value]: ";
		$line = <STDIN>;
		logit($line);
		chomp($line);
		if ($line =~ /^[\s]*$/) {
			printit "keeping current value.\n";
		} elsif ($line =~ /^NULL$/) {
			printit "nulling out current value .\n";
			$dndata{$listdn}{$parameter} = undef;
		} else {
			$dndata{$listdn}{$parameter} = $line;
			printit "\tnew value for $parameter => $dndata{$listdn}{$parameter}\n";
		}
		fcntl(STDIN, F_SETFL, $saveflags);
	} elsif ($line =~ /^l[\s]+([^\s]+)/) {
		my $listdn = $1;
		if (!exists($dndata{$listdn})) {
			printit "DN $listdn not found.\n";
			return;
		}
		printit "\nListing DN $listdn data:\n";
		foreach my $key (sort keys %{$dndata{$listdn}}) {
			if (defined($dndata{$listdn}{$key})) {
				printit "\t$key => $dndata{$listdn}{$key}\n";
			} else {
				printit "\t\$dndata{$listdn}{$key} undefined\n";
			}
		}
	} elsif ($line =~ /^l[\s]*$/) {
		printit "\nListing DNs:\n";
		foreach my $dn (sort keys %dndata) {
			printit "DN $dn\n";
			printit "\tregistrationstate => $dndata{$dn}{registrationstate}\n";
			printit "\toriginationstate => $dndata{$dn}{originationstate}\n";
			printit "\tterminationstate => $dndata{$dn}{terminationstate}\n";
		}
	} elsif ($line =~ /^d[\s]*$/) {
		printit "\nListing Dialogs:\n";
		foreach my $callid (sort keys %dialogs) {
			printit "Call-ID $callid\n";
		}
	} elsif ($line =~ /^r[\s]+([^\s]+)/) {
		my $regdn = $1;
		printit "\nRegistering DN $regdn.\n";
		queueReg($regdn, 0);
	} elsif ($line =~ /^pwd[\s]*$/) {
		my $pwd = getcwd;
		printit "PWD = ${pwd}\n";
	} elsif ($line =~ /^h[\s]*$/) {
		haltcallload();
	} elsif ($line =~ /^k[\s]*$/) {
		killcallload();
	} elsif ($line =~ /^k[\s]+([^\s]+)/) {
		my $dn = $1;
		killcall($dn);
	} elsif ($line =~ /^dpq[\s]*$/) {
		dumpPQ($pq);
	} elsif ($line =~ /^t[\s]*$/) {
		printit "\nCurrent time: ".scalar(localtime())."\n";
	} elsif ($line =~ /^cpq[\s]*$/) {
		printit "\nPQ count = ".$pq->count()."\n";
	} elsif ($line =~ /^o[\s]*$/) {
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "===============================================================\n";
		if (defined($previousorigdn) && defined($previoustermdn)) {
			printit "\nDN $previousorigdn calling DN $previoustermdn.\n";
			queueOrig($previousorigdn, $previoustermdn, 0, 0, undef, undef, undef);
			incrstats("origination","queueOrigCalled");
		} else {
			printit "Previous orig/term DNs not defined.\n";
		}
	} elsif ($line =~ /^o[\s]+([^\s]+)[\s]+([^\s]+)/) {
		my $origdn = $previousorigdn = $1;
		my $termdn = $previoustermdn = $2;
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "===============================================================\n";
		printit "\nDN $origdn calling DN $termdn.\n";
		queueOrig($origdn, $termdn, 0, 0, undef, undef, undef);
		incrstats("origination","queueOrigCalled");
	} elsif ($line =~ /^c[\s]+([\d]+)[\s]+([\d]+)[\s]*$/) {
		my $cps = $1;
		my $holdtime = $2;
		# give it BIG number to make it run forever.
		my $maxcalls = 4000000000;
		#
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "===============================================================\n";
		printit "\nStart call load:\n";
		printit "\tCalls Per Sec = $cps\n";
		printit "\tHold Time (secs) = $holdtime\n";
		#
		startcallload($cps, $holdtime, $maxcalls);
	} elsif ($line =~ /^c[\s]+([\d]+)[\s]+([\d]+)[\s]+([\d]+)[\s]*$/) {
		my $cps = $1;
		my $holdtime = $2;
		my $maxcalls = $3;
		#
		printit "\nCurrent time: ".scalar(localtime())."\n";
		printit "===============================================================\n";
		printit "\nStart call load:\n";
		printit "\tCalls Per Sec = $cps\n";
		printit "\tHold Time (secs) = $holdtime\n";
		printit "\tMaximum Number of Calls  = $maxcalls\n";
		#
		startcallload($cps, $holdtime, $maxcalls);
	} elsif ($line =~ /^\?/) {
		printit "\nhelp text:\n";
		printit "\t'?' - help text\n";
		printit "\t'c cps holdtime' - start a call load (indefinite).\n";
		printit "\t'c cps holdtime maxcalls' - start a call load.\n";
		printit "\t'cl' - list call originator-terminator pairs.\n";
		printit "\t'cls' - clear statistics.\n";
		printit "\t'cs' - list call load data.\n";
		printit "\t'ds' - dn call-load status counts.\n";
		printit "\t'd' - list active dialogs.\n";
		printit "\t'dp' - dump parameters.\n";
		printit "\t'ep' - edit parameters.\n";
		printit "\t'ep parameter' - edit specific parameter.\n";
		printit "\t'D' - toggle debugging.\n";
		printit "\t'dpq' - dump priority queue contents.\n";
		printit "\t'cpq' - count of priority queue items.\n";
		printit "\t'h' - stop call load gracefully.\n";
		printit "\t'k' - kill call load (send BYE).\n";
		printit "\t'k odn' - kill call for this originating DN.\n";
		printit "\t'l' - list all DN parameters.\n";
		printit "\t'l dn' - list DN parameters.\n";
		printit "\t'e dn' - edit DN all parameters.\n";
		printit "\t'e dn parameter' - edit specific DN parameter.\n";
		printit "\t'o dn dialdn' - dn calls dialdn.\n";
		printit "\t'o' - repeat last 'o dn dialdn' cmd.\n";
		printit "\t'pwd' - show present working directory\n";
		printit "\t'q' - quit\n";
		printit "\t'r dn' - register dn.\n";
		printit "\t's' - list statistics.\n";
		printit "\t't' - print current time\n";
		printit "\t'vi' - edit logfile..\n";
		printit "\t'V' - toggle verbose.\n";
		printit "\t'VR' - toggle RTP verbose (verbose must also be enabled).\n";
		printit "\t'w' - write a line of '====' to stdout/logfile.\n";
		printit "\t'w text' - write 'text' to stdout/logfile.\n";
	} else {
		printit "\nunknown cmd\n";
	}
	return;
}
#
sub handleAdminInput {
	my ($pdone) = @_;
	#
	my $line = <ADMIN>;
	if (!defined($line)) {
		return;
	}
	logit($line);
	chomp($line);
	#
	parseAndExecInput($line, $pdone);
	return;
}
#
sub handleInput {
	my ($pdone) = @_;
	#
	my $line = <STDIN>;
	if (!defined($line)) {
		return;
	}
	logit($line);
	chomp($line);
	#
	parseAndExecInput($line, $pdone);
	return;
}
#
#
sub handleRTP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	#
	my $msglen = length($rawmsg);
	if ($rtpverboseflag) {
		printit "\nRTP msg received: length=$msglen\n";
	}
	#
	my @rtpdata = unpack "N*", $rawmsg;
	#
	my $v = rbitval($rtpdata[0], 0, 1);
	my $p = rbitval($rtpdata[0], 2, 2);
	my $x = rbitval($rtpdata[0], 3, 3);
	my $cc = rbitval($rtpdata[0], 4, 7);
	my $m = rbitval($rtpdata[0], 8, 8);
	my $pt = rbitval($rtpdata[0], 9, 15);
	my $seqno = rbitval($rtpdata[0], 16, 31);
	my $tstamp = $rtpdata[1];
	my $ssrc = $rtpdata[2];
	#
	if ($rtpverboseflag) {
		printit "v=$v \n";
		printit "p=$p \n";
		printit "x=$x \n";
		printit "cc=$cc \n";
		printit "m=$m \n";
		printit "pt=$pt \n";
		printit "seqno=$seqno \n";
		printit "tstamp=$tstamp \n";
		printit "ssrc=$ssrc \n";
		#
		if ($rtplogflag) {
			print RTPLOGFILE "===START RTP===\n";
			print RTPLOGFILE @rtpdata;
			print RTPLOGFILE "\n===END RTP===\n";
		}
	}
	return;
}
#
sub handleRTCP {
	my ($current_time, $recvpaddr, $rawmsg) = @_;
	#
	my $msglen = length($rawmsg);
	if ($rtpverboseflag) {
		printit "\nRTCP msg received: length=$msglen\n";
	}
	#
	my @rtcpdata = unpack "N*", $rawmsg;
	#
	my $v = rbitval($rtcpdata[0], 0, 1);
	my $p = rbitval($rtcpdata[0], 2, 2);
	my $ic = rbitval($rtcpdata[0], 3, 7);
	my $pt = rbitval($rtcpdata[0], 8, 15);
	my $length = rbitval($rtcpdata[0], 16, 31);
	#
	if ($rtpverboseflag) {
		printit "v=$v \n";
		printit "p=$p \n";
		printit "ic=$ic \n";
		printit "pt=$pt \n";
		printit "length=$length \n";
		#
		if ($rtplogflag) {
			print RTPLOGFILE "===START RTCP===\n";
			print RTPLOGFILE @rtcpdata;
			print RTPLOGFILE "\n===END RTCP===\n";
		}
	}
	return;
}
#
sub handleCallLoad {
	my ($ptimer) = @_;
	#
	if (!isMyTimer("CALL-LOAD", $ptimer)) {
		$ptimer = undef;
		return;
	}
	if (($loaddata{callsstarted} >= $loaddata{maxcalls}) ||
	    ($loaddata{state} ne "running")) {
		# done with starting calls for now.
		if ($loaddata{callsstarted} >= $loaddata{maxcalls}) {
			printit "\n\thandleCallLoad: Call load done.\n";
		} elsif ($loaddata{state} ne "running") {
			printit "\n\thandleCallLoad: Halting call load.\n";
		}
		#
		$loaddata{state} = "idle";
		#
		printit "\nListing Call Load Data:\n";
		foreach my $key (sort keys %{loaddata}) {
			printit "\t$key ==>> $loaddata{$key}\n";
		}
		#
		printit "\nStatistics: \n";
		foreach my $key (sort keys %statistics) {
			printit "${key}:\n";
			foreach my $key2 (sort keys %{$statistics{$key}}) {
				printit "\t${key2} = $statistics{$key}{$key2}\n";
			}
		}
		return;
	}
	#
	my $newcalls = 0;
	#
	my $o=0;
	for ( ; $o<$loaddata{origdnsz}; $loaddata{currentorigdn}++, ${o}++) {
		my $i = $loaddata{currentorigdn}%$loaddata{origdnsz};
		my $odn = $loaddata{origdns}[$i];
		my $tdn = $loaddata{calls}{$odn};
		#
		if ($verboseflag) {
			vprint "handleCallLoad: ORID DN - TRYING $odn.\n";
			vprint "handleCallLoad: TERM DN - TRYING $tdn.\n";
		}
		#
		if ($dndata{$odn}{callloadstate} ne "INACTIVE") {
			# skip this orig dn.
			if ($verboseflag) {
				vprint "\thandlecallload: ORID DN - SKIPPING $odn (state=$dndata{$odn}{callloadstate}).\n";
				vprint "\thandleCallLoad: TERM DN - SKIPPING $tdn.\n";
			}

			next;
		}
		if ($verboseflag) {
			vprint "\thandlecallload: ORID DN - USING $odn (state=$dndata{$odn}{callloadstate}).\n";
			vprint "\thandlecallload: TERM DN - USING $tdn.\n";
		}
		# set max length of call.
		$dndata{$odn}{"maxcallduration"} = $loaddata{holdtime};
		# we found a dn to start a call.
		queueOrig($odn, $tdn, 1, 0, undef, undef, undef);
		$dndata{$odn}{callloadstate} = "SCHEDULED";
		incrstats("origination","queueOrigCalled");
		# 
		${newcalls}++;
		$loaddata{callsstarted}++;
		#
		if ((${newcalls} >= $loaddata{callspersec}) ||
		    ($loaddata{callsstarted} >= $loaddata{maxcalls})) {
			# done with starting calls for now.
			last;
		}
	}
	#
	vprint "handleCallLoad: $newcalls new calls queue ($newcalls out of $o).\n";
	# queue up next event to startup a call load.
	if ($loaddata{callsstarted} >= $loaddata{maxcalls}) {
		# done with starting calls for now.
		$loaddata{state} = "idle";
		printit "\thandleCallLoad: Call load done.\n";
		#
		printit "\nListing Call Load Data:\n";
		foreach my $key (sort keys %{loaddata}) {
			printit "\t$key ==>> $loaddata{$key}\n";
		}
		#
		printit "\nStatistics: \n";
		foreach my $key (sort keys %statistics) {
			printit "${key}:\n";
			foreach my $key2 (sort keys %{$statistics{$key}}) {
				printit "\t${key2} = $statistics{$key}{$key2}\n";
			}
		}
	} else {
		# schedule next call load event
		starttimer("CALL-LOAD", 1, "CALL-LOAD");
	}
	return;
}
#
sub handleCallReport {
	my ($ptimer) = @_;
	#
	if (!isMyTimer("CALL-REPORT", $ptimer)) {
		$ptimer = undef;
		return;
	}
	#
	printit "\nCall Statistics Report: ".scalar(localtime())."\n";
	printit "\nStatistics: \n";
	foreach my $key (sort keys %statistics) {
		printit "${key}:\n";
		foreach my $key2 (sort keys %{$statistics{$key}}) {
			printit "\t${key2} = $statistics{$key}{$key2}\n";
		}
	}
	#
	printit "\nListing Call Load Data:\n";
	foreach my $key (sort keys %{loaddata}) {
		printit "\t$key ==>> $loaddata{$key}\n";
	}
	#
	starttimer("CALL-REPORT", $parameters{interreporttime}, "CALL-REPORT");
}
#
sub handleTimer {
	my ($current_time, $ptimer) = @_;
	#
	if ($verboseflag) {
		vprint "\nTimer expired: \n";
	}
	# check if call id data exits
	if (!defined($ptimer->{callid})) {
		vprint "\nNo Call-ID defined in timer.\n";
		$ptimer = undef;
		return;
	}
	my $callid = $ptimer->{callid};
	if (!defined($dialogs{$callid})) {
		vprint "\nNo timer dialog data for $callid defined.\n";
		vprint "Call-ID: ".$ptimer->{callid}."\n";
		vprint "Tag    : ".$ptimer->{expire}."\n";
		vprint "Label  : ".$ptimer->{label}."\n";
		$ptimer = undef;
		return;
	}
	# check for call load event.
	if ($ptimer->{callid} eq "CALL-LOAD") {
		# start a call load.
		handleCallLoad($ptimer);
		return;
	}
	# check for call report event.
	if ($ptimer->{callid} eq "CALL-REPORT") {
		# start a call report.
		handleCallReport($ptimer);
		return;
	}
	#
	my $timertype = 0;
	if ($ptimer->{label} =~ /^ORIGINATION$/) {
		# call origination event
		$timertype = 2;
	} elsif ($ptimer->{label} =~ /^REGISTRATION$/) {
		# registration event
		$timertype = 3;
	} else {
		# regular timer
		$timertype = 1;
	}
	#
	my $handler = $dialogs{$callid}{handler};
	&{${handler}}(undef, $callid, $timertype, $current_time, 
		undef, undef, undef, $ptimer, 0);
	return;
}
#
sub handleTCP {
	my ($current_time, $fd, $reinvite) = @_;
	#
	# check if we have a complete msg.
	#
	if (!defined($input[$fd])) {
		# no message at all.
		return;
	}
	if ($input[$fd] !~ /^[^\r\n]*[\r\n]{4,4}/) {
		# not a complete message yet.
		return;
	}
	#
	# we have the hdr part of msg, check if we have the
	# payload part of the msg.
	#
	my $rawhdr = undef;
	my $rawdata = undef;
	#
	($rawhdr, $rawdata) = split /[\r\n]{4,4}/, $input[$fd], 2;
	return unless (defined($rawhdr));
	#
	# get the Content-Length and determine if we have read enough.
	#
	$rawhdr =~ /^.*Content-Length:[\s]*\([0-9][0-9]*\)[\s]*[\r\n]{2,2}/;
	if (!defined($1)) {
		# no content length found
		return;
	}
	my $contentlen = $1;
	#
	if ($contentlen <= 0) {
		# we have a complete message without a payload.
		my $rawmsg = $rawhdr."\r\n\r\n";
		my $rawmsglen = length($rawmsg);
		# adjust input stream buffer
		my $inputlen = length($input[$fd]);
		if ($rawmsglen <= $inputlen) {
			# everything was read.
			$input[$fd] = '';
		} else {
			# part of another message is left. remove
			# current message from input buffer.
			$input[$fd] = substr($input[$fd], $rawmsglen, 
					($inputlen-$rawmsglen));
		}
		# call SIP handler
		handleSIP($current_time, $fd2paddr[$fd], 
				$rawmsg, $reinvite, $fd);
	} else {
		# check if the raw data is greater than the content length.
		if (!defined($rawdata)) {
			# no data, keep reading
			return;
		}
		my $rawdatalen = length($rawdata);
		if ($rawdatalen < $contentlen) {
			# not enough data, keep reading
			return;
		}
		# get the payload from the rawdata
		$rawdata = substr($rawdata, 0, $contentlen);
		# we have a complete message with a payload.
		my $rawmsg = $rawhdr."\r\n\r\n".$rawdata;
		my $rawmsglen = length($rawmsg);
		# adjust input stream buffer
		my $inputlen = length($input[$fd]);
		if ($rawmsglen <= $inputlen) {
			# everything was read.
			$input[$fd] = '';
		} else {
			# part of another message is left. remove
			# current message from input buffer.
			$input[$fd] = substr($input[$fd], $rawmsglen, 
					($inputlen-$rawmsglen));
		}
		# call SIP handler
		handleSIP($current_time, $fd2paddr[$fd], 
				$rawmsg, $reinvite, $fd);
	}
	return;
}
#
sub handleSIP {
	my ($current_time, $recvpaddr, $rawmsg, $reinvite, $fd) = @_;
	#
	my $msglen = length($rawmsg);
	if ($msglen < $parameters{minmsgsize}) {
		# garbage msg, skip it ...
		incrstats("general", "discarded-min-size-msgs");
		return;
	}
	if ($verboseflag) {
		vprint "\nSIP msg received: length=$msglen\n";
		dumpIncomingMsg($recvpaddr, $rawmsg);
	}
	#
	my %hdrs = ();
	my @datablocks = ();
	my $rawhdr = undef;
	my $rawdata = undef;
	#
	my $ret = parseSipMsg($rawmsg, \$rawhdr, \$rawdata, \%hdrs, \@datablocks, $reinvite, $fd);
	if ($verboseflag) {
		dumphdrs(\%hdrs);
	}
	if ($ret == 404) {
		vprint "\nUnknown DN ".$hdrs{"dn"}."\n";
		incrstats("general", "handleSip-Unknown-DN");
		send404NotFound(\%hdrs);
		return;
	}
	#
	my $callid = undef;
	if (defined($hdrs{"Call-ID"}) && 
	    defined($dialogs{$hdrs{"Call-ID"}})) {
		$callid = $hdrs{"Call-ID"};
	} else {
		if ($verboseflag) {
			vprint "\nNo dialog data found.\n";
		}
		Incrstats("general", "handleSip-sendGratuitous200OK");
		sendGratuitous200OK(\%hdrs);
		return;
	}
	if (!defined($dialogs{$callid}{handler})) {
		vprint "\nNo handler for $callid defined.\n";
		incrstats("general", "handleSip-no-handler-for-call-id");
		send404NotFound(\%hdrs);
		return;
	}
	my $handler = $dialogs{$callid}{handler};
	&{${handler}}($rawmsg, $callid, 0, $current_time, $recvpaddr, 
			\@datablocks, \%hdrs, undef, $reinvite);
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
getopts('?xVl:m:r:', \%opts);
foreach my $opt (%opts) {
	if ($opt eq "l") {
		$parameters{logfile} = $opts{$opt};
		printit "\n==>> logfile = $parameters{logfile}\n";
		open(LOGFILE, ">".$parameters{logfile}) or 
			die "can't open $parameters{logfile}: $!";
		LOGFILE->autoflush(1);
		$logflag = 1;
	} elsif ($opt eq "m") {
		$parameters{msglogfile} = $opts{$opt};
		printit "\n==>> MSG logfile = $parameters{msglogfile}\n";
		open(MSGLOGFILE, ">".$parameters{msglogfile}) or 
			die "can't open $parameters{msglogfile}: $!";
		binmode(MSGLOGFILE);
		MSGLOGFILE->autoflush(1);
		$msglogflag = 1;
	} elsif ($opt eq "r") {
		$parameters{rtplogfile} = $opts{$opt};
		printit "\n==>> RTP logfile = $parameters{rtplogfile}\n";
		open(RTPLOGFILE, ">".$parameters{rtplogfile}) or 
			die "can't open $parameters{rtplogfile}: $!";
		# binmode(RTPLOGFILE);
		RTPLOGFILE->autoflush(1);
		$rtplogflag = 1;
	} elsif ($opt eq "x") {
		$debugflag = 1;
		printit "\n==>> debugflag = $debugflag\n";
	} elsif ($opt eq "V") {
		$verboseflag = 1;
		printit "\n==>> verboseflag = $verboseflag\n";
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
	$configfile = "nsipsimcfg";
}
readconfigfile($configfile);
#
dumpparams();
dumpdndata();
#
# set up socket connections
#
$verboseflag = 1;
#
my $udp_proto = getprotobyname('udp');
defined($udp_proto) or die "getprotobyname: $!";
my $tcp_proto = getprotobyname('tcp');
defined($tcp_proto) or die "getprotobyname: $!";
#
vprint "SIP IP  : $parameters{mysipip}\n";
vprint "SIP PORT: $parameters{mysipport}\n";
socket(SIP, PF_INET, SOCK_DGRAM, $udp_proto) or die "socket: $!";
my $ipaddr = gethostbyname($parameters{mysipip});
defined($ipaddr) or die "gethostbyname: $!";
my $paddr = sockaddr_in($parameters{mysipport}, $ipaddr);
defined($paddr) or die "sockaddr_in: $!";
bind(SIP, $paddr) or die "bind: $!";
#
# set up TCP SIP SERVER port.
vprint "TCP IP  : $parameters{mytcpsipip}\n";
vprint "TCP PORT: $parameters{mytcpsipport}\n";
socket(SIPTCP, PF_INET, SOCK_STREAM, $tcp_proto);
setsockopt(SIPTCP, SOL_SOCKET, SO_REUSEADDR, 1);
my $tcp_ipaddr = gethostbyname($parameters{mytcpsipip});
defined($tcp_ipaddr) or die "gethostbyname: $!";
my $tcp_paddr = sockaddr_in($parameters{mytcpsipport}, $tcp_ipaddr)
defined($tcp_paddr) or die "sockaddr_in: $!";
bind(SIPTCP, $tcp_paddr) or die "bind: $!";
listen(SIPTCP, SOMAXCONN) or die "listen: $!";
#
vprint "ADMIN IP  : $parameters{myadminip}\n";
vprint "ADMIN PORT: $parameters{myadminport}\n";
socket(ADMIN, PF_INET, SOCK_DGRAM, $udp_proto) or die "socket: $!";
my $admin_ipaddr = gethostbyname($parameters{myadminip});
defined($admin_ipaddr) or die "gethostbyname: $!";
my $admin_paddr = sockaddr_in($parameters{myadminport}, $admin_ipaddr);
defined($admin_paddr) or die "sockaddr_in: $!";
bind(ADMIN, $admin_paddr) or die "bind: $!";
#
if ($parameters{supportrtp}) {
	socket(RTP, PF_INET, SOCK_DGRAM, $udp_proto) or die "socket: $!";
	vprint "RTP IP  : $parameters{myrtpip}\n";
	vprint "RTP PORT: $parameters{myrtpport}\n";
	my $rtp_ipaddr = gethostbyname($parameters{myrtpip});
	defined($rtp_ipaddr) or die "gethostbyname: $!";
	my $rtp_paddr = sockaddr_in($parameters{myrtpport}, $rtp_ipaddr);
	defined($rtp_paddr) or die "sockaddr_in: $!";
	bind(RTP, $rtp_paddr) or die "bind: $!";
	#
	vprint "RTCP PORT: $parameters{myrtcpport}\n";
	socket(RTCP, PF_INET, SOCK_DGRAM, $udp_proto) or die "socket: $!";
	my $rtcp_paddr = sockaddr_in($parameters{myrtcpport}, $rtp_ipaddr);
	defined($rtcp_paddr) or die "sockaddr_in: $!";
	bind(RTCP, $rtcp_paddr) or die "bind: $!";
}
#
$rin = '';
$rout = '';
$win = '';
$wout = '';
$ein = '';
$eout = '';
#
vec($rin, fileno(STDIN), 1) = 1;
vec($rin, fileno(SIP), 1) = 1;
vec($rin, fileno(ADMIN), 1) = 1;
vec($rin, fileno(SIPTCP), 1) = 1;
if ($parameters{supportrtp}) {
	vec($rin, fileno(RTP), 1) = 1;
	vec($rin, fileno(RTCP), 1) = 1;
}
#
# we don't want to I/O block. if no input, just return.
#
fcntl(STDIN, F_SETFL, O_NONBLOCK);
fcntl(SIP, F_SETFL, O_NONBLOCK);
fcntl(ADMIN, F_SETFL, O_NONBLOCK);
fcntl(SIPTCP, F_SETFL, O_NONBLOCK);
if ($parameters{supportrtp}) {
	fcntl(RTP, F_SETFL, O_NONBLOCK);
	fcntl(RTCP, F_SETFL, O_NONBLOCK);
}
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
$pq = "mypqueue"->new();
defined($pq) or die "unable to create priority queue.";
#
# ISUP decoder/encoder
#
$isup = "myisup"->new(\&vprint);
#
# event loop for simulation.
#
my $mydelta;
my $key;
my $pdata;
my $nf;
my $timeleft;
#
# queue up default report generation
#
starttimer("CALL-REPORT", $parameters{interreporttime}, "CALL-REPORT");
#
# ignore SIGCHLD from dead processes. avoid zombies.
#
$SIG{CHLD} = "IGNORE";
#
while ( ! $done) {
	$previous_time = $current_time;
	#
	# determine how long to sleep.
	#
	if ($pq->isEmpty()) {
		# no timers, just use default value.
		$mydelta = $parameters{delta};
	} elsif ($pq->front(\$pdata) == 1) {
		# sleep until next scheduled event.
		$mydelta = $pdata->{expire} - $current_time;
		$mydelta = 0 if ($mydelta < 0);
	} else {
		printit "ERROR: event queue corruption, unable to calculate delta !!!\n";
		exit 2;
	}
	#
	# wait for input or timer to expire.
	#
	($nf, $timeleft) = select($rout=$rin, $wout=$win, $eout=$ein, $mydelta);
	#
	# update internal clocks
	#
	$current_time = time();
	$elapsed_time = $current_time-$previous_time;
	$total_elapsed_time = $current_time-$start_time;
	#
	# handle any expired timers
	#
	if ($timeleft <= 0) {
		while ($pq->dequeue(\$pdata) != 0) {
			if ($pdata->{expire} > $current_time) {
				$pq->enqueue($pdata);
				last;
			}
			handleTimer($current_time, $pdata);
		}
	}
	#
	# handle any input from user.
	#
	if (vec($rout, fileno(STDIN), 1)) {
		handleInput(\$done);
	}
	if (vec($rout, fileno(ADMIN), 1)) {
		handleAdminInput(\$done);
	}
	#
	# handle any msgs from sockets: RTP, RTCP, SIP.
	#
	# SIP UDP port
	#
	if (vec($rout, fileno(SIP), 1)) {
		while (defined($recvpaddr = recv(SIP, my $msg, 8*1024, 0))) {
			if ($msglogflag) {
				print MSGLOGFILE $msg;
			}
			handleSIP($current_time, $recvpaddr, $msg, 0, undef);
		}
	}
	#
	# SIP TCP server
	#
	if (vec($rout, fileno(SIPTCP), 1)) {
		my $NS = FileHandle->new();
		# accept returns the remote client IP/PORT.
		if (my $client_paddr = accept($NS, SIPTCP)) {
			my ($client_port, $client_packed_ip) =
				sockaddr_in($client_paddr);
			my $client_ascii_ip = inet_ntoa($client_packed_ip);
			#
			vec($rin, fileno($NS), 1) = 1;
			vec($ein, fileno($NS), 1) = 1;
			#
			$input[fileno($NS)] = '';
			$output[fileno($NS)] = '';
			#
			$fd2file[file($NS)] = $NS;
			$fd2paddr[file($NS)] = $client_paddr;
			$fd2ip[file($NS)] = $client_ascii_ip;
			$fd2port[file($NS)] = $client_port;
			#
			fcntl($NS, F_SETFL, O_NONBLOCK);
			push((@clients), fileno($NS));
		}
	}
	#
	# SIP TCP clients
	#
	@cl = @clients;
	#
	while ($fd = shift @cl) {
		if (vec($eout, $fd, 1)) {
			killclient($fd);
			next;
		}
		if (vec($wout, $fd, 1)) {
			$len = length($output[$fd]);
			if ($len > 0) {
				$w = syswrite($fd2file[$fd],$output[$fd],$len);
				if (!defined($w)) {
					printit "ERROR: syswrite failed.\n";
					killclient($fd);
				} else {
					$output[$fd] = substr($output[$fd], 
							$w, $len-$w);
				}
			} else {
				# nothing left to write.
				vec($win, $fd, 1) = 0;
			}
		}
		if (vec($rout, $fd, 1)) {
			$r = sysread($fd2file[$fd], $input[$fd], 2048, 
					length($input[$fd]));
			if (!defined($r) || !$r) {
				printit "ERROR: sysread failed.\n";
				killclient($fd);
			} else {
				# not a reinvite
				handleTCP($current_time, $fd, 0);
			}
		}
	}
	#
	# RTP and RTCP ports
	#
	if ($parameters{supportrtp}) {
		if (vec($rout, fileno(RTCP), 1)) {
			while (defined($recvpaddr =
				recv(RTCP, my $msg, 8*1024, 0))) {
				handleRTCP($current_time, $recvpaddr, $msg);
			}
		}
		if (vec($rout, fileno(RTP), 1)) {
			while (defined($recvpaddr =
				recv(RTP, my $msg, 8*1024, 0))) {
				handleRTP($current_time, $recvpaddr, $msg);
			}
		}
	}
}
#
close(SIP);
close(SIPTCP);
close(ADMIN);
if ($parameters{supportrtp}) {
	close(RTP);
	close(RTCP);
}
close(LOGFILE) if (exists($parameters{logfile}));
close(RTPLOGFILE) if (exists($parameters{rtplogfile}));
close(MSGLOGFILE) if (exists($parameters{msglogfile}));
#
exit 0;

