# ftp files to and from.
package provide hugftp 1.0
#
proc hug_ftp_killstream { cfgdata streamid } {
	upvar $cfgdata cfgd;
	if {![info exists cfgd(ftp,$streamid,spawn_id)]} {
		puts "\nhug_ftp_killstream: WARNING, stream $streamid does not exist.";
		return;
	}
	set spawn_id $cfgd(ftp,$streamid,spawn_id);
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; wait -nowait; } ignore;
	catch { unset cfgd(ftp,$streamid,spawn_id); } ignore;
	puts "\nhug_ftp_killstream: WARNING, stream $streamid unset.";
	return;
}
#
proc hug_ftp_open { cfgdata streamid ip login passwd {vflag 0} {tmout 30} { sleeptime 10 } { maxlogons 10 } } {
	#
	upvar $cfgdata cfgd;
	#
	global timeout;
	#
	# sanity check of arguments
	#
	if {[info exists cfgd(ftp,$streamid,spawn_id)]} {
		return "-1 - hug_ftp_open: stream id $streamid already exists.";
	} elseif {[string length $ip] <= 0} {
		return "-1 - hug_ftp_open: ip is null length.";
	} elseif {[string length $login] <= 0} {
		return "-1 - hug_ftp_open: login is null length.";
	} elseif {[string length $passwd] <= 0} {
		return "-1 - hug_ftp_open: passwd is null length.";
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
	# startup ftp process.
	#
	set done 0;
	for {set try 0} {$try<$maxlogons} {incr try} {
		spawn -noecho "/usr/bin/ftp" $ip;
		set cfgd(ftp,$streamid,spawn_id) $spawn_id;
		#
		expect {
		"Name" {
			send "$login\r";
			set done 1;
			break;
		}
		timeout {
			puts "\nhug_ftp_open: time out during ftp ($ip,$login). Retry.";
			hug_ftp_killstream cfgd $streamid;
			sleep $sleeptime;
			continue;
		}
		eof {
			puts "\nhug_ftp_open: eof during ftp ($ip,$login). Retry.";
			hug_ftp_killstream cfgd $streamid;
			sleep $sleeptime;
			continue;
		}
		}
	}
	#
	if {$try>=$maxlogons || !$done} {
		return "-1 - hug_ftp_open: unable to ftp to ($ip,$login).";
	}
	#
	expect {
	"Password:" {
		send "$passwd\r";
	}
	timeout {
		return "-1 - hug_ftp_open: time out during login ($ip,$login).";
	}
	eof {
		return "-1 - hug_ftp_open: eof during login ($ip,$login).";
	}
	}
	#
	expect {
	-re {230.*ftp> } {
		# success. able to logon.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	timeout {
		return "-1 - hug_ftp_open: time out during passwd ($ip,$login).";
	}
	eof {
		return "-1 - hug_ftp_open: eof during passwd ($ip,$login).";
	}
	}
	#
	set cfgd(ftp,$streamid,login) $login;
	set cfgd(ftp,$streamid,passwd) $passwd;
	#
	return "0 - success";
}
#
proc hug_ftp_get { cfgdata streamid frompath topath {asciiflag 0} {binaryflag 1} {ignorefilenotfound 0} {vflag 0} {tmout 30} } {
	upvar $cfgdata cfgd;
	#
	global timeout;
	#
	# sanity check of arguments
	#
	if {![info exists cfgd(ftp,$streamid,spawn_id)]} {
		return "-1 - hug_ftp_get: stream id $streamid does not exist.";
	} elseif {[string length $frompath] <= 0} {
		return "-1 - hug_ftp_get: from-file path is null length.";
	} elseif {[string length $topath] <= 0} {
		return "-1 - hug_ftp_get: to-file path is null length.";
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	set spawn_id $cfgd(ftp,$streamid,spawn_id);
	#
	# binary or ascii?
	#
	log_user 1;
	if {$binaryflag} {
		send "binary\r";
	} elseif {$asciiflag} {
		send "ascii\r";
	} else {
		# default
		send "binary\r";
	}
	set timeout 120;
	expect {
	-re "200.*ftp> " {
		# ok
	}
	timeout {
		log_user 0;
		return "-1 - hug_ftp_get: time out during binary or ascii operation.";
	}
	eof {
		log_user 0;
		return "-1 - hug_ftp_get: eof during binary or ascii operation.";
	}
	}
	log_user 0;
	#
	# time out value
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	# get remote file
	#
	send "get $frompath $topath\r";
	expect {
	-re {226.*ftp> } {
		# success. able to get file.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_get: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_get: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		return "-1 - hug_ftp_get: time out during get ($frompath,$topath).";
	}
	eof {
		return "-1 - hug_ftp_get: eof during get ($frompath,$topath).";
	}
	}
	#
	if {![file exists $topath]} {
		return "-1 - hug_ftp_get: to-file $topath does not exist.";
	}
	#
	return "0 - success";
}
#
proc hug_ftp_put { cfgdata streamid frompath topath {asciiflag 0} {binaryflag 1} {ignorefilenotfound 0} {vflag 0} {tmout 30} } {
	upvar $cfgdata cfgd;
	#
	global timeout;
	#
	# sanity check of arguments
	#
	if {![info exists cfgd(ftp,$streamid,spawn_id)]} {
		return "-1 - hug_ftp_put: stream id $streamid does not exist.";
	} elseif {[string length $frompath] <= 0} {
		return "-1 - hug_ftp_put: from-file path is null length.";
	} elseif {[string length $topath] <= 0} {
		return "-1 - hug_ftp_put: to-file path is null length.";
	} elseif {![file exists $frompath]} {
		if {$ignorefilenotfound == 0} {
			return "-1 - hug_ftp_put: from-file $frompath does not exist.";
		} else {
			puts "WARNING: 0 - hug_ftp_put: from-file $frompath does not exist.";
			return "0 - hug_ftp_put: from-file $frompath does not exist.";
		}
	} elseif {$tmout < 0} {
		set tmout -1;
	}
	#
	set spawn_id $cfgd(ftp,$streamid,spawn_id);
	#
	# binary or ascii?
	#
	log_user 1;
	if {$binaryflag} {
		send "binary\r";
	} elseif {$asciiflag} {
		send "ascii\r";
	} else {
		# default
		send "binary\r";
	}
	set timeout 120;
	expect {
	-re "200.*ftp> " {
		# ok
	}
	timeout {
		log_user 0;
		return "-1 - hug_ftp_put: time out during binary or ascii operation.";
	}
	eof {
		log_user 0;
		return "-1 - hug_ftp_put: eof during binary or ascii operation.";
	}
	}
	log_user 0;
	#
	# time out value
	#
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	# put file
	#
	send "put $frompath $topath\r";
	expect {
	-re {226.*ftp> } {
		# success. able to put file.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_put: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - hug_ftp_put: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		return "-1 - hug_ftp_put: time out during put ($frompath,$topath).";
	}
	eof {
		return "-1 - hug_ftp_put: eof during put ($frompath,$topath).";
	}
	}
	#
	return "0 - success";
}
proc hug_ftp_close { cfgdata streamid {vflag 0} {tmout 30} } {
	#
	upvar $cfgdata cfgd;
	#
	global timeout;
	#
	if {![info exists cfgd(ftp,$streamid,spawn_id)]} {
		# return "-1 - hug_ftp_close: stream id $streamid does not exist.";
		return "0 - success";
	}
	if {$tmout != 0} {
		set timeout $tmout;
	}
	#
	# set spawn_id $cfgd(ftp,$streamid,spawn_id);
	# catch { close $spawn_id; } ignore;
	hug_ftp_killstream cfgd $streamid;
	#
	return "0 - success";
}
#

