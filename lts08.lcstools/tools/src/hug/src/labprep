#!/opt/exp/bin/tclsh
#
# configure switch a required state before a HUG.
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
package require lock
package require hughardware
package require scripts
package require ncleis
#
# exit handlers
#
exit -onexit {
	global startok;
	global switchid;
	global printTemplate;
	if {$startok != 0} {
		V hug $switchid;
		set startok 0;
	}
	if {!$printTemplate} {
		puts "\nDone.";
	}
}
#
trap {
	global startok;
	global switchid;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "\nERROR: Got signal <$signame,$signo>";
	if {$startok != 0} {
		V hug $switchid;
		set startok 0;
	}
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-V] \[-T] ";
	puts "	\[-S trace level] \[-L logfile] \[-o outfile] \[data_filename]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-L filename - log file name";
	puts "	-o filename - script output file";
	puts "	-T - print template config data file";
	puts "";
	puts "Data file contains data describing switch to be configured."
	puts "Default data file name is 'switchdata'. If script output file is";
	puts "not given, then the script is written to stdout.";
	puts "";
}
#
proc printTemplate { } {
	puts "#";
	puts "# sample global data used by lab configuration preprocessor.";
	puts "#";
	puts "globals data {";
	puts "	scriptsdir .";
	puts "	#";
	puts "	switchid ihlcsg11";
	puts "	#";
	puts "	# email yourhandle@lucent.com";
	puts "	#";
	puts "	ihlcsg11 {";
	puts "		login root";
	puts "		passwd plexus9000";
	puts "		tl1login telica";
	puts "		tl1passwd telica";
	puts "		spa {";
	puts "			ip 135.1.104.205";
	puts "		}";
	puts "		spb {";
	puts "			ip 135.1.104.206";
	puts "		}";
	puts "	}";
	puts "	#";
	puts "	sp {";
	puts "		a {";
	puts "			state act";
	puts "		}";
	puts "		b {";
	puts "			state stdbyh";
	puts "		}";
	puts "	}";
	puts "	iom {";
	puts "		1 {";
	puts "			state act";
	puts "		}";
	puts "		2 {";
	puts "			state stbyh";
	puts "		}";
	puts "		3 {";
	puts "			state act";
	puts "		}";
	puts "		4 {";
	puts "			state stbyh";
	puts "		}";
	puts "		5 {";
	puts "			state oos";
	puts "		}";
	puts "		6 {";
	puts "			state act";
	puts "		}";
	puts "		7 {";
	puts "			state stbyh";
	puts "		}";
	puts "		8 {";
	puts "			state act";
	puts "		}";
	puts "		9 {";
	puts "			state act";
	puts "		}";
	puts "		10 {";
	puts "			state act";
	puts "		}";
	puts "		11 {";
	puts "			state act";
	puts "		}";
	puts "		12 {";
	puts "			state act";
	puts "		}";
	puts "		13 {";
	puts "			state act";
	puts "		}";
	puts "		14 {";
	puts "			state act";
	puts "		}";
	puts "		15 {";
	puts "			state act";
	puts "		}";
	puts "		16 {";
	puts "			state act";
	puts "		}";
	puts "		17 {";
	puts "			state act";
	puts "		}";
	puts "	}";
	puts "}";
	return 0;
}
#
#########################################################################
#
# default values
#
global env;
#
set stracelevel -1;
set logfile "";
set startok 0;
set outfile "";
set huglite 0;
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
	{^-T} { printTemplate; exit 0; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-L.*} { getoptval $argval logfile arg; }
	{^-o.*} { getoptval $argval outfile arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: unknown option: $argval\n"; usage; exit 2 }
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
# check if function trace is requested.
#
if {$stracelevel >= 0} {
	strace $stracelevel;
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
# read in script and execute.
#
if {$arg<$argc} {
	set scriptfile [lindex $argv $arg];
} elseif {[info exists env(HUGSCRIPT)]} {
	set scriptfile $env(HUGSCRIPT);
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
	puts "\nUsing default script name 'switchdata'.";
	set scriptfile "switchdata";
}
#
puts "\nReading Data File: $scriptfile";
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
printCfgData cfgdata;
#
# lock out other HUGs
#
if {![info exists cfgdata(globals,switchid)]} {
	puts "\nERROR: hugpp: switchid not defined in cfgdata.";
	exit 2;
}
set switchid $cfgdata(globals,switchid);
#
set status [P hug $switchid];
if {$status == 0} {
	set startok 0;
	puts "\nERROR: Another HUG of switch $switchid is already running.";
	exit 2;
} else {
	set startok 1;
}
#
# determine switch state
#
set status [hug_GetSwitchState cfgdata];
if {[isNotOk $status]} {
	puts "\nERROR: hug_GetSwitchState Failed: \n$status";
	exit 2;
}
#
printCfgData cfgdata;
#
set status [generateLabPrepCmds cfgdata cmddata $huglite];
if {[isNotOk $status]} {
	puts "\nERROR: generateHugCmds Failed: \n$status";
	exit 2;
}
#
set status [printHugScript cfgdata cmddata $outfile];
if {[isNotOk $status]} {
	puts "\nERROR: printHugScript Failed: \n$status";
	exit 2;
}
#
exit 0;
