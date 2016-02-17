#!/opt/exp/bin/tclsh
#
# attempt to diagnose and cure (recover) sick and ill SPs.
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
package require nhwmaint
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

	puts "Usage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level]";
	puts "	\[-u TL1 username] \[-p TL1 passwd] \[-U username] \[-P passwd]";
	puts "	\[-K] \[-a | -m] \[-M] \[labid]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-u username - TL1 login (default is telica)";
	puts "	-p passwd - TL1 passwd (default is telica)";
	puts "	-U username - Lynx-OS login (default is root)";
	puts "	-P passwd - Lynx-OS passwd (default is plexus9000)";
	# puts "	-K - interrupt SPs and drop into SKDB, then recover."
	puts "	-a - boot into app mode after recovery."
	puts "	-m - boot into min mode after recovery."
	puts "	-M - no clean up, just get machine booted as is."
	puts "";
	puts "Attempt to run sanity checks on both SPs and take steps to ";
	puts "recover the SPs, that is, bring them to a sane state.";
	puts "";
	puts "recoversps does the following:";
	puts "\n1) Disconnect all pxmonitor users for this labid.";
	puts "\n2) Attempt to logon using the lynx pxmonitor ports.";
	puts "\n3) If the SP is locked or unresponsive, then the SP is ";
	puts "interrupted using the ^_^K^_ sequence and rebooted from the ";
	puts "ibm* prompt.";
	puts "\n4) If the SP is found at the ibm* prompt or at SKDB prompt,";
	puts "then the SP is booted.";
	puts "\n5) If the SP responds, then login as root on both SPs.";
	puts "\n6) If login is successful, then the Telica_IP file is ";
	puts "populated with the correct mgt0 IP, and the interface and ";
	puts "routing are recreated using iprecover."
	puts "\n7) All local backups and old log files are deleted to release";
	puts "space.";
	puts "\n8) The file systems (hd2b, hd2c, hd2d) are auditted using fsck.";
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
set bootmode "none";
set useskdb 1;
set minimal 0;
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
	{^-a} {
		# boot into app mode when done
		set bootmode "app";
	}
	{^-M} {
		# just get machine booted and going.
		set minimal 1;
	}
	{^-m} {
		# boot into min mode when done
		set bootmode "min";
	}
	{^-K} {
		# interrupt SPs and drop into kernel debugger, SKDB.
		set useskdb 1;
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
set logfile "/lcsl100/tmp/[file tail $argv0].[pid]";
catch { log_file -noappend -a $logfile; } ignore;
catch { system /usr/bin/chmod 777 $logfile; } ignore;
puts "\nLog file is $logfile.";
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
set spips(sp-a) ${sp_a_ip};
set spips(sp-b) ${sp_b_ip};
set spips(lynx-a) ${sp_a_ip};
set spips(lynx-b) ${sp_b_ip};
set spips(tl1-a) ${sp_a_ip};
set spips(tl1-b) ${sp_b_ip};
#
puts "\nAttempt to Diagnose/Recover SPs for LABID ${labid}:"
puts "\nSP-A IP    : ${sp_a_ip}";
puts "SP-B IP    : ${sp_b_ip}";
puts "TL1 Login  : ${tl1username}";
puts "TL1 Passwd : ${tl1passwd}";
puts "Lynx Login : ${lynxusername}";
puts "Lynx Passwd: ${lynxpasswd}";
puts "SKDB flag  : ${useskdb}";
#
# start checking the SPs.
#
if {${minimal}} {
	set status [hw_maint_min_recover_sps ${labid} spips ${tl1username} ${tl1passwd} ${lynxusername} ${lynxpasswd} $useskdb ];
} else {
	set status [hw_maint_recover_sps ${labid} spips ${tl1username} ${tl1passwd} ${lynxusername} ${lynxpasswd} $useskdb ];
}
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_recover_sps failed: \n${status}";
	exit 2;
}
#
# do we reboot the switch?
#
global env;
#
if {${bootmode} == "app"} {
	puts "\nBooting switch ${labid} into APP mode.";
	catch { system "$env(LCSTOOLSBIN)/lcsreboot -a ${labid}"; } ignore;
} elseif {${bootmode} == "min"} {
	puts "\nBooting switch ${labid} into MIN mode.";
	catch { system "$env(LCSTOOLSBIN)/lcsreboot -m ${labid}"; } ignore;
}
#
# all done
#
puts "\nDone.";
exit 0;
