#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$hugtype = $FORM_DATA{hugtype};
$uchugtype = uc(${hugtype});
$emailaddress = $FORM_DATA{emailaddress};
$hugcfgfile = $FORM_DATA{prehugcfgfile};
$hugoper = $FORM_DATA{hugoper};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> ${uchugtype} HUG: ${labid}</h3>\n";
print "</center>\n";
#
print "<h4>\n";
print "<hr>";
print "LABID: $labid <br>";
print "HUG TYPE: $hugtype <br>";
print "PRE-HUG CFG FILE: $hugcfgfile <br>";
print "HUG OPER: $hugoper <br>";
if ($emailaddress eq "") {
	print "EMAIL ADDRESS: NONE <br>";
} else {
	print "EMAIL ADDRESS: $emailaddress <br>";
}
print "</h4>";
print "<hr>";
#
$hugscript = "/lcsl100/tmp/${hugtype}hug.script.${labid}.$$";
$huglogfile = "/lcsl100/tmp/${hugtype}hug.logfile.${labid}.$$";
$hugupshlogfile = "/lcsl100/tmp/${hugtype}hug.upsh.logfile.${labid}.$$";
#
$hugpp = "/home/lcstools/tools/bin/web${hugtype}hugpp";
$hugcmd = "${hugpp} -o ${hugscript} ${hugcfgfile}";
#
$upsh = "/home/lcstools/tools/bin/webupsh";
$upshverifycmd = "${upsh} -X -s ${hugtype}hug ${hugscript}";
$upshexeccmd = "${upsh} -s ${hugtype}hug ${hugscript}";
#
print "<h4>\n";
print "${uchugtype} HUG CFG FILE: ${hugcfgfile} <br>\n";
print "${uchugtype} HUG LOG FILE: ${huglogfile} <br>\n";
print "${uchugtype} HUG SCRIPT FILE: ${hugscript} <br>\n";
print "${uchugtype} HUG CMD: ${hugcmd} <br>\n";
print "${uchugtype} HUG UPSH VERIFY CMD: ${upshverifycmd} <br>\n";
print "${uchugtype} HUG UPSH EXEC CMD: ${upshexeccmd} <br>\n";
print "${uchugtype} HUG UPSH LOG FILE: ${hugupshlogfile} <br>\n";
print "</h4>\n";
print "<hr>\n";
#
print "<pre>\n";
#
open(OUT, ">${huglogfile}");
open(PIPE, "${hugcmd} 2>&1 |");
#
OUT->autoflush(1);
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	print OUT "==>> ${urec}\n" || fatalerrormsg(0,"PRINT OUT: $! \n");
	print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
}
#
close(PIPE);
#
# check if we verify-only or execute the hug update script.
#
if ($hugoper eq "genandverify") {
	$SIG{CHLD} = 'IGNORE';
	if ($pid = fork) {
		# parent process
		print "\nPARENT: UPSH VERIFY ($pid) started. Parent will exit.";
	} else {
		# child process.
		system("${upshverifycmd} 1>${hugupshlogfile} 2>&1");
		exit 0;
	}
	print "</pre>\n";
	print "<hr>\n";
	print "<h4>\n";
	print "TAIL THIS LOG FILE TO GET THE FINAL RESULTS: <br> ${hugupshlogfile}\n";
	print "</h4>\n";
} elsif ($hugoper eq "genandexec") {
	$SIG{CHLD} = 'IGNORE';
	if ($pid = fork) {
		# parent process
		print "\nPARENT: UPSH EXEC ($pid) started. Parent will exit.";
	} else {
		# child process.
		system("${upshexeccmd} 1>${hugupshlogfile} 2>&1");
		exit 0;
	}
	print "</pre>\n";
	print "<hr>\n";
	print "<h4>\n";
	print "TAIL THIS LOG FILE TO GET THE FINAL RESULTS: <br> ${hugupshlogfile}\n";
	print "</h4>\n";
} else {
	print "</pre>\n";
}
#
close(OUT);
STDOUT->autoflush(0);
#
print "</body>\n";
print "</html>\n";
#
exit 0;
