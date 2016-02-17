#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
package require lock
package require getoptval
package require checkenv
#
proc usage { } {
	global argv0;
	puts "";
	puts "usage: [file tail $argv0] \[-\?] \[-t tracelevel] \[-L] \[-V|-P] locktype unique_label";
	puts "";
	puts "where:";
	puts "";
	puts "\t-? - print usage message";
	puts "\t-t tracelevel - set trace to given stack level";
	puts "\t-P - lock the given resource";
	puts "\t-V - release the given resource";
	puts "\t-L - list current locks (default operation)";
}
#
########################################################################
#
set oper "L";
log_user 1;
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-t.*} {
		getoptval $argval stracelevel arg;
		if {$stracelevel >= 0} {
			strace $stracelevel;
		}
	}
	{^-\?} { usage; exit 0; }
	{^-L} { set oper "L"; }
	{^-P} { set oper "P"; }
	{^-V} { set oper "V"; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: unknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
checkenv;
#
global argv0;
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
if {($arg+1)<$argc} {
	set locktype [lindex $argv $arg];
	set label [lindex $argv [incr arg] ];
} elseif {$oper != "L"} {
	puts "\nERROR: locktype and/or lab were not given.";
	usage;
	exit 2;
}
#
if {$oper == "V"} {
	set status [V $locktype $label];
	if {$status} {
		puts "V($locktype $label) succeeded ...";
	} else {
		puts "V($locktype $label) failed ...";
	}
} elseif {$oper == "P"} {
	set status [P $locktype $label ];
	if {$status} {
		puts "P($locktype $label) succeeded ...";
	} else {
		puts "P($locktype $label) failed ...";
	}
} elseif {$oper == "L"} {
	set status [L];
	if {$status} {
		puts "L() succeeded ...";
	} else {
		puts "L() failed ...";
	}
} else {
	puts "\nERROR: unknown lock operation: $oper.";
	exit 2;
}
#
exit 0;
