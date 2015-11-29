#!/usr/bin/perl

# import module
use Finance::Quote;

# create object
my $q = Finance::Quote->new();

# retrieve stock quote
my %data = $q->fetch('nyse', 'AAPL');

# print price
print "The current price of AAPL on the NYSE is " . $data{'AAPL', 'price'} . "\n";

# done
exit 0;
