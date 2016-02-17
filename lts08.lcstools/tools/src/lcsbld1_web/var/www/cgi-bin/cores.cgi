#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
logmsg("Find CORES starting");
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Find Cores </h3>\n";
#
print "<h4> Mounting File Systems </h4></center>\n";
#
print "<pre>\n";
open(PIPE, "/usr/bin/mountlabs 2>&1 |") || fatalerrormsg(1,"ERROR: unable to mountlabs - $!");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	$urec =~ s/</&lt;/g;
	print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
}
close(PIPE);
STDOUT->autoflush(0);
print "</pre>\n";
#
print "<center> <h4> Searching For Core Files</h4> </center>\n";
#
print "<pre>\n";
open(PIPE, "/bin/ls /labs/*home1/Telica/swCPU/core 2>&1 |") || fatalerrormsg(1,"ERROR: unable to ls /labs - $!");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	$urec =~ s/</&lt;/g;
	print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
}
close(PIPE);
STDOUT->autoflush(0);
print "</pre>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;
