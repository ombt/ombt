#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$branch = $FORM_DATA{branch};
$hugbranch = $FORM_DATA{hugbranch};
#
if ($branch eq "NONE" && $hugbranch eq "NONE") {
	fatalerrormsg(1,"ERROR: Both Branch and HUG Branch are set to NONE.");
} elsif ($branch ne "NONE" && $hugbranch ne "NONE") {
	fatalerrormsg(1,"ERROR: Both Branch and HUG Branch were chosen. Choose only one.");
}
#
$hug = "";
if ($hugbranch ne "NONE") {
	$hug = "HUG";
	$branch = ${hugbranch};
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> ${hug} Scripts: ${branch} </h3>\n";
#
print "<form method=\"post\" action=\"scripts3.cgi\">\n";
#
print "<select name=load>\n";
#
chdir("/lcsl100/${hug}scripts/${branch}");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/base.$$");
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
#
close(BASEFD);
#
unlink("/tmp/base.$$");
#
print "</select>\n";
print "<br><br>\n";
#
print "<input type=hidden name=\"hug\" value=\"${hug}\">\n";
print "<input type=hidden name=\"branch\" value=\"${branch}\">\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

