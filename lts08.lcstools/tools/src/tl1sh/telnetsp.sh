#!/opt/exp/bin/expect
#
# start up an interactive Lynx-OS shell. log cmds if requested.
#
#########################################################################
#
# libraries
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
source $env(LCSTOOLSLIB)/chooseone
source $env(LCSTOOLSLIB)/lcstelnet
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V]";
	puts "\[-S trace level] \[-l logfile] \[-F cmd_file]";
	puts "\[-A | -B] \[-u username] \[-p passwd] \[-P] \[labid] ";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-V - verbose mode";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-l logfile - log all output to a logfile";
	puts "	-P - user Telica High Priority telnet port, 1982";
	puts "	-A - choose SP-A";
	puts "	-B - choose SP-B";
	puts "	-F cmd_file - execute cmds in file remotely";
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
set username "root";
set userpasswd "plexus9000";
set maxlogons 5;
set stracelevel -1;
set logfile "";
set activeip "";
set labid "";
set verbose 0;
set port 23;
set sp "";
set cmdfile "";
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
	{^-V} { set verbose 1; }
	{^-P} { set port 1982; }
	{^-A} { set sp "A"; }
	{^-B} { set sp "B"; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-F.*} { getoptval $argval cmdfile arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
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
	puts "telnetsp: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "telnetsp: no IPs found for labid $labid.";
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
	chooseone "Choose active SP-IP: " choices activeip;
	if {$activeip == "QUIT"} {
		puts "exiting telnetsp.";
		exit 0;
	}
}
#
puts "connecting to $labid at $activeip $port ...";
#
set ipdata [split $activeip " \t"];
set activeip [lindex $ipdata 1];
#
# check if logging is enabled.
#
if {[string length $logfile] > 0} {
	cd ${pwd};
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nlogging failed for file $logfile:\n$status";
		exit 2;
	}
}
if {$verbose > 0} {
	log_user 1;
}
#
# do we telnet interactively or do we exec cmds from a file?
#
if {[string length "${cmdfile}"] > 0} {
	# check if file exists
	if {![file exists $cmdfile]} {
		puts "\nfile $cmdfile does not exist.";
		exit 2;
	}
	#
	if {[catch {set infd [open $cmdfile "r"]} status]} {
		puts "\nopen failed for file $cmdfile:\n${status}";
		exit 2;
	}
	# telnet to sp
	telnet_to $activeip ip_id;
	#
	while {[gets $infd inbuf] >= 0 && ![eof $infd]} {
		# skip comments and white space
		switch -regexp -- ${inbuf} {
		{^[ \t]*$} { continue; }
		{^[ \t]*#} { continue; }
		}
		#
		remote_exec $ip_id "${inbuf}" 300;
	}
	#
	catch { close $infd; } ignore;
	#
	close_telnet $ip_id;
} else {
	# interactively telnet to SP.
	interactive_telnet_to $activeip ip_id $port;
	# switch to interactive mode
	set timeout -1;
	set spawn_id $ip_id;
	send "\r";
	#
	catch { interact {
	-o eof {
		send_user "\n\nend-of-file ...\n\n";
	}
	timeout 21600 {
		send_user "\n\ntime out, no input in last 60 minutes.\n\n";
	}
	}; } ignore;
	#
	close_telnet $ip_id;
}
#
exit 0
