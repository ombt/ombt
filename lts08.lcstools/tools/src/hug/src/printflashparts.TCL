#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
package require db
package require checkenv
package require checkretval
package require flashparts
#
log_user 0;
#
checkenv;
#
set status [initFlashPartsData];
if {[isNotOk $status]} {
	puts "\nERROR: initFlashPartsData failed: \n${status}";
	exit 2;
}
set status [printFlashPartsData];
if {[isNotOk $status]} {
	puts "\nERROR: printFlashPartsData failed: \n${status}";
	exit 2;
}
#
exit 0;
