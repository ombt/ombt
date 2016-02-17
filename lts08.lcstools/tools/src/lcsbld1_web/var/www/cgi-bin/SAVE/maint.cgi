#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/cgidata.cgi";
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Maintenance </h3>\n";
#
print "<form method=\"post\" action=\"maint2.cgi\">\n";
#
print "<h4>Choose operation:\n";
print "<select name=operation>\n";
foreach my $maintoper (sort keys %maintmenu) {
	if ($maintmenu{${maintoper}}{active} eq "no") {
		next;
	}
	if ($maintmenu{${maintoper}}{selected} ne "yes") {
		print "<option> ${maintoper}\n";
	} else {
		print "<option selected> ${maintoper}\n";
	}
}
print "</select>\n";
#
print "</select>\n";
print "</h4>\n";
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
