#!/opt/exp/bin/tclsh
#
# create CM HUG scripts.
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
	puts "# sample global data used by CM HUG preprocessor.";
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
proc cmloadcmpairupdate { cfgdata cmddata globals command lowcm hicm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	# determine which is the standby CM.
	#
	if {$gld(iom${lowcm}iomstate) == "isact" && \
	    $gld(iom${hicm}iomstate) == "isstbyh"} {
		set activecm ${lowcm};
		set standbycm ${hicm};
	} elseif {$gld(iom${lowcm}iomstate) == "isstbyh" && \
	    $gld(iom${hicm}iomstate) == "isact"} {
		set activecm ${hicm};
		set standbycm ${lowcm};
	} else {
		return "-1 - cmloadcmpairupdate: CM pair (${lowcm},${hicm}) are not IS,ACT and IS,STBYH.";
	}
	#
	set cmdd(cmhug,[incr cmd]) "tl1 open -t30 cmhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rmv-eqpt::iom-${standbycm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t300 cmhugtl1 ${standbycm} oos,oosma";
	# is there a new CPU load?
	if {[info exists gld(cpuswversion)]} {
		set appswversion $gld(cpuswversion);
	} else {
		set appswversion $gld(activespswversion);
	}
	# set new CM version and CPU version.
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${standbycm}:::swversion=$gld(cmswversion),appswversion=${appswversion}";
	#
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v -t1200 cmhugtl1 cpy-mem::iom-${standbycm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${standbycm} oos,oosma";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rst-eqpt::iom-${standbycm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${standbycm} isstbyh";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 sw-toprotn-eqpt::iom-${activecm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${activecm} oos,oosma";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${activecm}:::swversion=$gld(cmswversion),appswversion=${appswversion}";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v -t1200 cmhugtl1 cpy-mem::iom-${activecm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${activecm} isstbyh";
	set cmdd(cmhug,[incr cmd]) "tl1 close -t 30 cmhugtl1";
	#
	return "0 - success";
}
#
proc cpuloadcmpairupdate { cfgdata cmddata globals command lowcm hicm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	# determine which is the standby CM.
	#
	if {$gld(iom${lowcm}iomstate) == "isact" && \
	    $gld(iom${hicm}iomstate) == "isstbyh"} {
		set activecm ${lowcm};
		set standbycm ${hicm};
	} elseif {$gld(iom${lowcm}iomstate) == "isstbyh" && \
	    $gld(iom${hicm}iomstate) == "isact"} {
		set activecm ${hicm};
		set standbycm ${lowcm};
	} else {
		return "-1 - cmloadcmpairupdate: CM pair (${lowcm},${hicm}) are not IS,ACT and IS,STBYH.";
	}
	#
	set cmdd(cmhug,[incr cmd]) "tl1 open -t30 cmhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rmv-eqpt::iom-${standbycm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t300 cmhugtl1 ${standbycm} oos,oosma";
	# is there a new CPU load?
	if {[info exists gld(cpuswversion)]} {
		set appswversion $gld(cpuswversion);
	} else {
		set appswversion $gld(activespswversion);
	}
	# set new CM version and CPU version.
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${standbycm}:::swversion=$gld(iom${standbycm}swversion),appswversion=${appswversion}";
	#
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rst-eqpt::iom-${standbycm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${standbycm} isstbyh";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 sw-toprotn-eqpt::iom-${activecm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${activecm} oos,oosma";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${activecm}:::swversion=$gld(iom${activecm}swversion),appswversion=${appswversion}";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rst-eqpt::iom-${activecm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${activecm} isact,isstbyh";
	set cmdd(cmhug,[incr cmd]) "tl1 close -t 30 cmhugtl1";
	#
	return "0 - success";
}
#
proc cmloadcmupdate { cfgdata cmddata globals command cm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	set cmdd(cmhug,[incr cmd]) "tl1 open -t30 cmhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rmv-eqpt::iom-${cm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t300 cmhugtl1 ${cm} oos,oosma";
	# is there a new CPU load?
	if {[info exists gld(cpuswversion)]} {
		set appswversion $gld(cpuswversion);
	} else {
		set appswversion $gld(activespswversion);
	}
	# set new CM version and CPU version.
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${cm}:::swversion=$gld(cmswversion),appswversion=${appswversion}";
	#
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v -t1200 cmhugtl1 cpy-mem::iom-${cm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${cm} oos,oosma";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rst-eqpt::iom-${cm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${cm} isact,isactnbk";
	set cmdd(cmhug,[incr cmd]) "tl1 close -t 30 cmhugtl1";
	#
	return "0 - success";
}
#
proc cpuloadcmupdate { cfgdata cmddata globals command cm } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	set cmdd(cmhug,[incr cmd]) "tl1 open -t30 cmhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rmv-eqpt::iom-${cm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t300 cmhugtl1 ${cm} oos,oosma";
	# is there a new CPU load?
	if {[info exists gld(cpuswversion)]} {
		set appswversion $gld(cpuswversion);
	} else {
		set appswversion $gld(activespswversion);
	}
	# set new CM version and CPU version.
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 ed-eqpt-all::iom-${cm}:::swversion=$gld(iom${cm}swversion),appswversion=${appswversion}";
	#
	set cmdd(cmhug,[incr cmd]) "tl1 exec -v cmhugtl1 rst-eqpt::iom-${cm}";
	set cmdd(cmhug,[incr cmd]) "waitforiomstate -t600 cmhugtl1 ${cm} isact,isactnbk";
	set cmdd(cmhug,[incr cmd]) "tl1 close -t 30 cmhugtl1";
	#
	return "0 - success";
}
#
proc makecmhugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	# check if the switch is the correct type of switch 
	# to have CMs and if indeed that switch has a 
	# CM.
	#
	puts "\nChassis Type: $gld(chassistype)";
	#
	switch -exact -- $gld(chassistype) {
	mgc {
		# ok
	}
	sg_mgc {
		# ok
	}
	default {
		return "-1 - makecmhugsec: chassis type [string toupper $gld(chassistype)] does not support CMs.";
	}
	}
	if {![info exists gld(cmlist)]} {
		return "-1 - makecmhugsec: switch contains NO CMs.";
	}
	#
	# check if we have a CM and/or CPU load.
	#
	if {[info exists gld(cmfilename)]} {
		set havecmload 1;
	} else {
		set havecmload 0;
	}
	if {[info exists gld(cpufilename)]} {
		set havecpuload 1;
	} else {
		set havecpuload 0;
	}
	if {!$havecpuload && !$havecmload} {
		return "-1 - makecmhugsec: No CM and CPU loads were given.";
	}
	#
	# start CM hug section
	#
	lappend cmdd(sections) "cmhug";
	#
	set cmd 0;
	#
	set cmdd(cmhug,[incr cmd]) "cmhug exec {";
	#
	# rememember to update CCMs first, then update the 
	# remaining CMs.
	#
	foreach lowcm {14 1 3 5 7 10 12} {
		# cm pairs
		set hicm [expr $lowcm + 1];
		# check if both CMs exist
		if {$gld(iom${lowcm}cleitype) == "cm" && \
		    $gld(iom${hicm}cleitype) == "cm"} {
			# both CCMs are there
			if {$havecmload} {
				set status [cmloadcmpairupdate cfgd cmdd gld cmd $lowcm $hicm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cmloadcmpairupdate failed:\n${status}";
				}
			} else {
				set status [cpuloadcmpairupdate cfgd cmdd gld cmd $lowcm $hicm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cpuloadcmpairupdate failed:\n${status}";
				}
			}
		} elseif {$gld(iom${hicm}cleitype) == "cm"} {
			# only one CCM exists.
			if {$havecmload} {
				set status [cmloadcmupdate cfgd cmdd gld cmd $hicm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cmloadcmupdate failed:\n${status}";
				}
			} else {
				set status [cpuloadcmupdate cfgd cmdd gld cmd $hicm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cpuloadcmupdate failed:\n${status}";
				}
			}
		} elseif {$gld(iom${lowcm}cleitype) == "cm"} {
			# only one CCM exists.
			if {$havecmload} {
				set status [cmloadcmupdate cfgd cmdd gld cmd $lowcm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cmloadcmupdate failed:\n${status}";
				}
			} else {
				set status [cpuloadcmupdate cfgd cmdd gld cmd $lowcm];
				if {[isNotOk $status]} {
					return "-1 - makehugsece: cpuloadcmupdate failed:\n${status}";
				}
			}
		}
	}
	#
	# close CM section off
	#
	set cmdd(cmhug,[incr cmd]) "}";
	#
	set cmdd(cmhug,first) 1;
	set cmdd(cmhug,last) $cmd;
	#
	puts "\n";
	puts "Generate CM HUG Cmds: ";
	puts "First Cmd: $cmdd(cmhug,first)";
	puts "Last Cmd: $cmdd(cmhug,last)";
	#
	return "0 - success";
}
#
proc makecmhugscript { cfgdata cmddata } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	set status [makefilesec cfgd cmdd];
	if {[isNotOk $status]} {
		return "-1 - writecmhugscript: makefilesec failed:\n${status}";
	}
	#
	set status [makeglobalssec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writecmhugscript: makeglobalssec failed:\n${status}";
	}
	#
	if {[info exists globals(cmlist)]} {
		set status [makecmhugsec cfgd cmdd globals];
		if {[isNotOk $status]} {
			return "-1 - writecmhugscript: makecmhugsec failed:\n${status}";
		}
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
	puts "\nUsing default script name 'cmhugpp.cfg'.";
	set scriptfile "cmhugpp.cfg";
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
set status [makecmhugscript cfgdata cmddata];
if {[isNotOk $status]} {
	puts "\nERROR: makecmhugscript Failed: \n$status";
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
