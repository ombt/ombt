#!/opt/exp/bin/expect
#
source $env(LCSTOOLSLIB)/lock
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/logging
#
proc usage { } {
	global argv0;
	puts "";
	puts "usage: [file tail $argv0] \[-\?] \[-t tracelevel] \[-L \[-a]] \[-V|-P] locktype resource";
	puts "";
	puts "where:";
	puts "";
	puts "\t-? - print usage message";
	puts "\t-t tracelevel - set trace to given stack level";
	puts "\t-P - lock the given resource";
	puts "\t-V - release the given resource";
	puts "\t-L - list current locks (default operation)";
	puts "\t-a - list all details for current locks (used only with -L)";
}
#
########################################################################
#
set oper "L";
set all 0;
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
	{^-a} { set all 1; }
	{^--} { incr arg; break; }
	{^-.*} { puts "unknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
checkenv;
#
logusage lockadm;
#
if {($arg+1)<$argc} {
	set locktype [lindex $argv $arg];
	set resource [lindex $argv [incr arg] ];
} elseif {$oper != "L"} {
	puts "locktype or resource were not given.";
	usage;
	logmsg lockadm "END: locktype or resource were not given.";
	exit 2;
}
#
if {$oper == "V"} {
	set status [V $locktype $resource];
	if {$status} {
		puts "Deleting LOCK ($locktype $resource) succeeded ...";
		logmsg lockadm "END: Deleting LOCK ($locktype $resource) succeeded ...";
	} else {
		puts "Deleting LOCK ($locktype $resource) failed ...";
		logmsg lockadm "END: Deleting LOCK ($locktype $resource) failed ...";
		exit 2;
	}
} elseif {$oper == "P"} {
	set status [P $locktype $resource ];
	if {$status} {
		puts "Creating LOCK ($locktype $resource) succeeded ...";
		logmsg lockadm "END: Creating LOCK ($locktype $resource) succeeded ...";
	} else {
		puts "Creating LOCK ($locktype $resource) failed ...";
		logmsg lockadm "END: Creating LOCK ($locktype $resource) failed ...";
		exit 2;
	}
} elseif {$oper == "L"} {
	set status [L ${all}];
	if {$status} {
		puts "\nListing Locks succeeded ...";
		logmsg lockadm "END: Listing Locks succeeded ...";
	} else {
		puts "\nListing Locks failed ...";
		logmsg lockadm "END: Listing Locks failed ...";
		exit 2;
	}
} else {
	puts "unknown lock operation: $oper.";
	logmsg lockadm "END: unknown lock operation: $oper.";
	exit 2;
}
#
exit 0;

