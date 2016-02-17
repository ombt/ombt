# functions related to hardware
package provide hughardware 1.0
#
package require checkretval
#
proc hug_hardware_cleanup { spawn_id } {
	catch { close; } ignore;
	catch { exec kill -9 [exp_pid]; } ignore;
	catch { wait -nowait; } ignore;
}
#
proc hw_tl1_open { spid spip login passwd { vflag 0 } { tmout 15 } { tl1port 2361 } { maxlogons 12 } { maxtl1logons 5 } } {
	upvar $spid spawn_id;
	#
	for {set i 0} {$i<$maxlogons} {incr i} {
		spawn -noecho "/usr/bin/telnet" $spip $tl1port;
		#
		expect {
		"Connected to " {
			# ok
			break;
		}
		"Connection refused" {
			hug_hardware_cleanup $spawn_id;
			puts "\nConnection refused. Sleep and retry.";
			sleep 5;
		}
		timeout {
			hug_hardware_cleanup $spawn_id;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
			# return "-1 - hw_tl1_open: time out during telnet ($spip,$login).";
		}
		eof {
			hug_hardware_cleanup $spawn_id;
			puts "\nEOF. Sleep and retry.";
			sleep 5;
			# return "-1 - hw_tl1_open: eof during telnet ($spip,$login).";
		}
		}
	}
	if {$i>=$maxlogons} {
		return "-1 - hw_tl1_open: unable to telnet to ($spip,$login).";
	}
	#
	sleep 2;
	#
	send "\r";
	expect {
	"Telica>" {
		# ok
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: timeout waiting for prompt ($spip,$login).";
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: eof waiting for prompt ($spip,$login).";
	}
	}
	#
	send "act-user::${login}:::${passwd};\r";
	set tl1logons 1;
	#
	expect {
	-re "This is the WORKING System Processor.*M.*\[0-9]+.*COMPLD.*;\r\nTelica>" {
		puts "\nCOMPLETED received ...";
	}
	-re "Host is in Standby Mode.*;\r\nTelica>" {
		puts "\nHost is in Standby Mode ($spip,$login).";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: Host is in Standby Mode ($spip,$login).";
	}
	-re "This is the PROTECTION System Processor.*;\r\nTelica>" {
		puts "\nThis is the PROTECTION System Processor ($spip,$login).";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: This is the PROTECTION System Processor ($spip,$login).";
	}
	-re "Login Not Active.*;\r\nTelica>" {
		puts "\nLogin Not Active ($spip,$login).";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: Login Not Active ($spip,$login).";
	}
	-re "Can't login.*;\r\nTelica>" {
		puts "\nCan't login ($spip,$login).";
		if {${tl1logons} <= ${maxtl1logons}} {
			puts "\nSleep and retry TL1 logon (${tl1logon}) for user ${login}.";
			sleep 30;
			send "act-user::${login}:::${passwd};\r";
			incr tl1logons;
			exp_continue;
		}
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: Can't login ($spip,$login).";
	}
	-re ".*;\r\nTelica>" {
		puts "\nUnknown error ($spip,$login).";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: Unknown error ($spip,$login).";
	}
	timeout {
		puts "\ntimeout waiting for a response ...";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: timeout waiting for act-user response ($spip,$login).";
	}
	eof {
		puts "\neof waiting for a response ...";
		hug_hardware_cleanup $spawn_id;
		return "-1 - hw_tl1_open: eof waiting for act-user response ($spip,$login).";
	}
	}
	#
	return "0 - success";
}
#
proc hw_telnet_open { spid spip login passwd { vflag 0 } { tmout 15 } } {
	upvar $spid spawn_id;
	#
	set timeout $tmout;
	#
	spawn -noecho telnet $spip;
	expect {
	"Connected to " {
		# ok, continue
	}
	"Connection refused" {
		hug_hardware_cleanup $spawn_id;
		puts "-1 - hw_telnet_open: telnet connection refused for $spip";
		return "-1 - hw_telnet_open: telnet connection refused for $spip";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: telnet timeout for $spip";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: telnet eof for $spip";
		puts $status;
		return $status;
	}
	}
	#
	expect {
	"user name:" {
		send "$login\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: username timeout for $spip";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: username eof for $spip";
		puts $status;
		return $status;
	}
	}
	#
	expect {
	"password:" {
		send "$passwd\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: passwd timeout for $spip";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: passwd eof for $spip";
		puts $status;
		return $status;
	}
	}
	#
	expect {
	-re "\[a-z]*\[0-9]+\[ab]#" {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "\[uU]nnamed_\[Ss]ystem#" {
		puts "WARNING: UNIX is active, but 'unnamed_system' prompt was received.";
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: PS1 timeout for $spip";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: PS1 eof for $spip";
		puts $status;
		return $status;
	}
	}
	#
	expect {
	-re ".*COMMAND_IS_DONE>" {
		# ok
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: new PS1 timeout for $spip";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "-1 - hw_telnet_open: new PS1 eof for $spip";
		puts $status;
		return $status;
	}
	}
	#
	puts "0 - success";
	return "0 - success";
}
#
proc iomStatesMatch { cfgdata iom } {
	upvar $cfgdata cfgd;
	#
	puts "";
	puts "Do iomStatesMatch (iom reqst st) = ($iom $cfgd(iom,$iom,reqiomstate) $cfgd(iom,$iom,iomstate)) ?";
	#
	regsub -all -- "," $cfgd(iom,$iom,reqiomstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	#
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {$reqstate == "ignore"} {
			# always a match
			puts "Yes ...";
			return 1;
		} elseif {$reqstate == $cfgd(iom,$iom,iomstate)} {
			puts "Yes ...";
			return 1;
		}
	}
	puts "No ...";
	return 0;
}
proc cpuStatesMatch { cfgdata spip } {
	upvar $cfgdata cfgd;
	#
	puts "";
	puts "Do cpuStatesMatch (sp reqst st) = ($spip $cfgd($spip,reqspstate) $cfgd($spip,spstate)) ?";
	#
	regsub -all -- "," $cfgd($spip,reqspstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	#
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {$reqstate == "ignore"} {
			# always a match
			puts "Yes ...";
			return 1;
		} elseif {$reqstate == $cfgd($spip,spstate)} {
			puts "Yes ...";
			return 1;
		}
	}
	puts "No ...";
	return 0;
}
#
# we have an interesting problem with states, it's really a hierarchy question.
# suppose the requested state is is,nbk, but the switch is already set at
# is,act, then what do we do? do we return false since the state can never
# be is,nbk since it already passed this state, or do we return true since the
# current state is actually  a "higher" level of operation than the is,nbk
# state? I'm going to assume that the switch was booted and the state will be
# transitioned and be met. Now, will we be able to catch a state or will it 
# transition to quickly? We'll see.
#
# another problem to consider, what if the switch takes a step back from 
# a given state, rather than progressing. A very common example is when
# synchronization fails and the state can go from is,act,sync to is,nbk.
# this represents a step back, I will mark this as a failure.
#
# further thoughts on this problem. if the requested state is ping, then
# update each state independently. the same for unix states, appmode or 
# minmode, do each independently. But, if the state is a TL1 state, then
# only the active SP can update both SP statuses. we don't want the stand-by
# sp overwriting it's status once the active SP has set it.
#
proc hug_sptl1active { cfgdata vflag tmout spdata sp { tl1port 2361 } { maxlogons 12 } { sleepbeforeescape 2 } } {
	upvar $cfgdata cfgd;
	upvar $spdata spd;
	#
	# puts "ENTERING sptl1active ... $sp";
	set timeout 30;
	#
	for {set i 0} {$i<$maxlogons} {incr i} {
		spawn -noecho "/usr/bin/telnet" $spd($sp,ip) $tl1port;
		#
		expect {
		"Connected to " {
			# ok
			break;
		}
		"Connection refused" {
			hug_hardware_cleanup $spawn_id;
			puts "\nConnection refused. Sleep and retry.";
			sleep 5;
		}
		timeout {
			hug_hardware_cleanup $spawn_id;
			puts "\nTime out. Sleep and retry.";
			sleep 5;
		}
		eof {
			hug_hardware_cleanup $spawn_id;
			puts "\nEOF. Sleep and retry.";
			sleep 5;
		}
		}
	}
	if {$i>=$maxlogons} {
		hug_hardware_cleanup $spawn_id;
		puts "0 - unable to telnet to $spd($sp,ip).";
		return "0 - unable to telnet to $spd($sp,ip).";
	}
	#
	sleep $sleepbeforeescape;
	#
	set msg "Waiting for Telica prompt -";
	#
	send "\r";
	expect {
	"Telica>" {
		# ok
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg timeout for $spd($sp,ip)";
		return "0 - $msg timeout for $spd($sp,ip)";
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg eof for $spd($sp,ip)";
		return "0 - $msg eof for $spd($sp,ip)";
	}
	}
	#
	set msg "act-user -";
	#
	send "act-user::$spd($sp,tl1login):::$spd($sp,tl1passwd);\r";
	#
	expect {
	-re "This is the WORKING System Processor.*M.*\[0-9]+.*COMPLD.*;\r\nTelica>" {
		# set spd($sp,state) "isactnbk";
		# set cfgd($spd($sp,ip),spstate) "isactnbk";
		# continue ...
	}
	-re "Host is in Standby Mode.*;\r\nTelica>" {
		# stop here. cannot do rtrv-eqpt on stand-by side.
		# set spd($sp,state) "isstbyc";
		# set cfgd($spd($sp,ip),spstate) "isstbyc";
		hug_hardware_cleanup $spawn_id;
		puts "0 - Host is in Standby Mode ($spd($sp,ip)).";
		return "0 - Host is in Standby Mode ($spd($sp,ip)).";
	}
	-re "This is the PROTECTION System Processor.*;\r\nTelica>" {
		# stop here. cannot do rtrv-eqpt on stand-by side.
		# set spd($sp,state) "isstbyc";
		# set cfgd($spd($sp,ip),spstate) "isstbyc";
		hug_hardware_cleanup $spawn_id;
		puts "0 - This is the PROTECTION System Processor ($spd($sp,ip)).";
		return "0 - This is the PROTECTION System Processor ($spd($sp,ip)).";
	}
	-re "Login Not Active.*;\r\nTelica>" {
		hug_hardware_cleanup $spawn_id;
		puts "-1 - hug_sptl1active: Login Not Active ($spd($sp,tl1login))";
		return "-1 - hug_sptl1active: Login Not Active ($spd($sp,tl1login))";
	}
	-re "Can't login.*;\r\nTelica>" {
		hug_hardware_cleanup $spawn_id;
		puts "0 - hug_sptl1active: Can't login ($spd($sp,tl1login)).";
		return "0 - hug_sptl1active: Can't login ($spd($sp,tl1login)).";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg timeout for $spd($sp,ip)";
		return "0 - $msg timeout for $spd($sp,ip)";
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg eof for $spd($sp,ip)";
		return "0 - $msg eof for $spd($sp,ip)";
	}
	}
	#
	set msg "RTRV-EQPT-ALL -";
	#
	send "RTRV-EQPT-ALL;\r";
	#
# exp_internal 1;
	expect {
	-re {IOM-([0-9]+):IOMODULETYPE=([^,]*),REDUNDANCY=([^,]*),MEMTHRESHOLD=([^,]*),NVMEMTHRESHOLD=([^,]*),BUSYTHRESHOLD=([^,]*),SWVERSION=([^,]*),REARMODULETYPE=([^,]*),APPSWVERSION=([^,]*):([A-Z0-9,&-]+)"} {
		set iom $expect_out(1,string);
		set clei [string tolower $expect_out(2,string)];
		set protection [string tolower $expect_out(3,string)];
		set version $expect_out(7,string);
		set rearclei [string tolower $expect_out(8,string)];
		#
		if {${rearclei} == "UNPROV" || ${rearclei} == "unprov"} {
			set rearclei "none";
		} elseif {${rearclei} == "NULL" || ${rearclei} == "null"} {
			set rearclei "none";
		} elseif {[string length "${rearclei}"] <= 0} {
			set rearclei "none";
		}
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
		set cfgd(iom,$iom,iomstate) $iomstate;
		set cfgd(iom,$iom,clei) $clei;
		set cfgd(iom,$iom,rearclei) $rearclei;
		set cfgd(iom,$iom,protection) $protection;
		set cfgd(iom,$iom,version) $version;
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
		if {${rearclei} == "UNPROV" || ${rearclei} == "unprov"} {
			set rearclei "none";
		} elseif {${rearclei} == "NULL" || ${rearclei} == "null"} {
			set rearclei "none";
		} elseif {[string length "${rearclei}"] <= 0} {
			set rearclei "none";
		}
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
		set cfgd(iom,$iom,iomstate) $iomstate;
		set cfgd(iom,$iom,clei) $clei;
		set cfgd(iom,$iom,rearclei) $rearclei;
		set cfgd(iom,$iom,protection) $protection;
		set cfgd(iom,$iom,version) $version;
		# log_user 1;
		exp_continue;
	}
	-re {SP-A:[^"]*:([A-Z0-9,&-]+)"} {
		puts "";
		puts "SP-A $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		if {!$cfgd($spd(spa,ip),spstatesetbyrtrveqpt)} {
			set spd(spa,state) $spstate;
			set cfgd($spd(spa,ip),spstate) $spstate;
			set cfgd($spd(spa,ip),spstatesetbyrtrveqpt) 1;
		}
		# log_user 1;
		exp_continue;
	}
	-re {SP-B:[^"]*:([A-Z0-9,&-]+)"} {
		puts "";
		puts "SP-B $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" spstate;
		set spstate [string tolower $spstate];
		if {!$cfgd($spd(spb,ip),spstatesetbyrtrveqpt)} {
			set spd(spb,state) $spstate;
			set cfgd($spd(spb,ip),spstate) $spstate;
			set cfgd($spd(spb,ip),spstatesetbyrtrveqpt) 1;
		}
		# log_user 1;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		# done
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg timeout for $spd($sp,ip)";
		return "0 - $msg timeout for $spd($sp,ip)";
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg eof for $spd($sp,ip)";
		return "0 - $msg eof for $spd($sp,ip)";
	}
	}
# exp_internal 0;
	#
	hug_hardware_cleanup $spawn_id;
	puts "0 - success";
	return "0 - success";
}
#
proc hug_spunixactive { cfgdata vflag tmout spdata sp  } {
	upvar $cfgdata cfgd;
	upvar $spdata spd;
	#
	# puts "ENTERING spunixactive ... $sp";
	set timeout 30;
	#
	set msg "telnet";
	spawn -noecho telnet $spd($sp,ip);
	expect {
	"Connected to " {
		# ok, continue
	}
	"Connection refused" {
		hug_hardware_cleanup $spawn_id;
		puts "0 - connection refused for $spd($sp,ip)";
		return "0 - connection refused for $spd($sp,ip)";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	set msg "username";
	expect {
	"user name:" {
		send "$spd($sp,login)\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	set msg "passwd";
	expect {
	"password:" {
		send "$spd($sp,passwd)\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	set msg "PS1";
	expect {
	-re "\[a-z]*\[0-9]+\[ab]#" {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "\[a-zA-Z0-9_-]+#" {
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	-re "\[uU]nnamed_\[Ss]ystem#" {
		puts "WARNING: UNIX is active, but 'unnamed_system' prompt was received.";
		send "PS1=\"COMMAND_IS_DONE>\"\r";
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	set msg "PROMPT";
	expect {
	-re ".*COMMAND_IS_DONE>" {
		send "\[ -x /Telica/swCPU/CurrRel/system/scripts/telica.rc.network ] && echo \"APPLICATIONMODE\";\r";
	}
	}
	#
	set msg "APPLICATIONMODE";
	expect {
	-re ".*\"APPLICATIONMODE\";" {
		# ok
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	set msg "Setting MODE";
	expect {
	-re ".*APPLICATIONMODE" {
		if {!$cfgd($spd($sp,ip),spstatesetbyrtrveqpt)} {
			set spd($sp,state) "appmode";
			set cfgd($spd($sp,ip),spstate) "appmode";
		}
		# is tl1 active?
		set status [hug_sptl1active cfgd vflag tmout spd "$sp"];
		if {[isNotOk $status]} {
			hug_hardware_cleanup $spawn_id;
			set status "-1 - hug_spunixactive: hug_sptl1active failed for SP ($spd($sp,ip)):\n$status";
			puts $status;
			return $status;
		}
	}
	-re "COMMAND_IS_DONE>" {
		if {!$cfgd($spd($sp,ip),spstatesetbyrtrveqpt)} {
			set spd($sp,state) "minmode";
			set cfgd($spd($sp,ip),spstate) "minmode";
		}
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} timeout for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		set status "0 - ${msg} eof for $spd($sp,ip)";
		puts $status;
		return $status;
	}
	}
	#
	hug_hardware_cleanup $spawn_id;
	puts "0 - success";
	return "0 - success";
}
#
proc hug_spping { cfgdata vflag tmout spdata sp  } {
	upvar $cfgdata cfgd;
	upvar $spdata spd;
	#
	# puts "ENTERING spping ... $sp";
	set timeout 30;
	#
	spawn ping $spd($sp,ip);
	expect {
	"alive" {
		# we can ping the sp.
		if {!$cfgd($spd($sp,ip),spstatesetbyrtrveqpt)} {
			set spd($sp,state) "ping";
			set cfgd($spd($sp,ip),spstate) "ping";
		}
		# is unix active?
		set status [hug_spunixactive cfgd vflag tmout spd $sp ];
		if {[isNotOk $status]} {
			set status "-1 - hug_spping: hug_spunixactive failed for SP ($spd($sp,ip)):\n$status";
		}
	}
	timeout {
		set spd($sp,state) "unknown";
		set cfgd($spd($sp,ip),spstate) "unknown";
		set status "0 - timeout for $spd($sp,ip)";
	}
	eof {
		set spd($sp,state) "unknown";
		set cfgd($spd($sp,ip),spstate) "unknown";
		set status "0 - eof for $spd($sp,ip)";
	}
	}
	#
	hug_hardware_cleanup $spawn_id;
	puts "$status";
	return "$status";
}
#
proc hug_spstatus { cfgdata vflag tmout spdata } {
	upvar $cfgdata cfgd;
	upvar $spdata spd;
	#
	set spd(spa,state) "unknown";
	set cfgd($spd(spa,ip),spstate) "unknown";
	set spd(spb,state) "unknown";
	set cfgd($spd(spb,ip),spstate) "unknown";
	#
	# can we ping the sps.
	#
	if {$spd(spa,reqstate) != "ignore"} {
		# can switch be pinged?
		set status [hug_spping cfgd vflag tmout spd "spa"];
		if {[isNotOk $status]} {
			puts "-1 - hug_spstatus: hug_spping failed for SP-A ($spd(spa,ip)):\n$status";
			return "-1 - hug_spstatus: hug_spping failed for SP-A ($spd(spa,ip)):\n$status";
		}
	}
	if {$spd(spb,reqstate) != "ignore"} {
		# can switch be pinged?
		set status [hug_spping cfgd vflag tmout spd "spb"];
		if {[isNotOk $status]} {
			puts "-1 - hug_spstatus: hug_spping failed for SP-B ($spd(spb,ip)):\n$status";
			return "-1 - hug_spstatus: hug_spping failed for SP-B ($spd(spb,ip)):\n$status";
		}
	}
	#
	puts "0 - success";
	return "0 - success";
}
#
proc hug_waitforcpustate { cfgdata vflag tmout spdata { ignorefault 0 } { interiterationsleeptime 30 } } {
	upvar $cfgdata cfgd;
	upvar $spdata spd;
	# sanity checks
	#
	if {[string length $spd(spa,ip)] <= 0} {
		return "-1 - hug_waitforcpustate: spa ip is null length.";
	} elseif {[string length $spd(spb,ip)] <= 0} {
		return "-1 - hug_waitforcpustate: spb ip is null length.";
	} elseif {[string length $spd(spa,reqstate)] <= 0} {
		return "-1 - hug_waitforcpustate: spa reqstate is null length.";
	} elseif {[string length $spd(spb,reqstate)] <= 0} {
		return "-1 - hug_waitforcpustate: spb reqstate is null length.";
	} elseif {$tmout <= 0} {
		return "-1 - hug_waitforcpustate: timeout <= 0.";
	}
	#
	if {$vflag} {
		log_user 1;
	}
	#
	# save spa data
	puts "vflag         : $vflag";
	puts "tmout         : $tmout";
	puts "spaip         : $spd(spa,ip)";
	puts "spareqcpustate: $spd(spa,reqstate)";
	puts "spalogin      : $spd(spa,login)";
	puts "spapasswd     : $spd(spa,passwd)";
	puts "spatl1login   : $spd(spa,tl1login)";
	puts "spatl1passwd  : $spd(spa,tl1passwd)";
	puts "spbip         : $spd(spb,ip)";
	puts "spbreqcpustate: $spd(spb,reqstate)";
	puts "spblogin      : $spd(spb,login)";
	puts "spbpasswd     : $spd(spb,passwd)";
	puts "spbtl1login   : $spd(spb,tl1login)";
	puts "spbtl1passwd  : $spd(spb,tl1passwd)";
	#
	# save requested states
	#
	set cfgd($spd(spa,ip),reqspstate) $spd(spa,reqstate);
	set cfgd($spd(spb,ip),reqspstate) $spd(spb,reqstate);
	#
	set endtime [expr [clock seconds] + $tmout];
	#
	while { 1 } {
		# initialize states to unknown 
		#
		if {$cfgd($spd(spa,ip),reqspstate) != "ignore"} {
			# start from scratch
			set cfgd($spd(spa,ip),spstate) "unknown";
		} else {
			# ignore this SP
			set cfgd($spd(spa,ip),spstate) \
				$cfgd($spd(spa,ip),reqspstate);
		}
		if {$cfgd($spd(spb,ip),reqspstate) != "ignore"} {
			# start from scratch
			set cfgd($spd(spb,ip),spstate) "unknown";
		} else {
			# ignore this SP
			set cfgd($spd(spb,ip),spstate) \
				$cfgd($spd(spb,ip),reqspstate);
		}
		#
		set cfgd($spd(spa,ip),spstatesetbyrtrveqpt) 0;
		set cfgd($spd(spb,ip),spstatesetbyrtrveqpt) 0;
		#
		set status [hug_spstatus cfgd $vflag $tmout spd];
		if {[isNotOk $status]} {
			if {$vflag} {
				log_user 0;
			}
			return "-1 - hug_waitforcpustate: spstatus failed:\n$status";
		}
		puts "\nSTATUS FOR THIS ITERATION: $status";
		#
		# first check if we matched the requested states, then
		# check if either SP faulted. if either SP faulted, then
		# fail immediately.
		#
		if {[cpuStatesMatch cfgd $spd(spa,ip)] && \
		    [cpuStatesMatch cfgd $spd(spb,ip)]} {
			# done 
			break;
		}
		#
		set spastate $cfgd($spd(spa,ip),spstate);
		set spbstate $cfgd($spd(spb,ip),spstate);
		#
		if {!$ignorefault} {
			if {[regexp -- {.*oos.*flt.*} ${spastate}] == 1} {
				return "-1 - hug_waitforcpustate: SP-A has faulted: ${spastate}";
			} elseif {[regexp -- {.*oos.*flt.*} ${spbstate}] == 1} {
				return "-1 - hug_waitforcpustate: SP-B has faulted: ${spbstate}";
			}
		}
		# check if ran out of time.
		set currenttime [clock seconds];
		if {$currenttime > $endtime} {
			if {$vflag} {
				log_user 0;
			}
			return "-1 - hug_waitforcpustate: timeout waiting for status.";
		}
		#
		if {$interiterationsleeptime > 0} {
			sleep $interiterationsleeptime;
		}
	}
	if {$vflag} {
		log_user 0;
	}
	return "0 - success";
}
#
proc hug_rtrveqpt { cfgdata vflag tmout iomdata } {
	upvar $cfgdata cfgd;
	upvar $iomdata iomd;
	#
	puts "ENTERING rtrveqpt ... IOM = $iomd(iom)";
	set timeout 30;
	#
	set spawn_id $cfgd(tl1,$iomd(streamid),spawn_id);
	if {![info exists cfgd(tl1,$iomd(streamid),spawn_id)]} {
		puts "-1 - hug_rtrveqpt: stream id does not exist.";
		return "-1 - hug_rtrveqpt: stream id does not exist.";
	}
	set iom $iomd(iom);
	#
	set msg "RTRV-EQPT -";
	#
	send "RTRV-EQPT;\r";
	#
# exp_internal 1;
	expect {
	-re "IOM-$iom:\[^\"]*:(OOS-(AU|MA)*,(ACT|FLT|INIT|MEA|NBK|PRI|PSI|STBYC|STBYH|UAS|UEQ|UPDT|WRK|&)*)\"" {
		puts "IOM-$iom $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		set iomd(state) $iomstate;
		set cfgd(iom,$iomd(iom),iomstate) $iomstate;
		exp_continue;
	}
	-re "IOM-$iom:\[^\"]*:(OOS,(ACT|FLT|INIT|MEA|NBK|PRI|PSI|STBYC|STBYH|UAS|UEQ|UPDT|WRK|&)*)\"" {
		puts "IOM-$iom $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		set iomd(state) $iomstate;
		set cfgd(iom,$iomd(iom),iomstate) $iomstate;
		exp_continue;
	}
	-re "IOM-$iom:\[^\"]*:(IS,(ACT|FLT|INIT|MEA|NBK|PRI|PSI|STBYC|STBYH|UAS|UEQ|UPDT|WRK|&)*)\"" {
		puts "IOM-$iom $expect_out(1,string) ...";
		regsub -all -- "\[-,&]" $expect_out(1,string) "" iomstate;
		set iomstate [string tolower $iomstate];
		set iomd(state) $iomstate;
		set cfgd(iom,$iomd(iom),iomstate) $iomstate;
		exp_continue;
	}
	-re ";\r\nTelica>" {
		# done
	}
	timeout {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg timeout for IOM $iomd(iom)";
		return "0 - $msg timeout for IOM $iomd(iom)";
	}
	eof {
		hug_hardware_cleanup $spawn_id;
		puts "0 - $msg eof for IOM $iomd(iom)";
		return "0 - $msg eof for IOM $iomd(iom)";
	}
	}
# exp_internal 0;
	#
	puts "0 - success";
	return "0 - success";
}
#
proc hug_iomstatus { cfgdata vflag tmout iomdata } {
	upvar $cfgdata cfgd;
	upvar $iomdata iomd;
	#
	set iomd(state) "unknown";
	set cfgd(iom,$iomd(iom),iomstate) "unknown";
	#
	# can we ping the sps.
	#
	if {$iomd(reqstate) != "ignore"} {
		set status [hug_rtrveqpt cfgd vflag tmout iomd]; 
		if {[isNotOk $status]} {
			puts "-1 - hug_iomstatus: hug_rtrveqpt failed for IOM ($iomd(iom)):\n$status";
			return "-1 - hug_iomstatus: hug_rtrveqpt failed for IOM ($iomd(iom)):\n$status";
		}
	}
	#
	puts "0 - success";
	return "0 - success";
}
#
proc hug_waitforiomstate { cfgdata vflag tmout iomdata { interiterationsleeptime 30 } } {
	upvar $cfgdata cfgd;
	upvar $iomdata iomd;
	# sanity checks
	#
	if {[string length $iomd(streamid)] <= 0} {
		return "-1 - hug_waitforiomstate: stream id is null length.";
	} elseif {[string length $iomd(iom)] <= 0} {
		return "-1 - hug_waitforiomstate: iom is null length.";
	} elseif {[string length $iomd(reqstate)] <= 0} {
		return "-1 - hug_waitforiomstate: reqstate is null length.";
	} elseif {$tmout <= 0} {
		return "-1 - hug_waitforiomstate: timeout <= 0.";
	}
	#
	if {$vflag} {
		log_user 1;
	}
	#
	# save spa data
	puts "vflag   : $vflag";
	puts "tmout   : $tmout";
	puts "streamid: $iomd(streamid)";
	puts "iom     : $iomd(iom)";
	puts "reqstate: $iomd(reqstate)";
	#
	# save requested state
	#
	set cfgd(iom,$iomd(iom),reqiomstate) $iomd(reqstate);
	#
	set endtime [expr [clock seconds] + $tmout];
	#
	while { 1 } {
		# initialize states to unknown 
		#
		if {$cfgd(iom,$iomd(iom),reqiomstate) != "ignore"} {
			# start from scratch
			set cfgd(iom,$iomd(iom),iomstate) "unknown";
		} else {
			# ignore this IOM
			set cfgd(iom,$iomd(iom),iomstate) \
				$cfgd(iom,$iomd(iom),reqiomstate);
		}
		#
		set status [hug_iomstatus cfgd $vflag $tmout iomd];
		if {[isNotOk $status]} {
			if {$vflag} {
				log_user 0;
			}
			return "-1 - hug_waitforiomstate: iomstatus failed:\n$status";
		}
		puts "\nSTATUS FOR THIS ITERATION: $status";
		#
		if {[iomStatesMatch cfgd $iomd(iom)]} {
			break;
		}
		set currenttime [clock seconds];
		if {$currenttime > $endtime} {
			if {$vflag} {
				log_user 0;
			}
			return "-1 - hug_waitforiomstate: timeout waiting for status.";
		}
		#
		if {$interiterationsleeptime > 0} {
			sleep $interiterationsleeptime;
		}
	}
	if {$vflag} {
		log_user 0;
	}
	return "0 - success";
}
#
proc hug_GetCurrentLoads { cfgdata { verbose 0 } { tmout 60 } } {
	upvar $cfgdata cfgd;
	#
	# get CurrRel load
	#
	set swid $cfgd(globals,switchid);
	#
	set spdata(swid) $swid;
	set spdata(spa,ip) $cfgd(globals,$swid,spa,ip);
	set spdata(spb,ip) $cfgd(globals,$swid,spb,ip);
	#
	set login $cfgd(globals,$swid,login);
	set passwd $cfgd(globals,$swid,passwd);
	set tl1login $cfgd(globals,$swid,tl1login);
	set tl1passwd $cfgd(globals,$swid,tl1passwd);
	#
	set timeout 30;
	#
	foreach sp { spa spb } {
		set status [hw_telnet_open spid $spdata($sp,ip) $login $passwd];
		if {[isNotOk $status]} {
			return "-1 - hug_GetCurrentLoads: hw_telnet_open failed:\n$status";
		}
		#
		# determine the current load and where it is stored.
		#
		set done 0;
		send -i $spid "cd /Telica/swCPU/CurrRel\r";
		expect {
		-i $spid -nocase -re {file.*directory.*doesn't.*exist[^\r\n]*[\r\n]} {
			set cfgd(globals,$swid,$sp,currrel) "";
			return "-1 - hug_GetCurrentLoads: CurrRel does not exist";
		}
		-i $spid -re {(/[^\r\n]*)[\r\n]} {
			set cfgd(globals,$swid,$sp,currrel) $expect_out(1,string);
			set cfgd(globals,$swid,$sp,currcpuload) \
				[file tail $cfgd(globals,$swid,$sp,currrel)];
			exp_continue;
		}
		-i $spid -re {COMMAND_IS_DONE>} {
			# ok
			if {!$done} {
				incr done;
				send -i $spid "pwd -P\r";
				exp_continue;
			}
		}
		-i $spid timeout {
			hug_hardware_cleanup $spid;
			return "-1 - hug_GetCurrentLoads: cd CurrRel timed out:\n$status";
		}
		-i $spid eof {
			hug_hardware_cleanup $spid;
			return "-1 - hug_GetCurrentLoads: cd CurrRel eof:\n$status";
		}
		}
		#
		# get current disk usage 
		#
# exp_internal 1;
		send -i $spid "/bin/df | sed 's/  */ /g'\r";
		expect {
		-i $spid -re {/bin/df [^\r\n]*[\r\n]} {
			exp_continue;
		}
		-i $spid -nocase -re {filesystem *BlkSize *Blocks *Used *Free *%[^\r\n]*[\r\n]} {
			# ok
			exp_continue;
		}
		-i $spid -re {([^ \r\n]*)  *([^ \r\n]*)  *([^ \r\n]*)  *([^ \r\n]*)  *([^ \r\n]*)  *([^ \r\n]*)[\r\n]} {
			set hd $expect_out(1,string);
			if {[regexp -- {NFS} $expect_out(0,string)] == 1} {
				# skip any NFS file systems.
				exp_continue;
			}
			lappend cfgd(globals,$swid,$sp,harddisks) $hd;
			set cfgd(globals,$swid,$sp,$hd,blksize) $expect_out(2,string);
			set cfgd(globals,$swid,$sp,$hd,blocks) $expect_out(3,string);
			set cfgd(globals,$swid,$sp,$hd,used) $expect_out(4,string);
			set cfgd(globals,$swid,$sp,$hd,free) $expect_out(5,string);
			set cfgd(globals,$swid,$sp,$hd,percent) $expect_out(6,string);
			exp_continue;
		}
		-i $spid -re {COMMAND_IS_DONE>} {
			# ok
		}
		-i $spid timeout {
			hug_hardware_cleanup $spid;
			return "-1 - hug_GetCurrentLoads: df timed out:\n$status";
		}
		-i $spid eof {
			hug_hardware_cleanup $spid;
			return "-1 - hug_GetCurrentLoads: df eof:\n$status";
		}
		}
# exp_internal 0;
		#
		# get list of all software on the sp
		#
# exp_internal 1;
		foreach fs { "/home/Telica" "/home1/Telica" "/Telica" } {
			#
			lappend cfgd(globals,$swid,$sp,filesystems) $fs;
			#
			set cfgd(globals,$swid,$sp,softwarefs,$fs) "";
			#
			send -i $spid "ls -d ${fs}/sw*/* 2>&1 | sort -u \r";
			#
			expect {
			-i $spid -nocase -re {file.*directory.*doesn't.*exist[^\r\n]*[\r\n]} {
				# none found, just make a NULL length string
				set cfgd(globals,$swid,$sp,softwarefs,$fs) "";
				exp_continue;
			}
			-i $spid -re {ls -d[^\r\n]*[\r\n]} {
				exp_continue;
			}
			-i $spid -re {(/[^\r\n]*)[\r\n]} {
				lappend cfgd(globals,$swid,$sp,softwarefs,$fs) $expect_out(1,string);
				exp_continue;
			}
			-i $spid -re {COMMAND_IS_DONE>} {
				# ok
			}
			-i $spid timeout {
				hug_hardware_cleanup $spid;
				return "-1 - hug_GetCurrentLoads: ls ${fs}/sw*/* timed out:\n$status";
			}
			-i $spid eof {
				hug_hardware_cleanup $spid;
				return "-1 - hug_GetCurrentLoads: ls ${fs}/sw*/* eof:\n$status";
			}
			}
		}
# exp_internal 0;
		#
		hug_hardware_cleanup $spid;
	}
	#
	return "0 - success";
}
#
proc hug_getchassisdata { cfgdata vflag tmout } {
	upvar $cfgdata cfgd;
	#
	if {![info exists cfgd(globals,switchid)] || \
	    [string length $cfgd(globals,switchid)] <= 0} {
		return "-1 - hug_getchasisdata: switch id is not defined or is null-length";
	} else {
		set switchid $cfgd(globals,switchid);
	}
	if {![info exists cfgd(globals,$switchid,spa,ip)] || \
	     [string length $cfgd(globals,$switchid,spa,ip)] <= 0} {
		return "-1 - hug_getchasisdata: $switchid SP-A is unknown or null-length";
	} else {
		set spaip $cfgd(globals,$switchid,spa,ip);
	}
	if {![info exists cfgd(globals,$switchid,spb,ip)] || \
	    [string length $cfgd(globals,$switchid,spb,ip)] <= 0} {
		return "-1 - hug_getchasisdata: $switchid SP-B is unknown or null-length";
	} else {
		set spbip $cfgd(globals,$switchid,spb,ip);
	}
	#
	# determine which is the active sp.
	#
	if {![info exists cfgd($spaip,spstate)]} {
		return "-1 - hug_getchassisdata: SP-A state does not exist.";
	} elseif {![info exists cfgd($spbip,spstate)]} {
		return "-1 - hug_getchassisdata: SP-B state does not exist.";
	}
	if {$cfgd($spaip,spstate) == "isact" || \
	    $cfgd($spaip,spstate) == "isactnbk"} {
		set spip $spaip;
	} elseif {$cfgd($spbip,spstate) == "isact" || \
		$cfgd($spbip,spstate) == "isactnbk"} {
		set spip $spbip;
	} else {
		return "-1 - hug_getchassisdata: No active SP found.";
	}
	#
	set login $cfgd(globals,$switchid,tl1login);
	set passwd $cfgd(globals,$switchid,tl1passwd);
	#
	set status [hw_tl1_open spid $spip $login $passwd];
	if {[isNotOk $status]} {
		return "-1 - hug_getchassisdata: hw_tl1_open failed:\n$status";
	}
	#
	send -i $spid "RTRV-CHASSIS-EQPT;\r";
	#
# exp_internal 1;
	expect {
	-i $spid -re {[0-9]+ *COMPLD.*CHASSISTYPE=([^,]+),} {
		set cfgd(globals,$switchid,chassistype) $expect_out(1,string);
		exp_continue;
	}
	-i $spid -re ";\r\nTelica>" {
		# done
	}
	-i $spid timeout {
		hug_hardware_cleanup $spid;
		return "0 - hug_getchassisdata: RTRV-CHASSIS-EQPT timeout for $spd($sp,ip)";
	}
	-i $spid eof {
		hug_hardware_cleanup $spid;
		return "0 - hug_getchassisdata: RTRV-CHASSIS-EQPT eof for $spd($sp,ip)";
	}
	}
# exp_internal 0;
	#
	hug_hardware_cleanup $spid;
	return "0 - success";
}
#
proc hug_GetSwitchState { cfgdata } {
	upvar $cfgdata cfgd;
	#
	# pack data into an array
	#
	set swid $cfgd(globals,switchid);
	#
	set spdata(spa,ip) $cfgd(globals,$swid,spa,ip);
	set spdata(spa,reqstate) "isact,isstbyh";
	set spdata(spa,login) $cfgd(globals,$swid,login);
	set spdata(spa,passwd) $cfgd(globals,$swid,passwd);
	set spdata(spa,tl1login) $cfgd(globals,$swid,tl1login);
	set spdata(spa,tl1passwd) $cfgd(globals,$swid,tl1passwd);
	#
	set spdata(spb,ip) $cfgd(globals,$swid,spb,ip);
	set spdata(spb,reqstate) "isact,isstbyh";
	set spdata(spb,login) $cfgd(globals,$swid,login);
	set spdata(spb,passwd) $cfgd(globals,$swid,passwd);
	set spdata(spb,tl1login) $cfgd(globals,$swid,tl1login);
	set spdata(spb,tl1passwd) $cfgd(globals,$swid,tl1passwd);
	#
	puts "\nGet Switch State:\n";
	set status [hug_waitforcpustate cfgd 0 60 spdata];
	if {[isNotOk $status]} {
		return "-1 - hug_GetSwitchState: hug_waitforcpustate failed:\n$status";
	}
	#
	set status [hug_getchassisdata cfgd 0 60];
	if {[isNotOk $status]} {
		return "-1 - hug_GetSwitchState: hug_getchassisdata failed:\n$status";
	}
	# 
	puts "\nGet Current and Old Loads:\n";
	set status [hug_GetCurrentLoads cfgd 0 60];
	if {[isNotOk $status]} {
		return "-1 - hug_GetSwitchState: hug_GetCurrentLoads failed:\n$status";
	}
	#
	return "0 - success";
}

