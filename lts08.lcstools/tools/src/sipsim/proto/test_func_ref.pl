#!/opt/exp/bin/perl -w
#
sub hello_world {
	my ($msg) = @_;
	#
	if (defined($msg)) {
		print STDERR "${msg}\n";
	} else {
		print STDERR "Hello, World !!!\n";
	}
}
#
my ($fname) = @ARGV;
#
&{${fname}}("Hi, Harry !!!");
#
exit 0;
