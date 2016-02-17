#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$hugtype = $FORM_DATA{hugtype};
$uchugtype = uc(${hugtype});
$labid = $FORM_DATA{labid};
$emailaddress = $FORM_DATA{emailaddress};
$prehugcfgfile = $FORM_DATA{prehugcfgfile};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> ${uchugtype} HUG - ${labid} </h3>\n";
print "<hr>\n";
#
print "<form method=\"post\" action=\"hug4.cgi\">\n";
#
# have user confirm pre-hug cfg file data.
#
my $cfgfile = basename(${prehugcfgfile});
print "<h4> Pre-Hug CFG File ${cfgfile} Data <br><hr></h4>\n";
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
print "<input type=hidden name=\"hugtype\" value=\"${hugtype}\">\n";
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"emailaddress\" value=\"${emailaddress}\">\n";
print "<input type=hidden name=\"prehugcfgfile\" value=\"${prehugcfgfile}\">\n";
#
print "<hr>\n";
print "<center>\n";
print "<h4>Choose operation?\n";
print "<select name=hugoper>\n";
print "<option value=\"genonly\"selected> Generate ${uchugtype}-HUG script\n";
print "<option value=\"genandverify\"> Generate and verify ${uchugtype}-HUG script\n";
print "<option value=\"genandexec\"> Generate and execute ${uchugtype}-HUG script\n";
print "</select>\n";
print "</h4>\n";
print "<h4> Confirm, or enter 'Back' and choose another Pre-HUG CFG file:<br>\n";
print "<input type=submit value=\"Confirm\">\n";
print "<input type=reset value=\"reset\">\n";
print "</h4>\n";
print "</center>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;



