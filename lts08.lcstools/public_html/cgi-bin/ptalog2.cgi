#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
syslog("ptalog", "ptalog2.cgi", "starting");
#
parse_form_data();
$ptauser = $FORM_DATA{ptauser};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> PTA Log Files - ${ptauser} </h3>\n";
#
print "<form method=\"post\" action=\"ptalog3.cgi\">\n";
#
print "<h4>Choose Session: ";
print "<select name=ptasession>\n";
#
open(PIPE, "/bin/ls -dt /tester/log/${ptauser}* |");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	#
	if ($urec =~ /no *such *file *or *directory/i) {
		# nothing was found
		last;
	}
	if ( ! -d "${urec}") {
		# not a directory. just skip it.
		next;
	}
	#
	my ($user, $date, $sessionid) = split /_/, "${urec}", 3;
	print "<option> ${date}_${sessionid}\n";
}
close(PIPE);
STDOUT->autoflush(0);
#
print "</select>\n";
print "</h4>\n";
#
print "<h4>Unconditionally build database?\n";
print "<select name=bldptadb>\n";
print "<option selected> no\n";
print "<option> yes\n";
print "</select>\n";
print "</h4>\n";
#
print "<h4>Show Tests With This Status:\n";
print "<select name=statusfilter>\n";
print "<option value=\".*\" selected> All Status\n";
print "<option value=\".*SUCCESS.*\"> Success\n";
print "<option value=\".*FAIL.*\"> Fail\n";
print "</select>\n";
print "</h4>\n";
#
#
print "<input type=hidden name=\"ptauser\" value=\"${ptauser}\">\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
