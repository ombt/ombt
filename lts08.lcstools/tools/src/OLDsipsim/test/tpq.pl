#!/opt/exp/bin/perl -w
# test using select ...
push @INC, ".";
push @INC, "/home/lcstools/tools/src/sipsim";
#
use mypqueue;
#
my $pq = undef;
#
$pq = mypqueue::makepqueue();
#
foreach $arg (@ARGV) {
	print "queueing $arg ...\n";
	mypqueue::enqueue($pq, $arg, 2*$arg)
}
#
while (!mypqueue::isEmpty($pq)) {
	my $key;
	my $value;
	#
	mypqueue::dequeue($pq, \$key, \$value);
	#
	print "dequeue (key,value)=($key,$value)\n";
}
#
exit 0;
