#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$ftpbranch = $FORM_DATA{ftpbranch};
$ftpcpuload = $FORM_DATA{ftpcpuload};
$ftpuser = $FORM_DATA{ftpuser};
$ftppasswd = $FORM_DATA{ftppasswd};
$ftpmachine = $FORM_DATA{ftpmachine};
$ftpdirectory = $FORM_DATA{ftpdirectory};
$ftptypes = $FORM_DATA{ftptypes};
$ftpoperation = $FORM_DATA{ftpoperation};
#
$upshscript = "/lcsl100/tmp/ftpload.upsh.script.$$";
$upshlogfile = "/lcsl100/tmp/ftpload.upsh.log.$$";
#
$upsh = "/home/lcstools/tools/bin/webupsh";
#
if ("${ftpoperation}" eq "execute") {
	$upshcmd = "${upsh} -s web_ftpload ${upshscript}";
} else {
	$upshcmd = "${upsh} -X -s web_ftpload ${upshscript}";
}
#
if (!defined($ftpdirectory) || "${ftpdirectory}" eq "") {
	$ftpdirectory = ".";
}
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" basedir in /home/lcstools/tools/data/loads where branch req \"^${ftpbranch}\$\" and cpuload req \"^${ftpcpuload}\$\"| sort -u >/tmp/data.$$ 2>&1 ");
#
open(INFD, "/tmp/data.$$") or fatalerrormsg(1,"ERROR: can't open /tmp/data.$$ for read: $!");
if (defined($ftpbasedir = <INFD>)) {
	close(INFD);
	unlink("/tmp/data.$$");
	chomp($ftpbasedir);
} else {
	close(INFD);
	unlink("/tmp/data.$$");
	fatalerrormsg(1,"ERROR: unable to determine base directory for ${ftpbranch} and $%{ftpcpuload}.");
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> FTP Load: ${ftpbranch}, ${ftpcpuload}, ${ftpdirectory}, ${ftpbasedir} </h3>\n";
#
print "<hr>\n";
print "<h4>FILES TO FTP ARE:<br><br>\n";
$filecount = 0;
foreach $ftptype (split / /, "${ftptypes}") {
	my $ftpfile = "${ftptype}file";
	if (!defined($FORM_DATA{${ftpfile}}) ||
	     $FORM_DATA{${ftpfile}} eq "") {
		next;
	}
	#
	$ftpfilename = $FORM_DATA{${ftpfile}};
	$filepath = "${ftpbasedir}/${ftpbranch}/${ftptype}/${ftpfilename}";
	print "\nTYPE: ${ftptype}<br> FILEPATH: ${filepath}<br><br>\n";
	$filecount += 1;
	#
	$filepaths{${ftptype}} = ${filepath};
}
print "</h4>\n";
print "<hr>\n";
#
if ($filecount <= 0) {
	print "<h3>NO FILES TO FTP WERE CHOSEN.<br>DONE.<br></h3>\n";
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	exit 0;
}
#
print "</h4><h4>FTP LOAD UPSH SCRIPT: ${upshscript}<br></h4>\n";
print "</center>\n";
#
print "<h4>\n";
print "<pre>\n";
print "file script {\n";
print "\tscriptsdir .\n";
print "}\n";
#
print "globals data {\n";
print "\tftpbranch ${ftpbranch}\n";
print "\tftpcpuload ${ftpcpuload}\n";
print "\tftpuser ${ftpuser}\n";
print "\tftppasswd ${ftppasswd}\n";
print "\tftpdirectory ${ftpdirectory}\n";
print "\tftpmachine ${ftpmachine}\n";
#
foreach $ftptype (keys %filepaths) {
	print "\t${ftptype} $filepaths{${ftptype}}\n";
}
print "}\n";
print "web_ftpload exec {\n";
print "\tftp open ftpid &lt;ftpmachine> &lt;ftpuser> &lt;ftppasswd>\n";
foreach $ftptype (keys %filepaths) {
	my $filename = basename("$filepaths{${ftptype}}");
	print "\tftp put -t900 -b ftpid $filepaths{${ftptype}} ${ftpdirectory}/${filename}\n";
}
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
print OUTFD "\tftpbranch ${ftpbranch}\n";
print OUTFD "\tftpcpuload ${ftpcpuload}\n";
print OUTFD "\tftpuser ${ftpuser}\n";
print OUTFD "\tftppasswd ${ftppasswd}\n";
print OUTFD "\tftpdirectory ${ftpdirectory}\n";
print OUTFD "\tftpmachine ${ftpmachine}\n";
#
foreach $ftptype (keys %filepaths) {
	print OUTFD "\t${ftptype} $filepaths{${ftptype}}\n";
}
print OUTFD "}\n";
#
print OUTFD "web_ftpload exec {\n";
print OUTFD "\tftp open ftpid <ftpmachine> <ftpuser> <ftppasswd>\n";
foreach $ftptype (keys %filepaths) {
	my $filename = basename("$filepaths{${ftptype}}");
	print OUTFD "\tftp put -t900 -b ftpid $filepaths{${ftptype}} ${ftpdirectory}/${filename}\n";
}
print OUTFD "\tftp close ftpid\n";
print OUTFD "}\n";
#
close(OUTFD);
#
print "</pre>\n";
print "</h4>\n";
print "<hr>\n";
#
print "<h4>\n";
print "<pre>\n";
#
open(OUT, ">${upshlogfile}");
open(PIPE, "${upshcmd} 2>&1 |");
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
print "</pre>\n";
print "</h4>\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;



