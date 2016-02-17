#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$branch = $FORM_DATA{branch};
$load = $FORM_DATA{load};
$labid = $FORM_DATA{labid};
$script = $FORM_DATA{script};
$hug = $FORM_DATA{hug};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> ${hug} Scripts: ${branch}-${load}-${labid}-${script} </h3>\n";
print "</center>\n";
print "<pre>\n";
#
chdir("/lcsl100/${hug}scripts/${branch}/${load}/${labid}");
#
open(BASEFD, "${script}") or die "can't open ${script}";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "${brec}\n";
}
#
close(BASEFD);
#
print "</pre>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

