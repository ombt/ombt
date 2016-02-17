#!/opt/exp/bin/tclsh
#
# execute a TL1 command
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
	puts "Got signal <$signame,$signo>";
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-q] \[-S trace level]";
	puts "	\[-u username] \[-p passwd] \[-l labid] \[ sh | tl1-cmd \[tl1-cmd \[...]]]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-u username - TL1 login (default is telica)";
	puts "	-p passwd - TL1 passwd (default is telica)";
	puts "	-l labid - lab identifier (default is \$LABID)";
	puts "	-q - quiet. keep output to a minimum.";
	puts "";
	puts "Execute the TL1 cmd on the given chassis (labid). Lab id is";
	puts "read from the environment variable LABID or the -l option.";
	puts "";
	puts "The special TL1 cmd 'sh' causes the shell to become interactive.";
	puts "";
	puts "Examples:";
	puts "\n1) to run RTRV-EQPT command:";
	puts "	$ tl1 rtrv-eqpt";
	puts "\n2) to start an interactive TL1 shell:";
	puts "	$ tl1 sh";
	puts "\n3) to run multiple CPY-MEM commands:";
	puts "	$ tl1 cpy-mem::iom-1 cpy-mem::iom-2";
	puts "\n4) to run multiple commands and enter an interactive shell:";
	puts "	$ tl1 rtrv-eqpt rtrv-infop-eqpt sh";
	puts "\n5) to use a different login and logon for the first time:";
	puts "	$ tl1 -u mikerumore -p 12345678 ed-pid::mikerumore:::12345678,87654321 sh";
	puts "\n6) to use a different login (after ed-pid was executed):";
	puts "	$ tl1 -u mikerumore -p 87654321 sh";
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
set labid "";
set quiet 0;
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
	{^-q} {
		# keep output to a minimum
		set quiet 1;
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
	{^-l.*} {
		# get labid
		getoptval $argval labid arg;
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
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
# get labid 
#
if {[string length $labid] == 0} {
	if {[info exists env(LABID)]} {
		set labid $env(LABID);
	} else {
		puts "\nERROR: LABID is neither set nor given.";
		exit 2;
	}
}
#
# get tl1 command
#
if {$arg>=$argc} {
	puts "\nERROR: No TL1 commands were given.\n";
	usage; 
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
if {!${quiet}} {
	puts "";
	puts "Executing TL1 commands on LABID ${labid}:"
	puts "";
	puts "SP-A IP   : ${sp_a_ip}";
	puts "SP-B IP   : ${sp_b_ip}";
	puts "TL1 Login : ${username}";
	puts "TL1 Passwd: ${passwd}";
}
#
set status [hw_maint_tl1_open_silent spawn_id ${sp_a_ip} ${sp_b_ip} ${username} ${passwd} ${quiet}];
if {[isNotOk $status]} {
	puts "\nERROR: hw_maint_tl1_open failed: \n${status}";
	exit 2;
}
#
log_user 1;
#
for { } {$arg<$argc} {incr arg} {
	# exec the cmd
	set tl1cmd [lindex $argv $arg];
	# check for non-returning interactive shell request
	if {${tl1cmd} == "sh"} {
		# we have an interactive shell request
		set timeout -1;
		send ";\r";
		#
		catch { interact {
		-o eof {
			puts "\nEND-OF-FILE ...";
		}
		timeout 3600 {
			puts "\nTime out, no input in the last 60 minutes.";
		}
		}; } ignore;
		#
		hw_maint_close $spawn_id;
		puts "\nDone.";
		exit 0;
	}
	# execute TL1 command 
	puts "\n+++ Start of TL1 command - ${labid} - [exec date]";
	send "${tl1cmd};\r";
	expect {
	-re ";\r\nTelica>" {
		puts "\n${tl1cmd} - PROMPT RECEIVED";
		puts "\nEnd of TL1 command - ${labid} - [exec date] ++-";
	}
	-re "<\r\nTelica>" {
		puts "\n${tl1cmd} - IN-PROGRESS PROMPT RECEIVED";
		puts "\nEnd of TL1 command - ${labid} - [exec date] ++-";
	}
	timeout {
		puts "\nWARNING: ${tl1cmd} - TIME OUT";
		puts "\nEnd of TL1 command - ${labid} - [exec date] ++-";
	}
	eof {
		puts "\nERROR: ${tl1cmd} - EOF";
		puts "\nEnd of TL1 command - ${labid} - [exec date] ++-";
		hw_maint_close $spawn_id;
		exit 2;
	}
	}
}
#
hw_maint_close $spawn_id;
#
# all done
#
puts "\nDone.";
exit 0;
