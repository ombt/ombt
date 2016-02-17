#!/usr/bin/perl -w
use strict;

my $limit;   
my @primes;  

print "Enter an integer as an upper limit: ";
chomp( $limit = <STDIN> );

@primes = (0, 0, 2 .. $limit);

for (my $index = 2; $index <= sqrt($limit); $index++) {
    if ( $primes[$index] ) {
        my $cross_out = $index * 2;
        while ( $cross_out <= $limit ) {
            $primes[$cross_out] = 0;
            $cross_out += $index;
        }
    }
}

foreach my $prime_number ( @primes ) {
    if ( $prime_number ) {
        print "$prime_number is prime\n";
    }
}
