#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> SP Pre-HUG: ${labid}</h3>\n";
#
print "<form method=\"post\" action=\"spprehug3.cgi\">\n";
#
print "<hr>\n";
print "<h4> Branch/load for SP tar file:<br>\n";
print "<select name=load>\n";
print "<option selected> NONE\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s\\n\" branch cpuload in /home/lcstools/tools/data/labloads where labid req \"^${labid}\$\" | sort -u >/tmp/data.$$ 2>&1 ");
#
open(LOADSFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
while (defined($rec = <LOADSFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
close(LOADSFD);
unlink("/tmp/data.$$");
#
print "</select>\n";
#
print "<br>Or,<br>Full path to CPU tar file on FTP machine:<br>\n";
print "<input type=text name=\"cputarfilepath\" value=\"\" size=40>\n";
print "</h4>\n";
#
print "<hr>\n";
print "<h4>\n";
print "FTP Machine:\n";
print "<input type=text name=\"ftpmachine\" value=\"lts08.ih.lucent.com\" size=20>\n";
print "</h4>\n";
print "<h4>\n";
print "FTP User:\n";
print "<input type=text name=\"ftpuser\" value=\"lcsinteg\" size=12>\n";
print "</h4>\n";
print "<h4>\n";
print "FTP Passwd:\n";
print "<input type=text name=\"ftppasswd\" value=\"plexus9000\" size=12>\n";
print "</h4>\n";
#
#
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

