#!/opt/exp/bin/expect
#
# timer. interrupt when done.
#
trap {
	global start;
	catch {
		puts "\nSLEEP INTERRUPTED: [expr [timestamp] - $start] seconds.";
	} ignore;
	exit 0;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
set start [timestamp];
set maxtime [lindex $argv 0];
#
if {![info exists maxtime] || $maxtime <= 0} {
	# default 1 hour
	set maxtime 3600;
	puts "DEFAULT MAX SLEEP TIME: $maxtime";
} else {
	puts "MAX SLEEP TIME: $maxtime";
}
#
sleep $maxtime;
puts "SLEPT MAXTIME: $maxtime seconds.";
#
exit 0;
