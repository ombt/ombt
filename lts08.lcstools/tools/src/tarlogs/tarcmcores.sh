#!/opt/exp/bin/expect
#
# tar CM core files.
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
	puts "	\[-F ftp_path] \[cm_slot#\[,cpu1\[,cpu2,...]]] \[cm_slot#\[,cpu1\[,cpu2,...]]] ... ";
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
	puts "If a list of CMs is given, then the core files are collected";
	puts "for these CMs. If no CM list is given, then the list of CMs is";
	puts "generated from the hwioms relation. If the -F option is used,";
	puts "then the tar files are copied locally. If -F is not used, then";
	puts "the tar files are left on the SP.";
	puts "";
	puts "You can specify a list of CPUs along with the list of CMs.";
	puts "The syntax is to add a comma-separated list of CPUs to each CM";
	puts "that is requested. There can be no space between the CM number";
	puts "and the CPUs. A few examples is the best way to demonstrate.";
	puts "";
	puts "1) Getting core files from CM1:";
	puts "\n\t$ tarcmcores 1";
	puts "2) Getting core files from CM1, CPU 1,2,3,4:";
	puts "\n\t$ tarcmcores 1,1,2,3,4";
	puts "3) Getting core files from CM1, CPU 2,3 and CM2, CPU 1,4:";
	puts "\n\t$ tarcmcores 1,2,3 2,1,4";
	puts "4) Getting core files from CM1, and CM2, CPU 1,4:";
	puts "\n\t$ tarcmcores 1 2,1,4";
}
#
proc getcmcorefile { labid activesp activeip cm ftppath cmcpusnm } {
	upvar $cmcpusnm cmcpus;
	#
	global cm_cpu_ips;
	global sp_cpu_ips;
	global ftp_setuid;
	#
	puts "";
	puts "Retrieving $activesp CM $cm core files:";
	if {$ftppath != ""} {
		puts "";
		puts "Files will be FTP'ed to $ftppath.";
	}
	#
	telnet_to $activeip ip_id;
	#
	set cmcpuscnt [llength $cmcpus];
	#
	for {set idx 1} {$idx<$cmcpuscnt} {incr idx} {
		set cpu [lindex $cmcpus $idx];
		#
		if {![info exists cm_cpu_ips($cm,$cpu)]} {
			puts "CM $cm CPU $cpu IP: DOES NOT EXIST";
			continue;
		}
		set cm_cpu_ip $cm_cpu_ips($cm,$cpu);
		puts "CM $cm CPU $cpu IP: $cm_cpu_ip";
		#
		remote_exec $ip_id "cd /home";
		remote_exec $ip_id "rsh $cm_cpu_ip \"tar czf - /procLogCore/core /procLogCore/oldCore*\" >/home/${labid}_cm${cm}_cpu${cpu}_core.tar.gz " 600;
	}
	remote_exec $ip_id "tar cf /home/${labid}_cm${cm}_allcpu_core.tar /home/${labid}_cm${cm}_cpu*_core.tar.gz " 600;
	remote_exec $ip_id "rm -f /home/${labid}_cm${cm}_cpu*_core.tar.gz 1>/dev/null 2>&1" ;
	#
	close_telnet $ip_id;
	#
	if {$ftppath != ""} {
		set ftp_setuid 0;
		puts "";
		puts "FTP'ing remote file: /home/${labid}_cm${cm}_allcpu_core.tar";
		puts "To local file: ${ftppath}/${labid}_cm${cm}_allcpu_core.tar";
		ftp_get_binary_file $activeip "/home/${labid}_cm${cm}_allcpu_core.tar" "${ftppath}/${labid}_cm${cm}_allcpu_core.tar";
		ftp_chmod "${ftppath}/${labid}_cm${cm}_allcpu_core.tar" 0777;
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
	puts "tarcmlogs: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "tarcmlogs: no IPs found for labid $labid.";
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
		puts "exiting tarcmcores.";
		exit 0;
	}
}
#
set ipdata [split $activeip " \t"];
set activesp [lindex $ipdata 0];
set activeip [lindex $ipdata 1];
#
if {[dbselect obuf hwioms "labid req ^$labid\$ and type req ^cm\$" "iom" ] != 0} {
	puts "tarcmcores: dbselect of relation 'hwioms' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "tarcmcores: no CMs found for labid $labid.";
	exit 2;
}
foreach cm $obuf {
	set cms($cm) $cm;
}
#
# get cores files
#
puts "";
puts "Retrieving CM core files for $labid:";
puts "";
puts "All core files are under $activesp /home.";
#
if {$arg<$argc} {
	for { } {$arg<$argc} {incr arg} {
		set cmcpulist [lindex $argv $arg];
		set cmcpus [split $cmcpulist ","];
		#
		if {[llength $cmcpus] < 1} {
			puts "";
			puts "tarcmlogs: CM/CPU list is invalid: $cmcpulist";
			exit 2;
		} elseif {[llength $cmcpus] == 1} {
			# do all CPUs
			set cm [lindex $cmcpus 0];
			set cmcpus [list $cm 1 2 3 4];
		} else {
			set cm [lindex $cmcpus 0];
		}
		#
		if {[info exists cmseen($cm)]} {
			# this is the second time this cm was 
			# given. the previous log file will
			# will be overwritten, issue warnin.
			# 
			puts "\nWARNING: this is the second time CM $cm";
			puts "was entered. the previous tar file would";
			puts "be over-written. skipping this CM."
			continue;
		}
		set cmseen($cm) $cm;
		#
		if {![info exists cms($cm)]} {
			puts "";
			puts "CM $cm DOES NOT EXIST. SKIPPING IT.";
			continue;
		}
		getcmcorefile $labid $activesp $activeip $cm $ftppath cmcpus;
	}
} else {
	for {set cm 1} {$cm<=20} {incr cm} {
		if {![info exists cms($cm)]} {
			continue;
		}
		set cmcpus [list $cm 1 2 3 4];
		getcmcorefile $labid $activesp $activeip $cm $ftppath cmcpus;
	}
}
#
puts "exiting tarcmcores.";
exit 0

