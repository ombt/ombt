#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
package require db
package require checkenv
package require checkretval
package require iomips
#
log_user 0;
#
checkenv;
#
set status [initIomIpsData];
if {[isNotOk $status]} {
	puts "\nERROR: initIomIpsData failed: \n${status}";
	exit 2;
}
set status [printIomIpsData];
if {[isNotOk $status]} {
	puts "\nERROR: printIomIpsData failed: \n${status}";
	exit 2;
}
#
exit 0;
