#!/opt/exp/bin/expect
#
# tar non-CM log files.
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
	puts "	\[-A | -B] \[-u username] \[-p passwd] \[-L labid] ";
	puts "	\[-F ftp_path] \[iom_slot#] \[iom_slot#] ... ";
	puts "";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-V - verbose mode";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-l logfile - log all output to a logfile";
	puts "	-A - choose SP-A";
	puts "	-B - choose SP-B";
	puts "	-L labid - set labid";
	puts "	-F ftp_path - local path to store TAR files";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "";
	puts "labid is read from variable LABID, unless given.";
	puts "If a list of IOMs is given, then the logs files are collected";
	puts "for these CMs. If no IOM list is given, then the list of IOMs is";
	puts "generated from the hwioms relation. If the -F option is used,";
	puts "then the tar files are copied locally. If -F is not used, then";
	puts "the tar files are left on the SP.";
	puts "";
	puts "Examples:";
	puts "";
	puts "1) Getting log files from IOM1, IOM2:";
	puts "\n\t$ tariomlogs 1 2";
	puts "2) Getting log files from all IOMs:";
	puts "\n\t$ tariomlogs";
}
#
proc getiomlogfile { labid activesp activeip iom ftppath } {
	#
	global iom_cpu_ips;
	global sp_cpu_ips;
	global ftp_setuid;
	#
	puts "";
	puts "Retrieving $activesp IOM $iom log files:";
	if {$ftppath != ""} {
		puts "";
		puts "Files will be FTP'ed to $ftppath.";
	}
	#
	telnet_to $activeip spawn_id;
	#
	if {![info exists iom_cpu_ips($iom)]} {
		puts "IOM $iom IP: DOES NOT EXIST";
		return 0;
	}
	set iom_cpu_ip $iom_cpu_ips($iom);
	puts "IOM $iom: $iom_cpu_ip";
	#
	# remote_exec $spawn_id "cd /home";
	# remote_exec $spawn_id "rsh $iom_cpu_ip \"tar czf - /application/*.out\" >/home/${labid}_iom${iom}_log.tar.gz " 600;
	#
	close_telnet $spawn_id;
	#
	if {$ftppath != ""} {
		set ftp_setuid 0;
		puts "";
		puts "FTP'ing remote file: /home/${labid}_iom${iom}_log.tar";
		puts "To local file: ${ftppath}/${labid}_iom${iom}_log.tar";
		ftp_get_binary_file $activeip "/home/${labid}_iom${iom}_log.tar" "${ftppath}/${labid}_iom${iom}_log.tar";
		ftp_chmod "${ftppath}/${labid}_iom${iom}_log.tar" 0777;
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
set sp "";
set ftppath "";
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
		#
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
# get ip addresses for labid.
#
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
	puts "tariomlogs: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "tariomlogs: no IPs found for labid $labid.";
	exit 2;
}
#
set ips [lindex $obuf 0];
set ipdata [split $ips " \t"];
set cpu_a_ip [lindex $ipdata 0];
set cpu_b_ip [lindex $ipdata 1];
#
if {$sp == "A"} {
	set activeip "SP-A $cpu_a_ip";
} elseif {$sp == "B"} {
	set activeip "SP-B $cpu_b_ip";
} else {
	lappend choices "SP-A $cpu_a_ip" "SP-B $cpu_b_ip";
	set choices [linsert $choices 0 "QUIT"];
	chooseone "Choose active SP-IP: " choices activeip;
	if {$activeip == "QUIT"} {
		puts "exiting tariomlogs.";
		exit 0;
	}
}
#
set ipdata [split $activeip " \t"];
set activesp [lindex $ipdata 0];
set activeip [lindex $ipdata 1];
#
# get every IOM that is NOT a CM.
#
if {[dbselect obuf hwioms "labid req ^$labid\$ and type rne ^cm\$" "iom" ] != 0} {
	puts "tariomlogs: dbselect of relation 'hwioms' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "tariomlogs: no NON-CM IOMs found for labid $labid.";
	exit 2;
}
foreach iom $obuf {
	set ioms($iom) $iom;
}
#
# get log files
#
puts "";
puts "Retrieving IOM log files for $labid:";
puts "";
puts "All log files are under $activesp /home.";
#
if {$arg<$argc} {
	for { } {$arg<$argc} {incr arg} {
		set iom [lindex $argv $arg];
		#
		if {![info exists ioms($iom)]} {
			puts "";
			puts "IOM $iom DOES NOT EXIST. SKIPPING IT.";
			continue;
		}
		getiomlogfile $labid $activesp $activeip $iom $ftppath;
	}
} else {
	for {set iom 1} {$iom<=20} {incr iom} {
		if {![info exists ioms($iom)]} {
			continue;
		}
		getiomlogfile $labid $activesp $activeip $iom $ftppath;
	}
}
#
puts "exiting tariomlogs.";
exit 0
