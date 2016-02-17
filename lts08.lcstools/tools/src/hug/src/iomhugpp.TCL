#!/opt/exp/bin/tclsh
#
# create IOM HUG scripts.
#
#########################################################################
#
lappend auto_path [file join [file dirname [info script]] ../lib]
#
# packages we need 
#
package require Expect
#
# libraries
#
package require checkenv
package require getoptval
package require checkretval
package require cfgdata
package require hughardware
package require hwmaint
package require scripts
package require ncleis
package require hugmisc
#
# globals
#
set analist [list ana];
set vsmlist [list vs2 vs3 voip voip6];
set enalist [list ena ena2];
set ds3list [list octds3 octds3_2 octds3_3 trids3 trids3_3];
set octds3list [list octds3 octds3_2 octds3_3];
set trids3list [list trids3 trids3_3];
#
set tdmocprotiom(1) 2;
set tdmocprotiom(2) 1;
set tdmocprotiom(3) 4;
set tdmocprotiom(4) 3;
set tdmocprotiom(5) 0;
set tdmocprotiom(6) 0;
set tdmocprotiom(7) 0;
set tdmocprotiom(8) 0;
set tdmocprotiom(9) 0;
set tdmocprotiom(10) 0;
set tdmocprotiom(11) 0;
set tdmocprotiom(12) 0;
set tdmocprotiom(13) 0;
set tdmocprotiom(14) 15;
set tdmocprotiom(15) 14;
set tdmocprotiom(16) 17;
set tdmocprotiom(17) 16;
#
set ds3protiom(1) 5;
set ds3protiom(2) 5;
set ds3protiom(3) 5;
set ds3protiom(4) 5;
set ds3protiom(5) 0;
set ds3protiom(6) 5;
set ds3protiom(7) 5;
set ds3protiom(8) 5;
set ds3protiom(9) 13;
set ds3protiom(10) 13;
set ds3protiom(11) 13;
set ds3protiom(12) 13;
set ds3protiom(13) 0;
set ds3protiom(14) 13;
set ds3protiom(15) 13;
set ds3protiom(16) 13;
set ds3protiom(17) 13;
#
# exit handlers
#
exit -onexit {
	if {!$printTemplate} {
		puts "\nDone.";
	}
}
#
trap {
	set signame [trap -name];
	set signo [trap -number];
	puts "\nERROR: Got signal <$signame,$signo>";
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "usage: [file tail $argv0] \[-\?] \[-V] \[-T] ";
	puts "	\[-S trace level] \[-l logfile] \[-o outfile] \[data_filename]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-l filename - log file name";
	puts "	-o filename - script output file";
	puts "	-T - print template config data file";
	puts "";
	puts "Data file contains data describing switch about to be HUG'ed."
	puts "Default data file name is 'cpuhug.cfg'. If script output file is";
	puts "not given, then the script is written to stdout.";
	puts "";
}
#
proc printTemplate { } {
	puts "#";
	puts "# sample global data used by IOM HUG preprocessor.";
	puts "#";
	puts "globals data {";
	puts "        scriptsdir .";
	puts "        #";
	puts "        labid ihlcsg21";
	puts "        branch BP-6-2-1-2";
	puts "        ftpmachine lts08.ih.lucent.com";
	puts "        ftpuser lcsinteg";
	puts "        ftppasswd plexus9000";
	puts "        tl1user telica";
	puts "        tl1passwd telica";
	puts "        lynxuser root";
	puts "        lynxpasswd plexus9000";
	puts "        images {";
	puts "                iomtypes ana,cm,cpu,ena,ena2,octds3,octds3_2,octds3_3,tdmoc,trids3,trids3_3,vs2,vs3";
	puts "                ana NAME 6.2.1.2_ana.tar.gz";
	puts "                cm NAME 6.2.1.2.P2.1_cm.tar.gz";
	puts "                cpu NAME 6.2.1.2.SP.6.7_cpu.tar.gz";
	puts "                ena NAME 6.2.1.2.P1_ena.tar.gz";
	puts "                ena2 NAME 6.2.1.2_ena2.tar.gz";
	puts "                octds3 NAME 6.2.1.2.P3_octds3.tar.gz";
	puts "                octds3_2 NAME 6.2.1.2.P3_octds3_2.tar.gz";
	puts "                octds3_3 NAME 6.2.1.2.P3_octds3_3.tar.gz";
	puts "                tdmoc PATH /l148/text/BP-6-2-1-2/tdmoc/6.2.1.2.P3_tdmoc.tar.gz";
	puts "                trids3 PATH /l148/text/BP-6-2-1-2/trids3/6.2.1.2.P3_trids3.tar.gz";
	puts "                trids3_3 PATH /l148/text/BP-6-2-1-2/trids3_3/6.2.1.2.P3_trids3_3.tar.gz";
	puts "                vs2 NAME 6.2.1.2.P4_vs2.tar.gz";
	puts "                vs3 NAME 6.2.1.2.P4_vs3.tar.gz";
	puts "        }";
	puts "}";
	return 0;
}
#
proc inhsiglinks { iom } {
	# inhibit signaling links, if any.
	return "0 - success";
}
#
proc makeds1_2hugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	puts "\nGenerate ds1_2 IOM HUG Cmds: ";
	#
	# check if any DS1_2
	#
	if {![info exists gld(ds1_2list)]} {
		puts "Switch contains NO ds1_2 IOMs.";
		return "0 - success";
	} else {
		puts "Switch contains ds1_2 IOMs.";
	}
	#
	# check if are updating DS1_2?
	#
	if {[info exists gld(ds1_2swversion)]} {
		puts "DS1_2 IOMs are being updated: $gld(ds1_2swversion)";
	} else {
		puts "DS1_2 IOMs are NOT being updated. Returning.";
		return "0 - success";
	}
	# 
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby DS1_2
	#
	set seciomcnt 0;
	#
	foreach seciom $gld(ds1_2list) {
		# check if standby
		if {$gld(iom${seciom}protection) != "sec"} {
			# not a protection IOM
			continue;
		} else {
			# count how many secondaries were found
			incr seciomcnt;
		}
		# update standby IOMs
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(ds1_2swversion)";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc";
	}
	#
	# update active DS1_2
	#
	foreach priiom $gld(ds1_2list) {
		# check if standby
		if {$gld(iom${priiom}protection) != "pri"} {
			# not a protection IOM
			continue;
		}
		# inhibit or OOS any signaling links on this IOM.
		set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
		set cmdd(iomhug,[incr cmd]) "tl1 rmvsiglnks iomhugtl1 ${priiom}";
		set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
		#
		# update primary IOMs
		if {$seciomcnt > 0} {
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(ds1_2swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact";
		} else {
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(ds1_2swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
		}
		#
		# restore any signaling links on this IOM.
		set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
		set cmdd(iomhug,[incr cmd]) "tl1 rstsiglnks iomhugtl1 ${priiom}";
		set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc makeds3hugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	global ds3list;
	global ds3protiom;
	#
	puts "\nGenerate octds3* and trids3* IOM HUG Cmds: ";
	#
	# check if any DS3 exist
	#
	set ds3typesfound "";
	#
	foreach ds3type ${ds3list} {
		if {[info exists gld(${ds3type}list)]} {
			puts "Switch contains ${ds3type} IOMs.";
			lappend ds3typesfound ${ds3type};
		} else {
			puts "Switch contains NO ${ds3type} IOMs.";
		}
	}
	if {[llength ${ds3typesfound}] <= 0} {
		puts "Switch contains NO octds3* and trids3* IOMs.";
		return "0 - success";
	} else {
		puts "Switch contains these ds3 IOM types: ${ds3typesfound}";
	}
	#
	# check if are updating octds3* or trids3*?
	#
	set ds3typesupdate "";
	#
	foreach ds3type ${ds3typesfound} {
		if {[info exists gld(${ds3type}swversion)]} {
			puts "${ds3type} IOMs are being updated: $gld(${ds3type}swversion)";
			lappend ds3typesupdate ${ds3type};
		} else {
			puts "${ds3type} IOMs are NOT being updated.";
		}
	}
	# 
	if {[llength ${ds3typesupdate}] <= 0} {
		puts "No updates for octds3* and trids3* IOMs.";
		return "0 - success";
	} else {
		puts "Updates for these ds3 IOM types: ${ds3typesupdate}";
	}
	#
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby ds3
	#
	set seciomcnt 0;
	#
	foreach ds3type ${ds3typesupdate} {
		foreach seciom $gld(${ds3type}list) {
			# check if standby
			if {$gld(iom${seciom}protection) != "sec"} {
				# not a protection IOM
				continue;
			} else {
				# count how many secondaries were found
				incr seciomcnt;
				set protioms(${seciom}) 1;
			}
			# update standby IOMs
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(${ds3type}swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc";
		}
	}
	#
	# update active ds3
	#
	foreach ds3type ${ds3typesupdate} {
		foreach priiom $gld(${ds3type}list) {
			# check if standby
			if {$gld(iom${priiom}protection) != "pri"} {
				# not a protection IOM
				continue;
			}
			# inhibit or OOS any signaling links on this IOM.
			set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
			set cmdd(iomhug,[incr cmd]) "tl1 rmvsiglnks iomhugtl1 ${priiom}";
			set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
			# update primary IOMs
			if {[info exists protioms($ds3protiom(${priiom}))]} {
				# protection IOM exists for this IOM.
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${ds3type}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact";
			} else {
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${ds3type}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
			}
			# restore any signaling links on this IOM.
			set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
			set cmdd(iomhug,[incr cmd]) "tl1 rstsiglnks iomhugtl1 ${priiom}";
			set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
		}
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc makeenahugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	global enalist;
	#
	puts "\nGenerate ena* IOM HUG Cmds: ";
	#
	# check if any ENA exist
	#
	set enatypesfound "";
	#
	foreach enatype ${enalist} {
		if {[info exists gld(${enatype}list)]} {
			puts "Switch contains ${enatype} IOMs.";
			lappend enatypesfound ${enatype};
		} else {
			puts "Switch contains NO ${enatype} IOMs.";
		}
	}
	if {[llength ${enatypesfound}] <= 0} {
		puts "Switch contains NO ena* IOMs.";
		return "0 - success";
	} else {
		puts "Switch contains these ena IOM types: ${enatypesfound}";
	}
	#
	# check if are updating ena*
	#
	set enatypesupdate "";
	#
	foreach enatype ${enatypesfound} {
		if {[info exists gld(${enatype}swversion)]} {
			puts "${enatype} IOMs are being updated: $gld(${enatype}swversion)";
			lappend enatypesupdate ${enatype};
		} else {
			puts "${enatype} IOMs are NOT being updated.";
		}
	}
	# 
	if {[llength ${enatypesupdate}] <= 0} {
		puts "No updates for ena* IOMs.";
		return "0 - success";
	} else {
		puts "Updates for these ena IOM types: ${enatypesupdate}";
	}
	#
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby ena
	#
	set seciomcnt 0;
	#
	foreach enatype ${enatypesupdate} {
		foreach seciom $gld(${enatype}list) {
			# check if standby
			if {$gld(iom${seciom}protection) != "sec"} {
				# not a protection IOM
				continue;
			} else {
				# count how many secondaries were found
				incr seciomcnt;
			}
			# update standby IOMs
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(${enatype}swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc,isact";
		}
	}
	#
	# update active ena
	#
	foreach enatype ${enatypesupdate} {
		foreach priiom $gld(${enatype}list) {
			# check if standby
			if {$gld(iom${priiom}protection) != "pri"} {
				# not a protection IOM
				continue;
			}
			# update primary IOMs
			if {$seciomcnt > 0} {
				# protection IOM exists for this IOM.
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${enatype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isstbyc,isstbyh";
			} else {
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${enatype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
			}
		}
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc makevsmhugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	global vsmlist;
	#
	puts "\nGenerate vsm* IOM HUG Cmds: ";
	#
	# check if any VSM exist
	#
	set vsmtypesfound "";
	#
	foreach vsmtype ${vsmlist} {
		if {[info exists gld(${vsmtype}list)]} {
			puts "Switch contains ${vsmtype} IOMs.";
			lappend vsmtypesfound ${vsmtype};
		} else {
			puts "Switch contains NO ${vsmtype} IOMs.";
		}
	}
	if {[llength ${vsmtypesfound}] <= 0} {
		puts "Switch contains NO vsm* IOMs.";
		return "0 - success";
	} else {
		puts "Switch contains these vsm IOM types: ${vsmtypesfound}";
	}
	#
	# check if are updating vsm*
	#
	set vsmtypesupdate "";
	#
	foreach vsmtype ${vsmtypesfound} {
		if {[info exists gld(${vsmtype}swversion)]} {
			puts "${vsmtype} IOMs are being updated: $gld(${vsmtype}swversion)";
			lappend vsmtypesupdate ${vsmtype};
		} else {
			puts "${vsmtype} IOMs are NOT being updated.";
		}
	}
	# 
	if {[llength ${vsmtypesupdate}] <= 0} {
		puts "No updates for vsm* IOMs.";
		return "0 - success";
	} else {
		puts "Updates for these vsm IOM types: ${vsmtypesupdate}";
	}
	#
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby vsm
	#
	set seciomcnt 0;
	#
	foreach vsmtype ${vsmtypesupdate} {
		foreach seciom $gld(${vsmtype}list) {
			# check if standby
			if {$gld(iom${seciom}protection) != "sec"} {
				# not a protection IOM
				continue;
			} else {
				# count how many secondaries were found
				incr seciomcnt;
			}
			# update standby IOMs
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(${vsmtype}swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc";
		}
	}
	#
	# update active vsm
	#
	foreach vsmtype ${vsmtypesupdate} {
		foreach priiom $gld(${vsmtype}list) {
			# check if standby
			if {$gld(iom${priiom}protection) != "pri"} {
				# not a protection IOM
				continue;
			}
			# update primary IOMs
			if {$seciomcnt > 0} {
				# protection IOM exists for this IOM.
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${vsmtype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact";
			} else {
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${vsmtype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
			}
		}
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc maketdmochugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	global tdmoclist;
	global tdmocprotiom;
	#
	puts "\nGenerate tdmoc IOM HUG Cmds: ";
	#
	# check if any TDMOC exist
	#
	if {[info exists gld(tdmoclist)]} {
		puts "Switch contains tdmoc IOMs.";
	} else {
		puts "Switch contains NO tdmoc IOMs.";
		return "0 - success";
	}
	#
	if {[info exists gld(tdmocswversion)]} {
		puts "tdmoc IOMs are being updated: $gld(tdmocswversion)";
	} else {
		puts "No updates for tdmoc IOMs.";
		return "0 - success";
	}
	#
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby tdmoc
	#
	set seciomcnt 0;
	#
	foreach seciom $gld(tdmoclist) {
		# check if standby
		if {$gld(iom${seciom}protection) != "sec"} {
			# not a protection IOM
			continue;
		} else {
			# count how many secondaries were found
			incr seciomcnt;
			set protioms(${seciom}) 1;
		}
		# update standby IOMs
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(tdmocswversion)";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
		set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
		set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc";
	}
	#
	# update active tdmoc
	#
	foreach priiom $gld(tdmoclist) {
		# check if standby
		if {$gld(iom${priiom}protection) != "pri"} {
			# not a protection IOM
			continue;
		}
		# update primary IOMs
		if {[info exists protioms($tdmocprotiom(${priiom}))]} {
			# protection IOM exists for this IOM.
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(tdmocswversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact";
		} else {
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(tdmocswversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
		}
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc makeanahugsec { cfgdata cmddata globals command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	global analist;
	#
	puts "\nGenerate ana* IOM HUG Cmds: ";
	#
	# check if any ANA exist
	#
	set anatypesfound "";
	#
	foreach anatype ${analist} {
		if {[info exists gld(${anatype}list)]} {
			puts "Switch contains ${anatype} IOMs.";
			lappend anatypesfound ${anatype};
		} else {
			puts "Switch contains NO ${anatype} IOMs.";
		}
	}
	if {[llength ${anatypesfound}] <= 0} {
		puts "Switch contains NO ana* IOMs.";
		return "0 - success";
	} else {
		puts "Switch contains these ana IOM types: ${anatypesfound}";
	}
	#
	# check if are updating ana*
	#
	set anatypesupdate "";
	#
	foreach anatype ${anatypesfound} {
		if {[info exists gld(${anatype}swversion)]} {
			puts "${anatype} IOMs are being updated: $gld(${anatype}swversion)";
			lappend anatypesupdate ${anatype};
		} else {
			puts "${anatype} IOMs are NOT being updated.";
		}
	}
	# 
	if {[llength ${anatypesupdate}] <= 0} {
		puts "No updates for ana* IOMs.";
		return "0 - success";
	} else {
		puts "Updates for these ana IOM types: ${anatypesupdate}";
	}
	#
	# open TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 open -t30 iomhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-eqpt";
	set cmdd(iomhug,[incr cmd]) "tl1 exec -i -v iomhugtl1 rtrv-info-eqpt";
	#
	# update standby ana
	#
	set seciomcnt 0;
	#
	foreach anatype ${anatypesupdate} {
		foreach seciom $gld(${anatype}list) {
			# check if standby
			if {$gld(iom${seciom}protection) != "sec"} {
				# not a protection IOM
				continue;
			} else {
				# count how many secondaries were found
				incr seciomcnt;
			}
			# update standby IOMs
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${seciom}:::swversion=$gld(${anatype}swversion)";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} oos,oosma";
			set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${seciom}";
			set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${seciom} isstbyc,isact";
		}
	}
	#
	# update active ana
	#
	foreach anatype ${anatypesupdate} {
		foreach priiom $gld(${anatype}list) {
			# check if standby
			if {$gld(iom${priiom}protection) != "pri"} {
				# not a protection IOM
				continue;
			}
			# update primary IOMs
			if {$seciomcnt > 0} {
				# protection IOM exists for this IOM.
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 sw-toprotn-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${anatype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isstbyc,isstbyh";
			} else {
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rmv-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t300 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 ed-eqpt::iom-${priiom}:::swversion=$gld(${anatype}swversion)";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v -t1200 iomhugtl1 cpy-mem::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} oos,oosma";
				set cmdd(iomhug,[incr cmd]) "tl1 exec -v iomhugtl1 rst-eqpt::iom-${priiom}";
				set cmdd(iomhug,[incr cmd]) "waitforiomstate -t600 iomhugtl1 ${priiom} isact,isactnbk";
			}
		}
	}
	#
	# close TL1 stream
	#
	set cmdd(iomhug,[incr cmd]) "tl1 close -t 30 iomhugtl1";
	#
	return "0 - success";
}
#
proc makeiomhugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	# start IOM hug section
	#
	lappend cmdd(sections) "iomhug";
	#
	set cmd 0;
	#
	set cmdd(iomhug,[incr cmd]) "iomhug exec {";
	#
	# write script according to IOM type.
	#
	# DS1_2 IOMS
	set status [makeds1_2hugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: makeds1_2hugsec failed:\n${status}";
	}
	# TRIDS3 and OCTDS3 IOMS
	set status [makeds3hugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: maketrids3hugsec failed:\n${status}";
	}
	# ENA IOMs
	set status [makeenahugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: makeenahugsec failed:\n${status}";
	}
	# ANA IOMs
	set status [makeanahugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: makeanahugsec failed:\n${status}";
	}
	# VOIP, VS2, VS3 IOMs
	set status [makevsmhugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: makevsmhugsec failed:\n${status}";
	}
	# TDMOC IOMs
	set status [maketdmochugsec cfgd cmdd gld cmd];
	if {[isNotOk $status]} {
		return "-1 - makeiomhugsec: maketdmochugsec failed:\n${status}";
	}
	#
	# close IOM section off
	#
	set cmdd(iomhug,[incr cmd]) "}";
	#
	set cmdd(iomhug,first) 1;
	set cmdd(iomhug,last) $cmd;
	#
	puts "\n";
	puts "Generate IOM HUG Cmds: ";
	puts "First Cmd: $cmdd(iomhug,first)";
	puts "Last Cmd: $cmdd(iomhug,last)";
	#
	return "0 - success";
}
#
proc makeiomhugscript { cfgdata cmddata } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	set status [makefilesec cfgd cmdd];
	if {[isNotOk $status]} {
		return "-1 - writeiomhugscript: makefilesec failed:\n${status}";
	}
	#
	set status [makeglobalssec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writeiomhugscript: makeglobalssec failed:\n${status}";
	}
	#
	set status [makeiomhugsec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writeiomhugscript: makeiomhugsec failed:\n${status}";
	}
	#
	return "0 - success";
}
#
#########################################################################
#
# default values
#
global env;
#
set stracelevel -1;
set logfile "";
set startok 0;
set outfile "";
#
log_user 0;
#
# get cmd line options
#
set argc [llength $argv];
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { log_user 1; }
	{^-T} { printTemplate; exit 0; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-l.*} { getoptval $argval logfile arg; }
	{^-o.*} { getoptval $argval outfile arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nERROR: unknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
# check environment
checkenv;
#
set status [initCleiData];
if {[isNotOk $status]} {
	puts "\nERROR: initCleiData Failed: \n$status";
	exit 2;
}
#
global argv0;
global argv;
catch { system "lcssyslog -q syslog \"Starting $argv0 $argv.\""; } ignore;
#
# check if function trace is requested.
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
#
# check if logging is requested.
#
if {[string length $logfile] > 0} {
	if {[catch {log_file -noappend -a $logfile; } status]} {
		puts "\nERROR: logging failed for file $logfile:\n$status";
		exit 2;
	}
	catch { system /usr/bin/chmod 777 $logfile; } ignore;
}
#
# read in script and execute.
#
if {$arg<$argc} {
	set scriptfile [lindex $argv $arg];
} else {
	puts "\nUsing default script name 'iomhugpp.cfg'.";
	set scriptfile "iomhugpp.cfg";
}
#
puts "\nReading Data File: $scriptfile";
#
set status [initCfgData cfgdata];
if {[isNotOk $status]} {
	puts "\nERROR: initCfgData Failed: \n$status";
	exit 2;
}
#
set status [initCmdData cmddata];
if {[isNotOk $status]} {
	puts "\nERROR: initCmdData Failed: \n$status";
	exit 2;
}
#
set status [readCfgDataFile cfgdata cmddata $scriptfile];
if {[isNotOk $status]} {
	puts "\nERROR: readCfgDataFile Failed: \n$status";
	exit 2;
}
#
printCfgData cfgdata;
#
# determine switch state
#
puts "\nDetermining Switch State:";
set status [hug_GetSwitchState cfgdata];
if {[isNotOk $status]} {
	puts "\nERROR: hug_GetSwitchState Failed: \n$status";
	exit 2;
}
printCfgData cfgdata;
#
puts "\nConverting IOM Cleis to IOM types:";
set status [convertCleis cfgdata]
if {[isNotOk $status]} {
	puts "\nERROR: convertCleis Failed: \n$status";
	exit 2;
}
printCfgData cfgdata;
#
set status [makeiomhugscript cfgdata cmddata];
if {[isNotOk $status]} {
	puts "\nERROR: makeiomhugscript Failed: \n$status";
	exit 2;
}
#
set status [printHugScript cfgdata cmddata $outfile];
if {[isNotOk $status]} {
	puts "\nERROR: printHugScript Failed: \n$status";
	exit 2;
}
#
exit 0;
