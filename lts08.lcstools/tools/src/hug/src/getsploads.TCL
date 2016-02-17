#!/opt/exp/bin/tclsh
#
# gather load data on a switch, both SPs.
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
# exit handlers
#
exit -onexit {
	puts "\nDone.";
}
#
trap {
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
	puts "	\[-u username] \[-p passwd] \[-A] \[-B] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-x - enter TCL debugger";
	puts "	-V - enable verbose output";
	puts "	-s tracelevel - set trace to given stack level";
	puts "	-u username - Lynx-OS login (default is root)";
	puts "	-p passwd - Lynx-OS passwd (default is plexus9000)";
	puts "	-A - get loads on SP-A";
	puts "	-B - get loads on SP-B";
	puts "";
	puts "determine and list loads under /Telica/sw* for each SP.";
	puts "by default, load data are obtained for both SPs. labid is";
	puts "obtain from the command line or from LABID.";
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
set dospa 0;
set dospb 0;
set labid "";
set verbose 0;
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
		set verbose 1;
	}
	{^-A} {
		# get data for SP-A
		set dospa 1;
	}
	{^-B} {
		# get data for SP-B
		set dospb 1;
	}
	{^-s.*} {
		# set function/stack trace level
		getoptval $argval stracelevel arg;
		if {$stracelevel >= 0} {
			strace $stracelevel;
		}
	}
	{^-u.*} {
		# get Lynx-OS user name
		getoptval $argval lynxusername arg;
	}
	{^-p.*} {
		# get Lynx-OS passwd
		getoptval $argval lynxpasswd arg;
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
# get labid 
#
if {$arg<$argc} {
	for { } {$arg<$argc} {incr arg} {
		lappend labids [lindex $argv $arg];
	}
} elseif {[info exists env(LABID)]} {
	lappend labids $env(LABID);
} else {
	puts "\nERROR: LABID is neither set nor given.";
	exit 2;
}
#
if {!$dospa && !$dospb} {
	# default to both SPs.
	set dospa 1;
	set dospb 1;
}
#
# cyle thru all labids
#
foreach labid ${labids} {
	global argv0;
	global argv;
	catch { system "lcssyslog -q syslog \"Starting $argv0 $argv for $labid.\""; } ignore;
	#
	# get the lab IPs.
	#
	set status [dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip"];
	if {[isNotOk $status]} {
		puts "\nERROR: dbselect of relation 'chassis' failed: \n${status}";
		puts "\nGo on to next LABID.";
		continue;
	}
	if {![info exists obuf] || [llength $obuf] == 0} {
		puts "\nERROR: No IPs found for labid $labid. Unknown labid.";
		puts "\nGo on to next LABID.";
		continue;
	}
	#
	set ips [lindex $obuf 0];
	set ipdata [split $ips " \t"];
	set sp_a_ip [lindex $ipdata 0];
	set sp_b_ip [lindex $ipdata 1];
	#
	puts "";
	puts "Get Load Data for LABID ${labid}:"
	puts "";
	puts "SP-A IP    : ${sp_a_ip}";
	puts "SP-B IP    : ${sp_b_ip}";
	puts "Lynx Login : ${lynxusername}";
	puts "Lynx Passwd: ${lynxpasswd}";
	puts "Do SP-A    : ${dospa}";
	puts "Do SP-B    : ${dospb}";
	#
	# get data for SP-A, if required.
	#
	if {$dospa} {
		set status [hw_maint_getsploads ${labid} ${sp_a_ip} "sp-a" spdata ${lynxusername} ${lynxpasswd} ${verbose} ];
		if {[isNotOk $status]} {
			puts "\nERROR: hw_maint_getsploads failed for SP-A: \n${status}";
			puts "\nGo on to next LABID.";
			if {[info exists spdata]} {
				unset spdata;
			}
			continue;
		}
	}
	if {$dospb} {
		set status [hw_maint_getsploads ${labid} ${sp_b_ip} "sp-b" spdata ${lynxusername} ${lynxpasswd} ${verbose} ];
		if {[isNotOk $status]} {
			puts "\nERROR: hw_maint_getsploads failed for SP-B: \n${status}";
			puts "\nGo on to next LABID.";
			if {[info exists spdata]} {
				unset spdata;
			}
			continue;
		}
	}
	# print out data
	if {[info exists spdata]} {
		puts "\nSP Data:\n";
		foreach name [lsort [array names spdata]] {
			puts "spdata(${name}) = $spdata(${name})";
		}
		unset spdata;
	}
}
#
# all done
#
exit 0;
