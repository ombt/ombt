#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$test = $FORM_DATA{test};
#
logmsg("Run GDB starting");
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> GDB - ($test) </h3>\n";
#
print "<form method=\"post\" action=\"gdb2.cgi\">\n";
#
print "<h4>Choose labid:\n";
print "<select name=labid>\n";
#
# use /etc/auto.spfs as the source of labids since it contains all
# SP file systems that are cross-mounted on lcsbld1, ie, if the labid
# is not in here, then it not accessible. another thing, it gives
# me a convenient place to store labids.
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
print "<h4>Choose SP:\n<select name=searchsp>\n";
print "<option value=\"spa\"> SP-A\n";
print "<option value=\"spb\"> SP-B\n";
print "<option value=\"both\" selected> BOTH\n";
print "</select></h4>\n";
#
print "<h4>Choose Partition to Search:\n<select name=searchpart>\n";
print "<option value=\"home1\"> home1\n";
print "<option value=\"root\"> root\n";
print "<option value=\"both\" selected> both\n";
print "</select></h4>\n";
#
print "<input type=hidden name=\"test\" value=\"${test}\">\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


