# check HUG environment
package provide checkenv 1.0
#
proc checkenv { } {
	global env;
	set errcnt 0;
	#
	set hugvars [list LCSHUGBIN LCSHUGDATA LCSHUGLIB LCSHUGLOGFILES];
	#
	foreach hugvar $hugvars {
		if {![info exists env(${hugvar})]} {
			puts "checkenv: ${hugvar} not defined."
			incr errcnt;
		}
	}
	if {$errcnt > 0} {
		exit 2;
	}
	return 0;
}
