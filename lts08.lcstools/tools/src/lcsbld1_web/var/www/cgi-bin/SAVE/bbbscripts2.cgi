#!/opt/exp/bin/perl
#
use Socket;
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
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> MANUAL MAKE BBB TL1 SCRIPTS: (${bbblabid}, ${bbbbranch}, ${bbboper}, ${bbbtype}, ${bbbverbose}, ${bbbmaxi})</h3>\n";
#
print "<form method=\"post\" action=\"bbbscripts3.cgi\">\n";
#
$calldatasuffix = "";
#
for (my ${i}=1; ${i}<${bbbmaxi} && $FORM_DATA{"script".${i}} ne "NONE"; ${i}++) {
	$bbbscriptname = "script" . ${i};
	$bbbscript = $FORM_DATA{"script" . ${i}};
	#
	print "<input type=hidden name=\"${bbbscriptname}\" value=\"${bbbscript}\">\n";
	#
	if ($bbbscript =~ /calldata/i) {
		$bbbscript =~ /^.*calldata[^\.]*\.*(.*)/i;
		$calldatasuffix = ".${1}";
	}
}
#
$bbbscriptname = "tl1.${bbblabid}.${bbbbranch}${calldatasuffix}";
$bbbscriptpath = "/lcsl100/auto/tmp/";
#
#
print "<h4>\n";
print "BBB TL1 File Path: <br> ${bbbscriptpath}\n";
print "</h4>\n";
#
print "<h4>\n";
print "Default BBB TL1 File Name:<br>\n";
print "<input type=text name=\"bbbscriptname\" value=\"${bbbscriptname}\" size=40>\n";
print "</h4>\n";
print "<hr>\n";
#
print "<h4>\n";
print "Optional FTP: <br><br>Fill in data to FTP the file, or leave empty to skip FTP.\n";
print "</h4>\n";
#
print "<h4>\n";
print "Remote Machine:\n";
print "<input type=text name=\"bbbftpmachine\" value=\"\" size=40>\n";
print "</h4>\n";
print "<h4>\n";
print "Remote Machine Directory:\n";
print "<input type=text name=\"bbbftpdirectory\" value=\"/tmp\" size=40>\n";
print "</h4>\n";
#
print "<h4>\n";
print "Remote Machine User:\n";
print "<input type=text name=\"bbbftpuser\" value=\"\" size=15>\n";
print "</h4>\n";
#
print "<h4>\n";
print "Remote Machine Passwd:\n";
print "<input type=text name=\"bbbftppasswd\" value=\"\" size=15>\n";
print "</h4>\n";
#
print "<br>\n";
print "<hr>\n";
#
print "<input type=hidden name=\"bbblabid\" value=\"${bbblabid}\">\n";
print "<input type=hidden name=\"bbbbranch\" value=\"${bbbbranch}\">\n";
print "<input type=hidden name=\"bbboper\" value=\"${bbboper}\">\n";
print "<input type=hidden name=\"bbbtype\" value=\"${bbbtype}\">\n";
print "<input type=hidden name=\"bbbverbose\" value=\"${bbbverbose}\">\n";
print "<input type=hidden name=\"bbbmaxi\" value=\"${bbbmaxi}\">\n";
print "<input type=hidden name=\"bbbscriptpath\" value=\"${bbbscriptpath}\">\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

