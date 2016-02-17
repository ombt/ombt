#!/opt/exp/bin/expect
#
# tail and log tl1 cmds
#
#########################################################################
#
# libraries
#
source $env(LCSTOOLSLIB)/logging
source $env(LCSTOOLSLIB)/checkenv
#
# functions
#
proc getoptval { argval varname argname } {
	global argv;
	upvar $varname var;
	upvar $argname arg;
	#
	if {[string length $argval] == 2} {
		incr arg;
		set var [lindex $argv $arg];
	} else {
		set var [string range $argval 2 end];
	}
}
#
proc usage { } {
	global argv0;
	global maxlogons;
	global username;
	global userpasswd;
	global ipaddr;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-m max_logon_attempts]";
	puts "              \[-S trace level] \[-l logfile] \[-u username] \[-p passwd] \[-i ip-addr] ";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "";
	puts "	default max_logon_attempts = $maxlogons";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	default ip-addr = $ipaddr";
}
#
#########################################################################
#
# default values
#
set username "tl1tailer";
set userpasswd "87654321";
set ipaddr "127.0.0.1"
set maxlogons 10;
set stracelevel -1;
set logfile "";
#
# get cmd line options
#
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-m.*} { getoptval $argval maxlogons arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-i.*} { getoptval $argval ipaddr arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# check if any arguments left.
#
if {$argc < 1} {
	usage
	exit 2;
}
#
checkenv;
#
# check if logging is enabled.
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -a $logfile; } status]} {
		puts "\nlogging failed for file $logfile:\n$status";
		exit 2;
	}
}
#
# open tl1 interface ...
#
log_user 1;
#
set id "";
set timeout 45;
set tl1port 2361;
set tl1_id "";
#
puts "\nconnecting to machine $ipaddr ...";
for {set i 0} {$i<$maxlogons} {incr i} {
	puts "\nattempt $i to logon machine $ipaddr ...";
	#
	spawn -noecho telnet $ipaddr $tl1port;
	expect {
	"Connected to " {
		puts "\nconnected to $ipaddr at port $tl1port ...";
		set tl1_id $spawn_id;
		break;
	}
	"Connection refused" {
		puts "sleep 2 sec, and try again ..."
		set tl1_id "";
		sleep 2;
	}
	timeout {
		puts "\ntimeout during login ...";
		set tl1_id "";
		exit 2;
	}
	}
}
#
if {([string length $tl1_id] <= 0) || ($i>=$maxlogons)} {
	puts "\nunable to telnet to machine $ipaddr, port $tl1port.";
	exit 2;
}
#
send "\r";
expect {
"Telica>" {
	puts "\nTL1 prompt received ..."
}
timeout {
	puts "\ntimeout trying to get TL1 prompt ...";
	exit 2;
}
}
#
# log tl1 output
#
send ";\r";
#
set timeout -1;
#
expect {
-re "(.*)\r\n" {
		exp_continue;
}
-re "(.*)\n" {
		exp_continue;
}
eof {
	puts "end-of-file ...";
}
timeout {
	puts "time-out ...";
}
}
#
catch { close; wait; } ignore;
#
exit 0
