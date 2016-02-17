#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$bbblabid = $FORM_DATA{bbblabid};
$bbbbranch = $FORM_DATA{bbbbranch};
$bbboper = $FORM_DATA{bbboper};
$bbbtype = $FORM_DATA{bbbtype};
$bbbverbose = $FORM_DATA{bbbverbose};
$bbbmaxi = $FORM_DATA{bbbmaxi};
$bbbscriptname = $FORM_DATA{bbbscriptname};
$bbbscriptpath = $FORM_DATA{bbbscriptpath};
#
$bbbftpmachine = $FORM_DATA{bbbftpmachine};
$bbbftpdirectory = $FORM_DATA{bbbftpdirectory};
$bbbftpuser = $FORM_DATA{bbbftpuser};
$bbbftppasswd = $FORM_DATA{bbbftppasswd};
#
$bbbscriptfullpath = "${bbbscriptpath}/${bbbscriptname}";
$basescriptsdir = "/lcsl100/basescripts/${bbblabid}/${bbbbranch}/current";
chdir($basescriptsdir);
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
#
print "<h3>MAKING BBB TL1 SCRIPT </h4>";
print "<h4>Base Scripts Directory: ${basescriptsdir} </h4>";
print "<h4>Auto TL1 Script Path: ${bbbscriptfullpath} </h4>";
#
print "<h4>Processing TL1 Script: users </h4>\n";
#
system("/usr/bin/cp /lcsl100/basescripts/commonfiles/users ${bbbscriptfullpath}");
chmod 0777, ${bbbscriptfullpath};
#
open(BBBFD, ">>${bbbscriptfullpath}") || fatalerrormsg(0,"ERROR: FILE ${bbbscriptfullpath} - $! \n");
BBBFD->autoflush(1);
#
for (my ${i}=1; ${i}<${bbbmaxi} && $FORM_DATA{"script".${i}} ne ""; ${i}++) {
	$basescript = $FORM_DATA{"script" . ${i}};
	print "<h4>Processing TL1 Script: ${basescript} </h4>\n";
	#
	print BBBFD "# START BASE SCRIPT - ${basescript}\n";
	#
	print "<pre>\n";
	open(BASEFD,"${basescript}") || fatalerrormsg(0,"ERROR: $! \n");
	while (defined($baserec=<BASEFD>)) {
		chomp($baserec);
		#
		if (($baserec =~ /init-sys:/i) ||
		    ($baserec =~ /ed-chassis-eqpt:/i) ||
		    ($baserec =~ /ent-eqpt:/i) ||
		    ($baserec =~ /ent-eqpt-all:/i) ||
		    ($baserec =~ /cpy-mem:/i) ||
		    ($baserec =~ /ent-iproute:/i)) {
			$baserec = "# ".${baserec};
			print "Commenting: ${baserec}\n";
		}
		#
		print BBBFD "${baserec}\n";
	}
	close(BASEFD);
	print BBBFD "# END BASE SCRIPT - ${basescript}\n";
	print "</pre>\n";
}
#
close(BBBFD);
#
print "<h4>BBB Script Is ${bbbscriptfullpath}.</h4>";
#
# check if we need to FTP the file somewhere.
#
if ($bbbftpmachine ne "" && $bbbftpuser ne "" && $bbbftppasswd ne "") {
	print "<h4>Attempting to FTP File to ${bbbftpdirectory} on ${bbbftpmachine}:</h4>";
	#
	$upshscript = "/lcsl100/tmp/makebbb.upsh.script.$$";
	$upshlogfile = "/lcsl100/tmp/makebbb.upsh.log.$$";
	#
	$upsh = "/home/lcstools/tools/bin/webupsh";
	#
	$upshcmd = "${upsh} -s web_ftpload ${upshscript}";
	#
	if (!defined($bbbftpdirectory) || "${bbbftpdirectory}" eq "") {
		$bbbftpdirectory = ".";
	}
	#
	print "<h4>Writing FTP upsh script: ${upshscript}</h4>\n";
	#
	print "<pre>\n";
	print "file script {\n";
	print "\tscriptsdir .\n";
	print "}\n";
	#
	print "globals data {\n";
	print "\tftpuser ${bbbftpuser}\n";
	print "\tftppasswd ${bbbftppasswd}\n";
	print "\tftpdirectory ${bbbftpdirectory}\n";
	print "\tftpmachine ${bbbftpmachine}\n";
	print "\tbbbscript ${bbbscriptfullpath}\n";
	print "\tbbbscriptname ${bbbscriptname}\n";
	print "}\n";
	#
	print "web_ftpload exec {\n";
	print "\tftp open ftpid &lt;ftpmachine> &lt;ftpuser> &lt;ftppasswd>\n";
	print "\tftp put -t900 -b ftpid &lt;bbbscript> &lt;ftpdirectory>/&lt;bbbscriptname>\n";
	print "\tftp close ftpid\n";
	print "}\n";
	#
	# write upsh script
	#
	open(OUTFD, ">${upshscript}") || fatalerrormsg(0,"ERROR: unable to write upsh script: $! \n");
	#
	print OUTFD "file script {\n";
	print OUTFD "\tscriptsdir .\n";
	print OUTFD "}\n";
	#
	print OUTFD "globals data {\n";
	print OUTFD "\tftpuser ${bbbftpuser}\n";
	print OUTFD "\tftppasswd ${bbbftppasswd}\n";
	print OUTFD "\tftpdirectory ${bbbftpdirectory}\n";
	print OUTFD "\tftpmachine ${bbbftpmachine}\n";
	print OUTFD "\tbbbscript ${bbbscriptfullpath}\n";
	print OUTFD "\tbbbscriptname ${bbbscriptname}\n";
	print OUTFD "}\n";
	#
	print OUTFD "web_ftpload exec {\n";
	print OUTFD "\tftp open ftpid <ftpmachine> <ftpuser> <ftppasswd>\n";
	print OUTFD "\tftp put -t900 -b ftpid <bbbscript> <ftpdirectory>/<bbbscriptname>\n";
	print OUTFD "\tftp close ftpid\n";
	print OUTFD "}\n";
	#
	close(OUTFD);
	#
	print "</pre>\n";
	print "<hr>\n";
	#
	print "<h4>Executing FTP upsh script: ${upshscript}</h4>\n";
	print "<pre>\n";
	#
	open(OUT, ">${upshlogfile}");
	open(PIPE, "${upshcmd} 2>&1 |");
	#
	OUT->autoflush(1);
	PIPE->autoflush(1);
	STDOUT->autoflush(1);
	
	while (defined($urec = <PIPE>)) {
		chomp($urec);
		$urec =~ s/</&lt;/g;
		print OUT "==>> ${urec}\n" || fatalerrormsg(0,"PRINT OUT: $! \n");
		print "==>> ${urec}\n" || fatalerrormsg(0,"PRINT: $! \n");
	}
	#
	close(PIPE);
	close(OUT);
	print "</pre>\n";
} else {
	print "<h4>Skipping FTP (it was not requested):<h4>";
}
#
print "<h4>Done.</h4>";
#
print "</body>\n";
print "</html>\n";
#
exit 0;
