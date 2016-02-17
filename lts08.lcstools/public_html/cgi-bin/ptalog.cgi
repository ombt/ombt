#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
print "Content-type: text/html\n\n";
#
syslog("ptalog", "ptalog.cgi", "starting");
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> PTA Log Files </h3>\n";
#
print "<form method=\"post\" action=\"ptalog2.cgi\">\n";
#
print "<h4>Show data for PTA User: ";
print "<select name=ptauser>\n";
#
if ( ! -d "/tester/log" ) {
	fatalerrormsg(0,"ERROR: PTA Log file directory, /tester/log, was not found.");
}
#
open(PIPE, "/bin/ls /tester/log |");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
my $currentuser = "";
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	#
	if ($urec =~ /no *such *file *or *directory/i) {
		# nothing was found
		last;
	}
	#
	if ( ! -d "/tester/log/${urec}") {
		# not a directory. just skip it.
		next;
	}
	#
	my ($user, $date, $sessionid) = split /_/, "${urec}", 3;
	#
	if ($currentuser eq $user) {
		# same user. skip it.
		next;
	}
	$currentuser = $user;
	#
	print "<option> ${currentuser}\n";
}
close(PIPE);
STDOUT->autoflush(0);
#
print "</select>\n";
print "</h4>\n";
#
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
