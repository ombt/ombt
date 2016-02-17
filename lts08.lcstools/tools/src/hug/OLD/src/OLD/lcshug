#!/opt/exp/bin/expect
#
# run a TL1 script remotely
#
#########################################################################
#
# libraries
#
source $env(LCSTOOLSLIB)/logging
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/chooseone
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
#
# functions
#
		puts "$msg";
	}
}
#
proc getoptval { argval varname argname } {
	global argv;
	upvar $varname var;
	upvar $argname arg;
	#
	if {[string length $argval] == 2} {
		incr arg;
		set var [lindex $argv $arg];
	} else {
		set var [string range $argval 2 end];
	}
}
#
proc stripcmd { cmd } {
	traceputs "stripcmd: cmd is <$cmd>";
	set cmd [string trim $cmd];
	traceputs "stripcmd: strip leading/trailing whitespace, cmd is <$cmd>";
	set cmd [string trim $cmd "\""];
	traceputs "stripcmd: strip leading/trailing \", cmd is <$cmd>";
	set cmd [string trimright $cmd ";"];
	traceputs "stripcmd: strip trailing ;, cmd is <$cmd>";
	return $cmd;
}
#
proc usage { } {
	global argv0;
	global maxlogons;
	global username;
	global userpasswd;
	global ipaddr;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-T] \[-V] ";
	puts "	\[-S trace level] \[-l logfile] scriptname";
	puts "";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-T - enable trace messages";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-l filename - log file name";
	puts "";
	puts "The script header section contains all the information";
	puts "describing the switch to be updated. If a log file name";
	puts "is not given, then one is created be default.";
}
#
proc initcounters { arrayname } {
	upvar $arrayname counters;
	set counters(COMPLETED) 0;
	set counters(DENIED) 0;
	set counters(INPROGRESS) 0;
	set counters(TIMEOUT) 0;
	set counters(EXECUTED) 0;
	set counters(MISCELLANEOUS) 0;
	set counters(SLEEP) 0;
	set counters(COMMENTS) 0;
}
#
proc dumpcounters { arrayname } {
	upvar $arrayname counters;
	set counternames [array names counters]
	set maxlen 0;
	puts "\n\nCMD RESULTS SUMMARY:";
	foreach cname $counternames {
		set cnamelen [string length $cname];
		if {$maxlen < $cnamelen} {
			set maxlen $cnamelen;
		}
	}
	if {$maxlen <= 0} {
		return;
	}
	puts "";
	foreach cname $counternames {
		puts [format "%-${maxlen}s: %d" $cname $counters($cname)];
	}
}
#
#########################################################################
#
# default values
#
set tid "telica";
set username "telica";
set userpasswd "telica";
set ipaddr "";
set scriptname "";
set maxlogons 3;
set traceflag 0;
set verbose 0;
set exitondeny 0;
set stracelevel -1;
set logfile "";
set labid "";
set sp "";
#
# save current working directory
#
set currcd $env(PWD);
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
	{^-T} { set traceflag 1; }
	{^-V} { set verbose 1; }
	{^-D} { set exitondeny 1; }
	{^-A} { set sp "A"; }
	{^-B} { set sp "B"; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-L.*} { getoptval $argval labid arg; }
	{^-m.*} { getoptval $argval maxlogons arg; }
	{^-t.*} { getoptval $argval tid arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-i.*} { getoptval $argval ipaddr arg; }
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
# check if any arguments left.
#
if {$argc < 1} {
	usage
	exit 2;
}
#
# check if IP or LABID are used.
#
if {$ipaddr == ""} {
	if {$labid == ""} {
		if {[info exists env(LABID)]} {
			set labid $env(LABID);
		} else {
			puts "LABID is neither set nor given.";
			exit 2;
		}
	}
	#
	if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
		puts "tl1exec: dbselect of relation 'chassis' failed.";
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
			puts "exiting tl1exec.";
			exit 0;
		}
	}
	#
	set ipdata [split $activeip " \t"];
	set activeip [lindex $ipdata 1];
	set ipaddr $activeip;
}
#
puts "tid    : $tid";
puts "ipaddr : $ipaddr";
puts "login  : $username";
puts "passwd : [regsub -all -- "." $userpasswd "X"]";
puts "logfile: $logfile";
#
checkenv;
#
# reset current working directory
#
cd $currcd;
#
# check if logging is enabled.
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nlogging failed for file $logfile:\n$status";
		exit 2;
	}
}
#
# read file into a list
#
set filename [lindex $argv $arg];
if {![file readable $filename]} {
	puts "\nfile $filename is not readable.";
	exit 2;
}
#
if {[catch {set fd [open $filename "r"]} status]} {
	puts "\nunable to open for read:\n$status";
	exit 2;
}
#
# act-user MUST always be cmd 0.
#
set actuser "act-user::USERNAME:::USERPASSWD"
regsub -- "USERNAME" $actuser "$username" actuser;
regsub -- "USERPASSWD" $actuser "$userpasswd" actuser;
lappend tl1cmds "$actuser";
while {[gets $fd inbuf] > 0} {
	traceputs "$inbuf";
	traceputs "stripped cmd is [stripcmd $inbuf]";
	lappend tl1cmds "[stripcmd $inbuf]";
}
catch { close $fd; wait; } ignore;
#
# open tl1 interface ...
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
set id "";
set timeout 5;
set tl1port 2361;
set tl1_id "";
#
traceputs "\nconnecting to machine $ipaddr ...";
for {set i 0} {$i<$maxlogons} {incr i} {
	traceputs "\nattempt $i to logon machine $ipaddr ...";
	#
	spawn -noecho telnet $ipaddr $tl1port;
	expect {
	"Connected to " {
		traceputs "\nconnected to $ipaddr at port $tl1port ...";
		set tl1_id $spawn_id;
		break;
	}
	"Connection refused" {
		traceputs "sleep 2 sec, and try again ..."
		set tl1_id "";
		sleep 2;
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
	puts "\nunable to telnet to machine $ipaddr, port $tl1port.";
	exit 2;
}
#
send "\r";
expect {
"Telica>" {
	traceputs "\nTL1 prompt received ..."
}
timeout {
	puts "\ntimeout trying to get TL1 prompt ...";
	exit 2;
}
}
#
# log cmds according to final status.
#
if {[string length $logfile] > 0} {
	set home $env(HOME);
	logopen "${home}/inprogress.tl1exec" inprogressfd;
	logopen "${home}/completed.tl1exec" completedfd;
	logopen "${home}/denied.tl1exec" deniedfd;
	logopen "${home}/timedout.tl1exec" timedoutfd;
	logopen "${home}/misc.tl1exec" miscfd;
}
#
# start executing TL1 cmds
#
initcounters counters;
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
set timeout 10;
set status "";
set maxactusercmds 3;
#
catch {
set cmdcnt [llength $tl1cmds];
for {set cmd 0; set actusercnt 0; } \
    {($cmd<$cmdcnt) && ($actusercnt<$maxactusercmds)} { } {
	#
	set tl1cmd [lindex $tl1cmds $cmd];
	#
	if {$verbose} {
		log_user 1;
	} else {
		log_user 0;
	}
	# enable if a rtrv cmd.
	switch -regexp -- ${tl1cmd} {
	{[rR][tT][rR][vV]-} {
		log_user 1;
	}
	{^[ \t]*$} {
		# skip empty lines
		incr cmd;
		continue;
	}
	{^[ \t]*#} {
		# skip comments
		incr counters(COMMENTS);
		incr cmd;
		continue;
	}
	}
	# check for request to sleep.
	if {[regexp -- {^[ \t]*sleep[ \t]+([0-9]+)} $tl1cmd mstr submstr] == 1} {
		incr counters(SLEEP);
		sleep $submstr;
		incr cmd;
		continue;
	}
	#
	if {$cmd > 0} {
		puts "\nTL1 cmd <$tl1cmd> ...";
	} else {
		puts "\nTL1 cmd <act-user::${username}:::[regsub -all -- "." $userpasswd "X"] > ...";
	}
	incr counters(EXECUTED);
	send "$tl1cmd;\r";
	#
	set checkforprompt 1;
	set checkfordeny 0;
	set incrflag 1;
	#
	expect {
	-re "M.*\[0-9]+.*COMPLD" {
		incr counters(COMPLETED);
		puts "\nCOMPLETED received ...";
		if {[string length $logfile] > 0} {
			logwrite $completedfd "$tl1cmd";
		}
	}
	-re "M.*\[0-9]+.*IP" {
		incr counters(INPROGRESS);
		puts "\nIN PROGRESS received ...";
		if {[string length $logfile] > 0} {
			logwrite $inprogressfd "$tl1cmd";
		}
	}
	-re "M.*\[0-9]+.*DENY" {
		incr counters(DENIED);
		puts "\nDENY received ...";
		set checkfordeny 1;
		if {[string length $logfile] > 0} {
			logwrite $deniedfd "$tl1cmd";
		}
		if {$exitondeny} {
			puts "\nEXITING ON FIRST FAILURE ...\n";
			exit 2;
		}
	}
	-re ";\r\nTelica>" {
		incr counters(MISCELLANEOUS);
		set checkforprompt 0;
		traceputs "\nTelica prompt received ...";
		if {[string length $logfile] > 0} {
			logwrite $miscfd "$tl1cmd";
		}
	}
	timeout {
		incr counters(TIMEOUT);
		puts "\ntimeout waiting for a response ...";
		if {[string length $logfile] > 0} {
			logwrite $timedoutfd "$tl1cmd";
		}
		set checkforprompt 0;
	}
	}
	#
	if {$checkforprompt} {
		expect {
		-re ";\r\nTelica>" {
			traceputs "\nTelica prompt received ...";
			if {$checkfordeny} {
				if {[regexp "Host is in Standby Mode" $expect_out(buffer)] == 1} {
					puts "\n$ipaddr is the STANDBY MACHINE.";
					ucondclose;
					exit 2;
				} elseif {[regexp "This is the PROTECTION System Processor" $expect_out(buffer)] == 1} {
					puts "\n$ipaddr is the STANDBY MACHINE.";
					ucondclose;
					exit 2;
				} elseif {[regexp "Login Not Active" $expect_out(buffer)] == 1} {
					puts "\nLogin not active on machine $ipaddr.";
					ucondclose;
					exit 2;
				} elseif {[regexp "Can't login" $expect_out(buffer)] == 1} {
					puts "\nNot able to logon machine $ipaddr.";
					if {$cmd != 0} {
						ucondclose;
						exit 2;
					} else {
						# remember, cmd 0 is act-user.
						# since act-user failed, try again.
						puts "retry act-user on machine $ipaddr.";
						sleep 5;
						set incrflag 0;
						incr actusercnt;
					}
				}
			}
		}
		timeout {
			incr counters(TIMEOUT);
			puts "\ntimeout waiting for a response ...";
		}
		}
	}
	#
	# increment to next cmd
	#
	if {$incrflag} { incr cmd; }
}
} status;
if {[string length $status] > 0} {
	# puts "\nCAUGHT ERROR: $status";
	set finalstatus 2;
} else {
	dumpcounters counters;
	set finalstatus 0;
}
#
if {[string length $logfile] > 0} {
	logclose $inprogressfd;
	logclose $completedfd;
	logclose $deniedfd;
	logclose $timedoutfd;
	logclose $miscfd;
}
#
ucondclose;
#
exit $finalstatus
