#!/opt/exp/bin/expect
#
# start up an interactive tl1 shell. log cmds if requested.
#
#########################################################################
#
# libraries
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
source $env(LCSTOOLSLIB)/chooseone
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "usage: [file tail $argv0] \[-\?] \[-x]";
	puts "\[-S trace level] \[-l logfile] ";
	puts "\[-A | -B] \[-u username] \[-p passwd] \[labid] ";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-l logfile - log all output to a logfile";
	puts "	-L logfile - log and APPEND all output to a logfile";
	puts "	-A - choose SP-A ";
	puts "	-B - choose SP-B ";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	labid is read from variable LABID, unless given.";
}
#
#########################################################################
#
# default values
#
set username "telica";
set userpasswd "telica";
set maxlogons 5;
set stracelevel -1;
set logfile "";
set appendtologfile 0;
set activeip "";
set labid "";
set sp "";
#
set pwd $env(PWD);
#
log_user 0;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; set appendtologfile 0; }
	{^-L.*} { getoptval $argval logfile arg; set appendtologfile 1; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-A} { set sp "A"; }
	{^-B} { set sp "B"; }
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
checkenv;
#
# get ip addresses for labid.
#
if {$arg<$argc} {
	set labid [lindex $argv $arg];
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
} else {
	puts "LABID is neither set nor given.";
	exit 2;
}
#
if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
	puts "tl1sh: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "tl1sh: no IPs found for labid $labid.";
	exit 2;
}
#
set ips [lindex $obuf 0];
set ipdata [split $ips " \t"];
set cpu_a_ip [lindex $ipdata 0];
set cpu_b_ip [lindex $ipdata 1];
#
if {$sp == "A"} {
	set activeip "SP-A-IP-ADDR $cpu_a_ip";
} elseif {$sp == "B"} {
	set activeip "SP-B-IP-ADDR $cpu_b_ip";
} else {
	lappend choices "SP-A-IP-ADDR $cpu_a_ip" "SP-B-IP-ADDR $cpu_b_ip";
	set choices [linsert $choices 0 "QUIT"];
	chooseone "Choose active $labid SP-IP: " choices activeip;
	if {$activeip == "QUIT"} {
		puts "exiting tl1sh.";
		exit 0;
	}
}
#
puts "connecting to $labid at $activeip ...";
#
set ipdata [split $activeip " \t"];
set activeip [lindex $ipdata 1];
#
# check if logging is enabled.
#
if {[string length $logfile] > 0} {
	cd ${pwd};
	if {$appendtologfile} {
		if {[catch {log_file -a $logfile; } status]} {
			puts "\nappend logging failed for file $logfile:\n$status";
			exit 2;
		}
	} else {
		if {[catch {log_file -noappend -a $logfile; } status]} {
			puts "\nlogging failed for file $logfile:\n$status";
			exit 2;
		}
	}
}
log_user 1;
#
# open tl1 interface ...
#
set id "";
set timeout 45;
set tl1port 2361;
set tl1_id "";
#
puts "\nconnecting to machine $labid at $activeip ...";
#
for {set i 0} {$i<$maxlogons} {incr i} {
	puts "\nattempt $i to logon machine $activeip ...";
	#
	spawn -noecho telnet $activeip $tl1port;
	expect {
	"Connected to " {
		puts "\nconnected to $activeip at port $tl1port ...";
		set tl1_id $spawn_id;
		break;
	}
	"Connection refused" {
		puts "sleep 2 sec, and try again ..."
		set tl1_id "";
		sleep 2;
	}
	eof {
		puts "\nReceived an unexpected EOF.";
		set tl1_id "";
		exit 2;
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
	puts "\nunable to telnet to machine $activeip, port $tl1port.";
	exit 2;
}
#
send "\r";
expect {
"Telica>" {
	puts "\nTL1 prompt received ..."
}
eof {
	puts "\nReceived an unexpected EOF.";
	exit 2;
}
timeout {
	puts "\ntimeout trying to get TL1 prompt ...";
	exit 2;
}
}
#
# login as the given user.
#
set actuser "act-user::${username}:::${userpasswd}"
#
send "$actuser;\r";
#
set checkforprompt 1;
set checkfordeny 0;
#
expect {
-re "M.*\[0-9]+.*COMPLD" {
	puts "\nCOMPLETED received ...";
	if {[regexp "PROTECTION.*System.*Processor" $expect_out(buffer)] == 1} {
		puts "\n$activeip is the STANDBY MACHINE.";
		exit 2;
	} elseif {[regexp "Login Not Active" $expect_out(buffer)] == 1} {
		puts "\nLogin not active on machine $activeip.";
		exit 2;
	} elseif {[regexp "Can't login" $expect_out(buffer)] == 1} {
		puts "\nNot able to logon machine $activeip.";
		exit 2;
	}
}
-re "M.*\[0-9]+.*IP" {
	puts "\nIN PROGRESS received ...";
}
-re "M.*\[0-9]+.*DENY" {
	set checkfordeny 1;
	puts "\nDENY received ...";
}
-re ";\r\nTelica>" {
	set checkforprompt 0;
	puts "\nTelica prompt received ...";
}
eof {
	puts "\nReceived an unexpected EOF.";
	exit 2;
}
timeout {
	puts "\ntimeout waiting for a response ...";
	exit 2;
}
}
#
if {$checkforprompt} {
	expect {
	-re ";\r\nTelica>" {
		if {$checkfordeny} {
			if {[regexp "Host is in Standby Mode" $expect_out(buffer)] == 1} {
				puts "\n$activeip is the STANDBY MACHINE.";
				exit 2;
			} elseif {[regexp "Login Not Active" $expect_out(buffer)] == 1} {
				puts "\nLogin not active on machine $activeip.";
				exit 2;
			} elseif {[regexp "Can't login" $expect_out(buffer)] == 1} {
				puts "\nNot able to logon machine $activeip.";
				exit 2;
			}
		}
	}
	eof {
		puts "\nReceived an unexpected EOF.";
		exit 2;
	}
	timeout {
		puts "\ntimeout waiting for a response ...";
	}
	}
}
#
# switch to interactive mode
#
set timeout -1;
send ";\r";
#
catch { interact {
-o eof {
	puts "\n\nend-of-file ...\n\n";
}
timeout 3600 {
	puts "\n\ntime out, no input in last 60 minutes.\n\n";
}
}; } ignore;
#
exit 0

