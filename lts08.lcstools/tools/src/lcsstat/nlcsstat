#!/opt/exp/bin/expect
#
# get the status of a LCS.
#
#########################################################################
#
# library
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
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
	global tl1username;
	global tl1userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-i] \[-V] \[-S level] \[-u username] \[-p passwd] \[-U TL1_username] \[-P TL1_passwd] \[labid]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-V - enable verbose output";
	puts "	-S level - set trace level";
	puts "	-i - do not check TL1";
	puts "	-u - lynx-os username";
	puts "	-p - lynx-os username passwd";
	puts "	-U - TL1 username";
	puts "	-P - Tl1 username passwd";
	puts "	-I - only ping the SP";
	puts "";
	puts "	default UNIX username = $username";
	puts "	default UNIX passwd = $userpasswd";
	puts "	default TL1 username = $tl1username";
	puts "	default TL1 passwd = $tl1userpasswd";
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
	eof { set status dead; }
	}
	#
	catch { close; wait -nowait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
#
proc lcstl1active { ip sp } {
	#
	global ignoretl1;
	global tl1username;
	global tl1userpasswd;
	global timeout;
	set status dead;
	set tl1port 2361;
	set savetimeout $timeout;
	#
	# check TL1 ???
	if {$ignoretl1} {
		# skip this check
		set status ignored;
		return $status;
	}
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
	eof {
		vputs "EOF connecting to $ip ...";
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
	eof {
		vputs "EOF waiting for TL1 prompt from $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	# activate user for TL1
	set actuser "act-user::USERNAME:::USERPASSWD";
	regsub -- "USERNAME" $actuser "$tl1username" actuser;
	regsub -- "USERPASSWD" $actuser "$tl1userpasswd" actuser;
	#
	set imdonemrduck 0;
	set isprotection 0;
	if {[catch {
		send "$actuser;\r";
	} ignore] != 0} {
		return $status;
	}
	expect {
	"Telica>" {
		vputs "TL1 prompt received AFTER ACT-USER for $ip ...";
		if (!$imdonemrduck) {
			set status alive;
		}
		vputs "STATUS IS <$status> AFTER ACT-USER for $ip ...";
	}
	-re "WORKING *System *Processor" {
		vputs "WORKING received AFTER ACT-USER for $ip ...";
		set status "alive - working system processor";
		set imdonemrduck 1;
		set isprotection 0;
		exp_continue;
	}
	-re "PROTECTION *System *Processor" {
		vputs "PROTECTION received AFTER ACT-USER for $ip ...";
		set status "alive - protection system processor";
		set imdonemrduck 1;
		set isprotection 1;
		exp_continue;
	}
	-re "Username.*active TL1 session" {
		vputs "Active TL1 Session received AFTER ACT-USER for $ip ...";
		set status "alive - Active TL1 Session on this processor";
		set imdonemrduck 1;
		set isprotection 1;
		exp_continue;
	}
	timeout {
		vputs "Timed out waiting for TL1 prompt from $ip ...";
		set status "alive (time out waiting for ACT-USER)";
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		vputs "EOF out waiting for TL1 prompt from $ip ...";
		set status "alive (EOF waiting for ACT-USER)";
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	# check if we have a protection SP. TL1 will fail
	# on the protection SP, so don't do the RTRV-EQPT.
	#
	if ($isprotection) {
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	# enter RTRV-EQPT command and get status.
	set imdonemrduck 0;
	send "RTRV-EQPT;\r";
	#
# exp_internal 1;
	expect {
	-re "$sp.*:IS,ACT\"" {
		vputs "IS,ACT received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,ACT";
		set imdonemrduck 1;
		exp_continue;
	}
	-re "$sp.*:IS,ACT&NBK\"" {
		vputs "IS,ACT,NBK received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,ACT,NBK";
		set imdonemrduck 1;
		exp_continue;
	}
	-re "$sp.*:IS,ACT.*SYNC\"" {
		vputs "IS,ACT,SYNC received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,ACT,SYNC";
		set imdonemrduck 1;
		exp_continue;
	}
	-re "$sp.*:IS,STBYC" {
		vputs "IS,STBYC received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,STBYC";
		set imdonemrduck 1;
		exp_continue;
	}
	-re "$sp.*:IS,STBYC.*SYNC" {
		vputs "IS,STBYC,SYNC received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,STBYC,SYNC";
		set imdonemrduck 1;
		exp_continue;
	}
	-re "$sp.*:IS,STBYH" {
		vputs "IS,STBYH received AFTER RTRV-EQPT for $ip,$sp ...";
		set status "alive - IS,STBYH";
		set imdonemrduck 1;
		exp_continue;
	}
	"Telica>" {
		vputs "TL1 prompt received AFTER RTRV-EQPT for $ip,$sp ...";
		if (!$imdonemrduck) {
			exp_continue;
		}
	}
	timeout {
		vputs "Timed out waiting for TL1 prompt from $ip,$sp ...";
		set status "alive (time out waiting for RTRV-EQPT)";
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		vputs "EOF waiting for TL1 prompt from $ip,$sp ...";
		set status "alive (EOF waiting for RTRV-EQPT)";
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
# exp_internal 0;
	#
	catch { close; wait -nowait; } ignore;
	#
	set timeout $savetimeout;
	#
	return $status;
}
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
	eof {
		vputs "EOF connecting to $ip ...";
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
	eof {
		vputs "EOF waiting for username prompt to $ip ...";
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
	eof {
		vputs "EOF out waiting for passwd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re "\[a-z]*\[0-9]+\[ab]#" {
		set status active;
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		set status active;
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "unnamed_system#" {
		puts "WARNING: UNIX is active, but 'unnamed_system' prompt was received.";
		set status active;
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		vputs "Timed out waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		vputs "EOF waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "\[ -x /Telica/swCPU/CurrRel/system/scripts/telica.rc.network ] && echo \"APPLICATIONMODE\";\r";
	}
	timeout {
		# ignore time-outs
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		# ignore eofs
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re ".*\"APPLICATIONMODE\";" {
		# ok
	}
	timeout {
		# ignore time-outs
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		# ignore eofs
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re ".*APPLICATIONMODE" {
		# set appstring $expect_out(0,string);
		# puts "APPMODE STRING: $appstring";
		set status "active - application mode";
	}
	-re "COMMAND_IS_DONE>" {
		# set minstring $expect_out(0,string);
		# puts "MINMODE STRING: $minstring";
		set status "active - minimum mode, OS-only";
	}
	timeout {
		# ignore time-outs
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		# ignore eofs
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
	eof {
		vputs "EOF connecting to $ip ...";
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
	eof {
		vputs "EOF waiting for username prompt to $ip ...";
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
	eof {
		vputs "EOF for passwd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re "\[a-z]*\[0-9]+\[ab]#" {
		set timeout 20;
		send "grep 'Setting Initial State: ACTIVE:\[01]' /Telica/History/MajorEvtsLog.log | sed -n '\$,\$p'\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		set timeout 20;
		send "grep 'Setting Initial State: ACTIVE:\[01]' /Telica/History/MajorEvtsLog.log | sed -n '\$,\$p'\r";
	}
	timeout {
		vputs "Timed out waiting for cmd prompt to $ip ...";
		set status dead;
		catch { close; wait -nowait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	eof {
		vputs "EOF waiting for cmd prompt to $ip ...";
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
	-re "\[a-z]*\[0-9]+\[ab]#" {
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
	eof {
		vputs "EOF waiting for cmd prompt to $ip ...";
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
	global onlyping;
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
		if {$spastatus == "alive" && $onlyping} {
			puts "$labid SP_A_IP is alive (ignore Unix and TL1).";
			myflush;
		} elseif {$spastatus == "alive"} {
			puts "$labid SP_A_IP is alive.";
			myflush;
			#
			# check if SP-A is active
			# set spastatus [lcsspactive $lcsips(SP_A_IP)];
			# puts "$labid SP_A_IP is $spastatus.";
			# myflush;
			#
			# check if SP-A UNIX is active
			set spastatus [lcsunixactive $lcsips(SP_A_IP)];
			puts "$labid SP_A_IP UNIX is $spastatus.";
			myflush;
			#
			if {[string match "active*" $spastatus]} {
				# check if tl1 is active
				set spastatus [lcstl1active $lcsips(SP_A_IP) "SP-A"];
				vputs "$labid SP_A_IP TL1 port is $spastatus.";
				if {$spastatus == "dead"} {
					puts "$labid SP_A_IP TL1 port is not responding.";
					myflush;
				} else {
					puts "$labid SP_A_IP TL1 port is $spastatus.";
					myflush;
				}
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
		if {$spbstatus == "alive" && $onlyping} {
			puts "$labid SP_B_IP is alive (ignore Unix and TL1).";
			myflush;
		} elseif {$spbstatus == "alive"} {
			puts "$labid SP_B_IP is alive.";
			myflush;
			#
			if {$onlyping} {
				return;
			}
			# check if SP-B is active
			# set spastatus [lcsspactive $lcsips(SP_B_IP)];
			# puts "$labid SP_B_IP is $spastatus.";
			# myflush;
			#
			# check if SP-B UNIX is active
			set spastatus [lcsunixactive $lcsips(SP_B_IP)];
			puts "$labid SP_B_IP UNIX is $spastatus.";
			myflush;
			#
			if {[string match "active*" $spastatus]} {
				# check if tl1 is active
				set spastatus [lcstl1active $lcsips(SP_B_IP) "SP-B"];
				vputs "$labid SP_B_IP TL1 port is $spastatus.";
				if {$spastatus == "dead"} {
					puts "$labid SP_B_IP TL1 port is not responding.";
					myflush;
				} else {
					puts "$labid SP_B_IP TL1 port is $spastatus.";
					myflush;
				}
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
set tl1username "telica";
set tl1userpasswd "telica";
set verbose 0;
set ignoretl1 0;
set onlyping 0;
set logfile "";
set stracelevel -1;
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
	{^-i} { set ignoretl1 1; }
	{^-I} { set onlyping 1; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-U.*} { getoptval $argval tl1username arg; }
	{^-P.*} { getoptval $argval tl1userpasswd arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
if {$stracelevel >= 0} {
	strace $stracelevel;
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
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nlogging failed for file $logfile:\n$status";
		exit 2;
	}
	log_user 1;
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
