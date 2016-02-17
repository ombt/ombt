#!/opt/exp/bin/perl -w
#
# test a general SIP msg parser.
#
###########################################################################
#
sub dumpSipMsg {
	my ($phdrs) = @_;
	#
	foreach my $key (sort keys %{$phdrs}) {
		print "$key ==>> @{$phdrs->{$key}}\n";
	}
}
#
sub parseSipMsg {
	my ($rawmsg, $prawhdr, $prawdata, $phdrs, $pdatablocks) = @_;
	#
	# separate the hdr from the payload.
	#
	($$prawhdr, $$prawdata) = split /[\r\n]{4,4}/, $rawmsg, 2;
	return unless (defined($$prawhdr));
	#
	# split fields in header. each header field is 
	# separated by CRLF. each header in turn is separated
	# by commas first, then semicolons. also, headers can 
	# be repeated. each repeated header field can be combined
	# into one header field separated by commas. each header field
	# can contain parameters which are separated by semicolons.
	#
	#
	my @fldhdrs = split /[\r\n]+/, $$prawhdr;
	#
	foreach my $fldhdr (@fldhdrs) {
		($fldname, $fldvalues) = split /\s*:\s*/, $fldhdr, 2;
		if (!defined($fldname) || !defined($fldvalues)) {
			next;
		}
		@fldvals = split /,/, $fldvalues;
		foreach $fldval (@fldvals) {
			unshift(@{$phdrs->{$fldname}}, $fldval);
		}
	}
	#
	# check if we have more than one data block
	#
	if (!exists($phdrs->{"Content-Type"})) {
		# no data payload
		@{$pdatablocks} = undef;
	} elsif ($phdrs->{"Content-Type"} =~ /^multipart\/mixed/) {
		# multiple data blocks, separate them.
		$phdrs->{"Content-Type"} =~ /^multipart\/mixed;boundary=\"([^\"]+)\"/;
		my $dbdelimiter = $1;
		die "delimiter not found in Content-Type = mixed." unless 
			defined($dbdelimiter);
		@{$pdatablocks} = split /${dbdelimiter}/, $$prawdata;
	} else {
		# only one data block.
		$pdatablocks->[0] = $$prawdata;
	}
	#
	return;
}
#
foreach my $fname (@ARGV) {
	my $rawmsg = "";
	#
	open(INFILE, $fname) || die "can't open file $fname: $!";
	while (defined(my $line = <INFILE>)) {
		chomp($line);
		$rawmsg .= "$line\r\n";
	}
	#
	my $rawhdr;
	my $rawdata;
	my %siphdrs;
	my %sdphdrs;
	my @datablocks;
	#
	parseSipMsg($rawmsg, \$rawhdr, \$rawdata, \%siphdrs, \@datablocks);
	#
	dumpSipMsg(\%siphdrs);
}
#
exit 0;

