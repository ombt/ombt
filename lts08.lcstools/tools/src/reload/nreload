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
	puts "\nERROR: Got signal <$signame,$signo>";
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
	puts "CONGRATULATIONS -- RELOAD HAS COMPLETED SUCCESSFULLY !!!";
	puts "";
	puts "THESE ARE SUGGESTED STEPS AFTER A SUCCESSFUL RELOAD:";
	puts "";
	puts "1) For simplex labs, run the following on the LSP:";
	puts "	$ postlcsreload -s -D -h -c 10 -m";
	puts "or do exactly this as a short cut (give options in this order):"
	puts "	$ postlcsreload -a -s";
	puts "";
	puts "For duplex labs, run:";
	puts "	$ postlcsreload -d -D -h -c 10 -m";
	puts "or do this as a short cut:"
	puts "	$ postlcsreload -a";
	puts "";
	puts "2) For simplex labs, reboot EITHER SP-A or SP-B, NEVER BOTH SPs:";
	puts "	$ lcsreboot -A	# for SP-A init, do NOT boot SP-B";
	puts "or";
	puts "	$ lcsreboot -B	# for SP-B init, do NOT boot SP-A";
	puts "For duplex labs, just enter:";
	puts "	$ lcsreboot	# init's both SPs";
	puts "or";
	puts "	$ lcsreboot -a	# init's both SPs";
	puts "";
	puts "3) For duplex labs, wait for the SPs to be IS,STBYH and IS,ACT.";
	puts "and for simplex labs, for the one SP to be IS,NBK, then you can";
	puts "continue on to provisioning.";
}
#
proc tstamp { } {
	global dotimestamp;
	if {$dotimestamp != 0} {
		puts "TIMESTAMP: [timestamp]";
	}
}
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-s] \[-B] \[-V] \[-K|-X] \[-R] \[-D] \[-T] \[-U]";
	puts "\[-S trace level] \[-l logfile] \[-u username] \[-p passwd] \[-Q]";
	puts "\[ -\[i|I] simplex_a | -\[i|I] simplex_b | -\[i|I] duplex ] ";
	puts "\[-c cpuload \[-a] \[-b] ] \[-m mapfile] \[-t tarfile] \[labid]";
	puts "\nwhere:";
	puts "	-x - enable TCL debugger";
	puts "	-s - enable timestamps";
	puts "	-S tracelevel - set trace to given stack level ";
	puts "	-K - do loadimage only; skip expansion.";
	puts "	-X - do expansion only; skip loadimage";
	puts "	-D - do NOT ask before proceeding with expansion (default)";
	puts "	-A - do ask before proceeding with expansion";
	puts "	-l logfile - log all output to a logfile";
	puts "	-V - turn on verbose";
	puts "	-U - unconditional";
	puts "	-T - help text for recovering from time outs.";
	puts "	-Q - ask which IOM/CPU image to reload.";
	puts "	-R - remove lock file. see below before using this option.";
	puts "	-m mapfile - file contains list of private products to install";
	puts "	-t tarfile - file contains list of private tar files to install";
	puts "\nOptions for reloading in the background are:"
	puts "	-B - background reload.";
	puts "	-c cpuload - cpu load to used for reload.";
	puts "	-a - reload SP-A";
	puts "	-b - reload SP-B";
	puts "";
	puts "	default username = $username";
	puts "	default passwd = $userpasswd";
	puts "	labid is read from variable LABID, unless given.";
	puts "";
	puts "By default, only the CPU and the IOMs listed in the chassis2ioms";
	puts "relation are reloaded. If a chassis2ioms tuple does not exist";
	puts "for the chosen lab and branch, then all IOMs/CPU are reloaded.";
	puts "If you want to be asked which IOMs/CPU to reload, use the -Q ";
	puts "option.";
	puts "";
	puts "You can automatically reboot the switch after a reload by ";
	puts "using the -i option. The choices are:";
	puts "";
	puts "	-i simplex_a - reload, split lab, boot SP-A active";
	puts "	-i simplex_b - reload, split lab, boot SP-B active";
	puts "	-i duplex - reload, duplex lab, boot SP-A and SP-B active";
	puts "";
	puts "	-I simplex_a - skip reload, split lab, boot SP-A active";
	puts "	-I simplex_b - skip reload, split lab, boot SP-B active";
	puts "	-I duplex - skip reload, duplex lab, boot SP-A and SP-B active";
	puts "";
	puts "If you are absolutely sure that NO ONE else is running a";
	puts "a reload and you are blocked by the lock file, then you can";
	puts "run reload with the -R option to remove the lock. Remember,";
	puts "if you remove the lock and there really is another reload";
	puts "in progress for the same lab, each reload session will trash";
	puts "the other. So, don't remove the lock, unless you are sure.";
	puts "";
	puts "If you use the -c option to choose a CPU load, then you must";
	puts "use the -a or -b options to choose whether or not to";
	puts "reload SP-A or SP-B. Use both -a and -b togther to reload ";
	puts "both SPs.";
	puts "";
	puts "When using the '-m mapfile' option, the map file must have this";
	puts "format:";
	puts "";
	puts "path_to_user_product1 path_to_chassis_product1";
	puts "path_to_user_product2 path_to_chassis_product2";
	puts "...";
	puts "";
	puts "Here is an example of a map file, both files names are on the same line:";
	puts "\n/lcsl101/text/Main/cpu/Telica/swCPU/6.2.1.0.49/apps/tl1agent/tl1agent /Telica/swCPU/6.2.1.0.49/apps/tl1agent/tl1agent";
	puts "";
	puts "When using the '-t tarfile' option, the tar file must have this";
	puts "format:";
	puts "";
	puts "path_to_user_tar_file";
	puts "path_to_user_tar_file2";
	puts "...";
	puts "";
	puts "Here's an example of a tarfile. The full paths are given to the private tar files:";
	puts "/lcsl101/text/Main/cpu/6.2.1.0.52_cpu.tar.gz";
	puts "/lcsl101/text/Main/cm/6.2.1.0.20_cm.tar.gz";
	puts "...";
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
proc sendemail { { subject "reload default subject" } { message "reload default message" } } {
	global emailaddress;
	global labid;
	if {[string length ${labid}] <= 0} {
		set emaillabid "UNKNOWN";
	} else {
		set emaillabid ${labid};
	}
	catch {
		system "echo \"Reload of LABID <${emaillabid}>:\n[ exec id ]\n${message}\" | /usr/bin/mailx -s \"${subject} - ${emaillabid}\" ${emailaddress}";
	} ignore;
	catch { wait -nowait; } ignore;
	return;
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
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		set timeout $savetimeout;
		return $status;
	}
	timeout {
		puts "Timed out connecting to $ip ...";
		set status "telnet time out";
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
	-re "(\[a-zA-Z0-9_-]+#)" {
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		set timeout $savetimeout;
		return $status;
	}
	}
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
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
proc getimages2load { labid reloadcpuload bname lname ilistname } {
	upvar $ilistname imagelist;
	upvar $bname branch;
	upvar $lname cpuload;
	global logfile;
	global usingdefaultlogfile;
	global query;
	global unconditional;
	global fileowner;
	#
	# check if file owner is running reload or not.
	#
	if {$fileowner} {
		set statusclause "";
	} else {
		set statusclause " and status req ^oper\$";
	}
	puts "\nstatus clause is: ${statusclause}";
	#
	if {$reloadcpuload == ""} {
		if {[dbselect llobuf labloads "labid req ^$labid\$$statusclause" "branch cpuload" ] != 0} {
			puts "\nERROR: getimages2load: dbselect of relation 'labloads' failed.";
			sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'labloads' failed.";
			logmsg reload "END: getimages2load: dbselect of relation 'labloads' failed.";
			exit 2;
		}
	} else {
		if {[dbselect llobuf labloads "labid req ^$labid\$ and cpuload req ^$reloadcpuload\$$statusclause" "branch cpuload" ] != 0} {
			puts "\nERROR: getimages2load: dbselect of relation 'labloads' failed.";
			sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'labloads' failed.";
			logmsg reload "END: getimages2load: dbselect of relation 'labloads' failed.";
			exit 2;
		}
	}
	if {![info exists llobuf] || [llength llobuf] == 0} {
		puts "\nERROR: getimages2load: no loads found for labid $labid.";
		sendemail "Reload ERROR" "END: getimages2load: no loads found for labid $labid.";
		logmsg reload "END: getimages2load: no loads found for labid $labid.";
		exit 2;
	}
	#
	if {$reloadcpuload == ""} {
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
			sendemail "Reload SUCCESS" "END: user chooses to exit.";
			logmsg reload "END: user chooses to exit.";
			exit 0;
		}
	} else {
		set choice [lindex $llobuf 0];
	}
	#
	set ldata [split $choice " \t"];
	set branch [lindex $ldata 0];
	set cpuload [lindex $ldata 1];
	#
	puts "Will reload load <$branch, $cpuload>.";
	#
	if {[dbselect iobuf images "branch req ^$branch\$ and cpuload req ^$cpuload\$ and type rne ^bts\$ and type rne ^tca\$ and type rne debug" "type name" ] != 0} {
		puts "\nERROR: getimages2load: dbselect of relation 'images' failed.";
		sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'images' failed.";
		logmsg reload "END: getimages2load: dbselect of relation 'images' failed.";
		exit 2;
	}
	if {![info exists iobuf] || [llength iobuf] == 0} {
		puts "\nERROR: getimages2load: no images found for <$branch, $cpuload>.";
		sendemail "Reload ERROR" "END: getimages2load: no images found for <$branch, $cpuload>.";
		logmsg reload "END: getimages2load: no images found for <$branch, $cpuload>.";
		exit 2;
	}
	#
	set timeout -1;
	if {[info exists imagelist]} {
		unset imagelist;
	}
	#
	# if we are not asking the user for which images to reload,
	# then query the database. if nothing is found, then reload
	# everything, else only reload what the database relation
	# chassis2ioms returned.
	#
	if {$unconditional} {
		# skip the check of relation chassis2ioms.
		set iomdataexists 0;
		puts "Chassis-To-IOM-Type Data for $labid: unconditional reload, ignore data.";
		sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: unconditional reload, ignore data.";
		logmsg reload "Chassis-To-IOM-Type Data for $labid: unconditional reload, ignore data.";
	} elseif {$query == 0} {
		# get the list of ioms for this chassis
		if {[dbselect chiomsbuf chassis2ioms "branch req ^$branch\$ and labid req ^$labid\$" "type" ] != 0} {
			puts "\nERROR: getimages2load: dbselect of relation 'chassis2ioms' failed.";
			sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'chassis2ioms' failed.";
			logmsg reload "END: getimages2load: dbselect of relation 'chassis2ioms' failed.";
			exit 2;
		}
		if {![info exists chiomsbuf] || [llength chiomsbuf] == 0} {
			#
			# we do not have data for this branch and labid.
			# try getting the data for the base branch and
			# the same labid.
			#
			puts "Chassis-To-IOM-Type Data for $labid: none available for branch $branch; try again using base branch.";
			sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: none available for branch $branch; try again using base branch.";
			logmsg reload "Chassis-To-IOM-Type Data for $labid: none available for branch $branch; try again using base branch.";
			#
			if {[dbselect basebuf basebranch "branch req ^$branch\$ and type req ^tl1\$" "base" ] != 0} {
				puts "\nERROR: getimages2load: dbselect of relation 'basebranch' failed.";
				sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'basebranch' failed.";
				logmsg reload "END: getimages2load: dbselect of relation 'basebranch' failed.";
				exit 2;
			}
			if {![info exists basebuf] || [llength basebuf] == 0} {
				set iomdataexists 0;
				puts "Chassis-To-IOM-Type Data for $labid: base branch does not exist for $branch. reload everything.";
				sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: base branch does not exist for $branch. reload everything.";
				logmsg reload "Chassis-To-IOM-Type Data for $labid: base branch does not exist for $branch. reload everything.";
			} else {
				# we found the base branch. use it to 
				# get the chassis iom data.
				set basebranch [lindex $basebuf 0];
				puts "Chassis-To-IOM-Type Data for $labid: base branch for $branch is $basebranch.";
				sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: base branch for $branch is $basebranch.";
				logmsg reload "Chassis-To-IOM-Type Data for $labid: base branch for $branch is $basebranch.";
				#
				# get the list of ioms for this chassis
				if {[dbselect chiomsbuf chassis2ioms "branch req ^$basebranch\$ and labid req ^$labid\$" "type" ] != 0} {
					puts "\nERROR: getimages2load: dbselect of relation 'chassis2ioms' failed for base branch.";
					sendemail "Reload ERROR" "END: getimages2load: dbselect of relation 'chassis2ioms' failed for base branch.";
					logmsg reload "END: getimages2load: dbselect of relation 'chassis2ioms' failed for base branch.";
					exit 2;
				}
				if {![info exists chiomsbuf] || [llength chiomsbuf] == 0} {
					set iomdataexists 0;
					puts "Chassis-To-IOM-Type Data for $labid: none available for base branch ${basebranch}; reload everything.";
					sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: none available for base branch ${basebranch}; reload everything.";
					logmsg reload "Chassis-To-IOM-Type Data for $labid: none available for base branch ${basebranch}; reload everything.";
				} else {
					# always add cpu
					lappend chiomsbuf cpu;
					set iomdataexists 1;
					puts "Chassis-To-IOM-Type Data for $labid and base branch $basebranch: $chiomsbuf";
					sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid and base branch $basebranch: $chiomsbuf";
					logmsg reload "Chassis-To-IOM-Type Data for $labid and base branch $basebranch: $chiomsbuf";
				}
			}
		} else {
			# always add cpu
			lappend chiomsbuf cpu;
			set iomdataexists 1;
			puts "Chassis-To-IOM-Type Data for $labid and branch $branch: $chiomsbuf";
			sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid and branch $branch: $chiomsbuf";
			logmsg reload "Chassis-To-IOM-Type Data for $labid and branch $branch: $chiomsbuf";
		}
	} else {
		puts "Chassis-To-IOM-Type Data for $labid: asking user, ignore data.";
		sendemail "Reload INFO" "Chassis-To-IOM-Type Data for $labid: asking user, ignore data.";
		logmsg reload "Chassis-To-IOM-Type Data for $labid: asking user, ignore data.";
	}
	#
	foreach image $iobuf {
		set iomtype [lindex $image 0];
		set iomfile [lindex $image 1];
		#
		if {$iomtype == "bts"} {
			# chassis does not need BTS images
			puts "\nWARNING: skipping unneeded BTS image - $iomfile.";
			continue;
		} elseif {$iomtype == "tca"} {
			# chassis does not need TCA images
			puts "\nWARNING: skipping unneeded TCA image - $iomfile.";
			continue;
		}
		#
		if {$query == 0} {
			if {$iomdataexists} {
				if {[lsearch -exact $chiomsbuf $iomtype] != -1} {
					puts "Will reload $image ...\n";
					lappend imagelist $image;
				} 
			} else {
				puts "Will reload $image ...\n";
				lappend imagelist $image;
			}
		} else {
			send_user "\nReload this image <$image>? \[y/n/cr=n] ";
			expect_user {
			-re "y\n" {
				puts "Will reload $image ...\n";
				lappend imagelist $image;
			}
			-re "\[^\n]*\n" {
				puts "Will NOT reload $image ...\n";
			}
			eof {
				puts "\nERROR: END-OF-FILE. exiting reload.\n";
				sendemail "Reload ERROR" "END: END-OF-FILE. exiting reload.\n";
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
		puts "\nERROR: timeout during telnet ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during telnet ...";
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
			puts "\nERROR: timeout during telnet, waiting for password: ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during telnet, waiting for password: ...";
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
		puts "\nERROR: timeout during prompt ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during prompt ...";
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
		puts "\nERROR: timeout during PS1= ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during PS1= ...";
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# send "/bin/find $remotefs -type f -print | /bin/cat \r";
		# clear files and links ...
		send "/bin/find $remotefs \\( -type f -o -type l \\) -print | /bin/xargs rm -f \r";
	}
	timeout {
		puts "\nERROR: timeout during cd ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during cd ...";
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
		puts "\nERROR: timeout during rm ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during rm ...";
		logmsg reload "END: timeout during rm ...";
		exit 2;
	}
	};
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	#
	set timeout $savetimeout;
}
#
proc move_remote_file { machine remotefile savedir } {
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
		puts "\nERROR: timeout during telnet ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during telnet ...";
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
			puts "\nERROR: timeout during telnet, waiting for password: ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during telnet, waiting for password: ...";
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
		puts "\nERROR: timeout during prompt ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during prompt ...";
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
		puts "\nERROR: timeout during PS1= ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during PS1= ...";
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "/bin/mv ${remotefile} ${savedir}\r";
	}
	timeout {
		puts "\nERROR: timeout during cd ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during cd ...";
		logmsg reload "END: timeout during cd ...";
		exit 2;
	}
	};
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "\nERROR: timeout during mv to ${savedir} ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during mv to ${savedir} ...";
		logmsg reload "END: timeout during mv to ${savedir} ...";
		exit 2;
	}
	};
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	#
	set timeout $savetimeout;
}
#
proc remove_remote_file { machine remotefile { mvtotmp 0 } { rflag 0 } } {
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
		puts "\nERROR: timeout during telnet ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during telnet ...";
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
			puts "\nERROR: timeout during telnet, waiting for password: ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during telnet, waiting for password: ...";
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
		puts "\nERROR: timeout during prompt ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during prompt ...";
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
		puts "\nERROR: timeout during PS1= ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during PS1= ...";
		logmsg reload "END: timeout during PS1= ...";
		exit 2;
	}
	};
	#
	if {$mvtotmp == 0} {
		expect {
		-re ".*COMMAND_IS_DONE>" {
			if {$rflag != 0} {
				send "/bin/rm -rf $remotefile \r";
			} else {
				send "/bin/rm -f $remotefile \r";
			}
		}
		timeout {
			puts "\nERROR: timeout during cd ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during cd ...";
			logmsg reload "END: timeout during cd ...";
			exit 2;
		}
		};
	} else {
		expect {
		-re ".*COMMAND_IS_DONE>" {
			send "/bin/mv $remotefile /tmp\r";
		}
		timeout {
			puts "\nERROR: timeout during cd ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during cd ...";
			logmsg reload "END: timeout during cd ...";
			exit 2;
		}
		};
		expect {
		-re ".*COMMAND_IS_DONE>" {
			if {$rflag != 0} {
				send "/bin/rm -rf /tmp/$remotefile \r";
			} else {
				send "/bin/rm -f /tmp/$remotefile \r";
			}
		}
		timeout {
			puts "\nERROR: timeout during mv to tmp ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during mv to tmp ...";
			logmsg reload "END: timeout during mv to tmp ...";
			exit 2;
		}
		};
	}
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "\nERROR: timeout during rm ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during rm ...";
		logmsg reload "END: timeout during rm ...";
		exit 2;
	}
	};
	#
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	#
	set timeout $savetimeout;
}
#
proc loadprivprods { cpu_ip } {
	global mapfiledata;
	#
	if {![info exists mapfiledata]} {
		# nothing to do.
		return;
	}
	#
	puts "\nLoading private files on $cpu_ip:";
	#
	telnet_to $cpu_ip cpu_id;
	set fromfiles [array names mapfiledata];
	#
	foreach fromfile $fromfiles {
		set tofile $mapfiledata($fromfile);
		#
		puts "\nCPU-IP    : $cpu_ip";
		puts "From file : $fromfile";
		puts "To file   : $tofile";
		#
		# save official file when loading a private executable.
		#
		puts "\nMoving file $tofile to /home1 on $cpu_ip ..."
		move_remote_file $cpu_ip $tofile "/home1";
		#
		puts "\nLoading file $fromfile to $tofile on $cpu_ip ..."
		ftp_put_binary_file $cpu_ip $fromfile $tofile;
		#
		puts "\nMake file $tofile on $cpu_ip executable ..."
		set timeout 10;
		send -i $cpu_id "chmod 777 ${tofile}\r"; 
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			# ok
		}
		eof {
			# ok???
		}
		}
	}
	#
	# close telnet session
	#
	set spawn_id $cpu_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	#
	return;
}
#
proc loadimage { labid cpu_ip branch cpuload ilistname } {
	upvar $ilistname images2load;
	global tarfiledata;
	# get text file system
	if {[dbselect fsobuf filesystems "branch req ^$branch\$ and type req text" "path" ] != 0} {
		puts "\nERROR: loadimage: dbselect of relation 'filesystems' failed.";
		sendemail "Reload ERROR" "END: loadimage: dbselect of relation 'filesystems' failed.";
		logmsg reload "END: loadimage: dbselect of relation 'filesystems' failed.";
		exit 2;
	}
	if {![info exists fsobuf] || [llength fsobuf] == 0} {
		puts "\nERROR: loadimage: no filesystem found for branch $branch.";
		sendemail "Reload ERROR" "END: loadimage: no filesystem found for branch $branch.";
		logmsg reload "END: loadimage: no filesystem found for branch $branch.";
		exit 2;
	}
	set filesystem [lindex $fsobuf 0];
	#
	tstamp;
	puts "\nClearing /home filesystem on $cpu_ip ...";
	clear_remote_fs $cpu_ip "/home";
	puts "\nClearing /home1 filesystem on $cpu_ip ...";
	clear_remote_fs $cpu_ip "/home1";
	puts "\nRemoving tar files /*.tar.gz on $cpu_ip ..."
	remove_remote_file $cpu_ip "/*.tar.gz" 1 0;
	puts "\nRemoving directories under /home1/Telica on $cpu_ip ..."
	remove_remote_file $cpu_ip "/home1/Telica/*" 0 1;
	tstamp;
	#
	foreach imagedata $images2load {
		set idata [split $imagedata " \t"];
		set swtype [lindex $idata 0];
		set swfile [lindex $idata 1];
		#
		if {[regexp "^\[a-z0-9A-Z\.]*(_.*)$" $swfile ignore suffix] != 1} {
			puts "\nERROR: loadimage: invalid file name - $swfile.";
			sendemail "Reload ERROR" "END: loadimage: invalid file name - $swfile.";
			logmsg reload "END: loadimage: invalid file name - $swfile.";
			exit 2;
		}
		#
		if {[info exists tarfiledata($swtype)]} {
			set fromfile $tarfiledata($swtype);
			set swfile [file tail ${fromfile}];
			puts "\nWARNING: Using private tar file for $swtype: $fromfile";
		} else {
			set fromfile "$filesystem/$branch/$swtype/$swfile";
		}
		set tofile "/home/$swfile";
		#
		puts "";
		tstamp;
		puts "CPU-IP    : $cpu_ip";
		puts "Suffix    : $suffix";
		puts "From file : $fromfile";
		puts "To file   : $tofile";
		#
		tstamp;
		puts "\nLoading file $tofile ..."
		ftp_put_binary_file $cpu_ip $fromfile $tofile;
		tstamp;
	}
}
#
proc do_we_proceed { { question "installation" } } {
	send_user "Proceed with $question? \[y/n/cr=n] ";
	expect_user {
	-re "\[yY].*\n" {
		puts "Proceeding ...\n\n";
	}
	-re "(\[^\n]*)\n" {
		puts "\nERROR: Exiting reload.\n\n";
		sendemail "Reload ERROR" "END: Exiting reload.";
		logmsg reload "END: Exiting reload.";
		exit 2;
	}
	eof {
		puts "\nERROR: END-OF-FILE. exiting reload.\n";
		sendemail "Reload ERROR" "END: END-OF-FILE. exiting reload.";
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
		puts "\n==>> Will reload lab $labid $sp.\n\n";
	}
	-re "(\[^\n]*)\n" {
		puts "\n==>> Will NOT reload lab $labid.\n\n";
		set cpu_ip "";
	}
	eof {
		puts "\nERROR: END-OF-FILE. exiting reload.\n";
		sendemail "Reload ERROR" "END: END-OF-FILE. exiting reload.";
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
		puts "\nERROR: timeout during login ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during login ...";
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
			puts "\nERROR: timeout during passwd ...";
			timeouthelp;
			sendemail "Reload ERROR" "END: timeout during passwd ...";
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
		puts "\nERROR: timeout during PS1= ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during PS1= ...";
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
		puts "\nERROR: timeout during cd ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during cd ...";
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
		puts "\nERROR: timeout during cd ...";
		timeouthelp;
		sendemail "Reload ERROR" "END: timeout during cd ...";
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
		# catch { close $cpu_a_id; } ignore;
		set spawn_id $cpu_a_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		#
		# catch { close $cpu_b_id; } ignore;
		set spawn_id $cpu_b_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
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
	# catch { close $cpu_id; } ignore;
	set spawn_id $cpu_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
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
	tstamp;
	telnet_to $cpu_ip cpu_id;
	#
	tstamp;
	set timeout 120;
	send -i $cpu_id "/Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed\r";
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "removeoldload: timeout waiting for remove_configuration_data.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	tstamp;
	#
	set timeout 600;
	#
	tstamp;
	puts "\nDoes dbCurrent have to be restored?";
	send -i $cpu_id "\[\[ -h /Telica/dbCurrent ]] && /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent;\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# verify /Telica/dbCurrent is really a directory.
		send -i $cpu_id "\[\[ -d /Telica/dbCurrent ]]\r";
	}
	timeout {
		puts "removeoldload: timeout during restore of dbCurrent.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
		send -i $cpu_id "echo STATUS=$?\r";
	}
	timeout {
		puts "removeoldload: timeout echo STATUS.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	# get return code.
	#
	expect {
	-i $cpu_id -re {.*STATUS=([-0-9]+)[\r\n]*COMMAND_IS_DONE>} {
		# ok
		puts "\n Checking dbCurrent STATUS: $expect_out(1,string)";
		set status $expect_out(1,string);
		if {$status != 0} {
			puts "\nERROR: /Telica/dbCurrent is NOT a directory.";
			set spawn_id $cpu_id;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait; };
			sendemail "Reload ERROR" "END: removeoldload: /Telica/dbCurrent is NOT a directory.";
			logmsg reload "END: removeoldload: /Telica/dbCurrent is NOT a directory.";
			exit 2;
		}
	}
	timeout {
		puts "removeoldload: timeout checking STATUS.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		telnet_to $cpu_ip cpu_id;
	}
	}
	tstamp;
	#
	send -i $cpu_id "/bin/remove_all_extra_partitions\r";
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "removeoldload: timeout waiting for remove_all_extra_partitions.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	tstamp;
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
			# catch { close $cpu_id; } ignore;
			set spawn_id $cpu_id;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait; };
			puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
			telnet_to $cpu_ip cpu_id;
		}
	}
	timeout {
		puts "removeoldload: timeout during setup.sh.";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		telnet_to $cpu_ip cpu_id;
	}
	}
	#
	tstamp;
	puts "\nRemoving old load /Telica/sw*/* directories ...";
	send -i $cpu_id "rm -rf /Telica/sw*/* 1>/dev/null 1>&2\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		puts "removeoldload: timeout during rm of /Telica/sw*/*";
		# clean up and logon again.
		# catch { close $cpu_id; } ignore;
		puts "WARNING: restarting TELNET to $cpu_ip AGAIN !!!";
		set spawn_id $cpu_id;
		catch { close; } ignore;
		catch { exec kill -9 [exp_pid]; } ignore;
		catch { wait; };
		telnet_to $cpu_ip cpu_id;
	}
	}
	tstamp;
	#
	# this keeps coring. catch it and ignore it.
	#
	trap SIG_IGN SIGBUS;
	#
	catch {
		# clean up old files and directories.
		set timeout 300;
		#
		log_user 1;
		foreach dbcmd $dbcmds {
			tstamp;
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
		log_user 0;
		#
	} ignore;
	tstamp;
	#
	# catch { close $cpu_id; } ignore;
	set spawn_id $cpu_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	#
	trap SIG_DFL SIGBUS;
	#
	return;
}
#
proc loadkludge { cpu_ip telnet_spawn_id { timeout 20 } } {
	#
	foreach kfile { "restore_dbCurrent" "move_dbCurrent" "partition_create_extended"} {
		ftp_put_binary_file $cpu_ip "/lcsl101/text/Main/cpu/${kfile}" "/Telica/swCPU/CurrRel/system/scripts/${kfile}";
		#
		send -i $telnet_spawn_id "chmod 777 /Telica/swCPU/CurrRel/system/scripts/${kfile}\r";
		#
		expect {
		-i $telnet_spawn_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			# ok
		}
		eof {
			# ok???
		}
		}
	}
	#
	return "0 - success";
}
#
proc installnewload { cpu_ip cpu_load ilistname } {
	upvar $ilistname images2load;
	#
	global username;
	global userpasswd;
	global timeout;
	global verbose;
	global tarfiledata;
	#
	tstamp;
	telnet_to $cpu_ip cpu_id;
	#
	set timeout 1500;
	#
	# cpu tar file has to be installed first in case
	# we also have a cm tar file.
	#
	set swtypeorder [list cpu cm];
	#
	foreach searchswtype ${swtypeorder} {
		puts "\nInstall new load: looking for ${searchswtype}";
		foreach imagedata $images2load {
			set idata [split $imagedata " \t"];
			set swtype [lindex $idata 0];
			set swfile [lindex $idata 1];
			#
			# check if we have a required type
			#
			if {$swtype != ${searchswtype}} {
				# not what we are looking for
				puts "\nInstall new load: skipping ${swtype}";
				continue;
			}
			puts "\nInstall new load: found ${swtype}";
			#
			if {[info exists tarfiledata($swtype)]} {
				set fromfile $tarfiledata($swtype);
				set swfile [file tail ${fromfile}];
				set tarfile "/home/$swfile";
				puts "\nWARNING: Using private tar file for $swtype: $tarfile";
			} else {
				set tarfile "/home/$swfile";
			}
			#
			puts "";
			tstamp;
			puts "Expanding file $tarfile ...";
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
			# write last reload file on the switch.
			puts "Saving ${swtype} tar file name $swfile in /.last_reload_${swtype}";
			send -i $cpu_id "echo \"${swfile}\" >/.last_reload_${swtype}\r";
			#
			expect {
			-i $cpu_id -re ".*COMMAND_IS_DONE>" {
				# ok
			}
			timeout {
				puts "installnewload: timeout during /.last_reload_${swtype} creation.";
			}
			}
		}
	}
	#
	foreach imagedata $images2load {
		set idata [split $imagedata " \t"];
		set swtype [lindex $idata 0];
		set swfile [lindex $idata 1];
		#
		# check if we already did this type
		#
		set done 0;
		foreach searchswtype ${swtypeorder} {
			if {$swtype == $searchswtype} {
				# this is done
				set done 1;
				break;
			}
		}
		if {$done} {
			# skip it
			continue;
		}
		#
		if {[info exists tarfiledata($swtype)]} {
			set fromfile $tarfiledata($swtype);
			set swfile [file tail ${fromfile}];
			set tarfile "/home/$swfile";
			puts "\nWARNING: Using private tar file for $swtype: $tarfile";
		} else {
			set tarfile "/home/$swfile";
		}
		#
		puts "";
		tstamp;
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
		# write last reload file on the switch.
		puts "Saving ${swtype} tar file name $swfile in /.last_reload_${swtype}";
		send -i $cpu_id "echo \"${swfile}\" >/.last_reload_${swtype}\r";
		#
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "installnewload: timeout during /.last_reload_${swtype} creation.";
		}
		}
	}
	#
	set timeout 30;
	#
	puts "";
	tstamp;
	#
	# check if we have a private CPU tar file.
	#
	if {[info exists tarfiledata(cpu)]} {
		set fromfile $tarfiledata(cpu);
		set privtarfile [file tail ${fromfile}];
		# get cpu load of file
		if {[regexp "^(\[a-z0-9A-Z\.]*)_cpu.tar.gz$" $privtarfile ignore priv_cpu_load] != 1} {
			puts "\nERROR: unknown private cpu tar file load - $privtarfile.";
			sendemail "Reload ERROR" "END: ERROR: unknown private cpu tar file load - $privtarfile.";
			logmsg reload "END: ERROR: unknown private cpu tar file load - $privtarfile.";
			exit 2;
		}
		puts "Saving load name $priv_cpu_load in /.last_reload";
		send -i $cpu_id "echo \"${priv_cpu_load}\" >/.last_reload\r";
		#
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "installnewload: timeout during /.last_reload creation.";
		}
		}
		#
		regsub -- {([0-9][0-9]*.P[0-9][0-9]*)\.[0-9][0-9]*} $priv_cpu_load "\\1" link_cpu_load;
        	regsub -- {([0-9][0-9]*.SP\.[0-9][0-9]*)\.[0-9][0-9]*} $link_cpu_load "\\1" link_cpu_load2;
		puts "\nWARNING: Using CPU load ($priv_cpu_load) from private tar file: $privtarfile";
		puts "Linking $link_cpu_load2 to CurrRel ...";
		send -i $cpu_id "/bin/ln -s /Telica/swCPU/${link_cpu_load2} /Telica/swCPU/CurrRel\r";
	} else {
		puts "Saving load name $cpu_load in /.last_reload";
		send -i $cpu_id "echo \"${cpu_load}\" >/.last_reload\r";
		#
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "installnewload: timeout during /.last_reload creation.";
		}
		}
		#
		regsub -- {([0-9][0-9]*.P[0-9][0-9]*)\.[0-9][0-9]*} $cpu_load "\\1" link_cpu_load;
        	regsub -- {([0-9][0-9]*.SP\.[0-9][0-9]*)\.[0-9][0-9]*} $link_cpu_load "\\1" link_cpu_load2;

		puts "Linking $link_cpu_load2 to CurrRel ...";
		send -i $cpu_id "/bin/ln -s /Telica/swCPU/${link_cpu_load2} /Telica/swCPU/CurrRel\r";
	}
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
	# now is the time for all good kludges to kludge in ...
	#
	# loadkludge $cpu_ip $cpu_id;
	#
	puts "";
	tstamp;
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
	tstamp;
	#
	log_user 1;
	#
	send -i $cpu_id "./uprev.sh\r";
	#
	expect {
	-i $cpu_id -re ".*COMMAND_IS_DONE>" {
		# ok
		send -i $cpu_id "echo STATUS=$?\r";
	}
	timeout {
		puts "installnewload: timeout during 'uprev.sh'.";
	}
	eof {
		puts "installnewload: eof during 'uprev.sh'.";
	}
	}
	#
	# get return code.
	#
	expect {
	-i $cpu_id -re {.*STATUS=([-0-9]+)[\r\n]*COMMAND_IS_DONE>} {
		# ok
		puts "\nTIMES TEN RETURN STATUS: $expect_out(1,string)";
		set status $expect_out(1,string);
		if {$status != 0} {
			puts "\nERROR: INSTALLING TIMES TEN FAILED !!!";
			set spawn_id $cpu_id;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait; };
			sendemail "Reload ERROR" "END: installnewload: uprev.sh failed to install.";
			logmsg reload "END: installnewload: uprev.sh failed to install.";
			exit 2;
		}
	}
	timeout {
		puts "installnewload: timeout during 'uprev.sh'.";
	}
	eof {
		puts "installnewload: eof during 'uprev.sh'.";
	}
	}
	tstamp;
	#
	if {$verbose <= 0} {
		log_user 0;
	}
	#
	# catch { close $cpu_id; } ignore;
	set spawn_id $cpu_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
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
	catch { wait; } ignore;
}
#
# ship over files before reload.
#
proc preload { labid cpu_ip } {
	global env;
	#
	set files(1,from) $env(LCSTOOLSLYNXBIN)/rm;
	set files(1,to) /bin/rm;
	#
	set files(2,from) $env(LCSHUGSCRIPTS)/remove_hd2e;
	set files(2,to) /home/remove_hd2e;
	#
	set files(3,from) $env(LCSHUGSCRIPTS)/remove_all_extra_partitions;
	set files(3,to) /bin/remove_all_extra_partitions;
	#
	telnet_to $cpu_ip cpu_id;
	#
	for {set idx 1} {[info exists files(${idx},from)]} {incr idx} {
		# get file names
		set fromfile $files(${idx},from);
		set tofile $files(${idx},to);
		#
		puts "\nPreload:";
		puts "From File: ${fromfile}";
		puts "To File: ${tofile}";
		# send file over
		ftp_put_binary_file $cpu_ip ${fromfile} ${tofile};
		# make file executable.
		send -i $cpu_id "/bin/chmod 755 $files(${idx},to)\r";
		expect {
		-i $cpu_id -re ".*COMMAND_IS_DONE>" {
			# ok
		}
		timeout {
			puts "\nERROR: preload: timeout waiting for chmod 755 ${tofile}.";
			set spawn_id $cpu_id;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait; };
			sendemail "Reload ERROR" "END: preload: timeout waiting for chmod 755 ${tofile}.";
			logmsg reload "END: preload: timeout waiting for chmod 755 ${tofile}.";
			exit 2;
		}
		eof {
			puts "\nERROR: preload: eof waiting for chmod 755 ${tofile}.";
			set spawn_id $cpu_id;
			catch { close; } ignore;
			catch { exec kill -9 [exp_pid]; } ignore;
			catch { wait; };
			sendemail "Reload ERROR" "END: preload: eof waiting for chmod 755 ${tofile}.";
			logmsg reload "END: preload: eof waiting for chmod 755 ${tofile}.";
			exit 2;
		}
		}
	}
	#
	set spawn_id $cpu_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait; };
	return "0 - success";
}
#
# cmds for initializing dbCurrent directory.
#
proc initdbcmds { cmdsname } {
	upvar $cmdsname cmds;
	#
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVEdbCurrent 1>/dev/null 2>&1";
	#
	# check if we have to run the script to restore dbCurrent
	# from /home_e to /.
	#
	lappend cmds "\[\[ -h /Telica/dbCurrent ]] && /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent;";
	#
	lappend cmds "mv dbCurrent SAVEdbCurrent";
	lappend cmds "mkdir dbCurrent";
	lappend cmds "cd SAVEdbCurrent";
	lappend cmds "find . -type d -print | cpio -pduvm ../dbCurrent";
	lappend cmds "cp Telica_IP Telica_Hostname ../dbCurrent";
	lappend cmds "echo '# template rc.local file' >../dbCurrent/rc.local";
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVEdbCurrent 1>/dev/null 2>&1";
	#
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVErestore 1>/dev/null 2>&1";
	lappend cmds "mv restore SAVErestore";
	lappend cmds "mkdir restore";
	lappend cmds "cd SAVErestore";
	lappend cmds "find . -type d -print | cpio -pduvm ../restore";
	lappend cmds "echo '# template rc.local file' >../restore/rc.local";
	lappend cmds "cd /Telica";
	lappend cmds "rm -rf SAVErestore 1>/dev/null 2>&1";
	#
	lappend cmds "mv /.odbc.ini /tmp 1>/dev/null 1>&2";
	lappend cmds "rm -f /tmp/.odbc.ini 1>/dev/null 1>&2";
	lappend cmds "rm -rf /opt/TimesTen* 1>/dev/null 1>&2";
	lappend cmds "echo '# over-written by reload' >/etc/resolv.conf";
	lappend cmds "rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2";
	#
	lappend cmds "mkdir -p /home1/Telica/swCPU/core";
	#
	lappend cmds "rm -rf /lost+found/* 1>/dev/null 1>&2";
	lappend cmds "rm -rf /usr/tmp/vi.recover 1>/dev/null 1>&2";
	lappend cmds "mkdir -p /usr/tmp/vi.recover 1>/dev/null 1>&2";
	#
	lappend cmds "\[\[ -L /localbackup ]] && rm -f /localbackup 1>/dev/null 2>&1";
	lappend cmds "rm -f /Telica/localbackups/* 1>/dev/null 1>&2";
	lappend cmds "rm -f /Telica/localbackups.old/* 1>/dev/null 1>&2";
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
set ask 0;
set query 0;
set logfile "";
set verbose 0;
set unconditional 0;
set labid "";
set LABID "";
set removelock 0;
set initsp "none";
set dotimestamp 0;
set fileowner 0;
set usermapfile "";
set usertarfile "";
set emailaddress "mrumore@alcatel-lucent.com lcstools@lts08.ih.lucent.com";
#
set background 0;
set reloadcpuload "";
set reloadspa 0;
set reloadspb 0;
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
	{^-s} { set dotimestamp 1; }
	{^-K} { set doloadimage 1; set doexpansion 0; }
	{^-X} { set doloadimage 0; set doexpansion 1; }
	{^-D} { set ask 0; }
	{^-A} { set ask 1; }
	{^-a} { set reloadspa 1; }
	{^-b} { set reloadspb 1; }
	{^-Q} { set query 1; }
	{^-V} { set verbose 1; }
	{^-U} { set unconditional 1; }
	{^-R} { set removelock 1; }
	{^-B} { set background 1; set ask 0; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-i.*} { getoptval $argval initsp arg; }
	{^-c.*} { getoptval $argval reloadcpuload arg; }
	{^-m.*} { getoptval $argval usermapfile arg; }
	{^-t.*} { getoptval $argval usertarfile arg; }
	{^-I.*} { 
		getoptval $argval initsp arg; 
		set doloadimage 0; 
		set doexpansion 0;
		set ask 0;
	}
	{^-\?} { usage; exit 0; }
	{^-T} { timeouthelp; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: Unknown option: $argval\n"; usage; exit 2; }
	default { break; }
	}
}
#
# get labid 
#
if {$arg<$argc} {
	set labid [lindex $argv $arg];
} elseif {[info exists env(LABID)]} {
	set labid $env(LABID);
} else {
	puts "\nERROR: LABID is neither set nor given.";
	sendemail "Reload ERROR" "END: LABID is neither set nor given.";
	logmsg reload "END: LABID is neither set nor given.";
	exit 2;
}
puts "\nStarting reload of lab $labid:\n";
#
switch -- $initsp {
"simplex_a" { # ok }
"simplex_b" { # ok }
"duplex" { # ok }
"none" { # ok }
default {
	puts "\nERROR: Invalid '-i' option value: $initsp";
	sendemail "Reload ERROR" "END: Invalid '-i' option value: $initsp";
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
		puts "\nERROR: Logging failed for file $logfile:\n$status";
		sendemail "Reload ERROR" "END: Logging failed for file $logfile:\n$status";
		logmsg reload "END: Logging failed for file $logfile:\n$status";
		exit 2;
	}
	set usingdefaultlogfile 0;
} else {
	# default logging for now
	set mypid [pid];
	set logfile "/lcsl100/logfiles/reload/reload.${labid}.${mypid}";
	puts "\nDefault log file is $logfile.";
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nERROR: Logging failed for file $logfile:\n$status";
		sendemail "Reload ERROR" "END: Logging failed for file $logfile:\n$status";
		logmsg reload "END: Logging failed for file $logfile:\n$status";
		exit 2;
	}
	# if this flag is true and we are using the default log file
	# and we exit ok, then the default log file is deleted. the 
	# assumption being that we have nothing to look at. change this
	# flag to false so we always save the log files. it makes the 
	# flag useless, but that's ok. the log files can always be cleaned 
	# up afterwards.
	#
	# set usingdefaultlogfile 1;
	set usingdefaultlogfile 0;
}
if {$verbose > 0} {
	log_user 1;
}
#
checkenv;
#
# check if a map or tar file were given.
#
if {[string length $usermapfile] > 0} {
	puts "\nMap File Given. Processing $usermapfile ....";
	sendemail "Reload INFO" "RELOAD: Map File Given. Processing $usermapfile ....";
	logmsg reload "RELOAD: Map File Given. Processing $usermapfile ....";
	#
	if {![file readable $usermapfile]} {
		puts "\nERROR: Map file $usermapfile is not readable.";
		sendemail "Reload ERROR" "END: Map file $usermapfile is not readable.";
		logmsg reload "END: Map file $usermapfile is not readable.";
		exit 2;
	}
	if {[catch {set fd [open $usermapfile "r"]} status]} {
		puts "\nERROR: Unable to open map file $usermapfile:\n$status";
		sendemail "Reload ERROR" "END: Unable to open map file $usermapfile:\n$status";
		logmsg reload "END: Unable to open map file $usermapfile:\n$status";
		exit 2;
	}
	while {[gets $fd inbuf] > 0} {
		set inbuf [string trim $inbuf];
		set inbuf [string trimright $inbuf];
		#
		switch -regexp -- "${inbuf}" {
		{^[ \t]*$} {
			# skip empty lines
			continue;
		}
		{^[ \t]*#} {
			# skip commments
			continue;
		}
		default {
			puts "\nProcessing Map File Entry: ${inbuf}";
			set inbufdata [split $inbuf " \t"];
		}
		}
		#
		set fromfile [lindex $inbufdata 0];
		set tofile [lindex $inbufdata 1];
		set mapfiledata($fromfile) "$tofile";
		#
		if {![file readable $fromfile]} {
			puts "\nERROR: Private file $fromfile is not readable.";
			sendemail "Reload ERROR" "END: Private file $fromfile is not readable.";
			logmsg reload "END: Private file $fromfile is not readable.";
			exit 2;
		}
		puts "\nMap Local File: $fromfile";
		puts "To Chassis File: $tofile";
	}
	catch { close $fd; wait; } ignore;
}
if {[string length $usertarfile] > 0} {
	puts "\nTar File Given. Processing $usertarfile ....";
	sendemail "Reload INFO" "RELOAD: Tar File Given. Processing $usertarfile ....";
	logmsg reload "RELOAD: Tar File Given. Processing $usertarfile ....";
	#
	if {![file readable $usertarfile]} {
		puts "\nERROR: Tar file $usertarfile is not readable.";
		sendemail "Reload ERROR" "END: Tar file $usertarfile is not readable.";
		logmsg reload "END: Tar file $usertarfile is not readable.";
		exit 2;
	}
	if {[catch {set fd [open $usertarfile "r"]} status]} {
		puts "\nERROR: Unable to open tar file $usertarfile:\n$status";
		sendemail "Reload ERROR" "END: Unable to open tar file $usertarfile:\n$status";
		logmsg reload "END: Unable to open tar file $usertarfile:\n$status";
		exit 2;
	}
	while {[gets $fd inbuf] > 0} {
		set inbuf [string trim $inbuf];
		set inbuf [string trimright $inbuf];
		#
		switch -regexp -- "${inbuf}" {
		{^[ \t]*$} {
			# skip empty lines
			continue;
		}
		{^[ \t]*#} {
			# skip commments
			continue;
		}
		default {
			# ok
			puts "\nProcessing Private Tar File: ${inbuf}";
			set privtarpath ${inbuf};
			set privtarfile [file tail ${privtarpath}];
			# get type of file
			if {[regexp "^\[a-z0-9A-Z\.]*_(.*).tar.gz$" $privtarfile ignore tarfiletype] != 1} {
				puts "\nERROR: unknown tar file type - $tarfiletype.";
				sendemail "Reload ERROR" "END: ERROR: unknown tar file type - $tarfiletype.";
				logmsg reload "END: ERROR: unknown tar file type - $tarfiletype.";
				exit 2;
			}
			if {[info exists tarfiledata($tarfiletype)]} {
				puts "\nERROR: two tar files of same type given - $tarfiletype";
				sendemail "Reload ERROR" "END: ERROR: two tar files of same type given - $tarfiletype";
				logmsg reload "END: ERROR: two tar files of same type given - $tarfiletype";
				exit 2;
			}
			set tarfiledata($tarfiletype) "$privtarpath";
			puts "\nTar File Type: $tarfiletype";
			puts "Tar File Path: $privtarpath";
		}
		}
		#
		if {![file readable $privtarpath]} {
			puts "\nERROR: Tar file $privtarpath is not readable.";
			sendemail "Reload ERROR" "END: Tar file $privtarpath is not readable.";
			logmsg reload "END: Tar file $privtarpath is not readable.";
			exit 2;
		}
	}
	catch { close $fd; wait; } ignore;
}
# 
initdbcmds dbcmds;
#
# get ip addresses for labid.
#
if {[info exists env(FILEOWNER)] && $env(FILEOWNER) == "yes"} {
	set fileowner 1;
	puts "\nWarning: user is file owner.";
} else {
	set fileowner 0;
	puts "\nWarning: user is not file owner.";
}
#
set status [quietP locked $labid];
if {$status == 0} {
	puts "\nERROR: Cannot reload - lab $labid is locked.";
	sendemail "Reload ERROR" "END: cannot reload - labid $labid is locked.";
	logmsg reload "END: cannot reload - labid $labid is locked.";
	unset LABID;
	exit 2;
}
quietV locked $labid;
#
if {$removelock} {
	puts "WARNING: Forced removal of LOCK file for $labid.";
	sendemail "Reload INFO" "WARNING: Forced removal of LOCK file for $labid.";
	logmsg reload "WARNING: Forced removal of LOCK file for $labid.";
	set status [V reload $labid];
	sendemail "Reload INFO" "WARNING: status of removal of LOCK file for $labid: $status";
	logmsg reload "WARNING: status of removal of LOCK file for $labid: $status";
}
set status [P reload $labid];
if {$status == 0} {
	puts "\nERROR: Another reload of $labid in progress.";
	sendemail "Reload ERROR" "END: another reload of $labid in progress.";
	logmsg reload "END: another reload of $labid in progress.";
	unset LABID;
	exit 2;
}
#
set LABID $labid;
#
if {[dbselect obuf chassis "labid req ^$labid\$" "cpu_a_ip cpu_b_ip" ] != 0} {
	puts "\nERROR: reload: dbselect of relation 'chassis' failed.";
	sendemail "Reload ERROR" "END: reload: dbselect of relation 'chassis' failed.";
	logmsg reload "END: reload: dbselect of relation 'chassis' failed.";
	exit 2;
}
if {![info exists obuf] || [llength obuf] == 0} {
	puts "\nERROR: No IPs found for labid $labid. Unknown labid.";
	sendemail "Reload ERROR" "END: reload: no IPs found for labid $labid. Unknown labid.";
	logmsg reload "END: reload: no IPs found for labid $labid. Unknown labid.";
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
if {$reloadcpuload == ""} {
	do_we_reload $labid "SP-A" cpu_a_ip;
	do_we_reload $labid "SP-B" cpu_b_ip;
} else {
	sendemail "Reload INFO" "reload: command line reload (cpuload,doSPA,doSPB) = ($reloadcpuload,$reloadspa,$reloadspb).";
	logmsg reload "reload: command line reload (cpuload,doSPA,doSPB) = ($reloadcpuload,$reloadspa,$reloadspb).";
	if {$reloadspa == 0} {
		set cpu_a_ip "";
	}
	if {$reloadspb == 0} {
		set cpu_b_ip "";
	}
}
#
if {$cpu_a_ip != "" && !$unconditional && ($doloadimage || $doexpansion)} {
	puts "\nCurrent SP-A Status:\n";
	set spstatus [getspstatus $labid $cpu_a_ip "SP-A"];
	sendemail "Reload INFO" "SP-STATUS: $labid $cpu_a_ip SP-A is $spstatus.";
	logmsg reload "SP-STATUS: $labid $cpu_a_ip SP-A is $spstatus.";
	if {[string match "*active*application*" $spstatus]} {
		puts "\nERROR: Exiting reload because NOT in Min-Mode.";
		sendemail "Reload ERROR" "END: Exiting reload because NOT in Min-Mode.";
		logmsg reload "END: Exiting reload because NOT in Min-Mode.";
		exit 2;
	}
}
if {$cpu_b_ip != "" && !$unconditional && ($doloadimage || $doexpansion)} {
	puts "\nCurrent SP-B Status:\n";
	set spstatus [getspstatus $labid $cpu_b_ip "SP-B"];
	sendemail "Reload INFO" "SP-STATUS: $labid $cpu_b_ip SP-B is $spstatus.";
	logmsg reload "SP-STATUS: $labid $cpu_b_ip SP-B is $spstatus.";
	if {[string match "*active*application*" $spstatus]} {
		puts "\nERROR: Exiting reload because NOT in Min-Mode.";
		sendemail "Reload ERROR" "END: Exiting reload because NOT in Min-Mode.";
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
	sendemail "Reload SUCCESS" "END: nothing chosen to reload.";
	logmsg reload "END: nothing chosen to reload.";
	puts "Nothing to reload.";
	puts "Exiting reload.";
	exit 0;
}
#
getimages2load $labid $reloadcpuload branch cpuload images2load;
if {![info exists images2load] || [llength images2load] == 0} {
	puts "\nERROR: reload: no images chosen for reload. Exiting reload.";
	sendemail "Reload ERROR" "END: reload: no images chosen for reload. Exiting reload.";
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
		sendemail "Reload SUCCESS" "END: Backgrounding. Parent reload exiting.";
		logmsg reload "END: Backgrounding. Parent reload exiting.";
		unset LABID;
		exit 0;
	}
	# let parent exit and update log file, then continue.
	sleep 5;
	sendemail "Reload INFO" "Backgrounding. Child disconnecting and continuing.";
	logmsg reload "Backgrounding. Child disconnecting and continuing.";
	disconnect;
}
#
if {$cpu_a_ip != "" && $cpu_b_ip != ""} {
	sendemail "Reload INFO" "START: reloading $labid SP-A and SP-B with load $branch, $cpuload.";
	logmsg reload "START: reloading $labid SP-A and SP-B with load $branch, $cpuload.";
	sendemail "Reload INFO" "START: PRELOAD $labid SP-A and SP-B.";
	logmsg reload "START: PRELOAD $labid SP-A and SP-B.";
	preload $labid $cpu_a_ip;
	preload $labid $cpu_b_ip;
	if {$doloadimage} {
		sendemail "Reload INFO" "START: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
		logmsg reload "START: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
		loadimage $labid $cpu_a_ip $branch $cpuload images2load;
		loadimage $labid $cpu_b_ip $branch $cpuload images2load;
		sendemail "Reload INFO" "END: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
		logmsg reload "END: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.";
	} else {
		puts "Skipping loadimage stage ...";
	}
	if {$doexpansion} {
		if {$ask} {
			do_we_proceed "removing old load";
		}
		sendemail "Reload INFO" "START: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
		logmsg reload "START: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
		killemf $cpu_a_ip $cpu_b_ip;
		removeoldload $cpu_a_ip images2load;
		removeoldload $cpu_b_ip images2load;
		if {$ask} {
			do_we_proceed "installation";
		}
		installnewload $cpu_a_ip $cpuload images2load;
		loadprivprods $cpu_a_ip;
		installnewload $cpu_b_ip $cpuload images2load;
		loadprivprods $cpu_b_ip;
		what_to_do_next;
		sendemail "Reload INFO" "END: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
		logmsg reload "END: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.";
	} else {
		puts "Skipping expansion stage ...";
	}
	if {$initsp != "none"} {
		sendemail "Reload INFO" "START: INIT of $labid with load $branch, $cpuload.";
		logmsg reload "START: INIT of $labid with load $branch, $cpuload.";
		set postcmd "";
		set rebootcmd "";
		#
		switch -- $initsp {
		"simplex_a" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -A -a $labid";
		}
		"simplex_b" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -B -a $labid";
		}
		"duplex" {
			set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a $labid";
			set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -a $labid";
		}
		}
		#
		puts "\nPOST CMD: $postcmd ";
		sendemail "Reload INFO" "POST CMD: $postcmd ";
		logmsg reload "POST CMD: $postcmd ";
		puts "\nREBOOT CMD: $rebootcmd ";
		sendemail "Reload INFO" "REBOOT CMD: $rebootcmd ";
		logmsg reload "REBOOT CMD: $rebootcmd ";
		#
		if {[catch { system "$postcmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nERROR: postlcsreload: $errmsg";
				sendemail "Reload INFO" "ERROR: postlcsreload: $errmsg";
				logmsg reload "ERROR: postlcsreload: $errmsg";
			} else {
				puts "\nERROR: postlcsreload: unknown error.";
				sendemail "Reload INFO" "ERROR: postlcsreload: unknown error.";
				logmsg reload "ERROR: postlcsreload: unknown error.";
			}
		} elseif {[catch { system "$rebootcmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nERROR: lcsreboot: $errmsg";
				sendemail "Reload ERROR" "ERROR: lcsreboot: $errmsg";
				logmsg reload "ERROR: lcsreboot: $errmsg";
			} else {
				puts "\nERROR: lcsreboot: unknown error.";
				sendemail "Reload ERROR" "ERROR: lcsreboot: unknown error.";
				logmsg reload "ERROR: lcsreboot: unknown error.";
			}
		}
		catch { wait; } ignore;
		sendemail "Reload INFO" "END: INIT of $labid with load $branch, $cpuload.";
		logmsg reload "END: INIT of $labid with load $branch, $cpuload.";
	}
	if {$usingdefaultlogfile != 0} {
		if {[file exists $logfile]} {
			file delete -force -- $logfile;
		}
	}
	sendemail "Reload SUCCESS" "END: reloading $labid SP-A and SP-B with load $branch, $cpuload.";
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
	sendemail "Reload SUCCESS" "END: nothing chosen to reload.";
	logmsg reload "END: nothing chosen to reload.";
	puts "Nothing to reload.";
	puts "Exiting reload.";
	exit 0
}
#
# reload one side only
#
sendemail "Reload INFO" "START: reloading $labid $sp2load with load $branch, $cpuload.";
logmsg reload "START: reloading $labid $sp2load with load $branch, $cpuload.";
#
sendemail "Reload INFO" "START: PRELOAD $labid $sp2load.";
logmsg reload "START: PRELOAD $labid $sp2load.";
#
preload $labid $cpu_ip;
if {$doloadimage} {
	sendemail "Reload INFO" "START: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
	logmsg reload "START: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
	loadimage $labid $cpu_ip $branch $cpuload images2load;
	sendemail "Reload INFO" "END: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
	logmsg reload "END: LOADIMAGES $labid $sp2load with load $branch, $cpuload.";
} else {
	puts "Skipping loadimage stage ...";
}
if {$doexpansion} {
	if {$ask} {
		do_we_proceed "removing old load";
	}
	sendemail "Reload INFO" "START: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
	logmsg reload "START: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
	killemf $cpu_ip "";
	removeoldload $cpu_ip images2load;
	if {$ask} {
		do_we_proceed "installation";
	}
	installnewload $cpu_ip $cpuload images2load;
	loadprivprods $cpu_ip;
	what_to_do_next;
	sendemail "Reload INFO" "END: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
	logmsg reload "END: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.";
} else {
	puts "Skipping expansion stage ...";
}
if {$initsp != "none"} {
	sendemail "Reload INFO" "START: INIT of $labid with load $branch, $cpuload.";
	logmsg reload "START: INIT of $labid with load $branch, $cpuload.";
	set postcmd "";
	set rebootcmd "";
	#
	switch -- $initsp {
	"simplex_a" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -A -a $labid";
	}
	"simplex_b" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a -s $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -B -a $labid";
	}
	"duplex" {
		set postcmd "$env(LCSTOOLSBIN)/postlcsreload -a $labid";
		set rebootcmd "$env(LCSTOOLSBIN)/lcsreboot -U -a $labid";
	}
	}
	#
	puts "\nPOST CMD: $postcmd ";
	sendemail "Reload INFO" "POST CMD: $postcmd ";
	logmsg reload "POST CMD: $postcmd ";
	puts "\nREBOOT CMD: $rebootcmd ";
	sendemail "Reload INFO" "REBOOT CMD: $rebootcmd ";
	logmsg reload "REBOOT CMD: $rebootcmd ";
	#
	if {[catch { system "$postcmd"; } errmsg] != 0} {
		if {[string length $errmsg] > 0} {
			puts "\nERROR: postlcsreload: $errmsg";
			sendemail "Reload ERROR" "ERROR: postlcsreload: $errmsg";
			logmsg reload "ERROR: postlcsreload: $errmsg";
		} else {
			puts "\nERROR: postlcsreload: unknown error.";
			sendemail "Reload ERROR" "ERROR: postlcsreload: unknown error.";
			logmsg reload "ERROR: postlcsreload: unknown error.";
		}
	} elseif {[catch { system "$rebootcmd"; } errmsg] != 0} {
		if {[string length $errmsg] > 0} {
			puts "\nERROR: lcsreboot: $errmsg";
			sendemail "Reload ERROR" "ERROR: lcsreboot: $errmsg";
			logmsg reload "ERROR: lcsreboot: $errmsg";
		} else {
			puts "\nERROR: lcsreboot: unknown error.";
			sendemail "Reload ERROR" "ERROR: lcsreboot: unknown error.";
			logmsg reload "ERROR: lcsreboot: unknown error.";
		}
	}
	catch { wait; } ignore;
	sendemail "Reload INFO" "END: INIT of $labid with load $branch, $cpuload.";
	logmsg reload "END: INIT of $labid with load $branch, $cpuload.";
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
sendemail "Reload SUCCESS" "END: reloading $labid $sp2load with load $branch, $cpuload.";
logmsg reload "END: reloading $labid $sp2load with load $branch, $cpuload.";
puts "Exiting reload.";
#
exit 0;

