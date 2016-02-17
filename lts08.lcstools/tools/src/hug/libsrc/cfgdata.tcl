# maintain configuration data
package provide cfgdata 1.0
#
proc deleteLastIndex { indices { separator "," } } {
	upvar $indices idx;
	if {[string length $idx] <= 0} {
		# nothing to do.
		return;
	}
	#
	set lidx [split $idx $separator];
	set lidxlen [llength $lidx];
	#
	if {$lidxlen == 1} {
		set idx "";
	} else {
		set lidxlen [expr $lidxlen-1];
		set lidx [lreplace $lidx $lidxlen $lidxlen];
		set idx [join $lidx $separator];
	}
}
#
proc insertLastIndex { indices { newindex "" } { separator "," } } {
	upvar $indices idx;
	if {[string length $idx] > 0} {
		append idx $separator $newindex;
	} else {
		set idx $newindex;
	}
}
#
proc initCmdData { cmddata } {
	upvar $cmddata cmdd;
	#
	# remove old data.
	#
	if {[info exists cmdd]} {
		foreach item [array names cmdd] {
			unset cmdd($item);
		}
		unset cmdd;
	}
	#
	# store default data
	#
	set cmdd(maxcmd) 0;
	#
	return "0 - success";
}
#
proc initCfgData { cfgdata } {
	upvar $cfgdata cfgd;
	#
	# remove old data.
	#
	if {[info exists cfgd]} {
		foreach item [array names cfgd] {
			unset cfgd($item);
		}
		unset cfgd;
	}
	#
	# store default data
	#
	set cfgd(internal,type) internal;
	#
	set cfgd(internal,numberofsecs,exec) 0;
	set cfgd(internal,numberofsecs,data) 0;
	set cfgd(internal,numberofsecs,script) 0;
	set cfgd(internal,numberofsecs,internal) 1;
	set cfgd(internal,numberofsecs,unknown) 0;
	#
	set cfgd(internal,namesofsecs,exec) "";
	set cfgd(internal,namesofsecs,data) "";
	set cfgd(internal,namesofsecs,script) "";
	set cfgd(internal,namesofsecs,internal) "internal";
	set cfgd(internal,namesofsecs,unknown) "";
	#
	set cfgd(internal,sectype,internal) internal;
	#
	return "0 - success";
}
#
proc filesub { cfgdata filename substring } {
	upvar $cfgdata cfgd;
	upvar $filename fname;
	#
	if {[string length $substring] <= 0} {
		return "-1 - filesub: null length substring.";
	}
	#
	set fname $substring;
	#
	set sanitycounter 0;
	while {[regexp -- {<([_a-zA-Z0-9\.]+)>} $fname ignore var] == 1} {
		incr sanitycounter;
		if {$sanitycounter>1000} {
			return "-1 - filesub: over 1000 iterations.";
		}
		#
		set cidx "file,$var";
		regsub -all -- "\\\." $cidx "," newcidx;
		set cidx $newcidx;
		#
		if {![info exists cfgd($cidx)]} {
			return "-1 - filesub: undefined variable $cidx";
		}
		regsub -all -- "<$var>" $fname $cfgd($cidx) newfname;
		set fname $newfname;
	}
	#
	return "0 - success";
}
#
proc updateinternal { cfgdata blockname blocktype } {
	upvar $cfgdata cfgd;
	#
	if {[info exists cfgd(internal,numberofsecs,$blocktype)]} {
		incr cfgd(internal,numberofsecs,$blocktype);
		set cfgd(internal,$blocktype,$cfgd(internal,numberofsecs,$blocktype)) $blockname;
	} else {
		incr cfgd(internal,numberofsecs,unknown);
		set cfgd(internal,unknown,$cfgd(internal,numberofsecs,unknown)) $blockname;
	}
	if {[info exists cfgd(internal,namesofsecs,$blocktype)]} {
		append cfgd(internal,namesofsecs,$blocktype) " " $blockname;
	} else {
		append cfgd(internal,namesofsecs,unknown) " " $blockname;
	}
	if {[info exists cfgd(internal,sectype,$blockname)]} {
		return "-1 - updateinternal: duplicate block name $blockname";
	} else {
		set cfgd(internal,sectype,$blockname) $blocktype;
	}
	
	return "0 - success";
}
#
proc readAndParseCfgDataFile { cfgdata cmddata cfgdidx block blocknames cfgfile  {vflag 0} } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	upvar $cfgdidx cidx;
	upvar $block blk;
	upvar $blocknames blknms;
	#
	# basic sanity checks
	#
	if {[string length $cfgfile] <= 0} {
		return "-1 - readAndParseCfgDataFile: null length cfg filename.";
	} elseif {![file readable $cfgfile]} {
		return "-1 - readAndParseCfgDataFile: cfg file $cfgfile is not readable";
	}
	#
	# open file for read.
	#
	if {[catch {set fd [open $cfgfile "r"]} status]} {
		return "-1 - readAndParseCfgDataFile: unable to open $cfgfile for read: $status";
	}
	#
	# read in file and populate data structure
	#
	for {set lnno 1} {[gets $fd inbuf] >= 0 && ![eof $fd]} {incr lnno} {
		# puts "File $cfgfile, Lnno $lnno: $inbuf";
		#
		# skip comments and white-space
		#
		switch -regexp -- ${inbuf} {
		{^[ \t]*$} {
			# skip empty lines
			continue;
		}
		{^[ \t]*#} {
			# skip comments
			continue;
		}
		}
		#
		# strip leading white space
		#
		set cmd [string trim $inbuf]
		#
		# parse command line.
		#
		if {[regexp -- {^([a-zA-Z_0-9]+)[ \t]+([a-zA-Z_0-9]+)[ \t]+\{ *([a-zA-Z0-9_ ]+ *)\}[ \t]+\{[ \t]*$} $cmd mstr blockname blocktype arguments ] == 1} {
			# start of a new block, increase block level
			# and store name.
			incr blk;
			set blknms($blk) $blockname;
			# update cfgdata index to include new block name.
			insertLastIndex cidx $blockname;
			# we need to know the type of section.
			if {$blk != 1} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: block level ($blk) != 1, block type is not allowed at lnno $lnno in $cfgfile";
			}
			set status [updateinternal cfgd $blockname $blocktype];
			if {[isNotOk $status]} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: updateinternal failed at lnno $lnno in $cfgfile: \n$status";
			}
			set cfgd($cidx,maxcmd) 0;
			set cfgd($cidx,type) $blocktype;
			# if arguments exists, then we have a section
			# that can be called with arguments.
			if {[string length "$arguments"] > 0} {
				set arguments [string trim $arguments];
				set arguments [string trimright $arguments];
			}
			set cfgd($cidx,arguments) "$arguments";
			set cfgd($cidx,isafunction) 1;
		} elseif {[regexp -- {^([a-zA-Z_0-9]+)[ \t]+([a-zA-Z_0-9]+)[ \t]+\{[ \t]*$} $cmd mstr blockname blocktype ] == 1} {
			# start of a new block, increase block level
			# and store name.
			incr blk;
			set blknms($blk) $blockname;
			# update cfgdata index to include new block name.
			insertLastIndex cidx $blockname;
			# we need to know the type of section.
			if {$blk != 1} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: block level ($blk) != 1, block type is not allowed at lnno $lnno in $cfgfile";
			}
			set status [updateinternal cfgd $blockname $blocktype];
			if {[isNotOk $status]} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: updateinternal failed at lnno $lnno in $cfgfile: \n$status";
			}
			set cfgd($cidx,maxcmd) 0;
			set cfgd($cidx,type) $blocktype;
			# no arguments exists. this section can NOT be
			# called with arguments. 
			if {[info exists cfgd($cidx,arguments)]} {
				unset cfgd($cidx,arguments);
			}
			set cfgd($cidx,isafunction) 0;
		} elseif {[regexp -- {^([a-zA-Z_0-9]+)[ \t]+\{[ \t]*$} $cmd mstr blockname] == 1} {
			# start of a new block, increase block level
			# and store name.
			incr blk;
			set blknms($blk) $blockname;
			# update cfgdata index to include new block name.
			insertLastIndex cidx $blockname;
			# for level 1, type is required.
			if {$blk == 1} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: block level ($blk) == 1, block type is required at lnno $lnno in $cfgfile";
			} elseif {$cfgd($blknms(1),type) == "exec"} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: nested blocks not allowed for exec block at lnno $lnno in $cfgfile";
			}
		} elseif {[regexp -- {^\}[ \t]*$} $cmd mstr] == 1} {
			incr blk -1;
			if {$blk < 0} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: block level < 0 at lnno $lnno in $cfgfile";
			}
			deleteLastIndex cidx;
		} elseif {[regexp -- {^source[ \t]+(.*)$} $cmd mstr submstr] == 1} {
			# read in another file
			set status [filesub cfgd newfilename $submstr];
			if {[isNotOk $status]} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: filesub failed at lnno $lnno in $cfgfile: \n$status";
			}
			set status [readAndParseCfgDataFile cfgd cmdd cidx blk blknms $newfilename]
			if {[isNotOk $status]} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: readAndParseCfgDataFile failed at lnno $lnno in $cfgfile: \n$status";
			}
		} elseif {[string length $cidx] > 0} {
			if {[regexp -- {^([a-zA-Z_0-9]+)[ \t]+(.*)$} $cmd mstr submstr submstr2] != 1} {
				catch { close $fd; } ignore;
				return "-1 - readAndParseCfgDataFile: name-value error at lnno $lnno in $cfgfile";
			}
			if {$cfgd($blknms(1),type) == "exec"} {
				incr cfgd($cidx,maxcmd);
				incr cmdd(maxcmd);
				# store cmd in cfg data
				set cfgd($cidx,$cfgd($cidx,maxcmd)) "$submstr $submstr2";
				set cmdd($cmdd(maxcmd)) "$submstr $submstr2";
				if {$vflag} {
					puts "";
					puts "Name : <$cidx,$cfgd($cidx,maxcmd)>";
					puts "Value: <$cfgd($cidx,$cfgd($cidx,maxcmd))>";
				}
			} else {
				# store data in cfg data
				set cfgd($cidx,$submstr) $submstr2;
				if {$vflag} {
					puts "";
					puts "Name : <$cidx,$submstr>";
					puts "Value: <$cfgd($cidx,$submstr)>";
				}
			}
		} else {
			catch { close $fd; } ignore;
			return "-1 - readAndParseCfgDataFile: name-value pair not in block at lnno $lnno in $cfgfile";
		}
	}
	catch { close $fd; } ignore;
	#
	return "0 - success";
}
#
proc readCfgDataFile { cfgdata cmddata cfgfile {vflag 0} } {
	upvar $cfgdata cfgd;
	upvar $cmddata cmdd;
	#
	set cfgdidx "";
	set block 0;
	set blocknames(0) 0;
	#
	# start reading in data 
	#
	set status [readAndParseCfgDataFile cfgd cmdd cfgdidx block blocknames $cfgfile $vflag]
	if {[isNotOk $status]} {
		return "-1 - readCfgDataFile: readAndParseCfgDataFile failed: \n$status";
	} elseif {$block != 0} {
		return "-1 - readCfgDataFile: block level ($block) != 0 in $cfgfile";
	}
	#
	return "0 - success";
}
#
proc auditCfgData { cfgdata } {
	upvar $cfgdata cfgd;
	#
	return "0 - success";
}
#
proc printCfgData { cfgdata } {
	upvar $cfgdata cfgd;
	#
	if {[info exists cfgd]} {
		puts "\nContents of CfgData:\n";
		foreach item [lsort [array names cfgd]] {
			puts "cfgdata\($item\): $cfgd($item)";
		}
	}
	#
	return "0 - success";
}
#
proc printCmdData { cmddata } {
	upvar $cmddata cmdd;
	#
	if {[info exists cmdd]} {
		puts "\nContents of CmdData:\n";
		for {set cmd 1} {$cmd<=$cmdd(maxcmd)} {incr cmd} {
			puts "cmddata\($cmd\): $cmdd($cmd)";
		}
	}
	#
	return "0 - success";
}
