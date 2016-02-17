#!/opt/exp/bin/tclsh
#
# remote exec commands ... mostly a test program for a library.
#
###########################################################################
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
package require nhwmaint
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
	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level]";
	puts "	\[-m machine] \[-u user] \[-p passwd] ";
	puts "	\[-f cmd_file | \[cmd \[cmd_options]]]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-x - debugger";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-m machine - remote machine URI or IP (default - lts08).";
	puts "	-u user - remote machine login name (default - lcsinteg).";
	puts "	-p passwd - remote machine login passwd (default - XXXXXXXX).";
	puts "	-f filename - file containing commands to execute.";
	puts "	-t timeout - cmd time out value (default - 30 sec).";
	puts "";
	puts "remotely execute a command. a telnet session is started to the";
	puts "given machine using the given login/passwd, and the commands";
	puts "are executed. the commands are either in a file (-f) or given";
	puts "in the command line (use either file or command line, but not both together).";
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
set machine "lts08.ih.lucent.com";
set username "lcsinteg";
set userpasswd "plexus9000";
set cmdfilename "";
set tmout 30;
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
	{^-m.*} { 
		getoptval $argval machine arg; 
	}
	{^-u.*} { 
		getoptval $argval username arg; 
	}
	{^-p.*} { 
		getoptval $argval userpasswd arg; 
	}
	{^-f.*} { 
		getoptval $argval cmdfilename arg; 
	}
	{^-t.*} { 
		getoptval $argval tmout arg; 
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
if {[string length "${cmdfilename}"] > 0} {
	if {![file exists $cmdfilename]} {
		puts "\nERROR: file $cmdfilename does not exist.";
		exit 2;
	}
	if {[catch {set fd [open $cmdfilename "r"]} status ]} {
		puts "\nERROR: unable to open file ${cmdfilename}:\n${status}";
		exit 2;
	}
	for {set maxcmd 0} {[gets $fd cmdbuf] >= 0 && ![eof $fd]} { } {
		switch -regexp -- "${cmdbuf}" {
		{^[ \t]*$} {
			# skip empty line
			continue;
		}
		{^[ \t]*#} {
			# skip comment
			continue;
		}
		default { 
			# we may have a command
			set cmd [string trim $cmdbuf];
			if {[string length $cmd] <= 0} {
				continue;
			}
			incr maxcmd;
			set cmds(${maxcmd}) ${cmd};
		}
		}
	}
	set cmds(maxcmd) ${maxcmd};
} elseif {$arg<$argc} {
	# get command for command line
	for {set maxcmd 1} {$arg<$argc} {incr arg} {
		lappend cmds(${maxcmd}) [lindex $argv $arg];
	}
	set cmds(maxcmd) ${maxcmd};
} else {
	puts "\nERROR: No command or command file was given.";
	exit 2;
}
#
# do commands
#
puts "\nREMOTE EXEC CMDS:\n";
puts "Machine: ${machine}";
puts "Login  : ${username}";
puts "Passwd : ${userpasswd}\n";
#
set status [hw_maint_telnet_open spawn_id ${machine} ${username} ${userpasswd}];
if {[isNotOk $status]} {
	puts "\nERROR: unable to telnet to ${machine}:\n${status}";
	exit 2;
}
#
set maxcmd $cmds(maxcmd);
for {set cmd 1} {$cmd <= ${maxcmd}} {incr cmd} {
	puts [format "\nRUNNING CMD %03d: %s\n" $cmd $cmds(${cmd})];
	set status [hw_maint_telnet_exec spawn_id $cmds(${cmd}) ${tmout} ];
	if {[isNotOk $status]} {
		hw_maint_close ${spawn_id};
		puts "\nERROR: exec to ${machine} failed for cmd:\n${cmd}\n${status}";
		exit 2;
	}
}
#
hw_maint_close ${spawn_id};
#
# all done
#
puts "\nDone.";
exit 0;
