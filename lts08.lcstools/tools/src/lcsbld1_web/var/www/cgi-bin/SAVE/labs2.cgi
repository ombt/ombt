#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/lcsbu.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$reload = $FORM_DATA{reload};
#
# check of a reload was requested?
#
if ("${reload}" eq "yes") {
	# reload lab
	my $ret = lcsbu(${labid});
	exit ${ret};
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Labs: ${labid} </h3>\n";
print "<pre>\n";
#
system("/opt/exp/lib/unity/bin/uprint -h 'Lab Loads' /home/lcstools/tools/data/labloads where labid req \"^${labid}\$\" >/tmp/data.$$ 2>&1 ");
system("/opt/exp/lib/unity/bin/uprint -h 'Chassis' /home/lcstools/tools/data/chassis where labid req \"^${labid}\$\" >>/tmp/data.$$ 2>&1 ");
system("/opt/exp/lib/unity/bin/uprint -h 'Chassis-To-IOMs' /home/lcstools/tools/data/chassis2ioms where labid req \"^${labid}\$\" >>/tmp/data.$$ 2>&1 ");
#
open(UNITYFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($urec = <UNITYFD>)) {
	chomp($urec);
	print "${urec}\n";
}
#
close(UNITYFD);
#
unlink("/tmp/unity.$$");
#
print "</pre>\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

