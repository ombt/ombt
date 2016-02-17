#!/opt/exp/bin/expect
#
# tar SP log files.
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
source $env(LCSTOOLSLIB)/lcsftp
#
# data
#
source $env(LCSTOOLSLIB)/mgcips
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;
	#
	puts "";
	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-V]";
	puts "	\[-S trace level] \[-l logfile] ";
	puts "	\[-A | -B] \[-u username] \[-p passwd] ";
	puts "	\[-N] \[-F ftp_path] \[labid \[labid2 ...]]";
	puts "";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-V - verbose mode";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-l logfile - log all output to a logfile";
	puts "	-A - choose SP-A";
	puts "	-B - choose SP-B";
	puts "	-F ftp_path - local path to store TAR files";
	puts "	-N - tar only the newest log files";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "";
	puts "labid is read from variable LABID, unless it is given.";
	puts "If the -F option is used, then the tar files are copied";
	puts "locally. If -F is not used, then the tar files are left ";
	puts "on the SP in /home. If neither -A or -B is given, then";
	puts "the log files are collected on both SPs.";
}
#
proc getsplogfile { labid activesp activeip ftppath newestonly } {
	global sp_cpu_ips;
	global ftp_setuid;
	#
	puts "";
	puts "Retrieving $activesp log files:";
	if {$ftppath != ""} {
		puts "";
		puts "Files will be FTP'ed to $ftppath.";
	}
	#
	telnet_to $activeip ip_id;
	#
	remote_exec $ip_id "cd /home";
	if {${newestonly}} {
		remote_exec $ip_id "tar czvf /home/${labid}_${activesp}_log.tar.gz /Telica/swCPU/*/log /home/Telica/swCPU/*/log /home1/Telica/swCPU/*/log /Telica/dbCurrent/*log /Telica/History" 600;
	} else {
		remote_exec $ip_id "tar czvf /home/${labid}_${activesp}_log.tar.gz /Telica/swCPU/*/log /Telica/swCPU/*/old* /home/Telica/swCPU/*/log /home/Telica/swCPU/*/old* /home1/Telica/swCPU/*/log /home1/Telica/swCPU/*/old* /Telica/dbCurrent/*log /Telica/dbCurrent/*bak /Telica/History" 600;
	}
	#
	close_telnet $ip_id;
	#
	if {$ftppath != ""} {
		set ftp_setuid 0;
		puts "";
		puts "FTP'ing remote file: /home/${labid}_${activesp}_log.tar.gz";
		puts "To local file: ${ftppath}/${labid}_${activesp}_log.tar.gz";
		ftp_get_binary_file $activeip "/home/${labid}_${activesp}_log.tar.gz" "${ftppath}/${labid}_${activesp}_log.tar.gz";
		ftp_chmod "${ftppath}/${labid}_${activesp}_log.tar.gz" 0777;
	}
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
set sp "both";
set ftppath "";
set newestonly 0;
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
	{^-A} { set sp "A"; }
	{^-B} { set sp "B"; }
	{^-N} { set newestonly 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-L.*} { getoptval $argval labid arg; }
	{^-F.*} { 
		# since FTP does not like the path ".", 
		# convert it to a full path via pwd.
		#
		getoptval $argval ftppath arg;
		if {![file exists "${ftppath}"]} {
			catch { file mkdir ${ftppath}; } ignore;
		}
		if {![file isdirectory "${ftppath}"]} {
			puts "";
			puts "WARNING: ${ftppath} is not a directory.";
			puts "Will not FTP any files.";
			set ftppath "";
		} else {
			cd $ftppath;
			set ftppath [pwd];
		}
	}
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# function tracing
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
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
checkenv;
#
# get log files for the given labids
#
if {$arg >= $argc} {
	if {[info exists env(LABID)]} {
		set labid $env(LABID);
	} else {
		puts "LABID is neither set nor given.";
		exit 2;
	}
	#
	if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
		puts "tarsplogs: dbselect of relation 'chassis' failed.";
		exit 2;
	}
	if {![info exists obuf] || [llength obuf] == 0} {
		puts "tarsplogs: no IPs found for labid $labid.";
		exit 2;
	}
	#
	set ips [lindex $obuf 0];
	set ipdata [split $ips " \t"];
	set cpu_a_ip [lindex $ipdata 0];
	set cpu_b_ip [lindex $ipdata 1];
	#
	if {$sp == "A" || $sp == "both"} {
		puts "";
		puts "Retrieving SP-A log files for $labid:";
		puts "All log files are under SP-A /home.";
		getsplogfile $labid "SP-A" $cpu_a_ip $ftppath $newestonly;
	} 
	if {$sp == "B" || $sp == "both"} {
		puts "";
		puts "Retrieving SP-B log files for $labid:";
		puts "All log files are under SP-B /home.";
		getsplogfile $labid "SP-B" $cpu_b_ip $ftppath $newestonly;
	}
} else {
	for { } {$arg<$argc} {incr arg} {
		set labid [lindex $argv $arg];
		if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
			puts "";
			puts "tarsplogs: dbselect of relation 'chassis' failed.";
			exit 2;
		}
		if {![info exists obuf] || [llength obuf] == 0} {
			puts "";
			puts "tarsplogs: no IPs found for labid $labid.";
			puts "skipping labid $labid";
			continue;
		}
		#
		set ips [lindex $obuf 0];
		set ipdata [split $ips " \t"];
		set cpu_a_ip [lindex $ipdata 0];
		set cpu_b_ip [lindex $ipdata 1];
		unset obuf;
		#
		if {$sp == "A" || $sp == "both"} {
			puts "";
			puts "Retrieving SP-A log files for $labid:";
			puts "All log files are under SP-A /home.";
			getsplogfile $labid "SP-A" $cpu_a_ip $ftppath $newestonly;
		} 
		if {$sp == "B" || $sp == "both"} {
			puts "";
			puts "Retrieving SP-B log files for $labid:";
			puts "All log files are under SP-B /home.";
			getsplogfile $labid "SP-B" $cpu_b_ip $ftppath $newestonly;
		}
	}
}
#
puts "";
puts "exiting tarsplogs.";
#
exit 0
