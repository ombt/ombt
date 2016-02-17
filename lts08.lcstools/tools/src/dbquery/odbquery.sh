#!/opt/exp/bin/expect
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
#
proc usage { } {
	global argv0;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] relation \[\"where clause\"]";
	puts "\nor:"
	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-l|-D] relation";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-V - enable verbose output";
	puts "	-l - list available relations";
	puts "	-D - list fields for a relation";
	puts "";
	puts "and clause does NOT include the keyword 'where'.";
}
#
proc listrelations { } {
	global env;
	puts "listing available relations.";
	cd $env(LCSTOOLSDATA);
	system "/usr/bin/ls -C D\* | /usr/bin/sed 's/D//g;'";
}
#
proc listfields { relname } {
	global env;
	cd $env(LCSTOOLSDATA);
	if {[string length $relname] == 0} {
		puts "null-length relation name given.";
		return;
	} elseif {[file exists D$relname] == 0} {
		puts "relation $relname does not exist.";
		return;
	} elseif {[file readable D$relname] == 0} {
		puts "relation $relname is not readable.";
		return;
	}
	puts "listing fields for relation $relname.";
	system "/usr/bin/cat D$relname";
}
#
set lflag 0;
set Dflag 0;
set verbose 0;
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { set verbose 1; }
	{^-l} { set lflag 1; }
	{^-D} { set Dflag 1; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
checkenv
#
if {$verbose > 0} {
	log_user 1;
} else {
	log_user 0;
}
#
if {$lflag != 0} {
	listrelations;
	exit 2;
} elseif {$Dflag != 0} {
	set relname [lindex $argv $arg];
	listfields $relname
	exit 2;
}
#
cd $env(LCSTOOLSDATA);
#
if {[expr $argc -$arg] == 1} {
	dbselect tupbuf [lindex $argv $arg];
} elseif {[expr $argc -$arg] == 2} {
	dbselect tupbuf [lindex $argv $arg] [lindex $argv [incr arg]];
} else {
	usage;
	exit 2;
}
#
if {[info exists tupbuf]} {
	set tupcnt [llength $tupbuf];
	for { set itup 0} {$itup<$tupcnt} {incr itup} {
		puts "[lindex $tupbuf $itup]";
		#puts "tup $itup: [lindex $tupbuf $itup]";
	}
}
#
exit 0
