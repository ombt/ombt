#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
logmsg("LOADS - starting");
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> LOADS </h3>\n";
#
print "<form method=\"post\" action=\"loads2.cgi\">\n";
#
print "<h4>Choose Load:\n";
#
print "<select name=load>\n";
#
open(LOADSFD, "/bin/ls -d /build/lcstools/*/Telica/swCPU |") or fatalerrormsg(0,"ERROR: can't list /build/lcstools/* : $!");
#
while (defined($rec = <LOADSFD>)) {
	chomp($rec);
	print "<option> ${rec}\n";
}
#
close(LOADSFD);
#
print "</select></h4>\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
