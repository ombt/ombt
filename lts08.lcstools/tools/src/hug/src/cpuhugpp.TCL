#!/opt/exp/bin/tclsh
#
# create CPU HUG scripts.
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
	puts "# sample global data used by CPU HUG preprocessor.";
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
proc makecpuhugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	set chassistype $gld(chassistype);
	#
	lappend cmdd(sections) "cpuhug";
	#
	set cmd 0;
	#
	set cmdd(cpuhug,[incr cmd]) "cpuhug exec {";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	#
	# get data from current active SP
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-chassis-eqpt";
	#
	# turn off backup and protection switches
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-bkupsched-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 sched-bkup-eqpt:::::::1-day,0-1,0-0";
	#
	foreach iom $gld(iomlist) {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-exsched-eqpt::iom-${iom}";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 inh-ex-eqpt::iom-${iom}";
	}
	#
	switch -exact -- ${chassistype} {
	mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-call-captverbose";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 dlt-call-captverbose::1";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::oos";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	sg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-sg-mgcassoc";
	}
	mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-status-mg";
	}
	sg_mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-status-mg";
	}
	sg_mgc_mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-call-captverbose";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 dlt-call-captverbose::1";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::oos";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	sg_mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-call-captverbose";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 dlt-call-captverbose::1";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::oos";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	default {
		return "-1 - makecpuhugsec: unknown chassis type - $gld(chassistype)";
	}
	}
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1800 cpuhugtl1 exec-backup-plexus::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),/tmp";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 ed-eqpt::$gld(standbysp):::swversion=$gld(cpuswversion)";
	if {[needcpymem $gld(standbyspswversion)]} {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 cpy-mem::$gld(standbysp)";
		set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
		set cmdd(cpuhug,[incr cmd]) "sleep <postcpucpymemsleep>";
		set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	}
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rmv-eqpt::$gld(standbysp)";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isactnbk,oosmahalt,oosma $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isactnbk,oosmahalt,oosma $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	#
	set cmdd(cpuhug,[incr cmd]) "sleep <preupgradedelay>";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1200 cpuhugtl1 exec-run-upgrade:$gld(switchid):$gld(standbysp)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rst-eqpt::$gld(standbysp)";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-info-eqpt";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 ed-eqpt::$gld(activesp):::swversion=$gld(cpuswversion)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	#
	if {[needcpymem $gld(activespswversion)]} {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 cpy-mem::$gld(activesp)";
		set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
		set cmdd(cpuhug,[incr cmd]) "sleep <postcpucpymemsleep>";
		set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	}
	#
	# switch to protection SP.
	#
	if {[neednorestore $gld(activespswversion)]} {
		set cmdd(cpuhug,[incr cmd]) "sleep <precpuswtoprotndelay>";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 sw-toprotn-eqpt::$gld(activesp):::frcd&norst";
		set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
		if {$gld(activesp) == "sp-a"} {
			set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) ignore $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isactnbk $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		} elseif {$gld(activesp) == "sp-b"} {
			set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isactnbk $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) ignore $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		} else {
			return "-1 - makecpuhugsec: unknown active SP - $gld(activesp)";
		}
	} else {
		set cmdd(cpuhug,[incr cmd]) "sleep <precpuswtoprotndelay>";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 sw-toprotn-eqpt::$gld(activesp)";
		set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
		if {$gld(activesp) == "sp-a"} {
			set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) ignore $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isactnbk $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		} elseif {$gld(activesp) == "sp-b"} {
			set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isactnbk $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) ignore $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
		} else {
			return "-1 - makecpuhugsec: unknown active SP - $gld(activesp)";
		}
	}
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(standbyspip) $gld(tl1login) $gld(tl1passwd)";
	#
	# this is strange. the DLP says you have to remove the new standby SP
	# after the switch, but the rmv-eqpt fails because the SP is already
	# in the OOS-MA,SDWN state. So, issue the command and ignore the 
	# status. the second waitforcpustate will resynch with the correct
	# state, i hope. if not ...
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rmv-eqpt::$gld(activesp)";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v -t60 cpuhugtl1 rtrv-alm-all";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	#
	switch -exact -- ${chassistype} {
	mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	sg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-sg-mgcassoc";
	}
	mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-status-mg";
	}
	sg_mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-status-mg";
	}
	sg_mgc_mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	sg_mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-pm-cc";
	}
	default {
		return "-1 - makecpuhugsec: unknown chassis type - $gld(chassistype)";
	}
	}
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isactnbk,oosmahalt,oosma,oosmasdwn $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isactnbk,oosmahalt,oosma,oosmasdwn $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "sleep <preupgradedelay>";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(standbyspip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1200 cpuhugtl1 exec-run-upgrade:$gld(switchid):$gld(activesp)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v cpuhugtl1 rst-eqpt::$gld(activesp)";
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	set cmdd(cpuhug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "tl1 open -t30 cpuhugtl1 $gld(standbyspip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-eqpt";
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-info-eqpt";
	switch -exact -- ${chassistype} {
	mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::is";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
	}
	sg {
		# nothing to do
	}
	mg {
		# nothing to do
	}
	sg_mg {
		# nothing to do
	}
	sg_mgc_mg {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::is";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
	}
	sg_mgc {
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 ed-trafficstats:::::is";
		set cmdd(cpuhug,[incr cmd]) "tl1 exec -i -v cpuhugtl1 rtrv-trafficstats";
	}
	default {
		return "-1 - makecpuhugsec: unknown chassis type - $gld(chassistype)";
	}
	}
	set cmdd(cpuhug,[incr cmd]) "tl1 exec -v -t1800 cpuhugtl1 exec-backup-plexus::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),/tmp";
	#
	set cmdd(cpuhug,[incr cmd]) "tl1 close -t 30 cpuhugtl1";
	#
	set cmdd(cpuhug,[incr cmd]) "}";
	#
	set cmdd(cpuhug,first) 1;
	set cmdd(cpuhug,last) $cmd;
	#
	puts "\n";
	puts "Generate CPU HUG Cmds: ";
	puts "First Cmd: $cmdd(cpuhug,first)";
	puts "Last Cmd: $cmdd(cpuhug,last)";
	#
	return "0 - success";
}
#
proc makecpuhugscript { cfgdata cmddata } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	set status [makefilesec cfgd cmdd];
	if {[isNotOk $status]} {
		return "-1 - writecpuhugscript: makefilesec failed:\n${status}";
	}
	#
	set status [makeglobalssec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writecpuhugscript: makeglobalssec failed:\n${status}";
	}
	#
	set status [makecpuhugsec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writecpuhugscript: makecpuhugsec failed:\n${status}";
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
	puts "\nUsing default script name 'cpuhugpp.cfg'.";
	set scriptfile "cpuhugpp.cfg";
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
set status [makecpuhugscript cfgdata cmddata];
if {[isNotOk $status]} {
	puts "\nERROR: makecpuhugscript Failed: \n$status";
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
