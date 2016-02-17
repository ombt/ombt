#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$emailaddress = $FORM_DATA{emailaddress};
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
print "<form method=\"post\" action=\"cpuhug3.cgi\">\n";
#
# have user choose the prehug cfg file.
#
print "<h4> Choose Pre-Hug CFG file used to load lab: <br>\n";
print "<select name=prehugcfgfile>\n";
#
open(PIPE, "/usr/bin/ls -lt /lcsl100/tmp/prehug.cfgfile.${labid}.* 2>&1 | sed -n '1,10p' |");
#
PIPE->autoflush(1);
STDOUT->autoflush(1);
#
while (defined($urec = <PIPE>)) {
	chomp($urec);
	if ($urec =~ /no *such *file *or *directory/i) {
		print "</select>\n";
		print "</h4>\n";
		fatalerrormsg(0,"ERROR: no prehug cfg files found for ${labid}.");
	}
	$urec =~ /^(.*) ([^ ]*)$/;
	print "<option value=\"${2}\"> ${2}\n";
}
close(PIPE);
STDOUT->autoflush(0);
#
print "</select>\n";
print "</h4>\n";
#
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"emailaddress\" value=\"${emailaddress}\">\n";
#
print "<hr>\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
