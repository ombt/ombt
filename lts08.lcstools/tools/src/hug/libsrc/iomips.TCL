package provide iomips 1.0
#
package require db
package require checkretval
# 
set iomips(default,default) "default";
#
# iom-1
# set iomips(1,1) "192.168.252.1"; 	#iom-1-cpu-a iom-1
# set iomips(1,2) "192.168.252.33"; 	#iom-1-cpu-b
# set iomips(1,3) "192.168.252.65"; 	#iom-1-cpu-c
# set iomips(1,4) "192.168.252.97"; 	#iom-1-cpu-d
# iom-2
# set iomips(2,1) "192.168.252.2"; 	#iom-2-cpu-a iom-2
# set iomips(2,2) "192.168.252.34"; 	#iom-2-cpu-b
# set iomips(2,3) "192.168.252.66"; 	#iom-2-cpu-c
# set iomips(2,4) "192.168.252.98"; 	#iom-2-cpu-d
# iom-3
# set iomips(3,1) "192.168.252.3"; 	#iom-3-cpu-a iom-3
# set iomips(3,2) "192.168.252.35"; 	#iom-3-cpu-b
# set iomips(3,3) "192.168.252.67"; 	#iom-3-cpu-c
# set iomips(3,4) "192.168.252.99"; 	#iom-3-cpu-d
# iom-4
# set iomips(4,1) "192.168.252.4"; 	#iom-4-cpu-a iom-4
# set iomips(4,2) "192.168.252.36"; 	#iom-4-cpu-b
# set iomips(4,3) "192.168.252.68"; 	#iom-4-cpu-c
# set iomips(4,4) "192.168.252.100"; 	#iom-4-cpu-d
# iom-5
# set iomips(5,1) "192.168.252.5"; 	#iom-5-cpu-a iom-5
# set iomips(5,2) "192.168.252.37"; 	#iom-5-cpu-b
# set iomips(5,3) "192.168.252.69"; 	#iom-5-cpu-c
# set iomips(5,4) "192.168.252.101"; 	#iom-5-cpu-d
# iom-6
# set iomips(6,1) "192.168.252.6"; 	#iom-6-cpu-a iom-6
# set iomips(6,2) "192.168.252.38"; 	#iom-6-cpu-b
# set iomips(6,3) "192.168.252.70"; 	#iom-6-cpu-c
# set iomips(6,4) "192.168.252.102"; 	#iom-6-cpu-d
# iom-7
# set iomips(7,1) "192.168.252.7"; 	#iom-7-cpu-a iom-7
# set iomips(7,2) "192.168.252.39"; 	#iom-7-cpu-b
# set iomips(7,3) "192.168.252.71"; 	#iom-7-cpu-c
# set iomips(7,4) "192.168.252.103"; 	#iom-7-cpu-d
# iom-8
# set iomips(8,1) "192.168.252.8"; 	#iom-8-cpu-a iom-8
# set iomips(8,2) "192.168.252.40"; 	#iom-8-cpu-b
# set iomips(8,3) "192.168.252.72"; 	#iom-8-cpu-c
# set iomips(8,4) "192.168.252.104"; 	#iom-8-cpu-d
# iom-9, CM not permitted
# set iomips(9,1) "192.168.252.9"; 	#iom-9-cpu-a iom-9
# set iomips(9,2) "0.0.0.0";
# set iomips(9,3) "0.0.0.0";
# set iomips(9,4) "0.0.0.0";
# iom-10
# set iomips(10,1) "192.168.252.10"; 	#iom-10-cpu-a iom-10
# set iomips(10,2) "192.168.252.42"; 	#iom-10-cpu-b
# set iomips(10,3) "192.168.252.74"; 	#iom-10-cpu-c
# set iomips(10,4) "192.168.252.106"; 	#iom-10-cpu-d
# iom-11
# set iomips(11,1) "192.168.252.11"; 	#iom-11-cpu-a iom-11
# set iomips(11,2) "192.168.252.43"; 	#iom-11-cpu-b
# set iomips(11,3) "192.168.252.75"; 	#iom-11-cpu-c
# set iomips(11,4) "192.168.252.107"; 	#iom-11-cpu-d
# iom-12
# set iomips(12,1) "192.168.252.12"; 	#iom-12-cpu-a iom-12
# set iomips(12,2) "192.168.252.44"; 	#iom-12-cpu-b
# set iomips(12,3) "192.168.252.76"; 	#iom-12-cpu-c
# set iomips(12,4) "192.168.252.108"; 	#iom-12-cpu-d
# iom-13
# set iomips(13,1) "192.168.252.13"; 	#iom-13-cpu-a iom-13
# set iomips(13,2) "192.168.252.45"; 	#iom-13-cpu-b
# set iomips(13,3) "192.168.252.77"; 	#iom-13-cpu-c
# set iomips(13,4) "192.168.252.109"; 	#iom-13-cpu-d
# iom-14
# set iomips(14,1) "192.168.252.14"; 	#iom-14-cpu-a iom-14
# set iomips(14,2) "192.168.252.46"; 	#iom-14-cpu-b
# set iomips(14,3) "192.168.252.78"; 	#iom-14-cpu-c
# set iomips(14,4) "192.168.252.110"; 	#iom-14-cpu-d
# iom-15
# set iomips(15,1) "192.168.252.15"; 	#iom-15-cpu-a iom-15
# set iomips(15,2) "192.168.252.47"; 	#iom-15-cpu-b
# set iomips(15,3) "192.168.252.79"; 	#iom-15-cpu-c
# set iomips(15,4) "192.168.252.111"; 	#iom-15-cpu-d
# iom-16
# set iomips(16,1) "192.168.252.16"; 	#iom-16-cpu-a iom-16
# set iomips(16,2) "192.168.252.48"; 	#iom-16-cpu-b
# set iomips(16,3) "192.168.252.80"; 	#iom-16-cpu-c
# set iomips(16,4) "192.168.252.112"; 	#iom-16-cpu-d
# iom-17
# set iomips(17,1) "192.168.252.17"; 	#iom-17-cpu-a iom-17
# set iomips(17,2) "192.168.252.49"; 	#iom-17-cpu-b
# set iomips(17,3) "192.168.252.81"; 	#iom-17-cpu-c
# set iomips(17,4) "192.168.252.113"; 	#iom-17-cpu-d
#
proc printIomIpsData { } {
	global iomips;
	foreach item [lsort -dictionary [array names iomips]] {
		puts "iomips\($item\): $iomips($item)";
	}
	return "0 - success";
}
#
proc initIomIpsData { } {
	set status [dbselect tuples iomips "" "iom cpu ip"];
	if {[isNotOk $status]} {
		return "-1 - initIomIpsData: dbselect of relation 'iomips' failed: \n${status}";
	}
	if {![info exists tuples] || [llength $tuples] == 0} {
		return "-1 - initIomIpsData: No data found in relation 'iomips'.";
	}
	#
	global iomips;
	unset iomips;
	#
	foreach tuple $tuples {
		set iom [lindex $tuple 0];
		set cpu [lindex $tuple 1];
		set ip [lindex $tuple 2];
		#
		set iomips($iom,$cpu) $ip;
	}
	#
	return "0 - success";
}

