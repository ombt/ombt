#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$baselabid = $FORM_DATA{baselabid};
$basebranch = $FORM_DATA{basebranch};
$basedir = $FORM_DATA{basedir};
$basetype = $FORM_DATA{basetype};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> ${basetype} Base Scripts: ${baselabid}-${basebranch} </h3>\n";
#
print "<form method=\"post\" action=\"basescripts4.cgi\">\n";
#
print "<select name=basescript>\n";
#
chdir("/lcsl100/${basedir}/${baselabid}/${basebranch}/current");
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
print "<input type=hidden name=\"baselabid\" value=\"${baselabid}\">\n";
print "<input type=hidden name=\"basebranch\" value=\"${basebranch}\">\n";
print "<input type=hidden name=\"basedir\" value=\"${basedir}\">\n";
print "<input type=hidden name=\"basetype\" value=\"${basetype}\">\n";
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

