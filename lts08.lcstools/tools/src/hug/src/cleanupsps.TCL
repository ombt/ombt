#!/opt/exp/bin/tclsh
#
# clean up files from one or both sps.
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
package require db
package require checkenv
package require getoptval
package require checkretval
package require hwmaint
#
# signal handler
#
trap {
	global startok;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "\nERROR: Got signal <$signame,$signo>";
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level]";
	puts "	\[-u username] \[-p passwd] \[-C] \[-c] \[-I] \[-i]";
	puts "	\[-A|-B] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-u username - Lynx-OS login (default is root)";
	puts "	-p passwd - Lynx-OS passwd (default is plexus9000)";
	puts "	-A - clean up SP-A";
	puts "	-B - clean up SP-B";
	puts "	-C - remove current CPU files (only in min-mode)";
	puts "	-c - remove old CPU files (default)";
	puts "	-I - remove current IOM files";
	puts "	-i - remove old IOM files (default)";
	puts "";
	puts "if NONE of these options (-C,-c,-I,-i) are given, then";
	puts "-c and -i are assumed (defaults). if neither -A or -B is given,";
	puts "then both SPs are cleaned. Labid is read from the command line";
	puts "or from the shell variable LABID.";
	puts "";
}
#
#########################################################################
#
# declarations and default values.
#
global env;
set stracelevel -1;
set lynxusername "root";
set lynxpasswd "plexus9000";
set spstoclean "sp-a sp-b";
#
set rmvcurrcpuload 0;
set rmvcurriomload 0;
set rmvoldcpuload 0;
set rmvoldiomload 0;
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
	{^-x} {
		# enable TCL debugger
		debug -now;
	}
	{^-V} {
		# turn on verbose
		log_user 1;
	}
	{^-S.*} {
		# set function/stack trace level
		getoptval $argval stracelevel arg;
		if {$stracelevel >= 0} {
			strace $stracelevel;
		}
	}
	{^-C} {
		# remove current cpu load (only in min-mode)
		set rmvcurrcpuload 1;
	}
	{^-c} {
		# remove old cpu loads
		set rmvoldcpuload 1;
	}
	{^-I} {
		# remove current iom load
		set rmvcurriomload 1;
	}
	{^-i} {
		# remove old iom loads
		set rmvoldiomload 1;
	}
	{^-A} {
		# clean SP-A
		set spstoclean "sp-a";
	}
	{^-B} {
		# clean SP-B
		set spstoclean "sp-b";
	}
	{^-u.*} {
		# get Lynx-OS user name
		getoptval $argval lynxusername arg;
	}
	{^-p.*} {
		# get Lynx-OS passwd
		getoptval $argval lynxpasswd arg;
	}
	{^-c.*} {
		# get maximum simultaneous cpy-mems
		getoptval $argval maxcpymems arg;
		if {$maxcpymems <= 0 || $maxcpymems > 10} {
			puts "\nERROR: maxcpymems out of range; allowed range is 1 <= maxcpymems <= 10.";
			exit 2;
		}
	}
	{^-\?} {
		# print help message and quit
		usage; 
		exit 0;
	}
	{^--} {
		# skip and done
		incr arg;
		break;
	}
	{^-.*} {
		puts "\nERROR: unknown option: $argval\n";
		usage; 
		exit 2;
	}
	default {
		# unknown and done
		break;
	}
	}
}
#
# check environment
checkenv;
#
global argv0;
set logfile "/lcsl100/tmp/[file tail $argv0].[pid]";
puts "\nDefault logfile is ${logfile}.";
if {[catch {log_file -noappend -a $logfile; } status]} {
	puts "\nWARNING: Logging failed for file $logfile:\n$status";
}
catch { system /usr/bin/chmod 777 $logfile; } ignore;
#
# get labid 
#
if {$arg<$argc} {
	set labid [lindex $argv $arg];
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
} else {
	puts "\nERROR: LABID is neither set nor given.";
	exit 2;
}
#
global argv0;
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
# get the lab IPs.
#
set status [dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip"];
if {[isNotOk $status]} {
	puts "\nERROR: dbselect of relation 'chassis' failed: \n${status}";
	exit 2;
}
if {![info exists obuf] || [llength $obuf] == 0} {
	puts "\nERROR: No IPs found for labid $labid. Unknown labid.";
	exit 2;
}
#
set ips [lindex $obuf 0];
set ipdata [split $ips " \t"];
set spips(sp-a) [lindex $ipdata 0];
set spips(sp-b) [lindex $ipdata 1];
#
if {!${rmvcurrcpuload} && !${rmvoldcpuload} && !${rmvcurriomload} && !${rmvoldiomload}} {
	puts "\nWARNING: Defaulting to clean up of old CPU and IOM loads.";
	set rmvoldcpuload 1;
	set rmvoldiomload 1;
}
#
puts "";
puts "Clean Up SPs for LABID ${labid}:"
puts "";
puts "SP-A IP     : ${spips(sp-a)}";
puts "SP-B IP     : ${spips(sp-b)}";
puts "Lynx Login  : ${lynxusername}";
puts "Lynx Passwd : ${lynxpasswd}";
puts "SPs to Clean: ${spstoclean}";
#
puts "\nLoads to Delete:";
puts "Current CPU: ${rmvcurrcpuload}";
puts "Old CPU    : ${rmvoldcpuload}";
puts "Current IOM: ${rmvcurriomload}";
puts "Old IOM    : ${rmvoldiomload}";
#
# wipe this sp out !!! 
#
set status [hw_maint_cleanup ${labid} spips ${lynxusername} ${lynxpasswd} ${spstoclean} ${rmvcurrcpuload} ${rmvoldcpuload} ${rmvcurriomload} ${rmvoldiomload}];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_cleanup failed: \n${status}";
	exit 2;
}
#
# all done
#
puts "\nDone.";
exit 0;

