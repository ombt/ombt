#!/opt/exp/bin/perl
#
use File::Basename;
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
#
if ($bbboper eq "mcreate") {
	# allow user to choose scripts. 
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	print "<h3> MANUAL MAKE BBB TL1 SCRIPTS </h3>\n";
	print "<h4>($bbblabid, $bbbbranch, $bbboper, $bbbtype, $bbbverbose)</h4>\n";
	#
	$scriptsdir = "/lcsl100/basescripts/${bbblabid}/${bbbbranch}/current";
	#
	print "<h3> BASE SCRIPTS DIRECTORY: ${scriptsdir}</h3>\n";
	#
	if (-d ${scriptsdir}) {
		chdir(${scriptsdir});
	} else {
		print "<h2> ERROR: Base scripts directory ${scriptsdir} does not exist. </h2>\n";
		print "<h2> Hit back button and choose different branch. </h2>\n";
		print "</center>\n";
		print "</body>\n";
		print "</html>\n";
		exit 0;
	}
	#
	print "<ul>\n";
	system("/opt/exp/gnu/bin/find . -type f -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u >/tmp/scripts.$$");
	open(SCRIPTFD, "/tmp/scripts.$$") or die "can't open /tmp/scripts.$$";
	print "<h4> Available scripts: </h4>\n";
	my $bbbmaxi = 1;
	for ( ; defined($rec = <SCRIPTFD>); ${bbbmaxi}++) {
		chomp($rec);
		print "<li> ${rec}\n";
		$scripts[${bbbmaxi}] = ${rec};
	}
	close(SCRIPTFD);
	unlink("/tmp/scripts.$$");
	print "</ul>\n";
	#
	if ($bbbmaxi == 1) {
		print "<h2> ERROR: No scripts found in directory ${scriptsdir}. </h2>\n";
		print "<h2> Hit back button and choose different branch. </h2>\n";
		print "</center>\n";
		print "</body>\n";
		print "</html>\n";
		exit 0;
	}
	#
	print "<h3> Choose script order:</h3>\n";
	#
	print "<form method=\"post\" action=\"bbbscripts2.cgi\">\n";
	#
	for (my ${i}=1; ${i}<${bbbmaxi}; ${i}++) {
		print "<h4> Script ${i} to run:\n";
		print "<select name=script${i}>\n";
		print "<option selected> NONE\n";
		for (my ${s}=1; ${s}<${bbbmaxi}; ${s}++) {
			print "<option> $scripts[${s}]\n";
		}
		print "</select>\n";
		print "</h4>\n";
	}
	print "<br>\n";
	#
	print "<input type=hidden name=\"bbblabid\" value=\"${bbblabid}\">\n";
	print "<input type=hidden name=\"bbbbranch\" value=\"${bbbbranch}\">\n";
	print "<input type=hidden name=\"bbboper\" value=\"${bbboper}\">\n";
	print "<input type=hidden name=\"bbbtype\" value=\"${bbbtype}\">\n";
	print "<input type=hidden name=\"bbbverbose\" value=\"${bbbverbose}\">\n";
	print "<input type=hidden name=\"bbbmaxi\" value=\"${bbbmaxi}\">\n";
	print "<input type=submit value=\"continue\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	
} else {
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	print "<h3> AUTOMATED MAKE BBB TL1 SCRIPTS </h3>\n";
	#
	print "<h4>($bbblabid, $bbbbranch, $bbboper, $bbbtype, $bbbverbose)</h4>\n";
	#
	$cmd = "/home/lcstools/tools/bin/weblcsbbbscripts";
	#
	if ($bbboper eq "traverse") {
		$cmd .= " +W";
	}
	if ($bbbtype ne "none") {
		$cmd .= " -s ${bbbtype}";
	}
	if ($bbbbranch ne "ALL_BRANCHES") {
		$cmd .= " -b ${bbbbranch}";
	}
	if ($bbbverbose eq "on") {
		$cmd .= " -D";
	}
	$cmd .= " ${bbblabid}";
	#
	print "<h4>CMD: $cmd</h4>\n";
	#
	print "<hr>\n";
	print "<h3> Start Command?</h3>\n";
	print "<form method=\"post\" action=\"execcmd2.cgi\">\n";
	print "<input type=hidden name=\"outfile\" value=\"lcsbbbscripts.${bbblabid}\">\n";
	print "<input type=hidden name=\"cmd\" value=\"${cmd}\">\n";
	print "<input type=submit value=\"confirm\">\n";
	print "<hr>\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
}
#
exit 0;



