#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$huglabid = $FORM_DATA{huglabid};
$bbblabid = $FORM_DATA{bbblabid};
#
if ($labid eq "NONE" && $huglabid eq "NONE" && $bbblabid eq "NONE") {
	fatalerrormsg(1,"ERROR: All Labids are set to NONE.");
} else {
	$count = 0;
	if ($labid ne "NONE") {
		$count += 1;
	}
	if ($huglabid ne "NONE") {
		$count += 1;
	}
	if ($bbblabid ne "NONE") {
		$count += 1;
	}
	if ($count > 1) {
		fatalerrormsg(1,"ERROR: More than one labid was given. Choose only one.");
	}
}
#
if ($labid ne "NONE") {
	$basetype = "OFC";
	$basedir = "basescripts";
	$baselabid = ${labid};
}
if ($huglabid ne "NONE") {
	$basetype = "HUG";
	$basedir = "HUGbasescripts";
	$baselabid = ${huglabid};
}
if ($bbblabid ne "NONE") {
	$basetype = "BBB";
	$basedir = "bbbbasescripts";
	$baselabid = ${bbblabid};
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> ${basetype} Base Scripts: ${baselabid} </h3>\n";
#
print "<form method=\"post\" action=\"basescripts3.cgi\">\n";
#
print "<select name=basebranch>\n";
#
chdir("/lcsl100/${basedir}/${baselabid}");
system("/opt/exp/gnu/bin/find . -type d -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u | grep -v '^\.\$' >/tmp/base.$$");
#
open(BASEFD, "/tmp/base.$$") or die "can't open /tmp/base.$$";
#
while (defined($brec = <BASEFD>)) {
	chomp($brec);
	print "<option> ${brec}\n";
}
#
close(BASEFD);
#
unlink("/tmp/base.$$");
#
print "</select>\n";
print "<br><br>\n";
#
print "<input type=hidden name=\"basetype\" value=\"${basetype}\">\n";
print "<input type=hidden name=\"basedir\" value=\"${basedir}\">\n";
print "<input type=hidden name=\"baselabid\" value=\"${baselabid}\">\n";
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

