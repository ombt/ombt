#!/opt/exp/bin/perl -w
#
use Socket;
#
(my $machine, my @dummy) = @ARGV;
#
if (defined($machine)) {
	print "machine is: $machine\n";
	if (defined(my $ipaddr = gethostbyname($machine))) {
		print "translated IP addr = $ipaddr\n";
	} else {
		print "IP addr = $ipaddr\n";
	}
} else {
	print "machine is undefined.\n";
}
#
exit 0;
