#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/cgidata.cgi";
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Maintenance </h3>\n";
#
print "<form method=\"post\" action=\"maint2.cgi\">\n";
#
print "<h4>Choose operation:<h4>\n";
#
print "<table>\n";
#
print "<h4>\n";
foreach my $maintoper (sort keys %maintmenu) {
	if ($maintmenu{${maintoper}}{active} eq "no") {
		next;
	}
	print "<tr><td>\n";
	if ($maintmenu{${maintoper}}{selected} ne "yes") {
		print "<input type=\"radio\" name=\"operation\" value=\"${maintoper}\"> ${maintoper}\n";
	} else {
		print "<input type=\"radio\" name=\"operation\" value=\"${maintoper}\" checked> ${maintoper}\n";
	}
	print "</td></tr>\n";
}
#
print "</h4>\n";
#
print "</table><br>\n";
#
print "<input type=hidden name=\"hugonly\" value=\"no\">\n";
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
