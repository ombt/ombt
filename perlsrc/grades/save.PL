#!/usr/bin/perl -w
#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
use strict;
#
use FileHandle;
use File::Basename;
use Getopt::Std;
use Cwd;
#
my $cmd = $0;
#
my $delimiter = ",";
my $verbose = "no";
my $promotion_scale = 1;
my $fields_expected = 4;
#
my %honors_grade_promotion = (
	0 => {
		example => "Example: Honor's B = Regular B",
		"A+" => "A+",
		"A"  => "A",
		"A-" => "A-",
		"B+" => "B+",
		"B"  => "B",
		"B-" => "B-",
		"C+" => "C+",
		"C"  => "C",
		"C-" => "C-",
		"D+" => "D+",
		"D"  => "D",
		"D-" => "D-",
		"F+" => "F+",
		"F"  => "F",
		"F-" => "F-",
	},
	1 => {
		example => "Example: Honor's B = Regular B+",
		"A+" => "A+",
		"A"  => "A+",
		"A-" => "A",
		"B+" => "A-",
		"B"  => "B+",
		"B-" => "B",
		"C+" => "B-",
		"C"  => "C+",
		"C-" => "C",
		"D+" => "C-",
		"D"  => "D+",
		"D-" => "D",
		"F+" => "D-",
		"F"  => "F+",
		"F-" => "F",
	},
	2 => {
		example => "Example: Honor's B = Regular A-",
		"A+" => "A+",
		"A"  => "A+",
		"A-" => "A+",
		"B+" => "A",
		"B"  => "A-",
		"B-" => "B+",
		"C+" => "B",
		"C"  => "B-",
		"C-" => "C+",
		"D+" => "C",
		"D"  => "C-",
		"D-" => "D+",
		"F+" => "D",
		"F"  => "D-",
		"F-" => "F+",
	},
	3 => {
		example => "Example: Honor's B = Regular A",
		"A+" => "A+",
		"A"  => "A+",
		"A-" => "A+",
		"B+" => "A+",
		"B"  => "A",
		"B-" => "A-",
		"C+" => "B+",
		"C"  => "B",
		"C-" => "B-",
		"D+" => "C+",
		"D"  => "C",
		"D-" => "C-",
		"F+" => "D+",
		"F"  => "D",
		"F-" => "D-",
	},
);
#
my %pay_scale_per_grade = (
	regular => {
		"A+" => 25,
		"A"  => 25,
		"A-" => 25,
		"B+" => 0,
		"B"  => 0,
		"B-" => 0,
		"C+" => 0,
		"C"  => 0,
		"C-" => 0,
		"D+" => 0,
		"D"  => 0,
		"D-" => 0,
		"F+" => 0,
		"F"  => 0,
		"F-" => 0,
	},
	honors => {
		"A+" => 30,
		"A"  => 30,
		"A-" => 30,
		"B+" => 20,
		"B"  => 20,
		"B-" => 20,
		"C+" => 0,
		"C"  => 0,
		"C-" => 0,
		"D+" => 0,
		"D"  => 0,
		"D-" => 0,
		"F+" => 0,
		"F"  => 0,
		"F-" => 0,
	},
);
#
my %pay_scale_all_grades = (
	"A+" => 1500,
	"A"  => 150,
	"A-" => 100,
	"B+" => 0,
	"B"  => 0,
	"B-" => 0,
	"C+" => 0,
	"C"  => 0,
	"C-" => 0,
	"D+" => 0,
	"D"  => 0,
	"D-" => 0,
	"F+" => 0,
	"F"  => 0,
	"F-" => 0,
);
#
sub usage {
	my ($arg0) = @_;
	print <<EOF;

usage: $arg0 [-?] [-v] [-d delimiter] [-p 0|1|2|3] grade_file

where:
	-? - print usage.
	-v - verbose
	-p 0|1|2|3 - how many grades to promote Honor's class grades.
		     for example, consider an Honor's B:

	                 0 means Honor's B = regular B
	                 1 means Honor's B = regular B+
	                 2 means Honor's B = regular A-
	                 3 means Honor's B = regular A

EOF
}
#
sub get_grades {
	my ($pgrfile, $pgr) = @_;
	#
	print "\nReading File: $pgrfile\n";
	#
	open(FH, $pgrfile) or die $!;
	my @rows = <FH>;
	close(FH);
	#
	chomp(@rows);
	#
	foreach my $row (@rows)
	{
		my $ROW = "\U$row";
		next if ($ROW =~ /#/); # skip comments
		#
		my @values = split /$delimiter/, $ROW;
		my $fields_found = scalar @values;
		if ($fields_found != $fields_expected) {
			print "ERROR: wrong number of fields: $fields_found\n";
			next;
		}
		#
		my $class = "$values[0]";
		my $quarter = "$values[1]";
		my $honors = "$values[2]";
		my $grade = "$values[3]";
		my $promoted_grade = "$values[3]";
		#
		next if ($class eq "CLASS");
		print "ROW: ${ROW}\n" if ($verbose eq "yes");
		#
		if ($honors eq "YES") {
			if (exists $honors_grade_promotion{$promotion_scale}->{$grade}) {
				$promoted_grade = $honors_grade_promotion{$promotion_scale}->{$grade};
			} else {
				print "ERROR: unknown honors grade: ${grade} \n";
				next;
			}
		}
		#
		push @{$pgr}, {
			class => "${class}",
			quarter => "${quarter}",
			honors => "${honors}",
			grade => "${grade}",
			promoted_grade => "${promoted_grade}",
		};
	}
}
#
sub get_payment {
	my ($pgr, $ppay) = @_;
	#
	print "\nPayment Per Grade Per Quarter:\n";
	#
	my $total_payment_per_grade_per_quarter = 0;
	my $total_payment_all_grades_per_quarter = 0;
	#
	my %lowest_grade = (
		"1" => "%%",
		"2" => "%%",
		"3" => "%%",
		"4" => "%%",
		"YR" => "%%",
	);
	#
	foreach my $pclass (@{$pgr}) {
		my $class = $pclass->{class};
		my $quarter = $pclass->{quarter};
		my $grade = $pclass->{grade};
		my $promoted_grade = $pclass->{promoted_grade};
		my $payment = 0;
		#
		if ($quarter !~ /^[1234]$/) {
			# print "SKIP: Class: $class, Grade: $grade, Promoted Grade: $promoted_grade, Quarter: $quarter\n";
			next;
		}
		#
		if (exists($pay_scale_per_grade{$promoted_grade})) {
			$payment = $pay_scale_per_grade{$promoted_grade};
		} else {
			print "ERROR: unknown grade ${grade} for $class in qaurter $quarter.\n";
			next;
		}
		print "Class: $class, Grade: $grade, Promoted_Grade: $promoted_grade, Quarter: $quarter, Payment: $payment\n";
		$total_payment_per_grade_per_quarter += $payment;
		#
		my $current_lowest_grade = $lowest_grade{$quarter};
		#
		my $pg_just_grade = substr $promoted_grade, 0, 1;
		my $clg_just_grade = substr $current_lowest_grade, 0, 1;
		#
		if ($clg_just_grade lt $pg_just_grade) {
			$lowest_grade{$quarter} = $promoted_grade;
		} elsif ($clg_just_grade eq $pg_just_grade) {
			my $len_pg  = length($promoted_grade);
			my $len_clg = length($current_lowest_grade);
			#
			my $sign_pg  = ",";
			my $sign_clg = ",";
			#
			if ($len_pg == 2) {
				$sign_pg = substr $promoted_grade, 1, 1;
			}
			if ($len_clg == 2) {
				$sign_clg = substr $current_lowest_grade, 1, 1;
			}
			#
			if ($sign_clg lt $sign_pg) {
				$lowest_grade{$quarter} = $promoted_grade;
			}
		}
		$current_lowest_grade = $lowest_grade{$quarter};
	}
	#
	print "\nTotal Payment Per Grade Per Quarter = $total_payment_per_grade_per_quarter\n";
	$ppay->{Total_Payment_Per_Grade_Per_Quarter} = $total_payment_per_grade_per_quarter;
	#
	print "\nLowest Grade Per Quarter:\n";
	foreach my $quarter (sort keys %lowest_grade) {
		next if ($quarter !~ /^[1234]$/);
		my $quarter_lowest_grade = $lowest_grade{$quarter};
		my $quarter_payment = $pay_scale_all_grades{$quarter_lowest_grade};
		print "Quarter: ${quarter}, Lowest Grade: $quarter_lowest_grade, Payment: $quarter_payment\n";
		$total_payment_all_grades_per_quarter = $quarter_payment;
	}
	#
	print "\nTotal Payment All Grades Per Quarter = $total_payment_all_grades_per_quarter\n";
	$ppay->{Total_Payment_All_Grades_Per_Quarter} = $total_payment_all_grades_per_quarter;
}
#
sub print_summary {
	my ($ppay) = @_;
	#
	print "\n<<<< SUMMARY >>>>\n";
	#
	my $total_payment_per_grade_per_quarter = $ppay->{Total_Payment_Per_Grade_Per_Quarter};
	print "Total Payment Per Grade Per Quarter = $total_payment_per_grade_per_quarter\n";
	#
	my $total_payment_all_grades_per_quarter = $ppay->{Total_Payment_All_Grades_Per_Quarter};
	print "Total Payment All Grades Per Quarter = $total_payment_all_grades_per_quarter\n";
	#
	my $grand_total = $total_payment_per_grade_per_quarter +
	                  $total_payment_all_grades_per_quarter;
	#
	print "\n\nGRAND TOTAL PAYMENT: $grand_total\n";
}
#
my %opts;
if (getopts('?vd:p:', \%opts) != 1) {
	usage($cmd);
	exit 2;
}
#
foreach my $opt (%opts) {
	if ($opt eq "?") {
		usage($cmd);
		exit 0;
	} elsif ($opt eq "v") {
		$verbose = "yes";
	} elsif ($opt eq "d") {
		$delimiter = $opts{$opt}
	} elsif ($opt eq "p") {
		$promotion_scale = $opts{$opt};
		die "invalid promotion option. use 0, 1, 2 or 3." unless ($promotion_scale =~ /^[0-3]$/);
	}
}
#
die "grade file not given" unless ($#ARGV >= 0);
my $gradefile = shift @ARGV;
#
print "\nHonor's Grades Promoted ${promotion_scale} Grades.\n";
print "\n$honors_grade_promotion{$promotion_scale}->{example}\n";
#
my @grades = ();
get_grades($gradefile, \@grades);
#
my %payment = ();
get_payment(\@grades, \%payment);
#
print_summary(\%payment);
#
exit 0;
