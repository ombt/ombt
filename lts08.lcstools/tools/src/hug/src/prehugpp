#!/opt/exp/bin/tclsh
#
# create pre-HUG scripts.
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

	puts "usage: [file tail $argv0] \[-\?] \[-V] \[-T] \[-L|-D]";
	puts "	\[-S trace level] \[-l logfile] \[-o outfile] \[data_filename]";
	puts "";
	puts "where:";
	puts "	-? - usage message";
	puts "	-V - enable verbose output";
	puts "	-S tracelevel - set trace to given stack level";
	puts "	-l filename - log file name";
	puts "	-o filename - script output file";
	puts "	-T - print template config data file";
	puts "	-L - pre-HUG script only contains TL1-base FTP/TAR commands";
	puts "	-D - pre-HUG script only contains FTP/TAR commands";
	puts "";
	puts "Data file contains data describing switch about to be HUG'ed."
	puts "Default data file name is 'prehug.cfg'. If script output file is";
	puts "not given, then the script is written to stdout.";
	puts "";
}
#
proc printTemplate { } {
	puts "#";
	puts "# sample global data used by pre-HUG preprocessor.";
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
proc ftputils { cfgdata cmddata globals secname command } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	upvar $command cmd;
	#
	set cmdd(${secname},[incr cmd]) "telnet open -t30 telnetact $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(${secname},[incr cmd]) "telnet open -t30 telnetstby $gld(standbyspip) $gld(login) $gld(passwd)";
	set cmdd(${secname},[incr cmd]) "ftp open ftpact $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(${secname},[incr cmd]) "ftp open ftpstby $gld(standbyspip) $gld(login) $gld(passwd)";
	#
	set cmdd(${secname},[incr cmd]) "ftp put -t300 -b ftpact /home/lcstools/tools/src/hug/scripts/disablediskcleanup /home/disablediskcleanup";
	set cmdd(${secname},[incr cmd]) "ftp put -t300 -b ftpstby /home/lcstools/tools/src/hug/scripts/disablediskcleanup /home/disablediskcleanup";
	#
	set cmdd(${secname},[incr cmd]) "telnet exec -v telnetact chmod 777 /home/disablediskcleanup";
	set cmdd(${secname},[incr cmd]) "telnet exec -v telnetstby chmod 777 /home/disablediskcleanup";
	#
	set cmdd(${secname},[incr cmd]) "ftp close ftpact";
	set cmdd(${secname},[incr cmd]) "ftp close ftpstby";
	set cmdd(${secname},[incr cmd]) "telnet close telnetact";
	set cmdd(${secname},[incr cmd]) "telnet close telnetstby";
	#
	return " 0 - success";
}
#
proc makedirectprehugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	lappend cmdd(sections) "prehug";
	#
	set cmd 0;
	#
	set cmdd(prehug,[incr cmd]) "prehug exec {";
	#
	# send over any utilities
	#
	ftputils cfgd cmdd gld prehug cmd;
	#
	# open telnet and ftp streams
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "ftp open prehugftpactivesp $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "ftp open prehugftpstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
	#
	# remove old files 
	#
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp mv /*.tar.gz /tmp";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /tmp/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /home1/*.tar.gz";
	# set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -rf \"$gld(activespbackupbasedir)/Telica/sw*/*\[0-9]*"\";
	#
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp mv /*.tar.gz /tmp";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /tmp/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /home1/*.tar.gz";
	# set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -rf \"$gld(standbyspbackupbasedir)/Telica/sw*/*\[0-9]*\"";
	#
	# check if we have a cpu tar file to ship and expand
	#
	if {[info exists gld(cpufullpath)]} {
		# we have a CPU tar file
		puts "\nWriting CPU TAR file commands.";
		#
		set cmdd(prehug,[incr cmd]) "ftp put -t1800 -b prehugftpactivesp $gld(cpufullpath) $gld(activespbackupbasedir)/$gld(cpufilename)";
		set cmdd(prehug,[incr cmd]) "ftp put -t1800 -b prehugftpstandbysp $gld(cpufullpath) $gld(standbyspbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp cd $gld(activespbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp cd $gld(standbyspbackupbasedir)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -t1800 -v prehugtelnetactivesp /bin/tar xzvf $gld(activespbackupbasedir)/$gld(cpufilename)";
		set cmdd(prehug,[incr cmd]) "telnet exec -t1800 -v prehugtelnetstandbysp /bin/tar xzvf $gld(standbyspbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -t300 -v prehugtelnetactivesp /home/disablediskcleanup";
		set cmdd(prehug,[incr cmd]) "telnet exec -t300 -v prehugtelnetstandbysp /home/disablediskcleanup";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp rm -f $gld(activespbackupbasedir)/$gld(cpufilename)";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp rm -f $gld(standbyspbackupbasedir)/$gld(cpufilename)";
	} else {
		puts "\nThere is NO CPU TAR file to ship.";
	}
	#
	# FTP and expand any IOM tar files.
	#
	foreach iomtype $gld(iomtypes) {
		if {$iomtype == "cpu"} {
			# CPU was already shipped.
			continue;
		}
		if {![info exists gld(${iomtype}fullpath)]} {
			puts "\nWARNING: No TAR file found for IOM type ${iomtype}";
			continue;
		}
		#
		set cmdd(prehug,[incr cmd]) "ftp put -t1800 -b prehugftpactivesp $gld(${iomtype}fullpath) /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "ftp put -t1800 -b prehugftpstandbysp $gld(${iomtype}fullpath) /$gld(${iomtype}filename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp cd /";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp cd /";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -t1800 -v prehugtelnetactivesp /bin/tar xzvf /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "telnet exec -t1800 -v prehugtelnetstandbysp /bin/tar xzvf /$gld(${iomtype}filename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp rm -f /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp rm -f /$gld(${iomtype}filename)";
	}
	#
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	set cmdd(prehug,[incr cmd]) "ftp close prehugftpactivesp";
	set cmdd(prehug,[incr cmd]) "ftp close prehugftpstandbysp";
	#
	set cmdd(prehug,[incr cmd]) "}";
	#
	set cmdd(prehug,first) 1;
	set cmdd(prehug,last) $cmd;
	#
	puts "\n";
	puts "Generate PreHUG Cmds: ";
	puts "First Cmd: $cmdd(prehug,first)";
	puts "Last Cmd: $cmdd(prehug,last)";
	#
	return "0 - success";
}
#
proc makeliteprehugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	lappend cmdd(sections) "prehug";
	#
	set cmd 0;
	#
	set cmdd(prehug,[incr cmd]) "prehug exec {";
	#
	# send over any utilities
	#
	ftputils cfgd cmdd gld prehug cmd;
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp mv /*.tar.gz /tmp";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /tmp/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f /home1/*.tar.gz";
	# set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -rf \"$gld(activespbackupbasedir)/Telica/sw*/*\[0-9]*\"";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
	#
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp mv /*.tar.gz /tmp";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /tmp/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f /home1/*.tar.gz";
	# set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -rf \"$gld(standbyspbackupbasedir)/Telica/sw*/*\[0-9]*\"";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	#
	if {[info exists gld(cpufullpath)]} {
		# we have a CPU tar file
		puts "\nWriting CPU TAR file commands.";
		#
		set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),$gld(cpufullpath),$gld(activespbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,$gld(activespbackupbasedir)/$gld(cpufilename),$gld(standbyspbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cd:::::$gld(activespbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::$gld(cpufilename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::$gld(standbyspbackupbasedir)/$gld(cpufilename),$gld(standbyspbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
		#
		# remove tar files once we are done expanding.
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f $gld(activespbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp /home/disablediskcleanup";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f $gld(standbyspbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp /home/disablediskcleanup";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	} else {
		puts "\nThere is NO CPU TAR file to ship.";
	}
	#
	# FTP and expand any IOM tar files.
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
	#
	foreach iomtype $gld(iomtypes) {
		if {$iomtype == "cpu"} {
			# CPU was already shipped.
			continue;
		}
		if {![info exists gld(${iomtype}fullpath)]} {
			puts "\nWARNING: No TAR file found for IOM type ${iomtype}";
			continue;
		}
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),$gld(${iomtype}fullpath),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,/$gld(${iomtype}filename),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::/$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp rm -f /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::$gld(${iomtype}filename),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp rm -f /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	}
	#
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	#
	set cmdd(prehug,[incr cmd]) "}";
	#
	set cmdd(prehug,first) 1;
	set cmdd(prehug,last) $cmd;
	#
	puts "\n";
	puts "Generate PreHUG Cmds: ";
	puts "First Cmd: $cmdd(prehug,first)";
	puts "Last Cmd: $cmdd(prehug,last)";
	#
	return "0 - success";
}
#
proc makeprehugsec { cfgdata cmddata globals } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $globals gld;
	#
	lappend cmdd(sections) "prehug";
	#
	set cmd 0;
	#
	set cmdd(prehug,[incr cmd]) "prehug exec {";
	#
	# send over any utilities
	#
	ftputils cfgd cmdd gld prehug cmd;
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	#
	# get data from current active SP
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-eqpt";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-chassis-eqpt";
	#
	# determine where CurrRel points.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	#
	# list and clean up backup file system.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,$gld(activespbackupcurrreldir)";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";

	# set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-r,\"$gld(activespbackupcurrreldir)/*\[0-9]*\"";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	#
	# remove any old tar files.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home1/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	#
	# remove old CPU loads, not including the current load, though.
	# compare the swCPU loads with what is currently active, and if they 
	# don't match, remove them.
	#
	set switchid $gld(switchid);
	set currel $gld(activespcurrrel);
	set spname $gld(activespname);
	#
	foreach fs $cfgd(globals,$switchid,$spname,filesystems) {
		if {![info exists cfgd(globals,$switchid,$spname,softwarefs,$fs)]} {
			puts "\nWARNING: cfgd(globals,$switchid,$spname,softwarefs,$fs) does NOT exist. Skipping it.";
			continue;
		}
		foreach cpudir $cfgd(globals,$switchid,$spname,softwarefs,$fs) {
			if {[regexp -- {^.*/swCPU/.*$} $cpudir] == 0 || \
			    [regexp -- {^.*/swCPU/core.*$} $cpudir] == 1 || \
			    [regexp -- {^.*/swCPU/CurrRel.*$} $cpudir] == 1} {
				continue;
			} elseif {"$currel" != "$cpudir"} {
				set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-r,$cpudir";
				set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
			}
		}
	}
	#
	# START SWITCHING SPS; THE STANDBY WILL BECOME ACTIVE AND THE PROCEDURE
	# IS REPEATED.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -v prehugtl1 rmv-eqpt::$gld(standbysp)";
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	#
	set cmdd(prehug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isactnbk,oosmahalt,oosma $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isactnbk,oosmahalt,oosma $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -v prehugtl1 rst-eqpt::$gld(standbysp)";
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	#
	set cmdd(prehug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(activespip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(prehug,[incr cmd]) "tl1 exec -v prehugtl1 sw-toprotn-eqpt::$gld(activesp)";
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	#
	set cmdd(prehug,[incr cmd]) "waitforcpustate -t1800 $gld(spaip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd) $gld(spbip) isact,isstbyh $gld(login) $gld(passwd) $gld(tl1login) $gld(tl1passwd)";
	#
	# get data from new active SP
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 $gld(standbyspip) $gld(tl1login) $gld(tl1passwd)";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-eqpt";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 rtrv-chassis-eqpt";
	#
	# determine where CurrRel points.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,/Telica/swCPU/CurrRel";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	#
	# list and clean up backup file system.
	#
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-ls:::::-l,$gld(standbyspbackupcurrreldir)";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";

	# set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-r,\"$gld(standbyspbackupcurrreldir)/*\[0-9]*\"";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home1/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/home/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-f,/*.tar.gz";
	set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
	#
	# remove old CPU loads, not including the current load, though.
	# compare the swCPU loads with what is currently active, and if they 
	# don't match, remove them.
	#
	set switchid $gld(switchid);
	set currel $gld(standbyspcurrrel);
	set spname $gld(standbyspname);
	#
	foreach fs $cfgd(globals,$switchid,$spname,filesystems) {
		if {![info exists cfgd(globals,$switchid,$spname,softwarefs,$fs)]} {
			puts "\nWARNING: cfgd(globals,$switchid,$spname,softwarefs,$fs) does NOT exist. Skipping it.";
			continue;
		}
		foreach cpudir $cfgd(globals,$switchid,$spname,softwarefs,$fs) {
			if {[regexp -- {^.*/swCPU/.*$} $cpudir] == 0 || \
			    [regexp -- {^.*/swCPU/core.*$} $cpudir] == 1 || \
			    [regexp -- {^.*/swCPU/CurrRel.*$} $cpudir] == 1} {
				continue;
			} elseif {"$currel" != "$cpudir"} {
				set cmdd(prehug,[incr cmd]) "tl1 exec -i -v prehugtl1 exec-rm:::::-r,$cpudir";
				set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
			}
		}
	}
	#
	# start FTP'ing and expanding the tar files. 
	#
	# remember, because of the# sw-toprtn-eqpt command, the original 
	# standby SP is now active.
	#
	if {[info exists gld(cpufullpath)]} {
		# we have a CPU tar file
		puts "\nWriting CPU TAR file commands.";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),$gld(cpufullpath),$gld(standbyspbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,$gld(standbyspbackupbasedir)/$gld(cpufilename),$gld(activespbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cd:::::$gld(standbyspbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::$gld(cpufilename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::$gld(activespbackupbasedir)/$gld(cpufilename),$gld(activespbackupbasedir)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
		#
		# remove tar files once we are done expanding.
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp rm -f $gld(activespbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetactivesp /home/disablediskcleanup";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
		#
		set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp rm -f $gld(standbyspbackupbasedir)/$gld(cpufilename)";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -i -v prehugtelnetstandbysp /home/disablediskcleanup";
		set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	} else {
		puts "\nThere is NO CPU TAR file to ship.";
	}
	#
	# FTP and expand any IOM tar files.
	#
	set cmdd(prehug,[incr cmd]) "tl1 open -t30 prehugtl1 <standbyspip> <tl1login> <tl1passwd>";
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetactivesp $gld(activespip) $gld(login) $gld(passwd)";
	set cmdd(prehug,[incr cmd]) "telnet open -t30 prehugtelnetstandbysp $gld(standbyspip) $gld(login) $gld(passwd)";
	#
	foreach iomtype $gld(iomtypes) {
		if {$iomtype == "cpu"} {
			# CPU was already shipped.
			continue;
		}
		if {![info exists gld(${iomtype}fullpath)]} {
			puts "\nWARNING: No TAR file found for IOM type ${iomtype}";
			continue;
		}
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-ftp-get::com:::$gld(ftpmachine),$gld(ftplogin),$gld(ftppasswd),$gld(${iomtype}fullpath),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-cp-tostby:::::-rp,/$gld(${iomtype}filename),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extract:::::/$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetstandbysp rm -f /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
		set cmdd(prehug,[incr cmd]) "tl1 exec -t900 -v prehugtl1 exec-tar-extractstby:::::$gld(${iomtype}filename),/";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		set cmdd(prehug,[incr cmd]) "telnet exec -v prehugtelnetactivesp rm -f /$gld(${iomtype}filename)";
		set cmdd(prehug,[incr cmd]) "sleep <interunixcmddelay>";
		#
	}
	#
	set cmdd(prehug,[incr cmd]) "tl1 close -t 30 prehugtl1";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetactivesp";
	set cmdd(prehug,[incr cmd]) "telnet close prehugtelnetstandbysp";
	#
	set cmdd(prehug,[incr cmd]) "}";
	#
	set cmdd(prehug,first) 1;
	set cmdd(prehug,last) $cmd;
	#
	puts "\n";
	puts "Generate PreHUG Cmds: ";
	puts "First Cmd: $cmdd(prehug,first)";
	puts "Last Cmd: $cmdd(prehug,last)";
	#
	return "0 - success";
}
#
proc makeprehugscript { cfgdata cmddata { scripttype "dlpfull" } } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	set status [makefilesec cfgd cmdd];
	if {[isNotOk $status]} {
		return "-1 - writeprehugscript: makefilesec failed:\n${status}";
	}
	#
	set status [makeglobalssec cfgd cmdd globals];
	if {[isNotOk $status]} {
		return "-1 - writeprehugscript: makeglobalssec failed:\n${status}";
	}
	#
	if {$scripttype eq "dlplite"} {
		set status [makeliteprehugsec cfgd cmdd globals];
		if {[isNotOk $status]} {
			return "-1 - writeprehugscript: makeliteprehugsec failed:\n${status}";
		}
	} elseif {$scripttype eq "direct"} {
		set status [makedirectprehugsec cfgd cmdd globals];
		if {[isNotOk $status]} {
			return "-1 - writeprehugscript: makedirectprehugsec failed:\n${status}";
		}
	} else {
		set status [makeprehugsec cfgd cmdd globals];
		if {[isNotOk $status]} {
			return "-1 - writeprehugscript: makeprehugsec failed:\n${status}";
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
set scripttype "dlpfull";
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
	{^-L} { set scripttype "dlplite"; }
	{^-D} { set scripttype "direct"; }
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
	puts "\nUsing default script name 'prehugpp.cfg'.";
	set scriptfile "prehugpp.cfg";
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
set status [makeprehugscript cfgdata cmddata $scripttype];
if {[isNotOk $status]} {
	puts "\nERROR: makeprehugscript Failed: \n$status";
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


