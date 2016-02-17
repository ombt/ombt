#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$emailaddress = $FORM_DATA{emailaddress};
$prehugcfgfile = $FORM_DATA{prehugcfgfile};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> CPU HUG - ${labid} </h3>\n";
print "<hr>\n";
#
print "<form method=\"post\" action=\"cpuhug4.cgi\">\n";
#
# have user confirm pre-hug cfg file data.
#
print "<h3> Pre-Hug CFG File ${prehugcfgfile} Data <br><hr></h3>\n";
print "</center>\n";
print "<pre>\n";
#
open(PIPE, "/usr/bin/cat ${prehugcfgfile} 2>&1 |");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	if ($urec =~ /cat:.*cannot.*open.*${prehugcfgfile}/i) {
		print "</pre>\n";
		fatalerrormsg(0,"ERROR: prehug cfg file ${prehugcfgfile} does n ot exist.")
	}
	print "${urec}\n";
}
close(PIPE);
STDOUT->autoflush(0);
#
print "</pre>\n";
#
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"emailaddress\" value=\"${emailaddress}\">\n";
print "<input type=hidden name=\"prehugcfgfile\" value=\"${prehugcfgfile}\">\n";
#
print "<hr>\n";
print "<center>\n";
print "<h3> Confirm or enter 'Back' and choose another Pre-HUG CFG file:<br>\n";
print "<input type=submit value=\"Confirm\">\n";
print "<input type=reset value=\"reset\">\n";
print "</h3>\n";
print "</center>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;
