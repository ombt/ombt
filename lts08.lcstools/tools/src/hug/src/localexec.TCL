#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
#
# libraries
#
package require checkenv
package require getoptval
package require checkretval
package require local
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
	puts "	cmd \[options]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "";
	puts "execute a command locally. used for testing only.;
	puts "";
}
#
#########################################################################
#
# declarations and default values.
#
global env;
#
set stracelevel -1;
#
log_user 0;
#
# get cmd line options
#
global argv0;
global argv;
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
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
set logfile "/lcsl100/tmp/[file tail $argv0].[pid]";
catch { log_file -noappend -a $logfile; } ignore;
catch { system /usr/bin/chmod 777 $logfile; } ignore;
puts "\nLog file is $logfile.";
#
if {$arg>=$argc} {
	puts "\nERROR: No command given.";
	exit 2;
}
#
set cmd [lindex $argv $arg]; incr arg;
#
for {set cmdopts ""} {$arg<$argc} {incr arg} {
	lappend cmdopts [lindex $argv $arg];
}
#
switch -regexp -- ${cmd} {
{^df$} {
	set status [local_df data "${cmdopts}"];
}
{^du$} {
	set status [local_du data "${cmdopts}"];
}
default {
	puts "\nERROR: CMD ${cmd} not supported.";
	exit 2;
}
}
#
if {[isNotOk $status]} {
	puts "\nERROR: CMD ${cmd} failed: \n${status}";
	exit 2;
}
#
puts "\nCMD ${cmd} Data:\n";
foreach name [lsort [array names data]] {
	puts "data(${name}) = $data(${name})";
}
#
# all done
#
puts "\nDone.";
exit 0;
