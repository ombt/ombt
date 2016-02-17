#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$baselabid = $FORM_DATA{baselabid};
$basebranch = $FORM_DATA{basebranch};
$basescript = $FORM_DATA{basescript};
$basetype = $FORM_DATA{basetype};
$basedir = $FORM_DATA{basedir};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> ${basetype} Base Scripts: ${baselabid}-${basebranch}-${basescript} </h3>\n";
#
print "<pre>\n";
#
chdir("/lcsl100/${basedir}/${baselabid}/${basebranch}/current");
system("cat ${basescript} | sed 's/</\\&lt\\;/g' >/tmp/base.$$");
#
print "</center>\n";
print "<pre>\n";
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "${brec}\n";
}
#
close(BASEFD);
unlink("/tmp/base.$$");
#
print "</pre>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;
