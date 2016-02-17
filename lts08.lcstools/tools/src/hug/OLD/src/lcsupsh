#!/opt/exp/bin/expect
#
# execute scripts to update a switch. 
#
#########################################################################
#
strace 4;
#
# access libraries
#
# set auto_path [linsert $auto_path 0 $env(LCSHUGLIB)]
# puts "[set auto_path]";
#
package require getoptval;
package require logging;
package require checkenv;
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?]";
	puts "	\[-X] \[-S trace level] \[-V] \[-L logfile]";
	puts "	update_script";
	puts "\nwhere:";
	puts "	-? - print usage message";
	puts "	-X - enable TCL debugger";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-L logfile - set logfile name";
	puts "\nIf a log file name is not given, then a name is generated";
	puts "and printed.";
	puts "";
}
#
#########################################################################
#
# default values
#
set verbose 0;
set logfile "";
set stracelevel -1;
#
# get cmd line options
#
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-X} { debug -now; }
	{^-V} { set verbose 1; }
	{^-L.*} { getoptval $argval logfile arg; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# trace level and debugging
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# check variables
#
checkenv
#
logusage lcsupsh;
#
# logging on or off?
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
#
# start logging of all input and output.
# 
if {[string length $logfile] <= 0} {
	set logfile "$env(LCSHUGLOGFILES)/lcsupsh[pid]";
	puts "\nDefault log file is: $logfile";
}
logmsg lcsupsh "Log file name is: $logfile";
if {[catch {log_file -noappend -a $logfile; } status]} {
	logerror lcsupsh "Logging failed for file $logfile:\n$status";
	exit 2;
}
#
# parse and exec script file
#
if {$arg >= $argc} {
	logerror lcsupsh "Script file was not given.";
	exit 2;
}
#
set script [lindex $argv $arg];
#
set status [parsescript $script];
if {$status != 0} {
	logerror lcsupsh "parsescript failed for $script: status=$status.";
	exit 2;
}
set status [execscript $script];
if {$status != 0} {
	logerror lcsupsh "execscript failed for $script: status=$status.";
	exit 2;
}
#
# done
#
logmsg lcsupsh "Exiting lcsupsh.";
#
exit 0
