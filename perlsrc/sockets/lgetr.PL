#!/usr/bin/perl
#
use IO::Socket;
#
my $server = shift;
#
my $fh = IO::Socket::INET->new($server);
#
my $line = <$fh>;
#
print $line;
#
close($fh);
#
exit 0;
