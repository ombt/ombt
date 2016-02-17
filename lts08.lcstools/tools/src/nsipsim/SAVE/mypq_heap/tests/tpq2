#!/opt/exp/bin/perl -w
#
use lib ".";
#
use mypqueue;
use mytimer;
#
my $pq = undef;
#
$pq = "mypqueue"->new();
print "ref(pq) = ".ref($pq)."\n";
#
foreach $arg (@ARGV) {
	print "queueing $arg ...\n";
	my $ptimer = "mytimer"->new("hello", $arg, 2*$arg, "bye");
	$pq->enqueue($ptimer);
	print "hash index is ... ".$ptimer->heap()."\n";
	print "PQ AFTER INSERT OF ELEMENT ...\n";
	$pq->dump();
}
#
exit 0;
