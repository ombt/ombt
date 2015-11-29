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

# set mutual fund code
# each fund has a unique code
my $fcode = '1#761';

# retrieve mutual fund NAV
my %quotes = $q->fetch('indiamutual', $fcode);

# print details
if ($quotes{$fcode, 'success'})
{
      print "NAV of " . $quotes{$fcode, 'name'} . " is " . $quotes{$fcode, 'nav'}; } else
{
      die "Data retrieval error";
}

# done
exit 0;
