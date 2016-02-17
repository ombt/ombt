#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/saveload.cgi";
#
parse_form_data();
$load = $FORM_DATA{load};
$saveload = $FORM_DATA{saveload};
#
# check for saveload
#
if ("${saveload}" eq "yes") {
	# save a load
	my $ret = saveload();
	exit ${ret};
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Loads: ${load} </h3>\n";
print "<pre>\n";
#
system("/opt/exp/lib/unity/bin/uprint -h 'Lab Loads' /home/lcstools/tools/data/labloads where cpuload req \"^${load}\$\" >/tmp/data.$$ 2>&1 ");
system("/opt/exp/lib/unity/bin/uprint -h 'Images' -w300 /home/lcstools/tools/data/images where cpuload req \"^${load}\$\" >>/tmp/data.$$ 2>&1 ");
#
open(UNITYFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($urec = <UNITYFD>)) {
	chomp($urec);
	print "${urec}\n";
}
#
close(UNITYFD);
unlink("/tmp/unity.$$");
#
print "</pre>\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

