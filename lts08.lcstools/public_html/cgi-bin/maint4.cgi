#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/cgidata.cgi";
#
sub cleanupsps {
	$labid = $FORM_DATA{labid};
	$sps = $FORM_DATA{sps};
	$lynxuser = $FORM_DATA{lynxuser};
	$lynxpasswd = $FORM_DATA{lynxpasswd};
	$maxidx = $FORM_DATA{maxidx};
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"FFFFFF\">\n";
	print "<center>\n";
	#
	print "<h3> Clean Up SPs</h3>\n";
	#
	print "<h4>($labid, $maxidx, $sps, $lynxuser, $lynxpasswd)</h4>\n";
	print "<h4>Files to delete:<br><br>\n";
	#
	$filestoremove = 0;
	$spafilestoremove = 0;
	$spbfilestoremove = 0;
	$spamustbeminmode = 0;
	$spbmustbeminmode = 0;
	#
	for (my $idx=0; $idx<$maxidx; ${idx}++) {
		$data = $FORM_DATA{"path${idx}"};
		if ($data eq "") {
			next;
		}
		($sp, $filepath, $iscurrrel) = split / /, $data, 3;
		if ($sp eq "sp-a") {
			$spafilestoremove += 1;
			if ($iscurrrel eq "CURRREL") {
				$spamustbeminmode = 1;
			}
		} elsif ($sp eq "sp-b") {
			$spbfilestoremove += 1;
			if ($iscurrrel eq "CURRREL") {
				$spbmustbeminmode = 1;
			}
		}
		$filestoremove += 1;
		print "${data}<br>\n";
	}
	print "<br>Total Files to Delete: ${filestoremove}</h4>\n";
	#
	if ($filestoremove <= 0) {
		fatalerrormsg(0,"ERROR: no files chosen.");
	}
	# get SP IPs
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s\\n\" cpu_a_ip cpu_b_ip in /home/lcstools/tools/data/chassis where labid req \"^${labid}\$\" >/tmp/spips.$$ 2>/dev/null ");
	if ( -s "/tmp/spips.$$" ) {
		open(SPIPSFD, "/tmp/spips.$$") or die "can't open /tmp/spips.$$";
		while (defined($spips = <SPIPSFD>)) {
			chomp($spips);
			last;
		}
		close(SPIPSFD);
		unlink("/tmp/spips.$$");
		($spaip, $spbip) = split / /, $spips, 2;
		if (length($spaip) == 0 || length($spbip) == 0) {
			fatalerrormsg(0, "ERROR: ${labid} SP IPs are NULL length.");
		}
	} else {
		unlink("/tmp/spips.$$");
		fatalerrormsg(1, "ERROR: Unable to determine ${labid} SP IPs.");
	}
	#
	print "<h4>SP-A-IP: ${spaip}<br>SP-B-IP: ${spbip}<h4>\n";
	#
	$upshscript = "/lcsl100/tmp/cleanupsps.${labid}.upsh.script.$$";
	$upsh = "/home/lcstools/tools/bin/webupsh";
	$upshcmd = "${upsh} -s web_cleanupsps ${upshscript}";
	#
	print "<hr>\n";
	#
	print "</h4><h4>Cleanup SPs UPSH script: ${upshscript}<br></h4>\n";
	print "</center>\n";
	#
	open(OUTFD, ">${upshscript}") || fatalerrormsg(0,"ERROR: unable to write upsh script: $! \n");
	#
	print "<h4>\n";
	print "<pre>\n";
	#
	print "file script {\n";
	print "\tscriptsdir .\n";
	print "}\n";
	print OUTFD "file script {\n";
	print OUTFD "\tscriptsdir .\n";
	print OUTFD "}\n";
	#
	print "globals data {\n";
	print "\tuser ${lynxuser}\n";
	print "\tpasswd ${lynxpasswd}\n";
	print "\tspaip ${spaip}\n";
	print "\tspbip ${spbip}\n";
	print "}\n";
	print OUTFD "globals data {\n";
	print OUTFD "\tuser ${lynxuser}\n";
	print OUTFD "\tpasswd ${lynxpasswd}\n";
	print OUTFD "\tspaip ${spaip}\n";
	print OUTFD "\tspbip ${spbip}\n";
	print OUTFD "}\n";
	#
	print "web_cleanupsps exec {\n";
	print OUTFD "web_cleanupsps exec {\n";
	#
	if ($spafilestoremove > 0 && ($sps eq "spa" || $sps eq "both")) {
		print "\ttelnet open spaid &lt;spaip> &lt;user> &lt;passwd>\n";
		print OUTFD "\ttelnet open spaid <spaip> <user> <passwd>\n";
		#
		if ($spamustbeminmode == 1) {
			# MUST BE min-mode to delete CurrRel.
			print "\techo VERIFY SP-A IS IN MIN-MODE\n";
			print "\ttelnet exec spaid [[ \$(ps xf | grep -s emf) == 0 ]]\n";
			print "\ttelnet exec spaid [ -x /Telica/swCPU/CurrRel/scripts/telica.rc.network ]\n";
			print OUTFD "\techo VERIFY SP-A IS IN MIN-MODE\n";
			print OUTFD "\ttelnet exec spaid [[ \$(ps xf | grep -s emf) == 0 ]]\n";
			print OUTFD "\ttelnet exec spaid [ -x /Telica/swCPU/CurrRel/scripts/telica.rc.network ]\n";
		}
		#
		for (my $idx=0; $idx<$maxidx; ${idx}++) {
			$data = $FORM_DATA{"path${idx}"};
			if ($data eq "") {
				next;
			}
			($sp, $filepath, $iscurrrel) = split / /, $data, 3;
			if ($sp ne "sp-a") {
				next;
			}
			print "\ttelnet exec spaid /bin/rm -rf ${filepath}\n";
			print OUTFD "\ttelnet exec spaid /bin/rm -rf ${filepath}\n";
		}
		#
		print "\ttelnet close spaid\n";
		print OUTFD "\ttelnet close spaid\n";
	}
	if ($spbfilestoremove > 0 && ($sps eq "spb" || $sps eq "both")) {
		print "\ttelnet open spbid &lt;spbip> &lt;user> &lt;passwd>\n";
		print OUTFD "\ttelnet open spbid <spbip> <user> <passwd>\n";
		#
		if ($spbmustbeminmode == 1) {
			# MUST BE min-mode to delete CurrRel.
			print "\techo VERIFY SP-B IS IN MIN-MODE\n";
			print "\ttelnet exec spbid [[ \$(ps xf | grep -s emf) == 0 ]]\n";
			print "\ttelnet exec spbid [ -x /Telica/swCPU/CurrRel/scripts/telica.rc.network ]\n";
			print OUTFD "\techo VERIFY SP-B IS IN MIN-MODE\n";
			print OUTFD "\ttelnet exec spbid [[ \$(ps xf | grep -s emf) == 0 ]]\n";
			print OUTFD "\ttelnet exec spbid [ -x /Telica/swCPU/CurrRel/scripts/telica.rc.network ]\n";
		}
		#
		for (my $idx=0; $idx<$maxidx; ${idx}++) {
			$data = $FORM_DATA{"path${idx}"};
			if ($data eq "") {
				next;
			}
			($sp, $filepath, $iscurrrel) = split / /, $data, 3;
			if ($sp ne "sp-b") {
				next;
			}
			print "\ttelnet exec spbid /bin/rm -rf ${filepath}\n";
			print OUTFD "\ttelnet exec spbid /bin/rm -rf ${filepath}\n";
		}
		#
		print "\ttelnet close spbid\n";
		print OUTFD "\ttelnet close spbid\n";
	}
	#
	print "}\n";
	print OUTFD "}\n";
	#
	close(OUTFD);
	#
	print "</pre>\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	#
	print "<center>\n";
	print "<h3> Start Command?</h3>\n";
	print "<form method=\"post\" action=\"execcmd2.cgi\">\n";
	print "<input type=hidden name=\"outfile\" value=\"cleanupsps.${labid}\">\n";
	print "<input type=hidden name=\"cmd\" value=\"${upshcmd}\">\n";
	print "<input type=submit value=\"confirm\">\n";
	print "<hr>\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	return(0);
}
#
parse_form_data();
$operation = $FORM_DATA{operation};
#
my $retval = -2;
if (defined(&{${operation}})) {
	$retval = &{${operation}}();
} else {
	fatalerrormsg(1,"ERROR: Unknown maintenance operation: ${operation}");
}
#
exit($retval);

