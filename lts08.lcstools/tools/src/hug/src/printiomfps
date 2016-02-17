#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
package require db
package require checkenv
package require checkretval
package require iomfps
#
log_user 0;
#
checkenv;
#
set status [initIomFpsData];
if {[isNotOk $status]} {
	puts "\nERROR: initIomFpsData failed: \n${status}";
	exit 2;
}
set status [printIomFpsData];
if {[isNotOk $status]} {
	puts "\nERROR: printIomFpsData failed: \n${status}";
	exit 2;
}
#
exit 0;
