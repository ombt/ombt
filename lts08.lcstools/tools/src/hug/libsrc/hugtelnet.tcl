# start up a telnet session and exec commands
package provide hugtelnet 1.0
#
proc hug_telnet_killstream { cfgdata streamid } {
	upvar $cfgdata cfgd;
	if {![info exists cfgd(telnet,$streamid,spawn_id)]} {
		return;
	}
	set spawn_id $cfgd(telnet,$streamid,spawn_id);
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; wait -nowait; } ignore;
	catch { unset cfgd(telnet,$streamid,spawn_id); } ignore;
	return;
}
#
proc hug_telnet_open { cfgdata streamid vflag tmout ip login passwd { maxlogons 5 } { sleeptime 10 } } {
	#
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	# sanity check of arguments
	#
	if {[info exists cfgd(telnet,$streamid,spawn_id)]} {
		return "-1 - hug_telnet_open: stream id $streamid already exists.";
	} elseif {[string length $ip] <= 0} {
		return "-1 - hug_telnet_open: ip is null length.";
	} elseif {[string length $login] <= 0} {
		return "-1 - hug_telnet_open: login is null length.";
	} elseif {[string length $passwd] <= 0} {
		return "-1 - hug_telnet_open: passwd is null length.";
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	# time out value
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	# startup telnet process.
	#
	set done 0;
	#
	for {set try 0} {$try<$maxlogons} {incr try} {
		spawn -noecho "/usr/bin/telnet" "-l $login" $ip;
		set cfgd(telnet,$streamid,spawn_id) $spawn_id;
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
			hug_telnet_killstream cfgd $streamid;
			sleep $sleeptime;
			continue;
		}
		eof {
			puts "\nEOF during telnet attempt. Retry.";
			hug_telnet_killstream cfgd $streamid;
			sleep $sleeptime;
			continue;
		}
		}
	}
	#
	if {!$done || ($try>=$maxlogons)} {
		return "-1 - hug_telnet_open: unable to telnet to machine ($ip,$login).";
	}
	#
	expect {
	-re ".*> " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re ".*# " {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		# catch { close $cfgd(telnet,$streamid,spawn_id); } ignore;
		hug_telnet_killstream cfgd $streamid;
		return "-1 - hug_telnet_open: time out during PS1 ($ip,$login).";
	}
	eof {
		# catch { close $cfgd(telnet,$streamid,spawn_id); } ignore;
		hug_telnet_killstream cfgd $streamid;
		return "-1 - hug_telnet_open: eof during PS1 ($ip,$login).";
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
		# catch { close $cfgd(telnet,$streamid,spawn_id); } ignore;
		hug_telnet_killstream cfgd $streamid;
		return "-1 - hug_telnet_open: time out after PS1 ($ip,$login).";
	}
	eof {
		# catch { close $cfgd(telnet,$streamid,spawn_id); } ignore;
		hug_telnet_killstream cfgd $streamid;
		return "-1 - hug_telnet_open: eof after PS1 ($ip,$login).";
	}
	};
	#
	set cfgd(telnet,$streamid,login) $login;
	set cfgd(telnet,$streamid,passwd) $passwd;
	#
	return "0 - success";
}
#
proc hug_telnet_exec { cfgdata streamid iflag vflag tmout xcmd { rflag 0 } } {
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	puts "\nhug_telnet_exec: reverse flag = $rflag";
	#
	if {![info exists cfgd(telnet,$streamid,spawn_id)]} {
		return "-1 - hug_telnet_exec: stream id $streamid does not exist.";
	} elseif {[string length $xcmd] <= 0} {
		return "-1 - hug_telnet_exec: cmd is null length.";
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	set timeout $tmout;
	#
	if {$vflag} {
		log_user 1;
	}
	#
	send -i $cfgd(telnet,$streamid,spawn_id) "$xcmd\r";
	expect {
	-i $cfgd(telnet,$streamid,spawn_id) -re {.*[\r\n]COMMAND_IS_DONE>} {
		send -i $cfgd(telnet,$streamid,spawn_id) "echo STATUS=$?\r";
	}
	timeout {
		if {$vflag} { log_user 0; }
		if {$iflag} {
			return "0 - success";
		} else {
			return "-1 - hug_telnet_exec: timeout during cmd:\n$xcmd";
		}
	}
	eof {
		if {$vflag} { log_user 0; }
		if {$iflag} {
			return "0 - success";
		} else {
			return "-1 - hug_telnet_exec: timeout during cmd:\n$xcmd";
		}
	}
	}
	expect {
	-i $cfgd(telnet,$streamid,spawn_id) -re {.*STATUS=([-0-9]+)[\r\n]*COMMAND_IS_DONE>} {
		puts "\nStatus is $expect_out(1,string)";
		set status $expect_out(1,string);
		if {!$iflag && [isNotOk $status]} {
			if {$vflag} { log_user 0; }
			if {$rflag == 0 } {
				return "-1 - hug_telnet_exec: cmd failed with status $status:\n$xcmd";
			} else {
				return "0 - hug_telnet_exec: cmd failed with status $status:\n$xcmd";
			}
		}
	}
	timeout {
		if {$vflag} { log_user 0; }
		if {$iflag} {
			return "0 - success";
		} else {
			return "-1 - hug_telnet_exec: timeout during cmd:\n$xcmd";
		}
	}
	eof {
		if {$vflag} { log_user 0; }
		if {$iflag} {
			return "0 - success";
		} else {
			return "-1 - hug_telnet_exec: timeout during cmd:\n$xcmd";
		}
	}
	}
	#
	if {$vflag} { log_user 0; }
	#
	if {$rflag == 0 } {
		return "0 - success";
	} else {
		return "-1 - hug_telnet_exec: failure because of reverse flag";
	}
}
#
proc hug_telnet_close { cfgdata streamid vflag tmout } {
	#
	upvar $cfgdata cfgd;
	#
	global spawn_id;
	global timeout;
	#
	if {![info exists cfgd(telnet,$streamid,spawn_id)]} {
		return "0 - success";
	}
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	# set spawn_id $cfgd(telnet,$streamid,spawn_id);
	# catch { close $spawn_id; } ignore;
	hug_telnet_killstream cfgd $streamid;
	#
	return "0 - success";
}
#
