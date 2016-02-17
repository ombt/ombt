#
# the data used to populate relation IOMFPS is in the file:
# TelicaRoot/components/ga_dev/solution/eqm/src/eqmtables.c
# look at the structure:
# PARTITION_INFO_t PartitionInfo[NUM_FLASH_PARTITIONS+1]
#
package provide iomfps 1.0
#
package require db
package require checkretval
# 
set iomfps(default) "default";
#
proc printIomFpsData { } {
	global iomfps;
	foreach item [lsort -dictionary [array names iomfps]] {
		puts "iomfps\($item\): $iomfps($item)";
	}
	return "0 - success";
}
#
proc initIomFpsData { } {
	set status [dbselect tuples iomfps "" "filename appcmdname"];
	if {[isNotOk $status]} {
		return "-1 - initIomFpsData: dbselect of relation 'IomFps' failed: \n${status}";
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		return "-1 - initIomFpsData: No data found in relation 'iomfps'.";
	}
	#
	global iomfps;
	unset iomfps;
	#
	foreach tuple $tuples {
		set filename [lindex $tuple 0];
		set appcmdname [lindex $tuple 1];
		#
		set iomfps($filename) $appcmdname;
	}
	#
	return "0 - success";
}

