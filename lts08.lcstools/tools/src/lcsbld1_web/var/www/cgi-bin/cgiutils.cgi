# cgi-bin utilities
#
use IO::Handle;
use Socket;
#
sub errormsg {
	my ($printhdr, $errmsg) = @_;
	#
	if ($printhdr) {
		print "Content-type: text/html\n\n";
		#
		print "<html>\n";
		print "<body bgcolor=\"E0E0E0\">\n";
		print "<center>\n";
	}
	#
	print "<h2> ${errmsg} </h2>";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
}
#
sub fatalerrormsg {
	my ($printhdr, $errmsg) = @_;
	errormsg($printhdr, $errmsg);
	exit 0;
}
#
sub parse_form_data {
	(*FORM_DATA) = @_;
	local ( $request_method, @query_string, @key_value_pairs,
		$key_value, $key, $value);
	$request_method = $ENV{ 'REQUEST_METHOD'};
	if ($request_method eq "GET") {
		$query_string = $ENV{'QUERY_STRING'};
	} elsif ($request_method eq "POST") {
		read (STDIN, $query_string, $ENV{'CONTENT_LENGTH'});
	} else {
		return -1;
	}
  	@key_value_pairs = split (/&/, $query_string);
  	foreach $key_value (@key_value_pairs) {
		($key, $value) = split (/=/, $key_value);
		$value =~ tr/+/ /;
		$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		if (defined($FORM_DATA{key})) {
			$FORM_DATA{$key} = join ("\0", $FORM_DATA{$key}, $value);
		} else {
			$FORM_DATA{$key} = $value;
		}
	}
	return 0;
}
#
sub finalverification {
	my ($querymsg, $helpmsg, $cmd) = @_;
	#
	print "<br><br>\n";
	print "<hr>\n";
	print "<br><br>\n";
	print "<h3> ${querymsg} </h3>\n";
	print "<form method=\"post\" action=\"execcmd.cgi\">\n";
	print "<input type=hidden name=\"helpmsg\" value=\"${helpmsg}\">\n";
	print "<input type=hidden name=\"cmd\" value=\"${cmd}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<br><br>\n";
	print "<hr>\n";
	print "<br><br>\n";
	#
	return 0;
}
#
sub startcmd {
	my ($label, $cmd) = @_;
	#
	my $logfile = "/tmp/${label}.out.$$";
	#
	print "<pre>\n";
	print "CMD    : ${cmd}\n";
	print "LOGFILE: ${logfile}<br>\n";
	#
	open(OUT, ">${logfile}") || fatalerrormsg(1,"ERROR: unable to open log file: $!");
	open(PIPE, "${cmd} 2>&1 |") || fatalerrormsg(1,"ERROR: unable to run cmd: ${cmd} - $!");

	#
	OUT->autoflush(1);
	PIPE->autoflush(1);
	STDOUT->autoflush(1);
	#
	while (defined($urec = <PIPE>)) {
		chomp($urec);
		print OUT "==>> ${urec}\n" || fatalerrormsg(0,"PRINT OUT: $! \n");
		print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
	}
	close(PIPE);
	close(OUT);
	STDOUT->autoflush(0);
	#
	print "</pre>\n";
	#
	return 0;
}
#
sub CheckPermissions {
	my ($permfilename) = @_;
	# check permissions for this user.
	my $hostname = gethostbyaddr(inet_aton($ENV{REMOTE_ADDR}), AF_INET);
	if (!defined($hostname) || $hostname eq "") {
		fatalerrormsg(1,"ERROR: Unable to determine user.");
	}
	my $permfile = "/home/lcstools/${permfilename}";
	if ( ! -r ${permfile}) {
		fatalerrormsg(1,"ERROR: Permissions file not found.");
	}
	#
	my $found = 0;
	open(ALLOWFD, "${permfile}") or die "can't open ${permfile}.";
	while (!$found && defined($rec = <ALLOWFD>)) {
		chomp($rec);
		if ($hostname eq $rec) {
			$found = 1;
		}
	}
	close(ALLOWFD);
	#
	if (!$found) {
		# fatalerrormsg(1,"ERROR: Permission denied for ${hostname}.");
		fatalerrormsg(1,"ERROR: Permission denied.");
	}
	#
	return 1;
}
#
sub logmsg {
	my ($msg) = @_;
	# who is the user.
	my $hostname = gethostbyaddr(inet_aton($ENV{REMOTE_ADDR}), AF_INET);
	if (!defined($hostname) || $hostname eq "") {
		# use what we have
		$hostname = $ENV{REMOTE_ADDR};
	}
	#
	my $now = localtime;
	system("echo \"${now} - ${hostname} - ${msg}\" >> /var/www/cgi-bin/logs/logfile");
	#
	return 0;
}
#
1;
