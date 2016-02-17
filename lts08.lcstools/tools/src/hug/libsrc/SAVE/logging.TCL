# write msgs to a log file.
package provide logging 1.0
#
proc logopen { logfname logfdname } {
	upvar $logfdname logfd;
	if {[file exists $logfname]} {
		set logfd [open $logfname "a+"];
	} else {
		set logfd [open $logfname "w+"];
	}
	puts $logfd "===>>> opened $logfname at [exec date] <<<===";
	flush $logfd;
}
#
proc logwrite { logfd { logmsg "" } } {
	puts $logfd "$logmsg";
	flush $logfd;
}
#
proc logclose { logfd } {
	catch { close $logfd; } ignore;
}
#
proc logusage { logfname } {
	global env;
	global argv0;
	#
	logopen $env(LCSTOOLS)/tools/logfiles/$logfname logfd;
	logwrite $logfd "Running [file tail $argv0] - [ exec id ]";
	logclose $logfd;
}
#
proc logmsg { logfname logmsg } {
	global env;
	global argv0;
	#
	logopen $env(LCSTOOLS)/tools/logfiles/$logfname logfd;
	logwrite $logfd "$logmsg - [ exec id ]";
	logclose $logfd;
}
#
proc logcat { logfname { filename "" } } {
	global env;
	global argv0;
	#
	logopen $env(LCSTOOLS)/tools/logfiles/$logfname logfd;
	#
	if {![file readable $filename]} {
		# file does not exist or is not readable
		logwrite $logfd "file $filename is not readable - [ exec id ]";
		logclose $logfd;
		return;
	}
	if {[catch {set fd [open $filename "r"]} status]} {
		# not readable
		logwrite $logfd "unable to open file $filename - [ exec id ]";
		logclose $logfd;
		return;
	}
	#
	logwrite $logfd "printing contents of $filename - [ exec id ]";
	while {[gets $fd inbuf] != -1} {
		logwrite $logfd "$inbuf";
	}
	catch { close $fd; } ignore;
	#
	logclose $logfd;
}


