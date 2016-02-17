#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
parse_form_data();
$helpmsg = $FORM_DATA{helpmsg};
$cmd = $FORM_DATA{cmd};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> Cmd \"${cmd}\" started </h3>\n";
print "<h2> ${helpmsg} </h2>\n";
print "</center>\n";
#
system("${cmd} 1>/tmp/out$$ 2>&1");
#
sleep 5;
#
print "<pre>\n";
open(OUTFD, "/tmp/out$$") or die "can't open /tmp/out$$";
while (defined($orec = <OUTFD>)) {
	chomp($orec);
	print "${orec}\n";
}
close(OUTFD);
unlink("/tmp/out$$");
print "</pre>\n";
#
print "</body>\n";
print "</html>\n";
#
exit 0;

