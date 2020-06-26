# execute cmds in scripts.
package provide scripts 1.0
#
package require getoptval
package require hugftp
package require hugtelnet
package require hugtl1
package require checkretval
package require hughardware
package require cleis
#
proc execwaitforiomstate { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
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
		if {[lsearch -exact {ignore unknown isact isactnbk isstbyc isstbyh} $reqstate] == -1} {
			return "-1 - execwaitforiomstate: invalid requested iom state: $reqstate";
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
		if {[lsearch -exact {ignore unknown ping minmode appmode isact isactnbk isactsync isstbyc isstbysync isstbyh oosmahalt} $reqstate] == -1} {
			return "-1 - execwaitforcpustate: invalid requested spa state: $reqstate";
		}
	}
	regsub -all -- "," $spdata(spb,reqstate) " " reqstates;
	set reqstateslist [split $reqstates " "];
	set reqstatescount [llength $reqstateslist];
	for {set istate 0} {$istate<$reqstatescount} {incr istate} {
		set reqstate [lindex $reqstateslist $istate];
		if {[lsearch -exact {ignore unknown ping minmode appmode isact isactnbk isactsync isstbyc isstbysync isstbyh oosmahalt} $reqstate] == -1} {
			return "-1 - execwaitforcpustate: invalid requested spb state: $reqstate";
		}
	}
	#
	set status [hug_waitforcpustate cfgd $vflag $tmout spdata];
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
		incr arg; set ftpfrompath [lindex $argv $arg];
		incr arg; set ftptopath [lindex $argv $arg];
		set status [hug_ftp_put cfgd $streamid $ftpfrompath $ftptopath $aflag $bflag $iflag $vflag $tmout]
	}
	{^get$} {
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
	if {[lsearch -exact {open open2 exec file close} $oper] == -1} {
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
		incr arg; set filename [lindex $argv $arg];
		set status [hug_tl1_file cfgd $streamid $iflag $vflag $tmout $filename];
	}
	{^exec$} {
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
	set vflag 0; set iflag 0;
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
		set status [hug_telnet_open cfgd $streamid $vflag $tmout $tip $tlogin $tpasswd];
	}
	{^exec$} {
		set argv [lreplace $argv 0 $arg];
		set xcmd [string trim [join $argv " "]];
		set status [hug_telnet_exec cfgd $streamid $iflag $vflag $tmout $xcmd]l;
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
	set argv [split $xcmd " "];
	set argc [llength $argv];
	set sleeptime [lindex $argv 0];
	#
	if {$sleeptime < 0} {
		return "-1 - execsleep: sleep time ($sleeptime) < 0.";
	}
	#
	catch { sleep $sleeptime; } ignore;
	#
	return "0 - success";
}
#
proc execsystem { cfgdata section cmd xcmd } {
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
	system "$syscmd";
	catch { wait; } ignore;
	return "0 - success";
}
#
proc execwhereisload { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
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
	set spdata(spa,ip) [lindex $argv $arg];
	incr arg; set spdata(spb,ip) [lindex $argv $arg];
	incr arg; set spdata(login) [lindex $argv $arg];
	incr arg; set spdata(passwd) [lindex $argv $arg];
	#
	set status [hug_whereisload cfgd $vflag $tmout spdata];
	if {[isNotOk $status]} {
		return "-1 - execwhereisload: hug_whereisload failed.\n$status";
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
		return "-1 - unknown cmd: $xcmdtype.";
	} elseif {$status > 1} {
		return "-1 - ambiguous cmd: $xcmdtype.";
	}
	#
	# check if we have a function call
	#
	if {$xcmdtype == "call"} {
		# calling a section as a function, must do argument
		# substitution before global substitution.
		#
		puts "\nCalling $xcmd2exec to execute cmd: \n$xcmdtail";
		#
		set status [$xcmd2exec cfgd $section $cmd $xcmdtail];
		if {[isNotOk $status]} {
			return "-1 - execCmd: cmd $cmd ($xcmd2exec) failed.\n$status";
		} else {
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
	puts "\nCalling $xcmd2exec to execute cmd: \n$finalxcmd";
	#
	set status [$xcmd2exec cfgd $section $cmd $finalxcmd];
	if {[isNotOk $status]} {
		return "-1 - execCmd: cmd $cmd ($xcmd2exec) failed.\n$status";
	} else {
		return "0 - success";
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
proc execScript { cfgdata {startsec ""} {startcmd 1} } {
	upvar $cfgdata cfgd;
	#
	# find the the section to execute.
	#
	if {[string length $startsec] <= 0} {
		# look for the first section, should be marked
		# when data file was read in.
		#
		if {![info exists cfgd(internal,exec,1)]} {
			return "-1 - execScript: unknown first section to execute.";
		}
		#
		set execsec $cfgd(internal,exec,1);
	} else {
		set execsec $startsec;
		if {![info exists cfgd($execsec,type)]} {
			return "-1 - execScript: unknown type for section $execsec.";
		} elseif {$cfgd($execsec,type) != "exec"} {
			return "-1 - execScript: section $execsec is not executable.";
		}
	}
	#
	# check start and end cmds
	#
	if {$startcmd < 1} {
		return "-1 - execScript: startcmd ($startcmd) less than 1.";
	}
	#
	if {![info exists cfgd($execsec,maxcmd)]} {
		return "-1 - execScript: maxcmd for section $execsec is undefined.";
	}
	set maxcmd $cfgd($execsec,maxcmd);
	if {$maxcmd < 1} {
		return "-1 - execScript: maxcmd ($maxcmd) less than 1.";
	}
	if {$startcmd > $maxcmd} {
		return "-1 - execScript: startcmd ($startcmd) > maxcmd ($maxcmd)";
	}
	#
	# start executing the cmds
	#
	for {set cmdno $startcmd} {$cmdno<=$maxcmd} {incr cmdno} {
		if {![info exists cfgd($execsec,$cmdno)]} {
			return "-1 - execScript: cmd $cmdno for section $execsec is undefined.";
		}
		set xcmd $cfgd($execsec,$cmdno);
		set status [execCmd cfgd $execsec $cmdno $xcmd];
		if {[isNotOk $status]} {
			return "-1 - execScript: cmd $cmdno failed.\n$status";
		}
	}
	#
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
	set cmdd(globals,[incr cmd]) "activesp {";
	set cmdd(globals,[incr cmd]) "\tsp $spd(active)";
	set cmdd(globals,[incr cmd]) "\treqstate isact";
	set cmdd(globals,[incr cmd]) "}";
	set cmdd(globals,[incr cmd]) "standbysp {";
	set cmdd(globals,[incr cmd]) "\tsp $spd(standby)";
	set cmdd(globals,[incr cmd]) "\treqstate isstbyh";
	set cmdd(globals,[incr cmd]) "}";
	set cmdd(globals,[incr cmd]) "passwd $cfgd(globals,$switchid,passwd)";
	set cmdd(globals,[incr cmd]) \
			"tl1login $cfgd(globals,$switchid,tl1login)";
	set cmdd(globals,[incr cmd]) \
			"tl1passwd $cfgd(globals,$switchid,tl1passwd)";
	set cmdd(globals,[incr cmd]) "spa {";
	set cmdd(globals,[incr cmd]) "\tip $cfgd(globals,$switchid,spa,ip)";
	if ($spd(active) == "spa"} {
		set cmdd(globals,[incr cmd]) "\tstate actviesp";
	} elseif ($spd(standby) == "spa"} {
		set cmdd(globals,[incr cmd]) "\tstate standbysp";
	} else {
		return "-1 - generateGlobals: (SP-A) invalid data - active SP <$spd(active)>, standby SP <$spd(standby)>";
	}
	set cmdd(globals,[incr cmd]) "}";
	set cmdd(globals,[incr cmd]) "spb {";
	set cmdd(globals,[incr cmd]) "\tip $cfgd(globals,$switchid,spb,ip)";
	if ($spd(active) == "spb"} {
		set cmdd(globals,[incr cmd]) "\tstate actviesp";
	} elseif ($spd(standby) == "spb"} {
		set cmdd(globals,[incr cmd]) "\tstate standbysp";
	} else {
		return "-1 - generateGlobals: (SP-B) invalid data - active SP <$spd(active)>, standby SP <$spd(standby)>";
	}
	set cmdd(globals,[incr cmd]) "}";
	set cmdd(globals,[incr cmd]) "# START OF REQUIRED DEFINITIONS";
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
proc generatePreHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	lappend cmdd(sections) "prehug";
	#
	set cmd 0;
	set switchid $cfgd(globals,switchid);
	#
	set cmdd(prehug,[incr cmd]) "prehug exec {";
	#
	# DLP-700
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-eqpt";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-chassis-eqpt";
	#
	# DLP-701
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,/home1/Telica/swCPU/";
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
				set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-r,$cpudir";
			}
		}
	}
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rmv-eqpt::<<standbysp>>";
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "waitforcpustate -t900 <<activesp>.ip> isactnbk <login> <passwd> <tl1login> <tl1passwd> <<standbysp>.ip> oosmahalt <login> <passwd> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <<activesp>.ip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rst-eqpt::<<standbysp>>";
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "waitforcpustate -t900 <<activesp>.ip> isact <login> <passwd> <tl1login> <tl1passwd> <<standbysp>.ip> isstbh <login> <passwd> <tl1login> <tl1passwd>";
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
	return "0 - success";
}
#
proc generateCpuHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	return "0 - success";
}
#
proc generateIomHugCmds { cfgdata cmddata spdata iomdata} {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $spdata spd;
	upvar $iomdata iomd;
	#
	return "0 - success";
}
#
proc generateHugCmds { cfgdata cmddata } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	global cleis;
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
		set iomdata($iom,clei) $clei;
		if {[info exists cleis($branch,$clei)]} {
			set iomdata($iom,type) $cleis($branch,$clei);
			puts "IOM $iom Type ($branch): $iomdata($iom,type)";
		} elseif {[info exists cleis($clei)]} {
			set iomdata($iom,type) $cleis($clei);
			puts "IOM $iom Type: $iomdata($iom,type)";
		} else {
			puts "Unknown CLEI value $clei for IOM $iom";
			return "-1 - generateHugCmds: Unknown CLEI value $clei for IOM $iom";
		}
		#
		lappend iomdata($iomdata($iom,type)) $iom;
		set iomtype $iomdata($iom,type);
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
	set status [generateGlobals cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateGlobals failed.\n$status";
	}
	#
	set status [generatePreHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generatePreHugCmds failed.\n$status";
	}
	#
	set status [generateCpuHugCmds cfgd cmdd spdata iomdata];
	if {[isNotOk $status]} {
		return "-1 - generateHugCmds: generateCpuHugCmds failed.\n$status";
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
