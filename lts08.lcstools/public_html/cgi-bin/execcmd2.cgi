#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$outfile = $FORM_DATA{outfile};
$cmd = $FORM_DATA{cmd};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
#
startcmd($outfile, $cmd);
#
print "</body>\n";
print "</html>\n";
#
exit 0;

