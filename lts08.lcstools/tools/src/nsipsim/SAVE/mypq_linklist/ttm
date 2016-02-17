#!/opt/exp/bin/perl -w
#
use lib ".";
#
use mytimer;
#
my $ptm = undef;
#
# my $pt1 = undef;
# my $pt2 = undef;
# my $arg1 = undef;
# my $arg2 = undef;
# my $arg3 = undef;
# my $arg4 = undef;
#
for ( ; @{ARGV} ; ) {
	my $arg1 = shift @ARGV;
	my $arg2 = shift @ARGV;
	my $arg3 = shift @ARGV;
	my $arg4 = shift @ARGV;
	#
	my $pt1 = "mytimer"->new("first", $arg1, $arg2, "first");
	$pt1->dump();
	my $pt2 = "mytimer"->new("second", $arg3, $arg4, "second");
	$pt2->dump();
	#
	if ($pt1->cmp($pt2) < 0) {
		print "pt1 < pt2\n";
	} elsif ($pt1->cmp($pt2) > 0) {
		print "pt1 > pt2\n";
	} elsif ($pt1->cmp($pt2) == 0) {
		print "pt1 == pt2\n";
	} else {
		print "HUH ???\n";
	}
	if ($pt2->cmp($pt1) < 0) {
		print "pt2 < pt1\n";
	} elsif ($pt2->cmp($pt1) > 0) {
		print "pt2 > pt1\n";
	} elsif ($pt2->cmp($pt1) == 0) {
		print "pt2 == pt1\n";
	} else {
		print "HUH ???\n";
	}
}
#
exit 0;
