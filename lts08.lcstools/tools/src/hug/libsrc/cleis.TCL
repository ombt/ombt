package provide cleis 1.0
#
package require db
package require checkretval
# 
set cleis(default,default) "default";
#
# set cleis(default,ba1ax60aab) "voip";
# set cleis(default,ba2a30tgaa) "ena";
# set cleis(default,ba2a30tgab) "ena";
# set cleis(default,ba4a701faa) "octds3_2";
# set cleis(default,ba7atp0faa) "ds1_2";
# set cleis(default,ba9ats0faa) "cm";
# set cleis(default,ba9ats0fab) "cm";
# set cleis(default,ba9axy0faa) "octds3_3";
# set cleis(default,ba9iaa0aaa) "ds1";
# set cleis(default,bauiaa1eab) "vs3";
# set cleis(default,bauiaa1eac) "vs3";
# set cleis(default,bauiadpeaa) "ena2";
# set cleis(default,ba4a60zfaa) "trids3";
# set cleis(default,ba4aw60faa) "tdmoc";
# set cleis(default,ba9awx0faa) "trids3_3";
# set cleis(default,ba91x70aaa) "ds3";
# set cleis(default,ba91x70aab) "ds3";
#
# set cleis(Main,ba1ax60aab) "vs2";
# set cleis(Main,ba2a30tgaa) "ena2";
# set cleis(Main,ba2a30tgab) "ena2";
#
# set cleis(BRANCH-6-2-0,ba1ax60aab) "vs2";
# set cleis(BRANCH-6-2-0,ba2a30tgaa) "ena2";
# set cleis(BRANCH-6-2-0,ba2a30tgab) "ena2";
#
# set cleis(BP-6-2-0-1,ba1ax60aab) "vs2";
# set cleis(BP-6-2-0-1,ba2a30tgaa) "ena2";
# set cleis(BP-6-2-0-1,ba2a30tgab) "ena2";
#
# set cleis(BP-6-2-0-2,ba1ax60aab) "vs2";
# set cleis(BP-6-2-0-2,ba2a30tgaa) "ena2";
# set cleis(BP-6-2-0-2,ba2a30tgab) "ena2";
#
# set cleis(BP-6-2-0-3,ba1ax60aab) "vs2";
# set cleis(BP-6-2-0-3,ba2a30tgaa) "ena2";
# set cleis(BP-6-2-0-3,ba2a30tgab) "ena2";
#
# set cleis(BP-6-2-1-1,ba1ax60aab) "vs2";
# set cleis(BP-6-2-1-1,ba2a30tgaa) "ena2";
# set cleis(BP-6-2-1-1,ba2a30tgab) "ena2";
#
# set cleis(BP-6-2-1-2,ba1ax60aab) "vs2";
# set cleis(BP-6-2-1-2,ba2a30tgaa) "ena2";
# set cleis(BP-6-2-1-2,ba2a30tgab) "ena2";
#
# set cleis(BRANCH-DEV-6-2-1,ba2a30tgaa) "ena2";
# set cleis(BRANCH-DEV-6-2-1,ba2a30tgab) "ena2";
#
# set cleis(BRANCH-6-2-1,ba1ax60aab) "vs2";
# set cleis(BRANCH-6-2-1,ba2a30tgaa) "ena2";
# set cleis(BRANCH-6-2-1,ba2a30tgab) "ena2";
#
proc printCleiData { } {
	global cleis;
	foreach item [lsort -dictionary [array names cleis]] {
		puts "cleis\($item\): $cleis($item)";
	}
	return "0 - success";
}
#
proc initCleiData { } {
	set status [dbselect tuples cleis "" "branch clei type"];
	if {[isNotOk $status]} {
		return "-1 - initCleidData: dbselect of relation 'cleis' failed: \n${status}";
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		return "-1 - initCleidData: No data found in relation 'cleis'.";
	}
	#
	global cleis;
	unset cleis;
	#
	foreach tuple $tuples {
		set branch [lindex $tuple 0];
		set clei [lindex $tuple 1];
		set type [lindex $tuple 2];
		#
		set cleis($branch,$clei) $type;
	}
	#
	return "0 - success";
}
#
proc convertCleis { cfgdata { maxiom 17 } } {
	upvar $cfgdata cfgd;
	global cleis;
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
		if {[info exists cleis($branch,$clei)]} {
			set type $cleis($branch,$clei);
		} elseif {[info exists cleis(default,$clei)]} {
			set type $cleis(default,$clei);
		} else {
			return "-1 - convertCleis: Unknown CLEI value $clei for IOM $iom";
		}
		set cfgd(iom,${iom},cleitype) $type;
	}
	return "0 - success";
}
