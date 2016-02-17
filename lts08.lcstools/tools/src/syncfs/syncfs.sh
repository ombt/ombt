#!/opt/exp/bin/expect
#
# synchronize a file system on chibuild with a file system on lts08.
#
#########################################################################
#
# global data
#
set queue "/tmp/syncfsqueue";
set localmachine "lts08.ih.lucent.com";
set remotemachine "chibuild.telica.com";
set remotelogin "builder";
set remotepasswd "turk74";
set syncflag 1;
set stats(default) 0;
#
# exit handler
#
exit -onexit {
	global syncflag;
	if {${syncflag}} {
		V syncfs;
	}
}
#
# signal handler
#
trap {
	global syncflag;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "Got signal <$signame,$signo>";
	if {${syncflag}} {
		V syncfs;
	}
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# return option value.
#
proc getoptval { argval varname argname } {
	global argv;
	upvar $varname var;
	upvar $argname arg;
	#
	if {[string length $argval] == 2} {
		incr arg;
		set var [lindex $argv $arg];
	} else {
		set var [string range $argval 2 end];
	}
}
#
# check for success or failure
#
proc isOk { { status "-1 - failure" } } {
	if {[string match "0*" ${status}]} {
		return 1;
	} else {
		return 0;
	}
}
#
proc isNotOk { { status "-1 - failure" } } {
	if {[string match "0*" ${status}]} {
		return 0;
	} else {
		return 1;
	}
}
#
# semaphore lock 
#
proc P { branch } {
	# check if queue exists
	global queue;
	set qdir $queue;
	if {![file isdirectory $qdir]} {
		file mkdir $qdir;
	}
	# lock file
	set lockfile "${qdir}/${branch}";
	# check if lock file exists.
	if {[file exists $lockfile]} {
		return 0;
	}
	# create lock file - possible race condition ...
	set lockfd [open $lockfile "w"];
	puts $lockfd "$branch - [pid] - [exec id] - [exec date]";
	close $lockfd;
	#
	if {[file owned $lockfile]} {
		system /usr/bin/chmod 0777 $lockfile;
	}
	# our turn
	return 1;
}
#
# semaphore unlock
#
proc V { branch } {
	# lock file
	global queue;
	set qdir $queue;
	set lockfile "${qdir}/${branch}";
	# check if queue exists
	if {![file isdirectory $qdir]} {
		file mkdir $qdir;
		return 1;
	}
	#
	if {[file owned $lockfile]} {
		system /usr/bin/chmod 0777 $lockfile;
	}
	# check if lock file exists.
	if {[file exists $lockfile]} {
		file delete -force -- $lockfile;
	}
	return 1;
}
#
# telnet functions
#
proc telnet_close { spawn_idname } {
	upvar $spawn_idname spawn_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; wait -nowait; } ignore;
	return "0 - success";
}
#
proc telnet_open { ip login passwd remote_idname { maxlogons 5 } { sleeptime 10 } { logintimeout 30 } } {
	upvar ${remote_idname} remote_id;
	#
	global timeout;
	set timeout $logintimeout;
	#
	set done 0;
	#
	for {set try 0} {$try<$maxlogons} {incr try} {
		spawn -noecho "/usr/bin/telnet" "-l $login" $ip;
		set remote_id $spawn_id;
		#
		expect {
		-re {.*login:} {
			send "$login\r";
			exp_continue;
		}
		-re {.*user name:} {
			send "$login\r";
			exp_continue;
		}
		-re {.*[pP]assword:} {
			send "$passwd\r";
			set done 1;
			break;
		}
		timeout {
			puts "\nTime out during telnet attempt. Retry.";
			telnet_close remote_id;
			sleep $sleeptime;
			continue;
		}
		eof {
			puts "\nEOF during telnet attempt. Retry.";
			telnet_close remote_id;
			sleep $sleeptime;
			continue;
		}
		}
	}
	#
	if {!$done || ($try>=$maxlogons)} {
		return "-1 - telnet_open: unable to telnet to machine ($ip,$login).";
	}
	#
	expect {
	-re ".*> " {
		send "unset PROMPT_COMMAND\r";
	}
	-re ".*# " {
		send "unset PROMPT_COMMAND\r";
	}
	-re ".*\$ " {
		send "unset PROMPT_COMMAND\r";
	}
	-re ".*\$" {
		send "unset PROMPT_COMMAND\r";
	}
	timeout {
		telnet_close remote_id;
		return "-1 - telnet_open: time out during unset ($ip,$login).";
	}
	eof {
		telnet_close remote_id;
		return "-1 - telnet_close: eof during unset ($ip,$login).";
	}
	};
	expect {
	-re ".*> " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re ".*\$ " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re ".*\$" {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		telnet_close remote_id;
		return "-1 - telnet_open: time out during PS1 ($ip,$login).";
	}
	eof {
		telnet_close remote_id;
		return "-1 - telnet_close: eof during PS1 ($ip,$login).";
	}
	};
	#
	expect {
	-re {.*PS1="COMMAND_IS_DONE>"} {
		exp_continue;
	}
	-re {.*COMMAND_IS_DONE>} {
		# ok.
	}
	timeout {
		telnet_close remote_id;
		return "-1 - telnet_open: time out after PS1 ($ip,$login).";
	}
	eof {
		telnet_close remote_id;
		return "-1 - telnet_open: eof after PS1 ($ip,$login).";
	}
	};
	#
	return "0 - success";
}
#
proc telnet_exec { spawn_idname xcmd { xcmdtimeout 30 } } {
	upvar $spawn_idname spawn_id;
	#
	global timeout;
	set timeout $xcmdtimeout;
	#
	send "$xcmd\r";
	expect {
	-re {.*[\r\n]COMMAND_IS_DONE>} {
		send "echo STATUS=$?\r";
	}
	timeout {
		return "-1 - telnet_exec: timeout during cmd:\n$xcmd";
	}
	eof {
		return "-1 - telnet_exec: timeout during cmd:\n$xcmd";
	}
	}
	#
	expect {
	-re {.*STATUS=([-0-9]+)[\r\n]*COMMAND_IS_DONE>} {
		set status $expect_out(1,string);
		if {[isNotOk $status]} {
			return "-1 - telnet_exec: cmd failed with status $status:\n$xcmd";
		}
	}
	timeout {
		return "-1 - telnet_exec: timeout during cmd:\n$xcmd";
	}
	eof {
		return "-1 - telnet_exec: timeout during cmd:\n$xcmd";
	}
	}
	#
	return "0 - success";
}
#
# ftp functions
#
proc ftp_close { ftp_idname } {
	upvar $ftp_idname spawn_id;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; wait -nowait; } ignore;
	return "0 - success";
}
#
proc ftp_open { ftp_idname ip login passwd {tmout 30} { sleeptime 10 } { maxlogons 10 } } {
	#
	upvar $ftp_idname ftp_id;
	#
	global timeout;
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	set done 0;
	for {set try 0} {$try<$maxlogons} {incr try} {
		spawn -noecho "/usr/bin/ftp" $ip;
		set ftp_id $spawn_id;
		#
		expect {
		"Name" {
			send "$login\r";
			set done 1;
			break;
		}
		timeout {
			puts "\nftp_open: time out during ftp ($ip,$login). Retry.";
			ftp_close ftp_id;
			sleep $sleeptime;
			continue;
		}
		eof {
			puts "\nftp_open: eof during ftp ($ip,$login). Retry.";
			ftp_close ftp_id;
			sleep $sleeptime;
			continue;
		}
		}
	}
	#
	if {$try>=$maxlogons || !$done} {
		return "-1 - ftp_open: unable to ftp to ($ip,$login).";
	}
	#
	expect {
	"Password:" {
		send "$passwd\r";
	}
	timeout {
		return "-1 - ftp_open: time out during login ($ip,$login).";
	}
	eof {
		return "-1 - ftp_open: eof during login ($ip,$login).";
	}
	}
	#
	expect {
	-re {230.*ftp> } {
		# success. able to logon.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	timeout {
		return "-1 - ftp_open: time out during passwd ($ip,$login).";
	}
	eof {
		return "-1 - ftp_open: eof during passwd ($ip,$login).";
	}
	}
	#
	return "0 - success";
}
#
proc ftp_get_file { ftp_idname frompath topath {binaryflag 1} {tmout 30} } {
	upvar $ftp_idname spawn_id;
	#
	global timeout;
	#
	if {$binaryflag} {
		send "binary\r";
	} else {
		send "ascii\r";
	}
	set timeout 120;
	expect {
	-re "200.*ftp> " {
		# ok
	}
	timeout {
		return "-1 - ftp_get_file: time out during binary or ascii operation.";
	}
	eof {
		return "-1 - ftp_get_file: eof during binary or ascii operation.";
	}
	}
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	send "get $frompath $topath\r";
	expect {
	-re {226.*ftp> } {
		# success. able to get file.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - ftp_get_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - ftp_get_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		return "-1 - ftp_get_file: time out during get ($frompath,$topath).";
	}
	eof {
		return "-1 - ftp_get_file: eof during get ($frompath,$topath).";
	}
	}
	#
	if {![file exists $topath]} {
		return "-1 - ftp_get_file: to-file $topath does not exist.";
	}
	#
	return "0 - success";
}
#
proc ftp_put_file { ftp_idname frompath topath {binaryflag 1} {tmout 30} } {
	upvar $ftp_idname spawn_id;
	#
	global timeout;
	#
	if {$binaryflag} {
		send "binary\r";
	} else {
		send "ascii\r";
	}
	set timeout 120;
	expect {
	-re "200.*ftp> " {
		# ok
	}
	timeout {
		return "-1 - ftp_put_file: time out during binary or ascii operation.";
	}
	eof {
		return "-1 - ftp_put_file: eof during binary or ascii operation.";
	}
	}
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	send "put $frompath $topath\r";
	expect {
	-re {226.*ftp> } {
		# success. able to put file.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - ftp_put_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - ftp_put_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		return "-1 - ftp_put_file: time out during put ($frompath,$topath).";
	}
	eof {
		return "-1 - ftp_put_file: eof during put ($frompath,$topath).";
	}
	}
	#
	return "0 - success";
}
#
# synchronize to file systems
#
proc syncfs { remotemachine remotelogin remotepasswd remotepath localpath { unconditional 0 } } {
	#
	# open telnet session to remote machine and get filesystem data.
	#
	puts "\nOpening TELNET session to remote machine -- ${remotemachine}.";
	set status [telnet_open $remotemachine $remotelogin $remotepasswd remote_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_open failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nChecking remote path exists -- ${remotepath}.";
	set status [telnet_exec remote_id "\[ -d \"${remotepath}\" ]" 120];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_exec (-d remotepath) failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nExecuting 'find' on remote machine path -- ${remotepath}.";
	set remoteoutfile "/tmp/remotefsdata.[pid]";
	set status [telnet_exec remote_id "find ${remotepath}  -type f -print -exec sum '{}' \\; | paste - - | sed -n 's/	/ /g; s/  */ /gp;' | cut -d' ' -f1,2 1>${remoteoutfile} 2>&1" 300];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_exec (find) failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nOpening FTP stream to remote machine -- ${remotemachine}.";
	set status [ftp_open ftp_id ${remotemachine} ${remotelogin} ${remotepasswd}];
	if {[isNotOk $status]} {
		return "-1 - syncfs: ftp_open failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nGetting remote machine 'find' output machine -- ${remoteoutfile}.";
	set status [ftp_get_file ftp_id ${remoteoutfile} ${remoteoutfile} 1 300 ];
	if {[isNotOk $status]} {
		return "-1 - syncfs: ftp_get_file failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nClosing FTP stream to remote machine -- ${remotemachine}.";
	set status [ftp_close ftp_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: ftp_close failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nRemoving remote output file -- ${remoteoutfile}.";
	set status [telnet_exec remote_id "/bin/rm -f ${remoteoutfile} 1>/dev/null 2>&1"];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_exec (rm) failed for ${remotemachine}:\n${status}";
	}
	#
	puts "\nClosing TELNET session to remote machine -- ${remotemachine}.";
	set status [telnet_close remote_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_close failed for ${remotemachine}:\n${status}";
	}
	# read in remote data file
	if {[catch {set fd [open ${remoteoutfile} "r"]} status]} {
		return "-1 - syncfs: unable to open ${remoteoutfile} for read: $status";
	}
	#
	puts "\nReading data from remote output file.";
	for { } {[gets $fd inbuf] >= 0 && ![eof $fd]} { } {
		# get data from remote file
		set data [split [string trim ${inbuf}] " \t"]
		set filename [lindex $data 0]
		set filesum [lindex $data 1]
		#
		set filename [string trim "${filename}"];
		regsub -- "${remotepath}" "${filename}" "" newfilename;
		#
		set remotefiles(${newfilename}) ${filesum};
	}
	catch { close $fd; } ignore;
	#
	# get data for local filesystem
	#
	puts "\nExecuting 'find' on local machine path -- ${localpath}.";
	set localoutfile "/tmp/localfsdata.[pid]";
	system "find ${localpath} -type f -exec sum -r '{}' \\; |  sed -n 's/	/ /gp; s/  */ /gp;' | cut -d' ' -f1,3 >${localoutfile}";
	# 
	if {[catch {set fd [open ${localoutfile} "r"]} status]} {
		return "-1 - syncfs: unable to open ${localoutfile} for read: $status";
	}
	#
	puts "\nReading data from local output file.";
	for { } {[gets $fd inbuf] >= 0 && ![eof $fd]} { } {
		# get data from local file
		set data [split [string trim ${inbuf}] " \t"]
		set filesum [lindex $data 0]
		set filename [lindex $data 1]
		#
		set filename [string trim "${filename}"];
		regsub -- "${localpath}" "${filename}" "" newfilename;
		#
		set localfiles(${newfilename}) ${filesum};
	}
	catch { close $fd; } ignore;
	#
	# compare files in each file system.
	#
	puts "\nOpening TELNET session to remote machine -- ${remotemachine}.";
	set status [telnet_open $remotemachine $remotelogin $remotepasswd remote_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_open failed for ${remotemachine}:\n${status}";
	}
	puts "\nOpening FTP stream to remote machine -- ${remotemachine}.";
	set status [ftp_open ftp_id ${remotemachine} ${remotelogin} ${remotepasswd}];
	if {[isNotOk $status]} {
		telnet_close remote_id;
		return "-1 - syncfs: ftp_open failed for ${remotemachine}:\n${status}";
	}
	#
	global stats;
	set stats(total) 0;
	set stats(copied) 0;
	set stats(notcopied) 0;
	#
	puts "\nCompare Hash Sums for all files:";
	foreach localfile [array names localfiles] {
		incr stats(total);
		if {!${unconditional}} {
			if {[info exists remotefiles(${localfile})]} {
				puts "\nFile ${localfile} exists on remote machine.";
				puts "\nCompare local and remote file sums.";
				#
				set localfilesum $localfiles(${localfile});
				set remotefilesum $remotefiles(${localfile});
				#
				if {${localfilesum} == ${remotefilesum}} {
					incr stats(notcopied);
					puts "\nFiles have the same sums.";
					puts "\nSkipping it.";
					continue;
				} else {
					puts "\nFiles have different sums.";
					puts "\nCopy local file to remote file.";
				}
			} else {
				puts "\nFile ${localfile} does NOT exist on remote machine.";
				puts "\nCopy local file to remote file.";
			}
		} else {
			puts "\nUnconditional copy of local file ${localfile}.";
			puts "\nCopy local file to remote file.";
		}
		# copy file to remote machine
		set fulllocalpath "${localpath}/${localfile}";
		set fullremotepath "${remotepath}/${localfile}";
		set fullremotedir [file dirname "${fullremotepath}"];
		#
		set status [telnet_exec remote_id "\[ -d \"${fullremotedir}\" ] || mkdir -p \"${fullremotedir}\";"];
		if {[isNotOk $status]} {
			return "-1 - syncfs: telnet_exec (remote mkdir -p) failed for ${remotemachine}:\n${status}";
		}
		#
		set status [ftp_put_file ftp_id ${fulllocalpath} ${fullremotepath} 1 300 ];
		if {[isNotOk $status]} {
			return "-1 - syncfs: ftp_put_file failed for ${remotemachine}:\n${status}";
		}
		incr stats(copied);
	}
	#
	set status [ftp_close ftp_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: ftp_close failed for ${remotemachine}:\n${status}";
	}
	set status [telnet_close remote_id];
	if {[isNotOk $status]} {
		return "-1 - syncfs: telnet_close failed for ${remotemachine}:\n${status}";
	}
	#
	return "0 - success";
}
#
# help message
#
proc usage { } {
	global argv0;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level]";
	puts "		\[-U] local_fullpath remote_fullpath";
	puts "\nwhere:";
	puts "	-? - print usage message.";
	puts "	-x - enable TCL debugger.";
	puts "	-V - verbose mode.";
	puts "	-S tracelevel - set trace to given stack level.";
	puts "	-U - unconditionally copy ALL files.";
	puts "\nwhere local fullpath is the base directory to the ";
	puts "file system which is copied to the remote machine ";
	puts "based at remote fullpath. Files are copied from lts08";
	puts "to chibuild. Files can be overwritten on chibuild,";
	puts "but never deleted. If the same file exists on both";
	puts "machines, then the file is overwritten if the sum ";
	puts "checks are different. The -U option forces all remote";
	puts "files to be overwritten.";
}
#
proc printstats { } {
	global stats
	#
	puts "\n=============================================";
	puts "\nSync FS Data:";
	puts "Total Files     : $stats(total)";
	puts "Copied Files    : $stats(copied)";
	puts "Not Copied Files: $stats(notcopied)";
	#
	return "0 - success";
}
#
#########################################################################
#
# default values
#
puts "\nStarting LCS SYNCH FS:";
#
set unconditional 0;
set verbose 0;
set stracelevel -1;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { set verbose 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-U} { set unconditional 1; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: unknown option - $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
if {$verbose > 0} {
	puts "\nLogging Enabled ...";
	log_user 1;
} else {
	log_user 0;
}
#
if {[expr $arg + 2] != $argc} {
	puts "\nERROR: two directories are required.";
	usage;
	exit 2;
}
#
set localpath [lindex $argv $arg]; 
incr arg;
set remotepath [lindex $argv $arg];
#
if {![regexp -- {^/.*$} "${localpath}"]} {
	puts "\nERROR: 'Local' path is not a full path (needs to start with a '/').";
	exit 2;
} elseif {![regexp -- {^/.*$} "${remotepath}"]} {
	puts "\nERROR: 'Remote' path is not a full path (needs to start with a '/').";
	exit 2;
} elseif {![file isdirectory "${localpath}"]} {
	puts "\nERROR: 'Local' path is NOT a directory -- ${localpath}.";
	exit 2;
}
# 
# show options
#
puts "\nSynchronizing Remote FS with Local FS (copy from local to remote)";
puts "\nLocal Machine  : ${localmachine}";
puts "Local FS Path  : ${localpath}";
puts "\nRemote Machine : ${remotemachine}";
puts "Remote FS Path : ${remotepath}";
puts "\nUnconditional  : ${unconditional}";
#
set status [P syncfs]];
if {$status == 0} {
	puts "\nERROR: another syncfs in progress.";
	set syncflag 0;
	exit 2;
}
#
set status [syncfs ${remotemachine} ${remotelogin} ${remotepasswd} ${remotepath} ${localpath} ${unconditional}];
if {[isNotOk ${status}]} {
	puts "ERROR: syncfs failed:\n${status}";
	exit 2;
}
#
printstats;
puts "\nDone.";
#
exit 0;


