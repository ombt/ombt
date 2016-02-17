#!/opt/exp/bin/expect
#
# remotely update files on an SP after a reload.
#
#########################################################################
#
# libraries
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
source $env(LCSTOOLSLIB)/chooseone
source $env(LCSTOOLSLIB)/lcsftp
source $env(LCSTOOLSLIB)/lcstelnet
source $env(LCSTOOLSLIB)/logging
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-S trace level] \[-V]";
	puts "	\[-l logfile] \[-u username] \[-p passwd] ";
	puts "	\[-m] \[-t] \[-s|-d] \[-D|-C] \[-h] \[-c num] \[-a] \[-A] \[-B] \[labid]";
	puts "where:";
	puts "	-? - print usage";
	puts "	-x - enable TCL debugger";
	puts "	-V - set verbose";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-l logfile - log all output to a logfile";
	puts "	-u username - set username for logins";
	puts "	-p passwd - passwd for username";
	puts "";
	puts "	-m - update MOTD with load and labid";
	puts "	-s - simplex the lab";
	puts "	-d - duplex the lab";
	puts "	-D - disable Disk Clean Up";
	puts "	-C - enable Disk Clean Up";
	puts "	-h - add extra entries to /etc/hosts (ihgp, lts08, xtm*)";
	puts "	-c num - set number of simultaneous CPY-MEMs to 'num'";
	puts "	-t - set CPU date/time equal to LSP date/time";
	puts "	-R - disable AllowRebootOnShutdown (set flag to 0)."
	puts "	-i - update /etc/fstab to import /build from lcsbld1";
	puts "";
	puts "	-a - set options to '-i -t -m -d -D -h -c 10'";
	puts "";
	puts "	-A - update SP-A ONLY; default is both SPs.";
	puts "	-B - update SP-B ONLY; default is both SPs.";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	labid is read from variable LABID, unless given.";
}
#
#########################################################################
#
# default values
#
logusage postlcsreload;
#
set username "root";
set userpasswd "plexus9000";
set maxlogons 5;
set stracelevel -1;
set logfile "";
set labid "";
set labmode "";
set hostfiles "";
set motd "";
set diskcleanup "";
set cpymem "";
set cpymemnum 0;
set doSPA 1;
set doSPB 1;
set setdate 0;
set disableallowreboot "";
set importbld1 "";
#
# get cmd line options
#
log_user 0;
#
puts "\npostlcsreload options:";
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-\?} { 
		usage; 
		logmsg postlcsreload "END: print usage from postlcsreload.";
		exit 0; 
	}
	{^-x} { debug -now; }
	{^-V} { log_user 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-a} { 
		set labmode " -d"; puts "run duplex mode.";
		set diskcleanup " -D"; puts "disable disk cleanup.";
		set doSPA 1; puts "update SP-A.";
		set doSPB 1; puts "update SP-B.";
		set hostfiles " -h"; puts "update /etc/hosts file.";
		set motd " -m"; puts "update message-of-the-day, /etc/motd.";
		set cpymem " -c 10"; puts "set max cpy-mem to 10.";
		set setdate 1; puts "set CPU date/time = LSP date/time.";
		set importbld1 " -i"; puts "import/mount lcsbld1 /build.";
	}
	{^-s} { set labmode " -s"; puts "run simplex mode."; }
	{^-i} { set importbld1 " -i"; puts "import/mount lcsbld1 /build."; }
	{^-d} { set labmode " -d"; puts "run duplex mode."; }
	{^-C} { set diskcleanup " -C"; puts "enable disk cleanup."; }
	{^-D} { set diskcleanup " -D"; puts "disable disk cleanup."; }
	{^-A} { set doSPA 1; set doSPB 0; puts "update SP-A only."; }
	{^-B} { set doSPA 0; set doSPB 1; puts "update SP-B only."; }
	{^-h} { set hostfiles " -h"; puts "update /etc/hosts file."; }
	{^-m} { set motd " -m"; puts "update /etc/motd."; }
	{^-t} { set setdate 1; puts "set CPU date/time = LSP date/time."; }
	{^-R} { 
		set disableallowreboot " -R"; 
		puts "Disable AllowRebootOnShutDown.";
	}
	{^-c.*} { 
		getoptval $argval cpymemnum arg; 
		set cpymem " -c $cpymemnum"; 
		puts "set max cpy-mem to $cpymemnum.";
	}
	{^--} { incr arg; break; }
	{^-.*} { 
		puts "ERROR: Unknown option: $argval\n"; 
		logmsg postlcsreload "ERROR: Unknown option: $argval"; 
		usage; 
		exit 2 
	}
	default { break; }
	}
}
#
# debugging and sanity checks.
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "ERROR: Logging failed for file $logfile:\n$status";
		logmsg postlcsreload "ERROR: Logging failed for file $logfile:\n$status";
		exit 2;
	}
	log_user 1;
}
#
checkenv;
#
#
# get ip addresses for labid.
#
if {$arg<$argc} {
	set labid [lindex $argv $arg];
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
} else {
	puts "ERROR: LABID is neither set nor given.";
	logmsg postlcsreload "ERROR: LABID is neither set nor given.";
	exit 2;
}
puts "\nStarting postlcsreload of lab $labid:\n";
logmsg postlcsreload "Starting postlcsreload of lab $labid:";
#
# get SP IPs
#
if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
	puts "ERROR: postlcsreload: dbselect of relation 'chassis' failed.";
	logmsg postlcsreload "ERROR: postlcsreload: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "ERROR: postlcsreload: no IPs found for labid $labid.";
	logmsg postlcsreload "ERROR: postlcsreload: no IPs found for labid $labid.";
	exit 2;
}
#
set ips [lindex $obuf 0];
set ipdata [split $ips " \t"];
set cpu_a_ip [lindex $ipdata 0];
set cpu_b_ip [lindex $ipdata 1];
#
# get command line 
#
set cmd "/bin/postlcsreload$labmode$diskcleanup$hostfiles$cpymem$disableallowreboot$importbld1";
#
# update SPs.
#
if {$doSPA != 0} {
	set spacmd "$cmd";
	if {$motd == " -m"} {
		set spacmd "$cmd -m \"$labid SP-A\"";
	}
	puts "exec'ing cmd $spacmd on SP-A ...";
	logmsg postlcsreload "exec'ing cmd $spacmd on SP-A ...";
	#
	# send over files to SP.
	#
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/iprecover" "/bin/iprecover";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/removedefaultgw" "/bin/removedefaultgw";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSHUGSCRIPTS)/getcorelog" "/bin/getcorelog";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/mount_nfs" "/bin/mount_nfs";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/iomsfui" "/bin/iomsfui";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/mysfui" "/bin/mysfui";
	ftp_put_binary_file $cpu_a_ip "$env(LCSTOOLSLYNXBIN)/rm" "/bin/rm";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/lf" "/bin/lf";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/findcpucores" "/bin/findcpucores";
	ftp_put_ascii_file $cpu_a_ip "$env(LCSTOOLSBIN)/remotepostlcsreload" "/bin/postlcsreload";

	#
	# remotely exec commands
	#
	telnet_to $cpu_a_ip spaid;
	remote_exec $spaid "/bin/chmod 755 /bin/iprecover";
	remote_exec $spaid "/bin/chmod 755 /bin/removedefaultgw";
	remote_exec $spaid "/bin/chmod 755 /bin/getcorelog";
	remote_exec $spaid "/bin/chmod 755 /bin/mount_nfs";
	remote_exec $spaid "/bin/chmod 755 /bin/iomsfui";
	remote_exec $spaid "/bin/chmod 755 /bin/mysfui";
	remote_exec $spaid "/bin/chmod 755 /bin/rm";
	remote_exec $spaid "/bin/chmod 755 /bin/lf";
	remote_exec $spaid "/bin/chmod 755 /bin/findcpucores";
	remote_exec $spaid "/bin/chmod 755 /bin/postlcsreload";
	remote_exec $spaid "$spacmd";
	remote_exec $spaid "grep -v 'set.*-o.*emacs' /.profile >/.profile.[pid]";
	remote_exec $spaid "mv /.profile.[pid] /.profile";
	remote_exec $spaid "grep -v 'set.*-o.*vi' /.profile >/.profile.[pid]";
	remote_exec $spaid "echo 'set -o vi' >>/.profile.[pid]";
	remote_exec $spaid "mv /.profile.[pid] /.profile";
	if {$setdate != 0} {
		set env(TZ) "US/Central";
		set lsptime [clock format [clock seconds] -format "%Y%m%d%H%M"];
		puts "Setting CPU TIME to LSP TIME: $lsptime";
		remote_exec $spaid "/bin/date -d1 -z360 ${lsptime}";
		remote_exec $spaid "grep -v 'date.*-d1.*-z360' /.profile >/.profile.[pid]";
		remote_exec $spaid "echo '/bin/date -d1 -z360' >>/.profile.[pid]";
		remote_exec $spaid "mv /.profile.[pid] /.profile";
	}
	remote_exec $spaid "rm -f /.profile.*";
	close_telnet $spaid;
}
if {$doSPB != 0} {
	set spbcmd "$cmd";
	if {$motd == " -m"} {
		set spbcmd "$cmd -m \"$labid SP-B\"";
	}
	puts "exec'ing cmd $spbcmd on SP-B ...";
	logmsg postlcsreload "exec'ing cmd $spbcmd on SP-B ...";
	#
	# send over files to SP.
	#
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/iprecover" "/bin/iprecover";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/removedefaultgw" "/bin/removedefaultgw";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSHUGSCRIPTS)/getcorelog" "/bin/getcorelog";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/mount_nfs" "/bin/mount_nfs";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/iomsfui" "/bin/iomsfui";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/mysfui" "/bin/mysfui";
	ftp_put_binary_file $cpu_b_ip "$env(LCSTOOLSLYNXBIN)/rm" "/bin/rm";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/lf" "/bin/lf";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/findcpucores" "/bin/findcpucores";
	ftp_put_ascii_file $cpu_b_ip "$env(LCSTOOLSBIN)/remotepostlcsreload" "/bin/postlcsreload";
	#
	# remotely exec commands
	#
	telnet_to $cpu_b_ip spbid;
	remote_exec $spbid "/bin/chmod 755 /bin/iprecover";
	remote_exec $spbid "/bin/chmod 755 /bin/removedefaultgw";
	remote_exec $spbid "/bin/chmod 755 /bin/getcorelog";
	remote_exec $spbid "/bin/chmod 755 /bin/mount_nfs";
	remote_exec $spbid "/bin/chmod 755 /bin/iomsfui";
	remote_exec $spbid "/bin/chmod 755 /bin/mysfui";
	remote_exec $spbid "/bin/chmod 755 /bin/rm";
	remote_exec $spbid "/bin/chmod 755 /bin/lf";
	remote_exec $spbid "/bin/chmod 755 /bin/findcpucores";
	remote_exec $spbid "/bin/chmod 755 /bin/postlcsreload";
	remote_exec $spbid "$spbcmd";
	#
	remote_exec $spbid "grep -v 'set.*-o.*emacs' /.profile >/.profile.[pid]";
	remote_exec $spbid "mv /.profile.[pid] /.profile";
	remote_exec $spbid "grep -v 'set.*-o.*vi' /.profile >/.profile.[pid]";
	remote_exec $spbid "echo 'set -o vi' >>/.profile.[pid]";
	remote_exec $spbid "mv /.profile.[pid] /.profile";
	#
	if {$setdate != 0} {
		set env(TZ) "US/Central";
		set lsptime [clock format [clock seconds] -format "%Y%m%d%H%M"];
		puts "Setting CPU TIME to LSP TIME: $lsptime";
		remote_exec $spbid "/bin/date -d1 -z360 ${lsptime}";
		remote_exec $spbid "grep -v 'date.*-d1.*-z360' /.profile >/.profile.[pid]";
		remote_exec $spbid "echo '/bin/date -d1 -z360' >>/.profile.[pid]";
		remote_exec $spbid "mv /.profile.[pid] /.profile";
	}
	remote_exec $spbid "rm -f /.profile.*";
	close_telnet $spbid;
}
#
logmsg postlcsreload "END: success from postlcsreload.";
exit 0;

