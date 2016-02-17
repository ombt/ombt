#!/opt/exp/bin/perl
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> LCSTOOLS WEB-TMS </h3>\n";
print "<h1> DO NOT USE -- NOT SECURE !!! </h1>\n";
#
print "<form method=\"post\" action=\"tms2.cgi\">\n";
#
print "<h4>Login:\n";
print "<input type=text name=\"tmslogin\" value=\"\" size=40>\n";
print "</h4>\n";
print "<h4>Password:\n";
print "<input type=password name=\"tmspassword\" value=\"\" size=40>\n";
print "</h4>\n";
print "<h4>Machine:\n";
print "<input type=text name=\"tmsmachine\" value=\"ihecms.ih.lucent.com\" size=40>\n";
print "</h4>\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
