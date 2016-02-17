#!/opt/exp/bin/expect
#
# write a message to a log file.
#
#########################################################################
#
# library
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/logging
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-q] \[-V] logfile message";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-V - enable verbose output";
	puts "	-q - quiet mode";
	puts "";
	puts "write message to given log file.";
	puts "";
}
#
# get cmd line options
#
set verbose 0;
set quiet 0;
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-V} { set verbose 1; }
	{^-q} { set quiet 1; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# check LCSTOOLS variables
#
checkenv
#
# logging on or off?
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
#
# check the number of arguments
#
if {$arg >= $argc} {
	puts "\nInsufficient number of arguments.\n"; 
	usage;
	exit 2;
}
#
# get log file name
#
set logfile [lindex $argv $arg];
#
# logusage $logfile;
#
# get message
#
set msg "";
for {incr arg} {$arg<$argc} {incr arg} {
	set word [lindex $argv $arg];
	append msg $word " ";
}
set msg [string trimright $msg];
#
if { ! $quiet } {
	puts "${logfile} message: <${msg}>\n";
}
logmsg ${logfile} "${msg}";
#
exit 0
