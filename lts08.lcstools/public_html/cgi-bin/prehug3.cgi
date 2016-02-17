#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$switchid = $labid;
$switchid =~ s/ihlcsg([0-9]+)/ihlcsgw\1/;
$branch = $FORM_DATA{branch};
$ftpmachine = $FORM_DATA{ftpmachine};
$ftpuser = $FORM_DATA{ftpuser};
$ftppasswd = $FORM_DATA{ftppasswd};
$tl1user = $FORM_DATA{tl1user};
$tl1passwd = $FORM_DATA{tl1passwd};
$lynxuser = $FORM_DATA{lynxuser};
$lynxpasswd = $FORM_DATA{lynxpasswd};
$iomtypes = $FORM_DATA{iomtypes};
$prehugoper = $FORM_DATA{prehugoper};
$scripttype = $FORM_DATA{scripttype};
$emailaddress = $FORM_DATA{emailaddress};
$maswitchid = $FORM_DATA{maswitchid};
#
# get SP IPs
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s\\n\" cpu_a_ip cpu_b_ip in /home/lcstools/tools/data/chassis where labid req \"^${labid}\$\" >/tmp/spips.$$ 2>/dev/null ");
if ( -s "/tmp/spips.$$" ) {
	open(SPIPSFD, "/tmp/spips.$$") or die "can't open /tmp/spips.$$";
	while (defined($spips = <SPIPSFD>)) {
		chomp($spips);
		last;
	}
	close(SPIPSFD);
	unlink("/tmp/spips.$$");
	($spaip, $spbip) = split / /, $spips, 2;
	if (length($spaip) == 0 || length($spbip) == 0) {
		fatalerrormsg(1, "ERROR: ${labid} SP IPs are NULL length.");
	}
} else {
	unlink("/tmp/spips.$$");
	fatalerrormsg(1, "ERROR: Unable to determine ${labid} SP IPs.");
}
#
# check if switch id is given
#
if (${maswitchid} ne "") {
	$switchid = $maswitchid;
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Pre-HUG: ${labid}/${branch}</h3>\n";
print "</center>\n";
#
print "<h4>\n";
print "<hr>";
print "LABID: $labid <br>";
print "SWITCH ID: $switchid <br>";
print "SP-A IP: $spaip <br>";
print "SP-B IP: $spbip <br>";
print "BRANCH: $branch <br>";
print "FTP MACHINE: $ftpmachine <br>";
print "FTP USER: $ftpuser <br>";
print "FTP PASSWD: $ftppasswd <br>";
print "TL1 USER: $tl1user <br>";
print "TL1 PASSWD: $tl1passwd <br>";
print "LYNX USER: $lynxuser <br>";
print "LYNX PASSWD: $lynxpasswd <br>";
print "IOM TYPES: $iomtypes <br>";
print "SCRIPT TYPE: $scripttype <br>";
if ($emailaddress eq "") {
	print "EMAIL ADDRESS: NONE <br>";
} else {
	print "EMAIL ADDRESS: $emailaddress <br>";
}
print "<hr>";
#
my $iomcnt = 0;
my $finaliomtypes = "";
#
foreach $iomtype ( split /,/, $iomtypes ) {
	print "IOM TYPE: $iomtype <br>";
	#
	my $iomfile = "${iomtype}tarfilename";
	my $iompath = "${iomtype}tarfilepath";
	#
	if (exists($FORM_DATA{${iomfile}})) {
		$iomfile = $FORM_DATA{${iomfile}};
	}
	if (exists($FORM_DATA{${iompath}})) {
		if (length($FORM_DATA{${iompath}}) > 0) {
			$iompath = $FORM_DATA{${iompath}};
		} else {
			$iompath = "NONE";
		}
	}
	#
	if ($iomfile ne "NONE" && $iompath ne "NONE") {
		print "IOM TAR FILE NAME: $iomfile <br>";
		print "IOM TAR FILE PATH: $iompath <br>";
		fatalerrormsg(0, "ERROR: Both TAR file name and path were given for IOM type ${iomtype}.");
	} elsif ($iomfile eq "NONE" && $iompath ne "NONE") {
		$iomcnt += 1;
		$finaliomtypes .= ",${iomtype}";
		$iomfiles{$iomtype} = "PATH ${iompath}";
		print "IOM TAR FILE PATH: $iompath <br>";
	} elsif ($iomfile ne "NONE" && $iompath eq "NONE") {
		$iomcnt += 1;
		$finaliomtypes .= ",${iomtype}";
		$iomfiles{$iomtype} = "NAME ${iomfile}";
		print "IOM TAR FILE NAME: $iomfile <br>";
	} else {
		print "No TAR file given for IOM type ${iomtype}.<br>";
	}
}
$finaliomtypes =~ s/^,//;
#
if ($iomcnt <= 0) {
	fatalerrormsg(0, "ERROR: No TAR files chosen to FTP to switch.");
}
#
print "<hr>\n";
print "</h4>\n";
#
$prehugscript = "/lcsl100/tmp/prehug.script.${labid}.$$";
$prehugcfgfile = "/lcsl100/tmp/prehug.cfgfile.${labid}.$$";
$prehuglogfile = "/lcsl100/tmp/prehug.logfile.${labid}.$$";
$prehugupshlogfile = "/lcsl100/tmp/prehug.upsh.logfile.${labid}.$$";
#
open(CFGFD, ">${prehugcfgfile}") || fatalerrormsg(0,"ERROR: $! \n");
CFGFD->autoflush(1);
#
print CFGFD "globals data {\n";
print CFGFD "\tscriptsdir .\n";
print CFGFD "\t#\n";
print CFGFD "\tlabid ${labid}\n";
print CFGFD "\tswitchid ${switchid}\n";
if ($emailaddress ne "") {
	chomp($emailaddress);
	$emailaddress =~ s/^[\s,;:]*//;
	$emailaddress =~ s/[\s,;:]*$//;
	$emailaddress =~ s/[\s,;:]+/ /g;
	print CFGFD "\temail $emailaddress\n";
}
print CFGFD "\t${switchid} {\n";
print CFGFD "\t\tlogin $lynxuser\n";
print CFGFD "\t\tpasswd $lynxpasswd\n";
print CFGFD "\t\ttl1login $tl1user\n";
print CFGFD "\t\ttl1passwd $tl1passwd\n";
print CFGFD "\t\tspa {\n";
print CFGFD "\t\t\t ip ${spaip}\n";
print CFGFD "\t\t}\n";
print CFGFD "\t\tspb {\n";
print CFGFD "\t\t\t ip ${spbip}\n";
print CFGFD "\t\t}\n";
print CFGFD "\t}\n";
print CFGFD "\t#\n";
print CFGFD "\tload {\n";
print CFGFD "\t\tbranch $branch\n";
print CFGFD "\t\tftpmachine $ftpmachine\n";
print CFGFD "\t\tftplogin $ftpuser\n";
print CFGFD "\t\tftppasswd $ftppasswd\n";
print CFGFD "\t\timages {\n";
print CFGFD "\t\t\tiomtypes $finaliomtypes\n";
#
foreach $iomtype (sort keys %iomfiles) {
	print CFGFD "\t\t\t${iomtype} ".$iomfiles{$iomtype}."\n";
}
#
print CFGFD "\t\t}\n";
print CFGFD "\t}\n";
print CFGFD "}\n";
#
close(CFGFD);
#
if ($scripttype eq "lite") {
	$scripttype = "-L";
} elsif ($scripttype eq "direct") {
	$scripttype = "-D";
} else {
	$scripttype = "";
}
#
# direct only works from lts08.
#
if ($scripttype eq "-D" && $ftpmachine ne "lts08.ih.lucent.com") {
	fatalerrormsg(0, "ERROR: FTP machine MUST be lts08.ih.lucent.com when directly calling FTP and TAR (fastest method).");
}
#
$prehugpp = "/home/lcstools/tools/bin/webprehugpp";
$prehugcmd = "${prehugpp} ${scripttype} -o ${prehugscript} ${prehugcfgfile}";
#
$upsh = "/home/lcstools/tools/bin/webupsh";
$upshverifycmd = "${upsh} -X -s prehug ${prehugscript}";
$upshexeccmd = "${upsh} -s prehug ${prehugscript}";
#
print "<h4>\n";
print "PREHUG CFG FILE: ${prehugcfgfile} <br>\n";
print "PREHUG LOG FILE: ${prehuglogfile} <br>\n";
print "PREHUG SCRIPT FILE: ${prehugscript} <br>\n";
print "PREHUG CMD: ${prehugcmd} <br>\n";
print "PREHUG UPSH VERIFY CMD: ${upshverifycmd} <br>\n";
print "PREHUG UPSH EXEC CMD: ${upshexeccmd} <br>\n";
print "PREHUG UPSH LOG FILE: ${prehugupshlogfile} <br>\n";
print "</h4>\n";
print "<hr>\n";
#
print "<pre>\n";
#
open(OUT, ">${prehuglogfile}");
open(PIPE, "${prehugcmd} 2>&1 |");
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
#
close(PIPE);
#
# check if we verify-only or execute the prehug update script.
#
if ($prehugoper eq "genandverify") {
	if ($pid = fork) {
		# parent process
		$SIG{CHLD} = 'IGNORE';
		print "\nPARENT: UPSH VERIFY ($pid) started. Parent will exit.";
	} else {
		# child process.
		system("${upshverifycmd} 1>${prehugupshlogfile} 2>&1");
		exit 0;
	}
	print "</pre>\n";
	print "<hr>\n";
	print "<h4>\n";
	print "TAIL THIS LOG FILE TO GET THE FINAL RESULTS: <br> ${prehugupshlogfile}\n";
	print "</h4>\n";
} elsif ($prehugoper eq "genandexec") {
	if ($pid = fork) {
		# parent process
		$SIG{CHLD} = 'IGNORE';
		print "\nPARENT: UPSH EXEC ($pid) started. Parent will exit.";
	} else {
		# child process.
		system("${upshexeccmd} 1>${prehugupshlogfile} 2>&1");
		exit 0;
	}
	print "</pre>\n";
	print "<hr>\n";
	print "<h4>\n";
	print "TAIL THIS LOG FILE TO GET THE FINAL RESULTS: <br> ${prehugupshlogfile}\n";
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
