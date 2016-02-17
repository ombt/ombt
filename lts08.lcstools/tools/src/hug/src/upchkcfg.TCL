#!/opt/exp/bin/tclsh
#
# verify basic sanity of a HUG cfg file.
#
#########################################################################
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
# packages we need 
#
package require Expect
#
# libraries
#
package require checkenv
package require getoptval
package require checkretval
package require cfgdata
package require ncleis
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-V] ";
	puts "	\[-S trace level] \[-l logfile] configuration filename";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-l filename - log file name";
	puts "";
	puts "Configuration file is scanned for basic sanity.";
}
#
#########################################################################
#
# default values
#
set stracelevel -1;
set logfile "";
#
log_user 0;
#
# get cmd line options
#
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { log_user 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: unknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# check environment
checkenv;
#
set status [initCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: initCleiData Failed: \n$status";
	exit 2;
}
#
global argv0;
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
# check if logging is requested.
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nERROR: logging failed for file $logfile:\n$status";
		exit 2;
	}
	catch { system /usr/bin/chmod 777 $logfile; } ignore;
}
#
# check of function trace is requested.
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# check if any arguments left.
#
if {$argc < 1} {
	puts "\nERROR: missing arguments";
	usage
	exit 2;
}
#
# audit every config file given.
#
for { } {$arg<$argc} {incr arg} {
	set cfgfile [lindex $argv $arg];
	#
	puts "\nScanning Configuration File: $cfgfile";
	#
	set status [initCfgData cfgdata];
	if {[isNotOk $status]} {
		puts "\ninitCfgData Failed: \n$status";
		continue;
	} else {
		puts "\ninitCfgData Passed: \n$status";
		printCfgData cfgdata;
	}
	#
	set status [initCmdData cmddata];
	if {[isNotOk $status]} {
		puts "\ninitCmdData Failed: \n$status";
		continue;
	} else {
		puts "\ninitCmdData Passed: \n$status";
		printCmdData cmddata;
	}
	#
	set status [readCfgDataFile cfgdata cmddata $cfgfile 1 ];
	if {[isNotOk $status]} {
		puts "\nreadCfgFile Failed: \n$status";
		continue;
	} else {
		puts "\nreadCfgFile Passed: \n$status";
		printCfgData cfgdata;
		printCmdData cmddata;
	}
	#
	# set status [auditCfgData cfgdata];
	# if {[isNotOk $status]} {
		# puts "\nreadCfgFile Failed: $status";
		# continue;
	# } else {
		# puts "\nauditCfgData Passed: $status";
	# }
}
#
exit 0;
