#!/opt/exp/bin/perl -w 
#
my @data = (0x30, 0x32, 0x34, 0x38, 0x36);
print "data = @data\n";
#
my $rawdata = pack "N*", @data;
#
my $len = length($rawdata);
print "rawdata: $len \n";
print "rawdata: $rawdata \n";
#
exit 0;
