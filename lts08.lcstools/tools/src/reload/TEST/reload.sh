#!/opt/exp/bin/expect
#
# reload a lab from local load repository.
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
source $env(LCSTOOLSLIB)/logging
source $env(LCSTOOLSLIB)/lock
#
# exit handler
#
exit -onexit {
	global LABID;
	if {[info exists LABID] && [llength LABID] > 0} {
		V reload $LABID;
	}
}
#
trap {
	global LABID;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "Got signal <$signame,$signo)";
	if {[info exists LABID] && [llength LABID] > 0} {
		V reload $LABID;
		unset LABID;
	}
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
proc eofexit { status } {
	global LABID;
	if {[info exists LABID] && [llength LABID] > 0} {
		quietV reload $LABID;
		unset LABID;
	}
	exit $status;
}
#
# functions
#
proc what_to_do_next { } {
	puts "";
	puts "Suggested steps after a successful reload:";
	puts "";
	puts "1) For simplex labs, run the following on the LSP:";
	puts "	$ postlcsreload -s -D -h -c 10 -m";
	puts "or, for duplex labs, run:";
	puts "	$ postlcsreload -d -D -h -c 10 -m";
	puts "";
	puts "2) For simplex labs, reboot EITHER SP-A or SP-B, NEVER BOTH SPs:";
	puts " $ lcsreboot -A	# for SP-A init, do NOT boot SP-B";
	puts "or";
	puts " $ lcsreboot -B	# for SP-B init, do NOT boot SP-A";
	puts "For duplex labs, just enter:";
	puts " $ lcsreboot	# init's both SPs";
	puts "";
	puts "3) For duplex labs, wait for the SPs to be IS,STBYH and IS,ACT.";
	puts "and for simplex labs, for the one SP to be IS,NBK, then you can";
	puts "continue on to provisioning.";
}
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "usage: [file tail $argv0] \[-\?] \[-x] \[-B] \[-V] \[-K|-X|-H] \[-R] \[-D] \[-T]";
	puts "\[-S trace level] \[-l logfile] \[-u username] \[-p passwd] \[-Q]";
	puts "\[ -\[i|I] simplex_a | -\[i|I] simplex_b | -\[i|I] duplex ] \[labid] ";
	puts "";
	puts "where:";
	puts "	-x - enable TCL debugger";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-K - do loadimage only; skip expansion.";
	puts "	-X - do expansion only; skip loadimage";
	puts "	-H - generate hardware Tl1 scripts (disables -K,-X)";
	puts "	-D - do NOT ask before proceeding with expansion";
	puts "	-l logfile - log all output to a logfile";
	puts "	-V - turn on verbose";
	puts "	-T - help text for recovering from time outs.";
	puts "	-Q - ask which IOM/CPU image to reload.";
	puts "	-R - remove lock file. see below before using this option.";
	puts "	-B - background reload.";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	labid is read from variable LABID, unless given.";
	puts "";
	puts "	by default, all IOMs/CPU are reloaded. if you want to be";
	puts "	asked which IOMs/CPU to reload, use the -Q option.";
	puts "";
	puts "	You can automatically reboot the switch after a reload by ";
	puts "	using the -i option. The choices are:";
	puts "";
	puts "	-i simplex_a - reload, split lab, boot SP-A active";
	puts "	-i simplex_b - reload, split lab, boot SP-B active";
	puts "	-i duplex - reload, duplex lab, boot SP-A and SP-B active";
	puts "";
	puts "	-I simplex_a - skip reload, split lab, boot SP-A active";
	puts "	-I simplex_b - skip reload, split lab, boot SP-B active";
	puts "	-I duplex - skip reload, duplex lab, boot SP-A and SP-B active";
	puts "";
	puts "	If you are absolutely sure that NO ONE else is running a";
	puts "	a reload and you are blocked by the lock file, then you can";
	puts "	run reload with the -R option to remove the lock. Remember,";
	puts "	if you remove the lock and there really is another reload";
	puts "	in progress for the same lab, each reload session will trash";
	puts "	the other. So, don't remove the lock, unless you are sure.";
	puts "";
}
#
proc timeouthelp { } {
	global argv0;
	global username;
	global userpasswd;

	puts "";
	puts "A timeout has occurred while waiting for a response. The";
	puts "expected response was either from ftp or telnet. In";
	puts "either case, you should try to login using pxmonitor and";
	puts "check the sanity of the SP and IP connections. First, verify";
	puts "that you can logon the SP. If you can, then run:";
	puts "";
	puts "ifconfig mgt0";
	puts "";
	puts "If the IP comes back as 0.0.0.0, then you have to configure ";
	puts "the mgt0 interface; enter:";
	puts "";
	puts "/bin/iprecover";
	puts "";
	puts "in the same window. A second possible problem is when the SP";
	puts "drops to the PROM or debug mode. The prompt on the pxmonitor";
	puts "window will be 'ibmXXXX', 'Command\?' or '\*'. For this case,";
	puts "refer to here: ";
	puts "";
	puts "http://ihgpweb.ih.lucent.com/~rumore/LCS/booting-from-prom-mode-or-other";
	puts "";
	puts "For more information on recovering the IP connections, refer to";
	puts "here:";
	puts "";
	puts "http://ihgpweb.ih.lucent.com/~rumore/LCS/plexus_ip_recovery";
	puts "";
	puts "If you are unable to get a pxmonitor window, then run 'rmstat'";
	puts "and check if another user owns the lynx-a or lynx-b target. If";
	puts "this is true, then release the target using 'pxreset -t lynx-a'";
	puts "or 'pxreset -t lynx-b'. If no user owns the lynx targets and you";
	puts "cannot get a 'username' prompt, then have lab support reseat the";
	puts "SP board. Remember, reseating the board is the LAST choice.";
	puts "";
	puts "If you are able access the px target, but are unable to logon,";
	puts "then you can try dropping into the SP debugger by entering:";
	puts "CONTROL-SHIFT-underscore. If you receive the \"*\" prompt, then";
	puts "enter \"R\" and \"y\". If this fails, reseat the board." ;
	puts "";
	# puts "If all else fails, call me at x34396.";
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
proc lcsunixactive { ip } {
	global username;
	global userpasswd;
	global timeout;
	#
	set prompt unknown;
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
		puts "Connection refused to $ip ...";
		set status "telnet connection refused";
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	timeout {
		puts "Timed out connecting to $ip ...";
		set status "telnet time out";
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
		puts "Timed out waiting for username prompt to $ip ...";
		set status "telnet time out waiting for username prompt";
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
		puts "Timed out waiting for passwd prompt to $ip ...";
		set status "telnet time out waiting for passwd prompt";
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re "(\[a-z]\[0-9]+\[ab]#)" {
		set prompt "$expect_out(1,string)";
		set status "$prompt - active";
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "(unnamed_system#)" {
		# puts "WARNING: UNIX is active, but 'unnamed_system' prompt was received.";
		set prompt "$expect_out(1,string)";
		set status "$prompt - active";
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		puts "Timed out waiting for cmd prompt to $ip ...";
		set status "$prompt - time out waiting for cmd prompt (1)";
		catch { close; wait; } ignore;
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
		set status "$prompt - time out waiting for cmd prompt (2)";
		catch { close; wait; } ignore;
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
		set status "$prompt - time out waiting for cmd prompt (3)";
		catch { close; wait; } ignore;
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	expect {
	-re ".*APPLICATIONMODE" {
		# set appstring $expect_out(0,string);
		# puts "APPMODE STRING: $appstring";
		set status "$prompt - active - application mode";
	}
	-re "COMMAND_IS_DONE>" {
		# set minstring $expect_out(0,string);
		# puts "MINMODE STRING: $minstring";
		set status "$prompt - active - minimum mode, OS-only";
	}
	timeout {
		# ignore time-outs
		set status "$prompt - time out waiting for cmd prompt (4)";
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
proc getspstatus { labid labidip sp } {
	# check if responding on network
	set spstatus [lcsping $labidip];
	if {$spstatus == "alive"} {
		puts "$labid $sp $labidip is alive.";
		#
		set spstatus [lcsunixactive $labidip];
		puts "$labid $sp $labidip UNIX is $spstatus.";
		if {[string match "*active*application*" $spstatus]} {
			puts "\nWARNING: $sp is in APPLICATION mode.";
			puts "Boot Lab into MIN OS-only mode: lcsreboot -m\n";
		}
	} else {
		puts "$labid $sp $labidip is not responding.";
	}
	return $spstatus;
}
#
proc getimages2load { labid bname lname ilistname } {
	upvar $ilistname imagelist;
	upvar $bname branch;
	upvar $lname cpuload;
	global logfile;
	global usingdefaultlogfile;
	global query;
	#
	if {[dbselect llobuf labloads "labid req ^$labid\$" "branch cpuload" ] != 0} {
		puts "getimages2load: dbselect of relation 'labloads' failed.";
		logmsg reload "END: getimages2load: dbselect of relation 'labloads' failed.";
		exit 2;
	}
	if {![info exists llobuf] || [llength llobuf] == 0} {
		puts "getimages2load: no loads found for labid $labid.";
		logmsg reload "END: getimages2load: no loads found for labid $labid.";
		exit 2;
	}
	#
	set choices $llobuf;
	set choices [linsert $choices 0 "QUIT"];
	chooseone "Choose a load to reload: " choices choice;
	if {$choice == "QUIT"} {
		if {$usingdefaultlogfile != 0} {
			if {[file exists $logfile]} {
				file delete -force -- $logfile;
			}
		}
		puts "exiting reload.";
		exit 0;
	}
	#
	set ldata [split $choice " \t"];
	set branch [lindex $ldata 0];
	set cpuload [lindex $ldata 1];
	#
	puts "Will reload load <$branch, $cpuload>.";
	#
	if {[dbselect iobuf images "branch req ^$branch\$ and cpuload req ^$cpuload\$" "type name" ] != 0} {
		puts "getimages2load: dbselect of relation 'images' failed.";
		logmsg reload "END: getimages2load: dbselect of relation 'images' failed.";
		exit 2;
	}
	if {![info exists iobuf] || [llength iobuf] == 0} {
		puts "getimages2load: no images found for <$branch, $cpuload>.";
		logmsg reload "END: getimages2load: no images found for <$branch, $cpuload>.";
		exit 2;
	}
	#
	set timeout -1;
	if {[info exists imagelist]} {
		unset imagelist;
	}
	foreach image $iobuf {
		if {$query == 0} {
			lappend imagelist $image;
			send_tty "Will reload $image ...\n";
		} else {
			send_user "\nReload this image <$image>? \[y/n/cr=n] ";
			expect_user {
			-re "y\n" {
				lappend imagelist $image;
				send_tty "Will reload $image ...\n";
			}
			-re "\[^\n]*\n" {
				send_tty "Will NOT reload $image ...\n";
			}
			eof {
				send_tty "END-OF-FILE. exiting reload.\n";
				logmsg reload "END: END-OF-FILE. exiting reload.\n";
				eofexit 2;
			}
			}
		}
	}
}
#
proc clear_remote_fs { machine remotefs } {
	global username;
	global userpasswd;
	global timeout;
	#
	set loginseen 0;
	set savetimeout $timeout;
	set timeout 45;
	#
	spawn -noecho "/usr/bin/telnet" "-l $username" $machine;
	expect {
	-re ".*user name:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*password:" {
		set loginseen 0;
		send "$userpasswd\r";
	}
	timeout {
		puts "\ntimeout during telnet ...";
		timeouthelp;
		logmsg reload "END: timeout during telnet ...";
		exit 2;
	}
	};
	#
	if {$loginseen} {
		expect {
		-re ".*password:" {
			send "$userpasswd\r";
		}
		timeout {
			puts "\ntimeout during telnet, waiting for password: ...";
			timeouthelp;
			logmsg reload "END: timeout during telnet, waiting for password: ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		puts "\ntimeout during prompt ...";
		timeouthelp;
		logmsg reload "END: timeout during prompt ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "cd $remotefs \r";
	}
	timeout {
		puts "\ntimeout during PS1= ...";
		timeouthelp;
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# send "/bin/find $remotefs -type f -print | /bin/cat \r";
		send "/bin/find $remotefs -type f -print | /bin/xargs rm -f \r";
	}
	timeout {
		puts "\ntimeout during cd ...";
		timeouthelp;
		logmsg reload "END: timeout during cd ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "\ntimeout during rm ...";
		timeouthelp;
		logmsg reload "END: timeout during rm ...";
		exit 2;
	}
	};
	#
	catch { close; wait; } ignore;
	#
	set timeout $savetimeout;
}
#
proc remove_remote_file { machine remotefile } {
	global username;
	global userpasswd;
	global timeout;
	#
	set loginseen 0;
	set savetimeout $timeout;
	set timeout 15;
	#
	spawn -noecho "/usr/bin/telnet" "-l $username" $machine;
	expect {
	-re ".*user name:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*password:" {
		set loginseen 0;
		send "$userpasswd\r";
	}
	timeout {
		puts "\ntimeout during telnet ...";
		timeouthelp;
		logmsg reload "END: timeout during telnet ...";
		exit 2;
	}
	};
	#
	if {$loginseen} {
		expect {
		-re ".*password:" {
			send "$userpasswd\r";
		}
		timeout {
			puts "\ntimeout during telnet, waiting for password: ...";
			timeouthelp;
			logmsg reload "END: timeout during telnet, waiting for password: ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		puts "\ntimeout during prompt ...";
		timeouthelp;
		logmsg reload "END: timeout during prompt ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "cd /\r";
	}
	timeout {
		puts "\ntimeout during PS1= ...";
		timeouthelp;
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "/bin/rm -f $remotefile \r";
	}
	timeout {
		puts "\ntimeout during cd ...";
		timeouthelp;
		logmsg reload "END: timeout during cd ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "\ntimeout during rm ...";
		timeouthelp;
		logmsg reload "END: timeout during rm ...";
		exit 2;
	}
	};
	#
	catch { close; wait; } ignore;
	#
	set timeout $savetimeout;
}
#
proc loadimage { labid cpu_ip branch cpuload ilistname } {
	upvar $ilistname images2load;
	# get text file system
	if {[dbselect fsobuf filesystems "branch req ^$branch\$ and type req text" "path" ] != 0} {
		puts "loadimage: dbselect of relation 'filesystems' failed.";
		logmsg reload "END: loadimage: dbselect of relation 'filesystems' failed.";
		exit 2;
	}
	if {![info exists fsobuf] || [llength fsobuf] == 0} {
		puts "loadimage: no filesystem found for branch $branch.";
		logmsg reload "END: loadimage: no filesystem found for branch $branch.";
		exit 2;
	}
	set filesystem [lindex $fsobuf 0];
	#
	puts "";
	puts "Clearing /home filesystem on $cpu_ip ...";
	clear_remote_fs $cpu_ip "/home";
	#
	foreach imagedata $images2load {
		set idata [split $imagedata " \t"];
		set swtype [lindex $idata 0];
		set swfile [lindex $idata 1];
		#
		if {[regexp "^\[0-9A-Z\.]*(_.*)$" $swfile ignore suffix] != 1} {
			puts "loadimage: invalid file name - $swfile.";
			logmsg reload "END: loadimage: invalid file name - $swfile.";
			exit 2;
		}
		#
		set fromfile "$filesystem/$branch/$swtype/$swfile";
		set tofile "/home/$swfile";
		#
		puts "";
		puts "CPU-IP    : $cpu_ip";
		puts "Suffix    : $suffix";
		puts "From file : $fromfile";
		puts "To file   : $tofile";
		#
		puts "";
		puts "Removing file /*${suffix} ..."
		remove_remote_file $cpu_ip "/*${suffix}";
		puts "";
		puts "Removing file /home/*${suffix} ..."
		remove_remote_file $cpu_ip "/home/*${suffix}";
		#
		puts "";
		puts "Loading file $tofile ..."
		ftp_put_binary_file $cpu_ip $fromfile $tofile;
	}
}
#
proc do_we_proceed { } {
	send_user "Proceed with installation? \[y/n/cr=n] ";
	expect_user {
	-re "y\n" {
		send_tty "Proceeding ...\n\n";
	}
	-re "\[yY]\[eE]\[sS]\n" {
		send_tty "Proceeding ...\n\n";
	}
	-re "(\[^\n]*)\n" {
		send_tty "Exiting reload.\n\n";
		logmsg reload "END: Exiting reload.";
		exit 2;
	}
	eof {
		send_tty "END-OF-FILE. exiting reload.\n";
		logmsg reload "END: END-OF-FILE. exiting reload.";
		eofexit 2;
	}
	}
}
#
proc do_we_reload { labid sp cpu_ip_name } {
	upvar $cpu_ip_name cpu_ip;
	puts "\nReloading lab $labid $sp $cpu_ip.";
	send_user "Is this correct? \[y/n/cr=n] ";
	expect_user {
	-re "y\n" {
		send_tty "\n==>> Will reload lab $labid $sp.\n\n";
	}
	-re "(\[^\n]*)\n" {
		send_tty "\n==>> Will NOT reload lab $labid.\n\n";
		set cpu_ip "";
	}
	eof {
		send_tty "END-OF-FILE. exiting reload.\n";
		logmsg reload "END: END-OF-FILE. exiting reload.";
		eofexit 2;
	}
	}
}
#
proc telnet_to { ip spawn_id_name } {
	global username;
	global userpasswd;
	global timeout;
	#
	upvar $spawn_id_name spawn_id;
	#
	set loginseen 0;
	set savetimeout $timeout;
	set timeout 15;
	#
	spawn -noecho "/usr/bin/telnet" "-l $username" $ip;
	expect {
	-re ".*user name:" {
		set loginseen 1;
		send "$username\r";
	}
	-re ".*password:" {
		set loginseen 0;
		send "$userpasswd\r";
	}
	timeout {
		puts "\ntimeout during login ...";
		timeouthelp;
		logmsg reload "END: timeout during login ...";
		exit 2;
	}
	};
	#
	if {$loginseen} {
		expect {
		-re ".*password:" {
			send "$userpasswd\r";
		}
		timeout {
			puts "\ntimeout during passwd ...";
			timeouthelp;
			logmsg reload "END: timeout during passwd ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		puts "\ntimeout during PS1= ...";
		timeouthelp;
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "cd /\r";
	}
	timeout {
		puts "\ntimeout during cd ...";
		timeouthelp;
		logmsg reload "END: timeout during cd ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok.
	}
	timeout {
		puts "\ntimeout during cd ...";
		timeouthelp;
		logmsg reload "END: timeout during cd ...";
		exit 2;
	}
	};
	#
	set timeout $savetimeout;
}
#
proc killemf { cpu_a_ip cpu_b_ip } {
	global username;
	global userpasswd;
	global timeout;
	#
	if {$cpu_a_ip != "" && $cpu_b_ip != ""} {
		puts "\nKilling emf on $cpu_a_ip and $cpu_b_ip.\n";
		#
		set cpu_a_killed 0;
		set cpu_b_killed 0;
		#
		telnet_to $cpu_a_ip cpu_a_id;
		telnet_to $cpu_b_ip cpu_b_id;
		#
		set timeout 10;
		send -i $cpu_a_id "ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\r";
		send -i $cpu_b_id "ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\r";
		expect {
		-i $cpu_a_id -re ".*COMMAND_IS_DONE>" {
			if {!$cpu_b_killed} {
				set cpu_a_killed 1;
				exp_continue;
			}
		}
		-i $cpu_b_id -re ".*COMMAND_IS_DONE>" {
			if {!$cpu_a_killed} {
				set cpu_b_killed 1;
				exp_continue;
			}
		}
		timeout {
			# ok
		}
		}
		#
		catch { close $cpu_a_id; } ignore;
		catch { close $cpu_b_id; } ignore;
		#
		return;
	} elseif {$cpu_a_ip != ""} {
		set cpu_ip $cpu_a_ip;
	} elseif {$cpu_b_ip != ""} {
		set cpu_ip $cpu_b_ip;
	} else  {
		puts "killemf: both CPU IPs are NULL.";
		return;
	}
	#
	puts "\nKilling emf on $cpu_ip.\n";
	#
	telnet_to $cpu_ip cpu_id;
	#
	set timeout 10;
	send -i $cpu_id "ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\r";
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		# ok
	}
	}
	#
	catch { close $cpu_id; } ignore;
	return;
}
#
proc removeoldload { cpu_ip ilistname } {
	upvar $ilistname images2load;
	#
	global username;
	global userpasswd;
	global timeout;
	global dbcmds;
	#
	puts "\nRemoving load from $cpu_ip.\n";
	#
	telnet_to $cpu_ip cpu_id;
	#
	set timeout 30;
	send -i $cpu_id "/Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed\r";
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "removeoldload: timeout waiting for remove_configuration_data.";
		# clean up and logon again.
		catch { close $cpu_id; } ignore;
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	set yesnum 0;
	send -i $cpu_id "/opt/TimesTen4.*/32/bin/setup.sh -uninstall\r";
	expect {
	-i $cpu_id -re ".*completely.*remove.*TimesTen.*yes " {
		send -i $cpu_id "yes\r";
		incr yesnum;
		exp_continue;
	}
	-i $cpu_id -re ".*remove.*all.*files.*in.*no " {
		send -i $cpu_id "yes\r";
		incr yesnum;
		exp_continue;
	}
	-i $cpu_id -re ".*remove.*remaining.*system.*files.*no " {
		send -i $cpu_id "yes\r";
		incr yesnum;
		exp_continue;
	}
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		if {$yesnum != 3} {
			puts "removeoldload: setup.sh failed (yesnum=$yesnum).";
			# clean up and logon again.
			catch { close $cpu_id; } ignore;
			telnet_to $cpu_ip cpu_id;
		}
	}
	timeout {
		puts "removeoldload: timeout during setup.sh.";
		# clean up and logon again.
		catch { close $cpu_id; } ignore;
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	set timeout 120;
	foreach imagedata $images2load {
		set moreswtype2rm "";
		#
		set idata [split $imagedata " \t"];
		set swtype [lindex $idata 0];
		set swfile [lindex $idata 1];
		#
		# ugh. there are better ways, but it'll do for now.
		switch -regexp -- $swfile {
		{^.*_ana.tar.gz$} {
			set swtype2rm "swANA";
		}
		{^.*_atmds3.tar.gz$} {
			set swtype2rm "swATMDS3";
		}
		{^.*_cpu.tar.gz$} {
			set swtype2rm "swCPU";
			set moreswtype2rm "swCraft";
		}
		{^.*_cm.tar.gz$} {
			set swtype2rm "swCM";
		}
		{^.*_ds1_2.tar.gz$} {
			set swtype2rm "swDS1_2";
		}
		{^.*_ds1.tar.gz$} {
			set swtype2rm "swDS1";
		}
		{^.*_ds3.tar.gz$} {
			set swtype2rm "swDS3";
		}
		{^.*_e1.tar.gz$} {
			set swtype2rm "swE1";
		}
		{^.*_ena2.tar.gz$} {
			set swtype2rm "swENA2";
		}
		{^.*_ena.tar.gz$} {
			set swtype2rm "swENA";
		}
		{^.*_octds3_2.tar.gz$} {
			set swtype2rm "swOCTDS3_2";
		}
		{^.*_octds3_3.tar.gz$} {
			set swtype2rm "swOCTDS3_3";
		}
		{^.*_octds3.tar.gz$} {
			set swtype2rm "swOCTDS3";
		}
		{^.*_pna.tar.gz$} {
			set swtype2rm "swPNA";
		}
		{^.*_tdmoc.tar.gz$} {
			set swtype2rm "swTDMOC";
		}
		{^.*_trids3_3.tar.gz$} {
			set swtype2rm "swTRIDS3_3";
		}
		{^.*_trids3.tar.gz$} {
			set swtype2rm "swTRIDS3";
		}
		{^.*_voip6.tar.gz$} {
			set swtype2rm "swVOP6";
		}
		{^.*_voip.tar.gz$} {
			set swtype2rm "swVOP";
		}
		{^.*_vs3.tar.gz$} {
			set swtype2rm "swVS3";
		}
		default {
			puts "removeoldload: Unknown file type $swfile.";
			logmsg reload "END: removeoldload: Unknown file type $swfile.";
			exit 2;
		}
		}
		puts "";
		puts "Removing type $swtype2rm ($swtype) ...";
		send -i $cpu_id "rm -rf /Telica/${swtype2rm}/* 1>/dev/null 1>&2\r";
		#
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "removeoldload: timeout during rm of '$swtype'.";
			# clean up and logon again.
			catch { close $cpu_id; } ignore;
			telnet_to $cpu_ip cpu_id;
		}
		}
		if {$moreswtype2rm != ""} {
			puts "";
			puts "Removing type $moreswtype2rm ...";
			send -i $cpu_id "rm -rf /Telica/${moreswtype2rm}/* 1>/dev/null 1>&2\r";
			#
			expect {
			-i $cpu_id -re ".*COMMAND_IS_DONE>" {
				# ok
			}
			timeout {
				puts "removeoldload: timeout during rm of '$moreswtype2rm'.";
				# clean up and logon again.
				catch { close $cpu_id; } ignore;
				telnet_to $cpu_ip cpu_id;
			}
			}
		}
	}
	#
	# this keeps coring. catch it and ignore it.
	#
	trap SIG_IGN SIGBUS;
	#
	catch {
		# clean up old files and directories.
		set timeout 300;
		#
		foreach dbcmd $dbcmds {
			puts "";
			puts "Executing: $dbcmd ...";
			send -i $cpu_id "$dbcmd \r";
			#
			expect {
			-i $cpu_id -re ".*COMMAND_IS_DONE>" {
				# ok
			}
			timeout {
				puts "removeoldload: timeout during <$dbcmd>.";
			}
			}
		}
		#
	} ignore;
	#
	catch { close $cpu_id; } ignore;
	#
	trap SIG_DFL SIGBUS;
	#
	return;
}
#
proc installnewload { cpu_ip cpu_load ilistname } {
	upvar $ilistname images2load;
	#
	global username;
	global userpasswd;
	global timeout;
	#
	telnet_to $cpu_ip cpu_id;
	#
	set timeout 1500;
	#
	foreach imagedata $images2load {
		set idata [split $imagedata " \t"];
		set swtype [lindex $idata 0];
		set swfile [lindex $idata 1];
		#
		set tarfile "/home/$swfile";
		#
		puts "";
		puts "Expanding file $tarfile ...";
		# added 'm' option to allow DiskClean Up to work, as per
		# Dan Scanlon (6/6/05).
		# OLD: send -i $cpu_id "/bin/tar xzvf $tarfile\r";
		send -i $cpu_id "/bin/tar xzvmf $tarfile\r";
		#
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "installnewload: timeout during 'tar xzvmf $tarfile'.";
		}
		}
	}
	#
	set timeout 30;
	#
	puts "";
	puts "Linking $cpu_load to CurrRel ...";
	send -i $cpu_id "/bin/ln -s /Telica/swCPU/${cpu_load} /Telica/swCPU/CurrRel\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "installnewload: timeout during 'ln -s ...'.";
	}
	}
	#
	set timeout 900;
	#
	puts "";
	puts "Running uprev.sh ...";
	puts "";
	puts "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING";
	puts "";
	puts "DO NOT INTERRUPT RELOAD AT THIS POINT. TIMESTEN DATABASE IS ";
	puts "INSTALLING RIGHT NOW AND IF YOU INTERRUPT IT, YOU RISK TRASHING";
	puts "YOUR LAB AND HAVING TO RESTART.";
	puts "";
	puts "WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING";
	puts "";
	send -i $cpu_id "cd /Telica/swCPU/CurrRel/system/scripts\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "\ntimeout during cd ...";
	}
	};
	#
	send -i $cpu_id "./uprev.sh\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "installnewload: timeout during 'uprev.sh'.";
	}
	}
	#
	catch { close $cpu_id; } ignore;
	return;
}
#
# make hardware TL1 scripts using relations chassis and ioms.
#
proc makehwscripts { labid branch cpuload } {
	global env;
	#
	catch {
		system "$env(LCSTOOLSBIN)/mkhwscripts $branch $cpuload $labid";
	} ignore;
	catch { wait -nowait; } ignore;
	#
}
#
# cmds for initializing dbCurrent directory.
#
proc initdbcmds { cmdsname } {
	upvar $cmdsname cmds;
	#
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVEdbCurrent 1>/dev/null 2>&1";
	lappend cmds "mv dbCurrent SAVEdbCurrent";
	lappend cmds "mkdir dbCurrent";
	lappend cmds "cd SAVEdbCurrent";
	lappend cmds "find . -type d -print | cpio -pduvm ../dbCurrent";
	lappend cmds "cp Telica_IP Telica_Hostname ../dbCurrent";
	lappend cmds "echo '# template rc.local file' >../dbCurrent/rc.local";
	lappend cmds "rm -rf SAVEdbCurrent 1>/dev/null 2>&1";
	#
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVErestore 1>/dev/null 2>&1";
	lappend cmds "mv restore SAVErestore";
	lappend cmds "mkdir restore";
	lappend cmds "cd SAVErestore";
	lappend cmds "find . -type d -print | cpio -pduvm ../restore";
	lappend cmds "echo '# template rc.local file' >../restore/rc.local";
	lappend cmds "rm -rf SAVErestore 1>/dev/null 2>&1";
	#
	lappend cmds "rm -f /.odbc.ini 1>/dev/null 1>&2";
	lappend cmds "rm -rf /opt/TimesTen* 1>/dev/null 1>&2";
	lappend cmds "echo '# over-written by reload' >/etc/resolv.conf";
	lappend cmds "rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2";
}
#
#########################################################################
#
# default values
#
global env;
#
set username "root";
set userpasswd "plexus9000";
set maxlogons 5;
set stracelevel -1;
set usingdefaultlogfile 1;
set doloadimage 1;
set doexpansion 1;
set dohardware 0;
set ask 1;
set query 0;
set logfile "";
set verbose 0;
set labid "";
set LABID "";
set removelock 0;
set background 0;
set initsp "none";
#
# get cmd line options
#
log_user 0;
#
logusage reload;
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-K} { set doloadimage 1; set doexpansion 0; }
	{^-X} { set doloadimage 0; set doexpansion 1; }
	{^-D} { set ask 0; }
	{^-Q} { set query 1; }
	{^-V} { set verbose 1; }
	{^-R} { set removelock 1; }
	{^-B} { set background 1; set ask 0; }
	{^-H} { set dohardware 1; set doloadimage 0; set doexpansion 0; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-i.*} { getoptval $argval initsp arg; }
	{^-I.*} { 
		getoptval $argval initsp arg; 
		set doloadimage 0; 
		set doexpansion 0;
		set ask 0;
	}
	{^-\?} { usage; exit 0; }
	{^-T} { timeouthelp; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "Unknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
switch -- $initsp {
"simplex_a" { # ok }
"simplex_b" { # ok }
"duplex" { # ok }
"none" { # ok }
default {
	puts "Invalid '-i' option value: $initsp";
	logmsg reload "END: Invalid '-i' option value: $initsp";
	exit 2;
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
		puts "Logging failed for file $logfile:\n$status";
		logmsg reload "END: Logging failed for file $logfile:\n$status";
		exit 2;
	}
	set usingdefaultlogfile 0;
} else {
	# default logging for now
	set mypid [pid];
	set logfile "/lcsl100/logfiles/reload/reload$mypid";
	puts "\nDefault log file is $logfile.";
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "Logging failed for file $logfile:\n$status";
		logmsg reload "END: Logging failed for file $logfile:\n$status";
		exit 2;
	}
	set usingdefaultlogfile 1;
}
if {$verbose > 0} {
	log_user 1;
}
#
checkenv;
#
initdbcmds dbcmds;
#
# get ip addresses for labid.
#
if {$arg<$argc} {
	set labid [lindex $argv $arg];
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
} else {
	puts "LABID is neither set nor given.";
	logmsg reload "END: LABID is neither set nor given.";
	exit 2;
}
puts "\nStarting reload of lab $labid:\n";
#
if {$removelock} {
	puts "WARNING: Forced removal of LOCK file for $labid.";
	logmsg reload "WARNING: Forced removal of LOCK file for $labid.";
	set status [V reload $labid];
	logmsg reload "WARNING: status of removal of LOCK file for $labid: $status";
}
set status [P reload $labid];
if {$status == 0} {
	puts "Another reload of $labid in progress.";
	logmsg reload "END: another reload of $labid in progress.";
	unset LABID;
	exit 2;
}
set LABID $labid;
#
if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
	puts "reload: dbselect of relation 'chassis' failed.";
	logmsg reload "END: reload: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "reload: no IPs found for labid $labid.";
	logmsg reload "END: reload: no IPs found for labid $labid.";
	exit 2;
}
#
set ips [lindex $obuf 0];
set ipdata [split $ips " \t"];
set cpu_a_ip [lindex $ipdata 0];
set cpu_b_ip [lindex $ipdata 1];
#
set timeout -1;
set sp2load "";
#
do_we_reload $labid "SP-A" cpu_a_ip;
do_we_reload $labid "SP-B" cpu_b_ip;
#
if {$cpu_a_ip != "" && ($doloadimage || $doexpansion)} {
	puts "\nCurrent SP-A Status:\n";
	set spstatus [getspstatus $labid $cpu_a_ip "SP-A"];
	logmsg reload "SP-STATUS: $labid $cpu_a_ip SP-A is $spstatus.";
	if {[string match "*active*application*" $spstatus]} {
		puts "\nFAILURE: Exiting reload because NOT in Min-Mode.";
		logmsg reload "END: Exiting reload because NOT in Min-Mode.";
		exit 2;
	}
}
if {$cpu_b_ip != "" && ($doloadimage || $doexpansion)} {
	puts "\nCurrent SP-B Status:\n";
	set spstatus [getspstatus $labid $cpu_b_ip "SP-B"];
	logmsg reload "SP-STATUS: $labid $cpu_b_ip SP-B is $spstatus.";
	if {[string match "*active*application*" $spstatus]} {
		puts "\nFAILURE: Exiting reload because NOT in Min-Mode.";
		logmsg reload "END: Exiting reload because NOT in Min-Mode.";
		exit 2;
	}
}
puts "\n";
#
if {$cpu_a_ip == "" && $cpu_b_ip == ""} {
	# no problems, clean up default log file.
	if {$usingdefaultlogfile != 0} {
		if {[file exists $logfile]} {
			file delete -force -- $logfile;
		}
	}
	logmsg reload "END: nothing chosen to reload.";
	puts "Nothing to reload.";
	puts "Exiting reload.";
	exit 0;
}
#
getimages2load $labid branch cpuload images2load;
if {![info exists images2load] || [llength images2load] == 0} {
	puts "reload: no images chosen for reload. Exiting reload.";
	logmsg reload "END: reload: no images chosen for reload. Exiting reload.";
	exit 2;
}
#
# at this point, if we are backgrounding the reload, all interactive
# I/O is done since we will turn off 'ask' and not ask the "do we proceed?"
# question. So, disconnect the parent and let the child continue ...
#
if {$background != 0} {
	set ask 0;
	if {[fork] != 0} {
		puts "Backgrounding. Parent reload exiting.";
		logmsg reload "END: Backgrounding. Parent reload exiting.";
		unset LABID;
		exit 0;
	}
	# let parent exit and update log file, then continue.
	sleep 5;
	logmsg reload "Backgrounding. Child disconnecting and continuing.";
	disconnect;
}
#
if {$cpu_a_ip != "" && $cpu_b_ip != ""} {
	logmsg reload "START: reloading $labid SP-A and SP-B with load $branch, $cpuload.";
	if {$doloadimage} {
		logmsg reload "START: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
		loadimage $labid $cpu_a_ip $branch $cpuload images2load;
		loadimage $labid $cpu_b_ip $branch $cpuload images2load;
		logmsg reload "END: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
	} else {
		puts "Skipping loadimage stage ...";
	}
	if {$doexpansion} {
		if {$ask} {
			do_we_proceed;
		}
		logmsg reload "START: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
		killemf $cpu_a_ip $cpu_b_ip;
		removeoldload $cpu_a_ip images2load;
		removeoldload $cpu_b_ip images2load;
		installnewload $cpu_a_ip $cpuload images2load;
		installnewload $cpu_b_ip $cpuload images2load;
		what_to_do_next;
		logmsg reload "END: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
	} else {
		puts "Skipping expansion stage ...";
	}
	if {$initsp != "none"} {
		logmsg reload "START: INIT of $labid with load $branch, $cpuload.";
		set postcmd "";
		set rebootcmd "";
		#
		switch -- $initsp {
		"simplex_a" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -A -a $labid";
		}
		"simplex_b" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -B -a $labid";
		}
		"duplex" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -a $labid";
		}
		}
		#
		puts "\nPOST CMD: $postcmd ";
		logmsg reload "POST CMD: $postcmd ";
		puts "\nREBOOT CMD: $rebootcmd ";
		logmsg reload "REBOOT CMD: $rebootcmd ";
		#
		if {[catch { system "$postcmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\npostlcsreload: $errmsg";
				logmsg reload "ERROR: postlcsreload: $errmsg";
			} else {
				puts "\npostlcsreload: unknown error.";
				logmsg reload "ERROR: postlcsreload: unknown error.";
			}
		} elseif {[catch { system "$rebootcmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nlcsreboot: $errmsg";
				logmsg reload "ERROR: lcsreboot: $errmsg";
			} else {
				puts "\nlcsreboot: unknown error.";
				logmsg reload "ERROR: lcsreboot: unknown error.";
			}
		}
		catch { wait -nowait; } ignore;
		logmsg reload "END: INIT of $labid with load $branch, $cpuload.";
	}
	if {$dohardware} {
		logmsg reload "START: MAKEHWSCRIPTS $labid with load $branch, $cpuload.";
		makehwscripts $labid $branch $cpuload;
		logmsg reload "END: MAKEHWSCRIPTS $labid with load $branch, $cpuload.";
	} else {
		puts "Skipping make hardware scripts stage ...";
	}
	if {$usingdefaultlogfile != 0} {
		if {[file exists $logfile]} {
			file delete -force -- $logfile;
		}
	}
	logmsg reload "END: reloading $labid SP-A and SP-B with load $branch, $cpuload.";
	puts "Exiting reload.";
	exit 0
} elseif {$cpu_a_ip != ""} {
	set sp2load "SP-A";
	set cpu_ip $cpu_a_ip;
} elseif {$cpu_b_ip != ""} {
	set sp2load "SP-B";
	set cpu_ip $cpu_b_ip;
} else {
	# no problems, clean up default log file.
	if {$usingdefaultlogfile != 0} {
		if {[file exists $logfile]} {
			file delete -force -- $logfile;
		}
	}
	logmsg reload "END: nothing chosen to reload.";
	puts "Nothing to reload.";
	puts "Exiting reload.";
	exit 0
}
#
# reload one side only
#
logmsg reload "START: reloading $labid $sp2load with load $branch, $cpuload.";
if {$doloadimage} {
	logmsg reload "START: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
	loadimage $labid $cpu_ip $branch $cpuload images2load;
	logmsg reload "END: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
} else {
	puts "Skipping loadimage stage ...";
}
if {$doexpansion} {
	if {$ask} {
		do_we_proceed;
	}
	logmsg reload "START: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
	killemf $cpu_ip "";
	removeoldload $cpu_ip images2load;
	installnewload $cpu_ip $cpuload images2load;
	what_to_do_next;
	logmsg reload "END: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
} else {
	puts "Skipping expansion stage ...";
}
if {$initsp != "none"} {
	logmsg reload "START: INIT of $labid with load $branch, $cpuload.";
	set postcmd "";
	set rebootcmd "";
	#
	switch -- $initsp {
	"simplex_a" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -A -a $labid";
	}
	"simplex_b" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -B -a $labid";
	}
	"duplex" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -a $labid";
	}
	}
	#
	puts "\nPOST CMD: $postcmd ";
	logmsg reload "POST CMD: $postcmd ";
	puts "\nREBOOT CMD: $rebootcmd ";
	logmsg reload "REBOOT CMD: $rebootcmd ";
	#
	if {[catch { system "$postcmd"; } errmsg] != 0} {
		if {[string length $errmsg] > 0} {
			puts "\npostlcsreload: $errmsg";
			logmsg reload "ERROR: postlcsreload: $errmsg";
		} else {
			puts "\npostlcsreload: unknown error.";
			logmsg reload "ERROR: postlcsreload: unknown error.";
		}
	} elseif {[catch { system "$rebootcmd"; } errmsg] != 0} {
		if {[string length $errmsg] > 0} {
			puts "\nlcsreboot: $errmsg";
			logmsg reload "ERROR: lcsreboot: $errmsg";
		} else {
			puts "\nlcsreboot: unknown error.";
			logmsg reload "ERROR: lcsreboot: unknown error.";
		}
	}
	catch { wait -nowait; } ignore;
	logmsg reload "END: INIT of $labid with load $branch, $cpuload.";
}
if {$dohardware} {
	logmsg reload "START: MAKEHWSCRIPTS $labid with load $branch, $cpuload.";
	makehwscripts $labid $branch $cpuload;
	logmsg reload "END: MAKEHWSCRIPTS $labid with load $branch, $cpuload.";
} else {
	puts "Skipping make hardware scripts stage ...";
}
#
# clean up when no problems.
#
if {$usingdefaultlogfile != 0} {
	if {[file exists $logfile]} {
		file delete -force -- $logfile;
	}
}
#
logmsg reload "END: reloading $labid $sp2load with load $branch, $cpuload.";
puts "Exiting reload.";
#
exit 0;

