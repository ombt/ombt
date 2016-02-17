#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
syslog("ptalog", "ptalog4.cgi", "starting");
#
parse_form_data();
#
$ptasessionid = $FORM_DATA{sessionid};
$testcase = $FORM_DATA{sname};
$srecordno = $FORM_DATA{srecordno};
$erecordno = $FORM_DATA{erecordno};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
#
print "<center><h3>PTA Test Case Results:</h3></center><h4>\n";
print "Session ID  : ${ptasessionid}<br>\n";
print "Test Case   : ${testcase}<br>\n";
print "Start Record: ${srecordno}<br>\n";
print "End Record  : ${erecordno}<br>\n";
print "</h4>\n";
#
$ptalogdbname = "${ptasessionid}_ptalog";
$ptalogdb = "/lcsl100/tmp/${ptalogdbname}.testresults";
#
print "<center><h4> TEST ACTION RESULTS: </h4></center>\n";
#
print "<center><table border=2 width=\"100%\" style=\"max-width: 30em;\"><div>\n";
#
print "<tr>\n";
print "<th>time</th>\n";
# print "<th>level</th>\n";
print "<th>type</th>\n";
print "<th>name</th>\n";
print "<th>message</th>\n";
print "<th>srecordno</th>\n";
print "<th>erecordno</th>\n";
print "</tr>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s^%s^%s^%s^%s^%s^%s\\n\" srecordno erecordno etime elevel etype ename emessage in ${ptalogdb} where erecordno ge ${srecordno} and erecordno le ${erecordno} and etype req 'TEST ACTION END' >${ptalogdb}.out$$ 2>&1");
#
open(TMPFD, "${ptalogdb}.out$$");
while (defined($urec = <TMPFD>)) {
	chomp($urec);
	my ($srecordno, $erecordno, $time, $level, $type, $name, $message) = split /\^/, $urec, 7;
	if ($message eq "") {
		$message = "NONE";
	}
	if ($message =~ /FAILURE/) {
		$bgcolor = "bgcolor=\"red\"";
	} else {
		$bgcolor = "";
	}
	print "<tr ${bgcolor}><div>\n";
	print "<td>${time}</td>\n";
	# print "<td>${level}</td>\n";
	print "<td>${type}</td>\n";
	print "<td>${name}</td>\n";
	print "<td><a href=\"ptalog5.cgi?sessionid=${ptasessionid}&testcase=${testcase}\&srecordno=${srecordno}&erecordno=${erecordno}\">${message}</a></td>\n";
	print "<td>${srecordno}</td>\n";
	print "<td>${erecordno}</td>\n";
	print "</div></tr>\n";
}
close(TMPFD);
#
print "</div></table></center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;



