# lcsbu utilities
#
sub lcsbu {
	my ($labid) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	print "<h3> Reload: ${labid}</h3>\n";
	#
	print "<form method=\"post\" action=\"lcsbu2.cgi\">\n";
	#
	print "<h4> Load?\n";
	print "<select name=load>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s\\n\" branch cpuload in /home/lcstools/tools/data/labloads where labid req \"^${labid}\$\" and status req oper | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(LOADSFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	while (defined($rec = <LOADSFD>)) {
		chomp($rec);
		print "<option> ${rec}\n";
	}
	close(LOADSFD);
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	#
	print "<h4>Chassis mode?\n";
	print "<select name=mode>\n";
	print "<option selected> duplex\n";
	print "<option> simplex\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4> Active SP (only for simplex mode)?\n";
	print "<select name=activesp>\n";
	print "<option selected> SP-A\n";
	print "<option> SP-B\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	#
	print "<h4> Provision?\n";
	print "<select name=provision>\n";
	print "<option selected> yes\n";
	print "<option> no\n";
	print "<option value=\"only\"> clear data and provision only\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4> Use HUG TL1 scripts?\n";
	print "<select name=hug>\n";
	print "<option> yes\n";
	print "<option selected> no\n";
	print "</select>\n";
	print "</h4>\n";
	#
	# turn this off for now.
	#
	print "<input type=hidden name=\"updateioms\" value=\"no\">\n";
	#
	# print "<h4> Do UPDATEIOMS before CPYMEMS?\n";
	# print "<select name=updateioms>\n";
	# print "<option> yes\n";
	# print "<option selected> no\n";
	# print "</select>\n";
	# print "</h4>\n";
	#
	#
	print "<hr>\n";
	#
	print "<h4>\n";
	print "TL1 User:\n";
	print "<input type=text name=\"tl1user\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "TL1 Passwd:\n";
	print "<input type=text name=\"tl1passwd\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	#
	print "<h4> Start time?<br>MM DD hhmm:\n";
	print "<select name=month>\n";
	print "<option selected> now\n";
	for (my ${m}=1; ${m}<=12; ${m}++) {
		my $mm = sprintf("%02d", ${m});
		print "<option> ${mm}\n";
	}
	print "</select>\n";
	print "<select name=day>\n";
	print "<option selected> \n";
	for (my ${d}=1; ${d}<=31; ${d}++) {
		my $dd = sprintf("%02d", ${d});
		print "<option> ${dd}\n";
	}
	print "</select>\n";
	print "<select name=hour>\n";
	print "<option selected> \n";
	for (my ${h}=0; ${h}<24; ${h}++) {
		my $hh = sprintf("%02d", ${h});
		print "<option> ${hh}00\n";
	}
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4> Email addresses for notification:<br>\n";
	print "(Example: jones\@lucent.com smith\@lucent.com)<br>\n";
	print "<input type=text name=\"emailaddress\" size=60>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
	print "<input type=submit value=\"continue\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	return 0;
}
#
sub provision {
	my ($provision, $labid, $branch, $load, $mode, $activesp, $month, $day, $hour, $hug, $emailaddress, $updateioms, $tl1user, $tl1passwd, $lynxuser, $lynxpasswd) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	if (${hug} eq "yes") {
		$scriptsdir = "/lcsl100/HUGscripts/${branch}/${load}/${labid}";
	} else {
		$scriptsdir = "/lcsl100/scripts/${branch}/${load}/${labid}";
	}
	#
	print "<h3> SCRIPTS DIRECTORY: ${scriptsdir}</h3>\n";
	#
	if (-d ${scriptsdir}) {
		chdir(${scriptsdir});
	} else {
		print "<h2> ERROR: Scripts directory ${scriptsdir} does not exist. </h2>\n";
		print "<h2> Hit back button and choose different load or choose no for provision. </h2>\n";
		print "</center>\n";
		print "</body>\n";
		print "</html>\n";
		exit 0;
	}
	#
	if (${provision} eq "only") {
		print "<h3> ONLY Provisioning: ($labid, $branch, $load, $mode, $activesp, $month, $day, $hour, $hug, $emailaddress, $updateioms) </h3>\n";
	} else {
		print "<h3> Reloading with provisioning: ($labid, $branch, $load, $mode, $activesp, $month, $day, $hour, $hug, $emailaddress, $updateioms) </h3>\n";
	}
	print "<ul>\n";
	system("/opt/exp/gnu/bin/find . -type f -depth -maxdepth 1 -print | sed 's?\./??; s?/? ?g;' | cut -d' ' -f1 | sort -u >/tmp/scripts.$$");
	open(SCRIPTFD, "/tmp/scripts.$$") or die "can't open /tmp/scripts.$$";
	print "<h4> Available scripts: </h4>\n";
	my $maxi = 1;
	for ( ; defined($rec = <SCRIPTFD>); ${maxi}++) {
		chomp($rec);
		print "<li> ${rec}\n";
		$scripts[${maxi}] = ${rec};
	}
	close(SCRIPTFD);
	unlink("/tmp/scripts.$$");
	print "</ul>\n";
	#
	if ($maxi == 1) {
		print "<h2> ERROR: No scripts found in directory ${scriptsdir}. </h2>\n";
		print "<h2> Hit back button and choose different load or choose no for provision. </h2>\n";
		print "</center>\n";
		print "</body>\n";
		print "</html>\n";
		exit 0;
	}
	#
	print "<h3> Choose script order:</h3>\n";
	#
	print "<form method=\"post\" action=\"lcsbu3.cgi\">\n";
	#
	for (my ${i}=1; ${i}<${maxi}; ${i}++) {
		print "<h4> Script ${i} to run:\n";
		print "<select name=script${i}>\n";
		print "<option selected> NONE\n";
		for (my ${s}=1; ${s}<${maxi}; ${s}++) {
			print "<option> $scripts[${s}]\n";
		}
		print "</select>\n";
		print "</h4>\n";
	}
	print "<br><br>\n";
	#
	print "<input type=hidden name=\"provision\" value=\"${provision}\">\n";
	print "<input type=hidden name=\"hug\" value=\"${hug}\">\n";
	print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
	print "<input type=hidden name=\"branch\" value=\"${branch}\">\n";
	print "<input type=hidden name=\"load\" value=\"${load}\">\n";
	print "<input type=hidden name=\"mode\" value=\"${mode}\">\n";
	print "<input type=hidden name=\"activesp\" value=\"${activesp}\">\n";
	print "<input type=hidden name=\"maxi\" value=\"${maxi}\">\n";
	print "<input type=hidden name=\"month\" value=\"${month}\">\n";
	print "<input type=hidden name=\"day\" value=\"${day}\">\n";
	print "<input type=hidden name=\"hour\" value=\"${hour}\">\n";
	print "<input type=hidden name=\"emailaddress\" value=\"${emailaddress}\">\n";
	print "<input type=hidden name=\"updateioms\" value=\"${updateioms}\">\n";
	print "<input type=hidden name=\"tl1user\" value=\"${tl1user}\">\n";
	print "<input type=hidden name=\"tl1passwd\" value=\"${tl1passwd}\">\n";
	print "<input type=hidden name=\"lynxuser\" value=\"${lynxuser}\">\n";
	print "<input type=hidden name=\"lynxpasswd\" value=\"${lynxpasswd}\">\n";
	#
	print "<input type=submit value=\"continue\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	return 0;
}
#
1;
