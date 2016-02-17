#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Unity Relations </h3>\n";
#
print "<form method=\"post\" action=\"unity2.cgi\">\n";
#
print "<h4>Choose Relation: \n";
print "<select name=relation>\n";
#
system("ls /home/lcstools/tools/data/D* | sed 's?^.*/D??' | sort -f -u >/tmp/unity.$$");
#
open(UNITYFD, "/tmp/unity.$$") or die "can't open /tmp/unity.$$";
#
while (defined($urec = <UNITYFD>)) {
	chomp($urec);
	print "<option> ${urec}\n";
}
#
close(UNITYFD);
#
unlink("/tmp/unity.$$");
#
print "</select>\n";
print "</h4>\n";
#
print "<h4>Choose Operation: \n";
print "<select name=operation>\n";
#
print "<option value=\"data\" selected> Show Relation Data\n";
print "<option value=\"descriptor\"> Show Relation Descriptor File\n";
#
print "</select>\n";
print "</h4><br>\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


