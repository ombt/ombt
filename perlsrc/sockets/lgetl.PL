#!/usr/bin/perl
#
use IO::File;
#
my $file = shift;
#
my $fh = IO::File->new($file);
#
my $line = <$fh>;
#
print $line;
#
close($fh);
#
exit 0;
