#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$branch = $FORM_DATA{branch};
$load = $FORM_DATA{load};
$labid = $FORM_DATA{labid};
$hug = $FORM_DATA{hug};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> ${hug} Scripts: ${branch}-${load}-${labid} </h3>\n";
#
print "<form method=\"post\" action=\"scripts5.cgi\">\n";
#
print "<select name=script>\n";
#
chdir("/lcsl100/${hug}scripts/${branch}/${load}/${labid}");
system("/opt/exp/gnu/bin/find . -type f -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/script.$$");
#
open(BASEFD, "/tmp/script.$$") or die "can't open /tmp/script.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
#
close(BASEFD);
#
unlink("/tmp/script.$$");
#
print "</select>\n";
print "<br><br>\n";
#
print "<input type=hidden name=\"hug\" value=\"${hug}\">\n";
print "<input type=hidden name=\"branch\" value=\"${branch}\">\n";
print "<input type=hidden name=\"load\" value=\"${load}\">\n";
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

