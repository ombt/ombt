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
my $number_to_choose = 5;
#
sub usage {
	my ($arg0) = @_;
	print <<EOF;

usage: $arg0 [-?] [-n number_to_choose] file_with_numbers

where:
	-? - print usage.
	-n number - number of sets to choose

and file_with_number contains the date, and numbers chosen with
one data per row.

EOF
}
#
sub get_numbers {
	my ($numfile, $pdata) = @_;
	#
	open(FH, $numfile) or die $!;
	my @rows = <FH>;
	close(FH);
	#
	chomp(@rows);
	#
	foreach my $row (@rows)
	{
		print "ROW: ${row}\n";
		#
		my @values = split /,/, $row;
		#
		my $date = sprintf("%04d%02d%02d", 
				  (split /\//, $values[0], 3)[2,0,1]);
		#
		my $num1 = $values[1];
		my $num2 = $values[2];
		my $num3 = $values[3];
		my $num4 = $values[4];
		my $num5 = $values[5];
		#
		my $mega = (split /=/, $values[6], 2)[1];
		#
		$pdata->{$date} = {
			num1 => "$num1",
			num2 => "$num2",
			num3 => "$num3",
			num4 => "$num4",
			num5 => "$num5",
			mega => "$mega",
		};
	}
}
#
sub get_frequencies {
	my ($pdata, $preg, $pmega, $pmaxreg, $pmaxmega) = @_;
	#
	for (my $i=0; $i<=56; $i += 1)
	{
		$preg->[$i] = 0;
	}
	#
	for (my $i=0; $i<=46; $i += 1)
	{
		$pmega->[$i] = 0;
	}
	#
	$$pmaxreg = 0;
	$$pmaxmega = 0;
	#
	print "\n";
	foreach my $date (reverse sort keys %{${pdata}})
	{
		print "DATE: ${date}\n";
		$preg->[${pdata}->{$date}->{num1}] += 1;
		if ($preg->[${pdata}->{$date}->{num1}] > $$pmaxreg)
		{
			$$pmaxreg = $preg->[${pdata}->{$date}->{num1}];
		}
		$preg->[${pdata}->{$date}->{num2}] += 1;
		if ($preg->[${pdata}->{$date}->{num2}] > $$pmaxreg)
		{
			$$pmaxreg = $preg->[${pdata}->{$date}->{num2}];
		}
		$preg->[${pdata}->{$date}->{num3}] += 1;
		if ($preg->[${pdata}->{$date}->{num3}] > $$pmaxreg) 
		{
			$$pmaxreg = $preg->[${pdata}->{$date}->{num3}];
		}
		$preg->[${pdata}->{$date}->{num4}] += 1;
		if ($preg->[${pdata}->{$date}->{num4}] > $$pmaxreg) 
		{
			$$pmaxreg = $preg->[${pdata}->{$date}->{num4}];
		}
		$preg->[${pdata}->{$date}->{num5}] += 1;
		if ($preg->[${pdata}->{$date}->{num5}] > $$pmaxreg) 
		{
			$$pmaxreg = $preg->[${pdata}->{$date}->{num5}];
		}
		$pmega->[${pdata}->{$date}->{mega}] += 1;
		if ($pmega->[${pdata}->{$date}->{mega}] > $$pmaxmega) 
		{
			$$pmaxmega = $pmega->[${pdata}->{$date}->{mega}];
		}
	}
}
sub graph_frequencies {
	my ($preg, $pmega, $maxreg, $maxmega) = @_;
	#
	print "\nRegular: 1-56 (max=${maxreg})\n";
	for (my $i=1; $i<=56; $i += 1)
	{
		printf("%02d: %s\n", $i, "*" x $preg->[$i]);
	}
	#
	print "\nMega: 1-46 (max=${maxmega})\n";
	for (my $i=1; $i<=46; $i += 1)
	{
		printf("%02d: %s\n", $i, "*" x $pmega->[$i]);
	}
	#
	
}
#
sub sort_frequencies {
	my ($preg, $pmega, $psorted_reg, $psorted_mega) = @_;
	#
	for (my $i=1; $i<=56; $i += 1)
	{
		push @{$psorted_reg->{$preg->[$i]}}, $i;
	}
	#
	for (my $i=1; $i<=46; $i += 1)
	{
		push @{$psorted_mega->{$pmega->[$i]}}, $i;
	}
	#
}
#
sub graph_sort_frequencies {
	my ($psorted_reg, $psorted_mega, $maxreg, $maxmega) = @_;
	#
	print "\nRegular: 1-56 (max=${maxreg})\n";
	foreach my $i (reverse sort keys %{${psorted_reg}})
	{
		printf("%02d: ", $i);
		for (my $j=0; $j<$#{$psorted_reg->{$i}}; $j += 1)
		{
			printf("%d ", ${$psorted_reg->{$i}}[$j]);
		}
		printf("\n");
	}
	#
	print "\nMega: 1-46 (max=${maxmega})\n";
	foreach my $i (reverse sort keys %{${psorted_mega}})
	{
		printf("%02d: ", $i);
		for (my $j=0; $j<$#{$psorted_mega->{$i}}; $j += 1)
		{
			printf("%d ", ${$psorted_mega->{$i}}[$j]);
		}
		printf("\n");
	}
}
#
sub empirical_dist {
	my ($pemp_freq, $pemp_dist) = @_;
	#
	my $maxi = @{$pemp_freq};
	#
	for (my $i=0; $i<$maxi; $i += 1)
	{
		my $jmax = $pemp_freq->[$i];
		next unless ($jmax > 0);
		for (my $j=1; $j<=$jmax; $j += 1)
		{
			push @{$pemp_dist}, $i;
		}
	}
	
}
#
sub non_zero_empirical_dist {
	my ($pemp_freq, $pnz_emp_dist) = @_;
	#
	my $maxi = @{$pemp_freq};
	#
	for (my $i=0; $i<$maxi; $i += 1)
	{
		my $jmax = $pemp_freq->[$i];
		$jmax += 1;
		for (my $j=1; $j<=$jmax; $j += 1)
		{
			push @{$pnz_emp_dist}, $i;
		}
	}
}
#
sub choose_from_dist {
	my ($label, $pemp_dist_reg, $pemp_dist_mega, $number_to_choose) = @_;
	#
	my $maxregidx = @{$pemp_dist_reg};
	my $maxmegaidx = @{$pemp_dist_mega};
	#
	printf("\n%s\n", $label);
	#
	for (my $n=1; $n<=$number_to_choose; $n += 1)
	{
		my %chosen = ();
		printf("number %02d: ", $n);
		for (my $i=1; $i<=5; )
		{
			my $regidx = rand($maxregidx);
			my $candidate = $pemp_dist_reg->[$regidx];
			next if (exists $chosen{$candidate});
			$i += 1;
			$chosen{$candidate} = 1;
			printf("%02d ", $candidate);
		}
		my $megaidx = rand($maxmegaidx);
		printf("MEGA %02d\n", $pemp_dist_mega->[$megaidx]);
	}
}
#
sub get_birthdays {
	my ($pbirthdays_reg, $pbirthdays_mega) = @_;
	#
	# number are form 1-56 and 1-46
	#
	# Mark 02/01/96
	# Jacob 01/24/98
	# Inee 11/07/62
	# Mike 09/27/57
	# Cairn 06/19/08
	#
	my @bdnos = (2,1,96,1,24,98,11,7,62,9,27,57,6,19,8);
	#
	for (my $i=0; $i<=56; $i += 1)
	{
		$pbirthdays_reg->[$i] = 0;
	}
	#
	for (my $i=0; $i<=46; $i += 1)
	{
		$pbirthdays_mega->[$i] = 0;
	}
	#
	foreach my $bdno (@bdnos) {
		$pbirthdays_reg->[($bdno%56)+1] += 1;
	}
	#
	foreach my $bdno (@bdnos) {
		$pbirthdays_mega->[($bdno%46)+1] += 1;
	}
}
#
my %opts;
if (getopts('?n:', \%opts) != 1) {
	usage($cmd);
	exit 2;
}
#
foreach my $opt (%opts) {
	if ($opt eq "h") {
		usage($cmd);
		exit 0;
	} elsif ($opt eq "n") {
		$number_to_choose = $opts{$opt};
	}
}
#
die "number file not given" unless ($#ARGV >= 0);
my $numberfile = shift @ARGV;
#
my %data = ();
get_numbers($numberfile, \%data);
#
my $maxreg = 0;
my $maxmega = 0;
my @reg = ();
my @mega = ();
get_frequencies(\%data, \@reg, \@mega, \$maxreg, \$maxmega);
graph_frequencies(\@reg, \@mega, $maxreg, $maxmega);
#
my %sorted_reg = ();
my %sorted_mega = ();
sort_frequencies(\@reg, \@mega, \%sorted_reg, \%sorted_mega);
graph_sort_frequencies(\%sorted_reg, \%sorted_mega, $maxreg, $maxmega);
#
# empirical distributions
#
my @emp_dist_reg = ();
my @emp_dist_mega = ();
empirical_dist(\@reg, \@emp_dist_reg);
empirical_dist(\@mega, \@emp_dist_mega);
choose_from_dist("Empirical Dists", \@emp_dist_reg, \@emp_dist_mega, $number_to_choose);
#
my @nz_emp_dist_reg = ();
my @nz_emp_dist_mega = ();
non_zero_empirical_dist(\@reg, \@nz_emp_dist_reg);
non_zero_empirical_dist(\@mega, \@nz_emp_dist_mega);
choose_from_dist("Non-Zero Empirical Dists", \@nz_emp_dist_reg, \@nz_emp_dist_mega, $number_to_choose);
#
my @birthdays_reg = ();
my @birthdays_mega = ();
my @birthdays_reg_dist = ();
my @birthdays_mega_dist = ();
get_birthdays(\@birthdays_reg, \@birthdays_mega);
empirical_dist(\@birthdays_reg, \@birthdays_reg_dist);
empirical_dist(\@birthdays_mega, \@birthdays_mega_dist);
choose_from_dist("Birthdays Dists", \@birthdays_reg_dist, \@birthdays_mega_dist, $number_to_choose);

#
exit 0;
