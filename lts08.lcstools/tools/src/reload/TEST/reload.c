"source $env(LCSTOOLSLIB)/checkenv",
"source $env(LCSTOOLSLIB)/getoptval",
"source $env(LCSTOOLSLIB)/db",
"source $env(LCSTOOLSLIB)/chooseone",
"source $env(LCSTOOLSLIB)/lcsftp",
"source $env(LCSTOOLSLIB)/logging",
"source $env(LCSTOOLSLIB)/lock",
"exit -onexit {\n\
	global LABID;\n\
	if {[info exists LABID] && [llength LABID] > 0} {\n\
		V reload $LABID;\n\
	}\n\
}",
"trap {\n\
	global LABID;\n\
	puts \"\";\n\
	set signame [trap -name];\n\
	set signo [trap -number];\n\
	puts \"Got signal <$signame,$signo)\";\n\
	if {[info exists LABID] && [llength LABID] > 0} {\n\
		V reload $LABID;\n\
		unset LABID;\n\
	}\n\
	exit 2;\n\
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};",
"proc eofexit { status } {\n\
	global LABID;\n\
	if {[info exists LABID] && [llength LABID] > 0} {\n\
		quietV reload $LABID;\n\
		unset LABID;\n\
	}\n\
	exit $status;\n\
}",
"proc what_to_do_next { } {\n\
	puts \"\";\n\
	puts \"Suggested steps after a successful reload:\";\n\
	puts \"\";\n\
	puts \"1) For simplex labs, run the following on the LSP:\";\n\
	puts \"	$ postlcsreload -s -D -h -c 10 -m\";\n\
	puts \"or, for duplex labs, run:\";\n\
	puts \"	$ postlcsreload -d -D -h -c 10 -m\";\n\
	puts \"\";\n\
	puts \"2) For simplex labs, reboot EITHER SP-A or SP-B, NEVER BOTH SPs:\";\n\
	puts \" $ lcsreboot -A	# for SP-A init, do NOT boot SP-B\";\n\
	puts \"or\";\n\
	puts \" $ lcsreboot -B	# for SP-B init, do NOT boot SP-A\";\n\
	puts \"For duplex labs, just enter:\";\n\
	puts \" $ lcsreboot	# init's both SPs\";\n\
	puts \"\";\n\
	puts \"3) For duplex labs, wait for the SPs to be IS,STBYH and IS,ACT.\";\n\
	puts \"and for simplex labs, for the one SP to be IS,NBK, then you can\";\n\
	puts \"continue on to provisioning.\";\n\
}",
"proc usage { } {\n\
	global argv0;\n\
	global username;\n\
	global userpasswd;\n\
\n\
	puts \"usage: [file tail $argv0] \\[-\\?] \\[-x] \\[-B] \\[-V] \\[-K|-X|-H] \\[-R] \\[-D] \\[-T]\";\n\
	puts \"\\[-S trace level] \\[-l logfile] \\[-u username] \\[-p passwd] \\[-Q]\";\n\
	puts \"\\[ -\\[i|I] simplex_a | -\\[i|I] simplex_b | -\\[i|I] duplex ] \\[labid] \";\n\
	puts \"\";\n\
	puts \"where:\";\n\
	puts \"	-x - enable TCL debugger\";\n\
	puts \"	-S tracelevel - set trace to given stack level \";\n\
	puts \"	-K - do loadimage only; skip expansion.\";\n\
	puts \"	-X - do expansion only; skip loadimage\";\n\
	puts \"	-H - generate hardware Tl1 scripts (disables -K,-X)\";\n\
	puts \"	-D - do NOT ask before proceeding with expansion\";\n\
	puts \"	-l logfile - log all output to a logfile\";\n\
	puts \"	-V - turn on verbose\";\n\
	puts \"	-T - help text for recovering from time outs.\";\n\
	puts \"	-Q - ask which IOM/CPU image to reload.\";\n\
	puts \"	-R - remove lock file. see below before using this option.\";\n\
	puts \"	-B - background reload.\";\n\
	puts \"\";\n\
	puts \"	default username = $username\";\n\
	puts \"	default passwd = $userpasswd\";\n\
	puts \"	labid is read from variable LABID, unless given.\";\n\
	puts \"\";\n\
	puts \"	by default, all IOMs/CPU are reloaded. if you want to be\";\n\
	puts \"	asked which IOMs/CPU to reload, use the -Q option.\";\n\
	puts \"\";\n\
	puts \"	You can automatically reboot the switch after a reload by \";\n\
	puts \"	using the -i option. The choices are:\";\n\
	puts \"\";\n\
	puts \"	-i simplex_a - reload, split lab, boot SP-A active\";\n\
	puts \"	-i simplex_b - reload, split lab, boot SP-B active\";\n\
	puts \"	-i duplex - reload, duplex lab, boot SP-A and SP-B active\";\n\
	puts \"\";\n\
	puts \"	-I simplex_a - skip reload, split lab, boot SP-A active\";\n\
	puts \"	-I simplex_b - skip reload, split lab, boot SP-B active\";\n\
	puts \"	-I duplex - skip reload, duplex lab, boot SP-A and SP-B active\";\n\
	puts \"\";\n\
	puts \"	If you are absolutely sure that NO ONE else is running a\";\n\
	puts \"	a reload and you are blocked by the lock file, then you can\";\n\
	puts \"	run reload with the -R option to remove the lock. Remember,\";\n\
	puts \"	if you remove the lock and there really is another reload\";\n\
	puts \"	in progress for the same lab, each reload session will trash\";\n\
	puts \"	the other. So, don't remove the lock, unless you are sure.\";\n\
	puts \"\";\n\
}",
"proc timeouthelp { } {\n\
	global argv0;\n\
	global username;\n\
	global userpasswd;\n\
\n\
	puts \"\";\n\
	puts \"A timeout has occurred while waiting for a response. The\";\n\
	puts \"expected response was either from ftp or telnet. In\";\n\
	puts \"either case, you should try to login using pxmonitor and\";\n\
	puts \"check the sanity of the SP and IP connections. First, verify\";\n\
	puts \"that you can logon the SP. If you can, then run:\";\n\
	puts \"\";\n\
	puts \"ifconfig mgt0\";\n\
	puts \"\";\n\
	puts \"If the IP comes back as 0.0.0.0, then you have to configure \";\n\
	puts \"the mgt0 interface; enter:\";\n\
	puts \"\";\n\
	puts \"/bin/iprecover\";\n\
	puts \"\";\n\
	puts \"in the same window. A second possible problem is when the SP\";\n\
	puts \"drops to the PROM or debug mode. The prompt on the pxmonitor\";\n\
	puts \"window will be 'ibmXXXX', 'Command\\?' or '\\*'. For this case,\";\n\
	puts \"refer to here: \";\n\
	puts \"\";\n\
	puts \"http://ihgpweb.ih.lucent.com/~rumore/LCS/booting-from-prom-mode-or-other\";\n\
	puts \"\";\n\
	puts \"For more information on recovering the IP connections, refer to\";\n\
	puts \"here:\";\n\
	puts \"\";\n\
	puts \"http://ihgpweb.ih.lucent.com/~rumore/LCS/plexus_ip_recovery\";\n\
	puts \"\";\n\
	puts \"If you are unable to get a pxmonitor window, then run 'rmstat'\";\n\
	puts \"and check if another user owns the lynx-a or lynx-b target. If\";\n\
	puts \"this is true, then release the target using 'pxreset -t lynx-a'\";\n\
	puts \"or 'pxreset -t lynx-b'. If no user owns the lynx targets and you\";\n\
	puts \"cannot get a 'username' prompt, then have lab support reseat the\";\n\
	puts \"SP board. Remember, reseating the board is the LAST choice.\";\n\
	puts \"\";\n\
	puts \"If you are able access the px target, but are unable to logon,\";\n\
	puts \"then you can try dropping into the SP debugger by entering:\";\n\
	puts \"CONTROL-SHIFT-underscore. If you receive the \\\"*\\\" prompt, then\";\n\
	puts \"enter \\\"R\\\" and \\\"y\\\". If this fails, reseat the board.\" ;\n\
	puts \"\";\n\
	# puts \"If all else fails, call me at x34396.\";\n\
}",
"proc lcsping { ip } {\n\
	global timeout;\n\
	set status dead;\n\
	set savetimeout $timeout;\n\
	#\n\
	set timeout 3;\n\
	spawn ping $ip;\n\
	#\n\
	expect {\n\
	\"alive\" { set status alive; }\n\
	timeout { set status dead; }\n\
	}\n\
	#\n\
	catch { close; wait; } ignore;\n\
	#\n\
	set timeout $savetimeout;\n\
	#\n\
	return $status;\n\
}",
"proc lcsunixactive { ip } {\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	set prompt unknown;\n\
	set status dead;\n\
	set savetimeout $timeout;\n\
	set timeout 3;\n\
	#\n\
	spawn -noecho telnet $ip\n\
	expect {\n\
	\"Connected to \" {\n\
		set status alive;\n\
	}\n\
	\"Connection refused\" {\n\
		puts \"Connection refused to $ip ...\";\n\
		set status \"telnet connection refused\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	timeout {\n\
		puts \"Timed out connecting to $ip ...\";\n\
		set status \"telnet time out\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	\"user name:\" {\n\
		send \"$username\\r\";\n\
	}\n\
	timeout {\n\
		puts \"Timed out waiting for username prompt to $ip ...\";\n\
		set status \"telnet time out waiting for username prompt\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	\"password:\" {\n\
		send \"$userpasswd\\r\";\n\
	}\n\
	timeout {\n\
		puts \"Timed out waiting for passwd prompt to $ip ...\";\n\
		set status \"telnet time out waiting for passwd prompt\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	-re \"(\\[a-z]\\[0-9]+\\[ab]#)\" {\n\
		set prompt \"$expect_out(1,string)\";\n\
		set status \"$prompt - active\";\n\
		send \"PS1=\\\"COMMAND_IS_DONE>\\\"\\r\";\n\
	}\n\
	-re \"(unnamed_system#)\" {\n\
		# puts \"WARNING: UNIX is active, but 'unnamed_system' prompt was received.\";\n\
		set prompt \"$expect_out(1,string)\";\n\
		set status \"$prompt - active\";\n\
		send \"PS1=\\\"COMMAND_IS_DONE>\\\"\\r\";\n\
	}\n\
	timeout {\n\
		puts \"Timed out waiting for cmd prompt to $ip ...\";\n\
		set status \"$prompt - time out waiting for cmd prompt (1)\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		send \"\\[ -x /Telica/swCPU/CurrRel/system/scripts/telica.rc.network ] && echo \\\"APPLICATIONMODE\\\";\\r\";\n\
	}\n\
	timeout {\n\
		# ignore time-outs\n\
		set status \"$prompt - time out waiting for cmd prompt (2)\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*\\\"APPLICATIONMODE\\\";\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		# ignore time-outs\n\
		set status \"$prompt - time out waiting for cmd prompt (3)\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*APPLICATIONMODE\" {\n\
		# set appstring $expect_out(0,string);\n\
		# puts \"APPMODE STRING: $appstring\";\n\
		set status \"$prompt - active - application mode\";\n\
	}\n\
	-re \"COMMAND_IS_DONE>\" {\n\
		# set minstring $expect_out(0,string);\n\
		# puts \"MINMODE STRING: $minstring\";\n\
		set status \"$prompt - active - minimum mode, OS-only\";\n\
	}\n\
	timeout {\n\
		# ignore time-outs\n\
		set status \"$prompt - time out waiting for cmd prompt (4)\";\n\
		catch { close; wait; } ignore;\n\
		set timeout $savetimeout;\n\
		return $status;\n\
	}\n\
	}\n\
	#\n\
	catch { close; wait; } ignore;\n\
	#\n\
	set timeout $savetimeout;\n\
	#\n\
	return $status;\n\
}",
"proc getspstatus { labid labidip sp } {\n\
	# check if responding on network\n\
	set spstatus [lcsping $labidip];\n\
	if {$spstatus == \"alive\"} {\n\
		puts \"$labid $sp $labidip is alive.\";\n\
		#\n\
		set spstatus [lcsunixactive $labidip];\n\
		puts \"$labid $sp $labidip UNIX is $spstatus.\";\n\
		if {[string match \"*active*application*\" $spstatus]} {\n\
			puts \"\\nWARNING: $sp is in APPLICATION mode.\";\n\
			puts \"Boot Lab into MIN OS-only mode: lcsreboot -m\\n\";\n\
		}\n\
	} else {\n\
		puts \"$labid $sp $labidip is not responding.\";\n\
	}\n\
	return $spstatus;\n\
}",
"proc getimages2load { labid bname lname ilistname } {\n\
	upvar $ilistname imagelist;\n\
	upvar $bname branch;\n\
	upvar $lname cpuload;\n\
	global logfile;\n\
	global usingdefaultlogfile;\n\
	global query;\n\
	#\n\
	if {[dbselect llobuf labloads \"labid req ^$labid\\$\" \"branch cpuload\" ] != 0} {\n\
		puts \"getimages2load: dbselect of relation 'labloads' failed.\";\n\
		logmsg reload \"END: getimages2load: dbselect of relation 'labloads' failed.\";\n\
		exit 2;\n\
	}\n\
	if {![info exists llobuf] || [llength llobuf] == 0} {\n\
		puts \"getimages2load: no loads found for labid $labid.\";\n\
		logmsg reload \"END: getimages2load: no loads found for labid $labid.\";\n\
		exit 2;\n\
	}\n\
	#\n\
	set choices $llobuf;\n\
	set choices [linsert $choices 0 \"QUIT\"];\n\
	chooseone \"Choose a load to reload: \" choices choice;\n\
	if {$choice == \"QUIT\"} {\n\
		if {$usingdefaultlogfile != 0} {\n\
			if {[file exists $logfile]} {\n\
				file delete -force -- $logfile;\n\
			}\n\
		}\n\
		puts \"exiting reload.\";\n\
		exit 0;\n\
	}\n\
	#\n\
	set ldata [split $choice \" \\t\"];\n\
	set branch [lindex $ldata 0];\n\
	set cpuload [lindex $ldata 1];\n\
	#\n\
	puts \"Will reload load <$branch, $cpuload>.\";\n\
	#\n\
	if {[dbselect iobuf images \"branch req ^$branch\\$ and cpuload req ^$cpuload\\$\" \"type name\" ] != 0} {\n\
		puts \"getimages2load: dbselect of relation 'images' failed.\";\n\
		logmsg reload \"END: getimages2load: dbselect of relation 'images' failed.\";\n\
		exit 2;\n\
	}\n\
	if {![info exists iobuf] || [llength iobuf] == 0} {\n\
		puts \"getimages2load: no images found for <$branch, $cpuload>.\";\n\
		logmsg reload \"END: getimages2load: no images found for <$branch, $cpuload>.\";\n\
		exit 2;\n\
	}\n\
	#\n\
	set timeout -1;\n\
	if {[info exists imagelist]} {\n\
		unset imagelist;\n\
	}\n\
	foreach image $iobuf {\n\
		if {$query == 0} {\n\
			lappend imagelist $image;\n\
			send_tty \"Will reload $image ...\\n\";\n\
		} else {\n\
			send_user \"\\nReload this image <$image>? \\[y/n/cr=n] \";\n\
			expect_user {\n\
			-re \"y\\n\" {\n\
				lappend imagelist $image;\n\
				send_tty \"Will reload $image ...\\n\";\n\
			}\n\
			-re \"\\[^\\n]*\\n\" {\n\
				send_tty \"Will NOT reload $image ...\\n\";\n\
			}\n\
			eof {\n\
				send_tty \"END-OF-FILE. exiting reload.\\n\";\n\
				logmsg reload \"END: END-OF-FILE. exiting reload.\\n\";\n\
				eofexit 2;\n\
			}\n\
			}\n\
		}\n\
	}\n\
}",
"proc clear_remote_fs { machine remotefs } {\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	set loginseen 0;\n\
	set savetimeout $timeout;\n\
	set timeout 45;\n\
	#\n\
	spawn -noecho \"/usr/bin/telnet\" \"-l $username\" $machine;\n\
	expect {\n\
	-re \".*user name:\" {\n\
		set loginseen 1;\n\
		send \"$username\\r\";\n\
	}\n\
	-re \".*password:\" {\n\
		set loginseen 0;\n\
		send \"$userpasswd\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during telnet ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during telnet ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	if {$loginseen} {\n\
		expect {\n\
		-re \".*password:\" {\n\
			send \"$userpasswd\\r\";\n\
		}\n\
		timeout {\n\
			puts \"\\ntimeout during telnet, waiting for password: ...\";\n\
			timeouthelp;\n\
			logmsg reload \"END: timeout during telnet, waiting for password: ...\";\n\
			exit 2;\n\
		}\n\
		};\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*# \" {\n\
		send \"PS1=\\\"COMMAND_IS_DONE>\\\"\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during prompt ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during prompt ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		send \"cd $remotefs \\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during PS1= ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during PS1= ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		# send \"/bin/find $remotefs -type f -print | /bin/cat \\r\";\n\
		send \"/bin/find $remotefs -type f -print | /bin/xargs rm -f \\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during cd ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during cd ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during rm ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during rm ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	catch { close; wait; } ignore;\n\
	#\n\
	set timeout $savetimeout;\n\
}",
"proc remove_remote_file { machine remotefile } {\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	set loginseen 0;\n\
	set savetimeout $timeout;\n\
	set timeout 15;\n\
	#\n\
	spawn -noecho \"/usr/bin/telnet\" \"-l $username\" $machine;\n\
	expect {\n\
	-re \".*user name:\" {\n\
		set loginseen 1;\n\
		send \"$username\\r\";\n\
	}\n\
	-re \".*password:\" {\n\
		set loginseen 0;\n\
		send \"$userpasswd\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during telnet ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during telnet ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	if {$loginseen} {\n\
		expect {\n\
		-re \".*password:\" {\n\
			send \"$userpasswd\\r\";\n\
		}\n\
		timeout {\n\
			puts \"\\ntimeout during telnet, waiting for password: ...\";\n\
			timeouthelp;\n\
			logmsg reload \"END: timeout during telnet, waiting for password: ...\";\n\
			exit 2;\n\
		}\n\
		};\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*# \" {\n\
		send \"PS1=\\\"COMMAND_IS_DONE>\\\"\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during prompt ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during prompt ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		send \"cd /\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during PS1= ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during PS1= ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		send \"/bin/rm -f $remotefile \\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during cd ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during cd ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during rm ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during rm ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	catch { close; wait; } ignore;\n\
	#\n\
	set timeout $savetimeout;\n\
}",
"proc loadimage { labid cpu_ip branch cpuload ilistname } {\n\
	upvar $ilistname images2load;\n\
	# get text file system\n\
	if {[dbselect fsobuf filesystems \"branch req ^$branch\\$ and type req text\" \"path\" ] != 0} {\n\
		puts \"loadimage: dbselect of relation 'filesystems' failed.\";\n\
		logmsg reload \"END: loadimage: dbselect of relation 'filesystems' failed.\";\n\
		exit 2;\n\
	}\n\
	if {![info exists fsobuf] || [llength fsobuf] == 0} {\n\
		puts \"loadimage: no filesystem found for branch $branch.\";\n\
		logmsg reload \"END: loadimage: no filesystem found for branch $branch.\";\n\
		exit 2;\n\
	}\n\
	set filesystem [lindex $fsobuf 0];\n\
	#\n\
	puts \"\";\n\
	puts \"Clearing /home filesystem on $cpu_ip ...\";\n\
	clear_remote_fs $cpu_ip \"/home\";\n\
	#\n\
	foreach imagedata $images2load {\n\
		set idata [split $imagedata \" \\t\"];\n\
		set swtype [lindex $idata 0];\n\
		set swfile [lindex $idata 1];\n\
		#\n\
		if {[regexp \"^\\[0-9A-Z\\.]*(_.*)$\" $swfile ignore suffix] != 1} {\n\
			puts \"loadimage: invalid file name - $swfile.\";\n\
			logmsg reload \"END: loadimage: invalid file name - $swfile.\";\n\
			exit 2;\n\
		}\n\
		#\n\
		set fromfile \"$filesystem/$branch/$swtype/$swfile\";\n\
		set tofile \"/home/$swfile\";\n\
		#\n\
		puts \"\";\n\
		puts \"CPU-IP    : $cpu_ip\";\n\
		puts \"Suffix    : $suffix\";\n\
		puts \"From file : $fromfile\";\n\
		puts \"To file   : $tofile\";\n\
		#\n\
		puts \"\";\n\
		puts \"Removing file /*${suffix} ...\"\n\
		remove_remote_file $cpu_ip \"/*${suffix}\";\n\
		puts \"\";\n\
		puts \"Removing file /home/*${suffix} ...\"\n\
		remove_remote_file $cpu_ip \"/home/*${suffix}\";\n\
		#\n\
		puts \"\";\n\
		puts \"Loading file $tofile ...\"\n\
		ftp_put_binary_file $cpu_ip $fromfile $tofile;\n\
	}\n\
}",
"proc do_we_proceed { } {\n\
	send_user \"Proceed with installation? \\[y/n/cr=n] \";\n\
	expect_user {\n\
	-re \"y\\n\" {\n\
		send_tty \"Proceeding ...\\n\\n\";\n\
	}\n\
	-re \"\\[yY]\\[eE]\\[sS]\\n\" {\n\
		send_tty \"Proceeding ...\\n\\n\";\n\
	}\n\
	-re \"(\\[^\\n]*)\\n\" {\n\
		send_tty \"Exiting reload.\\n\\n\";\n\
		logmsg reload \"END: Exiting reload.\";\n\
		exit 2;\n\
	}\n\
	eof {\n\
		send_tty \"END-OF-FILE. exiting reload.\\n\";\n\
		logmsg reload \"END: END-OF-FILE. exiting reload.\";\n\
		eofexit 2;\n\
	}\n\
	}\n\
}",
"proc do_we_reload { labid sp cpu_ip_name } {\n\
	upvar $cpu_ip_name cpu_ip;\n\
	puts \"\\nReloading lab $labid $sp $cpu_ip.\";\n\
	send_user \"Is this correct? \\[y/n/cr=n] \";\n\
	expect_user {\n\
	-re \"y\\n\" {\n\
		send_tty \"\\n==>> Will reload lab $labid $sp.\\n\\n\";\n\
	}\n\
	-re \"(\\[^\\n]*)\\n\" {\n\
		send_tty \"\\n==>> Will NOT reload lab $labid.\\n\\n\";\n\
		set cpu_ip \"\";\n\
	}\n\
	eof {\n\
		send_tty \"END-OF-FILE. exiting reload.\\n\";\n\
		logmsg reload \"END: END-OF-FILE. exiting reload.\";\n\
		eofexit 2;\n\
	}\n\
	}\n\
}",
"proc telnet_to { ip spawn_id_name } {\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	upvar $spawn_id_name spawn_id;\n\
	#\n\
	set loginseen 0;\n\
	set savetimeout $timeout;\n\
	set timeout 15;\n\
	#\n\
	spawn -noecho \"/usr/bin/telnet\" \"-l $username\" $ip;\n\
	expect {\n\
	-re \".*user name:\" {\n\
		set loginseen 1;\n\
		send \"$username\\r\";\n\
	}\n\
	-re \".*password:\" {\n\
		set loginseen 0;\n\
		send \"$userpasswd\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during login ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during login ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	if {$loginseen} {\n\
		expect {\n\
		-re \".*password:\" {\n\
			send \"$userpasswd\\r\";\n\
		}\n\
		timeout {\n\
			puts \"\\ntimeout during passwd ...\";\n\
			timeouthelp;\n\
			logmsg reload \"END: timeout during passwd ...\";\n\
			exit 2;\n\
		}\n\
		};\n\
	}\n\
	#\n\
	expect {\n\
	-re \".*# \" {\n\
		send \"PS1=\\\"COMMAND_IS_DONE>\\\"\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during PS1= ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during PS1= ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		send \"cd /\\r\";\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during cd ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during cd ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	expect {\n\
	-re \".*COMMAND_IS_DONE>\" {\n\
		# ok.\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during cd ...\";\n\
		timeouthelp;\n\
		logmsg reload \"END: timeout during cd ...\";\n\
		exit 2;\n\
	}\n\
	};\n\
	#\n\
	set timeout $savetimeout;\n\
}",
"proc killemf { cpu_a_ip cpu_b_ip } {\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	if {$cpu_a_ip != \"\" && $cpu_b_ip != \"\"} {\n\
		puts \"\\nKilling emf on $cpu_a_ip and $cpu_b_ip.\\n\";\n\
		#\n\
		set cpu_a_killed 0;\n\
		set cpu_b_killed 0;\n\
		#\n\
		telnet_to $cpu_a_ip cpu_a_id;\n\
		telnet_to $cpu_b_ip cpu_b_id;\n\
		#\n\
		set timeout 10;\n\
		send -i $cpu_a_id \"ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\\r\";\n\
		send -i $cpu_b_id \"ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\\r\";\n\
		expect {\n\
		-i $cpu_a_id -re \".*COMMAND_IS_DONE>\" {\n\
			if {!$cpu_b_killed} {\n\
				set cpu_a_killed 1;\n\
				exp_continue;\n\
			}\n\
		}\n\
		-i $cpu_b_id -re \".*COMMAND_IS_DONE>\" {\n\
			if {!$cpu_a_killed} {\n\
				set cpu_b_killed 1;\n\
				exp_continue;\n\
			}\n\
		}\n\
		timeout {\n\
			# ok\n\
		}\n\
		}\n\
		#\n\
		catch { close $cpu_a_id; } ignore;\n\
		catch { close $cpu_b_id; } ignore;\n\
		#\n\
		return;\n\
	} elseif {$cpu_a_ip != \"\"} {\n\
		set cpu_ip $cpu_a_ip;\n\
	} elseif {$cpu_b_ip != \"\"} {\n\
		set cpu_ip $cpu_b_ip;\n\
	} else  {\n\
		puts \"killemf: both CPU IPs are NULL.\";\n\
		return;\n\
	}\n\
	#\n\
	puts \"\\nKilling emf on $cpu_ip.\\n\";\n\
	#\n\
	telnet_to $cpu_ip cpu_id;\n\
	#\n\
	set timeout 10;\n\
	send -i $cpu_id \"ps xf | grep './emf' | cut -d' ' -f1 | xargs kill -9\\r\";\n\
	expect {\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		# ok\n\
	}\n\
	}\n\
	#\n\
	catch { close $cpu_id; } ignore;\n\
	return;\n\
}",
"proc removeoldload { cpu_ip ilistname } {\n\
	upvar $ilistname images2load;\n\
	#\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	global dbcmds;\n\
	#\n\
	puts \"\\nRemoving load from $cpu_ip.\\n\";\n\
	#\n\
	telnet_to $cpu_ip cpu_id;\n\
	#\n\
	set timeout 30;\n\
	send -i $cpu_id \"/Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed\\r\";\n\
	expect {\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"removeoldload: timeout waiting for remove_configuration_data.\";\n\
		# clean up and logon again.\n\
		catch { close $cpu_id; } ignore;\n\
		telnet_to $cpu_ip cpu_id;\n\
	}\n\
	}\n\
	#\n\
	set yesnum 0;\n\
	send -i $cpu_id \"/opt/TimesTen4.*/32/bin/setup.sh -uninstall\\r\";\n\
	expect {\n\
	-i $cpu_id -re \".*completely.*remove.*TimesTen.*yes \" {\n\
		send -i $cpu_id \"yes\\r\";\n\
		incr yesnum;\n\
		exp_continue;\n\
	}\n\
	-i $cpu_id -re \".*remove.*all.*files.*in.*no \" {\n\
		send -i $cpu_id \"yes\\r\";\n\
		incr yesnum;\n\
		exp_continue;\n\
	}\n\
	-i $cpu_id -re \".*remove.*remaining.*system.*files.*no \" {\n\
		send -i $cpu_id \"yes\\r\";\n\
		incr yesnum;\n\
		exp_continue;\n\
	}\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		if {$yesnum != 3} {\n\
			puts \"removeoldload: setup.sh failed (yesnum=$yesnum).\";\n\
			# clean up and logon again.\n\
			catch { close $cpu_id; } ignore;\n\
			telnet_to $cpu_ip cpu_id;\n\
		}\n\
	}\n\
	timeout {\n\
		puts \"removeoldload: timeout during setup.sh.\";\n\
		# clean up and logon again.\n\
		catch { close $cpu_id; } ignore;\n\
		telnet_to $cpu_ip cpu_id;\n\
	}\n\
	}\n\
	#\n\
	set timeout 120;\n\
	foreach imagedata $images2load {\n\
		set moreswtype2rm \"\";\n\
		#\n\
		set idata [split $imagedata \" \\t\"];\n\
		set swtype [lindex $idata 0];\n\
		set swfile [lindex $idata 1];\n\
		#\n\
		# ugh. there are better ways, but it'll do for now.\n\
		switch -regexp -- $swfile {\n\
		{^.*_ana.tar.gz$} {\n\
			set swtype2rm \"swANA\";\n\
		}\n\
		{^.*_atmds3.tar.gz$} {\n\
			set swtype2rm \"swATMDS3\";\n\
		}\n\
		{^.*_cpu.tar.gz$} {\n\
			set swtype2rm \"swCPU\";\n\
			set moreswtype2rm \"swCraft\";\n\
		}\n\
		{^.*_cm.tar.gz$} {\n\
			set swtype2rm \"swCM\";\n\
		}\n\
		{^.*_ds1_2.tar.gz$} {\n\
			set swtype2rm \"swDS1_2\";\n\
		}\n\
		{^.*_ds1.tar.gz$} {\n\
			set swtype2rm \"swDS1\";\n\
		}\n\
		{^.*_ds3.tar.gz$} {\n\
			set swtype2rm \"swDS3\";\n\
		}\n\
		{^.*_e1.tar.gz$} {\n\
			set swtype2rm \"swE1\";\n\
		}\n\
		{^.*_ena2.tar.gz$} {\n\
			set swtype2rm \"swENA2\";\n\
		}\n\
		{^.*_ena.tar.gz$} {\n\
			set swtype2rm \"swENA\";\n\
		}\n\
		{^.*_octds3_2.tar.gz$} {\n\
			set swtype2rm \"swOCTDS3_2\";\n\
		}\n\
		{^.*_octds3_3.tar.gz$} {\n\
			set swtype2rm \"swOCTDS3_3\";\n\
		}\n\
		{^.*_octds3.tar.gz$} {\n\
			set swtype2rm \"swOCTDS3\";\n\
		}\n\
		{^.*_pna.tar.gz$} {\n\
			set swtype2rm \"swPNA\";\n\
		}\n\
		{^.*_tdmoc.tar.gz$} {\n\
			set swtype2rm \"swTDMOC\";\n\
		}\n\
		{^.*_trids3_3.tar.gz$} {\n\
			set swtype2rm \"swTRIDS3_3\";\n\
		}\n\
		{^.*_trids3.tar.gz$} {\n\
			set swtype2rm \"swTRIDS3\";\n\
		}\n\
		{^.*_voip6.tar.gz$} {\n\
			set swtype2rm \"swVOP6\";\n\
		}\n\
		{^.*_voip.tar.gz$} {\n\
			set swtype2rm \"swVOP\";\n\
		}\n\
		{^.*_vs3.tar.gz$} {\n\
			set swtype2rm \"swVS3\";\n\
		}\n\
		default {\n\
			puts \"removeoldload: Unknown file type $swfile.\";\n\
			logmsg reload \"END: removeoldload: Unknown file type $swfile.\";\n\
			exit 2;\n\
		}\n\
		}\n\
		puts \"\";\n\
		puts \"Removing type $swtype2rm ($swtype) ...\";\n\
		send -i $cpu_id \"rm -rf /Telica/${swtype2rm}/* 1>/dev/null 1>&2\\r\";\n\
		#\n\
		expect {\n\
		-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
			# ok\n\
		}\n\
		timeout {\n\
			puts \"removeoldload: timeout during rm of '$swtype'.\";\n\
			# clean up and logon again.\n\
			catch { close $cpu_id; } ignore;\n\
			telnet_to $cpu_ip cpu_id;\n\
		}\n\
		}\n\
		if {$moreswtype2rm != \"\"} {\n\
			puts \"\";\n\
			puts \"Removing type $moreswtype2rm ...\";\n\
			send -i $cpu_id \"rm -rf /Telica/${moreswtype2rm}/* 1>/dev/null 1>&2\\r\";\n\
			#\n\
			expect {\n\
			-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
				# ok\n\
			}\n\
			timeout {\n\
				puts \"removeoldload: timeout during rm of '$moreswtype2rm'.\";\n\
				# clean up and logon again.\n\
				catch { close $cpu_id; } ignore;\n\
				telnet_to $cpu_ip cpu_id;\n\
			}\n\
			}\n\
		}\n\
	}\n\
	#\n\
	# this keeps coring. catch it and ignore it.\n\
	#\n\
	trap SIG_IGN SIGBUS;\n\
	#\n\
	catch {\n\
		# clean up old files and directories.\n\
		set timeout 300;\n\
		#\n\
		foreach dbcmd $dbcmds {\n\
			puts \"\";\n\
			puts \"Executing: $dbcmd ...\";\n\
			send -i $cpu_id \"$dbcmd \\r\";\n\
			#\n\
			expect {\n\
			-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
				# ok\n\
			}\n\
			timeout {\n\
				puts \"removeoldload: timeout during <$dbcmd>.\";\n\
			}\n\
			}\n\
		}\n\
		#\n\
	} ignore;\n\
	#\n\
	catch { close $cpu_id; } ignore;\n\
	#\n\
	trap SIG_DFL SIGBUS;\n\
	#\n\
	return;\n\
}",
"proc installnewload { cpu_ip cpu_load ilistname } {\n\
	upvar $ilistname images2load;\n\
	#\n\
	global username;\n\
	global userpasswd;\n\
	global timeout;\n\
	#\n\
	telnet_to $cpu_ip cpu_id;\n\
	#\n\
	set timeout 1500;\n\
	#\n\
	foreach imagedata $images2load {\n\
		set idata [split $imagedata \" \\t\"];\n\
		set swtype [lindex $idata 0];\n\
		set swfile [lindex $idata 1];\n\
		#\n\
		set tarfile \"/home/$swfile\";\n\
		#\n\
		puts \"\";\n\
		puts \"Expanding file $tarfile ...\";\n\
		# added 'm' option to allow DiskClean Up to work, as per\n\
		# Dan Scanlon (6/6/05).\n\
		# OLD: send -i $cpu_id \"/bin/tar xzvf $tarfile\\r\";\n\
		send -i $cpu_id \"/bin/tar xzvmf $tarfile\\r\";\n\
		#\n\
		expect {\n\
		-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
			# ok\n\
		}\n\
		timeout {\n\
			puts \"installnewload: timeout during 'tar xzvmf $tarfile'.\";\n\
		}\n\
		}\n\
	}\n\
	#\n\
	set timeout 30;\n\
	#\n\
	puts \"\";\n\
	puts \"Linking $cpu_load to CurrRel ...\";\n\
	send -i $cpu_id \"/bin/ln -s /Telica/swCPU/${cpu_load} /Telica/swCPU/CurrRel\\r\";\n\
	#\n\
	expect {\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"installnewload: timeout during 'ln -s ...'.\";\n\
	}\n\
	}\n\
	#\n\
	set timeout 900;\n\
	#\n\
	puts \"\";\n\
	puts \"Running uprev.sh ...\";\n\
	puts \"\";\n\
	puts \"WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\";\n\
	puts \"\";\n\
	puts \"DO NOT INTERRUPT RELOAD AT THIS POINT. TIMESTEN DATABASE IS \";\n\
	puts \"INSTALLING RIGHT NOW AND IF YOU INTERRUPT IT, YOU RISK TRASHING\";\n\
	puts \"YOUR LAB AND HAVING TO RESTART.\";\n\
	puts \"\";\n\
	puts \"WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\";\n\
	puts \"\";\n\
	send -i $cpu_id \"cd /Telica/swCPU/CurrRel/system/scripts\\r\";\n\
	#\n\
	expect {\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"\\ntimeout during cd ...\";\n\
	}\n\
	};\n\
	#\n\
	send -i $cpu_id \"./uprev.sh\\r\";\n\
	#\n\
	expect {\n\
	-i $cpu_id -re \".*COMMAND_IS_DONE>\" {\n\
		# ok\n\
	}\n\
	timeout {\n\
		puts \"installnewload: timeout during 'uprev.sh'.\";\n\
	}\n\
	}\n\
	#\n\
	catch { close $cpu_id; } ignore;\n\
	return;\n\
}",
"proc makehwscripts { labid branch cpuload } {\n\
	global env;\n\
	#\n\
	catch {\n\
		system \"$env(LCSTOOLSBIN)/mkhwscripts $branch $cpuload $labid\";\n\
	} ignore;\n\
	catch { wait -nowait; } ignore;\n\
	#\n\
}",
"proc initdbcmds { cmdsname } {\n\
	upvar $cmdsname cmds;\n\
	#\n\
	lappend cmds \"cd /Telica\";\n\
	lappend cmds \"rm -rf SAVEdbCurrent 1>/dev/null 2>&1\";\n\
	lappend cmds \"mv dbCurrent SAVEdbCurrent\";\n\
	lappend cmds \"mkdir dbCurrent\";\n\
	lappend cmds \"cd SAVEdbCurrent\";\n\
	lappend cmds \"find . -type d -print | cpio -pduvm ../dbCurrent\";\n\
	lappend cmds \"cp Telica_IP Telica_Hostname ../dbCurrent\";\n\
	lappend cmds \"echo '# template rc.local file' >../dbCurrent/rc.local\";\n\
	lappend cmds \"rm -rf SAVEdbCurrent 1>/dev/null 2>&1\";\n\
	#\n\
	lappend cmds \"cd /Telica\";\n\
	lappend cmds \"rm -rf SAVErestore 1>/dev/null 2>&1\";\n\
	lappend cmds \"mv restore SAVErestore\";\n\
	lappend cmds \"mkdir restore\";\n\
	lappend cmds \"cd SAVErestore\";\n\
	lappend cmds \"find . -type d -print | cpio -pduvm ../restore\";\n\
	lappend cmds \"echo '# template rc.local file' >../restore/rc.local\";\n\
	lappend cmds \"rm -rf SAVErestore 1>/dev/null 2>&1\";\n\
	#\n\
	lappend cmds \"rm -f /.odbc.ini 1>/dev/null 1>&2\";\n\
	lappend cmds \"rm -rf /opt/TimesTen* 1>/dev/null 1>&2\";\n\
	lappend cmds \"echo '# over-written by reload' >/etc/resolv.conf\";\n\
	lappend cmds \"rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2\";\n\
}",
"global env;",
"set username \"root\";",
"set userpasswd \"plexus9000\";",
"set maxlogons 5;",
"set stracelevel -1;",
"set usingdefaultlogfile 1;",
"set doloadimage 1;",
"set doexpansion 1;",
"set dohardware 0;",
"set ask 1;",
"set query 0;",
"set logfile \"\";",
"set verbose 0;",
"set labid \"\";",
"set LABID \"\";",
"set removelock 0;",
"set background 0;",
"set initsp \"none\";",
"log_user 0;",
"logusage reload;",
"for {set arg 0} {$arg<$argc} {incr arg} {\n\
	set argval [lindex $argv $arg];\n\
	switch -regexp -- $argval {\n\
	{^-x} { debug -now; }\n\
	{^-K} { set doloadimage 1; set doexpansion 0; }\n\
	{^-X} { set doloadimage 0; set doexpansion 1; }\n\
	{^-D} { set ask 0; }\n\
	{^-Q} { set query 1; }\n\
	{^-V} { set verbose 1; }\n\
	{^-R} { set removelock 1; }\n\
	{^-B} { set background 1; set ask 0; }\n\
	{^-H} { set dohardware 1; set doloadimage 0; set doexpansion 0; }\n\
	{^-S.*} { getoptval $argval stracelevel arg; }\n\
	{^-l.*} { getoptval $argval logfile arg; }\n\
	{^-u.*} { getoptval $argval username arg; }\n\
	{^-p.*} { getoptval $argval userpasswd arg; }\n\
	{^-i.*} { getoptval $argval initsp arg; }\n\
	{^-I.*} { \n\
		getoptval $argval initsp arg; \n\
		set doloadimage 0; \n\
		set doexpansion 0;\n\
		set ask 0;\n\
	}\n\
	{^-\\?} { usage; exit 0; }\n\
	{^-T} { timeouthelp; exit 0; }\n\
	{^--} { incr arg; break; }\n\
	{^-.*} { puts \"Unknown option: $argval\\n\"; usage; exit 2 }\n\
	default { break; }\n\
	}\n\
}",
"switch -- $initsp {\n\
\"simplex_a\" { # ok }\n\
\"simplex_b\" { # ok }\n\
\"duplex\" { # ok }\n\
\"none\" { # ok }\n\
default {\n\
	puts \"Invalid '-i' option value: $initsp\";\n\
	logmsg reload \"END: Invalid '-i' option value: $initsp\";\n\
	exit 2;\n\
}\n\
}",
"if {$stracelevel >= 0} {\n\
	strace $stracelevel;\n\
}",
"if {[string length $logfile] > 0} {\n\
	if {[catch {log_file -noappend -a $logfile; } status]} {\n\
		puts \"Logging failed for file $logfile:\\n$status\";\n\
		logmsg reload \"END: Logging failed for file $logfile:\\n$status\";\n\
		exit 2;\n\
	}\n\
	set usingdefaultlogfile 0;\n\
} else {\n\
	# default logging for now\n\
	set mypid [pid];\n\
	set logfile \"/lcsl100/logfiles/reload/reload$mypid\";\n\
	puts \"\\nDefault log file is $logfile.\";\n\
	if {[catch {log_file -noappend -a $logfile; } status]} {\n\
		puts \"Logging failed for file $logfile:\\n$status\";\n\
		logmsg reload \"END: Logging failed for file $logfile:\\n$status\";\n\
		exit 2;\n\
	}\n\
	set usingdefaultlogfile 1;\n\
}",
"if {$verbose > 0} {\n\
	log_user 1;\n\
}",
"checkenv;",
"initdbcmds dbcmds;",
"if {$arg<$argc} {\n\
	set labid [lindex $argv $arg];\n\
} elseif {[info exists env(LABID)]} {\n\
	set labid $env(LABID);\n\
} else {\n\
	puts \"LABID is neither set nor given.\";\n\
	logmsg reload \"END: LABID is neither set nor given.\";\n\
	exit 2;\n\
}",
"puts \"\\nStarting reload of lab $labid:\\n\";",
"if {$removelock} {\n\
	puts \"WARNING: Forced removal of LOCK file for $labid.\";\n\
	logmsg reload \"WARNING: Forced removal of LOCK file for $labid.\";\n\
	set status [V reload $labid];\n\
	logmsg reload \"WARNING: status of removal of LOCK file for $labid: $status\";\n\
}",
"set status [P reload $labid];",
"if {$status == 0} {\n\
	puts \"Another reload of $labid in progress.\";\n\
	logmsg reload \"END: another reload of $labid in progress.\";\n\
	unset LABID;\n\
	exit 2;\n\
}",
"set LABID $labid;",
"if {[dbselect obuf chassis \"labid req ^$labid\\$\" \"cpu_a_ip cpu_b_ip\" ] != 0} {\n\
	puts \"reload: dbselect of relation 'chassis' failed.\";\n\
	logmsg reload \"END: reload: dbselect of relation 'chassis' failed.\";\n\
	exit 2;\n\
}",
"if {![info exists obuf] || [llength obuf] == 0} {\n\
	puts \"reload: no IPs found for labid $labid.\";\n\
	logmsg reload \"END: reload: no IPs found for labid $labid.\";\n\
	exit 2;\n\
}",
"set ips [lindex $obuf 0];",
"set ipdata [split $ips \" \\t\"];",
"set cpu_a_ip [lindex $ipdata 0];",
"set cpu_b_ip [lindex $ipdata 1];",
"set timeout -1;",
"set sp2load \"\";",
"do_we_reload $labid \"SP-A\" cpu_a_ip;",
"do_we_reload $labid \"SP-B\" cpu_b_ip;",
"if {$cpu_a_ip != \"\" && ($doloadimage || $doexpansion)} {\n\
	puts \"\\nCurrent SP-A Status:\\n\";\n\
	set spstatus [getspstatus $labid $cpu_a_ip \"SP-A\"];\n\
	logmsg reload \"SP-STATUS: $labid $cpu_a_ip SP-A is $spstatus.\";\n\
	if {[string match \"*active*application*\" $spstatus]} {\n\
		puts \"\\nFAILURE: Exiting reload because NOT in Min-Mode.\";\n\
		logmsg reload \"END: Exiting reload because NOT in Min-Mode.\";\n\
		exit 2;\n\
	}\n\
}",
"if {$cpu_b_ip != \"\" && ($doloadimage || $doexpansion)} {\n\
	puts \"\\nCurrent SP-B Status:\\n\";\n\
	set spstatus [getspstatus $labid $cpu_b_ip \"SP-B\"];\n\
	logmsg reload \"SP-STATUS: $labid $cpu_b_ip SP-B is $spstatus.\";\n\
	if {[string match \"*active*application*\" $spstatus]} {\n\
		puts \"\\nFAILURE: Exiting reload because NOT in Min-Mode.\";\n\
		logmsg reload \"END: Exiting reload because NOT in Min-Mode.\";\n\
		exit 2;\n\
	}\n\
}",
"puts \"\\n\";",
"if {$cpu_a_ip == \"\" && $cpu_b_ip == \"\"} {\n\
	# no problems, clean up default log file.\n\
	if {$usingdefaultlogfile != 0} {\n\
		if {[file exists $logfile]} {\n\
			file delete -force -- $logfile;\n\
		}\n\
	}\n\
	logmsg reload \"END: nothing chosen to reload.\";\n\
	puts \"Nothing to reload.\";\n\
	puts \"Exiting reload.\";\n\
	exit 0;\n\
}",
"getimages2load $labid branch cpuload images2load;",
"if {![info exists images2load] || [llength images2load] == 0} {\n\
	puts \"reload: no images chosen for reload. Exiting reload.\";\n\
	logmsg reload \"END: reload: no images chosen for reload. Exiting reload.\";\n\
	exit 2;\n\
}",
"if {$background != 0} {\n\
	set ask 0;\n\
	if {[fork] != 0} {\n\
		puts \"Backgrounding. Parent reload exiting.\";\n\
		logmsg reload \"END: Backgrounding. Parent reload exiting.\";\n\
		unset LABID;\n\
		exit 0;\n\
	}\n\
	# let parent exit and update log file, then continue.\n\
	sleep 5;\n\
	logmsg reload \"Backgrounding. Child disconnecting and continuing.\";\n\
	disconnect;\n\
}",
"if {$cpu_a_ip != \"\" && $cpu_b_ip != \"\"} {\n\
	logmsg reload \"START: reloading $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
	if {$doloadimage} {\n\
		logmsg reload \"START: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
		loadimage $labid $cpu_a_ip $branch $cpuload images2load;\n\
		loadimage $labid $cpu_b_ip $branch $cpuload images2load;\n\
		logmsg reload \"END: LOADIMAGES $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
	} else {\n\
		puts \"Skipping loadimage stage ...\";\n\
	}\n\
	if {$doexpansion} {\n\
		if {$ask} {\n\
			do_we_proceed;\n\
		}\n\
		logmsg reload \"START: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
		killemf $cpu_a_ip $cpu_b_ip;\n\
		removeoldload $cpu_a_ip images2load;\n\
		removeoldload $cpu_b_ip images2load;\n\
		installnewload $cpu_a_ip $cpuload images2load;\n\
		installnewload $cpu_b_ip $cpuload images2load;\n\
		what_to_do_next;\n\
		logmsg reload \"END: LOADEXPANSION $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
	} else {\n\
		puts \"Skipping expansion stage ...\";\n\
	}\n\
	if {$initsp != \"none\"} {\n\
		logmsg reload \"START: INIT of $labid with load $branch, $cpuload.\";\n\
		set postcmd \"\";\n\
		set rebootcmd \"\";\n\
		#\n\
		switch -- $initsp {\n\
		\"simplex_a\" {\n\
			set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a -s $labid\";\n\
			set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -A -a $labid\";\n\
		}\n\
		\"simplex_b\" {\n\
			set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a -s $labid\";\n\
			set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -B -a $labid\";\n\
		}\n\
		\"duplex\" {\n\
			set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a $labid\";\n\
			set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -a $labid\";\n\
		}\n\
		}\n\
		#\n\
		puts \"\\nPOST CMD: $postcmd \";\n\
		logmsg reload \"POST CMD: $postcmd \";\n\
		puts \"\\nREBOOT CMD: $rebootcmd \";\n\
		logmsg reload \"REBOOT CMD: $rebootcmd \";\n\
		#\n\
		if {[catch { system \"$postcmd\"; } errmsg] != 0} {\n\
			if {[string length $errmsg] > 0} {\n\
				puts \"\\npostlcsreload: $errmsg\";\n\
				logmsg reload \"ERROR: postlcsreload: $errmsg\";\n\
			} else {\n\
				puts \"\\npostlcsreload: unknown error.\";\n\
				logmsg reload \"ERROR: postlcsreload: unknown error.\";\n\
			}\n\
		} elseif {[catch { system \"$rebootcmd\"; } errmsg] != 0} {\n\
			if {[string length $errmsg] > 0} {\n\
				puts \"\\nlcsreboot: $errmsg\";\n\
				logmsg reload \"ERROR: lcsreboot: $errmsg\";\n\
			} else {\n\
				puts \"\\nlcsreboot: unknown error.\";\n\
				logmsg reload \"ERROR: lcsreboot: unknown error.\";\n\
			}\n\
		}\n\
		catch { wait -nowait; } ignore;\n\
		logmsg reload \"END: INIT of $labid with load $branch, $cpuload.\";\n\
	}\n\
	if {$dohardware} {\n\
		logmsg reload \"START: MAKEHWSCRIPTS $labid with load $branch, $cpuload.\";\n\
		makehwscripts $labid $branch $cpuload;\n\
		logmsg reload \"END: MAKEHWSCRIPTS $labid with load $branch, $cpuload.\";\n\
	} else {\n\
		puts \"Skipping make hardware scripts stage ...\";\n\
	}\n\
	if {$usingdefaultlogfile != 0} {\n\
		if {[file exists $logfile]} {\n\
			file delete -force -- $logfile;\n\
		}\n\
	}\n\
	logmsg reload \"END: reloading $labid SP-A and SP-B with load $branch, $cpuload.\";\n\
	puts \"Exiting reload.\";\n\
	exit 0\n\
} elseif {$cpu_a_ip != \"\"} {\n\
	set sp2load \"SP-A\";\n\
	set cpu_ip $cpu_a_ip;\n\
} elseif {$cpu_b_ip != \"\"} {\n\
	set sp2load \"SP-B\";\n\
	set cpu_ip $cpu_b_ip;\n\
} else {\n\
	# no problems, clean up default log file.\n\
	if {$usingdefaultlogfile != 0} {\n\
		if {[file exists $logfile]} {\n\
			file delete -force -- $logfile;\n\
		}\n\
	}\n\
	logmsg reload \"END: nothing chosen to reload.\";\n\
	puts \"Nothing to reload.\";\n\
	puts \"Exiting reload.\";\n\
	exit 0\n\
}",
"logmsg reload \"START: reloading $labid $sp2load with load $branch, $cpuload.\";",
"if {$doloadimage} {\n\
	logmsg reload \"START: LOADIMAGES $labid $sp2load with load $branch, $cpuload.\";\n\
	loadimage $labid $cpu_ip $branch $cpuload images2load;\n\
	logmsg reload \"END: LOADIMAGES $labid $sp2load with load $branch, $cpuload.\";\n\
} else {\n\
	puts \"Skipping loadimage stage ...\";\n\
}",
"if {$doexpansion} {\n\
	if {$ask} {\n\
		do_we_proceed;\n\
	}\n\
	logmsg reload \"START: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.\";\n\
	killemf $cpu_ip \"\";\n\
	removeoldload $cpu_ip images2load;\n\
	installnewload $cpu_ip $cpuload images2load;\n\
	what_to_do_next;\n\
	logmsg reload \"END: LOADEXPANSION $labid $sp2load with load $branch, $cpuload.\";\n\
} else {\n\
	puts \"Skipping expansion stage ...\";\n\
}",
"if {$initsp != \"none\"} {\n\
	logmsg reload \"START: INIT of $labid with load $branch, $cpuload.\";\n\
	set postcmd \"\";\n\
	set rebootcmd \"\";\n\
	#\n\
	switch -- $initsp {\n\
	\"simplex_a\" {\n\
		set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a -s $labid\";\n\
		set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -A -a $labid\";\n\
	}\n\
	\"simplex_b\" {\n\
		set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a -s $labid\";\n\
		set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -B -a $labid\";\n\
	}\n\
	\"duplex\" {\n\
		set postcmd \"$env(LCSTOOLSBIN)/postlcsreload -a $labid\";\n\
		set rebootcmd \"$env(LCSTOOLSBIN)/lcsreboot -a $labid\";\n\
	}\n\
	}\n\
	#\n\
	puts \"\\nPOST CMD: $postcmd \";\n\
	logmsg reload \"POST CMD: $postcmd \";\n\
	puts \"\\nREBOOT CMD: $rebootcmd \";\n\
	logmsg reload \"REBOOT CMD: $rebootcmd \";\n\
	#\n\
	if {[catch { system \"$postcmd\"; } errmsg] != 0} {\n\
		if {[string length $errmsg] > 0} {\n\
			puts \"\\npostlcsreload: $errmsg\";\n\
			logmsg reload \"ERROR: postlcsreload: $errmsg\";\n\
		} else {\n\
			puts \"\\npostlcsreload: unknown error.\";\n\
			logmsg reload \"ERROR: postlcsreload: unknown error.\";\n\
		}\n\
	} elseif {[catch { system \"$rebootcmd\"; } errmsg] != 0} {\n\
		if {[string length $errmsg] > 0} {\n\
			puts \"\\nlcsreboot: $errmsg\";\n\
			logmsg reload \"ERROR: lcsreboot: $errmsg\";\n\
		} else {\n\
			puts \"\\nlcsreboot: unknown error.\";\n\
			logmsg reload \"ERROR: lcsreboot: unknown error.\";\n\
		}\n\
	}\n\
	catch { wait -nowait; } ignore;\n\
	logmsg reload \"END: INIT of $labid with load $branch, $cpuload.\";\n\
}",
"if {$dohardware} {\n\
	logmsg reload \"START: MAKEHWSCRIPTS $labid with load $branch, $cpuload.\";\n\
	makehwscripts $labid $branch $cpuload;\n\
	logmsg reload \"END: MAKEHWSCRIPTS $labid with load $branch, $cpuload.\";\n\
} else {\n\
	puts \"Skipping make hardware scripts stage ...\";\n\
}",
"if {$usingdefaultlogfile != 0} {\n\
	if {[file exists $logfile]} {\n\
		file delete -force -- $logfile;\n\
	}\n\
}",
"logmsg reload \"END: reloading $labid $sp2load with load $branch, $cpuload.\";",
"puts \"Exiting reload.\";",
"exit 0;",
