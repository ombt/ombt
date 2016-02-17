#!/opt/exp/bin/perl
#
use Socket;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/lcsbu.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$branch = $FORM_DATA{branch};
$load = $FORM_DATA{load};
$mode = $FORM_DATA{mode};
$activesp = $FORM_DATA{activesp};
$maxi = $FORM_DATA{maxi};
$month = $FORM_DATA{month};
$day = $FORM_DATA{day};
$hour = $FORM_DATA{hour};
$hug = $FORM_DATA{hug};
$emailaddress = $FORM_DATA{emailaddress};
$updateioms = $FORM_DATA{updateioms};
$provision = $FORM_DATA{provision};
#
$tl1user = $FORM_DATA{tl1user};
$tl1passwd = $FORM_DATA{tl1passwd};
$lynxuser = $FORM_DATA{lynxuser};
$lynxpasswd = $FORM_DATA{lynxpasswd};
#
$scripts = "";
#
for (my ${i}=1; ${i}<${maxi} && $FORM_DATA{"script".${i}} ne "NONE"; ${i}++) {
	if ($scripts eq "") {
		$scripts = $FORM_DATA{"script" . ${i}};
	} else {
		$scripts = ${scripts} . "," . $FORM_DATA{"script" . ${i}};
	}
}
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Reloading with provisioning: (${labid}, ${branch}, ${load}, ${mode}, ${activesp}, ${scripts}, ${month}, ${day}, ${hour}, ${hug}) </h3>\n";
#
if ($mode eq "simplex") {
	$activesp = "-" . substr($activesp, -1, 1);
} else {
	$activesp = "";
}
if ($month eq "now") {
	$starttime = 'now';
} else {
	$starttime = "${month}${day}${hour}";
}
if ($updateioms eq "yes") {
	$updateioms = "-F";
} else {
	$updateioms = "";
}
if ($provision eq "only") {
	$provonly = "-p";
} else {
	$provonly = "";
}
#
my $hostname = gethostbyaddr(inet_aton($ENV{REMOTE_ADDR}), AF_INET);
if (!defined($hostname) || $hostname eq "") {
	$hostname = "UNKNOWN";
}
#
if ($scripts ne "") {
	if ($hug eq "yes") {
		if (!defined($emailaddress) || $emailaddress eq "") {
			$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -P -U \'/lcsl100/HUGscripts/${branch}/${load}/${labid}\' -T \'${scripts}\' -R \'${hostname}\' -S \'${starttime}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${provonly} ${labid}";
		} else {
			$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -P -U \'/lcsl100/HUGscripts/${branch}/${load}/${labid}\' -T \'${scripts}\' -R \'${hostname}\' -S \'${starttime}\' -m \'${emailaddress}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${provonly} ${labid}";
		}
	} else {
		if (!defined($emailaddress) || $emailaddress eq "") {
			$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -P -T \'${scripts}\' -R \'${hostname}\' -S \'${starttime}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${provonly} ${labid}";
		} else {
			$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -P -T \'${scripts}\' -R \'${hostname}\' -S \'${starttime}\' -m \'${emailaddress}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${provonly} ${labid}";
		}
	}
} else {
	if (${provision} eq "only") {
		fatalerrormsg(0,"ERROR: ONLY provisioning, but no TL1 scripts were chosen.");
	}
	#
	print "<h2> WARNING: No scripts chosen; provisioning turned off. </h2>\n";
	if (!defined($emailaddress) || $emailaddress eq "") {
		$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -R \'${hostname}\' -S \'${starttime}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${labid}";
	} else {
		$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -R \'${hostname}\' -S \'${starttime}\' -m \'${emailaddress}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${labid}";
	}
}
#
print "<h3> Reload Command: ${cmd} </h3>\n";
#
finalverification("Start Reload?", "Use \'lcsbu -t ${labid}\' to monitor progress.", ${cmd});
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;

