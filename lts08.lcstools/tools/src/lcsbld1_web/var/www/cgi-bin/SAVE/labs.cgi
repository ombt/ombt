#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Labs </h3>\n";
#
print "<form method=\"post\" action=\"labs2.cgi\">\n";
#
print "<h4>Show data for: ";
print "<select name=labid>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis | grep -v lcsbld | sort -u >/tmp/data.$$ 2>&1 ");
#
open(LOADSFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($rec = <LOADSFD>)) {
	chomp($rec);
	if ($rec eq "ihlcs1") {
		# highlight our machines
		print "<option selected> ${rec}\n";
	} else {
		print "<option> ${rec}\n";
	}
}
#
close(LOADSFD);
#
unlink("/tmp/data.$$");
#
print "</select>\n";
print "</h4>\n";
#
print "<h4>or, Reload this lab?\n";
print "<select name=reload>\n";
print "<option> yes\n";
print "<option selected> no\n";
print "</select>\n";
print "<br><br>\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

