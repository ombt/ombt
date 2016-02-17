#!/opt/exp/bin/tclsh
#
# create HUG scripts.
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
package require oscripts
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

	puts "usage: [file tail $argv0] \[-\?] \[-V] \[-T] \[-l]";
	puts "	\[-S trace level] \[-L logfile] \[-o outfile] \[data_filename]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-L filename - log file name";
	puts "	-o filename - script output file";
	puts "	-T - print template config data file";
	puts "	-l - HUG lite, only minimal commands in prehug section";
	puts "";
	puts "Data file contains data describing switch about to be HUG'ed."
	puts "Default data file name is 'switchdata'. If script output file is";
	puts "not given, then the script is written to stdout.";
	puts "";
}
#
proc printTemplate { } {
	puts "#";
	puts "# sample global data used by HUG preprocessor.";
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
	puts "	load {";
	puts "		ftpmachine 135.111.82.6";
	puts "		ftplogin lcsinteg";
	puts "		ftppasswd plexus9000";
	puts "		textdir /l148/text/BP-5-3-0-6";
	puts "		branch BP-5-3-0-6";
	puts "		cpuload 5.3.0.6";
	puts "		# delete the IOM types that are NOT updated.";
	puts "		images {";
	puts "			types ana atmds3 cm cpu ds1 ds1_2 ds3 e1 ena ena2 octds3 octds3_2 octds3_3 pna tdmoc trids3 trids3_3 voip voip6 vs2 vs3";
	puts "			ana 5.3.0.6_ana.tar.gz";
	puts "			atmds3 5.3.0.6_atmds3.tar.gz";
	puts "			cm 5.3.0.6_cm.tar.gz";
	puts "			cpu 5.3.0.6_cpu.tar.gz";
	puts "			ds1 5.3.0.6_ds1.tar.gz";
	puts "			ds1_2 5.3.0.6_ds1_2.tar.gz";
	puts "			ds3 5.3.0.6_ds3.tar.gz";
	puts "			e1 5.3.0.6_e1.tar.gz";
	puts "			ena 5.3.0.6_ena.tar.gz";
	puts "			ena2 5.3.0.6_ena2.tar.gz";
	puts "			octds3 5.3.0.6_octds3.tar.gz";
	puts "			octds3_2 5.3.0.6_octds3_2.tar.gz";
	puts "			octds3_3 5.3.0.6_octds3_3.tar.gz";
	puts "			pna 5.3.0.6_pna.tar.gz";
	puts "			tdmoc 5.3.0.6_tdmoc.tar.gz";
	puts "			trids3 5.3.0.6_trids3.tar.gz";
	puts "			trids3_3 5.3.0.6_trids3_3.tar.gz";
	puts "			voip 5.3.0.6_voip.tar.gz";
	puts "			voip6 5.3.0.6_voip6.tar.gz";
	puts "			vs2 5.3.0.6_vs2.tar.gz";
	puts "			vs3 5.3.0.6_vs3.tar.gz";
	puts "			anaload 5.3.0.6";
	puts "			atmds3load 5.3.0.6";
	puts "			cmload 5.3.0.6";
	puts "			cpuload 5.3.0.6";
	puts "			ds1load 5.3.0.6";
	puts "			ds1_2load 5.3.0.6";
	puts "			ds3load 5.3.0.6";
	puts "			e1load 5.3.0.6";
	puts "			enaload 5.3.0.6";
	puts "			ena2load 5.3.0.6";
	puts "			octds3load 5.3.0.6";
	puts "			octds3_2load 5.3.0.6";
	puts "			octds3_3load 5.3.0.6";
	puts "			pnaload 5.3.0.6";
	puts "			tdmocload 5.3.0.6";
	puts "			trids3load 5.3.0.6";
	puts "			trids3_3load 5.3.0.6";
	puts "			voipload 5.3.0.6";
	puts "			voip6load 5.3.0.6";
	puts "			vs2load 5.3.0.6";
	puts "			vs3load 5.3.0.6";
	puts "		}";
	puts "	}";
	puts "	#";
	puts "	backup {";
	puts "		machine 135.111.82.6";
	puts "		login lcsinteg";
	puts "		passwd plexus9000";
	puts "		dir /tmp";
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
	{^-l} { set huglite 1; }
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
set status [generateHugCmds cfgdata cmddata $huglite];
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
