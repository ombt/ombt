#!/opt/exp/bin/perl -w
#
# print ISUP msgs structure.
#
use Carp;
use lib "$ENV{SIPSIMHOME}/bin";
use lib "$ENV{SIPSIMHOME}/lib";
#
use myisup;
#
sub printit {
	my ($msg) = @_;
	confess "printit: $!" unless (defined($msg));
	print STDOUT ${msg};
}
#
my $isup = "myisup"->new(\&printit);
# plain ansi
$isup->printmsgs("ansi");
# att ansi
$isup->printmsgs("ansi00");
# itu ansi
$isup->printmsgs("itu");
#
exit 0;
