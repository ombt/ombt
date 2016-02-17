#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$outfile = $FORM_DATA{outfile};
$cmd = $FORM_DATA{cmd};
#
print "Content-type: text/plain\n\n";
#
my $logfile = "/lcsl100/tmp/${outfile}.out.$$";
#
print "CMD    : ${cmd}\n";
print "LOGFILE: ${logfile}\n";
#
system("${cmd} 2>&1 | tee ${logfile}");
#
exit 0;
