#!/opt/exp/bin/tclsh
#
# audit all labs for various maladies. now, stick your tongue out and cough ...
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

	puts "Usage: [file tail $argv0] \[-\?] \[-x] \[-f] \[-V] \[-S trace level]";
	puts "	\[-U username] \[-P passwd] \[ -a | \[labid \[labid2 ...]] ]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-U username - Lynx-OS login (default is root)";
	puts "	-P passwd - Lynx-OS passwd (default is plexus9000)";
	puts "	-a - audit all labids";
	puts "	-f - fix problems";
	puts "";
	puts "Check IPs on all labs or on the given labs. If the -f flag";
	puts "is given, then logon using pxmonitor and fix the Telica_IP";
	puts "file. Run iprecover to recreate the mgt0 IP interface and";
	puts "routing.";
	puts "";
}
#
#########################################################################
#
# declarations and default values.
#
global env;
set stracelevel -1;
set lynxusername "root";
set lynxpasswd "plexus9000";
set allflag 0;
set fixflag 0;
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
	{^-a} {
		# audit all labids
		set allflag 1;
	}
	{^-f} {
		# fix any problems found
		set fixflag 1;
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
#
checkenv;
#
global argv0;
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
# open log file
#
set logfile "/lcsl100/tmp/auditsps.[pid]";
catch { log_file -noappend -a $logfile; } ignore;
catch { system /usr/bin/chmod 777 $logfile; } ignore;
puts "\nLog file is $logfile.";
#
# init data.
#
set status [initCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: initCleiData Failed: \n$status";
	exit 2;
}
#
# generate a list of labids.
#
if {$allflag} {
	# get list of labids from chassis relation
	set status [dbselect tuples chassis "" "labid cpu_a_ip cpu_b_ip"];
	if {[isNotOk $status]} {
		puts "\nERROR: dbselect of relation 'chassis' failed: \n${status}";
		exit 2;
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		puts "\nERROR: No IPs found in 'chassis' relation.";
		exit 2;
	}
	#
	set maxi [llength $tuples];
	#
	for {set i 0} {$i<$maxi} {incr i} {
		set tuple [lindex $tuples $i];
		set fields [split $tuple " \t"];
		set labid [lindex $fields 0];
		set sp_a_ip [lindex $fields 1];
		set sp_b_ip [lindex $fields 2];
		#
		if {[info exists spdata($labid)]} {
			continue;
		} else {
			set spdata($labid) $labid;
			set spdata($labid,spaip) $sp_a_ip;
			set spdata($labid,spbip) $sp_b_ip;
			set spdata($labid,lynx-a) $sp_a_ip;
			set spdata($labid,lynx-b) $sp_b_ip;
			lappend spdata(labids) $labid;
		}
	}
} elseif {$arg<$argc} {
	# get labids from command line
	for { } {$arg<$argc} {incr arg} {
		set labid [lindex $argv $arg];
		set status [dbselect tuples chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip"];
		if {[isNotOk $status]} {
			puts "\nERROR: dbselect of relation 'chassis' failed: \n${status}";
			exit 2;
		}
		if {![info exists tuples] || [llength $tuples] == 0} {
			puts "\nWARNING: No IPs found for labid $labid. Unknown labid. Skipping.";
			continue;
		}
		#
		set tuple [lindex $tuples 0];
		set fields [split $tuple " \t"];
		set sp_a_ip [lindex $fields 0];
		set sp_b_ip [lindex $fields 1];
		#
		if {[info exists spdata($labid)]} {
			continue;
		} else {
			set spdata($labid) $labid;
			set spdata($labid,spaip) $sp_a_ip;
			set spdata($labid,spbip) $sp_b_ip;
			set spdata($labid,lynx-a) $sp_a_ip;
			set spdata($labid,lynx-b) $sp_b_ip;
			lappend spdata(labids) $labid;
		}
	}
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
	set status [dbselect tuples chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip"];
	if {[isNotOk $status]} {
		puts "\nERROR: dbselect of relation 'chassis' failed: \n${status}";
		exit 2;
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		puts "\nWARNING: No IPs found for labid $labid. Unknown labid.";
		exit 2;
	}
	#
	set tuple [lindex $tuples 0];
	set fields [split $tuple " \t"];
	set sp_a_ip [lindex $fields 0];
	set sp_b_ip [lindex $fields 1];
	#
	lappend spdata(labids) $labid;
	#
	set spdata($labid) $labid;
	set spdata($labid,spaip) $sp_a_ip;
	set spdata($labid,spbip) $sp_b_ip;
	set spdata($labid,lynx-a) $sp_a_ip;
	set spdata($labid,lynx-b) $sp_b_ip;
} else {
	puts "\nERROR: No labids found.";
	exit 2;
}
#
# check sps
#
if {[info exists spdata(labids)]} {
	foreach labid [lsort $spdata(labids)] {
		if {[regexp -- {^ihlcs} $labid] != 1} {
			puts "\nSkipping lab ${labid} (does not use pxmonitor):";
			continue;
		}
		puts "\nChecking Lab ${labid}:";
		puts "SP-A IP: $spdata($labid,spaip)";
		puts "SP-B IP: $spdata($labid,spbip)";
		#
		# start checking the SPs.
		#
		set status [hw_maint_audit_sps ${labid} spdata ${lynxusername} ${lynxpasswd} $fixflag ];
		if {[isNotOk $status]} {
			puts "\nERROR: hw_maint_audit_sps failed: \n${status}";
			#exit 2;
			puts "Continue to next Labd ID.";
		}
	}
} else {
	puts "\nNo Labids found.";
}
#
puts "\nDone.";
exit 0;
