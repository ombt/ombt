#!/usr/bin/perl -w
require DBI;
#
my @drivers = DBI->available_drivers;
my $d = join(";", @drivers);
print $d, "\n";
print "DBD::ODBC";
print " not" if ($d !~ /ODBC/);
print " installed\n";
#
exit 0;
