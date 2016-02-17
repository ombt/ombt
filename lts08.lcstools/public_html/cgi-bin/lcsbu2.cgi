#!/opt/exp/bin/perl
#
use Socket;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/lcsbu.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
($branch, $load) = split / /, $FORM_DATA{load};
if (!defined($branch) || $branch eq  "") {
	fatalerrormsg(1,"ERROR: unknown branch.");
}
if (!defined($load) || $load eq  "") {
	fatalerrormsg(1,"ERROR: unknown load.");
}
$mode = $FORM_DATA{mode};
$activesp = $FORM_DATA{activesp};
$provision = $FORM_DATA{provision};
$month = $FORM_DATA{month};
$day = $FORM_DATA{day};
$hour = $FORM_DATA{hour};
$hug = $FORM_DATA{hug};
$emailaddress = $FORM_DATA{emailaddress};
chomp($emailaddress);
$emailaddress =~ s/^[\s,;:]*//;
$emailaddress =~ s/[\s,;:]*$//;
$emailaddress =~ s/[\s,;:]+/ /g;
$emailaddress =~ s/ /,/g;
$updateioms = $FORM_DATA{updateioms};
#
$tl1user = $FORM_DATA{tl1user};
$tl1passwd = $FORM_DATA{tl1passwd};
$lynxuser = $FORM_DATA{lynxuser};
$lynxpasswd = $FORM_DATA{lynxpasswd};
#
if (${provision} eq "yes" || ${provision} eq "only") {
	provision(${provision}, $labid, $branch, $load, $mode, $activesp, ${month}, ${day}, ${hour}, ${hug}, ${emailaddress}, ${updateioms}, ${tl1user}, ${tl1passwd}, ${lynxuser}, ${lynxpasswd});
} else {
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	print "<h3> Reloading with NO provisioning: (${labid}, ${branch}, ${load}, ${mode}, ${activesp}, ${provision}, ${month}, ${day}, ${hour}, ${emailaddress} ${updateioms}) </h3>\n";
	#
	if (${mode} eq "simplex") {
		$activesp = "-" . substr($activesp, -1, 1);
	} else {
		$activesp = "";
	}
	#
	if (${updateioms} eq "yes") {
		$updateioms = "-F";
	} else {
		$updateioms = "";
	}
	#
	if (${month} eq "now") {
		$starttime = 'now';
	} else {
		$starttime = "${month}${day}${hour}";
	}
	my $hostname = gethostbyaddr(inet_aton($ENV{REMOTE_ADDR}), AF_INET);
	if (!defined($hostname) || $hostname eq "") {
		$hostname = "UNKNOWN";
	}
	#
	if (!defined($emailaddress) || $emailaddress eq "") {
		$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -R \'${hostname}\' -S \'${starttime}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${labid}";
	} else {
		$cmd = "/home/lcstools/tools/hug/bin/buppx -b ${branch} -c ${load} -i ${mode} ${activesp} ${updateioms} -G -R \'${hostname}\' -S \'${starttime}\' -m \'${emailaddress}\' -1 \'${tl1user}\' -2 \'${tl1passwd}\' -3 \'${lynxuser}\' -4 \'${lynxpasswd}\' ${labid}";
	}
	#	
	print "<h3> Reload Command: ${cmd} </h3>\n";
	#
	finalverification("Start Reload?", "Use \'lcsbu -t ${labid}\' to monitor reload progress.", ${cmd});
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
}
#
exit 0;

