#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Base Scripts </h3>\n";
#
print "<form method=\"post\" action=\"basescripts2.cgi\">\n";
#
print "<h4> Show data for: \n";
print "<select name=labid>\n";
#
chdir("/lcsl100/basescripts");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/base.$$");
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
print "<option selected> NONE\n";
#
close(BASEFD);
#
unlink("/tmp/base.$$");
#
print "</select>\n";
print "</h4>\n";
#
print "<h3> Or,<br><br>HUG Base Scripts </h3>\n";
#
print "<h4> Show data for: \n";
print "<select name=huglabid>\n";
#
chdir("/lcsl100/HUGbasescripts");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/base.$$");
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
print "<option selected> NONE\n";
#
close(BASEFD);
#
unlink("/tmp/base.$$");
#
print "</select>\n";
print "</h4><br>\n";
#
print "<h3> Or,<br><br>BBB Base Scripts </h3>\n";
#
print "<h4> Show data for: \n";
print "<select name=bbblabid>\n";
#
chdir("/lcsl100/bbbbasescripts");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/base.$$");
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
print "<option selected> NONE\n";
#
close(BASEFD);
#
unlink("/tmp/base.$$");
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


