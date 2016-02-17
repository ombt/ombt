# miscellaneous routines
package provide hugmisc 1.0
#
package require checkretval
#
proc getiompath { branch type name fullpath } {
	upvar $fullpath fpath;
	#
	set status [dbselect fsobuf filesystems "branch req ^$branch\$ and type req text" "path"];
	if {[isNotOk $status]} {
		return "-1 - getiompath: dbselect of relation 'filesystems' failed: \n${status}";
	}
	if {![info exists fsobuf] || [llength $fsobuf] == 0} {
		return "-1 - getiompath: no filesystem found for branch $branch.";
	}
	set filesystem [lindex $fsobuf 0];
	set fpath ${filesystem}/${branch}/${type}/${name};
	#
	if {[file readable ${fpath}]} {
		return "0 - success";
	} else {
		return "-1 - getiompath: IOM file ${fpath} is not readable";
	}
}
#
proc getswversion { fullpath softwareversion } {
	upvar $softwareversion swversion;
	#
	if {![file readable ${fullpath}]} {
		return "-1 - getswversion: IOM file ${fullpath} is not readable";
	}
	#
	spawn gnutar tzvf $fullpath;
	expect {
	-re {Telica/sw[^/]*/([^/]*)/} {
		set swversion $expect_out(1,string);
		hw_maint_close $spawn_id;
	}
	timeout {
		hw_maint_close $spawn_id;
		return "-1 - getswversion: gnutar timeout for ${fullpath}.";
	}
	eof {
		hw_maint_close $spawn_id;
		return "-1 - getswversion: gnutar eof for ${fullpath}.";
	}
	}
	#
	return "0 - success";
}
#
proc removesoftlinks { path nolinkspathname } {
	upvar $nolinkspathname nolinkspath;
	#
	if {[catch { set nolinkspath [file readlink $path]; } errmsg] != 0} {
		set nolinkspath ${path};
	}
	#
	return "0 - success";
}
#
proc makeglobalssec { cfgdata cmddata globals { maxiom 17 } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	lappend cmdd(sections) "globals";
	#
	set cmd 0;
	#
	set switchid $cfgd(globals,switchid);
	set gld(switchid) $cfgd(globals,switchid);
	#
	if {[info exists cfgd(globals,labid)]} {
		set labid $cfgd(globals,labid);
		set gld(labid) $cfgd(globals,labid);
	} else {
		set labid ${switchid};
		set gld(labid) ${switchid};
	}
	#
	set branch $cfgd(globals,load,branch);
	set gld(branch) $cfgd(globals,load,branch);
	if {[info exists cfgd(globals,email)]} {
		set gld(email) $cfgd(globals,email);
	}
	#
	# switch data
	#
	set gld(chassistype) [string tolower $cfgd(globals,$switchid,chassistype)];
	#
	set spaip $cfgd(globals,$switchid,spa,ip);
	set spastate $cfgd($spaip,spstate);
	set spacurrrel $cfgd(globals,$switchid,spa,currrel);
	set gld(spaip) $cfgd(globals,$switchid,spa,ip);
	set gld(spastate) $cfgd($spaip,spstate);
	set gld(spacurrrel) $cfgd(globals,$switchid,spa,currrel);
	set gld(spaswversion) [file tail $gld(spacurrrel)];
	#
	set spbip $cfgd(globals,$switchid,spb,ip);
	set spbstate $cfgd($spbip,spstate);
	set spbcurrrel $cfgd(globals,$switchid,spb,currrel);
	set gld(spbip) $cfgd(globals,$switchid,spb,ip);
	set gld(spbstate) $cfgd($spbip,spstate);
	set gld(spbcurrrel) $cfgd(globals,$switchid,spb,currrel);
	set gld(spbswversion) [file tail $gld(spbcurrrel)];
	#
	if {$spastate == "isact"} {
		set gld(activesp) "sp-a";
		set gld(activespname) "spa";
		set gld(activespip) $spaip;
		set gld(activespstate) $spastate;
		set gld(activespcurrrel) $spacurrrel;
		set gld(activespswversion) [file tail $spacurrrel];
		#
		set gld(standbysp) "sp-b";
		set gld(standbyspname) "spb";
		set gld(standbyspip) $spbip;
		set gld(standbyspstate) $spbstate;
		set gld(standbyspcurrrel) $spbcurrrel;
		set gld(standbyspswversion) [file tail $spbcurrrel];
	} elseif {$spbstate == "isact"} {
		set gld(activesp) "sp-b";
		set gld(activespname) "spb";
		set gld(activespip) $spbip;
		set gld(activespstate) $spbstate;
		set gld(activespcurrrel) $spbcurrrel;
		set gld(activespswversion) [file tail $spbcurrrel];
		#
		set gld(standbysp) "sp-a";
		set gld(standbyspname) "spa";
		set gld(standbyspip) $spaip;
		set gld(standbyspstate) $spastate;
		set gld(standbyspcurrrel) $spacurrrel;
		set gld(standbyspswversion) [file tail $spacurrrel];
	} else {
		return "-1 - makeglobalssec: neither SP-A ($spastate) or SP-B ($spbstate) is is,act.";
	}
	#
	set gld(activespcurrreldir) [file dirname $gld(activespcurrrel)];
	if {[regexp -- {^/Telica/swCPU.*$} $gld(activespcurrreldir)] == 1} {
		set gld(activespbasedir) "/";
		set gld(activespbackupcurrreldir) "/home1/Telica/swCPU";
		set gld(activespbackupbasedir) "/home1";
	} elseif {[regexp -- {^/home1/Telica/swCPU.*$} $gld(activespcurrreldir)] == 1} {
		set gld(activespbasedir) "/home1";
		set gld(activespbackupcurrreldir) "/Telica/swCPU";
		set gld(activespbackupbasedir) "/";
	} else {
		return "-1 - makeglobalssec: unknown Active SP CurrRel directory - $gld(activespcurrreldir).";
	}
	set gld(standbyspcurrreldir) [file dirname $gld(standbyspcurrrel)];
	if {[regexp -- {^/Telica/swCPU.*$} $gld(standbyspcurrreldir)] == 1} {
		set gld(standbyspbasedir) "/";
		set gld(standbyspbackupcurrreldir) "/home1/Telica/swCPU";
		set gld(standbyspbackupbasedir) "/home1";
	} elseif {[regexp -- {^/home1/Telica/swCPU.*$} $gld(standbyspcurrreldir)] == 1} {
		set gld(standbyspbasedir) "/home1";
		set gld(standbyspbackupcurrreldir) "/Telica/swCPU";
		set gld(standbyspbackupbasedir) "/";
	} else {
		return "-1 - makeglobalssec: unknown Standby SP CurrRel directory - $gld(standbyspcurrreldir).";
	}
	#
	set gld(tl1login) $cfgd(globals,$switchid,tl1login);
	set gld(tl1passwd) $cfgd(globals,$switchid,tl1passwd);
	set gld(login) $cfgd(globals,$switchid,login);
	set gld(passwd) $cfgd(globals,$switchid,passwd);
	#
	set gld(ftpmachine) $cfgd(globals,load,ftpmachine);
	set gld(ftplogin) $cfgd(globals,load,ftplogin);
	set gld(ftppasswd) $cfgd(globals,load,ftppasswd);
	#
	regsub -all -- "," $cfgd(globals,load,images,iomtypes) " " gld(iomtypes);
	foreach iomtype $gld(iomtypes) {
		set data [split $cfgd(globals,load,images,${iomtype}) " "];
		set datatype [lindex $data 0];
		#
		switch -regexp -- $datatype {
		{^NAME$} {
			set name [lindex $data 1];
			puts "\nIOM TYPE: ${iomtype}, NAME: ${name}";
			set status [getiompath ${branch} ${iomtype} ${name} fullpath]; 
			if {[isNotOk $status]} {
				return "-1 - makeglobalssec: getiompath failed:\n${status}";
			}
			set status [getswversion $fullpath swversion]; 
			if {[isNotOk $status]} {
				return "-1 - makeglobalssec: getswversion failed:\n${status}";
			}
			puts "IOM TYPE: ${iomtype}, PATH: ${fullpath}";
			puts "IOM TYPE: ${iomtype}, SWVERSION: ${swversion}";
			set gld(${iomtype}swversion) ${swversion};
			removesoftlinks $fullpath nolinksfullpath;
			puts "IOM TYPE: ${iomtype}, NO LINKS PATH: ${nolinksfullpath}";
			set gld(${iomtype}fullpath) ${nolinksfullpath};
			set gld(${iomtype}filename) [file tail ${fullpath}];
		}
		{^PATH$} {
			set fullpath [lindex $data 1];
			set status [getswversion $fullpath swversion]; 
			if {[isNotOk $status]} {
				return "-1 - makeglobalssec: getswversion failed:\n${status}";
			}
			puts "\nIOM TYPE: ${iomtype}, PATH: ${fullpath}";
			puts "IOM TYPE: ${iomtype}, SWVERSION: ${swversion}";
			set gld(${iomtype}swversion) ${swversion};
			removesoftlinks $fullpath nolinksfullpath;
			puts "IOM TYPE: ${iomtype}, NO LINKS PATH: ${nolinksfullpath}";
			set gld(${iomtype}fullpath) ${nolinksfullpath};
			set gld(${iomtype}filename) [file tail ${fullpath}];
		}
		default {
			return "-1 - makeglobalssec: unknown file type for ${iomtype}: $cfgd(globals,load,images,${iomtype})";
		}
		}
	}
	#
	puts "\n";
	puts "Summary of HUG Switch Data:"
	puts "\n";
	puts "Switch ID: $gld(switchid)";
	puts "\n";
	puts "Chassis Type: $gld(chassistype)";
	puts "\n";
	puts "Active SP: $gld(activesp)";
	puts "Active SP Name: $gld(activespname)";
	puts "Active SP IP: $gld(activespip)";
	puts "Active SP State: $gld(activespstate)";
	puts "Active SP SW Version: $gld(activespswversion)";
	puts "Active SP CurrRel: $gld(activespcurrrel)";
	puts "Active SP CurrRel Directory: $gld(activespcurrreldir)";
	puts "Active SP Backup CurrRel Directory: $gld(activespbackupcurrreldir)";
	puts "Active SP Base Directory: $gld(activespbasedir)";
	puts "Active SP Backup Base Directory: $gld(activespbackupbasedir)";
	puts "\n";
	puts "Standsby SP: $gld(standbysp)";
	puts "Standsby SP Name: $gld(standbyspname)";
	puts "Standsby SP IP: $gld(standbyspip)";
	puts "Standsby SP State: $gld(standbyspstate)";
	puts "Standby SP SW Version: $gld(standbyspswversion)";
	puts "Standby SP CurrRel: $gld(standbyspcurrrel)";
	puts "Standby SP CurrRel Directory: $gld(standbyspcurrreldir)";
	puts "Standby SP Backup CurrRel Directory: $gld(standbyspbackupcurrreldir)";
	puts "Standby SP Base Directory: $gld(standbyspbasedir)";
	puts "Standby SP Backup Base Directory: $gld(standbyspbackupbasedir)";
	puts "\n";
	puts "TL1 Login: $gld(tl1login)";
	puts "TL1 Passwd: $gld(tl1passwd)";
	puts "\n";
	puts "Lynx-OS Login: $gld(login)";
	puts "Lynx-OS Passwd: $gld(passwd)";
	puts "\n";
	puts "FTP Machine: $gld(ftpmachine)";
	puts "FTP Login: $gld(ftplogin)";
	puts "FTP Passwd: $gld(ftppasswd)";
	puts "\n";
	puts "New Load Branch: $gld(branch)";
	puts "\n";
	#
	foreach iomtype $gld(iomtypes) {
		puts "IOM ${iomtype} Tar File Path: $gld(${iomtype}fullpath)";
		puts "IOM ${iomtype} Tar File: $gld(${iomtype}filename)";
	}
	#
	set gld(iomlist) " ";
	set gld(iomtypelist) " ";
	#
	for {set iom 1} {$iom<=$maxiom} {incr iom} {
		if {![info exists cfgd(iom,${iom},clei)] || \
		     [string length $cfgd(iom,${iom},clei)] <= 0} {
			set gld(iom${iom}clei) "NONE";
			set gld(iom${iom}rearclei) "none";
			set gld(iom${iom}cleitype) "NONE";
			set gld(iom${iom}iomstate) $cfgd(iom,${iom},iomstate);
			set gld(iom${iom}protection) "NONE";
			set gld(iom${iom}swversion) "NONE";
		} else {
			set cleitype $cfgd(iom,${iom},cleitype);
			#
			set gld(iom${iom}clei) $cfgd(iom,${iom},clei);
			set gld(iom${iom}rearclei) $cfgd(iom,${iom},rearclei);
			set gld(iom${iom}cleitype) $cfgd(iom,${iom},cleitype);
			set gld(iom${iom}iomstate) $cfgd(iom,${iom},iomstate);
			set gld(iom${iom}protection) $cfgd(iom,${iom},protection);
			set gld(iom${iom}swversion) $cfgd(iom,${iom},version);
			set gld(iomlist) "$gld(iomlist) ${iom}";
			#
			if {![info exists known($cleitype)]} {
				set gld(iomtypelist) "$gld(iomtypelist) ${cleitype}";
				set known($cleitype) 1;
				set gld(${cleitype}list) "${iom}";
			} else {
				set gld(${cleitype}list) "$gld(${cleitype}list) ${iom}";
			}
		}
	}
	#
	set gld(iomlist) [string trim $gld(iomlist)];
	set gld(iomtypelist) [string trim $gld(iomtypelist)];
	#
	if {[string length $gld(iomlist)] <= 0} {
		set gld(iomlist) "NONE";
		set gld(iomtypelist) "NONE";
	}
	#
	set cmdd(globals,[incr cmd]) "globals data {";
	#
	# write special globals
	#
	set cmdd(globals,[incr cmd]) "interunixcmddelay 30";
	set cmdd(globals,[incr cmd]) "preupgradedelay 300";
	set cmdd(globals,[incr cmd]) "precpuswtoprotndelay 120";
	set cmdd(globals,[incr cmd]) "postcpucpymemsleep 30";
	#
	# write all other globals
	#
	foreach name [lsort [array names gld]] {
		set cmdd(globals,[incr cmd]) "${name} $gld(${name})";
	}
	#
	set cmdd(globals,[incr cmd]) "}";
	#
	set cmdd(globals,first) 1;
	set cmdd(globals,last) $cmd;
	#
	puts "\n";
	puts "Generate Globals Header: ";
	puts "First Cmd: $cmdd(globals,first)";
	puts "Last Cmd: $cmdd(globals,last)";
	#
	return "0 - success";
}
#
proc needcpymem { swversion } {
	switch -regexp -- "${swversion}" {
	{^6\.1\.0\.3\.SP[5-9]$} { return 1; }
	{^6\.1\.0\.3\.SP[1-9][1-9].*} { return 1; }
	{^6\.1\.0\.[4-9].*} { return 1; }
	{^6\.1\.[1-9].*} { return 1; }
	{^6\.[2-9].*} { return 1; }
	default { return 0; }
	}
}
#
proc neednorestore { swversion } {
	switch -regexp -- "${swversion}" {
	{^6\.1\.0\.3\.SP[6-9]$} { return 1; }
	{^6\.1\.0\.3\.SP[1-9][1-9].*} { return 1; }
	{^6\.1\.0\.[4-9].*} { return 1; }
	{^6\.1\.[1-9].*} { return 1; }
	{^6\.[2-9].*} { return 1; }
	default { return 0; }
	}
}
#
proc makefilesec { cfgdata cmddata } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	lappend cmdd(sections) "file";
	#
	set cmd 0;
	#
	set cmdd(file,[incr cmd]) "file script {";
	set cmdd(file,[incr cmd]) "\tscriptsdir .";
	set cmdd(file,[incr cmd]) "}";
	#
	set cmdd(file,first) 1;
	set cmdd(file,last) $cmd;
	#
	puts "\n";
	puts "Generate File Header: ";
	puts "First Cmd: $cmdd(file,first)";
	puts "Last Cmd: $cmdd(file,last)";
	#
	return "0 - success";
}
#
