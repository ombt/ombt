# execute cmds in scripts.
package provide scripts 1.0
#
package require getoptval
package require hugftp
package require hugtelnet
package require hugtl1
package require checkretval
package require hughardware
package require ncleis
#
set newcurrrel(/home1/Telica/swCPU) /;
set newcurrrel(/home1/Telica/swCPU/) /;
set newcurrrel(/Telica/swCPU) /home1/;
set newcurrrel(/Telica/swCPU/) /home1/;
set dontexecute 0;
set maxiomslot 17;
#
proc execwaitforiomstate { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	if {$startcmd > $cmd} {
		puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
		return "0 - success - skipping command";
	}
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set vflag 0;
	set tmout 30;
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	# pack data into an array
	set iomdata(streamid) [lindex $argv $arg];
	incr arg; set iomdata(iom) [lindex $argv $arg];
	incr arg; set iomdata(reqstate) [lindex $argv $arg];
	#
	regsub -all -- "," $iomdata(reqstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {[lsearch -exact {ignore unknown isact isactnbk isstbyc isstbyh oosma oos oosaumea} $reqstate] == -1} {
			puts "execwaitforiomstate: WARNING: unknown requested IOM state - $reqstate.";
		}
	}
	#
	set status [hug_waitforiomstate cfgd $vflag $tmout iomdata];
	if {[isNotOk $status]} {
		return "-1 - execwaitforiomstate: hug_execwaitforiomstate failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc execwaitforcpustate { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set vflag 0;
	set tmout 30;
	set ignorefault 0;
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-i} { set ignorefault 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	# pack data into an array
	set spdata(spa,ip) [lindex $argv $arg];
	incr arg; set spdata(spa,reqstate) [lindex $argv $arg];
	incr arg; set spdata(spa,login) [lindex $argv $arg];
	incr arg; set spdata(spa,passwd) [lindex $argv $arg];
	incr arg; set spdata(spa,tl1login) [lindex $argv $arg];
	incr arg; set spdata(spa,tl1passwd) [lindex $argv $arg];
	incr arg; set spdata(spb,ip) [lindex $argv $arg];
	incr arg; set spdata(spb,reqstate) [lindex $argv $arg];
	incr arg; set spdata(spb,login) [lindex $argv $arg];
	incr arg; set spdata(spb,passwd) [lindex $argv $arg];
	incr arg; set spdata(spb,tl1login) [lindex $argv $arg];
	incr arg; set spdata(spb,tl1passwd) [lindex $argv $arg];
	#
	regsub -all -- "," $spdata(spa,reqstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {[lsearch -exact {ignore unknown ping minmode appmode isact isactnbk isactsync isstbyc isstbysync isstbyh oosmahalt oosma oos oosmasdwn oosinitwfbt} $reqstate] == -1} {
			puts "execwaitforcpustate: WARNING: unknown SP-A requested state - $reqstate.";
		}
	}
	regsub -all -- "," $spdata(spb,reqstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {[lsearch -exact {ignore unknown ping minmode appmode isact isactnbk isactsync isstbyc isstbysync isstbyh oosmahalt oosma oos} $reqstate] == -1} {
			puts "execwaitforcpustate: WARNING: unknown SP-B requested state - $reqstate.";
		}
	}
	#
	set status [hug_waitforcpustate cfgd $vflag $tmout spdata $ignorefault];
	if {[isNotOk $status]} {
		return "-1 - execwaitforcpustate: hug_execwaitforcpustate failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc execftp { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set oper [lindex $argv 0];
	if {[lsearch -exact {open put get close} $oper] == -1} {
		return "-1 - execftp: invalid operation: $oper";
	}
	# binary transfer is the default
	set vflag 0; set aflag 0; set bflag 1; set iflag 0;
	set tmout 30;
	#
	for {set arg 1} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-i} { set iflag 1; }
		{^-v} { set vflag 1; }
		{^-a} { set aflag 1; set bflag 0; }
		{^-b} { set aflag 0; set bflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	#
	set streamid [lindex $argv $arg];
	#
	switch -regexp -- $oper {
	{^open$} {
		incr arg; set ftpip [lindex $argv $arg];
		incr arg; set ftplogin [lindex $argv $arg];
		incr arg; set ftppasswd [lindex $argv $arg];
		set status [hug_ftp_open cfgd $streamid $ftpip $ftplogin $ftppasswd $vflag $tmout]
	}
	{^put$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		incr arg; set ftpfrompath [lindex $argv $arg];
		incr arg; set ftptopath [lindex $argv $arg];
		set status [hug_ftp_put cfgd $streamid $ftpfrompath $ftptopath $aflag $bflag $iflag $vflag $tmout]
	}
	{^get$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		incr arg; set ftpfrompath [lindex $argv $arg];
		incr arg; set ftptopath [lindex $argv $arg];
		set status [hug_ftp_get cfgd $streamid $ftpfrompath $ftptopath $aflag $bflag $iflag $vflag $tmout]
	}
	{^close$} {
		set status [hug_ftp_close cfgd $streamid $vflag $tmout]
	}
	default {
		return "-1 - execftp: unknown operation $oper.";
	}
	}
	#
	if {[isNotOk $status]} {
		return "-1 - execftp: hug_ftp_$oper failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc exectl1 { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set oper [lindex $argv 0];
 	if {[lsearch -exact {open open2 exec file close rmvallioms rstallioms cpymems updateioms rmvsiglnks rstsiglnks} $oper] == -1} {
		return "-1 - exectl1: invalid operation: $oper";
	}
	#
	set vflag 0;
	set iflag 0;
	set tmout 30;
	#
	for {set arg 1} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-i} { set iflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	set streamid [lindex $argv $arg];
	#
	switch -regexp -- $oper {
	{^open$} {
		incr arg; set tip [lindex $argv $arg];
		incr arg; set tlogin [lindex $argv $arg];
		incr arg; set tpasswd [lindex $argv $arg];
		set status [hug_tl1_open cfgd $streamid $vflag $tmout $tip $tlogin $tpasswd];
	}
	{^open2$} {
		incr arg; set tspaip [lindex $argv $arg];
		incr arg; set tspbip [lindex $argv $arg];
		incr arg; set tlogin [lindex $argv $arg];
		incr arg; set tpasswd [lindex $argv $arg];
		set status [hug_tl1_open2 cfgd $streamid $vflag $tmout $tspaip $tspbip $tlogin $tpasswd];
	}
	{^file$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		incr arg; set filename [lindex $argv $arg];
		set status [hug_tl1_file cfgd $streamid $iflag $vflag $tmout $filename];
	}
 	{^rmvallioms$} {
 		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
 		if {$startcmd > $cmd} {
 			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
 			return "0 - success - skipping command";
 		}
 		set status [hug_tl1_rmvallioms cfgd $streamid $iflag $vflag $tmout];
 	}
 	{^rstallioms$} {
 		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
 		if {$startcmd > $cmd} {
 			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
 			return "0 - success - skipping command";
 		}
 		set status [hug_tl1_rstallioms cfgd $streamid $iflag $vflag $tmout];
	}
	{^cpymems$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		incr arg; set maxcpymems [lindex $argv $arg];
		set status [hug_tl1_cpymems cfgd $streamid $iflag $vflag $tmout $maxcpymems];
	}
	{^updateioms$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		incr arg; set maxcpymems [lindex $argv $arg];
		set status [hug_tl1_updateioms cfgd $streamid $iflag $vflag $tmout $maxcpymems];
	}
 	{^rmvsiglnks$} {
 		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
 		if {$startcmd > $cmd} {
 			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
 			return "0 - success - skipping command";
 		}
		incr arg; set iom [lindex $argv $arg];
 		set status [hug_tl1_rmvsiglnks cfgd $streamid $iflag $vflag $tmout $iom];
 	}
 	{^rstsiglnks$} {
 		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
 		if {$startcmd > $cmd} {
 			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
 			return "0 - success - skipping command";
 		}
		incr arg; set iom [lindex $argv $arg];
 		set status [hug_tl1_rstsiglnks cfgd $streamid $iflag $vflag $tmout $iom];
 	}
	{^exec$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		set argv [lreplace $argv 0 $arg];
		set xcmd [string trim [join $argv " "]];
		set status [hug_tl1_exec cfgd $streamid $iflag $vflag $tmout $xcmd];
	}
	{^close$} {
		set status [hug_tl1_close cfgd $streamid $vflag $tmout];
	}
	default {
		return "-1 - exectl1: unknown operation $oper.";
	}
	}
	#
	if {[isNotOk $status]} {
		return "-1 - exectl1: hug_tl1_$oper failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc exectelnet { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set oper [lindex $argv 0];
	if {[lsearch -exact {open exec close} $oper] == -1} {
		return "-1 - exectelnet: invalid operation: $oper";
	}
	#
	set vflag 0; set iflag 0; set rflag 0;
	set tmout 30;
	#
	for {set arg 1} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-i} { set iflag 1; }
		{^-r} { set rflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	set streamid [lindex $argv $arg];
	#
	switch -regexp -- $oper {
	{^open$} {
		incr arg; set tip [lindex $argv $arg];
		incr arg; set tlogin [lindex $argv $arg];
		incr arg; set tpasswd [lindex $argv $arg];
		set status [hug_telnet_open cfgd $streamid $vflag $tmout $tip $tlogin $tpasswd];
	}
	{^exec$} {
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
			return "0 - success - skipping command";
		}
		set argv [lreplace $argv 0 $arg];
		set xcmd [string trim [join $argv " "]];
		set status [hug_telnet_exec cfgd $streamid $iflag $vflag $tmout $xcmd $rflag];
	}
	{^close$} {
		set status [hug_telnet_close cfgd $streamid $vflag $tmout];
	}
	default {
		return "-1 - exectelnet: unknown operation $oper.";
	}
	}
	#
	if {[isNotOk $status]} {
		return "-1 - exectelnet: hug_telnet_$oper failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc exececho { cfgdata section cmd xcmd } {
	puts "# $xcmd";
	return "0 - success";
}
#
proc execsubs { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	if {$startcmd > $cmd} {
		puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
		return "0 - success - skipping command";
	}
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execsubs: argv($arg) = <$argval>";
	}
	#
	return "0 - success";
}
#
proc execsleep { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	if {$startcmd > $cmd} {
		puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
		return "0 - success - skipping command";
	}
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	set sleeptime [lindex $argv 0];
	#
	if {$sleeptime < 0} {
		return "-1 - execsleep: sleep time ($sleeptime) < 0.";
	}
	#
	set status 0;
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	#
	if {$startcmd <= $cmd} {
		catch { sleep $sleeptime; } ignore;
	}
	#
	return "0 - success";
}
#
proc execpause { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	global timeout;
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	if {$startcmd > $cmd} {
		puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
		return "0 - success - skipping command";
	}
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	set sleeptime [lindex $argv 0];
	#
	if {$sleeptime < 0} {
		return "-1 - execpause: sleep time ($sleeptime) < 0.";
	}
	#
	set status 0;
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	#
	if {$startcmd <= $cmd} {
		set savetimeout $timeout;
		set timeout $sleeptime;
		#
		send_user "\nPausing for $sleeptime seconds \[<cr>=continue]: ";
		expect_user {
		-re ".*\n" {
			# just continue;
		}
		timeout {
			puts "\nTimer expired, will continue.";
		}
		eof {
			puts "\nEOF, will continue.";
		}
		}
		#
		set timeout $savetimeout;
	}
	#
	return "0 - success";
}
#
proc execsystem { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	if {$startcmd > $cmd} {
		puts "0 - success - skipping command ($startcmd,$cmd,$cfgd(stack,frame))";
		return "0 - success - skipping command";
	}
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set vflag ""; set iflag 0;
	set tmout 30;
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-i} { set iflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	#
	set argv [lreplace $argv 0 [incr arg -1]];
	set syscmd [string trim [join $argv " "]];
	#
	set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
	#
	if {$startcmd <= $cmd} {
		system "$syscmd";
		catch { wait; } ignore;
	}
	#
	return "0 - success";
}
#
proc globalssub { cfgdata command substring } {
	upvar $cfgdata cfgd;
	upvar $command cmd;
	#
	if {[string length $substring] <= 0} {
		return "-1 - globalssub: null length substring.";
	}
	#
	set cmd $substring;
	#
	set sanitycounter 0;
	while {[regexp -- {<([_a-zA-Z0-9\.]+)>} $cmd ignore var] == 1} {
		incr sanitycounter;
		if {$sanitycounter>1000} {
			return "-1 - globalssub: over 1000 iterations.";
		}
		set cidx "globals,$var";
		regsub -all -- "\\\." $cidx "," newcidx;
		set cidx $newcidx;
		#
		if {![info exists cfgd($cidx)]} {
			return "-1 - globalssub: undefined variable $cidx";
		}
		regsub -all -- "<$var>" $cmd $cfgd($cidx) newcmd;
		set cmd $newcmd;
	}
	#
	return "0 - success";
}
#
proc execCmd { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	# get cmd type
	#
	set status [regexp -- {^[ \t]*([a-zA-Z0-9_]+)[ \t]+(.*)$} $xcmd var0 xcmdtype xcmdtail];
	if {$status != 1} {
		return "-1 - execCmd: unknown cmd form: $xcmd";
	}
	#
	set xcmd2exec exec$xcmdtype;
	#
	set status [llength [info command $xcmd2exec]];
	if {$status <= 0} {
		return "-1 - execCmd: unknown cmd: $xcmdtype.";
	} elseif {$status > 1} {
		return "-1 - execCmd: ambiguous cmd: $xcmdtype.";
	}
	#
	# check if we have a function call
	#
	if {$xcmdtype == "call"} {
		# calling a section as a function, must do argument
		# substitution before global substitution.
		#
		set startcmd $cfgd(stack,$cfgd(stack,frame),startcmd);
		if {$startcmd > $cmd} {
			return "0 - success";
		}
		#
		incr cfgd(stack,frame);
		set cfgd(stack,$cfgd(stack,frame),startcmd) 1;
		#
		puts "\nCalling $xcmd2exec to execute cmd at [exec date]: \n$xcmdtail";
		#
		set status [$xcmd2exec cfgd $section $cmd $xcmdtail];
		if {[isNotOk $status]} {
			incr cfgd(stack,frame) -1;
			return "-1 - execCmd: cmd $cmd ($xcmd2exec) failed.\n$status";
		} else {
			incr cfgd(stack,frame) -1;
			return "0 - success";
		}
	}
	#
	# replace all variables in command.
	#
	set status [globalssub cfgd finalxcmd $xcmdtail]
	if {[isNotOk $status]} {
		return "-1 - execCmd: globalssub failed for cmd - $xcmdtail.\n$status";
	}
	#
	# run the command
	#
	puts "\nCalling $xcmd2exec to execute cmd at [exec date]: \n$finalxcmd";
	#
	global dontexecute;
	if {$dontexecute} {
		return "0 - success from DON'T EXECUTE";
	} else {
		set status [$xcmd2exec cfgd $section $cmd $finalxcmd];
		if {[isNotOk $status]} {
			return "-1 - execCmd: cmd $cmd ($xcmd2exec) failed.\n$status";
		} else {
			return "0 - success";
		}
	}
}
#
proc argssub { argvals command substring } {
	upvar $argvals argvs;
	upvar $command cmd;
	#
	if {[string length $substring] <= 0} {
		return "-1 - argssub: null length substring.";
	}
	set cmd $substring;
	#
	foreach var [array names argvs] {
		regsub -all -- "<$var>" $cmd $argvs($var) newcmd;
		set cmd $newcmd;
	}
	#
	return "0 - success";
}
#
proc execcall { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	# get section to execute and run basic checks.
	#
	set execsec [lindex $argv 0];
	if {![info exists cfgd($execsec,type)]} {
		return "-1 - execcall: unknown type for section $execsec.";
	} elseif {$cfgd($execsec,type) != "exec"} {
		return "-1 - execcall: section $execsec is not executable.";
	} elseif {![info exists cfgd($execsec,maxcmd)]} {
		return "-1 - execcall: maxcmd for section $execsec is undefined.";
	}
	set startcmd 1;
	set maxcmd $cfgd($execsec,maxcmd);
	if {$maxcmd < 1} {
		return "-1 - execcall: maxcmd ($maxcmd) less than 1.";
	}
	#
	# get argument list
	#
	if {![info exists cfgd($execsec,arguments)]} {
		return "-1 - execcall: arguments not found for section $execsec.";
	}
	set fargs $cfgd($execsec,arguments);
	regsub -all -- {[ \t][ \t]*} $fargs " " newfargs;
	set fargv [split $newfargs " "];
	set fargc [llength $fargv];
	#
	if {$argc != [expr $fargc + 1]} {
		return "-1 - execcall: number of arguments is wrong for call of section $execsec.";
	}
	for {set ifarg 0} {$ifarg<$fargc} {incr ifarg} {
		set farg [lindex $fargv $ifarg];
		set arg [lindex $argv [expr $ifarg + 1]];
		set argvals($farg) $arg;
		puts "FORMAL ARG: $farg ==>> ACTUAL ARG: $arg";
	}
	#
	# start executing the cmds
	#
	for {set cmdno $startcmd} {$cmdno<=$maxcmd} {incr cmdno} {
		if {![info exists cfgd($execsec,$cmdno)]} {
			return "-1 - execcall: cmd $cmdno for section $execsec is undefined.";
		}
		set xcmd $cfgd($execsec,$cmdno);
		#
		# substitute argument values before calling to
		# execute command.
		#
		set status [argssub argvals finalxcmd $xcmd]
		if {[isNotOk $status]} {
			return "-1 - execcall: globalssub failed for cmd - $xcmd.\n$status";
		}
		#
		set status [execCmd cfgd $execsec $cmdno $finalxcmd];
		if {[isNotOk $status]} {
			return "-1 - execcall: cmd $cmdno failed.\n$status";
		}
	}
	return "0 - success";
}
#
proc sendemail { { email "" } { sec "" } { msg "" } } {
	# check email addresses exist
	if {[string length $email] <= 0} {
		# no email addresses given
		return;
	}
	#
	catch {
		system "echo \"$msg\" | /usr/bin/mailx -s \"Section $sec status\" $email";
	} ignore;
	catch { wait -nowait; } ignore;
	return;
}
#
proc execScript { cfgdata {startsec ""} {startcmd 1} } {
	upvar $cfgdata cfgd;
	#
	# check for email 
	#
	if {[info exists cfgd(globals,email)]} {
		set email $cfgd(globals,email);
		puts "\nEMAIL address is ${email}";
	} else {
		set email "";
		puts "\nNo EMAIL address was given";
	}
	#
	# find the the section to execute.
	#
	if {[string length $startsec] <= 0} {
		# look for the first section, should be marked
		# when data file was read in.
		#
		if {![info exists cfgd(internal,exec,1)]} {
			sendemail $email "unknown" "-1 - execScript: unknown first section to execute.";
			return "-1 - execScript: unknown first section to execute.";
		}
		#
		set execsec $cfgd(internal,exec,1);
	} else {
		set execsec $startsec;
		if {![info exists cfgd($execsec,type)]} {
			sendemail $email $execsec "-1 - execScript: unknown type for section $execsec.";
			return "-1 - execScript: unknown type for section $execsec.";
		} elseif {$cfgd($execsec,type) != "exec"} {
			sendemail $email $execsec "-1 - execScript: section $execsec is not executable.";
			return "-1 - execScript: section $execsec is not executable.";
		}
	}
	#
	# check start and end cmds
	#
	if {$startcmd < 1} {
		sendemail $email $execsec "-1 - execScript: startcmd ($startcmd) less than 1.";
		return "-1 - execScript: startcmd ($startcmd) less than 1.";
	}
	#
	if {![info exists cfgd($execsec,maxcmd)]} {
		sendemail $email $execsec "-1 - execScript: maxcmd for section $execsec is undefined.";
		return "-1 - execScript: maxcmd for section $execsec is undefined.";
	}
	set maxcmd $cfgd($execsec,maxcmd);
	if {$maxcmd < 1} {
		sendemail $email $execsec "-1 - execScript: maxcmd ($maxcmd) less than 1.";
		return "-1 - execScript: maxcmd ($maxcmd) less than 1.";
	}
	if {$startcmd > $maxcmd} {
		sendemail $email $execsec "-1 - execScript: startcmd ($startcmd) > maxcmd ($maxcmd)";
		return "-1 - execScript: startcmd ($startcmd) > maxcmd ($maxcmd)";
	}
	#
	# start executing the cmds
	#
	set cfgd(stack,frame) 1;
	set cfgd(stack,1,startcmd) $startcmd;
	#
	for {set cmdno 1} {$cmdno<=$maxcmd} {incr cmdno} {
		if {![info exists cfgd($execsec,$cmdno)]} {
			sendemail $email $execsec "-1 - execScript: cmd $cmdno for section $execsec is undefined.";
			return "-1 - execScript: cmd $cmdno for section $execsec is undefined.";
		}
		set xcmd $cfgd($execsec,$cmdno);
		set status [execCmd cfgd $execsec $cmdno $xcmd];
		if {[isNotOk $status]} {
			sendemail $email $execsec "-1 - execScript: cmd $cmdno failed.\n$status";
			return "-1 - execScript: cmd $cmdno failed.\n$status";
		}
	}
	#
	sendemail $email $execsec "0 - success";
	return "0 - success";
}
#
proc generateFileHeader { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	lappend cmdd(sections) "file";
	#
	set cmd 0;
	#
	set cmdd(file,[incr cmd]) "file script {";
	set cmdd(file,[incr cmd]) "\tscriptsdir .";
	set cmdd(file,[incr cmd]) "}";
	#
	set cmdd(file,first) 1;
	set cmdd(file,last) $cmd;
	#
	puts "\n";
	puts "Generate File Header: ";
	puts "First Cmd: $cmdd(file,first)";
	puts "Last Cmd: $cmdd(file,last)";
	#
	return "0 - success";
}
#
proc generateGlobals { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	global newcurrrel;
	global env;
	global maxiomslot;
	#
	lappend cmdd(sections) "globals";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	set cmdd(globals,[incr cmd]) "globals data {";
	#
	set cmdd(globals,[incr cmd]) "switchid $switchid";
	set cmdd(globals,[incr cmd]) "login $cfgd(globals,$switchid,login)";
	set cmdd(globals,[incr cmd]) "activesp $spd(active)";
	set cmdd(globals,[incr cmd]) "standbysp $spd(standby)";
	set cmdd(globals,[incr cmd]) "passwd $cfgd(globals,$switchid,passwd)";
	set cmdd(globals,[incr cmd]) "tl1login $cfgd(globals,$switchid,tl1login)";
	set cmdd(globals,[incr cmd]) "tl1passwd $cfgd(globals,$switchid,tl1passwd)";
	set cmdd(globals,[incr cmd]) "spa {";
	set cmdd(globals,[incr cmd]) "\tip $cfgd(globals,$switchid,spa,ip)";
	set cmdd(globals,[incr cmd]) "\tcurrrel $cfgd(globals,$switchid,spa,currrel)";
	set cmdd(globals,[incr cmd]) "\tcurrcpuload $cfgd(globals,$switchid,spa,currcpuload)";
	set cmdd(globals,[incr cmd]) "\tnewcurrrel $newcurrrel([file dirname $cfgd(globals,$switchid,spa,currrel)])";
	set cmdd(globals,[incr cmd]) "}";
	set cmdd(globals,[incr cmd]) "spb {";
	set cmdd(globals,[incr cmd]) "\tip $cfgd(globals,$switchid,spb,ip)";
	set cmdd(globals,[incr cmd]) "\tcurrrel $cfgd(globals,$switchid,spb,currrel)";
	set cmdd(globals,[incr cmd]) "\tcurrcpuload $cfgd(globals,$switchid,spb,currcpuload)";
	set cmdd(globals,[incr cmd]) "\tnewcurrrel $newcurrrel([file dirname $cfgd(globals,$switchid,spb,currrel)])";
	set cmdd(globals,[incr cmd]) "}";
	#
	set cmdd(globals,[incr cmd]) "iom {";
	for {set iom 1} {$iom <= $maxiomslot} {incr iom} {
		if {![info exists iomd($iom,clei)] || \
		     [string length $iomd($iom,clei)] <= 0} {
			# empty slot, use default data.
			# set cmdd(globals,[incr cmd]) "\t$iom {";
			# set cmdd(globals,[incr cmd]) "\t\tclei NULL";
			# set cmdd(globals,[incr cmd]) "\t\tprotection NULL";
			# set cmdd(globals,[incr cmd]) "\t\tstate NULL";
			# set cmdd(globals,[incr cmd]) "\t\tversion NULL";
			# set cmdd(globals,[incr cmd]) "\t}";
		} else {
			set cmdd(globals,[incr cmd]) "\t$iom {";
			set cmdd(globals,[incr cmd]) "\t\tclei $iomd($iom,clei)";
			set cmdd(globals,[incr cmd]) "\t\trearclei $iomd($iom,rearclei)";
			set cmdd(globals,[incr cmd]) "\t\tprotection $iomd($iom,protection)";
			set cmdd(globals,[incr cmd]) "\t\tstate $iomd($iom,state)";
			set cmdd(globals,[incr cmd]) "\t\tversion $iomd($iom,version)";
			set cmdd(globals,[incr cmd]) "\t}";
		}
	}
	set cmdd(globals,[incr cmd]) "\tioms $iomd(ioms)";
	set cmdd(globals,[incr cmd]) "\ttypes $iomd(types)";
	foreach iomtype $iomd(types) {
		set cmdd(globals,[incr cmd]) "\t${iomtype}ioms $iomd(${iomtype}ioms)";
	}
	set cmdd(globals,[incr cmd]) "}";
	#
	set cmdd(globals,[incr cmd]) "ftpmachine $cfgd(globals,load,ftpmachine)";
	set cmdd(globals,[incr cmd]) "ftplogin $cfgd(globals,load,ftplogin)";
	set cmdd(globals,[incr cmd]) "ftppasswd $cfgd(globals,load,ftppasswd)";
	set cmdd(globals,[incr cmd]) "branch $cfgd(globals,load,branch)";
	set cmdd(globals,[incr cmd]) "cpuload $cfgd(globals,load,cpuload)";
	set cmdd(globals,[incr cmd]) "images $cfgd(globals,load,images,types)";
	#
	set cmdd(globals,[incr cmd]) "bkupmachine $cfgd(globals,backup,machine)";
	set cmdd(globals,[incr cmd]) "bkuplogin $cfgd(globals,backup,login)";
	set cmdd(globals,[incr cmd]) "bkuppasswd $cfgd(globals,backup,passwd)";
	set cmdd(globals,[incr cmd]) "bkupdir $cfgd(globals,backup,dir)";
	#
	foreach image $cfgd(globals,load,images,types) {
		set cmdd(globals,[incr cmd]) "$image [file tail $cfgd(globals,load,images,$image)]";
		set cmdd(globals,[incr cmd]) "${image}_path $cfgd(globals,load,images,$image)";
		set cmdd(globals,[incr cmd]) "${image}_load $cfgd(globals,load,images,${image}load)";
	}
	#
	if {[info exists cfgd(globals,email)]} {
		set cmdd(globals,[incr cmd]) "email $cfgd(globals,email)";
	}
	#
	set cmdd(globals,[incr cmd]) "# START OF REQUIRED DEFINITIONS";
	set cmdd(globals,[incr cmd]) "hugscripts $env(LCSHUGSCRIPTS)";
	set cmdd(globals,[incr cmd]) "interunixcmddelay 30";
	set cmdd(globals,[incr cmd]) "preupgradedelay 300";
	set cmdd(globals,[incr cmd]) "spa sp-a";
	set cmdd(globals,[incr cmd]) "spb sp-b";
	set cmdd(globals,[incr cmd]) "iom1 iom-1";
	set cmdd(globals,[incr cmd]) "iom2 iom-2";
	set cmdd(globals,[incr cmd]) "iom3 iom-3";
	set cmdd(globals,[incr cmd]) "iom4 iom-4";
	set cmdd(globals,[incr cmd]) "iom5 iom-5";
	set cmdd(globals,[incr cmd]) "iom6 iom-6";
	set cmdd(globals,[incr cmd]) "iom7 iom-7";
	set cmdd(globals,[incr cmd]) "iom8 iom-8";
	set cmdd(globals,[incr cmd]) "iom9 iom-9";
	set cmdd(globals,[incr cmd]) "iom10 iom-10";
	set cmdd(globals,[incr cmd]) "iom11 iom-11";
	set cmdd(globals,[incr cmd]) "iom12 iom-12";
	set cmdd(globals,[incr cmd]) "iom13 iom-13";
	set cmdd(globals,[incr cmd]) "iom14 iom-14";
	set cmdd(globals,[incr cmd]) "iom15 iom-15";
	set cmdd(globals,[incr cmd]) "iom16 iom-16";
	set cmdd(globals,[incr cmd]) "iom17 iom-17";
	set cmdd(globals,[incr cmd]) "# END OF REQUIRED DEFINITIONS";
	set cmdd(globals,[incr cmd]) "}";
	#
	set cmdd(globals,first) 1;
	set cmdd(globals,last) $cmd;
	#
	puts "\n";
	puts "Generate Globals Header: ";
	puts "First Cmd: $cmdd(globals,first)";
	puts "Last Cmd: $cmdd(globals,last)";
	#
	return "0 - success";
}
#
proc generateFunctions { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	global newcurrrel;
	#
	lappend cmdd(sections) "functions";
	#
	set cmd 0;
	#
	set cmdd(functions,[incr cmd]) "checkforcpucores exec { spip login passwd } {";
	#
	set cmdd(functions,[incr cmd]) "telnet open telnetsp_cpucore <spip> <login> <passwd>";
	set cmdd(functions,[incr cmd]) "telnet exec -v -r telnetsp_cpucore find / -type f -name core -print | grep core";
	set cmdd(functions,[incr cmd]) "telnet exec -v -r telnetsp_cpucore find / -type f -name *core -print | grep Telica | grep core";
	set cmdd(functions,[incr cmd]) "telnet exec -v -r telnetsp_cpucore find / -type f -name *oldCore* -print | grep Telica | grep oldCore";
	set cmdd(functions,[incr cmd]) "telnet close telnetsp_cpucore";
	set cmdd(functions,[incr cmd]) "}";
	#
	set cmdd(functions,first) 1;
	set cmdd(functions,last) $cmd;
	#
	puts "\n";
	puts "Generate Functions Section: ";
	puts "First Cmd: $cmdd(functions,first)";
	puts "Last Cmd: $cmdd(functions,last)";
	#
	return "0 - success";
}
#
proc generatePreHugIomCmds { cfgdata cmddata spdata iomdata command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	upvar $command cmd;
	#
	# DLP-712 on both sides (old files were already removed).
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp <<activesp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp df";
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp <<standbysp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp df";
	#
	# copy IOM tar files to switch and extract, DLP-713 and DLP-714.
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	foreach iomtype $iomd(types) {
		if {[lsearch -exact $cfgd(globals,load,images,types) $iomtype] == -1} {
			puts "\nNo new software for IOM type $iomtype.";
			puts "Skipping update of IOM type $iomtype.";
			continue;
		}
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::<ftpmachine>,<ftplogin>,<ftppasswd>,<${iomtype}_path>,<<activesp>.newcurrrel>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,<<activesp>.newcurrrel>/<$iomtype>,<<standbysp>.newcurrrel>/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::<<activesp>.newcurrrel>/<$iomtype>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp rm -f <<activesp>.newcurrrel>/<$iomtype>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::<$iomtype>,<<standbysp>.newcurrrel>/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp rm -f <<standbysp>.newcurrrel>/<$iomtype>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
	}
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	#
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	#
	return "0 - success";
}
#
proc generatePreHugCmds { cfgdata cmddata spdata iomdata { huglite 0 } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	if {$huglite} {
		set huglite "# ";
	} else {
		set huglite "";
	}
	#
	lappend cmdd(sections) "prehug";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	set cmdd(prehug,[incr cmd]) "prehug exec {";
	#
	# send tools over
	#
	set cmdd(prehug,[incr cmd]) "ftp open prehugftp <<activesp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "ftp put -a prehugftp <hugscripts>/findcpucores /bin/findcpucores";
	set cmdd(prehug,[incr cmd]) "ftp close prehugftp";
	set cmdd(prehug,[incr cmd]) "ftp open prehugftp <<standbysp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "ftp put -a prehugftp <hugscripts>/findcpucores /bin/findcpucores";
	set cmdd(prehug,[incr cmd]) "ftp close prehugftp";
	#
	# remove old core files, not part of DLP.
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnet <<activesp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}telnet exec -i -v prehugtelnet find / -type f -name core -print | xargs rm -f";
	set cmdd(prehug,[incr cmd]) "${huglite}telnet exec -i -v prehugtelnet find / -type f -name *oldCore* -print | grep /Telica | xargs rm -f";
	set cmdd(prehug,[incr cmd]) "telnet exec prehugtelnet chmod 755 /bin/findcpucores";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnet";
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnet <<standbysp>.ip> <login> <passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}telnet exec -i -v prehugtelnet find / -type f -name core -print | xargs rm -f";
	set cmdd(prehug,[incr cmd]) "${huglite}telnet exec -i -v prehugtelnet find / -type f -name *oldCore* -print | grep /Telica | xargs rm -f";
	set cmdd(prehug,[incr cmd]) "telnet exec prehugtelnet chmod 755 /bin/findcpucores";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnet";
	#
	# DLP-700 on original active SP
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 rtrv-eqpt";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 rtrv-chassis-eqpt";
	#
	# DLP-701 on original active SP
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/home1/Telica/swCPU/";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	#
	# remove old SWCPU loads. compare the swCPU loads with what is
	# currently active, and if they don't match, remove them.
	#
	set currel $cfgd(globals,$switchid,$spd(active),currrel);
	foreach fs $cfgd(globals,$switchid,$spd(active),filesystems) {
		foreach cpudir $cfgd(globals,$switchid,$spd(active),softwarefs,$fs) {
			if {[regexp -- {^.*/swCPU/.*$} $cpudir] == 0 || \
			    [regexp -- {^.*/swCPU/core.*$} $cpudir] == 1 || \
			    [regexp -- {^.*/swCPU/CurrRel.*$} $cpudir] == 1} {
				continue;
			} elseif {"$currel" != "$cpudir"} {
				set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-r,$cpudir";
				set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
			}
		}
	}
	#
	# remove old tar files.
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home1/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 rmv-eqpt::<<standbysp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 rst-eqpt::<<standbysp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 sw-toprotn-eqpt::<<activesp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	# DLP-700 on original standby SP (but now active)
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 rtrv-eqpt";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 rtrv-chassis-eqpt";
	#
	# DLP-701 on original standby SP (but now active)
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-ls:::::-l,/home1/Telica/swCPU/";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	#
	# remove old SWCPU loads. compare the swCPU loads with what is
	# currently active, and if they don't match, remove them.
	#
	set currel $cfgd(globals,$switchid,$spd(standby),currrel);
	foreach fs $cfgd(globals,$switchid,$spd(standby),filesystems) {
		foreach cpudir $cfgd(globals,$switchid,$spd(standby),softwarefs,$fs) {
			if {[regexp -- {^.*/swCPU/.*$} $cpudir] == 0 || \
			    [regexp -- {^.*/swCPU/core.*$} $cpudir] == 1 || \
			    [regexp -- {^.*/swCPU/CurrRel.*$} $cpudir] == 1} {
				continue;
			} elseif {"$currel" != "$cpudir"} {
				set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-r,$cpudir";
				set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
			}
		}
	}
	#
	# remove old tar files.
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home1/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -i -v prehugtl1 exec-rm:::::-f,/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "${huglite}sleep <interunixcmddelay>";
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 rmv-eqpt::<<activesp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 rst-eqpt::<<activesp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 open -t30 prehugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 exec -v prehugtl1 sw-toprotn-eqpt::<<standbysp>>";
	set cmdd(prehug,[incr cmd]) "${huglite}tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "${huglite}waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	# check if we have a CPU update?
	#
	if {[info exists cfgd(globals,load,images,cpu)]} {
		#
		# we have a CPU hug. DLP-702 on both SPs.
		#
		set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::<ftpmachine>,<ftplogin>,<ftppasswd>,<cpu_path>,<<activesp>.newcurrrel>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,<<activesp>.newcurrrel>/<cpu>,<<standbysp>.newcurrrel>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
		#
		# DLP-703 on both SPs.
		#
		set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cd:::::<<activesp>.newcurrrel>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::<<activesp>.newcurrrel>/<cpu>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::<cpu>,<<standbysp>.newcurrrel>";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
		#
		# remove tar files once we are done expanding.
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp <<activesp>.ip> <login> <passwd>";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f <<activesp>.newcurrrel>/<cpu>";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp <<standbysp>.ip> <login> <passwd>";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f <<standbysp>.newcurrrel>/<cpu>";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	}
	#
	# check if we have IOMs to update
	#
	if {[info exists iomd(noncpuioms)] && \
	    [string length $iomd(noncpuioms)] > 0} {
		set status [generatePreHugIomCmds cfgd cmdd spd iomd cmd];
		if {[isNotOk $status]} {
			return "-1 - generatePreHugCmds: generatePreHugIomCmds failed.\n$status";
		}
	}
	#
	set cmdd(prehug,[incr cmd]) "}";
	#
	set cmdd(prehug,first) 1;
	set cmdd(prehug,last) $cmd;
	#
	puts "\n";
	puts "Generate PreHUG Cmds: ";
	puts "First Cmd: $cmdd(prehug,first)";
	puts "Last Cmd: $cmdd(prehug,last)";
	#
	return "0 - success";
}
#
proc generatePostHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	lappend cmdd(sections) "posthug";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	set cmdd(posthug,[incr cmd]) "posthug exec {";
	#
	set cmdd(posthug,[incr cmd]) "telnet open -t30 posthugtelnet <<activesp>.ip> <login> <passwd>";
	set cmdd(posthug,[incr cmd]) "telnet exec -i -v -t600 posthugtelnet /bin/findcpucores";
	set cmdd(posthug,[incr cmd]) "telnet close posthugtelnet";
	set cmdd(posthug,[incr cmd]) "telnet open -t30 posthugtelnet <<standbysp>.ip> <login> <passwd>";
	set cmdd(posthug,[incr cmd]) "telnet exec -i -v -t600 posthugtelnet /bin/findcpucores";
	set cmdd(posthug,[incr cmd]) "telnet close posthugtelnet";
	#
	set cmdd(posthug,[incr cmd]) "tl1 open -t30 posthugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(posthug,[incr cmd]) "tl1 exec -i -v posthugtl1 rtrv-eqpt";
	set cmdd(posthug,[incr cmd]) "tl1 close -t 30 posthugtl1";
	#
	set cmdd(posthug,[incr cmd]) "}";
	#
	set cmdd(posthug,first) 1;
	set cmdd(posthug,last) $cmd;
	#
	puts "\n";
	puts "Generate PostHUG Cmds: ";
	puts "First Cmd: $cmdd(posthug,first)";
	puts "Last Cmd: $cmdd(posthug,last)";
	return "0 - success";
}
#
proc generateCpuHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	# check if we have a CPU Hug ..
	#
	if {![info exists cfgd(globals,load,images,cpu)]} {
		puts "\n";
		puts "No CPU update. Skipping CPU Hug section.";
		return "0 - success";
	}
	#
	lappend cmdd(sections) "cpuhug";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	set cmdd(cpuhug,[incr cmd]) "cpuhug exec {";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-call-captverbose";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 dlt-call-captverbose::1";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats::::::oos";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-sys-secu";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rmv-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd>";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rst-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 sw-toprotn-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	# after switch, standby is now active ...
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v -t30 cpuhugtl1 rtrv-pm-cc";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1800 cpuhugtl1 exec-backup-plexus::com:::<bkupmachine>,<bkuplogin>,<bkuppasswd>,<bkupdir>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 ed-eqpt::<<activesp>>:::swversion=<cpuload>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rmv-eqpt::<<activesp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd>";
	#
	# we're at the first critical point, the upgrade will run next. before
	# running the upgrade, you need to wait at least 5 minutes according
	# to DLP-705. let's wait, then continue.
	#
	set cmdd(cpuhug,[incr cmd]) "sleep <preupgradedelay>";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1200 cpuhugtl1 exec-run-upgrade:<switchid>:<<activesp>>";
	# check for cores after update
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<activesp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<standbysp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rst-eqpt::<<activesp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	# check for cores after restore
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<activesp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<standbysp>.ip> <login> <passwd>";
	#
	# now to update the other side, the old active sp.
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<standbysp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 ed-eqpt::<<standbysp>>:::swversion=<cpuload>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 sw-toprotn-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "sleep 10";
	#
	# at this point, we have the original standbysp (but currently 
	# active sp) running on the old load, and the original activesp
	# is running on the new load (but it is currently standby sp). we 
	# will switch to the new load for the first time. this leaves the
	# original standbysp on the old load and it will never synch
	# with the new load, so ignore it's state.
	#
	# set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> oos,oosma,oosmahalt,oosmasdwn,oosinitwfbt,isstbyc,isstbysync,isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> oos,oosma,oosmahalt,oosmasdwn,oosinitwfbt,isstbyc,isstbysync,isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	#
	if {$spd(standby) == "spa"} {
		set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> ignore <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isactnbk <login> <passwd> <tl1login> <tl1passwd>";
	} elseif {$spd(standby) == "spb"} {
		set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isactnbk <login> <passwd> <tl1login> <tl1passwd> <spb.ip> ignore <login> <passwd> <tl1login> <tl1passwd>";
	} else {
		return "-1 - generateCpuHugCmds: unknown standby SP - $spd(standby).";
	}
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rmv-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v -t60 cpuhugtl1 rtrv-alm-all";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v -t60 cpuhugtl1 rtrv-pm-cc";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isactnbk,oosmahalt,oosma <login> <passwd> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "sleep <preupgradedelay>";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1200 cpuhugtl1 exec-run-upgrade:<switchid>:<<standbysp>>";
	# check for cores after upgrade
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<activesp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<standbysp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rst-eqpt::<<standbysp>>";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 <spa.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd> <spb.ip> isact,isstbyh <login> <passwd> <tl1login> <tl1passwd>";
	# check for cores after restore
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<activesp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "call checkforcpucores <<standbysp>.ip> <login> <passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-info-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	#
	set cmdd(cpuhug,[incr cmd]) "}";
	#
	set cmdd(cpuhug,first) 1;
	set cmdd(cpuhug,last) $cmd;
	#
	puts "\n";
	puts "Generate CpuHUG Cmds: ";
	puts "First Cmd: $cmdd(cpuhug,first)";
	puts "Last Cmd: $cmdd(cpuhug,last)";
	return "0 - success";
}
#
proc generateNewCmLoadHugCmds { cfgdata cmddata spdata cmdno standbycm activecm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $cmdno cmd;
	#
	set cmdd(cmhug,[incr cmd]) \
		"tl1 open -t30 cmhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rmv-eqpt::<iom$standbycm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t300 cmhugtl1 $standbycm oos,oosma";
	#
	# check if we have to use ed-eqpt-all with appswversion.
	#
	set switchid $cfgd(globals,switchid);
	#
	if {[info exists cfgd(globals,load,images,cpuload)]} {
		set hugcpuload $cfgd(globals,load,images,cpuload);
		set hugcpuloadvar "<cpuload>";
	} elseif {[info exists cfgd(globals,${switchid},spa,currcpuload)]} {
		# assume SP-A and SP-B have the same current CPU load.
		set hugcpuload $cfgd(globals,${switchid},spa,currcpuload);
		set hugcpuloadvar "<<activesp>.currcpuload>";
	} else {
		set hugcpuload "";
	}
	if {[regexp -- {^6\.2\.1.*} "${hugcpuload}"] == 1} {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt-all::<iom$standbycm>:::swversion=<cm_load>,appswversion=${hugcpuloadvar}";
	} else {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt::<iom$standbycm>:::swversion=<cm_load>";
	}
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v -t1200 cmhugtl1 cpy-mem::<iom$standbycm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $standbycm oos,oosma";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rst-eqpt::<iom$standbycm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $standbycm isstbyh";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 sw-toprotn-eqpt::<iom$activecm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $activecm oos,oosma";
	#
	# check if we have to use ed-eqpt-all with appswversion.
	#
	if {[regexp -- {^6\.2\.1.*} "${hugcpuload}"] == 1} {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt-all::<iom$activecm>:::swversion=<cm_load>,appswversion=${hugcpuloadvar}";
	} else {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt::<iom$activecm>:::swversion=<cm_load>";
	}
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v -t1200 cmhugtl1 cpy-mem::<iom$activecm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $activecm isstbyh";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 close -t 30 cmhugtl1";
	return;
}
#
proc generateNewCpuLoadCmHugCmds { cfgdata cmddata spdata cmdno standbycm activecm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $cmdno cmd;
	#
	# check if we have to use ed-eqpt-all with appswversion.
	#
	set switchid $cfgd(globals,switchid);
	#
	if {[info exists cfgd(globals,load,images,cpuload)]} {
		set hugcpuload $cfgd(globals,load,images,cpuload);
		set hugcpuloadvar "<cpuload>";
	} elseif {[info exists cfgd(globals,${switchid},spa,currcpuload)]} {
		# assume SP-A and SP-B have the same current CPU load.
		set hugcpuload $cfgd(globals,${switchid},spa,currcpuload);
		set hugcpuloadvar "<<activesp>.currcpuload>";
	} else {
		set hugcpuload "";
	}
	#
	# get current CM loads,
	#
	# set standbycmload $cfgd(iom,${standbycm},version);
	set standbycmload "<iom.${standbycm},version>";
	# set activecmload $cfgd(iom,${activecm},version);
	set activecmload "<iom.${activecm}.version>";
	#
	set cmdd(cmhug,[incr cmd]) \
		"tl1 open -t30 cmhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	#
	if {[regexp -- {^6\.2\.1.*} "${hugcpuload}"] == 1} {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt-all::<iom$standbycm>:::swversion=${standbycmload},appswversion=${hugcpuloadvar}";
	}
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rmv-eqpt::<iom$standbycm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t300 cmhugtl1 $standbycm oos,oosma";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rst-eqpt::<iom$standbycm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $standbycm isstbyh";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 sw-toprotn-eqpt::<iom$activecm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $activecm oos,oosma";
	#
	if {[regexp -- {^6\.2\.1.*} "${hugcpuload}"] == 1} {
		set cmdd(cmhug,[incr cmd]) \
			"tl1 exec -v cmhugtl1 ed-eqpt-all::<iom$activecm>:::swversion=${activecmload},appswversion=${hugcpuloadvar}";
	}
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rmv-eqpt::<iom$activecm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t300 cmhugtl1 $activecm oos,oosma";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 exec -v cmhugtl1 rst-eqpt::<iom$activecm>";
	set cmdd(cmhug,[incr cmd]) \
		"waitforiomstate -t600 cmhugtl1 $activecm isstbyh";
	set cmdd(cmhug,[incr cmd]) \
		"tl1 close -t 30 cmhugtl1";
	return;
}
#
proc generateCmHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	# check if we have a CM Hug or a CPU Hug ...
	#
	if {![info exists cfgd(globals,load,images,cpu)] && 
	    ![info exists cfgd(globals,load,images,cm)]} {
		puts "\n";
		puts "No CM or CPU update. Skipping CM Hug section.";
		return "0 - success";
	}
	#
	lappend cmdd(sections) "cmhug";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	if {[info exists cfgd(globals,load,images,cmload)] && \
	    [string length $cfgd(globals,load,images,cmload)] > 0} {
		set newcmload 1;
	} else {
		set newcmload 0;
	}
	#
	set cmdd(cmhug,[incr cmd]) "cmhug exec {";
	#
	# scan CM pairs for updates. remember, CM 14 and 15
	# are updated first.
	#
	foreach lowcm {14 1 3 5 7 10 12} {
		# cm pairs
		set hicm [expr $lowcm + 1];
		# check if both CMs exist
		if {!([info exists iomd($lowcm,type)] && \
		    $iomd($lowcm,type) == "cm" && \
		    ($iomd($lowcm,state) == "isact" || 
		     $iomd($lowcm,state) == "isstbyh") &&
		    [info exists iomd($hicm,type)] && \
		    $iomd($hicm,type) == "cm" && \
		    ($iomd($hicm,state) == "isact" || 
		     $iomd($hicm,state) == "isstbyh"))} {
			puts "\n";
			puts "Skipping IOM slots: $lowcm and $hicm.";
			puts "CM pair must exist and be IS,ACT or IS,STBYH";
			puts "IOM $lowcm state: $cfgd(iom,$lowcm,iomstate)";
			puts "IOM $hicm state: $cfgd(iom,$hicm,iomstate)";
			continue;
		} else {
			puts "\n";
			puts "Will update CM pair $lowcm and $hicm.";
			puts "CM $lowcm state: $iomd($lowcm,state)";
			puts "CM $hicm state: $iomd($hicm,state)";
		}
		# write commands to update
		if {$iomd($lowcm,state) == "isact"} {
			set activecm $lowcm;
			set standbycm $hicm;
		} else {
			set activecm $hicm;
			set standbycm $lowcm;
		}
		#
		if {$newcmload} {
			generateNewCmLoadHugCmds cfgd cmdd spd cmd $standbycm $activecm;
		} else {
			generateNewCpuLoadCmHugCmds cfgd cmdd spd cmd $standbycm $activecm;
		}
	}
	#
	set cmdd(cmhug,[incr cmd]) "}";
	#
	set cmdd(cmhug,first) 1;
	set cmdd(cmhug,last) $cmd;
	#
	puts "\n";
	puts "Generate CmHUG Cmds: ";
	puts "First Cmd: $cmdd(cmhug,first)";
	puts "Last Cmd: $cmdd(cmhug,last)";
	return "0 - success";
}
#
proc generateIomHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	set switchid $cfgd(globals,switchid);
	#
	if {![info exists cfgd(globals,$switchid,chassistype)]} {
		return "-1 - generateIomHugCmds: chassis type does not exist.";
	}
	#
	lappend cmdd(sections) "iomhug";
	set cmd 0;
	#
	puts "\nGenerating IOM HUG Cmds:";
	#
	set cmdd(iomhug,[incr cmd]) "iomhug exec {";
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	#
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-sys-secu";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1800 iomhugtl1 exec-backup-plexus::com:::<bkupmachine>,<bkuplogin>,<bkuppasswd>,<bkupdir>";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-status-mg";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-si-mgcassoc";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	if {[info exists iomd(types)]} {
		foreach iomtype $iomd(types) {
			if {$iomtype == "cm"} {
				# CMs are not done here.
				continue;
			}
			if {![info exists iomd($iomtype)]} {
				# no data?
				return "-1 - generateIomHugCmds: no IOM data found for type $iomtype.";
			}
			if {![info exists cfgd(globals,load,images,${iomtype}load)] || \
	    		     [string length $cfgd(globals,load,images,${iomtype}load)] <= 0} {
				# no new load for this type of IOM.
				continue;
			}
			if {[info exists iomd(${iomtype},sec)]} {
				foreach iom $iomd(${iomtype},sec) {
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 $iom oos,oosma";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::<iom$iom>:::swversion=<${iomtype}_load>";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom oos,oosma";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom isstbyc";
				}
				if {[info exists iomd(${iomtype},pri)]} {
					foreach iom $iomd(${iomtype},pri) {
						set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::<iom$iom>";
						set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom oos,oosma";
						set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::<iom$iom>:::swversion=<${iomtype}_load>";
						set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::<iom$iom>";
						set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom isact";
					}
				}
			} else {
				foreach iom $iomd(${iomtype},pri) {
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 $iom oos,oosma";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::<iom$iom>:::swversion=<${iomtype}_load>";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom oos,oosma";
					set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::<iom$iom>";
					set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 $iom isact,isactnbk";
				}
			}
		}
	}
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	set cmdd(iomhug,[incr cmd]) "}";
	#
	set cmdd(iomhug,first) 1;
	set cmdd(iomhug,last) $cmd;
	#
	puts "\n";
	puts "Generate IomHUG Cmds: ";
	puts "First Cmd: $cmdd(iomhug,first)";
	puts "Last Cmd: $cmdd(iomhug,last)";
	#
	return "0 - success";
}
#
proc generateHugCmds { cfgdata cmddata { huglite 0 } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	global ncleis;
	#
	puts "\nGenerate HUG Cmds:";
	#
	# use cfgdata to generate HUG scripts.
	#
	if {![info exists cfgd(globals,switchid)] ||
	     [string length $cfgd(globals,switchid)] <= 0} {
		return "-1 - generateHugCmds: switchid is unknown or null-length.";
	}
	set switchid $cfgd(globals,switchid);
	set spdata(switchid) $switchid;
	#
	if {![info exists cfgd(globals,load,branch)] ||
	     [string length $cfgd(globals,load,branch)] <= 0} {
		return "-1 - generateHugCmds: branch is unknown or null-length.";
	}
	set branch $cfgd(globals,load,branch);
	set spdata(branch) $branch;
	#
	set spdata(spa,ip) $cfgd(globals,$switchid,spa,ip);
	set spdata(spb,ip) $cfgd(globals,$switchid,spb,ip);
	set spdata(spa,state) $cfgd($spdata(spa,ip),spstate);
	set spdata(spb,state) $cfgd($spdata(spb,ip),spstate);
	#
	if {$spdata(spa,state) == "isact"} {
		set spdata(active) spa;
		set spdata(standby) spb;
	} elseif {$spdata(spb,state) == "isact"} {
		set spdata(active) spb;
		set spdata(standby) spa;
	} else {
		return "-1 - generateHugCmds: active SP is unknown.";
	}
	puts "\n";
	puts "Switch ID       : $spdata(switchid)";
	puts "Branch          : $spdata(branch)";
	puts "Active SP       : $spdata(active)";
	puts "Active SP IP    : $spdata($spdata(active),ip)";
	puts "Active SP State : $spdata($spdata(active),state)";
	puts "Standby SP      : $spdata(standby)";
	puts "Standby SP IP   : $spdata($spdata(standby),ip)";
	puts "Standby SP State: $spdata($spdata(standby),state)";
	#
	# sort ioms by type
	#
	for {set iom 1} {[info exists cfgd(iom,$iom,iomstate)]} {incr iom} {
		# skip if clei code is not known
		if {![info exists cfgd(iom,$iom,clei)] || \
		    [string length $cfgd(iom,$iom,clei)] <= 0} {
			puts "\nSkipping IOM $iom ... no data.";
			continue;
		} else {
			puts "\nProcessing IOM $iom ...";
			lappend iomdata(ioms) $iom;
		}
		#
		set clei $cfgd(iom,$iom,clei);
		set rearclei $cfgd(iom,$iom,clei);
		#
		set iomdata($iom,clei) $clei;
		set iomdata($iom,rearclei) $rearclei;
		#
		if {[info exists ncleis($branch,$clei,$rearclei)]} {
			set iomdata($iom,type) $ncleis($branch,$clei,$rearclei);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis(default,$clei,$rearclei)]} {
			set iomdata($iom,type) $ncleis(default,$clei,$rearclei);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis($branch,$clei,none)]} {
			set iomdata($iom,type) $ncleis($branch,$clei,none);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis(default,$clei,none)]} {
			set iomdata($iom,type) $ncleis(default,$clei,none);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} else {
			puts "Unknown CLEI value $clei for IOM $iom";
			return "-1 - generateHugCmds: Unknown CLEI value $clei for IOM $iom";
		}
		#
		lappend iomdata($iomdata($iom,type)) $iom;
		set iomtype $iomdata($iom,type);
		#
		if {$iomtype != "cpu"} {
			lappend iomdata(noncpuioms) $iom;
		}
		lappend iomdata(${iomtype}ioms) $iom;
		#
		if {![info exists iomdata(types)] ||
		     [lsearch -exact $iomdata(types) $iomtype] == -1} {
			lappend iomdata(types) $iomtype;
		}
		#
		if {$cfgd(iom,$iom,protection) == "pri"} {
			lappend iomdata($iomdata($iom,type),pri) $iom;
		} elseif {$cfgd(iom,$iom,protection) == "sec"} {
			lappend iomdata($iomdata($iom,type),sec) $iom;
		} else {
			puts "Unknown protection value $cfgd(iom,$iom,protection) for IOM $iom";
			return "-1 - generateHugCmds: Unknown protection value $cfgd(iom,$iom,protection) for IOM $iom";
		}
		# copy remaining data
		set iomdata($iom,version) $cfgd(iom,$iom,version);
		set iomdata($iom,state) $cfgd(iom,$iom,iomstate);
		set iomdata($iom,protection) $cfgd(iom,$iom,protection);
	}
	#
	# dump spdata and iomdata
	#
	puts "\nCPU Data:";
	foreach item [lsort [array names spdata]] {
		puts "spdata\($item\): $spdata($item)";
	}
	puts "\nIOM Data:";
	foreach item [lsort [array names iomdata]] {
		puts "iomdata\($item\): $iomdata($item)";
	}
	#
	set status [generateFileHeader cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateFileHeader failed.\n$status";
	}
	#
	set status [generateGlobals cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateGlobals failed.\n$status";
	}
	#
	set status [generateFunctions cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateFunctions failed.\n$status";
	}
	#
	set status [generatePreHugCmds cfgd cmdd spdata iomdata $huglite];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generatePreHugCmds failed.\n$status";
	}
	#
	set status [generateCpuHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateCpuHugCmds failed.\n$status";
	}
	#
	set status [generateCmHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateCmHugCmds failed.\n$status";
	}
	#
	set status [generateIomHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateIomHugCmds failed.\n$status";
	}
	#
	set status [generatePostHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generatePostHugCmds failed.\n$status";
	}
	#
	return "0 - success";
}
#
proc printHugScript { cfgdata cmddata { outfile "" } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	if {![info exists cmdd(sections)] || \
	     [string length $cmdd(sections)] <= 0} {
		puts "printHugScript: no script to write.";
		return "-1 - printHugScript: no script to write.";
	}
	#
	puts "\nPrint HUG Script:";
	foreach sec $cmdd(sections) {
		set first $cmdd($sec,first);
		set last $cmdd($sec,last);
		#
		puts "###### START OF SECTION $sec ######";
		for {set cmd $first} {$cmd<=$last} {incr cmd} {
			puts "$cmdd($sec,$cmd)";
		}
		puts "###### END OF SECTION $sec ######";
	}
	#
	if {[string length $outfile] > 0} {
		set ofd [open $outfile "w"];
		foreach sec $cmdd(sections) {
			set first $cmdd($sec,first);
			set last $cmdd($sec,last);
			#
			puts $ofd "###### START OF SECTION $sec ######";
			for {set cmd $first} {$cmd<=$last} {incr cmd} {
				puts $ofd "$cmdd($sec,$cmd)";
			}
			puts $ofd "###### END OF SECTION $sec ######";
		}
		close $ofd;
	}
	#
	return "0 - success";
}
proc generateLabPrepCmds { cfgdata cmddata { huglite 0 } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	global ncleis;
	#
	puts "\nGenerate Lab Preparation Cmds:";
	#
	# use cfgdata to generate lab preparation scripts.
	#
	if {![info exists cfgd(globals,switchid)] ||
	     [string length $cfgd(globals,switchid)] <= 0} {
		return "-1 - generateLabPrepCmds: switchid is unknown or null-length.";
	}
	set switchid $cfgd(globals,switchid);
	set spdata(switchid) $switchid;
	#
	if {![info exists cfgd(globals,load,branch)] ||
	     [string length $cfgd(globals,load,branch)] <= 0} {
		return "-1 - generateLabPrepCmds: branch is unknown or null-length.";
	}
	set branch $cfgd(globals,load,branch);
	set spdata(branch) $branch;
	#
	set spdata(spa,ip) $cfgd(globals,$switchid,sp,a,ip);
	set spdata(spb,ip) $cfgd(globals,$switchid,sp,b,ip);
	set spdata(spa,state) $cfgd($spdata(spa,ip),spstate);
	set spdata(spb,state) $cfgd($spdata(spb,ip),spstate);
	#
	if {$spdata(spa,state) == "isact"} {
		set spdata(active) spa;
		set spdata(standby) spb;
	} elseif {$spdata(spb,state) == "isact"} {
		set spdata(active) spb;
		set spdata(standby) spa;
	} else {
		return "-1 - generateLabPrepCmds: active SP is unknown.";
	}
	puts "\n";
	puts "Switch ID       : $spdata(switchid)";
	puts "Branch          : $spdata(branch)";
	puts "Active SP       : $spdata(active)";
	puts "Active SP IP    : $spdata($spdata(active),ip)";
	puts "Active SP State : $spdata($spdata(active),state)";
	puts "Standby SP      : $spdata(standby)";
	puts "Standby SP IP   : $spdata($spdata(standby),ip)";
	puts "Standby SP State: $spdata($spdata(standby),state)";
	#
	# sort ioms by type
	#
	for {set iom 1} {[info exists cfgd(iom,$iom,iomstate)]} {incr iom} {
		# skip if clei code is not known
		if {![info exists cfgd(iom,$iom,clei)] || \
		    [string length $cfgd(iom,$iom,clei)] <= 0} {
			puts "\nSkipping IOM $iom ... no data.";
			continue;
		} else {
			puts "\nProcessing IOM $iom ...";
			lappend iomdata(ioms) $iom;
		}
		#
		set clei $cfgd(iom,$iom,clei);
		set rearclei $cfgd(iom,$iom,rearclei);
		#
		set iomdata($iom,clei) $clei;
		set iomdata($iom,rearclei) $rearclei;
		#
		if {[info exists ncleis($branch,$clei,$rearclei)]} {
			set iomdata($iom,type) $ncleis($branch,$clei,$rearclei);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis(default,$clei,$rearclei)]} {
			set iomdata($iom,type) $ncleis(default,$clei,$rearclei);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis($branch,$clei,none)]} {
			set iomdata($iom,type) $ncleis($branch,$clei,none);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} elseif {[info exists ncleis(default,$clei,none)]} {
			set iomdata($iom,type) $ncleis(default,$clei,none);
			puts "IOM $iom Type ($branch,$clei,$rearclei): $iomdata($iom,type)";
		} else {
			puts "Unknown CLEI value $clei for IOM $iom";
			return "-1 - generateLabPrepCmds: Unknown CLEI value $clei for IOM $iom";
		}
		#
		lappend iomdata($iomdata($iom,type)) $iom;
		set iomtype $iomdata($iom,type);
		#
		if {$iomtype != "cpu"} {
			lappend iomdata(noncpuioms) $iom;
		}
		lappend iomdata(${iomtype}ioms) $iom;
		#
		if {![info exists iomdata(types)] ||
		     [lsearch -exact $iomdata(types) $iomtype] == -1} {
			lappend iomdata(types) $iomtype;
		}
		#
		if {$cfgd(iom,$iom,protection) == "pri"} {
			lappend iomdata($iomdata($iom,type),pri) $iom;
		} elseif {$cfgd(iom,$iom,protection) == "sec"} {
			lappend iomdata($iomdata($iom,type),sec) $iom;
		} else {
			puts "Unknown protection value $cfgd(iom,$iom,protection) for IOM $iom";
			return "-1 - generateLabPrepCmds: Unknown protection value $cfgd(iom,$iom,protection) for IOM $iom";
		}
		# copy remaining data
		set iomdata($iom,version) $cfgd(iom,$iom,version);
		set iomdata($iom,state) $cfgd(iom,$iom,iomstate);
		set iomdata($iom,protection) $cfgd(iom,$iom,protection);
	}
	#
	# dump spdata and iomdata
	#
	puts "\nCPU Data:";
	foreach item [lsort [array names spdata]] {
		puts "spdata\($item\): $spdata($item)";
	}
	puts "\nIOM Data:";
	foreach item [lsort [array names iomdata]] {
		puts "iomdata\($item\): $iomdata($item)";
	}
	#
	# set status [generateFileHeader cfgd cmdd spdata iomdata];
	# if {[isNotOk $status]} {
		# return "-1 - generateLabPrepCmds: generatePrepFileHeader failed.\n$status";
	# }
	#
	# set status [generateLabPrepGlobals cfgd cmdd spdata iomdata];
	# if {[isNotOk $status]} {
		# return "-1 - generateLabPrepCmds: generatePrepGlobals failed.\n$status";
	# }
	#
	# set status [generateLabPrepFunctions cfgd cmdd spdata iomdata];
	# if {[isNotOk $status]} {
		# return "-1 - generateLabPrepCmds: generatePrepFunctions failed.\n$status";
	# }
	#
	# set status [generateLabPrepCmds cfgd cmdd spdata iomdata $huglite];
	# if {[isNotOk $status]} {
		# return "-1 - generateLabPrepCmds: generatePrepHugCmds failed.\n$status";
	# }
	#
	return "0 - success";
}
#
###########################################################################
#
# NOT DONE YET ...
#
###########################################################################
#
proc execksh { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set vflag ""; set iflag 0;
	set tmout 30;
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag "-x"; }
		{^-i} { set iflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	#
	set argv [lreplace $argv 0 [incr arg -1]];
	set xcmd [string trim [join $argv " "]];
	set kshcmd "$xcmd";
	#
	if {[catch { exec /usr/bin/ksh $vflag "$kshcmd"; } errmsg] != 0} {
		catch { wait; } ignore;
		if {[string length $errmsg] > 0} {
			return "-1 - execksh: system failed: $errmsg";
		} else {
			return "-1 - execksh: system failed: unknown reason";
		}
	} else {
		catch { wait; } ignore;
		return "0 - success";
	}
}
#
proc execsh { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set vflag ""; set iflag 0;
	set tmout 30;
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag "-x"; }
		{^-i} { set iflag 1; }
		{^-t.*} { lgetoptval argv $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	#
	set argv [lreplace $argv 0 [incr arg -1]];
	set xcmd [string trim [join $argv " "]];
	set kshcmd "$xcmd";
	#
	if {[catch { exec /usr/bin/ksh $vflag "$kshcmd"; } errmsg] != 0} {
		catch { wait; } ignore;
		if {[string length $errmsg] > 0} {
			return "-1 - execksh: system failed: $errmsg";
		} else {
			return "-1 - execksh: system failed: unknown reason";
		}
	} else {
		catch { wait; } ignore;
		return "0 - success";
	}
	#
	return "0 - success";
}
#
proc execrsh { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execrsh: argv($arg) = <$argval>";
	}
	#
	return "0 - success";
}




