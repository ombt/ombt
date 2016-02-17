#!/opt/exp/bin/expect
#
# audit files on one or both SPs. compare the files on the SPs with
# the files in the repository.
#
#########################################################################
#
# library
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/lcsftp
source $env(LCSTOOLSLIB)/lcstelnet
source $env(LCSTOOLSLIB)/logging
#
# signal handlers
#
trap SIG_IGN TSTP;
#
# functions
#
proc myflush { } {
	global user_spawn_id;
	flush $user_spawn_id;
}
#
proc vputs { msg } {
	global verbose;
	if {$verbose} {
		puts "\n$msg";
		myflush;
	}
}
#
proc ucondclose { } {
	catch { close; } status;
}
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-S trace level] \[-V] \[-u username] \[-p passwd] \[-A] \[-B] \[-c or -i] \[-L] \[labid]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "";
	puts "	-A - audit SP-A only, do not audit SP-B";
	puts "	-B - audit SP-B only, do not audit SP-A";
	puts "	-c - audit CPU files only";
	puts "	-i - audit IOM files only";
	puts "	-a - audit all IOM files in load";
	puts "	-L - get CPU load";
	puts "	-U - uncondtional rebuild of TOC.";
	puts "";
	puts "	default UNIX username = $username";
	puts "	default UNIX passwd = $userpasswd";
	puts "	default labid is from variable LABID";
	puts "";
	puts "If both -A and -B are given, the last option given in the";
	puts "cmd line takes effect. By default, both SPs are audited.";
	puts "The -a options causes all the IOM files in the load to be";
	puts "audited. If -a is not used and the chassis2ioms relation";
	puts "contains a list of the IOM types for the given labid and ";
	puts "branch, then only the list of IOM types in chassis2ioms";
	puts "is audited. If chassis2ioms does not contains data for the";
	puts "branch and labid, then all IOMs are audited.";
	puts "";
}
#
proc getlabidips { labid iparray } {
	global env;
	global expect_out;
	global spawn_id;
	upvar $iparray ips;
	#
	set errmsg "";
	if {[catch {
		cd $env(LCSTOOLSDATA);
		set timeout 3;
		spawn -noecho "uprintf" -q "-f%s %s\n" cpu_a_ip cpu_b_ip in chassis where labid req ^$labid\$;
		expect {
		eof {
			set ips(SP_A_IP) [lindex $expect_out(buffer) 0];
			set ips(SP_B_IP) [lindex $expect_out(buffer) 1];
			vputs "ips(SP_A_IP) = $ips(SP_A_IP)";
			vputs "ips(SP_B_IP) = $ips(SP_B_IP)";
		}
		timeout {
			puts "\ntimeout during uprintf ...";
			exit 2;
		}
		};
	} errmsg] != 0} {
		if {[string length $errmsg] > 0} {
			puts "\nCAUGHT ERROR IN getlabidips: $errmsg";
		} else {
			puts "\nCAUGHT ERROR IN getlabidips.";
		}
		exit 2;
	}
}
#
proc lcsping { ip } {
	global timeout;
	set status dead;
	set savetimeout $timeout;
	#
	set timeout 3;
	spawn ping $ip;
	#
	expect {
	"alive" { set status alive; } 
	timeout { set status dead; }
	}
	#
	catch { close; wait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
#
proc lcsunixactive { ip } {
	global username;
	global userpasswd;
	global timeout;
	#
	set status dead;
	set savetimeout $timeout;
	set timeout 3;
	#
	spawn -noecho telnet $ip 
	expect {
	"Connected to " {
		set status alive;
	}
	"Connection refused" {
		vputs "Connection refused to $ip ...";
		set status dead;
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	timeout {
		vputs "Timed out connecting to $ip ...";
		set status dead;
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	"user name:" {
		send "$username\r";
	}
	timeout {
		vputs "Timed out waiting for username prompt to $ip ...";
		set status dead;
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	"password:" {
		send "$userpasswd\r";
	}
	timeout {
		vputs "Timed out waiting for passwd prompt to $ip ...";
		set status dead;
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re "\[a-z]\[0-9]+\[ab]#" {
		set status active;
	}
	-re "unnamed_system#" {
		set status active;
	}
	-re "#" {
		set status active;
	}
	timeout {
		vputs "Timed out waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	catch { close; wait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
proc parse_cpu_load { fname } {
	# open file
	set cpu_load "UNKNOWN";
	set infd [open $fname "r"];
	while {[gets $infd line] != -1} {
		# puts "\nCPU LOAD LINE: <$line>";
		if {[regexp -nocase -- ".*/Telica/swCPU/CurrRel *-> */Telica/swCPU/(.*)" $line ignore cpu_load] > 0} {
			# puts "\nCPU LOAD (1): <$line> <$cpu_load>";
			close $infd;
			set cpu_load [file tail "${cpu_load}"];
			return $cpu_load;
		} elseif {[regexp -nocase -- ".*CurrRel *-> *(.*)$" $line ignore cpu_load] > 0} {
			# puts "\nCPU LOAD (2): <$line> <$cpu_load>";
			close $infd;
			set cpu_load [file tail "${cpu_load}"];
			return $cpu_load;
		}
	}
	# puts "\nCPU LOAD (3): <UNKNOWN>";
	close $infd;
	return "UNKNOWN";
}
#
proc auditsp { labid sp ip audtype verbose summary results ucond allfiles } {
	#
	global env;
	global loadonly;
	#
	# local PID for file names
	#
	append tfile "/tmp/out" [pid];
	append tfile2 "/tmp/out2" [pid];
	#
	# login remotely
	#
	telnet_to $ip ip_id;
	#
	# get the swCPU load name first.
	#
	set status [remote_exec $ip_id "ls -dl /Telica/swCPU/CurrRel 1>$tfile 2>/dev/null"];
	if {$status != 0} {
		puts "\nremote_exec failed getting CurrRel.";
		return 2;
	}
	ftp_get_ascii_file $ip "$tfile" "$tfile" 0777;
	set cpu_load [parse_cpu_load $tfile];
	if {"$cpu_load" == "UNKNOWN"} {
		puts "\nUnable to get CPU load for $labid $sp $ip.";
		close_telnet $ip_id;
		catch {
			file delete -force -- $tfile;
			file delete -force -- $tfile2;
		} ignore;
		return 2;
	}
	puts "\nIP $ip has CPU load $cpu_load.";
	#
	set lastreload "UNKNOWN";
	#
	send -i $ip_id "\[ -r /.last_reload ] && echo \"Last Reload: \$(cat /.last_reload)\";\r";
	expect {
	-i $ip_id -re {[\r\n]Last Reload: ([^\r\n]*)} {
		set lastreload  $expect_out(1,string);
		exp_continue;
	}
	-i $ip_id -re {.*COMMAND_IS_DONE>} {
		# ok
	}
	timeout {
		# ok
	}
	eof {
		# ok
	}
	};
	puts "Last Reload: $lastreload";
	#
	if {$loadonly} {
		# just print load, and return
		close_telnet $ip_id;
		catch {
			file delete -force -- $tfile;
			file delete -force -- $tfile2;
		} ignore;
		return 0;
	}
	#
	# get list of files on SP.
	#
	set status [remote_exec $ip_id "find /Telica/sw* -type f -exec ls -l \{\} \\; >$tfile2"];
	if {$status != 0} {
		puts "\nremote_exec failed running 'find ...'.";
		return 2;
	}
	ftp_get_ascii_file $ip "$tfile2" "$tfile2" 0777;
	#
	# call local audit to compare files.
	#
	catch { 
		system "$env(LCSTOOLSBIN)/comparefiles $labid $sp $cpu_load $tfile2 $audtype $verbose $summary $results $ucond $allfiles $lastreload";
	} ignore;
	catch { wait -nowait; } ignore;
	#
	# close connection and clean up.
	#
	close_telnet $ip_id;
	catch {
		file delete -force -- $tfile;
		file delete -force -- $tfile2;
	} ignore;
	#
	return 0;
}
#
proc faudit { labid audtype } {
	# variables
	global env;
	global auditSPA;
	global auditSPB;
	global appmode;
	global minmode;
	global verbose;
	global ucond;
	global allfiles;
	#
	append summary "/tmp/summary" [pid];
	append results "/tmp/results" [pid];
	#
	set lcsips(0) 0;
	set lcsips(1) 0;
	# check required environent variables
	if {![info exists env(LCSTOOLSDATA)]} {
		puts "\nLCSTOOLSDATA is not set.";
		exit 2;
	}
	# get ip addresses for labid.
	getlabidips $labid lcsips;
	#
	if {$auditSPA != 0} {
		if {$lcsips(SP_A_IP) != ""} {
			# check if responding on network
			set spastatus [lcsping $lcsips(SP_A_IP)];
			if {$spastatus == "alive"} {
				puts "\n$labid SP_A is alive.";
				myflush;
				# check if SP-A UNIX is active
				set spastatus [lcsunixactive $lcsips(SP_A_IP)];
				puts "$labid SP_A UNIX is $spastatus.";
				myflush;
				# audit if active
				if {$spastatus == "active"} {
					puts "\nauditing $labid SP_A.";
					auditsp $labid "SP_A" $lcsips(SP_A_IP) $audtype $verbose $summary $results $ucond $allfiles;
				} else {
					puts "\nskip auditing $labid SP_A.";
				}
			} else {
				puts "\n$labid SP_A is not responding.";
				myflush;
			}
		} else {
			puts "\n$labid No SP_A_IP found for labid $labid.";
			myflush;
		}
	} else {
		puts "\nskip audit $labid SP_A.";
		myflush;
	}
	if {$auditSPB != 0} {
		if {$lcsips(SP_B_IP) != ""} {
			# check if responding on network
			set spastatus [lcsping $lcsips(SP_B_IP)];
			if {$spastatus == "alive"} {
				puts "\n$labid SP_B is alive.";
				myflush;
				# check if SP-B UNIX is active
				set spastatus [lcsunixactive $lcsips(SP_B_IP)];
				puts "$labid SP_B UNIX is $spastatus.";
				myflush;
				# audit if active
				if {$spastatus == "active"} {
					puts "\nauditing $labid SP_B.";
					auditsp $labid "SP_B" $lcsips(SP_B_IP) $audtype $verbose $summary $results $ucond $allfiles;
				} else {
					puts "\nskip auditing $labid SP_B.";
				}
			} else {
				puts "\n$labid SP_B is not responding.";
				myflush;
			}
		} else {
			puts "\n$labid No SP_B_IP found for labid $labid.";
			myflush;
		}
	} else {
		puts "\nskip auditing $labid SP_B.";
		myflush;
	}
	#
	catch {
		system "/usr/bin/cat $results 2>/dev/null";
		catch { wait -nowait; } ignore;
		system "/usr/bin/cat $summary 2>/dev/null";
		catch { wait -nowait; } ignore;
		#
		logcat lcsspaudit $summary;
		#
		file delete -force -- $summary;
		file delete -force -- $results;
	} ignore;
	return;
}
#
#########################################################################
#
# default values
#
set username "root";
set userpasswd "plexus9000";
set verbose 0;
set logfile "";
set auditSPA 1;
set auditSPB 1;
set audtype "all";
set loadonly 0;
set stracelevel -1;
set ucond 0;
set allfiles 0;
#
# get cmd line options
#
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { set verbose 1; }
	{^-U} { set ucond 1; }
	{^-c} { set audtype "cpu"; }
	{^-i} { set audtype "iom"; }
	{^-L} { set loadonly 1; }
	{^-a} { set allfiles 1; }
	{^-A} { set auditSPA 1; set auditSPB 0; }
	{^-B} { set auditSPB 1; set auditSPA 0; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# trace level and debugging
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# check LCSTOOLS variables
#
checkenv
#
logusage lcsspaudit;
#
# logging on or off?
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
#
if {[string length $logfile] > 0} {
	log_user 1;
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nlogging failed for file $logfile:\n$status";
		exit 2;
	}
}
#
# audit lab SPs
#
if {$arg >= $argc} {
	# audit the lab for the current labid
	checklabid
	set labid $env(LABID);
	faudit $labid $audtype;
} else {
	# audit the labs for the given labids
	for { } {$arg<$argc} {incr arg} {
		set labid [lindex $argv $arg];
		logmsg lcsspaudit "START-AUDIT: labid=$labid, type=$audtype, doSPA=$auditSPA, doSPB=$auditSPB allfiles=$allfiles";
		faudit $labid $audtype;
		logmsg lcsspaudit "END-AUDIT: labid=$labid, type=$audtype, doSPA=$auditSPA, doSPB=$auditSPB allfiles=$allfiles";
	}
}
#
# done
#
exit 0
