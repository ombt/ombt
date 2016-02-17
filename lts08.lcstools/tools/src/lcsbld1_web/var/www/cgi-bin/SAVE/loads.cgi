#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Loads </h3>\n";
#
print "<form method=\"post\" action=\"loads2.cgi\">\n";
#
print "<h4>Show data for:\n";
print "<select name=load>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" cpuload in /home/lcstools/tools/data/loads | sort -u >/tmp/data.$$ 2>&1 ");
#
open(LOADSFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($rec = <LOADSFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
#
close(LOADSFD);
#
unlink("/tmp/data.$$");
#
print "</select>\n";
print "</h4>\n";
#
print "<h4>or, Saveload a new load?\n";
print "<select name=saveload>\n";
print "<option> yes\n";
print "<option selected> no\n";
print "</select>\n";
print "</h4>\n";
print "</select>\n";
#
print "<br>\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
