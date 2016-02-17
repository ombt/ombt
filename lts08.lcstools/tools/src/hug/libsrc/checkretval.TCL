# check return values
package provide checkretval 1.0
#
proc isOk { { status -1 } } {
	if {[string match "0*" ${status}]} {
		return 1;
	} else {
		return 0;
	}
}
#
proc isNotOk { { status -1 } } {
	if {[string match "0*" ${status}]} {
		return 0;
	} else {
		return 1;
	}
}
