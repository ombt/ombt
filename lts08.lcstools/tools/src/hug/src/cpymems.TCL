#!/opt/exp/bin/tclsh
#
# cpy-mem all IOMs in a lab
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
	puts "	\[-u TL1 username] \[-p TL1 passwd] \[-U username] \[-P passdw]";
	puts "	\[-R] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-u username - TL1 login (default is telica)";
	puts "	-p passwd - TL1 passwd (default is telica)";
	puts "	-U username - Lynx-OS login (default is root)";
	puts "	-P passwd - Lynx-OS passwd (default is plexus9000)";
	puts "	-c maxcpymems - set maximum simultaneous CPY-MEMs (default = 3)";
	puts "	-R - IOMs that fail with cpymem are recovered using update_flash_partition.";
	puts "";
	puts "Cpy-mem all IOMs for the given labid. Labid is either read from";
	puts "the command line or from the environment variable LABID. The";
	puts "procedure for cpy-mems: rmv-eqpt all IOMs, cpy-mem all IOMs, rsteqpt all IOMs."
	puts "The Lynx-OS login/passwd are only used if the -R was chosen.";
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
set maxcpymems 3;
set recoverioms 0;
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
	{^-R} {
		# recover failed IOMs
		set recoverioms 1;
	}
	{^-S.*} {
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
set sp_a_ip [lindex $ipdata 0];
set sp_b_ip [lindex $ipdata 1];
#
puts "";
puts "CPY-MEM ALL IOMs for LABID ${labid}:"
puts "";
puts "SP-A IP    : ${sp_a_ip}";
puts "SP-B IP    : ${sp_b_ip}";
puts "TL1 Login  : ${tl1username}";
puts "TL1 Passwd : ${tl1passwd}";
puts "Lynx Login : ${lynxusername}";
puts "Lynx Passwd: ${lynxpasswd}";
#
# remove the IOMs
#
set status [hw_maint_rmv_all_ioms ${labid} ${sp_a_ip} ${sp_b_ip} ${tl1username} ${tl1passwd}];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_rmv_all_ioms failed: \n${status}";
	exit 2;
}
#
# cpy-mem the IOMs
#
set status [hw_maint_cpymem_all_ioms ${labid} ${sp_a_ip} ${sp_b_ip} ${tl1username} ${tl1passwd} ${recoverioms} ${lynxusername} ${lynxpasswd} ${maxcpymems} ];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_cpymem_all_ioms failed: \n${status}";
	exit 2;
}
#
# restore the IOMs
#
set status [hw_maint_rst_all_ioms ${labid} ${sp_a_ip} ${sp_b_ip} ${tl1username} ${tl1passwd}];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_rst_all_ioms failed: \n${status}";
	exit 2;
}
#
# all done
#
puts "\nDone.";
exit 0;

