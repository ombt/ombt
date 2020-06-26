# execute cmds in scripts.
package provide scripts 1.0
#
package require getoptval
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
	set vflag 0; set aflag 0; set bflag 1;
	set tmout 30;
	#
	for {set arg 1} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-a} { set aflag 1; set bflag 0; }
		{^-b} { set aflag 0; set bflag 1; }
		{^-t.*} { getoptval $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	set streamid [lindex $argv $arg];
	#
	puts "oper: $oper";
	puts "aflag: $aflag";
	puts "vflag: $vflag";
	puts "bflag: $bflag";
	puts "tmout: $tmout";
	puts "streamid: $streamid";
	#
	set argv [lreplace $argv 0 $arg];
	set xcmd [string trim [join $argv " "]];
	puts "xcmd: <$xcmd>";
	#
	return "0 - success";
}
proc execcall { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argv} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execcall: argv($arg) = <$argval>";
	}
	#
	return "0 - success";
}
proc exectl1 { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set xcmd [string trim $xcmd];
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	set oper [lindex $argv 0];
	if {[lsearch -exact {open exec close} $oper] == -1} {
		return "-1 - exectl1: invalid operation: $oper";
	}
	#
	set vflag 0; 
	set tmout 30;
	#
	for {set arg 1} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		switch -regexp -- $argval {
		{^$} { #skip white spaces }
		{^-v} { set vflag 1; }
		{^-t.*} { getoptval $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	set streamid [lindex $argv $arg];
	#
	puts "oper: $oper";
	puts "vflag: $vflag";
	puts "tmout: $tmout";
	puts "streamid: $streamid";
	#
	set argv [lreplace $argv 0 $arg];
	set xcmd [string trim [join $argv " "]];
	puts "xcmd: <$xcmd>";
	#
	return "0 - success";
}
proc execksh { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execksh: argv($arg) = <$argval>";
	}
	#
	return "0 - success";
}
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
		{^-t.*} { getoptval $argval tmout arg; }
		{^--} { incr arg; break; }
		{^-.*} { break; }
		default { break; }
		}
	}
	set streamid [lindex $argv $arg];
	#
	puts "oper: $oper";
	puts "vflag: $vflag";
	puts "iflag: $iflag";
	puts "tmout: $tmout";
	puts "streamid: $streamid";
	#
	set argv [lreplace $argv 0 $arg];
	set xcmd [string trim [join $argv " "]];
	puts "xcmd: <$xcmd>";
	#
	return "0 - success";
}
proc execsh { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execsh: argv($arg) = <$argval>";
	}
	#
	return "0 - success";
}
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
proc execwait_for_boot { cfgdata section cmd xcmd } {
	upvar $cfgdata cfgd;
	#
	set argv [split $xcmd " "];
	set argc [llength $argv];
	#
	for {set arg 0} {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		puts "execwait_for_boot: argv($arg) = <$argval>";
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
	while {[regexp -- {<([_a-zA-Z0-9\.]+)>} $cmd ignore var] == 1} {
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
	set status [$xcmd2exec cfgs $section $cmd $finalxcmd];
	if {[isNotOk $status]} {
		return "-1 - execCmd: cmd $cmd ($xcmd2exec) failed.\n$status";
	} else {
		return "0 - success";
	}
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

