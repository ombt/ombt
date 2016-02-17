#!/opt/exp/bin/perl -w
#
# SIP REFER msg simulator.
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
$parameters{delta} = 10;
$parameters{minmsgsize} = 10;
#
%calldata = (
	DUMMY => {
		state => "INACTIVE",
		handler => \&refersuccess,
		Scenario => "refersuccess",
	},
);
#
# available call scenarios. add here they are written. then
# the scenario can be chosen from the cfg file via "Scenario"
# field.
#
%scenarios = (
	"refersuccess" => \&refersuccess,
	"referfailure" => \&referfailure,
);
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
	my ($callid, $timerflag, $current_time, $recvpaddr, 
	    $rawmsg, $rawhdr, $rawdata, $phdrs, $ptimer) = @_;
	#
	print STDERR "\nEntry refersuccess: Call-ID = <$callid>\n";
	#
	if (!exists($calldata{$callid})) {
		print STDERR "\nNo calldata for Call-ID $callid.\n";
		return;
	}
	if (!exists(${$calldata{$callid}}{state})) {
		print STDERR "\nNo calldata state for Call-ID $callid.\n";
		return;
	}
	dumpCallData();
	#
	my $currstate = ${$calldata{$callid}}{state};
	print STDERR "\nCurrent State for $callid = <$currstate>\n";
	print STDERR "Timer Flag for $callid = <$timerflag>\n";
	#
	if ($currstate =~ /^INACTIVE/) {
		if ($timerflag) {
			print STDERR "\nrefersuccess INACTIVE error:\n";
			print STDERR "Unexpected timer.\n";
			print STDERR "(callid=$callid).\n";
			#
			$ptimer = undef;
			${$calldata{$callid}}{state} = "INACTIVE";
		} elsif ($phdrs->{MSGTYPE} =~ /NOTIFY/) {
			$ackmsg = "SIP/2.0 200 Accepted\r\n";
			$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
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
			${$calldata{$callid}}{state} = "INACTIVE";
		} elsif ($phdrs->{MSGTYPE} =~ /INVITE/) {
			$ackmsg = $phdrs->{"FIRSTLINE"}."\r\n";
			$ackmsg =~ s/INVITE/REFER/;
			$ackmsg .= "Via: ".$phdrs->{"Via"}."\r\n";
			$ackmsg .= "To: ".$phdrs->{"To"}."\r\n";
			$ackmsg .= "From: ".$phdrs->{"From"}."\r\n";
			$ackmsg .= "Call-ID: ".$phdrs->{"Call-ID"}."\r\n";
			$ackmsg .= "CSeq: ".$phdrs->{"CSeq"}."\r\n";
			$ackmsg .= "Max-Forwards: ".$phdrs->{"Max-Forwards"}."\r\n";
			$ackmsg .= "Refer-To: ";
			$ackmsg .= ${$calldata{$callid}}{"Refer-To"}."\r\n";
			$ackmsg .= "Contact: ".$phdrs->{"Contact"}."\r\n";
			$ackmsg .= "Content-Length: 0\r\n";
			$ackmsg .= "\r\n";
			#
			dumpOutgoingMsg($recvpaddr, $ackmsg);
			defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or 
				die "send: $!";
			#
			my $endtime = time() + 
				${calldata{$callid}}{"WaitFor202Timer"};
			$newtimer{"Call-ID"} = $callid;
			mypqueue::enqueue($pq, $endtime, \%newtimer);
			${$calldata{$callid}}{state} = "WAITFOR202";
		} else {
			print STDERR "\nrefersuccess INACTIVE error:\n";
			print STDERR "Unexpected msg $phdrs->{MSGTYPE}\.\n";
			print STDERR "(callid=$callid).\n";
			#
			${$calldata{$callid}}{state} = "INACTIVE";
		}
	} elsif ($currstate =~ /^WAITFOR202/) {
		if ($timerflag) {
			print STDERR "\nrefersuccess WAITFOR202 error:\n";
			print STDERR "Timer expired.\n";
			print STDERR "(callid=$callid).\n";
			#
			$ptimer = undef;
			${$calldata{$callid}}{state} = "INACTIVE";
		} elsif ($phdrs->{MSGTYPE} =~ /^RETURNCODE/) {
			my $retcode = $phdrs->{RETURNCODE};
			if ($retcode == 202) {
				print STDERR "\nrefersuccess: 202 received.\n";
			} else {
				print STDERR "\nrefersuccess WAITFOR202 error:\n";
				print STDERR "202 NOT received.\n";
				print STDERR "(callid=$callid).\n";
			}
			#
			${$calldata{$callid}}{state} = "INACTIVE";
		} else {
			print STDERR "\nrefersuccess WAITFOR202 error:\n";
			print STDERR "Unexpected msg $phdrs->{MSGTYPE}.\n";
			print STDERR "(callid=$callid).\n";
			#
			${$calldata{$callid}}{state} = "INACTIVE";
		}
	} else {
		print STDERR "\nrefersuccess error:\n";
		print STDERR "Unexpected state $currstate.\n";
		print STDERR "(callid=$callid).\n";
		#
		${$calldata{$callid}}{state} = "INACTIVE";
	}
}
#
sub referfailure {
	print STDERR "\nreferfailure: not supported for now. Exiting.\n";
	exit 2;
}
########################################################################
#
# functions
#
sub dumpCallData {
	print STDERR "\n";
	print STDERR "Dumping Call-Data structure:\n";
	foreach $key (keys %calldata) {
		print STDERR "key --> <$key>\n";
		foreach $key2 (keys %{$calldata{$key}}) {
			print STDERR "\tkey2 --> <$key2>\n";
			if (defined($calldata{$key}{$key2})) {
				print STDERR "\t\t$calldata{$key}{$key2}\n";
			} else {
				print STDERR "\t\tundefined \$calldata{$key}{$key2}\n";
			}
		}
	}
}
#
sub readconfigfile {
	my ($configfile) = @_;
	#
	my $callid = "";
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
		if ($key =~ /Call-ID/) {
			print STDERR "\nReading data for Call-ID $value ...\n";
			$callid = $value;
			$calldata{$callid} = {
				state => "INACTIVE",
				handler => \&refersuccess,
				Scenario => "refersuccess",
			};
		} else {
			print STDERR "\$calldata{$callid}{$key} = $value\n";
			${$calldata{$callid}}{$key} = $value;
		}
	}
	close(CONFIGFILE);
	#
	foreach $key (keys %calldata) {
		if (exists $scenarios{${$calldata{$key}}{Scenario}}) {
			${$calldata{$callid}}{handler} = 
				$scenarios{${$calldata{$key}}{Scenario}};
		} else {
			print STDERR "\nUnknown scenario <${$calldata{$key}}{Scenario}>.\n";
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
	# my $packmsg = pack("u", $rawmsg);
	# print "packmsg = $packmsg\n";
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
}
#
sub dumpIncomingMsg {
	my ($recvpaddr, $msg) = @_;
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	print STDERR "\n";
	print STDERR "incoming msg <<<<<<<<<<<<<<<<<<\n";
	print STDERR "FROM IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		print("line=<$line>\n");
	}
	print STDERR "incoming msg <<<<<<<<<<<<<<<<<<\n";
}
#
sub dumpOutgoingMsg {
	my ($recvpaddr, $msg) = @_;
	#
	my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
	$recvipaddr = inet_ntoa($recvipaddr);
	my $recvaddr = "$recvipaddr:$recvport";
	#
	print STDERR "\n";
	print STDERR ">>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
	print STDERR "TO IP/PORT: $recvaddr\n";
	my @newmsg = split /[\r\n]+/, $msg;
	foreach $line (@newmsg) {
		print("line=<$line>\n");
	}
	print STDERR ">>>>>>>>>>>>>>>>>>>>> outgoing msg\n";
}
#
sub dumpHdrs {
	my ($phdrs) = @_;
	#
	print STDERR "\n";
	foreach $key (keys %$phdrs) {
		print STDERR "$key ==> ";
		print STDERR $phdrs->{$key};
		print STDERR "\n";
	}
}
#
sub sendDummyMsg {
	my ($recvpaddr, $phdrs) = @_;
	my $ackmsg = "GOT YOUR MSG ...\n";
	foreach $hdr (keys %$phdrs) {
		$ackmsg .= $phdrs->{$hdr};
		$ackmsg .= "\n";
	}
	#
	dumpOutgoingMsg($recvpaddr, $ackmsg);
	defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or die "send: $!";
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
		print STDERR "\nQuitting ...\n";
		$$done_ref = 1;
	}
}
#
sub handleEvent {
	my ($current_time, $recvpaddr, $rawmsg, $ptimer) = @_;
	#
	if (defined($ptimer)) {
		if (!defined($ptimer->{"Call-ID"})) {
			print STDERR "\nNo Call-ID defined in timer.\n";
			return;
		}
		if (!defined($calldata{$ptimer->{"Call-ID"}})) {
			my $callid = $ptimer->{"Call-ID"};
			print STDERR "\nNo calldata for Call-ID $callid defined.\n";
			return;
		}
		#
		my $callid = $ptimer->{"Call-ID"};
		$calldata{$callid}{handler}->($callid, 1, $current_time, 
			undef, undef, undef, undef, undef, $ptimer);
	} else {
		# ignore garbage msgs that are to small.
		if (length($rawmsg) < $parameters{minmsgsize})
		{
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
		if (!defined($hdrs{"Call-ID"})) {
			print STDERR "\nNo Call-ID defined in msg.\n";
			return;
		}
		if (!defined($calldata{$hdrs{"Call-ID"}})) {
			my $callid = $hdrs{"Call-ID"};
			print STDERR "\nNo calldata for Call-ID $callid defined.\n";
			return;
		}
		#
		my $callid = $hdrs{"Call-ID"};
		$calldata{$callid}{handler}->($callid, 0, $current_time, 
			$recvpaddr, $rawmsg, $rawhdr, $rawdata, \%hdrs, undef);
	}
	return;
}
#
#############################################################################
#
# get IP address and port from cmd line.
#
print STDERR "\nSIP REFER Simulator:\n";
#
#
# get cmd line options
#
my %opts;
getopts('d:', \%opts);
foreach my $opt (%opts) {
	if ($opt eq "d") {
		$parameters{delta} = $opts{$opt};
		print STDERR "==>> delta = $parameters{delta}\n";
	}
}
#
# sanity check
#
my ($udpip, $udpport, $configfile) = @ARGV;
if (!defined($udpip) || !defined($udpport) || !defined($configfile)) {
	print STDERR "\nusage: refersim [-d delta_in_secs] IP PORT configfile\n";
	exit 2;
} else {
	print STDERR "\n";
	print STDERR "IP address : $udpip\n";
	print STDERR "IP port    : $udpport\n";
	print STDERR "Delta(secs): $parameters{delta}\n";
}
#
# get data
#
readconfigfile($configfile);
#
# show new parameters, if any.
#
print STDERR "\n"; 
print STDERR "delta      = $parameters{delta}\n";
print STDERR "minmsgsize = $parameters{minmsgsize}\n";
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
#
exit 0;

# saving this.
# dumpOutgoingMsg($recvpaddr, $ackmsg);
# defined(send(SOCKET, $ackmsg, 0, $recvpaddr)) or die "send: $!";
