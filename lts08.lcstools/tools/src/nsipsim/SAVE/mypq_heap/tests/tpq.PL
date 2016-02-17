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
	print "ref(ptimer) = ".ref($ptimer)."\n";
	$pq->enqueue($ptimer);
}
print "\n\n\n BEFORE DELETE pq is: \n";
$pq->dump();
print "\n\n\n BEFORE COUNT pq is: ".$pq->count()."\n";
#
while (!$pq->isEmpty()) {
	my $timer;
	$pq->dequeue(\$timer);
	if (defined($timer)) {
		$timer->dump();
	} else {
		print "timer is UNDEFINED.\n";
	}
	$timer = undef;
}
print "\n\nAFTER DELETE npq is: \n";
$pq->dump();
print "\n\n\n AFTER COUNT pq is: ".$pq->count()."\n";
#
exit 0;
