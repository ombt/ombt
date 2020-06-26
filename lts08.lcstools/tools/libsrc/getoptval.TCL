# return option value.
package provide getoptval 1.0
#
proc getoptval { argval varname argname } {
	global argv;
	upvar $varname var;
	upvar $argname arg;
	#
	if {[string length $argval] == 2} {
		incr arg;
		set var [lindex $argv $arg];
	} else {
		set var [string range $argval 2 end];
	}
}
#
