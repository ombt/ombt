#!/opt/exp/bin/perl -w
# test using select ...
#
my $rin = '';
my $rout = '';
#
vec($rin, fileno(STDIN), 1) = 1;
#
$done = 0;
$start_time = time();
$current_time = $start_time;
#
while ( ! $done) {
	$previous_time = $current_time;
	$nf = select($rout=$rin, undef, undef, 2);
	$current_time = time();
	if ($nf <= 0) {
		print "time out ...\n";
		$time_elapsed = $current_time-$previous_time;
		print "time elapsed = $time_elapsed ...\n";
		$total_time_elapsed = $current_time-$start_time;
		print "total time elapsed = $total_time_elapsed ...\n";
		next;
	} elsif (vec($rout, fileno(STDIN), 1)) {
		print "Got some input ...\n";
		$line = <STDIN>;
		if (defined($line)) {
			chomp($line);
			print "Line is <$line>\n";
			if ($line =~ /^[qQ]/) {
				print "Quitting ...\n";
				exit 0;
			}
		}
	} else {
		print "weird ... ???\n";
	}
}
#
exit 0;
