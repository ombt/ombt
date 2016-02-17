#!/opt/exp/bin/perl
#
require "/home/lcstools/public_html/cgi-bin/cgiutils.cgi";
#
syslog("ptalog", "ptalog3.cgi", "starting");
#
parse_form_data();
#
$ptauser = $FORM_DATA{ptauser};
$ptasession = $FORM_DATA{ptasession};
$bldptadb = $FORM_DATA{bldptadb};
$statusfilter = $FORM_DATA{statusfilter};
#
$ptasessionid = "${ptauser}_${ptasession}";
$ptalogdbname = "${ptauser}_${ptasession}_ptalog";
$ptalogdb = "/lcsl100/tmp/${ptalogdbname}";
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"FFFFFF\">\n";
print "<center>\n";
#
print "<h3> PTA Log Files - ${ptauser}, ${ptasession}, ${statusfilter}, ${bldptadb} </h3>\n";
#
print "<h4>\n";
print "PTA Session ID: <br>${ptasessionid}<br>\n";
print "PTA Unity DB File: <br>${ptalogdb}\n";
print "</h4>\n";
#
# print "<pre>\n";
open(CMD, "/usr/bin/ls /tester/log/${ptasessionid}/${ptasessionid}*.htm 2>&1 |");
while (defined($urec = <CMD>)) {
	chomp($urec);
	# print "${urec}\n";
	if (${urec} !~ /XMLGEN/) {
		$ptalogfile = "${urec}";
		last;
	}
}
close(CMD);
#
print "<h4>\n";
print "PTA Log File: <br>${ptalogfile}<br>\n";
print "</h4>\n";
#
if ($bldptadb eq "yes" || ! -f "${ptalogdb}") {
	print "<h4> Building Unity DB from PTA log file: </h4>\n";
	print "</center><pre>\n";
	#
	open(PIPE, "/home/lcstools/tools/bin/ptalog.parse -d /lcsl100/tmp ${ptalogdbname} ${ptalogfile} |");
	#
	PIPE->autoflush(1);
	STDOUT->autoflush(1);
	#
	while (defined($urec = <PIPE>)) {
		chomp($urec);
		if ($urec =~ /no *such *file *or *directory/i) {
			print "${urec}\n";
			last;
		}
		print "${urec}\n";
	}
	close(PIPE);
	STDOUT->autoflush(0);
	print "</pre><center>\n";
} else {
	print "<h4> PTA Unity DB exists: </h4>\n";
}
#
print "<h3> TEST CASE RESULTS:</h3>\n";
#
print "<table border=2 width=\"100%\" style=\"max-width: 300px;\"><div>\n";
#
print "<tr>\n";
print "<th>stime</th>\n";
print "<th>etime</th>\n";
print "<th>sname</th>\n";
print "<th>emessage</th>\n";
print "<th>success</th>\n";
print "<th>fail</th>\n";
print "<th>inprogress</th>\n";
print "<th>other</th>\n";
print "<th>slevel</th>\n";
print "<th>elevel</th>\n";
print "<th>srecordno</th>\n";
print "<th>erecordno</th>\n";
# print "<th>ename</th>\n";
# print "<th>stype</th>\n";
# print "<th>smessage</th>\n";
# print "<th>etype</th>\n";
# print "<th>status</td>\n";
print "</tr>\n";
#
system("/opt/exp/lib/unity/bin/uprintf -q -f\"%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s\\n\" srecordno stime slevel stype sname smessage erecordno etime elevel etype ename emessage status success fail inprog other in ${ptalogdb}.testresults where stype req 'TEST CASE' and etype req 'TEST CASE' >${ptalogdb}.testresults.out$$ 2>&1");
#
my $recordsprinted = 0;
my $passed = 0;
my $failed = 0;
#
open(TMPFD, "${ptalogdb}.testresults.out$$");
while (defined($urec = <TMPFD>)) {
	chomp($urec);
	my ($srecordno, $stime, $slevel, $stype, $sname, $smessage, $erecordno, $etime, $elevel, $etype, $ename, $emessage, $status, $success, $fail, $inprog, $other) = split /\^/, $urec, 17;
	if ($smessage eq "") {
		$smessage = "NONE";
	}
	# check if record matches filter
	if ($emessage !~ /${statusfilter}/) {
		# skip record
		next;
	}
	if ($emessage =~ /FAILURE/) {
		$bgcolor = "bgcolor=\"red\"";
		$failed += 1;
	} else {
		$bgcolor = "";
		$passed += 1;
	}
	#
	print "<tr ${bgcolor}>\n";
	print "<td>${stime}</td>\n";
	print "<td>${etime}</td>\n";
	print "<td align=\"center\"><a href=\"ptalog4.cgi?sessionid=${ptasessionid}&sname=${sname}\&srecordno=${srecordno}&erecordno=${erecordno}\">${sname}</a>\n";
	print "<br><a href=\"ptalog4.1.cgi?sessionid=${ptasessionid}&sname=${sname}\&srecordno=${srecordno}&erecordno=${erecordno}\">details</a></td>\n";
	print "<td>${emessage}</td>\n";
	print "<td>${success}</td>\n";
	print "<td>${fail}</td>\n";
	print "<td>${inprog}</td>\n";
	print "<td>${other}</td>\n";
	print "<td>${slevel}</td>\n";
	print "<td>${elevel}</td>\n";
	print "<td>${srecordno}</td>\n";
	print "<td>${erecordno}</td>\n";
	# print "<td>${ename}</td>\n";
	# print "<td>${stype}</td>\n";
	# print "<td>${smessage}</td>\n";
	# print "<td>${etype}</td>\n";
	# print "<td>${status}</td>\n";
	print "</tr>\n";
	#
	$printedrecords += 1;
}
close(TMPFD);
#
print "</div></table>\n";
print "<h4>TOTAL: ${printedrecords} PASSED: ${passed} FAILED: ${failed}</h4></center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;
