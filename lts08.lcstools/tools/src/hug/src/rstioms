#!/opt/exp/bin/tclsh
#
# restore all IOMs on a chassis.
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

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level]";
	puts "	\[-u username] \[-p passwd] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-u username - TL1 login (default is telica)";
	puts "	-p passwd - TL1 passwd (default is telica)";
	puts "";
	puts "Restore all IOMs for the given labid. Labid is either read from";
	puts "the command line or from the environment variable LABID.";
	puts "";
}
#
#########################################################################
#
# declarations and default values.
#
global env;
set stracelevel -1;
set username "telica";
set passwd "telica";
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
	{^-u.*} {
		# get user name
		getoptval $argval username arg;
	}
	{^-p.*} {
		# get passwd
		getoptval $argval passwd arg;
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
set logfile "/lcsl100/tmp/[file tail $argv0].[pid]";
catch { log_file -noappend -a $logfile; } ignore;
catch { system /usr/bin/chmod 777 $logfile; } ignore;
puts "\nLog file is $logfile.";
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
puts "Restoring ALL IOMs for LABID ${labid}:"
puts "";
puts "SP-A IP   : ${sp_a_ip}";
puts "SP-B IP   : ${sp_b_ip}";
puts "TL1 Login : ${username}";
puts "TL1 Passwd: ${passwd}";
#
# restore the IOMs
#
set status [hw_maint_rst_all_ioms ${labid} ${sp_a_ip} ${sp_b_ip} ${username} ${passwd}];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_rst_all_ioms failed: \n${status}";
	exit 2;
}
#
# all done
#
puts "\nDone.";
exit 0;
