# saveload utilities
#
use Socket;
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
sub saveload {
	#
	# check permissions for this user.
	CheckPermissions(".allow_saveload");
	#
	print "Content-type: text/html\n\n";
	print "<html>\n";
	print "<body bgcolor=\"E0E0E0\">\n";
	print "<center>\n";
	#
	print "<form method=\"post\" action=\"saveload2.cgi\">\n";
	#
	print "<h4>Choose branch for Saveload? ";
	print "<select name=branch>\n";
	#
	system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s\\n\" branch in /home/lcstools/tools/data/buildmachines >/tmp/data.$$ 2>&1 ");
	open(READFD, "/tmp/data.$$") or die "can't open /tmp/data.$$";
	while (defined($rec = <READFD>)) {
		chomp($rec);
		if ($rec eq "Main") {
			print "<option selected> ${rec}\n";
		} else {
			print "<option> ${rec}\n";
		}
	}
	#
	close(READFD);
	#
	unlink("/tmp/data.$$");
	#
	print "</select>\n";
	print "</h4>\n";
	#
	print "<input type=submit value=\"submit\">\n";
	print "<input type=reset value=\"reset\">\n";
	#
	print "</center>\n";
	print "</body>\n";
	print "</html>\n";
	#
	exit 0;
}
#
1;
