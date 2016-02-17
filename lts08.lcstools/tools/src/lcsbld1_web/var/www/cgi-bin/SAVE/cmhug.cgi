#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> CM HUG </h3>\n";
#
print "<form method=\"post\" action=\"cmhug2.cgi\">\n";
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
print "<hr>\n";
print "<h4> Email addresses for notification:<br>\n";
print "(Example: jones\@lucent.com smith\@lucent.com)<br>\n";
print "<input type=text name=\"emailaddress\" size=60>\n";
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

