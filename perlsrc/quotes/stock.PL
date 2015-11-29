#!/usr/bin/perl
#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#

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
