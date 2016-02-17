#!/opt/exp/bin/tclsh
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
package require Expect
package require db
package require checkenv
package require checkretval
package require ncleis
#
log_user 0;
#
checkenv;
#
set status [initCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: initCleiData failed: \n${status}";
	exit 2;
}
set status [printCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: printCleiData failed: \n${status}";
	exit 2;
}
#
exit 0;
