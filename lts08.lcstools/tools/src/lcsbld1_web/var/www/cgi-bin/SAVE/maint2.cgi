#!/opt/exp/bin/perl
#
use File::Basename;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
require "/home/lcstools/public_html/cgi-bin/cgidata.cgi";
#
sub lcspicks {
	my ($moper) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> LCS Picks </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: \n";
	print "<select name=labid>\n";
	print "<option value=\"-a\"> all_labs\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub showlocks {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	#
	print "<center>\n";
	print "<h3> Show Locks: </h3>\n";
	print "</center>\n";
	#
	print "<pre>\n";
	#
	open(CMD, "/home/lcstools/tools/bin/weblcslocks -a 2>&1 |");
	#
	while (defined($urec = <CMD>)) {
		chomp($urec);
		print "${urec}\n";
	}
	#
	close(CMD);
	#
	print "</pre>\n";
	#
	print "</body>\n";
	print "</html>\n";
	#
	return(0);
}
#
sub cpymems {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> CPY-MEM ALL IOMS</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Max Simultaneous CPY-MEMS?\n";
	print "<select name=maxcpymems>\n";
	print "<option> 1\n";
	print "<option> 2\n";
	print "<option selected> 3\n";
	print "<option> 4\n";
	print "<option> 5\n";
	print "<option> 6\n";
	print "<option> 7\n";
	print "<option> 8\n";
	print "<option> 9\n";
	print "<option> 10\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Recover faulted IOMs using update_flash_partition?\n";
	print "<select name=updateioms>\n";
	print "<option selected> yes\n";
	print "<option> no\n";
	print "</select>\n";
	print "</h4>\n";
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
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	return(0);
}
#
sub rmvallioms {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> RMV-EQPT ALL IOMS</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "TL1 User:\n";
	print "<input type=text name=\"tl1user\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "TL1 Passwd:\n";
	print "<input type=text name=\"tl1passwd\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	return(0);
}
#
sub rstallioms {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> RST-EQPT ALL IOMS</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "TL1 User:\n";
	print "<input type=text name=\"tl1user\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "TL1 Passwd:\n";
	print "<input type=text name=\"tl1passwd\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub updpartioms {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> UPDATE IOM FLASH PARTITIONS </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Which flash partitions to update?<br>(minimum set is ife, efe, kdi for ENA and kdi for all non-ENA)<br>\n";
	print "<select name=flashparts>\n";
	print "<option selected> minimum\n";
	print "<option> all\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>IOM states to update?<br>\n";
	print "<input type=\"checkbox\" name=\"all\" value=\"on\"> all <br>or choose the states:<br>\n";
	print "<input type=\"checkbox\" name=\"oosaumaflt\" value=\"on\" checked> oosaumaflt\n";
	print "<input type=\"checkbox\" name=\"oosauflt\" value=\"on\" checked> oosauflt\n";
	print "<input type=\"checkbox\" name=\"oosma\" value=\"on\"> oosma\n";
	print "</h4>\n";
	#
	print "<h4>IOMs to update?<br>\n";
	print "<input type=\"checkbox\" name=\"iomall\" value=\"on\" checked> all <br>or choose the IOMs:<br>\n";
	for (my ${iomno}=1; ${iomno}<=17; ${iomno}++) {
		print "<input type=\"checkbox\" name=\"iom${iomno}\" value=\"on\" > iom-${iomno}\n";
	}
	print "</h4>\n";
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
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub recoversps {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> RECOVER SPS</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Boot SPs?\n";
	print "<select name=bootsps>\n";
	print "<option selected> no\n";
	print "<option> min-mode\n";
	print "<option> app-mode\n";
	print "</select>\n";
	print "</h4>\n";
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
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub auditips {
	my ($moper) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> AUDIT SP IPS</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	print "<option selected> ALL_LABS\n";
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		print "<option> ${rec}\n";
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Fix IPs?\n";
	print "<select name=fixips>\n";
	print "<option> yes\n";
	print "<option selected> no\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub taillogfiles {
	my ($moper) = @_;
	#
	$hugonly = $FORM_DATA{hugonly};
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> TAIL LOG FILES</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	print "<option> NONE\n";
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Type of log file?\n";
	print "<select name=logfiletype>\n";
	foreach my $filetype (sort keys %filedata) {
		if ($hugonly eq "yes") {
			if ($filedata{$filetype}{hug} eq "yes") {
				print "<option> ${filetype}\n";
			}
		} else {
			if ($filetype ne "lcsbu") {
				print "<option> ${filetype}\n";
			} else {
				print "<option selected> ${filetype}\n";
			}
		}
	}
	print "</select></h4>\n";
	print "<h4>How many lines?\n";
	print "<input type=text name=\"lines\" value=\"1000\" size=6>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
sub rtrveqptdata {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> RTRV EQPT DATA</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>TL1 Retrieve Cmd?\n";
	print "<select name=rtrveqptcmd>\n";
	print "<option> rtrv-aag-sub\n";
	print "<option> rtrv-aag-sys\n";
	print "<option> rtrv-aag-unregsub\n";
	print "<option> rtrv-alm-all\n";
	print "<option> rtrv-alm-au3\n";
	print "<option> rtrv-alm-com\n";
	print "<option> rtrv-alm-e1\n";
	print "<option> rtrv-alm-endpt\n";
	print "<option> rtrv-alm-enet\n";
	print "<option> rtrv-alm-eqpt\n";
	print "<option> rtrv-alm-isdnl2\n";
	print "<option> rtrv-alm-isdnl3\n";
	print "<option> rtrv-alm-line\n";
	print "<option> rtrv-alm-link\n";
	print "<option> rtrv-alm-lset\n";
	print "<option> rtrv-alm-oc\n";
	print "<option> rtrv-alm-plk\n";
	print "<option> rtrv-alm-rout\n";
	print "<option> rtrv-alm-secu\n";
	print "<option> rtrv-alm-slk\n";
	print "<option> rtrv-alm-stm\n";
	print "<option> rtrv-alm-sts1\n";
	print "<option> rtrv-alm-t0\n";
	print "<option> rtrv-alm-t1\n";
	print "<option> rtrv-alm-t3\n";
	print "<option> rtrv-alm-trk\n";
	print "<option> rtrv-alm-tu11\n";
	print "<option> rtrv-alm-tu12\n";
	print "<option> rtrv-alm-tug3\n";
	print "<option> rtrv-alm-vt1\n";
	print "<option> rtrv-annc-map\n";
	print "<option> rtrv-areacode\n";
	print "<option> rtrv-attr-log\n";
	print "<option> rtrv-attr-seculog\n";
	print "<option> rtrv-backup-sys\n";
	print "<option> rtrv-billsys\n";
	print "<option> rtrv-bkupsched-eqpt\n";
	print "<option> rtrv-call-captverbose\n";
	print "<option> rtrv-calling-area\n";
	print "<option> rtrv-capt-sys\n";
	print "<option> rtrv-carrier\n";
	print "<option> rtrv-cas-line\n";
	print "<option> rtrv-cas-trk\n";
	print "<option> rtrv-chassis-eqpt\n";
	print "<option> rtrv-cond-all\n";
	print "<option> rtrv-cond-au3\n";
	print "<option> rtrv-cond-com\n";
	print "<option> rtrv-cond-e1\n";
	print "<option> rtrv-cond-endpt\n";
	print "<option> rtrv-cond-enet\n";
	print "<option> rtrv-cond-eqpt\n";
	print "<option> rtrv-cond-isdnl2\n";
	print "<option> rtrv-cond-isdnl3\n";
	print "<option> rtrv-cond-line\n";
	print "<option> rtrv-cond-link\n";
	print "<option> rtrv-cond-lset\n";
	print "<option> rtrv-cond-oc\n";
	print "<option> rtrv-cond-plk\n";
	print "<option> rtrv-cond-rout\n";
	print "<option> rtrv-cond-secu\n";
	print "<option> rtrv-cond-slk\n";
	print "<option> rtrv-cond-stm\n";
	print "<option> rtrv-cond-sts1\n";
	print "<option> rtrv-cond-t0\n";
	print "<option> rtrv-cond-t1\n";
	print "<option> rtrv-cond-t3\n";
	print "<option> rtrv-cond-trk\n";
	print "<option> rtrv-cond-tu11\n";
	print "<option> rtrv-cond-tu12\n";
	print "<option> rtrv-cond-tug3\n";
	print "<option> rtrv-cond-vt1\n";
	print "<option> rtrv-country\n";
	print "<option> rtrv-dataserver-hostcfg\n";
	print "<option> rtrv-def-genie\n";
	print "<option> rtrv-digit-screen\n";
	print "<option> rtrv-digitmod-dn\n";
	print "<option> rtrv-digitmod-gen\n";
	print "<option> rtrv-digitmod-param\n";
	print "<option> rtrv-digitmod-tns\n";
	print "<option> rtrv-dns-sys\n";
	print "<option selected> rtrv-eqpt\n";
	print "<option> rtrv-eqpt-all\n";
	print "<option> rtrv-exsched-eqpt\n";
	print "<option> rtrv-failcnd\n";
	print "<option> rtrv-fan-eqpt\n";
	print "<option> rtrv-fraudcalls\n";
	print "<option> rtrv-gmsc-ansisys\n";
	print "<option> rtrv-gmsc-camelsys\n";
	print "<option> rtrv-gmsc-gsmsys\n";
	print "<option> rtrv-gmsc-laesdn\n";
	print "<option> rtrv-gmsc-winsys\n";
	print "<option> rtrv-gr303-chan\n";
	print "<option> rtrv-gr303-crv\n";
	print "<option> rtrv-gr303-if\n";
	print "<option> rtrv-gr303-lnk\n";
	print "<option> rtrv-hdr\n";
	print "<option> rtrv-inap-sys\n";
	print "<option> rtrv-info-chassis\n";
	print "<option> rtrv-info-eqpt\n";
	print "<option> rtrv-insys\n";
	print "<option> rtrv-iproute\n";
	print "<option> rtrv-isdn-bchan\n";
	print "<option> rtrv-isdn-if\n";
	print "<option> rtrv-isdn-lnk\n";
	print "<option> rtrv-lcc\n";
	print "<option> rtrv-li-screen\n";
	print "<option> rtrv-line-aagsub\n";
	print "<option> rtrv-list-aintrigger\n";
	print "<option> rtrv-list-authcode\n";
	print "<option> rtrv-list-screen\n";
	print "<option> rtrv-list-tollfree\n";
	print "<option> rtrv-list-trigesc\n";
	print "<option> rtrv-list-vmsprefix\n";
	print "<option> rtrv-lnpscreen-digits\n";
	print "<option> rtrv-log\n";
	print "<option> rtrv-lrn\n";
	print "<option> rtrv-mgc-mgassoc\n";
	print "<option> rtrv-mgc-mgiua\n";
	print "<option> rtrv-mgc-pc\n";
	print "<option> rtrv-mgc-pcown\n";
	print "<option> rtrv-mgc-prflcas\n";
	print "<option> rtrv-mgc-prflmegaco\n";
	print "<option> rtrv-mgc-protn\n";
	print "<option> rtrv-mgc-sgassoc\n";
	print "<option> rtrv-mgc-sys\n";
	print "<option> rtrv-msg-user\n";
	print "<option> rtrv-national-npa\n";
	print "<option> rtrv-netype\n";
	print "<option> rtrv-np-pooled\n";
	print "<option> rtrv-np-portedout\n";
	print "<option> rtrv-np-reserved\n";
	print "<option> rtrv-np-sys\n";
	print "<option> rtrv-pm-aag\n";
	print "<option> rtrv-pm-aagsub\n";
	print "<option> rtrv-pm-ain\n";
	print "<option> rtrv-pm-ansi41\n";
	print "<option> rtrv-pm-camel\n";
	print "<option> rtrv-pm-cc\n";
	print "<option> rtrv-pm-db\n";
	print "<option> rtrv-pm-dbtable\n";
	print "<option> rtrv-pm-dns\n";
	print "<option> rtrv-pm-enetsp\n";
	print "<option> rtrv-pm-gmsc\n";
	print "<option> rtrv-pm-gsm\n";
	print "<option> rtrv-pm-inap\n";
	print "<option> rtrv-pm-isdnmsg\n";
	print "<option> rtrv-pm-isupmsg\n";
	print "<option> rtrv-pm-m3uamgc\n";
	print "<option> rtrv-pm-mgcmegacomsg\n";
	print "<option> rtrv-pm-np\n";
	print "<option> rtrv-pm-rc\n";
	print "<option> rtrv-pm-rsrc\n";
	print "<option> rtrv-pm-services\n";
	print "<option> rtrv-pm-sipmsg\n";
	print "<option> rtrv-pm-trkgrp\n";
	print "<option> rtrv-pm-win\n";
	print "<option> rtrv-pmday\n";
	print "<option> rtrv-prfl-failcnd\n";
	print "<option> rtrv-prfl-fraudtrap\n";
	print "<option> rtrv-prfl-insap\n";
	print "<option> rtrv-prfl-is41tmr\n";
	print "<option> rtrv-prfl-isupansi\n";
	print "<option> rtrv-prfl-isupar\n";
	print "<option> rtrv-prfl-isupitu\n";
	print "<option> rtrv-prfl-isupituq767\n";
	print "<option> rtrv-prfl-isupmexico\n";
	print "<option> rtrv-prfl-isuppl\n";
	print "<option> rtrv-prfl-isupve\n";
	print "<option> rtrv-prfl-localtime\n";
	print "<option> rtrv-prfl-origpbx\n";
	print "<option> rtrv-prfl-paramdefaults\n";
	print "<option> rtrv-prfl-paramsuppress\n";
	print "<option> rtrv-prfl-pfp\n";
	print "<option> rtrv-prfl-protrel\n";
	print "<option> rtrv-prfl-sched\n";
	print "<option> rtrv-prfl-sdp\n";
	print "<option> rtrv-prfl-sip\n";
	print "<option> rtrv-prfl-sipt\n";
	print "<option> rtrv-prfl-termpbx\n";
	print "<option> rtrv-prfl-tgbilling\n";
	print "<option> rtrv-rate-center\n";
	print "<option> rtrv-rdt-l2\n";
	print "<option> rtrv-rdt-l3\n";
	print "<option> rtrv-route\n";
	print "<option> rtrv-route-digits\n";
	print "<option> rtrv-route-list\n";
	print "<option> rtrv-route-sched\n";
	print "<option> rtrv-rule-ipfltr\n";
	print "<option> rtrv-sccp-route\n";
	print "<option> rtrv-sccp-ssn\n";
	print "<option> rtrv-service-accesscode\n";
	print "<option> rtrv-sessions\n";
	print "<option> rtrv-sip-route\n";
	print "<option> rtrv-sip-sys\n";
	print "<option> rtrv-sip-tgmapaddr\n";
	print "<option> rtrv-slhr-scp\n";
	print "<option> rtrv-slk\n";
	print "<option> rtrv-snmp-access\n";
	print "<option> rtrv-snmp-comm\n";
	print "<option> rtrv-snmp-sys\n";
	print "<option> rtrv-snmp-trap\n";
	print "<option> rtrv-snmp-user\n";
	print "<option> rtrv-snmp-view\n";
	print "<option> rtrv-ss7-trk\n";
	print "<option> rtrv-state-mgcm3ua\n";
	print "<option> rtrv-status-almq\n";
	print "<option> rtrv-status-capt\n";
	print "<option> rtrv-status-cm\n";
	print "<option> rtrv-status-gr303crv\n";
	print "<option> rtrv-status-gr303if\n";
	print "<option> rtrv-status-mgcproc\n";
	print "<option> rtrv-sub-pbx\n";
	print "<option> rtrv-subscriber-cl4\n";
	print "<option> rtrv-switch-cfg\n";
	print "<option> rtrv-sys-secu\n";
	print "<option> rtrv-sys-tl1\n";
	print "<option> rtrv-tl1-inh\n";
	print "<option> rtrv-tollfree-npa\n";
	print "<option> rtrv-trafficstats\n";
	print "<option> rtrv-trans-plan\n";
	print "<option> rtrv-trans-sched\n";
	print "<option> rtrv-treatment\n";
	print "<option> rtrv-trkgrp\n";
	print "<option> rtrv-user-secu\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Additional TL1 Parameters:<br>\n";
	print "<input type=text name=\"tl1params\" value=\"\" size=30>\n";
	print "</h4>\n";
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
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub nlcsstat {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> LCS STATUS </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Options:<br>Type:\n";
	print "<select name=options>\n";
	print "<option value=\"all\" selected> all\n";
	print "<option value=\"ignoretl1\"> ignore tl1\n";
	print "<option value=\"pingonly\"> ping only\n";
	print "</select>\n";
	#
	print "Verbose: \n";
	print "<input type=\"checkbox\" name=\"verbose\" value=\"on\">\n";
	print "</h4>\n";
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
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub lcsspaudit {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> LCS SP/IOM AUDIT </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>SPs to Audit:\n";
	print "<select name=sps>\n";
	print "<option value=\"both\" selected> both\n";
	print "<option value=\"spa\"> sp-a only\n";
	print "<option value=\"spb\"> sp-b only\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Audit Type:\n";
	print "<select name=type>\n";
	print "<option value=\"all\" selected> all\n";
	print "<option value=\"cpu\"> cpu\n";
	print "<option value=\"knownioms\"> known ioms\n";
	print "<option value=\"allioms\"> all ioms\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Options:<br>\n";
	print "<input type=\"checkbox\" name=\"getcpuload\" value=\"on\"> get cpu load\n";
	print "<input type=\"checkbox\" name=\"ucondtoc\" value=\"on\"> unconditionally build toc\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub lcsreboot {
	my ($moper, $hugonly) = @_;
	#
	CheckPermissions(".allow_reboot");
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> LCS REBOOT </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: \n";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Boot type:\n";
	print "<select name=type>\n";
	print "<option value=\"minmode\"> min-mode (OS-only)\n";
	print "<option value=\"appmode\" selected> app-mode (APPs running)\n";
	print "<option value=\"reboot\"> redo last boot\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>SPs to boot:\n";
	print "<select name=sps>\n";
	print "<option value=\"both\" selected> both\n";
	print "<option value=\"spa\"> SP-A only\n";
	print "<option value=\"spb\"> SP-B only\n";
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Options:<br>\n";
	print "<input type=\"checkbox\" name=\"removesyncxml\" value=\"on\"> remove sync.xml\n";
	print "<input type=\"checkbox\" name=\"removeconfigdata\" value=\"on\"> remove config data\n";
	print "<input type=\"checkbox\" name=\"unconditional\" value=\"on\"> don't run sanity checks\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub bupp {
	my ($moper, $hugonly) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> LCS RELOAD AND PROVISION </h3>\n";
	#
	print "<form method=\"post\" action=\"labs2.cgi\">\n";
	#
	print "<h4>Reload lab: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(LOADSFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <LOADSFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(LOADSFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"reload\" value=\"yes\">\n";
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub gench2ioms {
	my ($moper, $hugonly) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Generate CHASSIS2IOMS </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<hr>\n";
	print "<h4> Choose Lab: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(INFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <INFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(INFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "<br>or,<br>Enter Lab Regular Expression: \n";
	print "<input type=text name=\"labre\" value=\"\" size=20>\n";
	print "<br>or,<br>Choose All Labs: \n";
	print "<input type=\"checkbox\" name=\"laball\" value=\"on\">\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	print "<h4> Choose Branch: ";
	print "<select name=branch>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" branch in /home/lcstools/tools/data/chassis | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(INFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <INFD>)) {
		chomp($rec);
		if ($rec eq "Main") {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(INFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "<br>or,<br>Choose All Branches: \n";
	print "<input type=\"checkbox\" name=\"branchall\" value=\"on\">\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	print "<h4>\n";
	print "TL1 User:\n";
	print "<input type=text name=\"tl1user\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "TL1 Passwd:\n";
	print "<input type=text name=\"tl1passwd\" value=\"telica\" size=12>\n";
	print "</h4>\n";
	#
	print "<hr>\n";
	print "<h3>\n";
	print "Merge results with official CHASSIS2IOMS? <input type=\"checkbox\" name=\"merge\" value=\"on\">\n";
	print "</h3>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub syncfs {
	my ($moper) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Synchronize MA Basescripts </h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: \n";
	print "<select name=labid>\n";
	print "<option> all_labs\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "<br><br>Unconditional Copy? <input type=\"checkbox\" name=\"unconditional\" value=\"on\">\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub ftpload {
	my ($moper, $hugonly) = @_;
	#
	CheckPermissions(".allow_ftpload");
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> FTP Load to a Remote Machine</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4> Branch/Load to copy?\n";
	print "<select name=ftpload>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s %s\\n\" branch cpuload in /home/lcstools/tools/data/loads | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(INFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	while (defined($rec = <INFD>)) {
		chomp($rec);
		print "<option> ${rec}\n";
	}
	close(INFD);
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Remote Machine:\n";
	print "<input type=text name=\"ftpmachine\" value=\"lts08.ih.lucent.com\" size=40>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Remote Machine Directory:\n";
	print "<input type=text name=\"ftpdirectory\" value=\"/tmp\" size=40>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Remote Machine User:\n";
	print "<input type=text name=\"ftpuser\" value=\"telica\" size=15>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Remote Machine Passwd:\n";
	print "<input type=text name=\"ftppasswd\" value=\"plexus9000\" size=15>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"continue\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub bbbscripts {
	my ($moper) = @_;
	#
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Create BBB TL1 Scripts</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: \n";
	print "<select name=bbblabid>\n";
	#
	system("ls /lcsl100/basescripts | grep -v commonfiles| sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ( ! -d "/lcsl100/basescripts/${rec}") {
			next;
		}
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>Operation:\n";
	print "<select name=bbboper>\n";
	print "<option value=\"mcreate\" selected> Manually Choose and Create BBB Scripts\n";
	print "<option value=\"create\" > Automatically Create BBB Scripts\n";
	print "<option value=\"traverse\"> Traverse Only\n";
	print "</select>\n";
	print "</h4>\n";
	#
	# print "<h4>Suffix Type:\n";
	# print "<select name=bbbtype>\n";
	# print "<option value=\"none\" selected> No Suffix\n";
	# print "<option value=\"ITU\"> ITU \n";
	# print "<option value=\"ANSI\"> ANSI \n";
	# print "</select>\n";
	# print "</h4>\n";
	# default value since I commented out the above code.
	print "<input type=hidden name=\"bbbtype\" value=\"none\">\n";
	#
	print "<h4>\n";
	print "Verbose? <input type=\"checkbox\" name=\"bbbverbose\" value=\"on\">\n";
	print "</h4>\n";
	#
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub getsploads {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Get SP Loads</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "SP-A: \n";
	print "<input type=\"checkbox\" name=\"dospa\" value=\"on\">\n";
	print "SP-B: \n";
	print "<input type=\"checkbox\" name=\"dospb\" value=\"on\">\n";
	print "BOTH SPs: \n";
	print "<input type=\"checkbox\" name=\"bothsps\" value=\"on\" checked>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Verbose: \n";
	print "<input type=\"checkbox\" name=\"verbose\" value=\"on\">\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub cleanupsps {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Clean Up SPs</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>SPs to Clean Up: <br>\n";
	print "<input type=\"radio\" name=\"sps\" value=\"spa\">\n";
	print "SP-A: \n";
	print "<input type=\"radio\" name=\"sps\" value=\"spb\">\n";
	print "SP-B: \n";
	print "<input type=\"radio\" name=\"sps\" value=\"both\" checked>\n";
	print "Both: \n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"continue\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub OLDcleanupsps {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Clean Up SPs</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>Lab ID: ";
	print "<select name=labid>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" labid in /home/lcstools/tools/data/chassis where webreload req yes | sort -u >/tmp/data.$$ 2>&1 ");
	#
	open(DATAFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	#
	while (defined($rec = <DATAFD>)) {
		chomp($rec);
		if ($rec eq "ihlcs1") {
			# highlight our machines
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(DATAFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<h4>SPs to Clean Up: <br>\n";
	print "<input type=\"radio\" name=\"sps\" value=\"spa\">\n";
	print "SP-A: \n";
	print "<input type=\"radio\" name=\"sps\" value=\"spb\">\n";
	print "SP-B: \n";
	print "<input type=\"radio\" name=\"sps\" value=\"both\" checked>\n";
	print "Both: \n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Loads To Remove under /Telica/sw*: <br>\n";
	print "<input type=\"checkbox\" name=\"removecurrentcpuloads\" value=\"on\">\n";
	print "Current SP (only in min-mode):\n";
	print "<input type=\"checkbox\" name=\"removeoldcpuloads\" value=\"on\" checked>\n";
	print "Old SP:\n";
	print "<br><input type=\"checkbox\" name=\"removecurrentiomloads\" value=\"on\">\n";
	print "Current IOMs: \n";
	print "<input type=\"checkbox\" name=\"removeoldiomloads\" value=\"on\" checked>\n";
	print "Old IOMs: \n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "Lynx-OS User:\n";
	print "<input type=text name=\"lynxuser\" value=\"root\" size=12>\n";
	print "</h4>\n";
	print "<h4>\n";
	print "Lynx-OS Passwd:\n";
	print "<input type=text name=\"lynxpasswd\" value=\"plexus9000\" size=12>\n";
	print "</h4>\n";
	#
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
sub procstatus {
	my ($moper, $hugonly) = @_;
	print "Content-type: text/html\n\n";
	#
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<h3> Process Status (PS)</h3>\n";
	#
	print "<form method=\"post\" action=\"maint3.cgi\">\n";
	#
	print "<h4>\n";
	print "User ID:\n";
	print "<input type=text name=\"userid\" value=\"nobody\" size=12>\n";
	print "</h4>\n";
	#
	print "<h4>\n";
	print "<input type=hidden name=\"operation\" value=\"${moper}\">\n";
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	print "</h4>\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	return(0);
}
#
parse_form_data();
$operation = $FORM_DATA{operation};
#
my $retval = -2;
if (defined(&{${maintmenu}{${operation}}{handler}})) {
	my $maintoper = ${maintmenu}{${operation}}{operation};
	$retval = &{${maintmenu}{${operation}}{handler}}($maintoper);
} else {
	fatalerrormsg(1,"ERROR: Unknown maintenance operation: ${operation}");
}
#
exit($retval);

