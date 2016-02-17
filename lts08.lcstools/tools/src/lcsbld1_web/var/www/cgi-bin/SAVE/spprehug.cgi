#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> SP Pre-HUG </h3>\n";
#
print "<form method=\"post\" action=\"spprehug2.cgi\">\n";
#
print "<hr>\n";
print "<h4>Lab ID: ";
print "<select name=labid>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
#
open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($rec = <DATAFD>)) {
	chomp($rec);
	if ($rec eq "ihlcs1") {
		# highlight our machines
		print "<option selected> ${rec}\n";
	} else {
		print "<option> ${rec}\n";
	}
}
#
close(DATAFD);
#
unlink("/tmp/data.$$");
#
print "</select>\n";
print "</h4>\n";
#
print "<h4>Branch: ";
print "<select name=branch>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" branch in /home/lcstools/tools/data/loads | sort -u >/tmp/data.$$ 2>&1 ");
#
open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
#
while (defined($rec = <DATAFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
#
close(DATAFD);
#
unlink("/tmp/data.$$");
#
print "</select>\n";
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
print "<hr>\n";
print "<h4>\n";
print "TL1 User:\n";
print "<input type=text name=\"tl1user\" value=\"telica\" size=12>\n";
print "</h4>\n";
print "<h4>\n";
print "TL1 Passwd:\n";
print "<input type=text name=\"tl1passwd\" value=\"telica\" size=12>\n";
print "</h4>\n";
print "<h4>\n";
print "Lynx-OS User:\n";
print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
print "</h4>\n";
print "<h4>\n";
print "Lynx-OS Passwd:\n";
print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
print "</h4>\n";
#
print "<hr>\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

