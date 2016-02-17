#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Scripts </h3>\n";
#
print "<form method=\"post\" action=\"scripts2.cgi\">\n";
#
print "<h4>Show data for: \n";
print "<select name=branch>\n";
#
chdir("/lcsl100/scripts");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/branch.$$");
#
open(SCRIPTFD, "/tmp/branch.$$") or die "can't open /tmp/branch.$$";
#
while (defined($rec = <SCRIPTFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
print "<option selected> NONE\n";
#
close(SCRIPTFD);
#
unlink("/tmp/script.$$");
#
print "</select\n";
print "</h4>\n";
#
print "<h3> Or,<br><br>HUG Scripts </h3>\n";
#
print "<h4>Show data for: \n";
print "<select name=hugbranch>\n";
#
chdir("/lcsl100/HUGscripts");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/branch.$$");
#
open(SCRIPTFD, "/tmp/branch.$$") or die "can't open /tmp/branch.$$";
#
while (defined($rec = <SCRIPTFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
print "<option selected> NONE\n";
#
close(SCRIPTFD);
#
unlink("/tmp/script.$$");
#
print "</select>\n";
print "</h4><br>\n";
#
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


