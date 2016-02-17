# functions related to hardware maintenance
package provide nhwmaint 1.0
#
package require db
package require ncleis
package require flashparts
package require checkretval
package require iomips
package require iomfps
#
# clean up routine ...
#
proc hw_maint_close { spawn_id } {
	catch { send "exit\r"; sleep 0.5; } ignore;
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
	return;
}
#
proc hw_maint_close_all { spids } {
	upvar $spids spawn_ids;
	foreach resource [array names spawn_ids] {
		set spawn_id $spawn_ids($resource);
		hw_maint_close $spawn_id;
	}
	return;
}
#
# telnet interface 
#
proc hw_maint_telnet_open { spid spip username passwd { timeout 30 } { maxlogons 5 } } {
	upvar $spid spawn_id;
	#
	for {set i 0} {$i<$maxlogons} {incr i} {
		spawn -noecho "/usr/bin/telnet" $spip;
		#
		expect {
		"Connected to " {
			# ok
			break;
		}
		"Connection refused" {
			hw_maint_close $spawn_id;
			puts "\nConnection refused. Sleep and retry.";
			sleep 5;
		}
		timeout {
			hw_maint_close $spawn_id;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
		}
		eof {
			hw_maint_close $spawn_id;
			puts "\nEOF. Sleep and retry.";
			sleep 5;
		}
		}
	}
	if {$i>=$maxlogons} {
		puts "\nUnable to telnet to ($spip,$username).";
		return "-1 - hw_maint_telnet_open: unable to telnet to $spip";
	} else {
		puts "\nTelnet to ($spip,$username) succeeded.";
	}
	#
	expect {
	-re ".*login:" {
		send "$username\r";
	}
	-re ".*user name:" {
		send "$username\r";
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: username timeout for $spip";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: username eof for $spip";
	}
	}
	#
	expect {
	-re ".*\[Pp]assword:" {
		send "$passwd\r";
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: passwd timeout for $spip";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: passwd eof for $spip";
	}
	}
	#
	expect {
	-re "\[a-z]*\[0-9]+\[ab]#" {
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	-re "\[uU]nnamed_\[Ss]ystem#" {
		puts "WARNING: UNIX is active, but 'unnamed_system' prompt was received.";
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	-re ".*> " {
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	-re ".*\$ " {
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	-re ".*\# " {
		send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: PS1 timeout for $spip";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: PS1 eof for $spip";
	}
	}
	#
	expect {
	-re ".*COMMAND_IS_DONE_[exp_pid]>" {
		# ok
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: new PS1 timeout for $spip";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_telnet_open: new PS1 eof for $spip";
	}
	}
	#
	return "0 - success";
}
#
proc hw_maint_telnet_exec { spid cmd { timeout 60 } { verbose 1 } } {
	upvar ${spid} spawn_id;
	#
	log_user ${verbose};
	#
	send "${cmd}\r";
	#
	expect {
	-re ".*STATUS=(\[-0-9]+)\[\r\n]*COMMAND_IS_DONE_[exp_pid]>" {
		log_user 0;
		puts "\nStatus is $expect_out(1,string)";
		set status $expect_out(1,string);
		if {[isNotOk $status]} {
			return "-1 - hw_maint_telnet_exec: cmd failed with status $status:\n$cmd";
		} else {
			return "0 - success";
		}
	}
	-re ".*\[\r\n]COMMAND_IS_DONE_[exp_pid]>" {
		send "echo STATUS=$?\r";
		exp_continue;
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_telnet_exec: timeout during cmd:\n$cmd";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_telnet_exec: timeout during cmd:\n$cmd";
	}
	}
	#
	log_user 0;
	return "0 - success";
}
#
# ftp functions
#
proc hw_maint_ftp_open { spawn_id_name ip login passwd {tmout 30} { sleeptime 10 } { maxlogons 10 } } {
	#
	upvar $spawn_id_name spawn_id;
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
		#
		expect {
		"Name" {
			send "$login\r";
			set done 1;
			break;
		}
		timeout {
			puts "\nhw_maint_ftp_open: time out during ftp ($ip,$login). Retry.";
			ftp_close ftp_id;
			sleep $sleeptime;
			continue;
		}
		eof {
			puts "\nhw_maint_ftp_open: eof during ftp ($ip,$login). Retry.";
			ftp_close ftp_id;
			sleep $sleeptime;
			continue;
		}
		}
	}
	#
	if {$try>=$maxlogons || !$done} {
		return "-1 - hw_maint_ftp_open: unable to ftp to ($ip,$login).";
	}
	#
	expect {
	"Password:" {
		send "$passwd\r";
	}
	timeout {
		return "-1 - hw_maint_ftp_open: time out during login ($ip,$login).";
	}
	eof {
		return "-1 - hw_maint_ftp_open: eof during login ($ip,$login).";
	}
	}
	#
	expect {
	-re {230.*ftp> } {
		# success. able to logon.
	}
	-re {([0-9][0-9][0-9].*)ftp> } {
		# some type of error.
		return "-1 - hw_maint_ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - hw_maint_ftp_open: ftp error ($ip,$login): $expect_out(1,string)";
	}
	timeout {
		return "-1 - hw_maint_ftp_open: time out during passwd ($ip,$login).";
	}
	eof {
		return "-1 - hw_maint_ftp_open: eof during passwd ($ip,$login).";
	}
	}
	#
	return "0 - success";
}
#
proc hw_maint_ftp_get_file { ftp_idname frompath topath {binaryflag 1} {verbose 0} {tmout 30} } {
	upvar $ftp_idname spawn_id;
	#
	global timeout;
	log_user $verbose;
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
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: time out during binary or ascii operation.";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: eof during binary or ascii operation.";
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
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: time out during get ($frompath,$topath).";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: eof during get ($frompath,$topath).";
	}
	}
	#
	if {![file exists $topath]} {
		log_user 0;
		return "-1 - hw_maint_ftp_get_file: to-file $topath does not exist.";
	}
	#
	log_user 0;
	return "0 - success";
}
#
proc hw_maint_ftp_put_file { ftp_idname frompath topath {binaryflag 1} {tmout 30} } {
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
		return "-1 - hw_maint_ftp_put_file: time out during binary or ascii operation.";
	}
	eof {
		return "-1 - hw_maint_ftp_put_file: eof during binary or ascii operation.";
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
		return "-1 - hw_maint_ftp_put_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	-re {(.*)ftp> } {
		# some type of error.
		return "-1 - hw_maint_ftp_put_file: ftp error ($frompath,$topath): $expect_out(1,string)";
	}
	timeout {
		return "-1 - hw_maint_ftp_put_file: time out during put ($frompath,$topath).";
	}
	eof {
		return "-1 - hw_maint_ftp_put_file: eof during put ($frompath,$topath).";
	}
	}
	#
	return "0 - success";
}
#
proc hw_maint_sfui { spid iom { timeout 30 } } {
	upvar ${spid} spawn_id;
	#
	log_user 1;
	send "/Telica/swCPU/CurrRel/system/sfui\r";
	#
	expect {
	-re {sfui> } {
		send "port ${iom} set\r";
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_sfui: timeout during sfui.";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_sfui: eof during sfui.";
	}
	}
	#
	log_user 1;
	expect {
	-re {sfui> } {
		send "mlw ${iom} 0x50000030 0x2\r";
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_sfui: timeout during port.";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_sfui: eof during port.";
	}
	}
	log_user 1;
	expect {
	-re {sfui> } {
		send "quit\r";
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_sfui: timeout during mlw.";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_sfui: eof during mlw.";
	}
	}
	log_user 1;
	expect {
	-re ".*\[\r\n]COMMAND_IS_DONE_[exp_pid]>" {
		# ok;
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_sfui: timeout during quit";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_sfui: eof during quit.";
	}
	}
	#
	log_user 0;
	puts "";
	return "0 - success";
}
#
# exit an tl1 session using canc-user;
#
proc hw_maint_canc_user { spid } {
	# upvar $spid spawn_id;
	#
	global timeout;
	set savetimeout $timeout;
	set timeout 1;
	log_user 0;
	#
	catch {
		send -i $spid "CANC-USER;\r";
		expect -i $spid {
		-re ";\r\nTelica>" { }
		timeout { }
		eof { }
	} } ignore;
	#
	# log_user 0;
	set timeout $savetimeout;
	#
	return "0 - success";
}
#
# determine active SP and open a TL1 session.
#
proc hw_maint_tl1_open { spid spaip spbip username passwd { tmout 15 } {maxlogons 5 } { tl1port 2361 } } {
	upvar $spid spawn_id;
	#
	# try both IPs to find active SP.
	#
	set foundactive 0;
	#
	log_user 1;
	# exp_internal 1;
	#
	foreach spip [list $spaip $spbip] {
		puts "\nTrying to telnet to IP $spip.";
		#
		for {set i 0} {$i<$maxlogons} {incr i} {
			spawn -noecho "/usr/bin/telnet" $spip $tl1port;
			#
			log_user 1;
			expect {
			"Connected to " {
				# ok
				break;
			}
			"Connection refused" {
				hw_maint_close $spawn_id;
				puts "\nConnection refused. Sleep and retry.";
				sleep 5;
			}
			timeout {
				hw_maint_close $spawn_id;
				puts "\nTime out. Sleep and retry.";
				sleep 5;
			}
			eof {
				hw_maint_close $spawn_id;
				puts "\nEOF. Sleep and retry.";
				sleep 5;
			}
			}
		}
		if {$i>=$maxlogons} {
			puts "\nUnable to telnet to ($spip,$username).";
			continue;
		} else {
			puts "\nTelnet to ($spip,$username) succeeded.";
		}
		#
		sleep 2;
		#
		log_user 1;
		send "\r";
		expect {
		"Telica>" {
			# ok
		}
		timeout {
			hw_maint_close $spawn_id;
			puts "\nTimeout waiting for Telica prompt ($spip,$username).";
			continue;
		}
		eof {
			hw_maint_close $spawn_id;
			puts "\nEOF waiting for Telica prompt ($spip,$username).";
			continue;
		}
		}
		#
		log_user 1;
		send "act-user::${username}:::${passwd};\r";
		#
		expect {
		-re "This is the WORKING System Processor.*M.*\[0-9]+.*COMPLD.*;\r\nTelica>" {
			puts "\nCOMPLETED received for ($spip,$username).";
			set foundactive 1;
			break;
		}
		-re "Host is in Standby Mode.*;\r\nTelica>" {
			puts "\nHost is in Standby Mode ($spip,$username).";
			hw_maint_canc_user $spawn_id;
			hw_maint_close $spawn_id;
			continue;
		}
		-re "This is the PROTECTION System Processor.*;\r\nTelica>" {
			puts "\nThis is the PROTECTION System Processor ($spip,$username).";
			hw_maint_canc_user $spawn_id;
			hw_maint_close $spawn_id;
			continue;
		}
		-re "Login Not Active.*;\r\nTelica>" {
			puts "\nLogin Not Active ($spip,$username).";
			hw_maint_close $spawn_id;
			continue;
		}
		-re "Can't login.*;\r\nTelica>" {
			puts "\nCan't login ($spip,$username).";
			hw_maint_close $spawn_id;
			continue;
		}
		-re ".*;\r\nTelica>" {
			puts "\nUnknown error ($spip,$username).";
			hw_maint_close $spawn_id;
			continue;
		}
		timeout {
			puts "\ntimeout waiting for a response ...";
			hw_maint_close $spawn_id;
			continue;
		}
		eof {
			puts "\neof waiting for a response ...";
			hw_maint_close $spawn_id;
			continue;
		}
		}
	}
	#
	log_user 0;
	# exp_internal 0;
	# check if were able to logon TL1.
	if {$foundactive} {
		return "0 - success";
	} else {
		return "-1 - hw_maint_tl1_open: unable to logon TL1";
	}
}
#
proc hw_maint_tl1_open_silent { spid spaip spbip username passwd silent { tmout 15 } {maxlogons 5 } { tl1port 2361 } } {
	upvar $spid spawn_id;
	#
	# try both IPs to find active SP.
	#
	set foundactive 0;
	#
	# log_user 1;
	# exp_internal 1;
	#
	foreach spip [list $spaip $spbip] {
		if {!${silent}} {
			puts "\nTrying to telnet to IP $spip.";
		}
		#
		for {set i 0} {$i<$maxlogons} {incr i} {
			spawn -noecho "/usr/bin/telnet" $spip $tl1port;
			#
			# log_user 1;
			expect {
			"Connected to " {
				# ok
				break;
			}
			"Connection refused" {
				hw_maint_close $spawn_id;
				if {!${silent}} {
					puts "\nConnection refused. Sleep and retry.";
				}
				sleep 5;
			}
			timeout {
				hw_maint_close $spawn_id;
				if {!${silent}} {
					puts "\nTime out. Sleep and retry.";
				}
				sleep 5;
			}
			eof {
				hw_maint_close $spawn_id;
				if {!${silent}} {
					puts "\nEOF. Sleep and retry.";
				}
				sleep 5;
			}
			}
		}
		if {$i>=$maxlogons} {
			if {!${silent}} {
				puts "\nUnable to telnet to ($spip,$username).";
			}
			continue;
		} else {
			if {!${silent}} {
				puts "\nTelnet to ($spip,$username) succeeded.";
			}
		}
		#
		sleep 2;
		#
		# log_user 1;
		send "\r";
		expect {
		"Telica>" {
			# ok
		}
		timeout {
			hw_maint_close $spawn_id;
			if {!${silent}} {
				puts "\nTimeout waiting for Telica prompt ($spip,$username).";
			}
			continue;
		}
		eof {
			hw_maint_close $spawn_id;
			if {!${silent}} {
				puts "\nEOF waiting for Telica prompt ($spip,$username).";
			}
			continue;
		}
		}
		#
		# log_user 1;
		send "act-user::${username}:::${passwd};\r";
		#
		expect {
		-re "This is the WORKING System Processor.*M.*\[0-9]+.*COMPLD.*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nCOMPLETED received for ($spip,$username).";
			}
			set foundactive 1;
			break;
		}
		-re "Host is in Standby Mode.*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nHost is in Standby Mode ($spip,$username).";
			}
			hw_maint_canc_user $spawn_id;
			hw_maint_close $spawn_id;
			continue;
		}
		-re "This is the PROTECTION System Processor.*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nThis is the PROTECTION System Processor ($spip,$username).";
			}
			hw_maint_canc_user $spawn_id;
			hw_maint_close $spawn_id;
			continue;
		}
		-re "Login Not Active.*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nLogin Not Active ($spip,$username).";
			}
			hw_maint_close $spawn_id;
			continue;
		}
		-re "Can't login.*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nCan't login ($spip,$username).";
			}
			hw_maint_close $spawn_id;
			continue;
		}
		-re ".*;\r\nTelica>" {
			if {!${silent}} {
				puts "\nUnknown error ($spip,$username).";
			}
			hw_maint_close $spawn_id;
			continue;
		}
		timeout {
			if {!${silent}} {
				puts "\ntimeout waiting for a response ...";
			}
			hw_maint_close $spawn_id;
			continue;
		}
		eof {
			if {!${silent}} {
				puts "\neof waiting for a response ...";
			}
			hw_maint_close $spawn_id;
			continue;
		}
		}
	}
	#
	# log_user 0;
	# exp_internal 0;
	# check if were able to logon TL1.
	if {$foundactive} {
		return "0 - success";
	} else {
		return "-1 - hw_maint_tl1_open_silent: unable to logon TL1";
	}
}

#
# get chassis data via rtrv-chassis-eqpt
#
proc hw_maint_rtrv_chassis_eqpt { spid dataname } {
	upvar $spid spawn_id;
	upvar $dataname data;
	#
	log_user 1;
	# exp_internal 1;
	#
	# set default values.
	#
	set data(chassis,shelfid) "unknown";
	set data(chassis,chassistype) "unknown";
	set data(chassis,mgtype) "unknown";
	set data(chassis,sgtype) "unknown";
	#
	send "RTRV-CHASSIS-EQPT;\r";
	#
	expect {
	-re {SHELFID=([^,]*),.*CHASSISTYPE=([^,]*),.*MGTYPE=([^,]*),.*SGTYPE=([^,]*),[^"]*"} {
		set shelfid $expect_out(1,string);
		set chassistype [string tolower $expect_out(2,string)];
		set mgtype [string tolower $expect_out(3,string)];
		set sgtype [string tolower $expect_out(4,string)];
		#
		puts "";
		puts "CHASSIS SHELFID: $shelfid ...";
		puts "CHASSIS CHASSIS TYPE: $chassistype ...";
		puts "CHASSIS MG TYPE: $mgtype ...";
		puts "CHASSIS SG TYPE: $sgtype ...";
		#
		set data(chassis,shelfid) $shelfid;
		set data(chassis,chassistype) $chassistype;
		set data(chassis,mgtype) $mgtype;
		set data(chassis,sgtype) $sgtype;
		#
		log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		# done
		puts "";
		puts "RTRV-CHASSIS-EQPT Done.";
	}
	timeout {
		puts "";
		puts "RTRV-CHASSIS-EQPT timeout";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_chassis_eqpt: RTRV-CHASSIS-EQPT timeout";
	}
	eof {
		puts "";
		puts "RTRV-CHASSIS-EQPT EOF";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_chassis_eqpt: RTRV-CHASSIS-EQPT EOF";
	}
	}
	#
	# exp_internal 0;
	log_user 0;
	#
	return "0 - success";
}
#
# get chassis data via rtrv-eqpt
#
proc hw_maint_rtrv_eqpt { spid dname } {
	upvar $spid spawn_id;
	upvar $dname data;
	#
	log_user 1;
	# exp_internal 1;
	#
	send "RTRV-EQPT;\r";
	#
	expect {
	-re "IOM-(\[0-9]+):(\[^,]*),(\[^,]*),\[^,]*,\[^,]*,\[^,]*,(\[^,]*):(\[A-Z0-9,&-]+)\"" {
		set iom $expect_out(1,string);
		set clei [string tolower $expect_out(2,string)];
		set protection [string tolower $expect_out(3,string)];
		set version $expect_out(4,string);
		#
		puts "";
		puts "IOM-$iom $expect_out(2,string) ...";
		puts "IOM-$iom $expect_out(3,string) ...";
		puts "IOM-$iom $expect_out(4,string) ...";
		puts "IOM-$iom $expect_out(5,string) ...";
		#
		regsub -all -- "\[-,&]" $expect_out(5,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		set clei [string tolower $clei];
		set protection [string tolower $protection];
		set version [string tolower $version];
		#
		set data(iom,$iom,state) $iomstate;
		set data(iom,$iom,clei) $clei;
		set data(iom,$iom,protection) $protection;
		set data(iom,$iom,version) $version;
		log_user 1;
		exp_continue;
	}
	-re "SP-A:\[^\"]*:(IS,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,a,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re "SP-A:\[^\"]*:(OOS,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,a,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re "SP-A:\[^\"]*:(OOS-(AU|MA)*,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,a,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re "SP-B:\[^\"]*:(IS,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-B $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,b,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re "SP-B:\[^\"]*:(OOS,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-B $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,b,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re "SP-B:\[^\"]*:(OOS-(AU|MA)*,(\[A-Z&]+))\"" {
		puts "";
		puts "SP-B $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,b,state) $spstate;
		log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		# done
		puts "";
		puts "RTRV-EQPT Done.";
	}
	timeout {
		puts "";
		puts "RTRV-EQPT timeout";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_eqpt: RTRV-EQPT timeout";
	}
	eof {
		puts "";
		puts "RTRV-EQPT EOF";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_eqpt: RTRV-EQPT EOF";
	}
	}
	#
	# exp_internal 0;
	log_user 0;
	#
	return "0 - success";
}
#
# set to a standard value
#
proc settodefault { varname { defaultvalue "none" } } {
	upvar $varname var;
	#
	if {[info exists var]} {
		# puts "\nsettodefault BEFORE: ${var}";
		if {${var} == "UNPROV" || ${var} == "unprov"} {
			set var ${defaultvalue};
		} elseif {${var} == "NULL" || ${var} == "null"} {
			set var ${defaultvalue};
		} elseif {[string length "${var}"] <= 0} {
			set var ${defaultvalue};
		}
		# puts "settodefault AFTER: ${var}";
	}
	#
	return "0 - success";
}
#
# get chassis data via rtrv-eqpt-all
#
proc hw_maint_rtrv_eqpt_all { spid dname } {
	upvar $spid spawn_id;
	upvar $dname data;
	#
	# log_user 1;
	# exp_internal 1;
	#
	send "RTRV-EQPT-ALL;\r";
	#
	expect {
	-re {IOM-([0-9]+):IOMODULETYPE=([^,]*),REDUNDANCY=([^,]*),MEMTHRESHOLD=([^,]*),NVMEMTHRESHOLD=([^,]*),BUSYTHRESHOLD=([^,]*),SWVERSION=([^,]*),REARMODULETYPE=([^,]*),APPSWVERSION=([^,]*):([A-Z0-9,&-]+)"} {
		set iom $expect_out(1,string);
		set clei [string tolower $expect_out(2,string)];
		set protection [string tolower $expect_out(3,string)];
		set version $expect_out(7,string);
		set rearclei [string tolower $expect_out(8,string)];
		#
		settodefault rearclei;
		#
		puts "";
		puts "IOM-$iom $expect_out(2,string) ...";
		puts "IOM-$iom $expect_out(3,string) ...";
		puts "IOM-$iom $expect_out(4,string) ...";
		puts "IOM-$iom $expect_out(5,string) ...";
		puts "IOM-$iom $expect_out(6,string) ...";
		puts "IOM-$iom $expect_out(7,string) ...";
		puts "IOM-$iom $expect_out(8,string) ...";
		puts "IOM-$iom $expect_out(9,string) ...";
		puts "IOM-$iom $expect_out(10,string) ...";
		#
		regsub -all -- "\[-,&]" $expect_out(10,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		#
		set data(iom,$iom,state) $iomstate;
		set data(iom,$iom,clei) $clei;
		set data(iom,$iom,rearclei) $rearclei;
		set data(iom,$iom,protection) $protection;
		set data(iom,$iom,version) $version;
		# log_user 1;
		exp_continue;
	}
	-re {IOM-([0-9]+):IOMODULETYPE=([^,]*),REDUNDANCY=([^,]*),MEMTHRESHOLD=([^,]*),NVMEMTHRESHOLD=([^,]*),BUSYTHRESHOLD=([^,]*),SWVERSION=([^,]*),REARMODULETYPE=([^,]*):([A-Z0-9,&-]+)"} {
		set iom $expect_out(1,string);
		set clei [string tolower $expect_out(2,string)];
		set protection [string tolower $expect_out(3,string)];
		set version $expect_out(7,string);
		set rearclei [string tolower $expect_out(8,string)];
		#
		settodefault rearclei;
		#
		puts "";
		puts "IOM-$iom $expect_out(2,string) ...";
		puts "IOM-$iom $expect_out(3,string) ...";
		puts "IOM-$iom $expect_out(4,string) ...";
		puts "IOM-$iom $expect_out(5,string) ...";
		puts "IOM-$iom $expect_out(6,string) ...";
		puts "IOM-$iom $expect_out(7,string) ...";
		puts "IOM-$iom $expect_out(8,string) ...";
		puts "IOM-$iom $expect_out(9,string) ...";
		#
		regsub -all -- "\[-,&]" $expect_out(9,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		#
		set data(iom,$iom,state) $iomstate;
		set data(iom,$iom,clei) $clei;
		set data(iom,$iom,rearclei) $rearclei;
		set data(iom,$iom,protection) $protection;
		set data(iom,$iom,version) $version;
		# log_user 1;
		exp_continue;
	}
	-re {SP-A:[^"]*:([A-Z0-9,&-]+)"} {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,a,state) $spstate;
		# log_user 1;
		exp_continue;
	}
	-re {SP-B:[^"]*:([A-Z0-9,&-]+)"} {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		set data(sp,b,state) $spstate;
		# log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		# done
		puts "";
		puts "RTRV-EQPT-ALL Done.";
	}
	timeout {
		puts "";
		puts "RTRV-EQPT-ALL timeout";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_eqpt_all: RTRV-EQPT-ALL timeout";
	}
	eof {
		puts "";
		puts "RTRV-EQPT EOF";
		# exp_internal 0;
		log_user 0;
		return "-1 - hw_maint_rtrv_eqpt_all: RTRV-EQPT-ALL EOF";
	}
	}
	#
	# exp_internal 0;
	log_user 0;
	#
	return "0 - success";
}
#
# retrieve all alarms
#
proc hw_maint_rtrv_alm_all { spid } {
	upvar $spid spawn_id;
	#
	log_user 1;
	catch {
		send "RTRV-ALM-ALL;\r";
		expect {
		-re ";\r\nTelica>" { }
		timeout { }
		eof { }
	} } ignore;
	log_user 0;
	#
	return "0 - success";
}
#
# restore signaling links on given list of IOMs
#
proc hw_maint_rst_signaling_links { labid spaip spbip username passwd iomsname { timeout 60 } } {
	upvar $iomsname ioms;
	# start up
	puts "\nStart ${labid} Remove Signaling Links:";
	# open active TL1 port 
	puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
	set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
	if {[isNotOk $status]} {
		return "-1 - hw_maint_rst_signaling_links: hw_maint_tl1_open failed:\n${status}";
	}
	set spawn_id $spid;
	# get IOMs for all signaling links.
	log_user 1;
	#
	send "RTRV-SLK;\r";
	#
	expect {
	-re {"([0-9]+):[^"]*TERMID=([^,]*),[^"]*"} {
		set slk $expect_out(1,string);
		set termid $expect_out(2,string);
		set slks($slk) ${termid};
		log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		puts "";
		puts "RTRV-SLK Done.";
	}
	timeout {
		puts "";
		puts "RTRV-SLK timeout";
		log_user 0;
		hw_maint_close $spid;
		return "-1 - hw_maint_rst_signaling_links: RTRV-SLK timeout";
	}
	eof {
		puts "";
		puts "RTRV-SLK EOF";
		log_user 0;
		hw_maint_close $spid;
		return "-1 - hw_maint_rst_signaling_links: RTRV-SLK EOF";
	}
	}
	log_user 0;
	#
	if {![info exists slks]} {
		puts "\nNo SLKS found for LABID ${labid}.";
		hw_maint_close $spid;
		return "0 - success";
	}
	#
	foreach slk [array names slks] {
		set data [split $slks($slk) "/"];
		set iom [lindex $data 1];
		puts "\nSLK: ${slk} ==>> TERMID: $slks($slk) ==>> IOM: ${iom}";
		lappend iom2slk(${iom}) ${slk};
	}
	#
	if {[info exist ioms(all)]} {
		set iomlist [array names iom2slk];
	} else {
		set iomlist [array names ioms];
	}
	#
	foreach iom ${iomlist} {
		# check if IOM has any SLKs.
		if {![info exists iom2slk(${iom})]} {
			puts "\nNo SLKs on IOM ${iom}. Skipping it.";
			continue;
		}
		puts "\nRestoring SLKs on IOM ${iom}: $iom2slk(${iom})";
		# remove slks
		foreach slk $iom2slk(${iom}) {
			set iscmd "ED-STATE-SGSLK::${slk}:::IS";
			#
			log_user 1;
			send "${iscmd};\r";
			#
			expect {
			-re ";\r\nTelica>" {
				puts "";
				puts "${iscmd} Done.";
			}
			timeout {
				puts "";
				puts "${iscmd} timeout";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - hw_maint_rst_signaling_links: ${iscmd} timeout";
			}
			eof {
				puts "";
				puts "${iscmd} EOF";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - rst_signaling_links: ${iscmd} EOF";
			}
			}
			log_user 0;
		}
	}
	#
	hw_maint_close $spid;
	return "0 - success";
}
#
# remove signaling links on given list of IOMs
#
proc hw_maint_rmv_signaling_links { labid spaip spbip username passwd iomsname { timeout 60 } } {
	upvar $iomsname ioms;
	# start up
	puts "\nStart ${labid} Remove Signaling Links:";
	# open active TL1 port 
	puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
	set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
	if {[isNotOk $status]} {
		return "-1 - hw_maint_rmv_signaling_links: hw_maint_tl1_open failed:\n${status}";
	}
	set spawn_id $spid;
	# get IOMs for all signaling links.
	log_user 1;
	#
	send "RTRV-SLK;\r";
	#
	expect {
	-re {"([0-9]+):[^"]*TERMID=([^,]*),[^"]*"} {
		set slk $expect_out(1,string);
		set termid $expect_out(2,string);
		set slks($slk) ${termid};
		log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		puts "";
		puts "RTRV-SLK Done.";
	}
	timeout {
		puts "";
		puts "RTRV-SLK timeout";
		log_user 0;
		hw_maint_close $spid;
		return "-1 - rmv_signaling_links: RTRV-SLK timeout";
	}
	eof {
		puts "";
		puts "RTRV-SLK EOF";
		log_user 0;
		hw_maint_close $spid;
		return "-1 - rmv_signaling_links: RTRV-SLK EOF";
	}
	}
	log_user 0;
	#
	if {![info exists slks]} {
		puts "\nNo SLKS found for LABID ${labid}.";
		hw_maint_close $spid;
		return "0 - success";
	}
	#
	foreach slk [array names slks] {
		set data [split $slks($slk) "/"];
		set iom [lindex $data 1];
		puts "\nSLK: ${slk} ==>> TERMID: $slks($slk) ==>> IOM: ${iom}";
		lappend iom2slk(${iom}) ${slk};
	}
	#
	if {[info exist ioms(all)]} {
		set iomlist [array names iom2slk];
	} else {
		set iomlist [array names ioms];
	}
	#
	foreach iom ${iomlist} {
		# check if IOM has any SLKs.
		if {![info exists iom2slk(${iom})]} {
			puts "\nNo SLKs on IOM ${iom}. Skipping it.";
			continue;
		}
		puts "\nRemoving SLKs on IOM ${iom}: $iom2slk(${iom})";
		# remove slks
		foreach slk $iom2slk(${iom}) {
			set inhcmd "ED-STATE-SGSLK::${slk}:::INH";
			set rtrvcmd "RTRV-STATUS-SGSLK::${slk}";
			#
			set ok 0;
			log_user 1;
			send "${inhcmd};\r";
			#
			expect {
			-re {M  *[0-9]+  *DENY.*\*/[^;]*;} {
				puts "";
				puts "${inhcmd} DENY; continue to place SLK in OOS state.";
				set ok 0;
			}
			-re ";\r\nTelica>" {
				puts "";
				puts "${inhcmd} Done.";
				set ok 1;
			}
			timeout {
				puts "";
				puts "${inhcmd} timeout";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - hw_maint_rmv_signaling_links: ${inhcmd} timeout";
			}
			eof {
				puts "";
				puts "${inhcmd} EOF";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - rmv_signaling_links: ${inhcmd} EOF";
			}
			}
			# check the status 
			send "${rtrvcmd};\r";
			#
			expect {
			-re ";\r\nTelica>" {
				puts "";
				puts "${rtrvcmd} Done.";
			}
			timeout {
				puts "";
				puts "${rtrvcmd} timeout - ignore";
			}
			eof {
				puts "";
				puts "${rtrvcmd} EOF - ignore";
			}
			}
			log_user 0;
			#
			if {$ok} {
				# SLK was inhibited, no need to OOS.
				continue;
			}
			#
			set ooscmd "ED-STATE-SGSLK::${slk}:::OOS";
			#
			log_user 1;
			send "${ooscmd};\r";
			#
			expect {
			-re ";\r\nTelica>" {
				puts "";
				puts "${ooscmd} Done.";
			}
			timeout {
				puts "";
				puts "${ooscmd} timeout";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - hw_maint_rmv_signaling_links: ${ooscmd} timeout";
			}
			eof {
				puts "";
				puts "${ooscmd} EOF";
				log_user 0;
				hw_maint_close $spid;
				return "-1 - rmv_signaling_links: ${ooscmd} EOF";
			}
			}
			# check the status 
			send "${rtrvcmd};\r";
			#
			expect {
			-re ";\r\nTelica>" {
				puts "";
				puts "${rtrvcmd} Done.";
			}
			timeout {
				puts "";
				puts "${rtrvcmd} timeout - ignore";
			}
			eof {
				puts "";
				puts "${rtrvcmd} EOF - ignore";
			}
			}
			log_user 0;
		}
	}
	#
	hw_maint_close $spid;
	return "0 - success";
}
#
# remove all IOMs
#
proc hw_maint_rmv_all_ioms { labid spaip spbip username passwd { maxiom 17 } { polltime 30 } { maxtime 1200 } { maxresets 5 } } {
	# init some data
	puts "\nInitializing ${labid} RMV-EQPT data.";
	for {set iom 1} {$iom<=$maxiom} {incr iom} {
		set data(iom,$iom,rmvstate) "NOTSTARTED";
		set data(iom,$iom,resetcounter) 0;
	}
	# maximum iterations to try
	set maxiters [expr ${maxtime}/${polltime}];
	# remove the IOMs
	puts "\nStart ${labid} RMV-EQPT cycle.";
	for {set done 0} { ! $done && $maxiters>0 } {sleep $polltime} {
		# counters
		set prirmvsinprog 0;
		set secrmvsinprog 0;
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip,$maxiters).";
		set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_rmv_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# rtrv-eqpt to get status
		puts "\nCall RTRV-EQPT to get IOM statuses.";
		set status [hw_maint_rtrv_eqpt_all spid data];
		if {[isNotOk $status]} {
			puts "\nERROR: hw_maint_rmv_all_ioms: hw_maint_rtrv_eqpt_all failed:\n${status}";
			puts "\nRe-opening TL1 session for ($labid,$spaip,$spbip).";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# remove any secondary IOMs
		puts "\nHandle RMV-EQPT of ${labid} Secondary IOMs.";
		for {set iom 1} {$iom<=$maxiom} {incr iom} {
			# check if IOM exists and is secondary
			if {![info exists data(iom,$iom,protection)]} {
				continue;
			} elseif {$data(iom,$iom,protection) != "sec"} {
				continue;
			}
			# check state of remove
			puts "\nChecking secondary IOM-$iom.";
			if {$data(iom,$iom,rmvstate) == "NOTSTARTED"} {
				# check if IOM is already OOS
				if {$data(iom,$iom,state) == "oosma"} {
					puts "\nIOM-$iom already OOS.";
					set data(iom,$iom,rmvstate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nIOM-$iom already OOSAUFLT.";
					set data(iom,$iom,rmvstate) "DONE";
					continue;
				}
				#
				log_user 1;
				puts "\nStart RMV-EQPT of IOM-$iom.";
				send "RMV-EQPT::IOM-$iom;\r";
				expect {
				-re "DENY.*Not in Valid State.*;\r\nTelica>" {
					puts "\nRMV-EQPT::IOM-$iom NOT-IN-VALID STATE. Retry.";
					incr data(iom,$iom,resetcounter);
					if {$data(iom,$iom,resetcounter) > $maxresets} {
						puts "\nERROR: Maximum attempts of RMV-EQPT::IOM-$iom exceeded.";
						# try to get alarms
						log_user 1;
						catch {
						send "RTRV-ALM-ALL;\r";
						expect {
						-re ";\r\nTelica>" { }
						timeout { }
						eof { }
						} } ignore;
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rmv_all_ioms: Maximum attempts of RMV-EQPT::IOM-$iom exceeded.";
					}
					# increment secrmvsinprog so we retry
					# again. see below.
				}
				-re ";\r\nTelica>" {
					puts "\nRMV-EQPT::IOM-$iom COMPLETED";
					set data(iom,$iom,rmvstate) "INPROG";
				}
				timeout {
					puts "\nRMV-EQPT::IOM-$iom timeout";
					set data(iom,$iom,rmvstate) "INPROG";
				}
				eof {
					puts "\nERROR: RMV-EQPT::IOM-$iom EOF";
					log_user 0;
					hw_maint_close $spid;
					return "-1 - hw_maint_rmv_all_ioms: RMV-EQPT::IOM-$iom EOF";
				}
				}
				incr secrmvsinprog;
				log_user 0;
			} elseif {$data(iom,$iom,rmvstate) == "INPROG"} {
				if {$data(iom,$iom,state) == "oosma"} {
					puts "\nFinished RMV-EQPT (oosma) of IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosaumaflt"} {
					puts "\nFinished RMV-EQPT of (oosaumaflt) IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nFinished RMV-EQPT of (oosauflt) IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} else {
					puts "\nIn Prog RMV-EQPT of IOM-$iom.";
					incr secrmvsinprog;
				}
			} elseif {$data(iom,$iom,rmvstate) == "DONE"} {
				# ok
				puts "\nDone RMV-EQPT of IOM-$iom.";
			}
		}
		#
		if {$secrmvsinprog > 0} {
			# let secondary removes finish before starting
			# to remove primaries.
			puts "\nSecondary RMV-EQPT still in prog ... skip primary for now.";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# remove any primary IOMs
		puts "\nHandle RMV-EQPT of ${labid} Primary IOMs.";
		for {set iom 1} {$iom<=$maxiom} {incr iom} {
			# check if IOM exists and is primary
			if {![info exists data(iom,$iom,protection)]} {
				continue;
			} elseif {$data(iom,$iom,protection) != "pri"} {
				continue;
			}
			# check state of remove
			puts "\nChecking primary IOM-$iom.";
			if {$data(iom,$iom,rmvstate) == "NOTSTARTED"} {
				# check if IOM is already OOS
				if {$data(iom,$iom,state) == "oosma"} {
					puts "\nIOM-$iom already OOS.";
					set data(iom,$iom,rmvstate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nIOM-$iom already OOSAUFLT.";
					set data(iom,$iom,rmvstate) "DONE";
					continue;
				}
				#
				log_user 1;
				puts "\nStart RMV-EQPT of IOM-$iom.";
				send "RMV-EQPT::IOM-$iom;\r";
				expect {
				-re "DENY.*Not in Valid State.*;\r\nTelica>" {
					puts "\nRMV-EQPT::IOM-$iom NOT-IN-VALID STATE. Retry.";
					incr data(iom,$iom,resetcounter);
					if {$data(iom,$iom,resetcounter) > $maxresets} {
						puts "\nERROR: Maximum attempts of RMV-EQPT::IOM-$iom exceeded.";
						log_user 1;
						catch {
						send "RTRV-ALM-ALL;\r";
						expect {
						-re ";\r\nTelica>" { }
						timeout { }
						eof { }
						} } ignore;
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rmv_all_ioms: Maximum attempts of RMV-EQPT::IOM-$iom exceeded.";
					}
					# increment prirmvsinprog so we retry
					# again. see below.
				}
				-re ";\r\nTelica>" {
					puts "\nRMV-EQPT::IOM-$iom COMPLETED";
					set data(iom,$iom,rmvstate) "INPROG";
				}
				timeout {
					puts "\nRMV-EQPT::IOM-$iom timeout";
					set data(iom,$iom,rmvstate) "INPROG";
				}
				eof {
					puts "\nERROR: RMV-EQPT::IOM-$iom EOF";
					log_user 0;
					hw_maint_close $spid;
					return "-1 - hw_maint_rmv_all_ioms: RMV-EQPT::IOM-$iom EOF";
				}
				}
				incr prirmvsinprog;
				log_user 0;
			} elseif {$data(iom,$iom,rmvstate) == "INPROG"} {
				if {$data(iom,$iom,state) == "oosma"} {
					puts "\nFinished RMV-EQPT (oosma) of IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosaumaflt"} {
					puts "\nFinished RMV-EQPT (oosaumaflt) of IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nFinished RMV-EQPT (oosauflt) of IOM-$iom.";
					set data(iom,$iom,rmvstate) "DONE";
				} else {
					puts "\nIn Prog RMV-EQPT of IOM-$iom.";
					incr prirmvsinprog;
				}
			} elseif {$data(iom,$iom,rmvstate) == "DONE"} {
				# ok
				puts "\nDone RMV-EQPT of IOM-$iom.";
			}
		}

		# check if in any rmvs are still going.
		if {$secrmvsinprog == 0 && $prirmvsinprog == 0} {
			set done 1;
			set polltime 0;
		}
		hw_maint_canc_user $spid;
		hw_maint_close $spid;
		incr maxiters -1;
	}
	#
	if {!$done} {
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
		set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_rmv_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# try to get alarms.
		log_user 1;
		catch {
		send "RTRV-ALM-ALL;\r";
		expect {
		-re ";\r\nTelica>" { }
		timeout { }
		eof { }
		} } ignore;
		log_user 0;
		# close TL1 session
		hw_maint_close $spid;
		puts "ERROR: time out. unable to remove all IOMs";
		return "-1 - hw_maint_rmv_all_ioms: time out. unable to remove all IOMs";
	} else {
		# close TL1 session
		return "0 - success";
	}
}
#
# restore all IOMs
#
proc hw_maint_rst_all_ioms { labid spaip spbip username passwd { maxiom 17 } { polltime 30 } { maxtime 1200 } { maxresets 5 } } {
	# init some data
	puts "\nInitializing ${labid} RST-EQPT data.";
	for {set iom 1} {$iom<=$maxiom} {incr iom} {
		set data(iom,$iom,rststate) "NOTSTARTED";
		set data(iom,$iom,resetcounter) 0;
	}
	# maximum iterations to try
	set maxiters [expr ${maxtime}/${polltime}];
	# restore the IOMs
	puts "\nStart ${labid} RST-EQPT cycle.";
	for {set done 0} { ! $done && $maxiters>0 } {sleep $polltime} {
		# counters
		set prirstsinprog 0;
		set secrstsinprog 0;
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip,$maxiters).";
		set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_rst_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# rtrv-eqpt to get status
		puts "\nCall RTRV-EQPT to get ${labid} IOM statuses.";
		set status [hw_maint_rtrv_eqpt_all spid data];
		if {[isNotOk $status]} {
			puts  "\nERROR: hw_maint_rst_all_ioms: hw_maint_rtrv_eqpt_all failed:\n${status}";
			puts "\nRe-opening TL1 session for ($labid,$spaip,$spbip).";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# restore any secondary IOMs
		puts "\nHandle RST-EQPT of ${labid} Secondary IOMs.";
		for {set iom 1} {$iom<=$maxiom} {incr iom} {
			# check if IOM exists and is secondary
			if {![info exists data(iom,$iom,protection)]} {
				continue;
			} elseif {$data(iom,$iom,protection) != "sec"} {
				continue;
			}
			# check state of restore
			puts "\nChecking secondary IOM-$iom.";
			if {$data(iom,$iom,rststate) == "NOTSTARTED"} {
				# check if IOM is already IS,ACT
				if {$data(iom,$iom,state) == "isact" || \
				    $data(iom,$iom,state) == "isactnbk" || \
				    $data(iom,$iom,state) == "isstbyh" || \
				    $data(iom,$iom,state) == "isstbyc"} {
					puts "\nIOM-$iom already IS,ACT.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosaumea"} {
					puts "\nIOM-$iom already OOS-AU,MEA.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				}
				#
				log_user 1;
				puts "\nStart RST-EQPT of IOM-$iom.";
				send "RST-EQPT::IOM-$iom;\r";
				expect {
				-re ";\r\nTelica>" {
					puts "\nRST-EQPT::IOM-$iom COMPLETED";
				}
				timeout {
					puts "\nRST-EQPT::IOM-$iom timeout";
				}
				eof {
					puts "\nERROR: RST-EQPT::IOM-$iom EOF";
					log_user 0;
					hw_maint_close $spid;
					return "-1 - hw_maint_rst_all_ioms: RST-EQPT::IOM-$iom EOF";
				}
				}
				log_user 0;
				set data(iom,$iom,rststate) "INPROG";
				incr secrstsinprog;
			} elseif {$data(iom,$iom,rststate) == "INPROG"} {
				if {$data(iom,$iom,state) == "isact" || \
				    $data(iom,$iom,state) == "isactnbk" || \
				    $data(iom,$iom,state) == "isstbyh" || \
				    $data(iom,$iom,state) == "isstbyc"} {
					puts "\nFinished RST-EQPT of IOM-$iom.";
					set data(iom,$iom,rststate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosaumea"} {
					puts "\nIOM-$iom already OOS-AU,MEA.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nIOM-$iom is OOS-AU,FLT.";
					# try a restart
					incr data(iom,$iom,resetcounter);
					if {$data(iom,$iom,resetcounter) > $maxresets} {
						puts "\nERROR: Maximum attempts of RST-EQPT::IOM-$iom exceeded.";
						log_user 1;
						catch {
						send "RTRV-ALM-ALL;\r";
						expect {
						-re ";\r\nTelica>" { }
						timeout { }
						eof { }
						} } ignore;
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rst_all_ioms: Maximum attempts of RST-EQPT::IOM-$iom exceeded.";
					}
					# try restoring again.
					log_user 1;
					puts "\nRe-start RST-EQPT of IOM-$iom after fault.";
					send "RST-EQPT::IOM-$iom;\r";
					expect {
					-re ";\r\nTelica>" {
						puts "\nRST-EQPT::IOM-$iom COMPLETED";
					}
					timeout {
						puts "\nRST-EQPT::IOM-$iom timeout";
					}
					eof {
						puts "\nERROR: RST-EQPT::IOM-$iom EOF";
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rst_all_ioms: RST-EQPT::IOM-$iom EOF";
					}
					}
					log_user 0;
					set data(iom,$iom,rststate) "INPROG";
					incr secrstsinprog;
				} else {
					puts "\nIn Prog RST-EQPT of IOM-$iom.";
					incr secrstsinprog;
				}
			} elseif {$data(iom,$iom,rststate) == "DONE"} {
				# ok
				puts "\nDone RST-EQPT of IOM-$iom.";
			}
		}
		#
		if {$secrstsinprog > 0} {
			# let secondary removes finish before starting
			# to remove primaries.
			puts "\nSecondary RST-EQPT still in prog ... skip primary for now.";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# remove any primary IOMs
		puts "\nHandle RST-EQPT of ${labid} Primary IOMs.";
		for {set iom 1} {$iom<=$maxiom} {incr iom} {
			# check if IOM exists and is primary
			if {![info exists data(iom,$iom,protection)]} {
				continue;
			} elseif {$data(iom,$iom,protection) != "pri"} {
				continue;
			}
			# check state of remove
			puts "\nChecking primary IOM-$iom.";
			if {$data(iom,$iom,rststate) == "NOTSTARTED"} {
				# check if IOM is already IS,ACT
				if {$data(iom,$iom,state) == "isact" || \
				    $data(iom,$iom,state) == "isactnbk" || \
				    $data(iom,$iom,state) == "isstbyh" || \
				    $data(iom,$iom,state) == "isstbyc"} {
					puts "\nIOM-$iom already IS,ACT.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosaumea"} {
					puts "\nIOM-$iom already OOS-AU,MEA.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				}
				#
				log_user 1;
				puts "\nStart RST-EQPT of IOM-$iom.";
				send "RST-EQPT::IOM-$iom;\r";
				expect {
				-re ";\r\nTelica>" {
					puts "\nRST-EQPT::IOM-$iom COMPLETED";
				}
				timeout {
					puts "\nRST-EQPT::IOM-$iom timeout";
				}
				eof {
					puts "\nERROR: RST-EQPT::IOM-$iom EOF";
					log_user 0;
					hw_maint_close $spid;
					return "-1 - hw_maint_rst_all_ioms: RST-EQPT::IOM-$iom EOF";
				}
				}
				log_user 0;
				set data(iom,$iom,rststate) "INPROG";
				incr prirstsinprog;
			} elseif {$data(iom,$iom,rststate) == "INPROG"} {
				if {$data(iom,$iom,state) == "isact" || \
				    $data(iom,$iom,state) == "isactnbk" || \
				    $data(iom,$iom,state) == "isstbyh" || \
				    $data(iom,$iom,state) == "isstbyc"} {
					puts "\nFinished RST-EQPT of IOM-$iom.";
					set data(iom,$iom,rststate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosaumea"} {
					puts "\nIOM-$iom already OOS-AU,MEA.";
					set data(iom,$iom,rststate) "DONE";
					continue;
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nIOM-$iom is OOS-AU,FLT.";
					# try a restart
					incr data(iom,$iom,resetcounter);
					if {$data(iom,$iom,resetcounter) > $maxresets} {
						puts "\nERROR: Maximum attempts of RST-EQPT::IOM-$iom exceeded.";
						log_user 1;
						catch {
						send "RTRV-ALM-ALL;\r";
						expect {
						-re ";\r\nTelica>" { }
						timeout { }
						eof { }
						} } ignore;
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rst_all_ioms: Maximum attempts of RST-EQPT::IOM-$iom exceeded.";
					}
					# try restoring again.
					log_user 1;
					puts "\nRe-start RST-EQPT of IOM-$iom after fault.";
					send "RST-EQPT::IOM-$iom;\r";
					expect {
					-re ";\r\nTelica>" {
						puts "\nRST-EQPT::IOM-$iom COMPLETED";
					}
					timeout {
						puts "\nRST-EQPT::IOM-$iom timeout";
					}
					eof {
						puts "\nERROR: RST-EQPT::IOM-$iom EOF";
						log_user 0;
						hw_maint_close $spid;
						return "-1 - hw_maint_rst_all_ioms: RST-EQPT::IOM-$iom EOF";
					}
					}
					log_user 0;
					set data(iom,$iom,rststate) "INPROG";
					incr prirstsinprog;
				} else {
					puts "\nIn Prog RST-EQPT of IOM-$iom.";
					incr prirstsinprog;
				}
			} elseif {$data(iom,$iom,rststate) == "DONE"} {
				# ok
				puts "\nDone RST-EQPT of IOM-$iom.";
			}
		}

		# check if in any rsts are still going.
		if {$secrstsinprog == 0 && $prirstsinprog == 0} {
			set done 1;
			set polltime 0;
		}
		hw_maint_canc_user $spid;
		hw_maint_close $spid;
		incr maxiters -1;
	}
	#
	if {!$done} {
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
		set status [hw_maint_tl1_open spid $spaip $spbip $username $passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_rst_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# try to get alarms
		log_user 1;
		catch {
		send "RTRV-ALM-ALL;\r";
		expect {
		-re ";\r\nTelica>" { }
		timeout { }
		eof { }
		} } ignore;
		log_user 0;
		# close TL1 session
		hw_maint_close $spid;
		puts "ERROR: time out. unable to restore all IOMs";
		return "-1 - hw_maint_rst_all_ioms: time out. unable to restore all IOMs";
	} else {
		# close TL1 session
		return "0 - success";
	}
}
#
# get branch and cpu load from the active sp.
#
proc hw_maint_get_cpuload { labid spaip spbip username passwd argdata argbranch argcpuload arglastbranch arglastreload { timeout 30 } } {
	upvar $argdata data;
	upvar $argbranch branch;
	upvar $argcpuload cpuload;
	upvar $arglastbranch lastbranch;
	upvar $arglastreload lastreload;
	#
	if {$data(sp,a,state) == "isact" || $data(sp,a,state) == "isactnbk"} {
		set activesp "a";
		set activeip $spaip;
	} elseif {$data(sp,b,state) == "isact" || $data(sp,b,state) == "isactnbk"} {
		set activesp "b";
		set activeip $spbip;
	} else {
		return "-1 - hw_maint_get_cpuload: unable to determine active SP.";
	}
	# open a telnet session to active sp
	set status [hw_maint_telnet_open spawn_id $activeip $username $passwd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_get_cpuload: hw_maint_telnet_open failed:\n${status}";
	}
	# determine the current load and where it is stored.
	set done 0;
	send "cd /Telica/swCPU/CurrRel\r";
	expect {
	-nocase -re {file.*directory.*doesn't.*exist[^\r\n]*[\r\n]} {
		set branch "";
		set cpuload "";
		return "-1 - hw_maint_get_cpuload: CurrRel does not exist";
	}
	-re {(/[^\r\n]*)[\r\n]} {
		set cpuload [file tail $expect_out(1,string)];
		exp_continue;
	}
	-re "COMMAND_IS_DONE_[exp_pid]>" {
		if {!$done} {
			incr done;
			send "pwd -P\r";
			exp_continue;
		}
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_get_cpuload: cd CurrRel timed out:\n$status";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - hw_maint_get_cpuload: cd CurrRel eof:\n$status";
	}
	}
	# get last reload
	set lastreload "UNKNOWN";
        #
        send "\[ -r /.last_reload ] && echo \"Last Reload: \$(cat /.last_reload)\";\r";
	expect {
	-re {[\r\n]Last Reload: ([^\r\n]*)} {
		set lastreload  $expect_out(1,string);
		exp_continue;
		}
	-re ".*COMMAND_IS_DONE_[exp_pid]>" {
		# ok
	}
	timeout {
		# ok
	}
	eof {
		# ok
	}
	};
	# get branch for this load.
	set status [dbselect obuf loads "cpuload req ^$cpuload\$" "branch"];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_get_cpuload: dbselect of relation 'loads' failed: \n${status}";
	}
	if {![info exists obuf] || [llength $obuf] == 0} {
		# return "-1 - hw_maint_get_cpuload: No BRANCH found for CPULOAD $cpuload.";
		puts "\nWARNING: hw_maint_get_cpuload: No BRANCH found for CurrRel CPULOAD $cpuload.";
		set branch "UNKNOWN";
	} else {
		set branch [lindex $obuf 0];
	}
	#
	if {[string length $lastreload] > 0} {
		set status [dbselect obuf loads "cpuload req ^$lastreload\$" "branch"];
		if {[isNotOk $status]} {
			return "-1 - hw_maint_get_cpuload: dbselect of relation 'loads' failed: \n${status}";
		}
		if {![info exists obuf] || [llength $obuf] == 0} {
			puts "\nWARNING: hw_maint_get_cpuload: No BRANCH found for lastreload CPULOAD $lastreload.";
			set lastbranch "UNKNOWN";
		} else {
			set lastbranch [lindex $obuf 0];
		}
	} else {
		set lastbranch "UNKNOWN";
	}
	#
	hw_maint_close $spawn_id;
	return "0 - success";
}
#
# rsh to an IOM interface
#
proc hw_maint_iom_rsh_open { spid ip { timeout 30 } { maxrshs 5 } } {
	upvar $spid spawn_id;
	#
	#
	log_user 1;
	for {set irsh 0} {$irsh<$maxrshs} {incr irsh} {
		set firsttime 1;
		send "rsh ${ip}\r";
		expect {
		-re {.*# } {
			# ok;
			log_user 0;
			return "0 - success";
		}
		timeout {
			if {$firsttime} {
				send "\r";
				set firsttime 0;
				exp_continue;
			}
			puts "\nhw_maint_iom_rsh_open: timeout from rsh to ${ip}. Sleep and retry.";
			sleep 5;
		}
		eof {
			log_user 0;
			return "-1 - hw_maint_iom_rsh_open: eof from rsh to ${ip}.";
		}
		}
	}
	# not ok
	log_user 0;
	puts "\nhw_maint_iom_rsh_open: unable to rsh to ${ip}.";
	return "-1 - hw_maint_iom_rsh_open: unable to rsh to ${ip}.";
}
#
proc hw_maint_iom_rsh_close { spid { timeout 30 } } {
	upvar $spid spawn_id;
	#
	log_user 1;
	#
	if {[catch {
		send "exit\r";
		expect {
		-re ".*COMMAND_IS_DONE_[exp_pid]>" {
			# ok.
			log_user 0;
			puts "";
			return "0 - success";
		}
		timeout {
			log_user 0;
			puts "";
			return "-1 - hw_maint_iom_rsh_close: timeout from rsh exit.";
		}
		eof {
			log_user 0;
			puts "";
			return "-1 - hw_maint_iom_rsh_close: eof from rsh exit.";
		}
		}
	} errmsg] != 0} {
		return "-1 - hw_maint_iom_rsh_close: interrupt caught: ${errmsg}.";
	}
	
}
#
proc hw_maint_iom_rsh_exec { spid cmd { timeout 300 } } {
	upvar $spid spawn_id;
	#
	log_user 1;
	send "${cmd}\r";
	expect {
	-re {.*# } {
		# ok;
		log_user 0;
		return "0 - success";
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_iom_rsh_exec: timeout from cmd ${cmd}.";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_iom_rsh_exec: eof from cmd ${cmd}.";
	}
	}
}
#
# update ioms using update_flash_partition
#
proc hw_maint_update_ioms { labid spaip spbip { tl1username "telica" } { tl1passwd "telica" } { lynxusername "root" } { lynxpasswd "plexus9000" } { iomstates "oosaumaflt,oosauflt" } { iomlist "" } { allflag 0 } { maxiom 17 } } {
	#
	puts "\nAttempting to update ${labid} IOMS:";
	# sanity checks
	if {[string length $iomstates] == 0 && [string length $iomlist] == 0} {
		return "-1 - hw_maint_update_ioms: neither IOM states or IOM list was given.";
	}
	# open active TL1 port 
	puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
	set status [hw_maint_tl1_open tl1spid $spaip $spbip $tl1username $tl1passwd]
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: hw_maint_tl1_open failed:\n${status}";
	}
	# do a rtrv-eqpt to get status
	puts "\nCall RTRV-EQPT to get ${labid} IOM statuses.";
	set status [hw_maint_rtrv_eqpt_all tl1spid data];
	if {[isNotOk $status]} {
		hw_maint_close $tl1spid;
		return "-1 - hw_maint_update_ioms: hw_maint_rtrv_eqpt_all failed:\n${status}";
	}
	# do a rtrv-chassis-eqpt to get status
	puts "\nCall RTRV-CHASSIS-EQPT to get chassis data.";
	set status [hw_maint_rtrv_chassis_eqpt tl1spid data];
	if {[isNotOk $status]} {
		hw_maint_close $tl1spid;
		return "-1 - hw_maint_update_ioms: hw_maint_rtrv_chassis_eqpt failed:\n${status}";
	}
	hw_maint_canc_user $tl1spid;
	hw_maint_close $tl1spid;
	# generate list of IOMs to update
	set iomcnt 0;
	set checkstate 0;
	if {[string length $iomstates] > 0 && $iomstates != "*"} {
		puts "\nChecking list of IOMs against requested states: ${iomstates}";
		set statelist [split $iomstates ","];
		set checkstate 1;
	}
	if {[string length $iomlist] > 0} {
		puts "\nChecking the given list of IOMs: $iomlist";
		foreach iom [split $iomlist ","] {
			if {![info exists data(iom,$iom,protection)] || \
		     	     [string length $data(iom,$iom,protection)] <= 0} {
				puts "\nIOM $iom does not exist.";
				return "-1 - hw_maint_update_ioms: IOM $iom does not exist.";
			} elseif {$checkstate && [lsearch -exact $statelist $data(iom,$iom,state)] == -1} {
				# not in right state. skip it.
				continue;
			}
			incr iomcnt;
			set ioms($iomcnt) $iom;
		}
	} else {
		puts "\nDetermining list of available IOMs:";
		for {set iom 1} {$iom<=$maxiom} {incr iom} {
			# check if IOM exists 
			if {![info exists data(iom,$iom,protection)] || \
		     	     [string length $data(iom,$iom,protection)] <= 0} {
				continue;
			} elseif {$checkstate && [lsearch -exact $statelist $data(iom,$iom,state)] == -1} {
				# not in right state. skip it.
				continue;
			}
			incr iomcnt;
			set ioms($iomcnt) $iom;
		}
	}
	# any IOMs to update?
	if {$iomcnt <= 0} {
		puts "\nNO IOMs found to update.";
		return "0 - success. NO IOMs found to update.";
	} else {
		puts -nonewline "\n$iomcnt IOMs to update: ";
		for {set idx 1} {$idx<=$iomcnt} {incr idx} {
			puts -nonewline "$ioms($idx) ";
		}
		puts "";
	}
	# get branch and cpu load
	set status [hw_maint_get_cpuload $labid $spaip $spbip $lynxusername $lynxpasswd data branch cpuload lastbranch lastreload];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: hw_maint_get_cpuload failed:\n${status}";
	}
	#
	set data(branch) $branch;
	set data(cpuload) $cpuload;
	set data(lastreload) $lastreload;
	set data(lastbranch) $lastbranch;
	#
	puts "\nSwitch CPU branch: $data(branch)";
	puts "Switch CPU load  : $data(cpuload)";
	puts "\nLast Reload branch: $data(lastbranch)";
	puts "Last Reload load  : $data(lastreload)";
	#
	# we have an interesting case. what if cpuload and lastreload
	# are not the same? which one to use? we can have a reload load
	# names that look like this: 6.2.1.2.SP.2.2 and the CPU load
	# should then look like this: 6.2.1.2.SP.2, that is, the CPU
	# load should be a prefix to the reload load. if this is not
	# true, then last reload is corrupted and trust the CPU load
	# obtained from CurrRel. if it is true, then use the load
	# stored in /.last_reload for the following queries. the assumption
	# is that the load and data are still there for a query. during
	# a reload this should be true.
	#
	if {$branch == "UNKNOWN" && $lastbranch == "UNKNOWN"} {
		return "-1 - hw_maint_update_ioms: Unable to determine BRANCH and LASTBRANCH.";
	} elseif {$branch == "UNKNOWN" && $lastbranch != "UNKNOWN"} {
		puts "\nBRANCH is unknown. Using LASTBRANCH ($lastbranch).";
		set branch $lastbranch;
	} elseif {$branch != "UNKNOWN" && $lastbranch == "UNKNOWN"} {
		puts "\nLASTBRANCH is unknown. Using BRANCH ($branch).";
		set lastbranch $branch;
	}
	#
	if {[regexp -- "^${cpuload}\.\[0-9]\[0-9]*" ${lastreload}] == 1} {
		puts "\nUsing last reload CPU load: ${lastreload}";
		set realbranch ${lastbranch};
		set realload ${lastreload};
	} else {
		puts "\nUsing CurrRel CPU load: ${cpuload}";
		set realbranch ${branch};
		set realload ${cpuload};
	}
	#
        # get text file system
	#
	set status [dbselect fsobuf filesystems "branch req ^$realbranch\$ and type req text" "path"];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: dbselect of relation 'filesystems' failed: \n${status}";
	}
	if {![info exists fsobuf] || [llength $fsobuf] == 0} {
		return "-1 - hw_maint_update_ioms: no filesystem found for branch $realbranch.";
	}
	set filesystem [lindex $fsobuf 0];
	#
	# get images
	#
	set status [dbselect obuf images "branch req ^$realbranch\$ and cpuload req ^$realload\$" "type name"];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: dbselect of relation 'images' failed: \n${status}";
	}
	if {![info exists obuf] || [llength $obuf] == 0} {
		return "-1 - hw_maint_update_ioms: No images found for load: $realbranch, $realload.";
	}
	#
	puts "\nImages for $realbranch, $realload:";
	foreach tuple $obuf {
		set fields [split $tuple " \t"];
		set type [lindex $fields 0]
		set name [lindex $fields 1]
		set data(files,$type,path) "${filesystem}/${realbranch}/${type}";
		set data(files,$type,name) $name;
		#
		puts "\nType: $type";
		puts "Path: $data(files,$type,path)";
		puts "Name: $data(files,$type,name)";
	}
	# get files for updates
	global ncleis;
	for {set idx 1} {$idx<=$iomcnt} {incr idx} {
		set iom $ioms($idx);
		set clei $data(iom,$iom,clei);
		set rearclei $data(iom,$iom,rearclei);
		#
		if {[info exists ncleis($realbranch,$clei,$rearclei)]} {
			set type $ncleis($realbranch,$clei,$rearclei);
		} elseif {[info exists ncleis(default,$clei,$rearclei)]} {
			set type $ncleis(default,$clei,$rearclei);
		} elseif {[info exists ncleis($realbranch,$clei,none)]} {
			set type $ncleis($realbranch,$clei,none);
		} elseif {[info exists ncleis(default,$clei,none)]} {
			set type $ncleis(default,$clei,none);
		} else {
			return "-1 - hw_maint_update_ioms: Unknown CLEI values $clei and $rearclei for IOM $iom";
		}
		lappend data(types) $type;
		lappend data($type,ioms) $iom;
		set data(iom,$iom,type) $type;
		#
		if {[info exists data(files,$type,name)]} {
			set data(iom,$iom,file) "$data(files,$type,path)/$data(files,$type,name)";;
		} else {
			set data(iom,$iom,file) "UNKNOWN";
		}
		#
		puts "\nIOM $iom data:";
		puts "CLEI: $data(iom,$iom,clei)";
		puts "Rear CLEI: $data(iom,$iom,rearclei)";
		puts "Type ($realbranch): $data(iom,$iom,type)";
		puts "File ($realbranch): $data(iom,$iom,file)";
	}
	#
	puts -nonewline "\nIOM Types to Update:";
	foreach type [lsort $data(types)] {
		if {![info exists known($type)]} {
			lappend uniquetypes $type;
			set known($type) $type;
			puts -nonewline " $type";
		}
	}
	puts "";
	set data(types) $uniquetypes;
	#
	# init global data
	#
	global flashparts;
	set status [initFlashPartsData];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: initFlashPartsData failed:\n${status}";
	}
	#
	global iomips;
	set status [initIomIpsData];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: initIomIpsData failed:\n${status}";
	}
	#
	global iomfps;
	set status [initIomFpsData];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: initIomFpsData failed:\n${status}";
	}
	#
	# expand tar files
	#
	puts "\nExpand tar files:";
	foreach type $data(types) {
		set tarpath $data(files,$type,path);
		set tarfilename $data(files,$type,name);
		set tarfile "${tarpath}/${tarfilename}";
		#
		puts "\nExpanding $tarfile (links only):"
		# log_user 1;
		spawn gnutar tzvf $tarfile;
		expect {
		-re {(Telica/sw[^ \t]*)[ \t]*->} {
			set linkpath $expect_out(1,string);
			set link [file tail $expect_out(1,string)];
			#
			if {[info exists iomfps(${link})]} {
				set ftype $iomfps(${link});
			} else {
				puts "\nSkipping unknown file type ${link}.";
				exp_continue;
			}
			#
			# set ftype [string tolower [regsub -- {_bin} $link ""]];
			# KLUDGE - fix announcements
			# if {$ftype == "announcement"} {
				# set ftype "announcements";
			# }
			puts "\nFPath: $linkpath";
			puts "FLink: $link";
			puts "FType: $ftype";
			#
			# vs3 contains two sets of tar files, so 
			# i have to append so that i don't lose
			# data. when the update command is finally 
			# constructed, then i have a kludge to handle
			# this special case. keep things as standardized
			# as long as possible.
			#
			lappend data(flinks,$type,$ftype) "/${linkpath}";
			lappend data(ftype,$type,$ftype) ${ftype};
			#
			if {![info exists data(ftype,$type,ftypes)] || \
			     [lsearch -exact $data(ftype,$type,ftypes) ${ftype}] == -1} {
				lappend data(ftype,$type,ftypes) ${ftype};
			}
			exp_continue;
		}
		timeout { puts "\ngnutar done (timeout)."; }
		eof { puts "\ngnutar done (eof)."; }
		}
		# log_user 0;
		hw_maint_close $spawn_id;
	}
	#
	# generate the commands to update each required flash partition
	# for each IOM.
	#
	puts "\nUpdating IOM Flash Partitions:";
	if {$data(sp,a,state) == "isact" || \
	    $data(sp,a,state) == "isactnbk"} {
		# set activespip "192.168.252.26";
		set activespip $iomips(spa,a);
		set extactivespip $spaip;
	} elseif {$data(sp,b,state) == "isact" || \
	    $data(sp,b,state) == "isactnbk"} {
		# set activespip "192.168.252.27";
		set activespip $iomips(spb,a);
		set extactivespip $spbip;
	} else {
		return "-1 - hw_maint_update_ioms: Unknown active SP.";
	}
	# open telnet port to active sp.
	set status [hw_maint_telnet_open spawn_id $extactivespip $lynxusername $lynxpasswd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_update_ioms: hw_maint_telnet_open failed:\n${status}";
	}
	#
	for {set idx 1} {$idx<=$iomcnt} {incr idx} {
		set iom $ioms($idx);
		set iomtype $data(iom,$iom,type);
		puts "";
		puts "IOM : $iom";
		puts "TYPE: $iomtype";
		if {$iomtype == "cm"} {
			set mincpu 1;
			set maxcpu 4;
		} else {
			set mincpu 1;
			set maxcpu 1;
		}
		# do a sfui for this iom
		set status [hw_maint_sfui spawn_id $iom];
		if {[isNotOk $status]} {
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_update_ioms: hw_maint_sfui failed:\n${status}";
		}
		# rsh to each cpu and update the silly thing.
		for {set cpu $mincpu} {$cpu<=$maxcpu} {incr cpu} {
			set rsh "rsh $iomips($iom,$cpu)";
			puts "\tIOM CPU: $cpu";
			puts "\tCPU IP : $iomips($iom,$cpu)";
			puts "\tRSH CMD: $rsh";
			#
			set status [hw_maint_iom_rsh_open spawn_id $iomips($iom,$cpu)];
			if {[isNotOk $status]} {
				# hw_maint_close $spawn_id;
				# return "-1 - hw_maint_update_ioms: hw_maint_iom_rsh_open failed:\n${status}";
				puts "ERROR: hw_maint_update_ioms: hw_maint_iom_rsh_open failed:\n${status}\nContinue to next CPU or IOM.";
				# close up the stream and reopen it.
				catch {
					hw_maint_iom_rsh_close spawn_id;
				} ignore;
				catch {
					hw_maint_close $spawn_id;
				} ignore;
				sleep 60;
				set status [hw_maint_telnet_open spawn_id $extactivespip $lynxusername $lynxpasswd];
				if {[isNotOk $status]} {
					return "-1 - hw_maint_update_ioms: hw_maint_telnet_open (re-open) failed:\n${status}";
				}
				continue;
			}
			#
			if {$allflag} {
				# update everything we know of
				set fps $data(ftype,$iomtype,ftypes);
			} else {
				# update a minimal set of partitions
				set fps $flashparts(minimal,$iomtype);
			}
			# check if we need to clean up old log files
			if {${iomtype} == "cm" && ${cpu} == 1 && ${allflag}} {
				set cleancmdisk "/application/clean_cm_disk";
				puts "\tRun CLEAN-UP CMD on CM ${iom} CPU ${cpu}: $cleancmdisk";
				hw_maint_iom_rsh_exec spawn_id $cleancmdisk;
			}
			# 
			foreach fp $fps {
				set linkpath $data(flinks,$iomtype,$fp);
				set ftype $data(ftype,$iomtype,$fp);
				if {[llength $linkpath] > 1} {
					# KLUDGE - check if it's vs3.
					if {$iomtype != "vs3"} {
						hw_maint_iom_rsh_close spawn_id;
						hw_maint_close $spawn_id;
						return "-1 - hw_maint_update_ioms: multiple files in one tar file, and not 'vs3'; it's $iomtype.";
					}
					set lplen [llength $linkpath]]
					for {set l 0} {$l<$lplen} {incr l} {
						set lp [lindex $linkpath $l];
						set ft [lindex $ftype $l];
						#
						switch -regexp -- $lp {
						{^.*swVS3PG.*$} {
							set linkpath $lp;
							set ftype $ft;
							if {$data(chassis,mgtype) == "pg"} {
								break;
							}
						}
						{^.*swVS3UMTS.*$} {
							set linkpath $lp;
							set ftype $ft;
							if {$data(chassis,mgtype) == "tdmip" || \
							    $data(chassis,mgtype) == "umts"} {
								break;
							}
						}
						}
					}
				}
				#
				set updcmd "/application/update_flash_partition --v ${activespip} ${linkpath} ${ftype}";
				puts "\t\tFTYPE: $ftype ($ftype)";
				puts "\t\tFPATH: $linkpath";
				puts "\t\tUPD CMD: $updcmd";
				set status [hw_maint_iom_rsh_exec spawn_id $updcmd];
				if {[isNotOk $status]} {
					hw_maint_iom_rsh_close spawn_id;
					hw_maint_close $spawn_id;
					return "-1 - hw_maint_update_ioms: hw_maint_iom_rsh_exec failed:\n${status}";
				}
			}
			#
			hw_maint_iom_rsh_close spawn_id;
		}
	}
	# all done
	hw_maint_close $spawn_id;
	return "0 - success";
}
#
# cpy-mem all IOMs
#
proc hw_maint_cpymem_all_ioms { labid spaip spbip tl1username tl1passwd { recoverioms 0 } { lynxusername "root" } { lynxpasswd "plexus9000" } { maxcpymems 3 } { maxiom 17 } { polltime 30 } { maxtime 1800 } { maxresets 5 } } {
	# start cpy-mem process
	puts "\nStart ${labid} CPY-MEM cycle.";
	# open active TL1 port 
	puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
	set status [hw_maint_tl1_open spid $spaip $spbip $tl1username $tl1passwd]
	if {[isNotOk $status]} {
		return "-1 - hw_maint_cpymem_all_ioms: hw_maint_tl1_open failed:\n${status}";
	}
	set spawn_id $spid;
	# init some data
	puts "\nInitializing ${labid} CPY-MEM data.";
	for {set iom 1} {$iom<=$maxiom} {incr iom} {
		set data(iom,$iom,cpymemstate) "NOTSTARTED";
		set data(iom,$iom,resetcounter) 0;
	}
	puts "\nMaximum simultaneous CPM-MEMs is ${maxcpymems}";
	# maximum iterations to try
	set maxiters [expr ${maxtime}/${polltime}];
	# verify all IOMs OOS. do a rtrv-eqpt to get status
	puts "\nCall RTRV-EQPT to get ${labid} IOM statuses.";
	set status [hw_maint_rtrv_eqpt_all spid data];
	if {[isNotOk $status]} {
		hw_maint_close $spid;
		return "-1 - hw_maint_cpymem_all_ioms: hw_maint_rtrv_eqpt_all failed:\n${status}";
	}
	# close TL1 session -- will reopen when needed
	hw_maint_canc_user $spid;
	hw_maint_close $spid;
	#
	set notoosioms "";
	set iomcnt 0;
	for {set iom 1} {$iom<=$maxiom} {incr iom} {
		# check if IOM exists and is OOS
		if {![info exists data(iom,$iom,protection)] || \
		     [string length $data(iom,$iom,protection)] <= 0} {
			continue;
		} elseif {$data(iom,$iom,state) != "oosma" && \
			  $data(iom,$iom,state) != "oosauflt" && \
			  $data(iom,$iom,state) != "oosaumaflt"} {
			set notoosioms "${notoosioms} ${iom}"
		}
		incr iomcnt;
		set ioms($iomcnt) $iom;
	}
	if {[string length "${notoosioms}"] > 0} {
		return "-1 - hw_maint_cpymem_all_ioms: IOMs ${notoosioms} are not OOS,MA.";
	} elseif {$iomcnt <= 0} {
		return "-1 - hw_maint_cpymem_all_ioms: NO IOMs found to CPY-MEM.";
	} else {
		puts -nonewline "\n$iomcnt IOMs to CPY-MEM are: ";
		for {set idx 1} {$idx<=$iomcnt} {incr idx} {
			puts -nonewline "$ioms($idx) ";
		}
		puts "";
	}
	# set the initial range of IOMs to cpy-mem
	set minidx 1;
	set maxidx $maxcpymems;
	if {$maxidx>$iomcnt} {
		set maxidx $iomcnt;
	}
	# start cpy-mems
	for {set done 0} { ! $done && $maxiters>0 } {sleep $polltime} {
		# counters
		set cpymemsinprog 0;
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip,$maxiters).";
		set status [hw_maint_tl1_open spid $spaip $spbip $tl1username $tl1passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_cpymem_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# rtrv-eqpt to get status
		puts "\nCall RTRV-EQPT to get ${labid} IOM statuses.";
		set status [hw_maint_rtrv_eqpt_all spid data];
		if {[isNotOk $status]} {
			puts  "\nERROR: hw_maint_cpymem_all_ioms: hw_maint_rtrv_eqpt_all failed:\n${status}";
			puts "\nRe-opening TL1 session for ($labid,$spaip,$spbip).";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# handle IOM cpy-mems
		puts "\nHandle CPY-MEMs of ${labid} IOMs.";
		for {set idx $minidx} {$idx<=$maxidx} {incr idx} {
			# current IOM
			set iom $ioms($idx);
			# check if IOM exists
			if {![info exists data(iom,$iom,protection)]} {
				continue;
			}
			# check state of restore
			puts "\nChecking IOM-$iom.";
			if {$data(iom,$iom,cpymemstate) == "NOTSTARTED"} {
				puts "\nStart CPY-MEM of IOM-$iom.";
				log_user 1;
				send "CPY-MEM::IOM-$iom;\r";
				expect {
				-re ";\r\nTelica>" {
					puts "\nCPY-MEM::IOM-$iom COMPLETED";
				}
				-re "<\r\nTelica>" {
					puts "\nCPY-MEM::IOM-$iom COMPLETED";
				}
				timeout {
					puts "\nCPY-MEM::IOM-$iom timeout";
				}
				eof {
					puts "\nERROR: CPY-MEM::IOM-$iom EOF";
					log_user 0;
					hw_maint_close $spid;
					return "-1 - hw_maint_cpymem_all_ioms: CPY-MEM::IOM-$iom EOF";
				}
				}
				log_user 0;
				set data(iom,$iom,cpymemstate) "INPROG";
				incr cpymemsinprog;
			} elseif {$data(iom,$iom,cpymemstate) == "INPROG"} {
				if {$data(iom,$iom,state) == "oosma"} {
					puts "\nFinished CPY-MEM of IOM-$iom.";
					set data(iom,$iom,cpymemstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosauflt"} {
					puts "\nFaulted CPY-MEM of IOM-$iom (oosauflt).";
					set data(iom,$iom,cpymemstate) "DONE";
				} elseif {$data(iom,$iom,state) == "oosaumaflt"} {
					puts "\nFaulted CPY-MEM of IOM-$iom (oosaumaflt).";
					set data(iom,$iom,cpymemstate) "DONE";
				} else {
					puts "\nIn Prog CPY-MEM of IOM-$iom.";
					incr cpymemsinprog;
				}
			} elseif {$data(iom,$iom,cpymemstate) == "DONE"} {
				# ok
				puts "\nDone CPY-MEM of IOM-$iom.";
			}
		}
		#
		if {$cpymemsinprog > 0} {
			puts "\nCPY-MEMs still in prog ... ";
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			incr maxiters -1;
			continue;
		}
		# done with one set of IOMs. reset IOM limits
		# for the next set, if it exists.
		incr minidx $maxcpymems;
		incr maxidx $maxcpymems;
		if {$minidx>$iomcnt} {
			# done
			set done 1;
			set polltime 0;
		} elseif {$maxidx>$iomcnt} {
			set maxidx $iomcnt;
		}
		hw_maint_canc_user $spid;
		hw_maint_close $spid;
		incr maxiters -1;
	}
	# how many IOMs are faulted?
	puts "\nChecking if any IOMs faulted:";
	#
	set faultedioms 0;
	for {set idx 1} {$idx<=$iomcnt} {incr idx} {
		# current IOM
		set iom $ioms($idx);
		# check if IOM exists
		if {![info exists data(iom,$iom,protection)]} {
			continue;
		}
		# check if IOM faulted?
		if {$data(iom,$iom,state) == "oosaumaflt"} {
			puts "\nCPY-MEM of IOM-$iom FAULTED (oosaumaflt).";
			incr faultedioms;
		} elseif {$data(iom,$iom,state) == "oosauflt"} {
			puts "\nCPY-MEM of IOM-$iom FAULTED (oosauflt).";
			incr faultedioms;
		}
	}
	#
	if {!$done || $faultedioms > 0} {
		# open active TL1 port 
		puts "\nOpening TL1 session for ($labid,$spaip,$spbip).";
		set status [hw_maint_tl1_open spid $spaip $spbip $tl1username $tl1passwd]
		if {[isNotOk $status]} {
			return "-1 - hw_maint_cpymem_all_ioms: hw_maint_tl1_open failed:\n${status}";
		}
		set spawn_id $spid;
		# try to get alarms
		log_user 1;
		catch {
		send "RTRV-ALM-ALL;\r";
		expect {
		-re ";\r\nTelica>" { }
		timeout { }
		eof { }
		} } ignore;
		log_user 0;
		# check if we should try to update manually
		if {!$recoverioms} {
			# don't attempt to recover
			# close TL1 session
			hw_maint_canc_user $spid;
			hw_maint_close $spid;
			return "-1 - hw_maint_cpymem_all_ioms: time out. unable to CPY-MEM all IOMs";
		}
		# close existing tl1 session.
		hw_maint_canc_user $spid;
		hw_maint_close $spid;
		# try to manually update IOMS that are OOS and faulted.
		set status [hw_maint_update_ioms $labid $spaip $spbip $tl1username $tl1passwd $lynxusername $lynxpasswd ];
		if {[isNotOk $status]} {
			return "-1 - hw_maint_cpymem_all_ioms: unable to CPY-MEM all IOMs (update_flash_partition failed also):\n${status}";
		}
	} else {
		# close TL1 session
		hw_maint_close $spid;
	}
	return "0 - success";
}
#
# diagnose and attempt to recover sps
#
proc hw_maint_clear_console_users { { labid unknown } } {
	#
	foreach resource [list lynx-a lynx-b tl1-a tl1-b] {
		# check resource usage
		spawn -noecho rmstat -L ${labid} -t ${resource};
		expect {
		-re {rmstat.*Resource is not in use} {
			# ok;
			puts "\nResource ${resource} is free."
			continue;
		}
		-re "${resource}.*\[0-9]\[0-9]*.*pxmonitor" {
			puts "\nResource ${resource} is IN-USE."
			# kill user
		}
		-re {rmstat.*No such resource for your LABID} {
			puts "\nResource ${resource} not found for ${labid}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: Resource ${resource} not found for ${labid}";
		}
		timeout {
			puts "\nTime out waiting for ${labid} ${resource}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: Time out waiting for ${labid} ${resource}";
		}
		eof {
			puts "\nEOF waiting for ${labid} ${resource}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: EOF waiting for ${labid} ${resource}";
		}
		}
		# remove users
		spawn -noecho pxreset -t ${resource} -L ${labid} -y;
		expect {
		-re "All.*interactive.*connections.*${resource}.*${labid}.*shutdown" {
			# ok;
			puts "\nResource ${resource} released via pxreset."
		}
		-re "ERROR.*No such resource has been configured" {
			puts "\nResource ${resource} not found for ${labid}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: Resource ${resource} not found for ${labid}";
		}
		timeout {
			puts "\nTime out waiting for ${labid} ${resource}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: Time out waiting for ${labid} ${resource}";
		}
		eof {
			puts "\nEOF waiting for ${labid} ${resource}";
			hw_maint_close $spawn_id;
			return "-1 - hw_maint_clear_console_users: EOF waiting for ${labid} ${resource}";
		}
		}
	}
	#
	hw_maint_close $spawn_id;
	#
	return "0 - success";
}
#
proc hw_maint_logon_lynx_consoles { labid allspips lynxusername lynxpasswd spids useskdb { timeout 30 } { maxretries 5 } { retrydelay 45 } } {
	upvar $allspips spips;
	upvar $spids spawn_ids;
	#
	foreach resource [list lynx-a lynx-b] {
		# init all counters
		set retries 0;
		set interrupts 0;
		# set interrupt character to delete, not control-C.
		set stty_init "intr ";
		# logon each sp via the console
		spawn -noecho pxmonitor -L ${labid} -t ${resource};
		#
		set spawn_ids(${resource}) $spawn_id;
		#
		expect {
		-re {Starting.*interactive.*mode} {
			send "\r";
			exp_continue;
		}
		-re {.*login:} {
			send "${lynxusername}\r";
			exp_continue;
		}
		-re {.*user name:} {
			send "${lynxusername}\r";
			exp_continue;
		}
		-re {.*[pP]assword:} {
			send "${lynxpasswd}\r";
			exp_continue;
		}
		-re ".*PS1=\"COMMAND_IS_DONE_[exp_pid]>\"" {
			exp_continue;
		}
		-re ".*COMMAND_IS_DONE_[exp_pid]>" {
			# ok.
		}
		-re {ibm750.*->} {
			# UGH. We have an SP stuck in the firmware mode.
			puts "\nRESOURCE ${resource} FOUND AT (ibm*) PROMPT.";
			puts "ATTEMPTING TO RECOVER ${resource}.";
			# set minimum parameters to sane values
			send "set boot_device flash\r";
			expect {
			-re {ibm750.*->} { }
			default { }
			}
			send "set boot_timeout 3\r";
			expect {
			-re {ibm750.*->} { }
			default { }
			}
			send "set com_enable 6\r";
			expect {
			-re {ibm750.*->} { }
			default { }
			}
			send "set ether_port management\r";
			expect {
			-re {ibm750.*->} { }
			default { }
			}
			send "pr -t var\r";
			expect {
			-re {ibm750.*->} { }
			default { }
			}
			#
			send "auto\r";
			incr retries;
			if ($retries>=$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_logon_lynx_consoles: Time out waiting for pxmonitor ${labid} ${resource}";
			}
			exp_continue;
		}
		-re {Are.*you.*sure.*reboot.*system} {
			# yup, kick this dog.
			send "y";
			exp_continue;
		}
		-re {.*> } {
			send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
			exp_continue;
		}
		-re {.*# } {
			send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
			exp_continue;
		}
		-re {[\r\n]\* } {
			# SKDB prompt. reboot.
			puts "\nRESOURCE ${resource} FOUND AT skdb PROMPT.";
			puts "ATTEMPTING TO RECOVER ${resource}.";
			send "R\r"
			incr retries;
			if ($retries>=$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_logon_lynx_consoles: Time out waiting for pxmonitor ${labid} ${resource}";
			}
			exp_continue;
		}
		-re {Command\? } {
			# SKDB prompt. reboot.
			puts "\nRESOURCE ${resource} FOUND AT Command? PROMPT.";
			puts "ATTEMPTING TO RECOVER ${resource}.";
			send "b\r"
			incr retries;
			if ($retries>=$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_logon_lynx_consoles: Time out waiting for pxmonitor ${labid} ${resource}";
			}
			exp_continue;
		}
		timeout {
			puts "\nTime out waiting for pxmonitor ${labid} ${resource}";
			# maybe something is holding the terminal. try once.
			if {$interrupts <= 0} {
				incr interrupts;
				if {$useskdb} {
					send "";
				} else {
					send "";
				}
				exp_continue;
			}
			incr retries;
			if ($retries>=$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_logon_lynx_consoles: Time out waiting for pxmonitor ${labid} ${resource}";
			}
			puts "\nTime out. Retry $retries for ${resource}."
			hw_maint_close $spawn_id;
			sleep $retrydelay;
			# set interrupt character to delete, not control-C.
			set stty_init "intr ";
			spawn -noecho pxmonitor -L ${labid} -t ${resource};
			set spawn_ids(${resource}) $spawn_id;
			exp_continue;
		}
		eof {
			puts "\nEOF waiting for pxmonitor ${labid} ${resource}";
			incr retries;
			if ($retries>=$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_logon_lynx_consoles: EOF waiting for pxmonitor ${labid} ${resource}";
			}
			puts "\nEOF. Retry $retries for ${resource}."
			hw_maint_close $spawn_id;
			sleep $retrydelay;
			# set interrupt character to delete, not control-C.
			set stty_init "intr ";
			spawn -noecho pxmonitor -L ${labid} -t ${resource};
			set spawn_ids(${resource}) $spawn_id;
			exp_continue;
		}
		}
	}
	#
	return "0 - success";
}
#
proc hw_maint_lynx_exec { resource spids cmd { timeout 30 } } {
	upvar ${spids} spawn_ids;
	#
	set spawn_id $spawn_ids(${resource});
	#
	log_user 1;
	send "${cmd}\r";
	#
	expect {
	-re ".*STATUS=(\[-0-9]+)\[\r\n]*COMMAND_IS_DONE_[exp_pid]>" {
		log_user 0;
		puts "\nStatus is $expect_out(1,string)";
		set status $expect_out(1,string);
		if {[isNotOk $status]} {
			return "-1 - hw_maint_lynx_exec: cmd failed with status $status:\n$cmd";
		} else {
			return "0 - cmd failed with status $status:\n$cmd";
		}
	}
	-re ".*\[\r\n]COMMAND_IS_DONE_[exp_pid]>" {
		send "echo STATUS=$?\r";
		exp_continue;
	}
	timeout {
		log_user 0;
		return "-1 - hw_maint_lynx_exec: timeout during cmd:\n$cmd";
	}
	eof {
		log_user 0;
		return "-1 - hw_maint_lynx_exec: timeout during cmd:\n$cmd";
	}
	}
	#
	log_user 0;
	return "0 - success";
}
#
proc hw_maint_sp_sanity_checks { labid allspips spids { timeout 15 } } {
	upvar $allspips spips;
	upvar $spids spawn_ids;
	#
	foreach resource [list lynx-a lynx-b] {
		#
		# recover IP connectivity
		#
		puts "\nResetting IP interface for ${resource}:"
		#
		puts "\nIP interface for ${resource} before reset:"
		hw_maint_lynx_exec ${resource} spawn_ids "ifconfig mgt0";
		#
		hw_maint_lynx_exec ${resource} spawn_ids "sed 's/mgt0_ip.*/mgt0_ip=$spips(${resource})/' /Telica/dbCurrent/Telica_IP >/tmp/[exp_pid].out";
		hw_maint_lynx_exec ${resource} spawn_ids "mv /tmp/[exp_pid].out /Telica/dbCurrent/Telica_IP";
		hw_maint_lynx_exec ${resource} spawn_ids "/bin/iprecover";
		#
		puts "\nIP interface for ${resource} after reset:"
		hw_maint_lynx_exec ${resource} spawn_ids "ifconfig mgt0";
		#
		# delete some old files, log and backup files.
		#
		puts "\nDeleting old LOG and BACKUP files for ${resource}:"
		#
		puts "\nDisk usage on ${resource} before deletes:"
		hw_maint_lynx_exec ${resource} spawn_ids "df";
		#
		hw_maint_lynx_exec ${resource} spawn_ids "rm -f /Telica/localbackups*/* 1>/dev/null 2>&1";
		hw_maint_lynx_exec ${resource} spawn_ids "rm -rf /Telica/swCPU/*/oldLog* 1>/dev/null 2>&1";
		#
		puts "\nDisk usage on ${resource} after deletes:"
		hw_maint_lynx_exec ${resource} spawn_ids "df";
		#
		# fix any file system problems
		#
		puts "\nRunning fsck on file systems on ${resource}:"
		#
		hw_maint_lynx_exec ${resource} spawn_ids "echo 'y' >/tmp/[exp_pid].ans";
		hw_maint_lynx_exec ${resource} spawn_ids "fsck -f /dev/hd2b </tmp/[exp_pid].ans";
		hw_maint_lynx_exec ${resource} spawn_ids "fsck -f /dev/hd2c </tmp/[exp_pid].ans";
		hw_maint_lynx_exec ${resource} spawn_ids "fsck -f /dev/hd2d </tmp/[exp_pid].ans";
	}
	#
	return "0 - success";
}
#
proc hw_maint_recover_sps { labid allspips tl1username tl1passwd lynxusername lynxpasswd useskdb { maxiom 17 } { polltime 30 } { maxtime 1800 } { maxresets 5 } } {
	upvar $allspips spips;
	# diagnose and recover sps
	puts "\nStart ${labid} SP Diagnostics/Recovery.";
	# release all console users, if any
	set status [hw_maint_clear_console_users $labid];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_recover_sps: hw_maint_clear_console_users failed:\n${status}";
	}
	# attempt to logon the SPs, fix any problems on the way.
	set status [hw_maint_logon_lynx_consoles $labid spips $lynxusername $lynxpasswd spawn_ids $useskdb];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_recover_sps: hw_maint_logon_lynx_consoles failed:\n${status}";
	}
	# check for space, IPs, processes running, etc. fix if any problems.
	set status [hw_maint_sp_sanity_checks $labid spips spawn_ids];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_recover_sps: hw_maint_sp_sanity_checks failed:\n${status}";
	}
	# verification
	puts "\nSP-A Sanity checks ...";
	hw_maint_lynx_exec "lynx-a" spawn_ids "ps xf";
	hw_maint_lynx_exec "lynx-a" spawn_ids "df";
	hw_maint_lynx_exec "lynx-a" spawn_ids "ifconfig mgt0";
	puts "\nSP-B Sanity checks ...";
	hw_maint_lynx_exec "lynx-b" spawn_ids "ps xf";
	hw_maint_lynx_exec "lynx-b" spawn_ids "df";
	hw_maint_lynx_exec "lynx-b" spawn_ids "ifconfig mgt0";
	# close all streams
	hw_maint_close_all spawn_ids;
	# done
	return "0 - success";
}
#
proc hw_maint_min_recover_sps { labid allspips tl1username tl1passwd lynxusername lynxpasswd useskdb { maxiom 17 } { polltime 30 } { maxtime 1800 } { maxresets 5 } } {
	upvar $allspips spips;
	# diagnose and recover sps
	puts "\nStart ${labid} SP Minimal Recovery.";
	# release all console users, if any
	puts "\nKill Current Users of ${labid} PXmonitor.";
	set status [hw_maint_clear_console_users $labid];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_min_recover_sps: hw_maint_clear_console_users failed:\n${status}";
	}
	# attempt to logon the SPs, fix any problems on the way.
	puts "\nAttempting to Logon SP-A and SP-B of ${labid}.";
	set status [hw_maint_logon_lynx_consoles $labid spips $lynxusername $lynxpasswd spawn_ids $useskdb];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_min_recover_sps: hw_maint_logon_lynx_consoles failed:\n${status}";
	}
	# close all streams
	puts "\nClosing Streams to SP-A and SP-B of ${labid}.";
	hw_maint_close_all spawn_ids;
	# done
	return "0 - success";
}
#
proc hw_maint_audit_sps { labid argspdata lynxusername lynxpasswd fixflag { maxretries 2 } { retrydelay 15 } } {
	upvar $argspdata spdata;
	# start this up
	puts "\nStart ${labid} SPs Audit:";
	# login each machine via pxmonitor.
	foreach resource [list lynx-a lynx-b] {
		# init all counters
		set retries 0;
		# set interrupt character to delete, not control-C.
		set stty_init "intr ";
		# logon each sp via the console
		spawn -noecho pxmonitor -L ${labid} -t ${resource};
		#
		set spawn_ids(${resource}) $spawn_id;
		#
		expect {
		-re {Starting.*interactive.*mode} {
			send "\r";
			exp_continue;
		}
		-re {.*login:} {
			send "${lynxusername}\r";
			exp_continue;
		}
		-re {.*user name:} {
			send "${lynxusername}\r";
			exp_continue;
		}
		-re {.*[pP]assword:} {
			send "${lynxpasswd}\r";
			exp_continue;
		}
		-re ".*PS1=\"COMMAND_IS_DONE_[exp_pid]>\"" {
			exp_continue;
		}
		-re ".*COMMAND_IS_DONE_[exp_pid]>" {
			# ok.
		}
		-re {ibm750.*->} {
			# UGH. We have an SP stuck in the firmware mode.
			puts "\nRESOURCE ${resource} FOUND AT (ibm*) PROMPT.";
			puts "ATTEMPTING TO RECOVER ${resource}.";
			send "auto\r";
			exp_continue;
		}
		-re {Are.*you.*sure.*reboot.*system} {
			# yup, kick this dog.
			send "y";
			exp_continue;
		}
		-re {.*> } {
			send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
			exp_continue;
		}
		-re {.*# } {
			send "PS1=\"COMMAND_IS_DONE_[exp_pid]>\"\r";
			exp_continue;
		}
		-re {[\r\n]\* } {
			# SKDB prompt. reboot.
			puts "\nRESOURCE ${resource} FOUND AT skdb PROMPT.";
			puts "ATTEMPTING TO RECOVER ${resource}.";
			send "R\r"
			exp_continue;
		}
		timeout {
			puts "\nTime out waiting for pxmonitor ${labid} ${resource}";
			# incr retries;
			# if ($retries>$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_audit_sps: Time out waiting for pxmonitor ${labid} ${resource}";
			# }
			# puts "\nTime out. Retry $retries for ${resource}."
			# hw_maint_close $spawn_id;
			# sleep $retrydelay;
			# set interrupt character to delete, not control-C.
			# set stty_init "intr ";
			# spawn -noecho pxmonitor -L ${labid} -t ${resource};
			# set spawn_ids(${resource}) $spawn_id;
			# exp_continue;
		}
		eof {
			puts "\nEOF waiting for pxmonitor ${labid} ${resource}";
			# incr retries;
			# if ($retries>$maxretries) {
				hw_maint_close_all spawn_ids;
				return "-1 - hw_maint_audit_sps: EOF waiting for pxmonitor ${labid} ${resource}";
			# }
			# puts "\nEOF. Retry $retries for ${resource}."
			# hw_maint_close $spawn_id;
			# sleep $retrydelay;
			# set interrupt character to delete, not control-C.
			# set stty_init "intr ";
			# spawn -noecho pxmonitor -L ${labid} -t ${resource};
			# set spawn_ids(${resource}) $spawn_id;
			# exp_continue;
		}
		}
	}
	# we are logged on  both SPs, now check the the IPs.
	foreach resource [list lynx-a lynx-b] {
		if {![info exists spawn_ids($resource)]} {
			puts "\nResource ${labid} $resource spawn_id does not exist. Skipping it.";
			continue;
		}
		set spawn_id $spawn_ids($resource);
		#
		send "ifconfig mgt0\n";
		#
		expect {
		-re {inet ([^ ][^ ]*) netmask.*[\r\n]} {
			set spip $expect_out(1,string);
			puts "\nLab ${labid} ${resource} IP: ${spip}";
			#
			exp_continue;
		}
		-re ".*COMMAND_IS_DONE_[exp_pid]>" {
			# ok.
			if {$spdata($labid,$resource) == $spip} {
				puts "$resource IP ($spdata($labid,$resource)) matches expected IP ($spip).";
			} else {
				puts "$resource IP ($spdata($labid,$resource)) does NOT match expected IP ($spip).";
				if {$fixflag} {
					puts "\nResetting IP interface for ${resource}:"
					#
					puts "\nIP interface for ${resource} before reset:"
					hw_maint_lynx_exec ${resource} spawn_ids "ifconfig mgt0";
					#
					hw_maint_lynx_exec ${resource} spawn_ids "sed 's/mgt0_ip.*/mgt0_ip=$spdata(${labid},${resource})/' /Telica/dbCurrent/Telica_IP >/tmp/[exp_pid].out";
					hw_maint_lynx_exec ${resource} spawn_ids "mv /tmp/[exp_pid].out /Telica/dbCurrent/Telica_IP";
					hw_maint_lynx_exec ${resource} spawn_ids "/bin/iprecover";
					#
					puts "\nIP interface for ${resource} after reset:"
					hw_maint_lynx_exec ${resource} spawn_ids "ifconfig mgt0";
				}
			}
		}
		timeout {
			puts "\nTime out waiting for 'ifconfig mgt0' results for $resource.";
			continue;
		}
		eof {
			puts "\nEOF waiting for 'ifconfig mgt0' results for $resource.";
			continue;
		}
		}
	}
	#
	hw_maint_close_all spawn_ids;
	# done
	return "0 - success";
}
#
# get list of loads availabla on an SP.
#
proc hw_maint_getsploads { labid spip splabel spdata_name { lynxusername "root" } { lynxpasswd "plexus9000" } { verbose 0 } } {
	upvar $spdata_name spdata;
	#
	puts "\nGetting the list of loads on LABID ${labid} SP ${splabel} SP-IP ${spip}:";
	#
	puts "\nOpen TELNET session to ${splabel} ${spip}:";
	set status [hw_maint_telnet_open telnet_spawn_id $spip $lynxusername $lynxpasswd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_telnet_open failed:\n${status}";
	}
	#
	puts "\nOpen FTP session to ${splabel} ${spip}:";
	set status [hw_maint_ftp_open ftp_spawn_id $spip $lynxusername $lynxpasswd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_ftp_open failed:\n${status}";
	}
	#
	puts "\nGetting list of IOM loads on ${splabel} ${spip}:";
	set iomloads "/tmp/iomloads.out.[pid]";
	#
	set status [hw_maint_telnet_exec telnet_spawn_id "/bin/ls -dl /Telica/sw*/*/* 2>/dev/null | grep -v swCPU | sed -n 's/  */ /gp' >${iomloads}" 60 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_telnet_exec failed:\n${status}";
	}
	#
	set status [hw_maint_ftp_get_file ftp_spawn_id $iomloads $iomloads 1 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_ftp_get_file failed:\n${status}";
	}
	#
	puts "\nGetting list of CPU loads on ${splabel} ${spip}:";
	set cpuloads "/tmp/cpuloads.out.[pid]";
	#
	set status [hw_maint_telnet_exec telnet_spawn_id "/bin/ls -dl /Telica/swCPU/* /home1/Telica/swCPU/* 2>/dev/null | grep -v core | sed -n 's/  */ /gp' >${cpuloads}" 60 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_telnet_exec failed:\n${status}";
	}
	#
	set status [hw_maint_ftp_get_file ftp_spawn_id $cpuloads $cpuloads 1 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploads: hw_maint_ftp_get_file failed:\n${status}";
	}
	#
	puts "\nClosing TELNET session to ${splabel} ${spip}:";
	hw_maint_close $telnet_spawn_id;
	#
	puts "\nClosing FTP session to ${splabel} ${spip}:";
	hw_maint_close $ftp_spawn_id;
	#
	puts "\nProcessing CPU data for ${splabel} ${spip}:\n";
	set cpufd [open $cpuloads "r"];
	for {set idx 1} {[gets $cpufd cpubuf] >= 0 && ![eof $cpufd]} { } {
		set cpubuf [string trim $cpubuf];
		if {[regexp -- {^.*/CurrRel  *->  *(.*)$} "${cpubuf}" ignore currelpath] == 1} {
			puts "CPU CURREL: ${currelpath}";
			set spdata(${splabel},cpudata,currel) "${currelpath}";
		} elseif {[regexp -- {^.*(/home1/Telica/(sw[^/]*)/.*)$} "${cpubuf}" ignore path] == 1 || 
		          [regexp -- {^.*(/Telica/(sw[^/]*)/.*)$} "${cpubuf}" ignore path] == 1} {
			puts "CPU PATH: ${path}";
			set spdata(${splabel},cpudata,${idx},path) "${path}";
			incr idx;
		} else {
			continue;
		}
	}
	close $cpufd;
	#
	puts "\nProcessing IOM data for ${splabel} ${spip}:\n";
	set iomfd [open $iomloads "r"];
	set oldiomswtype "";
	set oldiomswload "";
	for {set idx 1} {[gets $iomfd iombuf] >= 0 && ![eof $iomfd]} { } {
		set iombuf [string trim $iombuf];
		if {[regexp -- {^.*(/Telica/(sw[^/]*)/([^/]*)/.*)$} "${iombuf}" ignore path iomswtype iomswload] != 1} {
			continue;
		}
		if {"${oldiomswtype}" != "${iomswtype}" ||
		    "${oldiomswload}" != "${iomswload}"} {
			set oldiomswtype ${iomswtype};
			set oldiomswload ${iomswload};
			lappend spdata(${splabel},iomtypes) ${iomswtype};
			lappend spdata(${splabel},${iomswtype},loads) ${iomswload};
			set idx 1;
			puts "";
		}
		if {[regexp -- {^(.*)  *->  *(.*)$} "${path}" ignore linkpath actualfile] == 1} {
			puts "IOM ${iomswtype} LINK: ${linkpath}";
			puts "IOM ${iomswtype} FILE: ${actualfile}";
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},path) "${linkpath}";
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},file) "${actualfile}";
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},type) "link";
		} else {
			puts "IOM ${iomswtype} PATH: ${path}";
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},path) "${path}";
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},file) [file tail ${path}];
			set spdata(${splabel},iomdata,${iomswtype},${iomswload},${idx},type) "path";
		}
		incr idx;
	}
	close $iomfd;
	#
	return "0 - success";
}
proc hw_maint_getsploadsonly { labid spip splabel spdata_name { lynxusername "root" } { lynxpasswd "plexus9000" } { verbose 0 } } {
	upvar $spdata_name spdata;
	#
	puts "\nGetting the list of loads on LABID ${labid} SP ${splabel} SP-IP ${spip}:";
	#
	puts "\nOpen TELNET session to ${splabel} ${spip}:";
	set status [hw_maint_telnet_open telnet_spawn_id $spip $lynxusername $lynxpasswd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploadsonly: hw_maint_telnet_open failed:\n${status}";
	}
	#
	puts "\nOpen FTP session to ${splabel} ${spip}:";
	set status [hw_maint_ftp_open ftp_spawn_id $spip $lynxusername $lynxpasswd];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploadsonly: hw_maint_ftp_open failed:\n${status}";
	}
	#
	puts "\nGetting list of all loads on ${splabel} ${spip}:";
	set allloads "/tmp/allloads.out.[pid]";
	#
	set status [hw_maint_telnet_exec telnet_spawn_id "/bin/ls -dl /Telica/sw*/* /home1/Telica/sw*/* 2>/dev/null | grep -v core | grep -v -- '->' | sed -n 's/  */ /gp' >${allloads}" 60 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploadsonly: hw_maint_telnet_exec failed:\n${status}";
	}
	set status [hw_maint_telnet_exec telnet_spawn_id "/bin/ls -dl /Telica/swCPU/CurrRel 2>/dev/null | grep -- '->' | sed -n 's/  */ /gp' >>${allloads}" 60 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploadsonly: hw_maint_telnet_exec failed:\n${status}";
	}
	#
	set status [hw_maint_ftp_get_file ftp_spawn_id $allloads $allloads 1 ${verbose} ];
	if {[isNotOk $status]} {
		return "-1 - hw_maint_getsploadsonly: hw_maint_ftp_get_file failed:\n${status}";
	}
	#
	puts "\nClosing TELNET session to ${splabel} ${spip}:";
	hw_maint_close $telnet_spawn_id;
	#
	puts "\nClosing FTP session to ${splabel} ${spip}:";
	hw_maint_close $ftp_spawn_id;
	#
	puts "\nProcessing data for ${splabel} ${spip}:\n";
	set allfd [open $allloads "r"];
	for {set idx 1} {[gets $allfd allbuf] >= 0 && ![eof $allfd]} { } {
		set allbuf [string trim $allbuf];
		if {[regexp -- {^.*/Telica/swCPU/CurrRel *-> *(.*)$} "${allbuf}" ignore path] == 1} {
			set spdata(${splabel},currrel,path) "${path}";
			puts "CURRREL PATH: ${path}";
		} elseif {[regexp -- {^.*(/home1/Telica/(sw[^/]*)/([^/]*))$} "${allbuf}" ignore path swtype swload] == 1} {
			set spdata(${splabel},${idx},path) "${path}";
			puts "PATH ${idx}: ${path}";
		} elseif {[regexp -- {^.*(/Telica/(sw[^/]*)/([^/]*))$} "${allbuf}" ignore path swtype swload] == 1} {
			set spdata(${splabel},${idx},path) "${path}";
			puts "PATH ${idx}: ${path}";
		} else {
			continue;
		}
		incr idx;
	}
	close $allfd;
	#
	return "0 - success";
}
#
############################################################################
#
proc hw_maint_cleanup { labid spips_name lynxusername lynxpasswd splist { docurrcpu 0 } { dooldcpu 1 } { docurriom 0 } { dooldiom 1 } } {
	upvar ${spips_name} spips;
	# 
	puts "\nStart ${labid} Clean Up for SPs: ${splist}";
	#
	foreach sp ${splist} {
		set spip $spips(${sp});
		#
		puts "\nProcessing SP: ${sp} ${spip}";
		#
		# get lists of loads on this sp.
		#
		puts "\nGeting list of loads on SP: ${sp} ${spip}";
		set status [hw_maint_getsploads ${labid} ${spip} ${sp} spdata ${lynxusername} ${lynxpasswd}];
		if {[isNotOk $status]} {
			return "-1 - hw_maint_cleanup: hw_maint_getsploads failed:\n${status}";
		}
	}
	#
	return "0 - success";
}
