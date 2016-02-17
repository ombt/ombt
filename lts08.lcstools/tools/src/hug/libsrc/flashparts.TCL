#
package provide flashparts 1.0
#
package require db
package require checkretval
# 
set flashparts(default,default) "default";
#
# minimal set of flash partitions to update for each iom type.
#
# set flashparts(minimal,ana) "kdi";
# set flashparts(minimal,atmds3) "kdi";
# set flashparts(minimal,cm) "kdi";
# set flashparts(minimal,cpu) "kdi";
# set flashparts(minimal,debug_cpu) "kdi";
# set flashparts(minimal,ds1) "kdi";
# set flashparts(minimal,ds1_2) "kdi";
# set flashparts(minimal,ds3) "kdi";
# set flashparts(minimal,e1) "kdi";
# set flashparts(minimal,ena) "ife efe kdi";
# set flashparts(minimal,ena2) "ife efe kdi";
# set flashparts(minimal,octds3) "kdi";
# set flashparts(minimal,octds3_2) "kdi";
# set flashparts(minimal,octds3_3) "kdi";
# set flashparts(minimal,pna) "kdi";
# set flashparts(minimal,tdmoc) "kdi";
# set flashparts(minimal,trids3) "kdi";
# set flashparts(minimal,trids3_3) "kdi";
# set flashparts(minimal,voip) "kdi";
# set flashparts(minimal,voip6) "kdi";
# set flashparts(minimal,vs2) "kdi";
# set flashparts(minimal,vs3) "kdi";
#
proc printFlashPartsData { } {
	global flashparts;
	foreach item [lsort -dictionary [array names flashparts]] {
		puts "flashparts\($item\): $flashparts($item)";
	}
	return "0 - success";
}
#
proc initFlashPartsData { } {
	set status [dbselect tuples flashparts "" "list iomtype flashparts"];
	if {[isNotOk $status]} {
		return "-1 - initFlashPartsData: dbselect of relation 'flashparts' failed: \n${status}";
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		return "-1 - initFlashPartsData: No data found in relation 'flashparts'.";
	}
	#
	global flashparts;
	unset flashparts;
	#
	foreach tuple $tuples {
		set listname [lindex $tuple 0];
		set iomtype [lindex $tuple 1];
		regsub -all -- "," [lindex $tuple 2] " " fplist;
		#
		set flashparts($listname,$iomtype) $fplist;
	}
	#
	return "0 - success";
}

