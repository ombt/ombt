package provide ncleis 1.0
#
package require db
package require checkretval
# 
set ncleis(default,default,default) "default";
#
proc printCleiData { } {
	global ncleis;
	foreach item [lsort -dictionary [array names ncleis]] {
		puts "ncleis\($item\): $ncleis($item)";
	}
	return "0 - success";
}
#
proc initCleiData { } {
	set status [dbselect tuples ncleis "" "branch clei rearclei type"];
	if {[isNotOk $status]} {
		return "-1 - initCleidData: dbselect of relation 'ncleis' failed: \n${status}";
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		return "-1 - initCleidData: No data found in relation 'ncleis'.";
	}
	#
	global ncleis;
	unset ncleis;
	#
	foreach tuple $tuples {
		set branch [lindex $tuple 0];
		set clei [lindex $tuple 1];
		set rearclei [lindex $tuple 2];
		set type [lindex $tuple 3];
		#
		set ncleis($branch,$clei,$rearclei) $type;
	}
	#
	return "0 - success";
}
#
proc convertCleis { cfgdata { maxiom 17 } } {
	upvar $cfgdata cfgd;
	global ncleis;
	#
	set branch $cfgd(globals,load,branch);
	#
	for {set iom 1} {${iom}<=$maxiom} {incr iom} {
		# check if iom data exists
		set cfgd(iom,${iom},cleitype) "none";
		if {![info exists cfgd(iom,${iom},clei)]} {
			continue;
		} elseif {[string length $cfgd(iom,${iom},clei)] <= 0} {
			continue;
		}
		#
		set clei $cfgd(iom,${iom},clei);
		if {[info exists cfgd(iom,${iom},rearclei)]} {
			set rearclei $cfgd(iom,${iom},rearclei);
		} else {
			set rearclei $cfgd(iom,${iom},none);
		}
		#
		if {[info exists ncleis($branch,$clei,$rearclei)]} {
			set type $ncleis($branch,$clei,$rearclei);
		} elseif {[info exists ncleis(default,$clei,$rearclei)]} {
			set type $ncleis(default,$clei,$rearclei);
		} elseif {[info exists ncleis($branch,$clei,none)]} {
			set type $ncleis($branch,$clei,none);
		} elseif {[info exists ncleis(default,$clei,none)]} {
			set type $ncleis(default,$clei,none);
		} else {
			return "-1 - convertCleis: Unknown CLEI values $clei and $rearclei for IOM $iom";
		}
		set cfgd(iom,${iom},cleitype) $type;
	}
	return "0 - success";
}
