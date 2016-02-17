#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$test = $FORM_DATA{test};
$labid = $FORM_DATA{labid};
$searchsp = $FORM_DATA{searchsp};
$searchpart= $FORM_DATA{searchpart};
$corefile = $FORM_DATA{corefile};
$branchload = $FORM_DATA{branchload};
#
($branch,$load) = split /__/, $branchload, 2;
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> GDB </h3>\n";
#
print "<h4>Labid: $labid <br>\n";
print "SP(s): $searchsp <br>\n";
print "Partition(s): $searchpart <br>\n";
print "Core File: $corefile <br>\n";
print "Branch: $branch <br>\n";
print "Load: $load <br>\n";
print "Test: $test <br>\n";
#
chomp($corefile);
if ($corefile eq "") {
	fatalerrormsg(0,"ERROR: core file is not set.");
}
# try to get executable name from core file name. try CM first.
$corefile =~ /^(.*)_CM.*PROC.*INST.*\.core.*/;
$executable = $1;
if ($executable eq "") {
	$corefile =~ /^(.*)\.core.*/;
	$executable = $1;
	if ($executable eq "") {
		$corefile =~ /^(.*)\.oldCore.*/;
		$executable = $1;
		if ($executable eq "") {
			$executable = "unknown";
		}
	}
}
$executable = basename(${executable});
print "Excutable: $executable </h4>\n";
#
print "<form method=\"post\" action=\"gdb4.cgi\">\n";
#
print "<h4>Select a Debug File: \n";
print "<select name=debugfile>\n";
#
$debugdir = "/build/lcstools/${load}/Telica/swCPU";
#
open(PIPE, "/usr/bin/find ${debugdir} -type f -print |") or fatalerrmsg(0,"Error searching for debug files in ${debugdir}: $!");
PIPE->autoflush(1);
#
$found = 0;
#
while (defined($rec = <PIPE>)) {
	chomp($rec);
	if ($rec =~ /no *such *file *or *directory/i) {
		last;
	}
	if ($rec =~ /.*${executable}.*/) {
		print "<option selected> ${rec}\n";
		$found = 1;
	} else {
		print "<option> ${rec}\n";
	}
}
#
close(PIPE);
#
if ( ! $found) {
	# we didn't find a debug file. try the remaining files
	# on the switch.
	if ($corefile =~ /.*_spa_.*/) {
		$sp = "spa";
	} elsif ($corefile =~ /.*_spb_.*/) {
		$sp = "spb";
	} else {
		$sp = "unknown";
	}
	#
	$debugdir = "/labs/${labid}_${sp}_root/Telica/swCPU/*/apps";
	open(PIPE, "/usr/bin/find ${debugdir} -type f -print |") or fatalerrmsg(0,"Error searching for debug files in ${debugdir}: $!");
	PIPE->autoflush(1);
	#
	while (defined($rec = <PIPE>)) {
		chomp($rec);
		if ($rec =~ /no *such *file *or *directory/i) {
			last;
		}
		if ($rec =~ /.*${executable} *$/) {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(PIPE);
	#
	$debugdir = "/labs/${labid}_${sp}_home1/Telica/swCPU/*/apps";
	open(PIPE, "/usr/bin/find ${debugdir} -type f -print |") or fatalerrmsg(0,"Error searching for debug files in ${debugdir}: $!");
	PIPE->autoflush(1);
	#
	while (defined($rec = <PIPE>)) {
		chomp($rec);
		if ($rec =~ /no *such *file *or *directory/i) {
			last;
		}
		if ($rec =~ /.*${executable} *$/) {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(PIPE);
	#
	$debugdir = "/labs/${labid}_${sp}_root/Telica/swCPU/";
	open(PIPE, "/usr/bin/find ${debugdir} -name emf -type f -print |") or fatalerrmsg(0,"Error searching for debug files in ${debugdir}: $!");
	PIPE->autoflush(1);
	#
	while (defined($rec = <PIPE>)) {
		chomp($rec);
		if ($rec =~ /no *such *file *or *directory/i) {
			last;
		}
		if ($rec =~ /.*${executable} *$/) {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(PIPE);
	#
	$debugdir = "/labs/${labid}_${sp}_home1/Telica/swCPU/";
	open(PIPE, "/usr/bin/find ${debugdir} -name emf -type f -print |") or fatalerrmsg(0,"Error searching for debug files in ${debugdir}: $!");
	PIPE->autoflush(1);
	#
	while (defined($rec = <PIPE>)) {
		chomp($rec);
		if ($rec =~ /no *such *file *or *directory/i) {
			last;
		}
		if ($rec =~ /.*${executable} *$/) {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(PIPE);
}
#
print "</select>\n";
print "<br>-- OR --<br>Enter full path to private Debug File residing on lcsbld1:<br>\n";
print "<input type=text name=\"privatedebugfile\" value=\"\" size=70></h4>\n";
#
print "<h4>Analyze Frames? \n";
print "<select name=analyzeframes>\n";
print "<option> yes\n";
print "<option selected> no\n";
print "</select>\n";
print "</h4>\n";
#
print "<h4>Analyze Threads? \n";
print "<select name=analyzethreads>\n";
print "<option> yes\n";
print "<option selected> no\n";
print "</select>\n";
print "</h4>\n";
#
print "<input type=hidden name=\"test\" value=\"${test}\">\n";
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"searchsp\" value=\"${searchsp}\">\n";
print "<input type=hidden name=\"searchpart\" value=\"${searchpart}\">\n";
print "<input type=hidden name=\"corefile\" value=\"${corefile}\">\n";
print "<input type=hidden name=\"branch\" value=\"${branch}\">\n";
print "<input type=hidden name=\"load\" value=\"${load}\">\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


