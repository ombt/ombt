#!/usr/bin/perl
# book feed dump playback ... shows current book as it
# reads the log file and updates ask/bid books.
#
use Getopt::Std;
#
sub usage {
	print <<EOF;

usage: bfdmppb [-?] [-d] [-t] [-l maxlevel] feed_dump_file [...]

where:

	-d - print extra details (can be messy)
	-t - use last trades to update book.
	-l maxlevel - maximum levels to maintain.

EOF
}
#
my %bid_book;
my %ask_book;
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
			foreach my $price (sort keys %ask_book) {
				if ($price < $orig_ask_price) {
					delete $ask_book{$price};
					next;
				}
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
			foreach my $price (sort keys %bid_book) {
				if ($price > $orig_bid_price) {
					delete $bid_book{$price};
					next;
				}
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
			foreach my $price (sort keys %ask_book) {
				if ($price < $ask_price) {
					delete $ask_book{$price};
					next;
				}
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
			foreach my $price (sort keys %bid_book) {
				if ($price > $bid_price) {
					delete $bid_book{$price};
					next;
				}
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
				foreach my $price (sort keys %bid_book) {
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
				foreach my $price (sort keys %ask_book) {
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
		foreach my $price (reverse sort keys %ask_book) {
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
			printf "ASK(%8.2f) = %6d (%.2fx%d)", 
				$current_ask_book[$i]->{price},
				$current_ask_book[$i]->{qty},
				$current_ask_book[$i]->{price},
				$current_ask_book[$i]->{qty};
		}
		printf "\t<<<<==== BEST ASK\n" if ($mini<=$maxi);
		minmax(\$min_best_ask, \$max_best_ask, $best_ask);
		my $bid_level = 0;
		$nl = "\t<<<<==== BEST BID\n";
		foreach my $price (reverse sort keys %bid_book) {
			if ($bid_book{$price} <= 0) {
				delete $bid_book{$price};
				next;
			}
			minmax(\$min_bid, \$max_bid, $price);
			$bid_level += 1;
			$best_bid = $price if ($best_bid == -1);
			if ($opts{'d'} || $bid_level <= $maxlevels) {
				printf "\t\t";
				printf "BID(%8.2f) = %6d (%.2fx%d)%s", 
					$price, $bid_book{$price},
					$price, $bid_book{$price}, $nl;
				$nl = "\n";
			}
		}
		printf "\n\t\tSPREAD: BEST ASK UNKNOWN\n" if ($best_ask == -1);
		printf "\n\t\tSPREAD: BEST BID UNKNOWN\n" if ($best_bid == -1);
		if ($best_ask != -1 && $best_bid != -1) {
			printf "\n";
			printf "\t\tBEST ASK: %8.2f\n", $best_ask;
			printf "\t\tBEST BID: %8.2f\n", $best_bid;
			printf "\t\tSPREAD  : %8.2f\n", $best_ask-$best_bid;
			minmax(\$min_best_bid, \$max_best_bid, $best_bid);
		}
		printf "\n";
		if ($best_ask != -1) {
			printf "MIN BEST ASK: %8.2f\n", $min_best_ask;
			printf "MAX BEST ASK: %8.2f\n", $max_best_ask;
		}
		if ($best_bid != -1) {
			printf "MIN BEST BID: %8.2f\n", $min_best_bid;
			printf "MAX BEST BID: %8.2f\n", $max_best_bid;
		}
	}
}
#
our %opts;
if (getopts('?dtl:', \%opts) != 1) {
	usage();
	exit 2;
}
#
if ($opts{'?'}) {
	usage();
	exit 0;
}
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
	}
}
#
exit 0;

