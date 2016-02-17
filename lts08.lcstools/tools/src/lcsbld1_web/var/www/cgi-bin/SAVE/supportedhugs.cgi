#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Unity Relation: supportedhugs </h3>\n";
print "<pre>\n";
#
system("/opt/exp/lib/unity/bin/uprint -w300 -l50 /home/lcstools/tools/data/supportedhugs >/tmp/data.$$ 2>&1 ");
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
print "</center>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;

