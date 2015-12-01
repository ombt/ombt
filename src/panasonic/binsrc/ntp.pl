#!/usr/bin/perl 
# ntp.pl - a simple program to query an NTP server for the current
# time and use the reply to set the system clock.  For the last
# bit to work this has to run as root.
# Note: This is a simple program - there is no attempt at precison!
#       It will should the time to within a second or two which
#	for me is more than adequate.
#
# Author : David Parkinson
# Version: 1.0
# Date   : 17th August 2010
#
# Based on an original program (ntpdate.pl) by:
#   Tim Hogard (thogard@abnormal.com)
#   http://www.abnormal.com/~thogard/ntp/

use strict;
use Socket;

my( $HOSTNAME, $PORTNO, $MAXLEN, $MSG );
my( $ipaddr, $portaddr );
my( $zz, $netTime, $sysTime, $clock, $now );

$HOSTNAME = shift;
$HOSTNAME = "ntp.panasonicfa.com" unless $HOSTNAME ;	# our NTP server
$PORTNO   = 123;	# NTP is port 123
$MAXLEN   = 1024;	# check our buffers

my $verbose = 1;	# Set for more info

#we use the system call to open a UDP socket
socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "NTP: socket: $!";

#convert hostname to ipaddress if needed
$ipaddr   = inet_aton($HOSTNAME);
$portaddr = sockaddr_in($PORTNO, $ipaddr);

# build a message.  Our message is all zeros except for a one in the protocol version field
# $msg in binary is 00 001 000 00000000 ....  or in C msg[]={010,0,0,0,0,0,0,0,0,...}
# it should be a total of 48 bytes long
$MSG="\010"."\0"x47;

#send the data
send(SOCKET, $MSG, 0, $portaddr) == length($MSG) or die "NTP: cannot send to $HOSTNAME($PORTNO): $!";

# Use a 10 second timeout
vec($zz,fileno(SOCKET),1) = 1;
if( select($zz,undef,undef,10.0) )
{
  $portaddr = recv(SOCKET, $MSG, $MAXLEN, 0)      or die "NTP: recv: $!";
} else {
  die "NTP: Timeout";
}

# Get current system time
my $sysTime = time();

#We get 12 long words back in Network order
my @l=unpack("N12",$MSG);

##foreach(@l) {
##printf("%08x ",$_);
##print "\n" if($x++%4==3);
##}

# The high word of transmit time is the 10th word we get back
# $netTime is the time in seconds not accounting for network delays
# which should be way less than a second if this is a local NTP server
my $netTime=$l[10];	# get transmit time

# NTP is number of seconds since 0000 UT on 1 January 1900
# Unix time is seconds since 0000 UT on 1 January 1970
$netTime -= 2208988800;		# Align with system time

# If needed show what's what adjusted for our time zone
if( $verbose ) {
  $now   = localtime( $netTime );
  $clock = localtime( $sysTime );
  print "System: $clock ($sysTime)\n";
  print "Net:    $now ($netTime)\n";
}

# Now adjust the clock by the difference in seconds between
# our local clock and NTP time.
my $off = $netTime - $sysTime;
if( $off ) {
  system( "date --set=\'$off seconds\'" );
# system( "hwclock --systohc" );	# Optional sync
  $now = localtime( time() );
  print "NTP: Local clock set to $now (was out by $off seconds)\n" ;
} else {
  print "NTP: Local clock was accurate\n";
}
