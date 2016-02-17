#!/opt/exp/bin/expect
#
# get the status of a LCS.
#
#########################################################################
#
# library
#
# package require Expect
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
#
# functions
#
proc myflush { } {
	# global user_spawn_id;
	# flush $user_spawn_id;
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

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-u username] \[-p passwd] \[labid]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-V - enable verbose output";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	default labid is from variable LABID";
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
	catch { close; wait -nowait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
proc lcstl1active { ip } {
	#
	global timeout;
	set status dead;
	set tl1port 2361;
	set savetimeout $timeout;
	#
	set timeout 5;
	spawn -noecho telnet $ip $tl1port;
	expect {
	"Connected to " {
		set status alive;
		vputs "Connected to $ip ...";
	}
	"Connection refused" {
		vputs "Connection refused to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	timeout {
		vputs "Timed out connecting to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	# try to get TL1 prompt
	send "\r";
	expect {
	"Telica>" {
		vputs "TL1 prompt received for $ip ...";
		set status alive;
	}
	timeout {
		vputs "Timed out waiting for TL1 prompt from $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	catch { close; wait -nowait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
proc lcsspactive { ip } {
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
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	timeout {
		vputs "Timed out connecting to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
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
		catch { close; wait -nowait; } ignore;
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
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re "m\[0-9]+\[ab]#" {
		set timeout 5;
		send "grep 'Setting Initial State: ACTIVE:\[01]' /Telica/History/MajorEvtsLog.log | sed -n '\$,\$p'\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		set timeout 5;
		send "grep 'Setting Initial State: ACTIVE:\[01]' /Telica/History/MajorEvtsLog.log | sed -n '\$,\$p'\r";
	}
	timeout {
		vputs "Timed out waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re ACTIVE:0 {
		set status standby;
	}
	-re ACTIVE:1 {
		set status active;
	}
	-re "m\[0-9]+\[ab]#" {
		set status unknown;
	}
	-re "\[a-zA-Z0-9_-]+#" {
		set status unknown;
	}
	timeout {
		vputs "Timed out waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	catch { close; wait -nowait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
proc labstatus { labid } {
	# variables
	global env;
	set lcsips(0) 0;
	set lcsips(1) 0;
	# check required environent variables
	if {![info exists env(LCSTOOLSDATA)]} {
		puts "\nLCSTOOLSDATA is not set.";
		exit 2;
	}
	# get ip addresses for labid.
	getlabidips $labid lcsips;
	if {$lcsips(SP_A_IP) != ""} {
		# check if responding on network
		set spastatus [lcsping $lcsips(SP_A_IP)];
		if {$spastatus == "alive"} {
			puts "$labid SP_A_IP is alive.";
			myflush;
			# check if tl1 is active
			set spastatus [lcstl1active $lcsips(SP_A_IP)];
			if {$spastatus == "alive"} {
				puts "$labid SP_A_IP TL1 port is alive.";
				myflush;
				# check if SP-A is active
				set spastatus [lcsspactive $lcsips(SP_A_IP)];
				puts "$labid SP_A_IP is $spastatus.";
				myflush;
			} else {
				puts "$labid SP_A_IP TL1 port is not responding.";
				myflush;
			}
		} else {
			puts "$labid SP_A_IP is not responding.";
			myflush;
		}
	} else {
		puts "$labid No SP_A_IP found for labid $labid.";
		myflush;
	}
	if {$lcsips(SP_B_IP) != ""} {
		# check if responding on network
		set spbstatus [lcsping $lcsips(SP_B_IP)];
		if {$spbstatus == "alive"} {
			puts "$labid SP_B_IP is alive.";
			myflush;
			# check if tl1 is active
			set spastatus [lcstl1active $lcsips(SP_B_IP)];
			if {$spastatus == "alive"} {
				puts "$labid SP_B_IP TL1 port is alive.";
				myflush;
				# check if SP-B is active
				set spastatus [lcsspactive $lcsips(SP_B_IP)];
				puts "$labid SP_B_IP is $spastatus.";
				myflush;
			} else {
				puts "$labid SP_B_IP TL1 port is not responding.";
				myflush;
			}
		} else {
			puts "$labid SP_B_IP is not responding.";
			myflush;
		}
	} else {
		puts "$labid No SP_B_IP found for labid $labid.";
		myflush;
	}
	#
	return;
}
#
#########################################################################
#
# default values
#
set tid "telica";
set username "root";
set userpasswd "plexus9000";
set verbose 0;
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
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# check LCSTOOLS variables
#
checkenv
#
# logging on or off?
#
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
#
# report lab status
#
if {$arg >= $argc} {
	# get status of current labid
	checklabid
	set labid $env(LABID);
	labstatus $labid;
} else {
	# get status of given labids
	for { } {$arg<$argc} {incr arg} {
		set labid [lindex $argv $arg];
		labstatus $labid;
	}
}
#
# done
#
exit 0
