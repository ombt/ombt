#!/opt/exp/bin/perl
#
use IO::Handle;
use Socket;
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$load = $FORM_DATA{load};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> LOADS - ${load} </h3>\n";
#
logmsg("Listing Load Files - (${load})");
#
print "</center>\n";
#
print "<pre>\n";
open(PIPE, "/usr/bin/find ${load} -type f -print 2>&1 |") || fatalerrormsg(0,"ERROR: unable to run find ${load}: $!");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
}
close(PIPE);
STDOUT->autoflush(0);
#
print "</pre>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;

