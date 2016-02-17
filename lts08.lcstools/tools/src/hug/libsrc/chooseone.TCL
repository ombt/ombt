package provide chooseone 1.0
#
proc chooseone { prompt options option {optionslevel 1} {optionlevel 1} } {
	eval upvar $optionslevel $options oplist;
	eval upvar $optionlevel $option op;
	global timeout;
	global user_spawn_id;;
	#
	set op "";
	#
	set savetimeout $timeout;
	set timeout -1;
	#
	set iopstart 0;
	set iopdelta 20;
	set maxiop [llength $oplist];
	#
	while { $op == "" } {
		if {$iopstart >= $maxiop} {
			set iopstart 0;
		}
		set iopend [expr $iopstart + $iopdelta];
		if {$iopend >= $maxiop} {
			set iopend $maxiop;
		}
		for {set iop $iopstart} {$iop<$iopend} {incr iop} {
			set opval [lindex $oplist $iop];
			send_user "[format "%3d: %s" $iop $opval]\n";
		}
		set iopstart [expr $iopstart + $iopdelta];
		#
		if {$iopend < $maxiop} {
			set lprompt "";
			append lprompt "$prompt" " \[<cr>=more choices] " ;
		} else {
			set lprompt "$prompt";
		}
		send_user "$lprompt";
		#
		expect_user {
		-re "-(\[0-9]+)\n" {
			send_user "\nout-of-range choice.\n";
		}
		-re "(\[0-9]+)\n" {
			set choice $expect_out(1,string);
			if {$choice>=0 && $choice<$maxiop} {
				set op [lindex $oplist $choice];
				break;
			}
			send_user "\nout-of-range choice.\n";
		}
		-re ".*\n" {
			# just continue
		}
		eof {
			send_tty "\n\nEND-OF-FILE. exiting.\n\n";
			exit 2;
		}
		}
	}
	#
	set timeout $savetimeout;
}
