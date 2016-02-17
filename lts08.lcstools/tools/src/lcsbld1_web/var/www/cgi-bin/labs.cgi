#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
logmsg("LABS Logs - starting");
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> LAB LOG FILES </h3>\n";
#
print "<form method=\"post\" action=\"labs2.cgi\">\n";
#
print "<h4>Choose labid:\n";
print "<select name=labid>\n";
#
open(LABIDFD, "/etc/auto.spfs") or fatalerrormsg(0,"ERROR: can't open /etc/auto.spfs for read: $!");
#
$oldlabid = "";
#
while (defined($rec = <LABIDFD>)) {
	chomp($rec);
	$rec =~ /^([^_]*).*$/;
	$labid = $1;
	if ($labid ne $oldlabid) {
		print "<option> ${labid}\n";
		$oldlabid = $labid;
	}
}
#
close(LABIDFD);
#
print "</select></h4>\n";
#
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
