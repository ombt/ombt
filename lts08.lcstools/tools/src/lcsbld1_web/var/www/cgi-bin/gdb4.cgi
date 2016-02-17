#!/opt/exp/bin/perl
#
use IO::Handle;
use Socket;
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$test = $FORM_DATA{test};
$labid = $FORM_DATA{labid};
$searchsp = $FORM_DATA{searchsp};
$searchpart = $FORM_DATA{searchpart};
$branch = $FORM_DATA{branch};
$load = $FORM_DATA{load};
$corefile = $FORM_DATA{corefile};
$debugfile = $FORM_DATA{debugfile};
$privatedebugfile = $FORM_DATA{privatedebugfile};
$analyzethreads = $FORM_DATA{analyzethreads};
$analyzeframes = $FORM_DATA{analyzeframes};
#
if (${privatedebugfile} ne "") {
	if ( ! -r ${privatedebugfile}) {
		fatalerrormsg(1,"ERROR: unable to read private debug file: ${privatedebugfile}");
	}
	$debugfile = $privatedebugfile;
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> GDB </h3>\n";
print "<h4>($test, $labid, $branch, $load, $corefile, $debugfile, $analyzeframes, $analyzethreads)</h4>\n";
#
logmsg("Running GDB - $$ - ($labid, $branch, $load, $corefile, $debugfile, $analyzethreads)");
#
chomp($labid);
chomp($branch);
chomp($load);
chomp($corefile);
chomp($debugfile);
#
if ($labid eq "") {
	fatalerrormsg(0,"ERROR: labid is not set.");
} elsif ($branch eq "") {
	fatalerrormsg(0,"ERROR: branch is not set.");
} elsif ($load eq "") {
	fatalerrormsg(0,"ERROR: load is not set.");
} elsif ($corefile eq "") {
	fatalerrormsg(0,"ERROR: core file is not set.");
} elsif ($debugfile eq "") {
	fatalerrormsg(0,"ERROR: debug file is not set.");
}
#
$logfile = "/tmp/gdb.out.$$";
#
print "</center>\n";
#
if ($test eq "yes") {
	$cmd = "/var/www/cgi-bin/newlynxos.gdb.sh $branch $load $debugfile $corefile $analyzeframes $analyzethreads";
} else {
	$cmd = "/var/www/cgi-bin/lynxos.gdb.sh $branch $load $debugfile $corefile $analyzeframes $analyzethreads";
}
#
print "<pre>\n";
print "GDB CMD: ${cmd}\n";
print "GDB LOGFILE: ${logfile}<br>\n";
#
open(OUT, ">${logfile}") || fatalerrormsg(0,"ERROR: unable to open log file: $!");
open(PIPE, "${cmd} 2>&1 |") || fatalerrormsg(0,"ERROR: unable to run cmd: ${cmd} - $!");
#
OUT->autoflush(1);
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	$urec =~ s/</&lt;/g;
	print OUT "==>> ${urec}\n" || fatalerrormsg(0,"PRINT OUT: $! \n");
	print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
}
close(PIPE);
close(OUT);
STDOUT->autoflush(0);
#
print "</pre>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;

