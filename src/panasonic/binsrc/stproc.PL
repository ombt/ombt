#!/usr/bin/perl

use Getopt::Std;
#
sub usage {
	print <<EOF;

usage: storedproc [-?] [store_procedure.sql [store_procedure2.sql ...]]

where:
	-? - print usage message

if no *.sql file is given, the stdin is read.

EOF
}
#
sub parserecord {
	my ($recno, $rec) = @_;
	#
	printf "%04d:", $recno;
	printf " %s", $rec;
	printf "\n";
	#
	return(0);
}
#
sub parsefile {
	my ($infd) = @_;
	# remove any comments.
	for (my $recno=1; defined($rec = <$infd>); $recno+=1) {
		chomp($rec);
		next if ($rec =~ m/^\s*$/);
		next if ($rec =~ m/^\s*--/);
		die "parsing failed: $!" unless (parserecord($recno, $rec) == 0);
	}
}
#
our %opts;
if (getopts('?', \%opts) != 1) {
	usage();
	exit 2;
}
#
if ($opts{'?'}) {
	usage();
	exit 0;
}
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
