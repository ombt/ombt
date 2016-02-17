#!/opt/exp/bin/tclsh
#
# run a script to update a plexus switch.
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
package require checkenv
package require getoptval
package require checkretval
package require cfgdata
package require scripts
package require ncleis
package require lock
package require chooseone
#
# exit handlers
#
exit -onexit {
	global startok;
	if {$startok != 0} {
		# V hug running;
		set startok 0;
	}
}
trap {
	global startok;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "\nERROR: Got signal <$signame,$signo>";
	if {$startok != 0} {
		# V hug running;
		set startok 0;
	}
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-V] \[-U] \[-X]";
	puts "	\[-S trace level] \[-L logfile] ";
	puts "	\[-p] \[-l] \[-s section] \[-A] \[-c cmdno] \[script_filename]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-L filename - log file name";
	puts "";
	puts "	-s section - section in script to execute";
	puts "	-c cmdno - line# of command in section to start executing.";
	puts "	-p - print cfg data."
	puts "	-l - list all cmds after source files are read."
	puts "	-I - interactive mode."
	puts "	-U - unconditional. ignore lock and run."
	puts "	-X - expand commands, but don't execute."
	puts "	-A - run all sections in global variable 'sectionstorun'."
	puts "	-B - run upsh in the background; turn off all interactive I/O."
	puts "";
	puts "Script file contains Tl1 or Shell commands to update a switch."
	puts "If no section is given, then the first executable section is";
	puts "chosen and the commands in that section are executed one by one.";
	puts "Default script name is 'script'.";
	puts "";
	puts "Section types are data, cmds, and script. The type field";
	puts "within each section is required. If the type field is missing,";
	puts "then the script fails the basic sanity checks."
}
#
proc choosestartsec { cfgdata choice } {
	upvar $cfgdata cfgd;
	upvar $choice theOne;
	#
	if {![info exists cfgd(internal,namesofsecs,exec)]} {
		puts "\nERROR: upsh failed: no exec sections found.";
		exit 2;
	}
	#
	puts "";
	foreach section $cfgd(internal,namesofsecs,exec) {
		if {[info exists cfgd($section,isafunction)] && \
		    $cfgd($section,isafunction) == 0} {
			lappend choices $section;
		}
	}
	# set choices $cfgd(internal,namesofsecs,exec);
	set choices [linsert $choices 0 "QUIT"];
	chooseone "Choose a section to execute: " choices theOne;
}
#
#########################################################################
#
# default values
#
global env;
global dontexecute;
#
set stracelevel -1;
set logfile "";
set startcmd 1;
set startsec "";
set printdata 0;
set printcmds 0;
set startok 0;
set doInteractive 0;
set unconditional 0;
set dosectionstorun 0;
set background 0;
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
	{^-V} { log_user 1; }
	{^-U} { set unconditional 1; }
	{^-A} { set dosectionstorun 1; }
	{^-B} { set background 1; }
	{^-p} { set printdata 1; }
	{^-X} { set dontexecute 1; }
	{^-l} { set printcmds 1; }
	{^-I} { set doInteractive 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-L.*} { getoptval $argval logfile arg; }
	{^-c.*} { getoptval $argval startcmd arg; }
	{^-s.*} { getoptval $argval startsec arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
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
# lock out other HUGs
#
if {!$unconditional} {
	# set status [P hug running];
	# if {$status == 0} {
		# set startok 0;
		# puts "\nAnother UPSH is already running.";
		# exit 2;
	# } else {
		# set startok 1;
	# }
}
#
# check if logging is requested.
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nERROR: logging failed for file $logfile:\n$status";
		exit 2;
	}
	catch { system /usr/bin/chmod 777 $logfile; } ignore;
}
#
# check if function trace is requested.
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# read in script and execute.
#
if {$arg<$argc} {
	set scriptfile [lindex $argv $arg];
} elseif {[info exists env(UPSHSCRIPT)]} {
	set scriptfile $env(UPSHSCRIPT);
	set scriptdir [file dirname $scriptfile];
	if {![file isdirectory $scriptdir]} {
		puts "\nERROR: $scriptdir is not a directory or not readable.";
		exit 2;
	}
	cd $scriptdir;
	set scriptdir [pwd];
	puts "\nChanging to script directory: $scriptdir";
	set scriptfile [file tail $scriptfile];
} else {
	puts "\nUsing default script name 'script'.";
	set scriptfile "script";
}
#
puts "\nExecuting Script File: $scriptfile";
#
set status [initCfgData cfgdata];
if {[isNotOk $status]} {
	puts "\nERROR: initCfgData Failed: \n$status";
	exit 2;
}
#
set status [initCmdData cmddata];
if {[isNotOk $status]} {
	puts "\nERROR: initCmdData Failed: \n$status";
	exit 2;
}
#
set status [readCfgDataFile cfgdata cmddata $scriptfile];
if {[isNotOk $status]} {
	puts "\nERROR: readCfgDataFile Failed: \n$status";
	exit 2;
}
#
# check if we have an email address
#
set me "mrumore@alcatel-lucent.com lcstools@lts08.ih.lucent.com";
if {[info exists cfgdata(globals,email)]} {
	set email "${me} $cfgdata(globals,email)";
} else {
	set email "${me}";
}
#
if {$printdata != 0} {
	printCfgData cfgdata;
}
if {$printcmds != 0} {
	printCmdData cmddata;
}
if {$printdata == 0 && $printcmds == 0} {
	if {${background}} {
		# we are running in the background. exit parent 
		# and allow child to continue. turn off any interactive
		# requests. sorry. can't run interactive in the background.
		# if it dies, then it dies.
		set $doInteractive 0;
		if {[fork] != 0} {
			puts "\nBackgrounding. Parent upsh exiting.";
			sendemail $email "upsh" "Backgrounding. Parent upsh exiting.";
                	exit 0;
        	}
        	# let parent exit, then continue.
        	sleep 5;
        	disconnect;
	}
	if {!$dontexecute} {
		set updatestime [exec date];
		while { 1 } {
			if {[string length $startsec] <= 0 } {
				if {!$doInteractive} {
					# no start section and not interactive.
					puts "\nERROR: No start section given and running in non-interactive mode. Will exit.";
					sendemail $email "upsh ERROR" "ERROR: No start section given and running in non-interactive mode. Will exit.";
					exit 2;
				}
				choosestartsec cfgdata choice;
				if {$choice == "QUIT"} {
					sendemail $email "upsh" "Done.";
					puts "\nDone.";
					exit 0;
				}
				set startsec $choice;
			}
			#
			set stime [exec date];
			puts "\nStart time for section ${startsec}: ${stime}";
			sendemail $email "${startsec} START" "Start time for section ${startsec}: ${stime}";
			set status [execScript cfgdata $startsec $startcmd];
			puts "\nStart time for update: ${updatestime}";
			puts "\nStart time for section ${startsec}: ${stime}";
			puts "\nEnd time for section ${startsec} : [exec date]";
			sendemail $email "${startsec} END" "End time for section ${startsec} : [exec date]";
			#
			if {[isNotOk $status]} {
				puts "\nERROR: execScript Failed: \n$status";
				sendemail $email "${startsec} ERROR" "ERROR: execScript failed for ${startsec}: \n$status";
				if {!$doInteractive} { exit 2; }
			} else {
				sendemail $email "${startsec} SUCCESS" "SUCCESS: execScript passed for ${startsec}: \n$status";
			}
			if {!$doInteractive} { break; }
			set startsec "";
		}
	} else {
		# verify variable expansion
		set allerrmsgs "";
		foreach section $cfgdata(internal,namesofsecs,exec) {
			# skip functions
			if {[info exists cfgdata($section,isafunction)] && \
			    $cfgdata($section,isafunction) != 0} {
				# skip function
				continue;
			}
			# 
			puts "\nVerifying section $section";
			sendemail $email "${section} VERIFY" "Verifying section $section";
			set status [execScript cfgdata $section $startcmd];
			if {[isNotOk $status]} {
				puts "\nexecScript failed for section $section: \n$status";
				sendemail $email "${section} VERIFY ERROR" "execScript failed for section $section: \n$status";
				set allerrmsgs "${allerrmsgs}\nexecScript failed for section $section: \n$status";
			} else {
				puts "\nexecScript passed for section $section: \n$status";
				sendemail $email "${section} VERIFY SUCCESS" "execScript passed for section $section: \n$status";
			}
		}
		#
		if {[string length ${allerrmsgs}] > 0} {
			puts "\nError Msgs:\n${allerrmsgs}\n";
			sendemail $email "all sections VERIFY ERROR" "Error Msgs:\n${allerrmsgs}\n";
		}
	}
}
#
puts "\nDone.";
sendemail $email "upsh" "Done.";
exit 0;

