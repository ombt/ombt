#!/opt/exp/bin/perl
#
require "/var/www/cgi-bin/cgiutils.cgi";
#
parse_form_data();
#
$test = $FORM_DATA{test};
$labid = $FORM_DATA{labid};
$searchsp = $FORM_DATA{searchsp};
$searchpart = $FORM_DATA{searchpart};
#
print "Content-type: text/html\n\n";
#
print "<html>\n";
print "<body bgcolor=\"E0E0E0\">\n";
print "<center>\n";
#
print "<h3> GDB </h3>\n";
#
print "<h4>($test, $labid, $searchsp, $searchpart)</h4>\n";
#
print "<form method=\"post\" action=\"gdb3.cgi\">\n";
#
print "<h4>Select a Core File: \n";
print "<select name=corefile>\n";
#
@sps = (spa, spb);
@parts = (root, home1);
#
$spalastreload = "unknown";
$spblastreload = "unknown";
#
foreach my $sp (@sps) {
	# check of we want this sp
	if ($searchsp ne "both" and $sp ne $searchsp) {
		next;
	}
	#
	$lastreloadfile = "/labs/${labid}_${sp}_root/.last_reload";
	if ( -r $lastreloadfile ) {
		$spvar = "${sp}lastreload";
		$${spvar} = `cat ${lastreloadfile}`;
		chomp($${spvar});
	}
	#
	foreach my $part (@parts) {
		# check if we want this partition
		if ($searchpart ne "both" and $part ne $searchpart) {
			next;
		}
		# we want this sp and partition
		if ($part eq "root") {
			$partsubdir = "/Telica";
		} else {
			$partsubdir = "/";
		}
		# search for cores
		open(PIPE, "/usr/bin/find /labs/${labid}_${sp}_${part}${partsubdir} -type f -print | grep -i core |") or fatalerrmsg(0,"Error searching for cores in ${labid}, ${sp}: $!");
		PIPE->autoflush(1);
		#
		while (defined($rec = <PIPE>)) {
			chomp($rec);
			if ($rec =~ /no *such *file *or *directory/i) {
				last;
			}
			print "<option> ${rec}\n";
		}
		#
		close(PIPE);
	}
}
#
print "</select></h4>\n";
#
print "<h4>\n";
print "SP-A Last Reload: ${spalastreload}<br>\n";
print "SP-B Last Reload: ${spblastreload}\n";
print "</h4>\n";
#
if ($spalastreload ne "unknown") {
	$lastreload = $spalastreload;
} elsif ($spblastreload ne "unknown") {
	$lastreload = $spblastreload;
} else {
	$lastreload = "unknown";
	chomp($lastreload);
}
#
print "<h4>Choose Branch/Load:\n";
print "<select name=branchload>\n";
#
open(LOADSFD, "/bin/ls -d /build/lcstools/*/branch |") or fatalerrormsg(0,"ERROR: can't list /build/lcstools/* : $!");
#
while (defined($rec = <LOADSFD>)) {
	chomp($rec);
	#
	$branch = `cat $rec`;
	chomp($branch);
	($d1,$d2,$d3,$load,$f) = split /\//, $rec, 5;
	#
	if ($load =~ /${lastreload}/) {
		print "<option selected> ${branch}__${load}\n";
	} else {
		print "<option> ${branch}__${load}\n";
	}
}
#
close(LOADSFD);
#
print "</select></h4>\n";
#
print "<input type=hidden name=\"test\" value=\"${test}\">\n";
print "<input type=hidden name=\"labid\" value=\"${labid}\">\n";
print "<input type=hidden name=\"searchsp\" value=\"${searchsp}\">\n";
print "<input type=hidden name=\"searchpart\" value=\"${searchpart}\">\n";
#
print "<input type=submit value=\"continue\">\n";
print "<input type=reset value=\"reset\">\n";
#
print "</center>\n";
print "</body>\n";
print "</html>\n";
#
exit 0;


