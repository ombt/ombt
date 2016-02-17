#!/opt/exp/bin/perl
#
use Net::FTP;
use Net::Telnet;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/saveload.cgi";
#
parse_form_data();
$branch = $FORM_DATA{branch};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> Saveload for branch: ${branch} </h3>\n";
#
# get branch data
#
($pid = open(DATAFD, "/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s %s %s\\n\" branch machine directory type in /home/lcstools/tools/data/buildmachines 2>&1 |")) or fatalerrormsg(0,"ERROR: can't open pipe to read buildmachines: $!");
#
for ($found=0; !$found && defined($rec = <DATAFD>); ) {
	chomp($rec);
	if ($rec =~ /^${branch} /) {
		$found = 1;
	}
}
close(DATAFD);
#
if (!${found}) {
	fatalerrormsg(0,"ERROR: ${branch} not found in relation 'buildmachines'.");
}
($branch, $machine, $directory, $type) = split /\s+/, $rec, 4;
(defined($branch) && defined($machine) && defined($directory) && defined($type)) || fatalerrormsg(0,"ERROR: invalid ${branch} tuple from 'buildmachines'.");
#
# find branch for BTS
#
if ($branch =~ /^BP-/) {
	open(DATAFD, "/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s %s\\n\" branch type base in /home/lcstools/tools/data/basebranch 2>&1 |") or fatalerrormsg(0,"ERROR: can't open pipe to read basebranch: $!");
	for ($found=0; !$found && defined($rec = <DATAFD>); ) {
		chomp($rec);
		if ($rec =~ /^${branch} tl1 /) {
			$found = 1;
		}
	}
	close(DATAFD);
	if ($found) {
		($dummy, $type, $btsbranch) = split /\s+/, $rec, 3;
		if (!defined($btsbranch)) {
			$btsbranch = "UNKNOWN";
		}
	} else {
		$btsbranch = "UNKNOWN";
	}
} else {
	$btsbranch = ${branch};
}
#
print "<h4> Retrieving data for: \n";
print "<br>Branch: ${branch}\n";
print "<br>Machine: ${machine}\n";
print "<br>Directory: ${directory}\n";
print "<br>Type: ${type}\n";
print "<br>BTS Branch: ${btsbranch}\n";
print "</h4>\n";
#
# ($ftp = Net::FTP->new($machine)) or fatalerrormsg(0,"ERROR: can't ftp to ${machine}: $@");
# $ftp->login('lcstools','plexus9000') or fatalerrormsg(0,"ERROR: can't login for ftp.");
#
# $ftp->binary or fatalerrormsg(0,"ERROR: unable to change to binary mode.");
# $ftp->put("/home/lcstools/tools/bin/load2images", "/tmp/load2images${PID}");
#
# ($telnet = Net::Telnet->new($machine)) or fatalerrormsg(0,"ERROR: can't telnet to ${machine}: $@";
#
# $ftp->quit();
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
