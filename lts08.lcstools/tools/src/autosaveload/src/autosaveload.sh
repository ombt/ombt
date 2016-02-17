#!/opt/exp/bin/expect
#
# save a load from a build machine to the local repository.
#
#########################################################################
#
# global data
#
set branch "UNKNOWN";
set queue "/autosaveload/queue";
set testqueue "Autosaveload/queue";
set load2images "/home2/tester/bin/lcstools/tools/bin/load2images";
set requiredfiles(default) 1;
#
set stats(default) "";
#
# not used for now
#
set numbertosave(cpu) 20;
set numbertosave(cm) 20;
set numbertosave(default) 10;
#
# exit handler
#
exit -onexit {
	global branch;
	if {[info exists branch] && [llength branch] > 0} {
		V $branch;
	}
}
#
# signal handler
#
trap {
	global branch;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "Got signal <$signame,$signo>";
	if {[info exists branch] && [llength branch] > 0} {
		V $branch;
		unset branch;
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
# run load2images remotely and results.
#
proc load2images { remotemachine username userpasswd branch filetype imagedataname } {
	upvar ${imagedataname} imagedata;
	global load2images;
	# determine type of branch
	switch -regexp -- ${branch} {
	{^BP-.*$} {
		set branchtype "beta";
	}
	{BRANCH-.*$} {
		set branchtype "dev";
	}
	{^Main$} {
		set branchtype "dev";
	}
	default {
		return "-1 - load2images: unknown branch type for ${branch} - neither dev or beta.";
	}
	}
	# run load2images remotely and get results
	set status [telnet_open $remotemachine $username $userpasswd remote_id];
	if {[isNotOk $status]} {
		return "-1 - load2images: telnet_open failed for ${remotemachine}:\n${status}";
	}
	set outfile "/tmp/load2images.${branch}.[pid]";
	set status [telnet_exec remote_id "${load2images} ${branch} ${branchtype} ${filetype} 1>${outfile} 2>&1" 300];
	if {[isNotOk $status]} {
		return "-1 - load2images: telnet_exec (load2images) failed for ${remotemachine}:\n${status}";
	}
	#
	set status [ftp_open ftp_id ${remotemachine} ${username} ${userpasswd}];
	if {[isNotOk $status]} {
		return "-1 - load2images: ftp_open failed for ${remotemachine}:\n${status}";
	}
	set status [ftp_get_file ftp_id ${outfile} ${outfile} 1 300 ];
	if {[isNotOk $status]} {
		return "-1 - load2images: ftp_get_file failed for ${remotemachine}:\n${status}";
	}
	set status [ftp_close ftp_id];
	if {[isNotOk $status]} {
		return "-1 - load2images: ftp_close failed for ${remotemachine}:\n${status}";
	}
	#
	set status [telnet_exec remote_id "/bin/rm -f ${outfile} 1>/dev/null 2>&1"];
	if {[isNotOk $status]} {
		return "-1 - load2images: telnet_exec (rm) failed for ${remotemachine}:\n${status}";
	}
	set status [telnet_close remote_id];
	if {[isNotOk $status]} {
		return "-1 - load2images: telnet_close failed for ${remotemachine}:\n${status}";
	}
	#
	# open file for read.
	#
	if {[catch {set fd [open ${outfile} "r"]} status]} {
		return "-1 - load2images: unable to open $outfile for read: $status";
	}
	#
	# read in file and populate data structure
	#
	puts "\nReading data from load2image output file:";
	for { } {[gets $fd inbuf] >= 0 && ![eof $fd]} { } {
		# get data from load2images file
		set data [split $inbuf " \t"]
		set imagetype [lindex $data 0]
		set image [lindex $data 1]
		set imagesize [lindex $data 2]
		#
		puts "\nImage Type: ${imagetype}";
		puts "Image     : ${image}";
		puts "Image Size: ${imagesize}";
		#
		if {[regexp -- {NONE} ${image}]} {
			puts "\nNo images found for type ${imagetype}. Skipping it.";
			continue;
		}
		lappend imagedata(${imagetype}) ${image}
		lappend imagedata(${imagetype}size) ${imagesize}
	}
	catch { close $fd; } ignore;
	#
	return "0 - success";
}
#
# update the local repository with the listed files.
#
proc updaterepository { remotemachine username userpasswd branch basedirectory howmany threshold unconditional imagedataname} {
	upvar ${imagedataname} imagedata;
	global requiredfiles;
	#
	# open ftp stream
	#
	set status [ftp_open ftp_id ${remotemachine} ${username} ${userpasswd}];
	if {[isNotOk $status]} {
		return "-1 - updaterepositpory: ftp_open failed for ${remotemachine}:\n${status}";
	}
	#
	global stats;
	lappend stats(branches) ${branch};
	#
	set stats(${branch},totalfiles) 0;
	set stats(${branch},copiedfiles) 0;
	set stats(${branch},deletedfiles) 0;
	#
	puts "\nUpdating repository for ${branch}:";
	foreach imagetype [array names imagedata] {
		if {[regexp -- {.*size$} ${imagetype}]} {
			# skip image size lists
			# puts "\nSkipping SIZE list ... ${imagetype}";
			continue;
		}	
		#
		puts "\nImage type is: ${imagetype}";
		#
		set imagelist $imagedata(${imagetype});
		set imagesizelist $imagedata(${imagetype}size);
		#
		# get the required files.
		#
		if {$howmany <= 0 } {
			if {[info exists numbertosave(${imagetype})]} {
				set realhowmany $numbertosave(${imagetype});
			} else {
				set realhowmany $numbertosave(default);
			}
		} else {
			set realhowmany ${howmany};
		}
		puts "\nNumber to save for ${imagetype}: ${realhowmany}";
		#
		set imglen [llength ${imagelist}];
		for {set imgidx 0 } {${imgidx} < ${imglen} && ${imgidx} < ${realhowmany}} {incr imgidx} {
			set image [lindex ${imagelist} ${imgidx}];
			set imagesize [lindex ${imagesizelist} ${imgidx}];
			set localimage ${basedirectory}/${image};
			#
			puts "";
			puts "Remote Image ${imgidx}: ${image}";
			puts "Size ${imgidx}        : ${imagesize}";
			puts "Local Image ${imgidx} : ${localimage}";
			#
			incr stats(${branch},totalfiles);
			#
			set localimagepath [file dirname ${localimage}];
			if {![file isdirectory ${localimagepath}]} {
				puts "\nMaking directory ${localimagepath}:";
				file mkdir ${localimagepath};
			}
			#
			if {![info exists requiredfiles(${localimage})]} {
				set requiredfiles(${localimage}) "new";
			}
			#
			if {[file exists ${localimage}]} {
				set localsize [file size ${localimage}];
				if {!$unconditional && \
				    $localsize == $imagesize} {
					puts "\nFiles are the same the size. Skipping update.";
					continue;
				}
				# delete local files
				puts "\nDeleting existing file ${localimage}:";
				file delete -force -- ${localimage};
				incr stats(${branch},deletedfiles);
			}
			# get file
			puts "\nCopying files:";
			puts "Remote file: ${image}";
			puts "Local file : ${localimage}";
			#
			log_user 1;
			set status [ftp_get_file ftp_id ${image} ${localimage} 1 900 ];
			if {[isNotOk $status]} {
				return "-1 - updaterepository: ftp_get_file failed for ${remotemachine}:\n${status}";
			}
			incr stats(${branch},copiedfiles);
			log_user 0;
		}
		#
		# we have updated or copied the required files. any other
		# files in the list that exist on this machine are old
		# or no longer needed. finish going thru the list and
		# delete any file that exists and is not required.
		#
		puts "\nRemoving OLD ${imagetype} files:";
		for { } {${imgidx} < ${imglen}} {incr imgidx} {
			set image [lindex ${imagelist} ${imgidx}];
			set imagesize [lindex ${imagesizelist} ${imgidx}];
			set localimage ${basedirectory}/${image};
			#
			puts "";
			puts "OLD Remote Image ${imgidx}: ${image}";
			puts "OLD Local Image ${imgidx} : ${localimage}";
			#
			if {![file exists ${localimage}]} {
				# file does not exist. skip it.
				continue;
			}
			if {[info exists requiredfiles(${localimage})]} {
				# required file, do not delete.
				puts "\nRequired OLD file ${localimage} -- will NOT delete.";
				continue;
			}
			# delete local files
			puts "\nNOT required OLD file ${localimage} -- will delete.";
			file delete -force -- ${localimage};
			incr stats(${branch},deletedfiles);
		}
	}
	# close ftp stream
	ftp_close ftp_id;
	#
	return "0 - success";
}
#
# help message
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-T] \[-S trace level]";
	puts "	\[-b base directory] \[-h threshold] \[-n howmany] \[-U]";
	puts "	\[-R remote machine] \[-u username] \[-p passwd] ";
	puts "	\[-r required file]\[-t type] branch \[branch2 ...] ]";
	puts "\nwhere:";
	puts "	-? - print usage message.";
	puts "	-x - enable TCL debugger.";
	puts "	-V - verbose mode.";
	puts "	-T - test mode.";
	puts "	-S tracelevel - set trace to given stack level.";
	puts "	-b base directory - path to repository base.";
	puts "	-h threshold - upper percentage usage limit for repository.";
	puts "	-n howmany - how many files to keep for each file type.";
	puts "	-U - unconditionally update the repository.";
	puts "	-R machine - remote build machine containing release files.";
	puts "	-u - remote user name.";
	puts "	-p - remote user passwd.";
	puts "	-r filename - file containing a list of required files.";
	puts "	-t type - type of file to update.";
	puts "\nbranch(es) to copy from remote machine to current machine.";
	puts "\nallowed file types are:\n";
	puts "	all ana atmds3 cm cpu ds1_2 ds1_c ds1 ds3 e1";
	puts "	ena2 ena octds3_2 octds3_3 octds3 pna tdmoc";
	puts "	trids3_3 trids3_d trids3 voip6 voip vs2 vs3";
	puts "	bts tca";
}
#
proc printstats { { branchestoprint "all" } } {
	global stats
	#
	if {${branchestoprint} == "all"} {
		set branches $stats(branches);
	} else {
		set branches ${branchestoprint};
	}
	#
	set totalfiles 0;
	set copiedfiles 0;
	set deletedfiles 0;
	#
	puts "\n=============================================";
	puts "\nAuto Saveload Statistics:"
	foreach branch ${branches} {
		puts "\nBranch: ${branch}";
		puts "Total Files  : $stats(${branch},totalfiles)";
		puts "Copied Files : $stats(${branch},copiedfiles)";
		puts "Deleted Files: $stats(${branch},deletedfiles)";
		#
		incr totalfiles $stats(${branch},totalfiles);
		incr copiedfiles $stats(${branch},copiedfiles);
		incr deletedfiles $stats(${branch},deletedfiles);
	}
	if {${branchestoprint} == "all"} {
		puts "\n=============================================";
		puts "\nAll Branches:";
		puts "Total Files  : $totalfiles";
		puts "Copied Files : $copiedfiles";
		puts "Deleted Files: $deletedfiles";
	}
}
#
# write/read contents of required file, if any.
#
proc readrequiredfile { { filename "" } } {
	global requiredfiles;
	# check if file exists or was given
	if {[string length ${filename}] <= 0} {
		# nothing to read
		return "0 - success";
	}
	if {![file exists ${filename}]} {
		# file does not exist; just return.
		return "0 - success";
	}
	# read in data
	if {[catch {set fd [open ${filename} "r"]} status]} {
		return "-1 - readrequiredfile: unable to open $filename for read: $status";
	}
	#
	puts "\nReading data from required file: ${filename}";
	for { } {[gets $fd inbuf] >= 0 && ![eof $fd]} { } {
		set requiredfiles([string trim "${inbuf}"]) "old";
	}
	catch { close $fd; } ignore;
	# done
	return "0 - success";
}
#
proc writerequiredfile { { filename "" } } {
	global requiredfiles;
	# check if file exists or was given
	if {[string length ${filename}] <= 0} {
		# nothing to write into
		return "0 - success";
	}
	if {[file exists ${filename}]} {
		# append to existing file.
		set writeoper "a";
	} else {
		# create file for write
		set writeoper "w";
	}
	# open file for write
	if {[catch {set fd [open ${filename} "${writeoper}"]} status]} {
		return "-1 - writerequiredfile: unable to open $filename for write: $status";
	}
	#
	puts "\nWriting required files to file: ${filename}";
	foreach requiredfile [array names requiredfiles] {
		if {$requiredfiles(${requiredfile}) == "new"} {
			puts $fd ${requiredfile};
		}
	}
	catch { close $fd; } ignore;
	# done
	return "0 - success";
}
#
#########################################################################
#
# default values
#
puts "\nStarting AUTOSAVELOAD:";
#
set username "lcstools";
set userpasswd "plexus9000";
set remotemachine "chibuild.telica.com";
set unconditional 0;
set verbose 0;
set stracelevel -1;
set basedirectory ".";
set threshold 90.0;
set howmany 5;
set filetype "all";
set requiredfile "";
#
global env;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { set verbose 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-b.*} { getoptval $argval basedirectory arg; }
	{^-h.*} { getoptval $argval threshold arg; }
	{^-n.*} { getoptval $argval howmany arg; }
	{^-U} { set unconditional 1; }
	{^-T} { set queue ${testqueue}; }
	{^-R.*} { getoptval $argval remotemachine arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-t.*} { getoptval $argval filetype arg; }
	{^-r.*} { getoptval $argval requiredfile arg; }
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
# check if branch(es) were given.
#
if {$arg >= $argc} {
	puts "\nERROR: no branches given.";
	usage;
	exit 2;
}
#
# resolve base directory
#
set oldcwd [pwd];
cd ${basedirectory};
set basedirectory [pwd];
cd ${oldcwd};
#
# show options
#
puts "";
puts "Base Directory: ${basedirectory}";
puts "Threshold     : ${threshold}%";
puts "Files-to-Copy : ${howmany}";
puts "File Type     : ${filetype}";
puts "Remote Machine: ${remotemachine}";
puts "User Name     : ${username}";
puts "User Passwd   : ${userpasswd}";
puts "Unconditional : ${unconditional}";
puts "Required File : ${requiredfile}";
#
readrequiredfile $requiredfile;
#
# cycle over branches and copy files over.
#
for { } {$arg<$argc} {incr arg} {
	# next branch to process
	set branch [lindex $argv $arg];
	puts "\nBegin processing for branch ${branch}:";
	# check if a saveload is in progress for this branch.
	set status [P ${branch}];
	if {$status == 0} {
		puts "\nERROR: another ${branch} saveload in progress.";
		unset branch;
		exit 2;
	}
	# run remote load2images to get list of files for this branch
	puts "\nRunning remote load2images for branch ${branch}:";
	set status [load2images ${remotemachine} ${username} ${userpasswd} ${branch} ${filetype} imagedata];
	if {[isNotOk ${status}]} {
		puts "ERROR: load2images failed:\n${status}";
		exit 2;
	}
	#
	puts "\nUpdating repository for branch ${branch}:";
	set status [updaterepository ${remotemachine} ${username} ${userpasswd} ${branch} ${basedirectory} ${howmany} ${threshold} ${unconditional} imagedata];
	if {[isNotOk ${status}]} {
		puts "ERROR: updaterepository failed:\n${status}";
		exit 2;
	}
	# remove all data from old branch.
	unset imagedata;
	# remove lock
	set status [V ${branch}];
	# print stats
	printstats ${branch};
}
#
printstats all;
#
writerequiredfile ${requiredfile};
#
exit 0;
