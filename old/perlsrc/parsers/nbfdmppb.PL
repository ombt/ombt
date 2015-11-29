#!/usr/bin/perl
# book feed dump playback ... shows current book as it
# reads the log file and updates ask/bid books.
#
use Getopt::Std;
#
sub usage {
	print <<EOF;

usage: bfdmppb [-?] [-s type] [-d] [-N] [-t] [-l maxlevel] feed_dump_file [...]

where:

	-d - print extra details (can be messy)
	-t - use last trades to update book.
	-l maxlevel - maximum levels to maintain.
	-D - tablulate some distributions
	-N - do calculations NOT so naively
	-s type - type of smoothing. type can have the following forms:
		exp:alpha=X:tick=Y - exponential moving average 
			with 0 < X < 1, and tick >= 0. tick = 0
			means there is NO rounding to the nearest tick
			multiple. tick > 0 means round to the nearest
			tick multiple.
		sma:n=X:tick=Y - simple moving average where n > 0 is the
			number of measurements to average, and tick is the
			same as above.
		wma:n=X:tick=Y - weighted moving average where n > 0 is the
			number of measurements to average, and tick is the
			same as above.
		cma:tick=Y - cumulative moving average (no parameters). 
			tick is the same as above.
		round:tick=Y - only round to nearest tick. tick is the same
			as above.

EOF
}
#
our $expectation;
#
our $smoothing;
our %smoothing_params;
our $rounding;
#
our %bid_book;
our %ask_book;
#
our %dists = ();
#
# pass in the random variable function and the distribution
# used in the expectation calculation.
#
sub naive_expectation {
	my ($pdist, $rv_func) = @_;
	#
	my $totaly = 0;
	my $totalrv = 0;
	#
	foreach my $x (sort { $a <=> $b } keys %{${pdist}}) {
		my $y = ${pdist}->{$x};
		$totaly += ${y};
		$totalrv += ${y}*(${rv_func}->($x));
	}
	#
	$totaly = 1 if ($totaly == 0);
	return $totalrv/$totaly;
}
#
# this will only matter if the range of values is *many* orders of
# magnitude. otherwise, it's alot of crunching to get the same answers
# as above.
#
sub not_so_naive_expectation {
	my ($pdist, $rv_func) = @_;
	#
	my $totaly = 0;
	my $totalrv = 0;
	#
	my @ltzeroy;
	my @gtzeroy;
	my @ltzerorv;
	my @gtzerorv;
	foreach my $x (keys %{${pdist}}) {
		my $y = ${pdist}->{$x};
		if ($y < 0.0) {
			unshift @ltzeroy, abs($y);
		} elsif ($y > 0.0) {
			unshift @gtzeroy, $y;
		}
		my $rv = ${y}*(${rv_func}->($x));
		if ($rv < 0.0) {
			unshift @ltzerorv, abs($rv);
		} elsif ($rv > 0.0) {
			unshift @gtzerorv, $rv;
		}
	}
	#
	my $total_ltzeroy = 0;
	my $total_gtzeroy = 0;
	foreach my $y (sort { $a <=> $b } @ltzeroy) {
		$total_ltzeroy += $y;
	}
	foreach my $y (sort { $a <=> $b } @gtzeroy) {
		$total_gtzeroy += $y;
	}
	$totaly = $total_gtzeroy - $total_ltzeroy;
	#
	my $total_ltzerorv = 0;
	my $total_gtzerorv = 0;
	foreach my $rv (sort { $a <=> $b } @ltzerorv) {
		$total_ltzerorv += $rv;
	}
	foreach my $rv (sort { $a <=> $b } @gtzerorv) {
		$total_gtzerorv += $rv;
	}
	$totalrv = $total_gtzerorv - $total_ltzerorv;
	#
	$totaly = 1 if ($totaly == 0);
	return $totalrv/$totaly;
}
#
sub round_to_nearest_tick {
	my ($x, $tick) = @_;
	if ($tick > 0) {
		return int(($x+($tick/2.0))/$tick)*$tick;
	} else {
		return $x;
	}
}
#
sub exponential_moving_average {
	my $x = shift;
	my $label = shift;
	$label .= ".ema";
	#
	my $alpha = $smoothing_params{alpha};
	my $tick = $smoothing_params{tick};
	#
	if (exists($smoothing_params{${label}}->{s_before})) {
		my $s_before = $smoothing_params{${label}}->{s_before};
		$smoothing_params{${label}}->{s} = 
			${s_before} + ${alpha}*($x-${s_before});
	} else {
		$smoothing_params{${label}}->{s} = $x;
	}
	$smoothing_params{${label}}->{s_before} = 
		$smoothing_params{${label}}->{s};
	#
	return &{${rounding}}($smoothing_params{${label}}->{s}, $tick);
}
#
sub weighted_moving_average {
	my $x = shift;
	my $label = shift;
	$label .= ".wma";
	#
	my $n = $smoothing_params{n};
	my $tick = $smoothing_params{tick};
	my $divisor = $smoothing_params{divisor};
	#
	if (exists($smoothing_params{${label}}->{npoints})) {
		my $start = $smoothing_params{${label}}->{start};
		my $npoints = $smoothing_params{${label}}->{npoints};
		if ($npoints >= $n) { 
			my $total_before = 
				$smoothing_params{${label}}->{total_before};
			my $total = $total_before + $x - 
				$smoothing_params{${label}}->{data}[$start];
			my $numerator_before = 
				$smoothing_params{${label}}->{numerator_before};
			my $numerator = 
				$numerator_before + $n*$x - $total_before;
			my $s = $numerator/$divisor;
			$smoothing_params{${label}}->{data}[$start] = $x;
			$start = ($start + 1)%$n;
			$smoothing_params{${label}}->{start} = $start;
			$smoothing_params{${label}}->{s} = $s;
			$smoothing_params{${label}}->{total_before} = $total;
			$smoothing_params{${label}}->{numerator_before} = 
				$numerator;
		} else {
			unshift @{$smoothing_params{${label}}->{data}}, $x;
			$npoints += 1;
			$smoothing_params{${label}}->{npoints} = $npoints;
			if ($npoints == $n) { 
				my $numerator = 0;
				my $total = 0;
				for (my $i=0; $i<$n; ${i}++) {
					my $si = ($start + $i)%$n;
					$total += $smoothing_params{${label}}->{data}[$si];
					$numerator += ($i+1)*$smoothing_params{${label}}->{data}[$si];
				}
				$smoothing_params{${label}}->{total_before} = 
					$total;
				$smoothing_params{${label}}->{numerator_before} = 
					$numerator;
				$smoothing_params{${label}}->{s} = 
					$numerator/$divisor;
			} else {
				$smoothing_params{${label}}->{s} = $x;
			}
		}
	} else {
		$smoothing_params{${label}}->{s} = $x;
		$smoothing_params{${label}}->{start} = 0;
		unshift @{$smoothing_params{${label}}->{data}}, $x;
		$smoothing_params{${label}}->{npoints} = 1;
	}
	#
	return &{${rounding}}($smoothing_params{${label}}->{s}, $tick);
}
#
sub simple_moving_average {
	my $x = shift;
	my $label = shift;
	$label .= ".sma";
	#
	my $n = $smoothing_params{n};
	my $tick = $smoothing_params{tick};
	#
	if (exists($smoothing_params{${label}}->{npoints})) {
		my $start = $smoothing_params{${label}}->{start};
		my $npoints = $smoothing_params{${label}}->{npoints};
		if ($npoints >= $n) { 
			my $s_before = $smoothing_params{${label}}->{s_before};
			my $s = $s_before +
				($x-$smoothing_params{${label}}->{data}[$start])/$n;
			$smoothing_params{${label}}->{data}[$start] = $x;
			$start = ($start + 1)%$n;
			$smoothing_params{${label}}->{start} = $start;
			$smoothing_params{${label}}->{s} = $s;
			$smoothing_params{${label}}->{s_before} = $s;
		} else {
			unshift @{$smoothing_params{${label}}->{data}}, $x;
			$npoints += 1;
			$smoothing_params{${label}}->{npoints} = $npoints;
			if ($npoints == $n) { 
				my $total = 0;
				for (my $i=0; $i<$n; ${i}++) {
					my $si = ($start + $i)%$n;
					$total += $smoothing_params{${label}}->{data}[$si];
				}
				$smoothing_params{${label}}->{s_before} = $total/$n;
				$smoothing_params{${label}}->{s} = $total/$n;
			} else {
				$smoothing_params{${label}}->{s} = $x;
				$smoothing_params{${label}}->{s_before} = $x;
			}
		}
	} else {
		$smoothing_params{${label}}->{s} = $x;
		$smoothing_params{${label}}->{s_before} = $x;
		$smoothing_params{${label}}->{start} = 0;
		unshift @{$smoothing_params{${label}}->{data}}, $x;
		$smoothing_params{${label}}->{npoints} = 1;
	}
	#
	return &{${rounding}}($smoothing_params{${label}}->{s}, $tick);
}
#
sub cumulative_moving_average {
	my $x = shift;
	my $label = shift;
	$label .= ".cma";
	#
	my $tick = $smoothing_params{tick};
	#
	if (exists($smoothing_params{${label}}->{s_before})) {
		$smoothing_params{${label}}->{n} += 1;
		$smoothing_params{${label}}->{s} = 
			$smoothing_params{${label}}->{s_before} +
			($x-$smoothing_params{${label}}->{s_before})/
			 $smoothing_params{${label}}->{n};
		$smoothing_params{${label}}->{s_before} =
			$smoothing_params{${label}}->{s};
	} else {
		$smoothing_params{${label}}->{s_before} = 0;
		$smoothing_params{${label}}->{s} = $x;
		$smoothing_params{${label}}->{n} = 1;
	}
	#
	return &{${rounding}}($smoothing_params{${label}}->{s}, $tick);
}
#
sub round_only {
	my $x = shift;
	my $label = shift;
	#
	return &{${rounding}}($x, $smoothing_params{tick});
}
#
sub stats {
	my ($distlabel, $pdist) = @_;
	#
	printf "\n";
	#
	my $mean = &{${expectation}}($pdist, sub { return shift; });
	printf "%s DISTRIBUTION MEAN: %f\n", $distlabel, $mean;
	#
	my $secondmoment = &{${expectation}}($pdist, sub { my $x = shift; return $x*$x; });
	printf "%s DISTRIBUTION SECOND MOMENT: %f\n", $distlabel, $secondmoment;
	#
	printf "%s DISTRIBUTION STANDARD DEVIATION: %f\n", $distlabel, sqrt(abs($mean*$mean-$secondmoment));
}
#
sub printdist {
	my ($xlabel, $xformat, $yformat, $pdist) = @_;
	#
	foreach my $x (sort { $a <=> $b } keys %{${pdist}}) {
		printf "${xlabel}(${xformat}) = ${yformat}\n", $x, $pdist->{$x};
	}
}
#
sub print_distributions {
	my $mean = 0.0;
	my $secondmoment = 0.0;
	#
	printf "\nBEST ASK DISTRIBUTION:\n";
	printdist("ASK", "%16.8f", "%6d", \%{$dists{best_ask_dist}});
	stats("BEST ASK", \%{$dists{best_ask_dist}});
	#
	printf "\nBID DISTRIBUTION:\n";
	printdist("BID", "%16.8f", "%6d", \%{$dists{best_bid_dist}});
	stats("BEST BID", \%{$dists{best_bid_dist}});
	#
	printf "\nSPREAD DISTRIBUTION:\n";
	printdist("SPREAD", "%16.8f", "%6d", \%{$dists{spread_dist}});
	stats("SPREAD", \%{$dists{spread_dist}});
	#
	printf "\nSMOOTHED SPREAD DISTRIBUTION:\n";
	printdist("SPREAD", "%16.8f", "%6d", \%{$dists{smoothed_spread_dist}});
	stats("SPREAD", \%{$dists{smoothed_spread_dist}});
	#
	printf "\nTOTAL BOUGHT PER PREVIOUS SPREAD DISTRIBUTION:\n";
	printdist("TOTAL BOUGHT PER SPREAD", "%16.8f", "%6d", \%{$dists{bought_per_prev_spread_dist}});
	stats("TOTAL BOUGHT PER PREVIOUS SPREAD", \%{$dists{bought_per_prev_spread_dist}});
	#
	printf "\nTOTAL BOUGHT PER CURRENT SPREAD DISTRIBUTION:\n";
	printdist("TOTAL BOUGHT PER SPREAD", "%16.8f", "%6d", \%{$dists{bought_per_current_spread_dist}});
	stats("TOTAL BOUGHT PER CURRENT SPREAD", \%{$dists{bought_per_current_spread_dist}});
	#
	printf "\nTOTAL BOUGHT PER DELTA SPREAD DISTRIBUTION:\n";
	printdist("TOTAL BOUGHT PER DELTA SPREAD", "%16.8f", "%6d", \%{$dists{bought_per_delta_spread_dist}});
	stats("TOTAL BOUGHT PER DELTA SPREAD", \%{$dists{bought_per_delta_spread_dist}});
	#
	printf "\nSMOOTHED TOTAL BOUGHT PER DELTA SPREAD DISTRIBUTION:\n";
	printdist("TOTAL BOUGHT PER DELTA SPREAD", "%16.8f", "%6d", \%{$dists{smoothed_bought_per_delta_spread_dist}});
	stats("TOTAL BOUGHT PER DELTA SPREAD", \%{$dists{smoothed_bought_per_delta_spread_dist}});
	#
	printf "\nTOTAL TRADES PER PREVIOUS SPREAD DISTRIBUTION:\n";
	printdist("TOTAL TRADES", "%16.8f", "%6d", \%{$dists{trades_per_prev_spread_dist}});
	stats("TOTAL TRADES PER PREVIOUS SPREAD", \%{$dists{trades_per_prev_spread_dist}});
	#
	printf "\nTOTAL TRADES PER CURRENT SPREAD DISTRIBUTION:\n";
	printdist("TOTAL TRADES", "%16.8f", "%6d", \%{$dists{trades_per_current_spread_dist}});
	stats("TOTAL TRADES PER CURRENT SPREAD", \%{$dists{trades_per_current_spread_dist}});
	#
	printf "\nLAST SIZE DISTRIBUTION:\n";
	printdist("LAST SIZE", "%6d", "%6d", \%{$dists{last_size_dist}});
	stats("LAST SIZE", \%{$dists{last_size_dist}});
	#
	printf "\nLAST PRICE DISTRIBUTION:\n";
	printdist("LAST PRICE", "%16.8f", "%6d", \%{$dists{last_price_dist}});
	stats("LAST PRICE", \%{$dists{last_price_dist}});
}
#
sub distribution {
	my ($pdist, $value, $qty) = @_;
	return if (!$opts{'D'});
	if (exists($pdist->{$value})) {
		$pdist->{$value} += $qty;
	} else {
		$pdist->{$value} = $qty;
	}
}
#
sub minmax {
	my ($pcurr_min, $pcurr_max, $maybe) = @_;
	return if (!defined($maybe) || $maybe == -1);
	if ($maybe < $$pcurr_min) {
		$$pcurr_min = $maybe;
	}
	if ($maybe > $$pcurr_max) {
		$$pcurr_max = $maybe;
	}
	return;
}
#
sub parsefile {
	my ($infd) = @_;
	#
	my $min_best_ask = 1000000000.0;
	my $max_best_ask = 0.0;
	my $min_best_bid = 1000000000.0;
	my $max_best_bid = 0.0;
	#
	my $prev_spread = -1.0;
	#
	for (my $recno=1; defined($rec = <$infd>); $recno+=1) {
		chomp($rec);
		next if ($rec =~ m/^\s*$/);
		printf "\n%04d:", $recno;
		printf " %s", $rec if ($opts{'d'});
		printf "\n";
		#
		my $received_time = "";
		my $msg_time = "";
		my $publish_time = "";
		my $orig_ask_price = "";
		my $orig_ask_size = "";
		my $orig_bid_price = "";
		my $orig_bid_size = "";
		my $orig2_ask_price = "";
		my $orig2_ask_size = "";
		my $orig2_bid_price = "";
		my $orig2_bid_size = "";
		my $orig3_ask_price = "";
		my $orig3_ask_size = "";
		my $orig3_bid_price = "";
		my $orig3_bid_size = "";
		my $ask_price = "";
		my $ask_size = "";
		my $bid_price = "";
		my $bid_size = "";
		my $last_price = "";
		my $last_size = "";
		my $bookdiff_bbook_snapshot = "";
		my $bbook_snapshot = "";
		my $bookdiff_abook_snapshot = "";
		my $abook_snapshot = "";
		my $update_last_trade_ok = 1;
		#
		# get times
		#
		if ($rec =~ m/received_time *= *[^\(]+\(([^\)]+)/) {
			$received_time = ${1};
			printf "RECEIVED TIME: %s\n", $received_time;
		}
		if ($rec =~ m/msg_time *= *[^\(]+\(([^\)]+)/) {
			$msg_time = ${1};
			printf "MSG TIME: %s\n", $msg_time;
		}
		if ($rec =~ m/publish_time *= *[^\(]+\(([^\)]+)/) {
			$publish_time = ${1};
			printf "PUBLISH TIME: %s\n", $publish_time;
		}
		#
		# get the orig data and update book when these data
		# are published.
		#
		if ($rec =~ m/orig_ask_price *= *[^\(]+\(([^\)]+)/) {
			$orig_ask_price = ${1};
			printf "ORIG ASK PRICE: %s\n", $orig_ask_price;
		}
		if ($rec =~ m/orig_ask_size *= *[^\(]+\(([^\)]+)/) {
			$orig_ask_size = ${1};
			printf "ORIG ASK SIZE : %s\n", $orig_ask_size;
		}
		if ($orig_ask_size ne "" && $orig_ask_price ne "") {
			$update_last_trade_ok = 0;
			$ask_book{$orig_ask_price} = $orig_ask_size;
			foreach my $price (keys %ask_book) {
				next unless ($price < $orig_ask_price);
				delete $ask_book{$price};
			}
		}
		if ($rec =~ m/orig_bid_price *= *[^\(]+\(([^\)]+)/) {
			$orig_bid_price = ${1};
			printf "ORIG BID PRICE: %s\n", $orig_bid_price;
		}
		if ($rec =~ m/orig_bid_size *= *[^\(]+\(([^\)]+)/) {
			$orig_bid_size = ${1};
			printf "ORIG BID SIZE : %s\n", $orig_bid_size;
		}
		if ($orig_bid_size ne "" && $orig_bid_price ne "") {
			$update_last_trade_ok = 0;
			$bid_book{$orig_bid_price} = $orig_bid_size;
			foreach my $price (keys %bid_book) {
				next unless ($price > $orig_bid_price);
				delete $bid_book{$price};
			}
		}
		#
		# get the orig2 or 3 data. the book is NOT updated when
		# these data are published.
		#
		if ($rec =~ m/orig2_ask_price *= *[^\(]+\(([^\)]+)/) {
			$orig2_ask_price = ${1};
			printf "ORIG2 ASK PRICE: %s\n", $orig2_ask_price;
		}
		if ($rec =~ m/orig2_ask_size *= *[^\(]+\(([^\)]+)/) {
			$orig2_ask_size = ${1};
			printf "ORIG2 ASK SIZE : %s\n", $orig2_ask_size;
		}
		if ($rec =~ m/orig2_bid_price *= *[^\(]+\(([^\)]+)/) {
			$orig2_bid_price = ${1};
			printf "ORIG2 BID PRICE: %s\n", $orig2_bid_price;
		}
		if ($rec =~ m/orig2_bid_size *= *[^\(]+\(([^\)]+)/) {
			$orig2_bid_size = ${1};
			printf "ORIG2 BID SIZE : %s\n", $orig2_bid_size;
		}
		if ($rec =~ m/orig3_ask_price *= *[^\(]+\(([^\)]+)/) {
			$orig3_ask_price = ${1};
			printf "ORIG3 ASK PRICE: %s\n", $orig3_ask_price;
		}
		if ($rec =~ m/orig3_ask_size *= *[^\(]+\(([^\)]+)/) {
			$orig3_ask_size = ${1};
			printf "ORIG3 ASK SIZE : %s\n", $orig3_ask_size;
		}
		if ($rec =~ m/orig3_bid_price *= *[^\(]+\(([^\)]+)/) {
			$orig3_bid_price = ${1};
			printf "ORIG3 BID PRICE: %s\n", $orig3_bid_price;
		}
		if ($rec =~ m/orig3_bid_size *= *[^\(]+\(([^\)]+)/) {
			$orig3_bid_size = ${1};
			printf "ORIG3 BID SIZE : %s\n", $orig3_bid_size;
		}
		#
		# get the ask data
		#
		if ($rec =~ m/ask_price *= *[^\(]+\(([^\)]+)/) {
			$ask_price = ${1};
			printf "ASK PRICE: %s", $ask_price;
			printf "\t<<<<==== SIDE REMOVED !!!" if (abs($ask_price) == 99999);
			printf "\n";
		}
		if ($rec =~ m/ask_size *= *[^\(]+\(([^\)]+)/) {
			$ask_size = ${1};
			printf "ASK SIZE : %s\n", $ask_size;
		}
		#
		# update ask book
		#
		if ($ask_size ne "" && $ask_price ne "") {
			$update_last_trade_ok = 0;
			$ask_book{$ask_price} = $ask_size;
			foreach my $price (keys %ask_book) {
				next unless ($price < $ask_price);
				delete $ask_book{$price};
			}
		}
		#
		# get bid data
		#
		if ($rec =~ m/bid_price *= *[^\(]+\(([^\)]+)/) {
			$bid_price = ${1};
			printf "BID PRICE: %s", $bid_price;
			printf "\t<<<<==== SIDE REMOVED !!!" if (abs($bid_price) == 99999);
			printf "\n";
		}
		if ($rec =~ m/bid_size *= *[^\(]+\(([^\)]+)/) {
			$bid_size = ${1};
			printf "BID SIZE : %s\n", $bid_size;
		}
		#
		# update bid book
		#
		if ($bid_size ne "" && $bid_price ne "") {
			$update_last_trade_ok = 0;
			$bid_book{$bid_price} = $bid_size;
			foreach my $price (keys %bid_book) {
				next unless ($price > $bid_price);
				delete $bid_book{$price};
			}
		}
		#
		# single book updates
		#
		if ($rec =~ m/bbook_upd *= *BookUpdate\(\(([-0-9\.]+), *([-0-9\.]+)\)\)/) {
			printf "SINGLE BID BOOK UPDATE: (PRICE,DELTA) = (%s,%s)\n", $1, $2;
			$bid_book{$1} += $2;
		}
		if ($rec =~ m/abook_upd *= *BookUpdate\(\(([-0-9\.]+), *([-0-9\.]+)\)\)/) {
			printf "SINGLE ASK BOOK UPDATE: (PRICE,DELTA) = (%s,%s)\n", $1, $2;
			$ask_book{$1} += $2;
		}
		#
		# get last trade data !!!
		#
		if ($rec =~ m/last_price *= *[^\(]+\(([^\)]+)/) {
			$last_price = ${1};
			printf "LAST PRICE: %s\n", $last_price;
		}
		if ($rec =~ m/last_size *= *[^\(]+\(([^\)]+)/) {
			$last_size = ${1};
			printf "LAST SIZE : %s\n", $last_size;
		}
		#
		# now read the book updates and process
		#
		if ($rec =~ m/book_diff *= *Sequence.*abook_snapshot *= *Array.{([^}]*)}/) {
			$update_last_trade_ok = 0;
			$bookdiff_abook_snapshot = ${1};
			printf "BOOK DIFF ASK BOOK SNAPSHOT: ";
			printf "%s\n", $bookdiff_abook_snapshot if ($opts{'d'});
			printf "\n";
			my @bookupdates = 
				split /BookUpdate/, $bookdiff_abook_snapshot;
			for (my $i=1; $i<scalar(@bookupdates); $i++) {
				my $bookupdate = $bookupdates[$i];
				chomp($bookupdate);
				# printf "BOOK UPDATE %02d: %s\n", $i, $bookupdate;
				$bookupdate =~ m/\(\(([-0-9\.]+),\s+([-0-9\.]+)\)\)/;
				printf "BOOK UPDATE %02d: (PRICE,DELTA) = (%s,%s)\n", $i, $1, $2;
				$ask_book{$1} += $2;
			}
		} elsif ($rec =~ m/abook_snapshot *= *Array.{([^}]*)}/) {
			$update_last_trade_ok = 0;
			$abook_snapshot = ${1};
			printf "ASK BOOK SNAPSHOT: ";
			printf "%s\n", $abook_snapshot if ($opts{'d'});
			printf "\n";
			my @bookupdates = split /BookUpdate/, $abook_snapshot;
			for (my $i=1; $i<scalar(@bookupdates); $i++) {
				my $bookupdate = $bookupdates[$i];
				chomp($bookupdate);
				# printf "BOOK UPDATE %02d: %s\n", $i, $bookupdate;
				$bookupdate =~ m/\(\(([-0-9\.]+),\s+([-0-9\.]+)\)\)/;
				printf "BOOK UPDATE %02d: (PRICE,QTY) = (%s,%s)\n", $i, $1, $2;
				$ask_book{$1} = $2;
			}
		}
		if ($rec =~ m/book_diff *= *Sequence.*bbook_snapshot *= *Array.{([^}]*)}/) {
			$update_last_trade_ok = 0;
			$bookdiff_bbook_snapshot = ${1};
			printf "BOOK DIFF BID BOOK SNAPSHOT: ";
			printf "%s\n", $bookdiff_bbook_snapshot if ($opts{'d'});
			printf "\n";
			my @bookupdates = 
				split /BookUpdate/, $bookdiff_bbook_snapshot;
			for (my $i=1; $i<scalar(@bookupdates); $i++) {
				my $bookupdate = $bookupdates[$i];
				chomp($bookupdate);
				# printf "BOOK UPDATE %02d: %s\n", $i, $bookupdate;
				$bookupdate =~ m/\(\(([-0-9\.]+),\s+([-0-9\.]+)\)\)/;
				printf "BOOK UPDATE %02d: (PRICE,DELTA) = (%s,%s)\n", $i, $1, $2;
				$bid_book{$1} += $2;
			}
		} elsif ($rec =~ m/bbook_snapshot *= *Array.{([^}]*)}/) {
			$update_last_trade_ok = 0;
			$bbook_snapshot = ${1};
			printf "BID BOOK SNAPSHOT: ";
			printf "%s\n", $bbook_snapshot if ($opts{'d'});
			printf "\n";
			my @bookupdates = split /BookUpdate/, $bbook_snapshot;
			for (my $i=1; $i<scalar(@bookupdates); $i++) {
				my $bookupdate = $bookupdates[$i];
				chomp($bookupdate);
				# printf "BOOK UPDATE %02d: %s\n", $i, $bookupdate;
				$bookupdate =~ m/\(\(([-0-9\.\.]+),\s+([-0-9\.\.]+)\)\)/;
				printf "BOOK UPDATE %02d: (PRICE,QTY) = (%s,%s)\n", $i, $1, $2;
				$bid_book{$1} = $2;
			}
		}
		#
		# update using last trade, but only if no 
		# other updates occurred.
		#
		if ($opts{'t'} && $update_last_trade_ok &&
		    $last_price ne "" && $last_size ne "") {
			# update books using last trade
			if (exists($bid_book{$last_price})) {
				$bid_book{$last_price} -= $last_size;
				printf "LAST TRADE UPDATE - BID(%s) = %s", $last_price, $bid_book{$last_price};
				if ($bid_book{$last_price} <= 0) {
					delete $bid_book{$last_price};
					printf " - DELETED";
				}
				printf "\n";
				foreach my $price (sort { $a <=> $b } keys %bid_book) {
					if ($price > $last_price) {
						printf "LAST TRADE UPDATE - DELETING LEVEL - BID(%s) = %s\n", $price, $bid_book{$price};
						delete $bid_book{$price};
					}
				}
			}
			if (exists($ask_book{$last_price})) {
				$ask_book{$last_price} -= $last_size;
				printf "LAST TRADE UPDATE - ASK(%s) = %s", $last_price, $ask_book{$last_price};
				if ($ask_book{$last_price} <= 0) {
					delete $ask_book{$last_price};
					printf " - DELETED";
				}
				printf "\n";
				foreach my $price (sort { $a <=> $b } keys %ask_book) {
					if ($price < $last_price) {
						printf "LAST TRADE UPDATE - DELETING LEVEL - ASK(%s) = %s\n", $price, $ask_book{$price};
						delete $ask_book{$price};
					}
				}
			}
		}
		#
		my $best_ask = -1;
		my $best_bid = -1;
		#
		my $maxi = -1;
		my @current_ask_book = ();
		print "\nCURRENT BOOK:\n";
		foreach my $price (reverse sort { $a <=> $b } keys %ask_book) {
			if ($ask_book{$price} <= 0) {
				delete $ask_book{$price};
				next;
			}
			$best_ask = $price;
			$current_ask_book[++$maxi] = { 
				price => $price, 
				qty => $ask_book{$price} 
			};
		}
		my $mini = scalar(@current_ask_book) - $maxlevels;
		$mini = 0 if ($mini < 0 || $opts{'d'});
		my $nl = "";
		for (my $i=$mini; $i<=$maxi; $i++) {
			printf "%s\t\t", $nl;
			$nl = "\n";
			printf "ASK(%16.8f) = %6d (%.2fx%d)", 
				$current_ask_book[$i]->{price},
				$current_ask_book[$i]->{qty},
				$current_ask_book[$i]->{price},
				$current_ask_book[$i]->{qty};
		}
		printf "\t<<<<==== BEST ASK\n" if ($mini<=$maxi);
		minmax(\$min_best_ask, \$max_best_ask, $best_ask);
		my $bid_level = 0;
		$nl = "\t<<<<==== BEST BID\n";
		foreach my $price (reverse sort { $a <=> $b } keys %bid_book) {
			if ($bid_book{$price} <= 0) {
				delete $bid_book{$price};
				next;
			}
			minmax(\$min_bid, \$max_bid, $price);
			$bid_level += 1;
			$best_bid = $price if ($best_bid == -1);
			if ($opts{'d'} || $bid_level <= $maxlevels) {
				printf "\t\t";
				printf "BID(%16.8f) = %6d (%.2fx%d)%s", 
					$price, $bid_book{$price},
					$price, $bid_book{$price}, $nl;
				$nl = "\n";
			}
		}
		printf "\n\t\tSPREAD: BEST ASK UNKNOWN\n" if ($best_ask == -1);
		printf "\n\t\tSPREAD: BEST BID UNKNOWN\n" if ($best_bid == -1);
		if ($best_ask != -1 && $best_bid != -1) {
			printf "\n";
			printf "\t\tBEST ASK: %16.8f\n", $best_ask;
			printf "\t\tBEST BID: %16.8f\n", $best_bid;
			printf "\t\tSPREAD  : %16.8f\n", $best_ask-$best_bid;
			minmax(\$min_best_bid, \$max_best_bid, $best_bid);
			distribution(\%{$dists{spread_dist}}, $best_ask-$best_bid, 1);
			distribution(\%{$dists{smoothed_spread_dist}}, 
				&{${smoothing}}($best_ask-$best_bid, "smoothed_spread"), 1);
			if ($last_price ne "" && $last_price ne "") {
				if ($prev_spread >= 0) { 
					distribution(\%{$dists{bought_per_prev_spread_dist}}, $prev_spread, $last_size);
					distribution(\%{$dists{trades_per_prev_spread_dist}}, $prev_spread, 1);
					distribution(\%{$dists{bought_per_delta_spread_dist}}, $prev_spread-($best_ask-$best_bid), $last_size);
					distribution(\%{$dists{smoothed_bought_per_delta_spread_dist}}, &{${smoothing}}($prev_spread-($best_ask-$best_bid), "smoothed_bought_per_delta_spread"), $last_size);
				}
				distribution(\%{$dists{bought_per_current_spread_dist}}, $best_ask-$best_bid, $last_size);
				distribution(\%{$dists{trades_per_current_spread_dist}}, $best_ask-$best_bid, 1);
				distribution(\%{$dists{last_size_dist}}, $last_size, 1);
				distribution(\%{$dists{last_price_dist}}, $last_price, 1);
			}
			$prev_spread = $best_ask - $best_bid;
		}
		printf "\n";
		if ($best_ask != -1) {
			printf "MIN BEST ASK: %16.8f\n", $min_best_ask;
			printf "MAX BEST ASK: %16.8f\n", $max_best_ask;
			distribution(\%{$dists{best_ask_dist}}, $best_ask, 1);
		}
		if ($best_bid != -1) {
			printf "MIN BEST BID: %16.8f\n", $min_best_bid;
			printf "MAX BEST BID: %16.8f\n", $max_best_bid;
			distribution(\%{$dists{best_bid_dist}}, $best_bid, 1);
		}
	}
}
#
our %opts;
if (getopts('?ds:tl:DN', \%opts) != 1) {
	usage();
	exit 2;
}
#
if ($opts{'?'}) {
	usage();
	exit 0;
}
#
if ($opts{'N'}) {
	$expectation = \&not_so_naive_expectation;
} else {
	$expectation = \&naive_expectation;
}
#
$smoothing_params{tick} = 0.0;
$rounding = sub { return shift; };
#
if (!$opts{'s'}) {
	$smoothing = sub { return shift; }
} elsif ($opts{'s'} =~ m/^exp:alpha=([\.0-9]+):tick=([\.0-9]+)/) {
	$smoothing = \&exponential_moving_average;
	$smoothing_params{alpha} = ${1};
	$smoothing_params{tick} = ${2};
} elsif ($opts{'s'} =~ m/^sma:n=([0-9]+):tick=([\.0-9]+)/) {
	$smoothing = \&simple_moving_average;
	$smoothing_params{n} = ${1};
	$smoothing_params{tick} = ${2};
} elsif ($opts{'s'} =~ m/^wma:n=([0-9]+):tick=([\.0-9]+)/) {
	$smoothing = \&weighted_moving_average;
	$smoothing_params{n} = ${1};
	$smoothing_params{divisor} = ${1}*(${1}+1)/2.0;
	$smoothing_params{tick} = ${2};
} elsif ($opts{'s'} =~ m/^cma:tick=([\.0-9]+)/) {
	$smoothing = \&cumulative_moving_average;
	$smoothing_params{tick} = ${1};
} elsif ($opts{'s'} =~ m/^round:tick=([\.0-9]+)/) {
	$smoothing = \&round_only;
	$smoothing_params{tick} = ${1};
} else {
	die "Unknown filtering option or wrong format: $opts{'s'}";
}
#
$rounding = \&round_to_nearest_tick if ($smoothing_params{tick} > 0);
#
our $maxlevels = $opts{'l'} ? $opts{'l'} : 1000;
printf "MAX LEVELS: %d\n", $maxlevels if ($opts{'d'});
#
if ($#ARGV < 0) {
	printf "\nReading: STDIN\n";
	parsefile(\*STDIN);
} else {
	foreach my $fixlogfile (@ARGV) {
		open(INFD, "<".$fixlogfile) or
			die "can't open $fixlogfile: $!";
		printf "\nReading: %s\n", $fixlogfile;
		parsefile(\*INFD);
		close(INFD);
		print_distributions;
	}
}
#
exit 0;

