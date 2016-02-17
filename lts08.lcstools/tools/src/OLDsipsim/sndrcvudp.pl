#!/opt/exp/bin/perl -w
#
# send a UDP msg and wait for a response.
#
########################################################################
#
# uses these modules
#
use Getopt::Std;
use IO::Socket;
use Fcntl;
#
# functions
#
sub waitformsg {
	my $rin = '';
	my $rout = '';
	#
	vec($rin, fileno(MYSOCKET), 1) = 1;
	#
	my $nf = select($rout=$rin, undef, undef, $maxtime);
	if ($nf <= 0) {
		print "Time out. No msg available at MYSOCKET.\n";
	} elsif (vec($rout, fileno(MYSOCKET), 1)) {
		my $recvpaddr = recv(MYSOCKET, my $msg, 8*1024, 0);
		if (defined($recvpaddr)) {
			print "Msg available at MYSOCKET.\n";
			my ($recvport, $recvipaddr) = sockaddr_in($recvpaddr);
			$recvipaddr = inet_ntoa($recvipaddr);     
			my $recvaddr = "$recvipaddr:$recvport";   
			#                                         
			print "IP/PORT: $recvaddr\n";             
			print "incoming msg <<<<<<<<<<<<<<<<<<\n";
			my @newmsg = split /[\r\n]+/, $msg;       
			foreach $line (@newmsg) {                 
				print("line=<$line>\n");          
			}                                         
			print "incoming msg <<<<<<<<<<<<<<<<<<\n";
		} else {
			print "No msg available at MYSOCKET.\n";
		}
	} else {
		print "weird ... ???\n";
	}
}
#
# get IP address and port from cmd line.
#
print "Send and Receive UDP Msg:\n";
#
$mflag = 0;
$maxtime = 10;
$delimiter = "\n";;
#
my %opts;
getopts('md:t:', \%opts);
foreach my $opt (%opts) {
	if ($opt eq "m") {
		$mflag = 1;
		print "==>> merging all strings into one msg.\n";
	} elsif ($opt eq "t") {
		$maxtime = $opts{$opt};
		print "==>> maxtime = $maxtime\n";
	} elsif ($opt eq "d") {
		$delimiter = eval "\"$opts{$opt}\"";
		print "==>> delimiter = \"$delimiter\"\n";
	}
}
#
(my $myip, $myport, $remoteip, my $remoteport, my @remotemsgs) = @ARGV;
if (!defined($remoteip) || !defined($remoteport) || 
    !defined($myip) || !defined($myport)) {
	print STDERR "usage: sndrcvudp [-m] [-t maxtime] MYIP MYPORT REMOTEIP REMOTEPORT string ... \n";
	exit 2;
} else {
	print "My IP addr    : $myip\n";
	print "My IP port    : $myport\n";
	print "Remote IP addr: $remoteip\n";
	print "Remote IP port: $remoteport\n";
}
#
print "Creating Remote UDP Socket ...\n";
my $remoteipaddr = gethostbyname($remoteip);
defined($remoteipaddr) or die "gethostbyname: $!";
my $remotepaddr = sockaddr_in($remoteport, $remoteipaddr);
defined($remotepaddr) or die "getprotobyname: $!";
#
print "Creating My UDP Socket ...\n";
my $myproto = getprotobyname('udp');
defined($myproto) or die "getprotobyname: $!";
socket(MYSOCKET, PF_INET, SOCK_DGRAM, $myproto) or die "socket: $!";
my $myipaddr = gethostbyname($myip);
defined($myipaddr) or die "gethostbyname: $!";
my $mypaddr = sockaddr_in($myport, $myipaddr);
defined($mypaddr) or die "getprotobyname: $!";
bind(MYSOCKET, $mypaddr) or die "bind: $!";
fcntl(MYSOCKET, F_SETFL, O_NONBLOCK);
#
print "Sending msg ...\n";
if ($mflag) {
	$remotemsg = join($delimiter, @remotemsgs);
	$remotemsg =~ s/\\r/\r/g;
	$remotemsg =~ s/\\n/\n/g;
	print "Msg to send: <$remotemsg>\n";
	defined(send(MYSOCKET, $remotemsg, 0, $remotepaddr)) or die "send: $!";
	#
	waitformsg();
} else {
	foreach $remotemsg (@remotemsgs) {
		print "Msg to send: <$remotemsg>\n";
		defined(send(MYSOCKET, $remotemsg, 0, $remotepaddr)) or 
			die "send: $!";
		#
		waitformsg();
	}
}
#
close(MYSOCKET);
#
exit 0;


