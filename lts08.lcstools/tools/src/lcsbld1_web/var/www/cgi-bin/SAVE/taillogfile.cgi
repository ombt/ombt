#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$logfile = $FORM_DATA{logfile};
$lines = $FORM_DATA{lines};
#
print "Content-type: text/plain\n\n";
#
# print "<html>\n";
# print "<body bgcolor=\"E0E0E0\">\n";
#
# print "<pre>\n";
# print "TAILING LOGFILE: ${logfile}<br><br>\n";
print "TAILING LOGFILE: ${logfile}\n\n\n";
#
open(PIPE, "/usr/bin/tail -${lines} ${logfile} 2>&1 | ");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
while (defined($urec = <PIPE>)) {
	chomp($urec);
	# $urec =~ s/</&lt;/g;
	print "${urec}\n";
}
close(PIPE);
STDOUT->autoflush(0);
#
# print "</pre>\n";
# print "</body>\n";
# print "</html>\n";
#
exit 0;

