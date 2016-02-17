#!/opt/exp/bin/tclsh
#
# update IOMs in a lab using update_flash_partition
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
package require ncleis
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
	#
	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-s trace level]";
	puts "	\[-u TL1 username] \[-p TL1 passwd] \[-U username] \[-P passwd]";
	puts "	\[-A] \[+S | -S iomstate \[,iomstate2 ,...]] \[-I iom1 \[,iom2 ,...]] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-s tracelevel - set trace to given stack level";
	puts "	-u username - TL1 login (default is telica)";
	puts "	-p passwd - TL1 passwd (default is telica)";
	puts "	-U username - Lynx-OS login (default is root)";
	puts "	-P passwd - Lynx-OS passwd (default is plexus9000)";
	puts "	+S - disable IOM states checks";
	puts "	-S state,... - list of IOM states (default is oosaumaflt; ";
	puts "	               use +S to disable checks).";
	puts "	-I iom1,... - list of IOMs to update.";
	puts "	-A - update all flash partitions.";
	puts "";
	puts "update all IOMS with the given states or from the given list";
	puts "of IOMs. by default, the list of IOMs to update is determined ";
	puts "by comparing each IOM state against the the default state ";
	puts "of oosaumaflt. labid is determined from the command line or ";
	puts "from the environment variable LABID.";
	puts "";
	puts "by default, updateioms updates a minimal set of flash ";
	puts "partitions; it updates the ife, efe, and kdi for ENAs, and kdi ";
	puts "for non-ENA IOMs. if you wish to update all flash partitions, ";
	puts "then you must use the '-A' option.";
	puts "";
	puts "examples:";
	puts "\n1) to update all IOMs you need to turn off IOM state checks:";
	puts "	$ updateioms +S";
	puts "\n2) to update faulted IOMs (oosaumaflt) that were manually removed (default):";
	puts "	$ updateioms";
	puts "\n3) to update IOMs 1 and 3, regardless of state:";
	puts "	$ updateioms -I 1,3 +S ";
	puts "\n4) to update IOMs that are standby cold:";
	puts "	$ updateioms -S isstbyc";
	puts "\n5) to update all flash partitions on IOMs that are manually OOS:";
	puts "	$ updateioms -A -S oosma";
	puts "\n6) to update all flash partitions on IOMs 14 and 15:";
	puts "	$ updateioms +S -A -I 14,15";
	puts "\n7) to update all flash partitions on all IOMs:";
	puts "	$ updateioms +S -A";
	puts "";
}
#
#########################################################################
#
# declarations and default values.
#
global env;
set stracelevel -1;
set tl1username "telica";
set tl1passwd "telica";
set lynxusername "root";
set lynxpasswd "plexus9000";
set iomstates "oosaumaflt";
set iomlist "";
set allflag 0;
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
	{^-A} {
		# update all partitions
		set allflag 1;
	}
	{^-s.*} {
		# set function/stack trace level
		getoptval $argval stracelevel arg;
		if {$stracelevel >= 0} {
			strace $stracelevel;
		}
	}
	{^-u.*} {
		# get TL1 user name
		getoptval $argval tl1username arg;
	}
	{^-p.*} {
		# get TL1 passwd
		getoptval $argval tl1passwd arg;
	}
	{^-U.*} {
		# get Lynx-OS user name
		getoptval $argval lynxusername arg;
	}
	{^-P.*} {
		# get Lynx-OS passwd
		getoptval $argval lynxpasswd arg;
	}
	{^-I.*} {
		# list of ioms
		getoptval $argval iomlist arg;
	}
	{^\+S} {
		# turn off iom states checks
		set iomstates "*";
	}
	{^-S.*} {
		# list of iom states
		getoptval $argval iomstates arg;
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
		puts "\nunknown option: $argval\n";
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
set logfile "/lcsl100/tmp/[file tail $argv0].[pid]";
puts "\nDefault logfile is ${logfile}.";
if {[catch {log_file -noappend -a $logfile; } status]} {
	puts "\nWARNING: Logging failed for file $logfile:\n$status";
}
catch { system /usr/bin/chmod 777 $logfile; } ignore;
#
set status [initCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: initCleiData Failed: \n$status";
	exit 2;
}
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
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv for $labid.\""; } ignore;
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
set sp_a_ip [lindex $ipdata 0];
set sp_b_ip [lindex $ipdata 1];
#
puts "";
puts "Updating IOMs for LABID ${labid}:"
puts "";
puts "SP-A IP    : ${sp_a_ip}";
puts "SP-B IP    : ${sp_b_ip}";
puts "TL1 Login  : ${tl1username}";
puts "TL1 Passwd : ${tl1passwd}";
puts "Lynx Login : ${lynxusername}";
puts "Lynx Passwd: ${lynxpasswd}";
puts "IOM states : ${iomstates}";
puts "IOM list   : ${iomlist}";
puts "All Flag   : ${allflag}";
#
# update the ioms
#
set status [hw_maint_update_ioms ${labid} ${sp_a_ip} ${sp_b_ip} ${tl1username} ${tl1passwd} ${lynxusername} ${lynxpasswd} "${iomstates}" "${iomlist}" ${allflag} ];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_update_ioms failed: \n${status}";
	exit 2;
}
#
# all done
#
puts "\nDone.";
exit 0;
