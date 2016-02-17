# start up a tl1 session and exec commands
package provide hugtl1 1.0
#
package require checkretval
package require hwmaint
#
proc hug_tl1_killstream { cfgdata streamid } {
	upvar $cfgdata cfgd;
	if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		return;
	}
	set spawn_id $cfgd(tl1,$streamid,spawn_id);
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; wait -nowait; } ignore;
	catch { unset cfgd(tl1,$streamid,spawn_id); } ignore;
	return;
}
#
proc hug_tl1_open { cfgdata streamid vflag tmout ip login passwd {tl1port 2361} {maxlogons 12} { sleepbeforeescape 5 } } {
	#
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	# sanity check of arguments
	#
	if {[info exists cfgd(tl1,$streamid,spawn_id)]} {
		return "-1 - hug_tl1_open: stream id $streamid already exists.";
	} elseif {[string length $ip] <= 0} {
		return "-1 - hug_tl1_open: ip is null length.";
	} elseif {[string length $login] <= 0} {
		return "-1 - hug_tl1_open: login is null length.";
	} elseif {[string length $passwd] <= 0} {
		return "-1 - hug_tl1_open: passwd is null length.";
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	if {$tmout != 0} { set timeout $tmout; }
	if {$vflag} { log_user 1; }
	#
	# startup tl1 process.
	#
	for {set i 0} {$i<$maxlogons} {incr i} {
		spawn -noecho "/usr/bin/telnet" $ip $tl1port;
		set cfgd(tl1,$streamid,spawn_id) $spawn_id;
		#
		expect {
		"Connected to " {
			# ok
			break;
		}
		"Connection refused" {
			# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
			hug_tl1_killstream cfgd $streamid;
			puts "\nConnection refused. Sleep and retry.";
			sleep 5;
		}
		timeout {
			# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
			hug_tl1_killstream cfgd $streamid;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
		}
		eof {
			# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
			hug_tl1_killstream cfgd $streamid;
			puts "\nEOF. Sleep and retry.";
			sleep 5;
		}
		}
	}
	if {$i>=$maxlogons} {
		# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
		return "-1 - hug_tl1_open: unable to telnet to ($ip,$login).";
	}
	#
	sleep $sleepbeforeescape;
	#
	send "\r";
	expect {
	"Telica>" {
		# ok
	}
	timeout {
		# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: timeout waiting for prompt ($ip,$login).";
	}
	eof {
		# catch { close $cfgd(tl1,$streamid,spawn_id); } ignore;
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: eof waiting for prompt ($ip,$login).";
	}
	}
	#
	send "act-user::${login}:::${passwd};\r";
	#
	expect {
	-re "This is the WORKING System Processor.*M.*\[0-9]+.*COMPLD.*;\r\nTelica>" {
		puts "\nCOMPLETED received ...";
	}
	-re "Host is in Standby Mode.*;\r\nTelica>" {
		puts "\nHost is in Standby Mode ($ip,$login).";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: Host is in Standby Mode ($ip,$login).";
	}
	-re "This is the PROTECTION System Processor.*;\r\nTelica>" {
		puts "\nThis is the PROTECTION System Processor ($ip,$login).";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: This is the PROTECTION System Processor ($ip,$login).";
	}
	-re "Login Not Active.*;\r\nTelica>" {
		puts "\nLogin Not Active ($ip,$login).";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: Login Not Active ($ip,$login).";
	}
	-re "Can't login.*;\r\nTelica>" {
		puts "\nCan't login ($ip,$login).";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: Can't login ($ip,$login).";
	}
	-re ".*;\r\nTelica>" {
		puts "\nUnknown error ($ip,$login).";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: Unknown error ($ip,$login).";
	}
	timeout {
		puts "\ntimeout waiting for a response ...";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: timeout waiting for act-user response ($ip,$login).";
	}
	eof {
		puts "\neof waiting for a response ...";
		hug_tl1_killstream cfgd $streamid;
		return "-1 - hug_tl1_open: eof waiting for act-user response ($ip,$login).";
	}
	}
	#
	set cfgd(tl1,$streamid,login) $login;
	set cfgd(tl1,$streamid,passwd) $passwd;
	#
	# open log files, if any are requested.
	#
	puts "\nChecking for log files to open ...";
	#
	if {[info exists cfgd(globals,inproglogfile)] && \
	    [string length $cfgd(globals,inproglogfile)] > 0} {
		puts "\nOpening log file $cfgd(globals,inproglogfile)";
		set logfname $cfgd(globals,inproglogfile);
		if {[file exists $logfname]} {
			puts "\nAppending to TL1 log file $logfname.";
			set logfd [open $logfname "a+"];
		} else {
			puts "\nCreating to TL1 log file $logfname.";
			set logfd [open $logfname "w+"];
		}
		set cfgd(globals,inproglogfilefd) $logfd;
		puts $logfd "===>>> opened $logfname at [exec date] <<<===";
	}
	if {[info exists cfgd(globals,denylogfile)] && \
	    [string length $cfgd(globals,denylogfile)] > 0} {
		puts "\nOpening log file $cfgd(globals,denylogfile)";
		set logfname $cfgd(globals,denylogfile);
		if {[file exists $logfname]} {
			puts "\nAppending to TL1 log file $logfname.";
			set logfd [open $logfname "a+"];
		} else {
			puts "\nCreating to TL1 log file $logfname.";
			set logfd [open $logfname "w+"];
		}
		set cfgd(globals,denylogfilefd) $logfd;
		puts $logfd "===>>> opened $logfname at [exec date] <<<===";
	}
	if {[info exists cfgd(globals,misclogfile)] && \
	    [string length $cfgd(globals,misclogfile)] > 0} {
		puts "\nOpening log file $cfgd(globals,misclogfile)";
		set logfname $cfgd(globals,misclogfile);
		if {[file exists $logfname]} {
			puts "\nAppending to TL1 log file $logfname.";
			set logfd [open $logfname "a+"];
		} else {
			puts "\nCreating to TL1 log file $logfname.";
			set logfd [open $logfname "w+"];
		}
		set cfgd(globals,misclogfilefd) $logfd;
		puts $logfd "===>>> opened $logfname at [exec date] <<<===";
	}
	#
	if {$vflag} { log_user 0; }
	return "0 - success";
}
#
proc hug_tl1_open2 { cfgdata streamid vflag tmout spaip spbip login passwd {tl1port 2361} {maxlogons 12} { sleepbeforeescape 1 } } {
	#
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	# sanity check of arguments
	#
	if {[info exists cfgd(tl1,$streamid,spawn_id)]} {
		return "-1 - hug_tl1_open2: stream id $streamid already exists.";
	} elseif {[string length $spaip] <= 0} {
		return "-1 - hug_tl1_open2: spa-ip is null length.";
	} elseif {[string length $spbip] <= 0} {
		return "-1 - hug_tl1_open2: spb-ip is null length.";
	} elseif {[string length $login] <= 0} {
		return "-1 - hug_tl1_open2: login is null length.";
	} elseif {[string length $passwd] <= 0} {
		return "-1 - hug_tl1_open2: passwd is null length.";
	} 
	#
	# determine which is the active sp.
	#
	if {![info exists cfgd($spaip,spstate)]} {
		return "-1 - hug_tl1_open2: SP-A state does not exist.";
	} elseif {![info exists cfgd($spbip,spstate)]} {
		return "-1 - hug_tl1_open2: SP-B state does not exist.";
	}
	if {$cfgd($spaip,spstate) == "isact" || \
	    $cfgd($spaip,spstate) == "isactnbk"} {
		set spip $spaip;
	} elseif {$cfgd($spbip,spstate) == "isact" || \
	          $cfgd($spbip,spstate) == "isactnbk"} {
		set spip $spbip;
	} else {
		return "-1 - hug_tl1_open2: No active SP found.";
	}
	#
	set status [hug_tl1_open cfgd $streamid $vflag $tmout $spip $login $passwd $tl1port $maxlogons $sleepbeforeescape];
	if {[isNotOk $status]} {
		return "-1 - hug_tl1_open2: hug_tl1_open failed: \n$status";
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_exec { cfgdata streamid iflag vflag tmout xcmd } {
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		return "-1 - hug_tl1_exec: stream id $streamid does not exist.";
	} elseif {[string length $xcmd] <= 0} {
		return "-1 - hug_tl1_exec: cmd is null length.";
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	if {$tmout != 0} { set timeout $tmout; }
	if {$vflag} { log_user 1; }
	#
	# skip comments and white-space
	switch -regexp -- ${xcmd} {
	{^[ \t]*$} { return "0 - success"; }
	{^[ \t]*#} { return "0 - success"; }
	}
	# strip white space and other unwanted characters
	set xcmd [string trim $xcmd];
	set xcmd [string trim $xcmd "\""];
	set xcmd [string trimright $xcmd ";"];
	# get leading verb for IP pattern.
	set status [regexp -- {^([^:;]+)} $xcmd ignore verb];
	if {$status == 1} {
		set VERB [string toupper $verb];
		regsub -all -- "-" $VERB "\[-_]" VERB2;
		#set ippattern "/\\* *$VERB2 *\\*/.*\\\[\\r\\n];";
		set ippattern "/\\* *$VERB2\[^/]*\\*/\[^;]*;";
		set ipdenypattern "M  *\[0-9]+  *DENY.*/\\* *$VERB2\[^/]*\\*/\[^;]*;";
	} else {
		#set ippattern "/\\* *\[^ \\*/]+ *\\*/.*\\\[\\r\\n];";
		set ippattern "/\\* *\[^ \\*/]+ *\\*/\[^;]*;";
		set ipdenypattern "M  *\[0-9]+  *DENY.*/\\* *\[^ \\*/]+ *\\*/\[^;]*;";
	}
	puts "\nIP return pattern: $ippattern";
	# is this an init-sys cmd?
	set status [regexp -nocase -- {.*init-sys:.*} $xcmd ignore];
	if {$status == 1} {
		# EOF occurs after an init-sys, so just ignore it.
		set ignoreeof 1;
	} else {
		# not an init-sys, therefore eof can be an error.
		set ignoreeof 0;
	}
	#
	# is this a cpy-mem cmd?
	set status [regexp -nocase -- {.*cpy-mem:.*} $xcmd ignore];
	if {$status == 1} {
		# cpy-mems are synchronized independently.
		set ignoretmout 1;
	} else {
		# not a cpy-mem, don't ignore time out.
		set ignoretmout 0;
	}
	#
	# check for sleeps in scripts ...
	#
	if {[regexp -nocase -- {^[ \t]*sleep[ \t]+([0-9]+)} ${xcmd} ignore secs] == 1} {
		puts "\nSLEEP FOR $secs SECONDS ...";
		sleep $secs;
		if {$vflag} { log_user 0; }
		#
		return "0 - success";
	}
	#
	send -i $cfgd(tl1,$streamid,spawn_id) "${xcmd};\r";
	expect {
	-i $cfgd(tl1,$streamid,spawn_id) -re {M.*[0-9]+.*COMPLD.*;\r\nTelica>} {
		puts "\nCOMPLETED RECEIVED ...";
	}
	-i $cfgd(tl1,$streamid,spawn_id) -re {.*IP  *[0-9]+[\r\n]+< *[\r\n]+Telica>} {
		puts "\nIN PROGRESS RECEIVED ...";
		if {[info exists cfgd(globals,inproglogfilefd)]} {
			puts $cfgd(globals,inproglogfilefd) "${xcmd}";
		}
		expect {
		-i $cfgd(tl1,$streamid,spawn_id) -re $ipdenypattern {
			puts "\nEnd of $verb - DENY Received";
			if {[info exists cfgd(globals,denylogfilefd)]} {
				puts $cfgd(globals,denylogfilefd) "${xcmd}";
			}
			if {$iflag == 0} {
				if {$vflag} { log_user 0; }
				return "-1 - hug_tl1_exec: (IP return) DENY during cmd:\n$xcmd";
			}
		}
		-i $cfgd(tl1,$streamid,spawn_id) -re $ippattern {
			puts "\nEnd of $verb - OK";
		}
		timeout {
			if {[info exists cfgd(globals,misclogfilefd)]} {
				puts $cfgd(globals,misclogfilefd) "TIME OUT - ${xcmd}";
			}
			if {$iflag == 0 && $ignoretmout == 0} {
				if {$vflag} { log_user 0; }
				return "-1 - hug_tl1_exec: (IP return) timeout during cmd:\n$xcmd";
			}
		}
		eof {
			if {[info exists cfgd(globals,misclogfilefd)]} {
				puts $cfgd(globals,misclogfilefd) "EOF - ${xcmd}";
			}
			if {$iflag == 0 && $ignoreeof == 0} {
				if {$vflag} { log_user 0; }
				return "-1 - hug_tl1_exec: (IP return) eof during cmd:\n$xcmd";
			}
		}
		}
	}
	-i $cfgd(tl1,$streamid,spawn_id) -re {M.*[0-9]+.*DENY.*;\r\nTelica>} {
		puts "\nDENY RECEIVED ...";
		if {[info exists cfgd(globals,denylogfilefd)]} {
			puts $cfgd(globals,denylogfilefd) "${xcmd}";
		}
		if {$iflag == 0} {
			if {$vflag} { log_user 0; }
			return "-1 - hug_tl1_exec: DENY during cmd:\n$xcmd";
		}
	}
	timeout {
		if {[info exists cfgd(globals,misclogfilefd)]} {
			puts $cfgd(globals,misclogfilefd) "TIME OUT - ${xcmd}";
		}
		if {$iflag == 0 && $ignoretmout == 0} {
			if {$vflag} { log_user 0; }
			return "-1 - hug_tl1_exec: timeout during cmd:\n$xcmd";
		}
	}
	eof {
		if {[info exists cfgd(globals,misclogfilefd)]} {
			puts $cfgd(globals,misclogfilefd) "EOF - ${xcmd}";
		}
		if {$iflag == 0 && $ignoreeof == 0} {
			if {$vflag} { log_user 0; }
			return "-1 - hug_tl1_exec: eof during cmd:\n$xcmd";
		}
	}
	}
	#
	if {$vflag} { log_user 0; }
	#
	return "0 - success";
}
#
proc hug_tl1_close { cfgdata streamid vflag tmout } {
	#
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		if {[info exists cfgd(globals,inproglogfilefd)]} {
			catch { close $cfgd(globals,inproglogfilefd); } ignore;
			unset cfgd(globals,inproglogfilefd);
		}
		if {[info exists cfgd(globals,denylogfilefd)]} {
			catch { close $cfgd(globals,denylogfilefd); } ignore;
			unset cfgd(globals,denylogfilefd);
		}
		if {[info exists cfgd(globals,misclogfilefd)]} {
			catch { close $cfgd(globals,misclogfilefd); } ignore;
			unset cfgd(globals,misclogfilefd);
		}
		return "0 - success";
	}
	if {$tmout != 0} { set timeout $tmout; }
	if {$vflag} { log_user 1; }
	#
	# set spawn_id $cfgd(tl1,$streamid,spawn_id);
	# catch { close $spawn_id; } ignore;
	hug_tl1_killstream cfgd $streamid;
	#
	if {$vflag} { log_user 0; }
	#
	if {[info exists cfgd(globals,inproglogfilefd)]} {
		catch { close $cfgd(globals,inproglogfilefd); } ignore;
		unset cfgd(globals,inproglogfilefd);
	}
	if {[info exists cfgd(globals,denylogfilefd)]} {
		catch { close $cfgd(globals,denylogfilefd); } ignore;
		unset cfgd(globals,denylogfilefd);
	}
	if {[info exists cfgd(globals,misclogfilefd)]} {
		catch { close $cfgd(globals,misclogfilefd); } ignore;
		unset cfgd(globals,misclogfilefd);
	}
	#
	return "0 - success";
}
#
proc hug_tl1_file { cfgdata streamid iflag vflag tmout filename } {
	upvar $cfgdata cfgd;
	#
	if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		return "-1 - hug_tl1_file: stream id $streamid does not exist.";
	} elseif {[string length $filename] <= 0} {
		return "-1 - hug_tl1_file: filename is null length.";
	} elseif {![file readable $filename]} {
		return "-1 - hug_tl1_file: file $filename is not readable.";
	} 
	#
	if {[catch {set fd [open $filename "r"]} status]} {
		return "-1 - hug_tl1_file: unable to open $filename for read: $status";
	}
	#
	while {[gets $fd inbuf] >= 0 && ![eof $fd]} {
		# skip comments and white-space
		switch -regexp -- ${inbuf} {
		{^[ \t]*$} { continue; }
		{^[ \t]*#} { continue; }
		}
		# strip leading white space
		set cmd [string trim $inbuf]
		# run cmd
		set status [hug_tl1_exec cfgd $streamid $iflag $vflag $tmout $cmd];
		if {[isNotOk $status]} {
			catch { close $fd; } ignore;
			return "-1 - hug_tl1_file: hug_tl1_exec failed: \n$status";
		}
	}
	#
	catch { close $fd; } ignore;
	return "0 - success";
}
#
proc hug_tl1_rstallioms { cfgdata streamid iflag vflag tmout } {
	upvar $cfgdata cfgd;
	#
	# if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		# return "-1 - hug_tl1_rstallioms: stream id $streamid does not exist.";
	# } 
	# set spawn_id $cfgd(tl1,$streamid,spawn_id);
	#
	set labid $cfgd(globals,labid);
	set spaip $cfgd(globals,spa,ip);
	set spbip $cfgd(globals,spb,ip);
	set tl1username $cfgd(globals,spa,tl1login);
	set tl1passwd $cfgd(globals,spa,tl1passwd);
	#
	# set status [hw_maint_rstallioms spawn_id];
	# if {[isNotOk $status]} {
		# return "-1 - hug_tl1_rstallioms: hw_maint_rstallioms failed: \n$status";
	# } else {
		# return "0 - success";
	# }
	set status [hw_maint_rst_all_ioms $labid $spaip $spbip $tl1username $tl1passwd];
	if {[isNotOk $status]} {
		if {${iflag}} {
			puts "\nhug_tl1_rstallioms: WARNING - hw_maint_rst_all_ioms failed: \n$status";
			return "0 - hug_tl1_rstallioms: WARNING - hw_maint_rst_all_ioms failed: \n$status";
		} else {
			return "-1 - hug_tl1_rstallioms: hw_maint_rst_all_ioms failed: \n$status";
		}
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_rmvallioms { cfgdata streamid iflag vflag tmout } {
	upvar $cfgdata cfgd;
	#
	# if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		# return "-1 - hug_tl1_rmvallioms: stream id $streamid does not exist.";
	# } 
	# set spawn_id $cfgd(tl1,$streamid,spawn_id);
	#
	set labid $cfgd(globals,labid);
	set spaip $cfgd(globals,spa,ip);
	set spbip $cfgd(globals,spb,ip);
	set tl1username $cfgd(globals,spa,tl1login);
	set tl1passwd $cfgd(globals,spa,tl1passwd);
	#
	# set status [hw_maint_rmvallioms spawn_id];
	# if {[isNotOk $status]} {
		# return "-1 - hug_tl1_rmvallioms: hw_maint_rmvallioms failed: \n$status";
	# } else {
		# return "0 - success";
	# }
	set status [hw_maint_rmv_all_ioms $labid $spaip $spbip $tl1username $tl1passwd];
	if {[isNotOk $status]} {
		if {${iflag}} {
			puts "\nhug_tl1_rmv_all_ioms: WARNING - hw_maint_rmv_all_ioms failed: \n$status";
			return "0 - hug_tl1_rmv_all_ioms: WARNING - hw_maint_rmv_all_ioms failed: \n$status";
		} else {
			return "-1 - hug_tl1_rmv_all_ioms: hw_maint_rmv_all_ioms failed: \n$status";
		}
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_cpymems { cfgdata streamid iflag vflag tmout maxcpymems } {
	upvar $cfgdata cfgd;
	#
	# if {![info exists cfgd(tl1,$streamid,spawn_id)]} {
		# return "-1 - hug_tl1_cpymems: stream id $streamid does not exist.";
	# } 
	# set spawn_id $cfgd(tl1,$streamid,spawn_id);
	#
	set labid $cfgd(globals,labid);
	set spaip $cfgd(globals,spa,ip);
	set spbip $cfgd(globals,spb,ip);
	set tl1username $cfgd(globals,spa,tl1login);
	set tl1passwd $cfgd(globals,spa,tl1passwd);
	set lynxusername $cfgd(globals,spa,login);
	set lynxpasswd $cfgd(globals,spa,passwd);
	set recoverioms 1;
	#
	# set status [hw_maint_cpymemallioms spawn_id $maxcpymems $recoverioms];
	# if {[isNotOk $status]} {
		# return "-1 - hug_tl1_cpymems: hw_maint_cpymemallioms failed: \n$status";
	# } else {
		# return "0 - success";
	# }
	set status [hw_maint_cpymem_all_ioms $labid $spaip $spbip $tl1username $tl1passwd $recoverioms $lynxusername $lynxpasswd $maxcpymems];
	if {[isNotOk $status]} {
		if {${iflag}} {
			puts "\nhug_tl1_cpymems: WARNING - hw_maint_cpymem_all_ioms failed: \n$status";
			return "0 - hug_tl1_cpymems: WARNING - hw_maint_cpymem_all_ioms failed: \n$status";
		} else {
			return "-1 - hug_tl1_cpymems: hw_maint_cpymem_all_ioms failed: \n$status";
		}
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_updateioms { cfgdata streamid iflag vflag tmout maxcpymems } {
	upvar $cfgdata cfgd;
	#
	set labid $cfgd(globals,labid);
	set spaip $cfgd(globals,spa,ip);
	set spbip $cfgd(globals,spb,ip);
	set tl1username $cfgd(globals,spa,tl1login);
	set tl1passwd $cfgd(globals,spa,tl1passwd);
	set lynxusername $cfgd(globals,spa,login);
	set lynxpasswd $cfgd(globals,spa,passwd);
	#
	set status [hw_maint_update_ioms $labid $spaip $spbip $tl1username $tl1passwd $lynxusername $lynxpasswd "*"];
	if {[isNotOk $status]} {
		if {${iflag}} {
			puts "\nhug_tl1_updateioms: WARNING - hw_maint_updateioms failed: \n$status";
			return "0 - hug_tl1_updateioms: WARNING - hw_maint_updateioms failed: \n$status";
		} else {
			return "-1 - hug_tl1_updateioms: hw_maint_updateioms failed: \n$status";
		}
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_rmvsiglnks { cfgdata streamid iflag vflag tmout iom } {
	upvar $cfgdata cfgd;
	#
	set labid $cfgd(globals,labid);
	#
	if {[info exist cfgd(globals,spa,ip)]} {
		set spaip $cfgd(globals,spa,ip);
	} elseif {[info exists cfgd(globals,spaip)]} {
		set spaip $cfgd(globals,spaip);
	} else {
		return "-1 - hug_tl1_rmvsiglnks: SP-A IP not defined";
	}
	if {[info exist cfgd(globals,spb,ip)]} {
		set spbip $cfgd(globals,spb,ip);
	} elseif {[info exists cfgd(globals,spbip)]} {
		set spbip $cfgd(globals,spbip);
	} else {
		return "-1 - hug_tl1_rmvsiglnks: SP-B IP not defined";
	}
	if {[info exists cfgd(globals,spa,tl1login)]} {
		set tl1username $cfgd(globals,spa,tl1login);
	} elseif {[info exists cfgd(globals,tl1login)]} {
		set tl1username $cfgd(globals,tl1login);
	} else {
		return "-1 - hug_tl1_rmvsiglnks: Tl1 login not defined";
	}
	if {[info exists cfgd(globals,spa,tl1passwd)]} {
		set tl1passwd $cfgd(globals,spa,tl1passwd);
	} elseif {[info exists cfgd(globals,tl1passwd)]} {
		set tl1passwd $cfgd(globals,tl1passwd);
	} else {
		return "-1 - hug_tl1_rmvsiglnks: Tl1 passwd not defined";
	}
	set ioms($iom) $iom;
	#
	set status [hw_maint_rmv_signaling_links $labid $spaip $spbip $tl1username $tl1passwd ioms];
	if {[isNotOk $status]} {
		return "-1 - hug_tl1_rmvsiglnks: hw_maint_rmv_signaling_links failed: \n$status";
	} else {
		return "0 - success";
	}
}
#
proc hug_tl1_rstsiglnks { cfgdata streamid iflag vflag tmout iom } {
	upvar $cfgdata cfgd;
	#
	set labid $cfgd(globals,labid);
	#
	if {[info exist cfgd(globals,spa,ip)]} {
		set spaip $cfgd(globals,spa,ip);
	} elseif {[info exists cfgd(globals,spaip)]} {
		set spaip $cfgd(globals,spaip);
	} else {
		return "-1 - hug_tl1_rstsiglnks: SP-A IP not defined";
	}
	if {[info exist cfgd(globals,spb,ip)]} {
		set spbip $cfgd(globals,spb,ip);
	} elseif {[info exists cfgd(globals,spbip)]} {
		set spbip $cfgd(globals,spbip);
	} else {
		return "-1 - hug_tl1_rstsiglnks: SP-B IP not defined";
	}
	if {[info exists cfgd(globals,spa,tl1login)]} {
		set tl1username $cfgd(globals,spa,tl1login);
	} elseif {[info exists cfgd(globals,tl1login)]} {
		set tl1username $cfgd(globals,tl1login);
	} else {
		return "-1 - hug_tl1_rstsiglnks: Tl1 login not defined";
	}
	if {[info exists cfgd(globals,spa,tl1passwd)]} {
		set tl1passwd $cfgd(globals,spa,tl1passwd);
	} elseif {[info exists cfgd(globals,tl1passwd)]} {
		set tl1passwd $cfgd(globals,tl1passwd);
	} else {
		return "-1 - hug_tl1_rstsiglnks: Tl1 passwd not defined";
	}
	set ioms($iom) $iom;
	#
	set status [hw_maint_rst_signaling_links $labid $spaip $spbip $tl1username $tl1passwd ioms];
	if {[isNotOk $status]} {
		return "-1 - hug_tl1_rstsiglnks: hw_maint_rst_signaling_links failed: \n$status";
	} else {
		return "0 - success";
	}
}
#
