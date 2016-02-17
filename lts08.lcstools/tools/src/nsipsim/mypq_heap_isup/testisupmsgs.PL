#!/opt/exp/bin/perl -w
#
use Carp;
use lib "$ENV{SIPSIMHOME}/bin";
use lib "$ENV{SIPSIMHOME}/lib";
#
use myisup;
#
sub printit {
	my ($msg) = @_;
	print STDERR ${msg};
}
#
my ($isupfile) = @ARGV;
if (!defined($isupfile)) {
	$isupfile = "isupfile";
}
#
my $isup = "myisup"->new(\&printit);
#
my $rawmsgs = { };
$isup->parseinfile($isupfile, $rawmsgs);
#
foreach my $key (sort keys %{$rawmsgs}) {
	my $protocol = $key;
	printit "\n\nDECODING PROTOCOL: $protocol\n";
	foreach my $msgid (sort keys %{$rawmsgs->{$protocol}}) {
		printit "\nDECODING MSGID $msgid: \n";
		$isup->decode($protocol, $rawmsgs->{$protocol}{$msgid});
	}
}
#
exit 0;
