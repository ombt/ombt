#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$labid = $FORM_DATA{labid};
$branch = $FORM_DATA{branch};
$ftpmachine = $FORM_DATA{ftpmachine};
$ftpuser = $FORM_DATA{ftpuser};
$ftppasswd = $FORM_DATA{ftppasswd};
$tl1user = $FORM_DATA{tl1user};
$tl1passwd = $FORM_DATA{tl1passwd};
$lynxuser = $FORM_DATA{lynxuser};
$lynxpasswd = $FORM_DATA{lynxpasswd};
$filter = $FORM_DATA{filter};
$emailaddress = $FORM_DATA{emailaddress};
$maswitchid = $FORM_DATA{maswitchid};
#
if ($filter eq "yes") {
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" type in /home/lcstools/tools/data/chassis2ioms where labid req \"^${labid}\$\" and branch req \"^${branch}\$\" | sort -u >/tmp/ch2ioms.$$ 2>/dev/null ");
	if ( -s "/tmp/ch2ioms.$$" ) {
		open(CH2IOMSFD, "/tmp/ch2ioms.$$") or die "can't open /tmp/ch2ioms.$$";
		while (defined($iom = <CH2IOMSFD>)) {
			chomp($iom);
			$ch2ioms{$iom} = "yes";
		}
		close(CH2IOMSFD);
	} else {
		# try reading the parent branch data.
		$parentbranch = "";
		system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" base in /home/lcstools/tools/data/basebranch where branch req \"^${branch}\$\" and type req \"^tl1\$\" >/tmp/bb.$$ 2>/dev/null ");
		if ( -s "/tmp/bb.$$" ) {
			open(BBFD, "/tmp/bb.$$") or die "can't open /tmp/bb.$$";
			while (defined($parentbranch = <BBFD>)) {
				chomp($parentbranch);
				last;
			}
			close(BBFD);
		}
		unlink("/tmp/bb.$$");
		#
		if (length($parentbranch) > 0) {
			# try again using the parent branch
			system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" type in /home/lcstools/tools/data/chassis2ioms where labid req \"^${labid}\$\" and branch req \"^${parentbranch}\$\" | sort -u >/tmp/ch2ioms.$$ 2>/dev/null ");
			if ( -s "/tmp/ch2ioms.$$" ) {
				open(CH2IOMSFD, "/tmp/ch2ioms.$$") or die "can't open /tmp/ch2ioms.$$";
				while (defined($iom = <CH2IOMSFD>)) {
					chomp($iom);
					$ch2ioms{$iom} = "yes";
				}
				close(CH2IOMSFD);
			} else {
				$filter = "no";
			}

		} else {
			$filter = "no";
		}
	}
	unlink("/tmp/ch2ioms.$$");
}
# always need the cpu
$ch2ioms{cpu} = "yes";
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Pre-HUG: ${labid}/${branch}</h3>\n";
print "</center>\n";
#
print "<form method=\"post\" action=\"prehug3.cgi\">\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" type in /home/lcstools/tools/data/images where branch req \"^${branch}\$\" | sort -u >/tmp/types.$$ 2>&1 ");
#
print "<h4>Choose the TAR files to FTP to switch.\n";
print "Choose only the ones you need.<br><br>\n";
print "Use the drop-down menus if the FTP machine is the default machine of lts08.ih.lucent.com and you are using the official tar files.\n";
print "Give the full path to the tar file if you are using private tar files located on lts08, OR if the FTP machine is not lts08.<br>\n";
print "</h4>\n";
#
$iomtypes = "";
#
print "<h4>\n";
open(TYPESFD, "/tmp/types.$$") or die "can't open /tmp/types.$$";
while (defined($iomtype = <TYPESFD>)) {
	chomp($iomtype);
	#
	if ($iomtype eq "bts" || $iomtype eq "tca" || 
	    $iomtype eq "debug_cpu") {
		next;
	}
	if ($filter eq "yes") {
		if ($ch2ioms{$iomtype} ne "yes") {
			next;
		}
	}
	#
	print "<hr>\n";
	print "Choose ${iomtype} tar file:\n";
	print "<select name=${iomtype}tarfilename>\n";
	print "<option selected> NONE\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" name in /home/lcstools/tools/data/images where branch req \"^${branch}\$\" and type req \"^${iomtype}\$\" | sort -u >/tmp/names.$$ 2>&1 ");
	#
	open(NAMESFD, "/tmp/names.$$") or die "can't open /tmp/names.$$";
	while (defined($name = <NAMESFD>)) {
		chomp($name);
		print "<option> ${name}\n";
	}
	close(NAMESFD);
	unlink("/tmp/names.$$");
	#
	print "</select>\n";
	print "<br>Or enter full path to ${iomtype} tar file:\n";
	print "<input type=text name=\"${iomtype}tarfilepath\" value=\"\" size=40>\n";
	#
	$iomtypes .= ",${iomtype}";
}
$iomtypes =~ s/^,*//;
#
print "<hr><hr>\n";
print "</h4>\n";
close(TYPESFD);
unlink("/tmp/types.$$");
#
print "<center>\n";
print "<h3>Choose type of pre-HUG script to generate?\n";
print "<select name=scripttype>\n";
print "<option value=\"full\" selected> script follows DLPs exactly (slow)\n";
print "<option value=\"lite\"> script only contains TL1-base FTP/TAR (not so slow)\n";
print "<option value=\"direct\"> script calls FTP and TAR directly (fastest)\n";
print "</select>\n";
print "</h3>\n";
#
print "<h3>Choose operation?\n";
print "<select name=prehugoper>\n";
print "<option value=\"genonly\"selected> Generate pre-HUG script\n";
print "<option value=\"genandverify\"> Generate and verify pre-HUG script\n";
print "<option value=\"genandexec\"> Generate and execute pre-HUG script\n";
print "</select>\n";
print "</h3>\n";
#
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"branch\" value=\"${branch}\">\n";
print "<input type=hidden name=\"ftpmachine\" value=\"${ftpmachine}\">\n";
print "<input type=hidden name=\"ftpuser\" value=\"${ftpuser}\">\n";
print "<input type=hidden name=\"ftppasswd\" value=\"${ftppasswd}\">\n";
print "<input type=hidden name=\"tl1user\" value=\"${tl1user}\">\n";
print "<input type=hidden name=\"tl1passwd\" value=\"${tl1passwd}\">\n";
print "<input type=hidden name=\"lynxuser\" value=\"${lynxuser}\">\n";
print "<input type=hidden name=\"lynxpasswd\" value=\"${lynxpasswd}\">\n";
print "<input type=hidden name=\"iomtypes\" value=\"${iomtypes}\">\n";
print "<input type=hidden name=\"emailaddress\" value=\"${emailaddress}\">\n";
print "<input type=hidden name=\"maswitchid\" value=\"${maswitchid}\">\n";
#
print "<input type=submit value=\"submit\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
