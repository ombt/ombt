#!/opt/exp/bin/tcl
#
# filter official TL1 basescripts to create TL1 scripts for BBB.
#
#########################################################################
#
package require Expect
#
# library functions
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/db
source $env(LCSTOOLSLIB)/checkretval
#
# directed graph describing the order to run the TL1 scripts. remember,
# it is not a tree, but a graph since there can be more than one path
# to or through a file, e.g., a common file like iproute.
#
set children(hardware) { hardware copymem };
set children(copymem) { copymem iproute calldata };
set children(iproute) { iproute calldata };
set children(calldata) { calldata calllines prov_linefeats };
set children(calllines) { callines prov_linefeats };
set children(prov_linefeats) { prov_linefeats };
#
# exit handler
#
exit -onexit {
	puts "\nDone.";
}
#
trap {
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "Got signal <$signame,$signo>";
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
# functions
#
proc usage { } {
	global argv0;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-V] \[-S trace level] \[+W]";
	puts "	\[-s suffix] \[-b branch\[,branch...] ] ";
	puts "	\[-D] \[-L | \[labid \[labid ...] ] ]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-S level - set trace level.";
	puts "	-b branch\[,branch...]  - create scripts for these branches.";
	puts "	-L - create scripts for all labids.";
	puts "	+W - do NOT write out BBB scripts, just traverse.";
	puts "	-s suffix - only ITU or ANSI are supported.";
	puts "	-D - print operation details.";
	puts "";
	puts "create BBB scripts for the given list of labids. if a labid is";
	puts "not given, then use the shell variable LABID. all scripts are";
	puts "created in the LCSBBBBASESCRIPTS directory. if no branch is given,";
	puts "then all branches for the labid are used.";
}
#
proc myputs { { string "" } } {
	global details;
	if {${details} != 0} {
		puts "${string}";
	}
	return "0 - success";
}
#
proc initstack { stack_name } {
	upvar $stack_name stack;
	set stack(left) 0;
	set stack(right) 0;
	return "0 - success";
}
proc isempty { array_name } {
	upvar $array_name array;
	#
	if {$array(left) >= $array(right)} {
		return 1;
	} else {
		return 0;
	}
}
proc depth { array_name } {
	upvar $array_name array;
	return [expr $array(right) - $array(left)];
}
proc initq { queue_name } {
	upvar $queue_name queue;
	set queue(left) 0;
	set queue(right) 0;
	return "0 - success";
}
proc nq { queue_name item } {
	upvar $queue_name queue;
	#
	set queue($queue(right)) $item;
	incr queue(right);
	#
	return "0 - success";
}
proc dq { queue_name item_name } {
	upvar $queue_name queue;
	upvar $item_name item;
	#
	if {[isempty queue]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $queue($queue(left));
	incr queue(left);
	#
	return "0 - success";
}
proc push { stack_name item } {
	upvar $stack_name stack;
	#
	set stack($stack(right)) $item;
	incr stack(right);
	#
	return "0 - success";
}
proc pop { stack_name item_name } {
	upvar $stack_name stack;
	upvar $item_name item;
	#
	if {[isempty stack]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	incr stack(right) -1;
	set item $stack($stack(right));
	#
	return "0 - success";
}
proc peeklhs { stack_name item_name } {
	upvar $stack_name stack;
	upvar $item_name item;
	#
	if {[isempty stack]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $stack($stack(left));
	#
	return "0 - success";
}
proc peekrhs { stack_name item_name } {
	upvar $stack_name stack;
	upvar $item_name item;
	#
	if {[isempty stack]} {
		# empty
		if {[info exists item]} {
			unset item;
		}
		return "0 - empty";
	}
	#
	set item $stack([expr $stack(right) - 1]);
	#
	return "0 - success";
}
#
proc isNoMatch { status } {
	if {[string match "0*no*match" ${status}]} {
		return 1;
	} else {
		return 0;
	}
}
#
proc savepath { stack_name paths_name labid current spaces } {
	upvar $stack_name stack;
	upvar $paths_name paths;
	#
	if {[isempty stack]} {
		# nothing to save
		return "0 - success";
	}
	#
	peekrhs stack lastpath;
	set lastfile [file tail ${lastpath}];
	set newdepth [depth stack];
	#
	if {[info exists paths(${lastfile},currentdepth)]} {
		set currentdepth $paths(${lastfile},currentdepth);
		if {${currentdepth} > ${newdepth}} {
			# keep current path
			return "0 - success - keep current path";
		} elseif {${currentdepth} < ${newdepth}} {
			# save new path since it is longer.
			set paths(${lastfile},currentdepth) ${newdepth};
			set paths(${lastfile},${newdepth},numberoffiles) 1;
			for {set idx $stack(left)} {${idx}<$stack(right)} {incr idx} {
				lappend paths(${lastfile},${newdepth},1) $stack(${idx});
			}
		} else {
			# we have another path to the same file and with 
			# the same path length. save it.
			set numberoffiles $paths(${lastfile},${newdepth},numberoffiles);
			incr numberoffiles;
			set paths(${lastfile},${newdepth},numberoffiles) ${numberoffiles};
			for {set idx $stack(left)} {${idx}<$stack(right)} {incr idx} {
				lappend paths(${lastfile},${newdepth},${numberoffiles}) $stack(${idx});
			}
		}
	} else {
		# first path to this file.
		set paths(${lastfile},currentdepth) ${newdepth};
		set paths(${lastfile},${newdepth},numberoffiles) 1;
		for {set idx $stack(left)} {${idx}<$stack(right)} {incr idx} {
			lappend paths(${lastfile},${newdepth},1) $stack(${idx});
		}
	}
	return "0 - sucess";
}
#
proc writepaths { paths_name labid current } {
	upvar $paths_name paths;
	#
	puts "\nPATHS FOUND:";
	foreach name [lsort [array names paths]] {
		if {[regexp -- {^.*currentdepth$} "${name}"] == 0} {
			continue;
		}
		set depth $paths(${name});
		if {[regexp -- {^(.*),currentdepth$} "${name}" ignore filename] == 1} {
			puts "\nFILENAME: ${filename}";
		} else {
			puts "\nWARNING: NO FILENAME FOUND FOR -- ${name}.";
			continue;
		}
		set numberoffiles $paths(${filename},${depth},numberoffiles);
		for {set idx 1} {${idx}<=${numberoffiles}} {incr idx} {
			puts "\nPATH (${idx}) TO ${filename}: $paths(${filename},${depth},${idx})";
			initstack stack;
			foreach path $paths(${filename},${depth},${idx}) {
				push stack ${path};
			}
			writedata stack ${labid} ${current};
		}
	}
	#
	return "0 - success";
}
#
proc writedata { stack_name labid current { spaces "" }} {
	upvar $stack_name stack;
	global env;
	#
	set bbbfilename "BBB";
	puts "\n${spaces}FILES IN CURRENT ${labid} BBB FILE ARE:\n";
	for {set idx $stack(left)} {${idx}<$stack(right)} {incr idx} {
		puts "${spaces}==>> $stack(${idx})";
		set abbr "";
		set script [file tail $stack(${idx})];
		if {[regsub -- {hardware} "${script}" "hw" abbr] == 1 ||
		    [regsub -- {calldata} "${script}" "cd" abbr] == 1 ||
		    [regsub -- {copy.*mem} "${script}" "cpm" abbr] == 1 ||
		    [regsub -- {iproute} "${script}" "ipr" abbr] == 1 ||
		    [regsub -- {calllines} "${script}" "cl" abbr] == 1 ||
		    [regsub -- {prov.*line.*feats} "${script}" "pl" abbr] == 1 ||
		    [regsub -- {users} "${script}" "us" abbr] == 1} {
			myputs "${spaces}ABBREVIATION: ${abbr}";
			append bbbfilename ".${abbr}";
		} else {
			myputs "${spaces}ABBREVIATION: NONE";
			append bbbfilename ".NONE";
		}
	}
	#
	set bbbdir "$env(LCSBBBBASESCRIPTS)/${labid}/${current}";
	set bbbfile "${bbbdir}/${bbbfilename}";
	#
	puts "\n${spaces}CURRENT ${labid} BBB FILE NAME:\n";
	puts "${spaces}==>> ${bbbfile}\n";
	#
	global writebbb;
	if {!${writebbb}} {
		# do not write out data, just return.
		return "0 - success - no writing of data";
	}
	#
	if {![file isdirectory "${bbbdir}"]} {
		puts "\n${spaces}==>> creating directory ${bbbdir}.";
		file mkdir ${bbbdir};
	}
	#
	set bbbfd [open $bbbfile "w"];
	#
	for {set idx $stack(left)} {${idx}<$stack(right)} {incr idx} {
		set filename "$stack(${idx})";
		puts "\n${spaces}==>> writing ${filename} to BBB file.";
		#
		set infd [open $filename "r"];
		puts $bbbfd "# INPUT FILE: ${filename}";
		for { } {[gets $infd inbuf] >= 0 && ![eof $infd]} { } {
			set lcinbuf [string tolower "${inbuf}"];
			switch -regexp -- "${lcinbuf}" {
			{init-sys:} {
				puts $bbbfd "# ${inbuf}";
				puts "==>> commenting ==>> '# ${inbuf}'";
			}
			{ed-chassis-eqpt:} {
				puts $bbbfd "# ${inbuf}";
				puts "==>> commenting ==>> '# ${inbuf}'";
			}
			{ent-eqpt:} {
				puts $bbbfd "# ${inbuf}";
				puts "==>> commenting ==>> '# ${inbuf}'";
			}
			{ent-eqpt-all:} {
				puts $bbbfd "# ${inbuf}";
				puts "==>> commenting ==>> '# ${inbuf}'";
			}
			{cpy-mem:} {
				puts $bbbfd "# ${inbuf}";
				puts "==>> commenting ==>> '# ${inbuf}'";
			}
			default {
				puts $bbbfd "${inbuf}";
			}
			}
		}
		close $infd;
	}
	puts "";
	#
	close $bbbfd;
	#
	return " 0 - success";
}
#
proc traverse { labid branch current scripts_name paths_name stack_name script type number suffix spaces childfound_name { autozone 0 } { maxchildnumber 5 } } {
	upvar ${paths_name} paths;
	upvar ${scripts_name} scripts;
	upvar ${stack_name} stack;
	upvar ${childfound_name} childfound;
	#
	global children;
	#
	myputs "${spaces}CHECK: ${type}-${number}-${script}";
	#
	if {[regexp -- "^.*${type}${number}\[^\.]*(.*)$" "${script}" ignore newsuffix] == 0 && [regexp -- "^.*${type}\[^\.]*(.*)$" "${script}" ignore newsuffix] == 0} {
		set newsuffix "";
	}
	#
	set suffixlen [string length "${suffix}"];
	set newsuffixlen [string length "${newsuffix}"];
	#
	myputs "${spaces}BEFORE: <suffix,newsuffix> - <${suffix},${newsuffix}>";
	if {"${suffix}" == "${newsuffix}"} {
		set ok 1;
	} elseif {${suffixlen} == 0 && ${newsuffixlen} > 0} {
		set ok 1;
	} elseif {${suffixlen} > 0 && ${newsuffixlen} == 0} {
		set ok 1;
		set newsuffix "${suffix}";
	} elseif {${type} != "calldata" && ${type} != "calllines" && [regexp -- "${newsuffix}$" "${suffix}"]} {
		set ok 1;
		set newsuffix "${suffix}";
	} elseif {[regexp -- "${suffix}$" "${newsuffix}"]} {
		set ok 1;
	} elseif {[regexp -- "^.*ITU.*$" "${suffix}"] == 1 && [regexp -- "^.*ANSI.*$" "${newsuffix}"] == 0 && $newsuffixlen > 0 && ${type} == "calldata"} {
		set ok 1;
	} else {
		set ok 0;
	}
	# check if auto filtering is on
	if {${autozone}} {
		# auto filtering does NOT allow calldata, callline
		# or prov_linefeats scripts that do not also 
		# have the auto suffix.
		if {[regexp -nocase -- "^.*calldata.*$" "${script}"] == 1 || \
		    [regexp -nocase -- "^.*calllines.*$" "${script}"] == 1 || \
		    [regexp -nocase -- "^.*prov_linefeat.*$" "${script}"] == 1} {
			if {[regexp -nocase -- "^.*auto.*$" "${script}"] == 0} {
				set ok 0;
			}
		}
	}
	#
	myputs "${spaces}AFTER: <suffix,newsuffix,ok> - <${suffix},${newsuffix},${ok}>";
	if {!${ok}} {
		myputs "${spaces}REJECT: ${type}-${number}-${script}";
		# no match
		myputs "<<< RETURN at 'NOT OK' >>>";
		return "0 - success - no match";
	}
	# check if dealing with auto scripts
	if {[regexp -nocase -- {.*auto.*} $script ignore] == 1} {
		set autozone 1;
	}
	#
	incr childfound;
	myputs "${spaces}ACCEPT: ${type}-${number}-${script}";
	if {${autozone}} {
		puts "${spaces}${type}-${number}-${script} (auto)";
	} else {
		puts "${spaces}${type}-${number}-${script} (no-auto)";
	}
	#
	push stack ${script};
	#
	set oldspaces $spaces;
	append spaces "    ";
	#
	set newchildfound 0;
	#
	myputs "${spaces}PARENT TYPE: ${type}";
	myputs "${spaces}CHILDREN TYPES: $children(${type})";
	foreach childtype $children(${type}) {
		if {${childtype} == ${type}} {
			set childnumber ${number};
			incr childnumber;
		} else {
			set childnumber 1;
		}
		for { } {${childnumber}<=${maxchildnumber}} {incr childnumber} {
			myputs "${spaces}CHECKING: ${childtype}-${childnumber}";
			if {[info exists scripts(${childtype}${childnumber})]} {
				myputs "${spaces}CHECKING CHILDREN: $scripts(${childtype}${childnumber})";
				foreach childscript $scripts(${childtype}${childnumber}) {
					set status [traverse ${labid} ${branch} ${current} scripts paths stack $childscript ${childtype} ${childnumber} "${newsuffix}" "${spaces}" newchildfound ${autozone} ];
					if {[isNotOk $status]} {
						return "-1 - traverse: traverse failed:\n${status}";
					}
				}
			}
		}
	}
	# done traversing, check if it is a leaf.
	if {${newchildfound} <= 0} {
		# the node was a leaf -- no children found.
		puts "${oldspaces}${type}-${number}-depth: [depth stack]";
		myputs "<<< RETURN at found NO child >>>";
		savepath stack paths $labid $current $oldspaces;
	} else {
		myputs "<<< RETURN at found child >>>";
	}
	pop stack dummy;
	return "0 - success";
}
#
proc makebbb { labid branch current { suffix "" } } {
	puts "\nLABID  : ${labid}";
	puts "BRANCH : ${branch}";
	puts "CURRENT: ${current}\n";
	#
	set scripts [glob -nocomplain "${current}/*"];
	if {[string length "${scripts}"] <= 0} {
		puts "\nWARNING: no files found under ${current}.";
		return "0 - success - no files found under ${current}";
	}
	unset scripts;
	#
	foreach script [glob "${current}/*"] {
		if {![file isfile "${script}"]} {
			continue;
		}
		switch -regexp -- "${script}" {
		{^.*hardware([2-9][0-9]*).*$} {
			regexp {^.*hardware([0-9]).*$} "${script}" dummy num;
			lappend scripts(hardware${num}) ${script};
			puts "HARDWARE (${num}) FILE: ${script}";
		}
		{^.*hardware.*$} {
			lappend scripts(hardware1) ${script};
			puts "HARDWARE (1) FILE: ${script}";
		}
		{^.*calldata([2-9][0-9]*).*$} {
			regexp {^.*calldata([0-9][0-9]*).*$} "${script}" dummy num;
			lappend scripts(calldata${num}) ${script};
			puts "CALLDATA (${num}) FILE: ${script}";
		}
		{^.*calldata.*$} {
			lappend scripts(calldata1) ${script};
			puts "CALLDATA (1) FILE: ${script}";
		}
		{^.*copy.*mem([2-9][0-9]*).*$} {
			regexp {^.*copymem([0-9][0-9]*).*$} "${script}" dummy num;
			lappend scripts(copymem${num}) ${script};
			puts "COPYMEM (${num}) FILE: ${script}";
		}
		{^.*copy.*mem.*$} {
			lappend scripts(copymem1) ${script};
			puts "COPYMEM (1) FILE: ${script}";
		}
		{^.*iproute([2-9][0-9]*).*$} {
			regexp {^.*iproute([0-9]).*$} "${script}" dummy num;
			lappend scripts(iproute${num}) ${script};
			puts "IPROUTE (${num}) FILE: ${script}";
		}
		{^.*iproute.*$} {
			lappend scripts(iproute1) ${script};
			puts "IPROUTE (1) FILE: ${script}";
		}
		{^.*calllines([2-9][0-9]*).*$} {
			regexp {^.*calllines([0-9][0-9]*).*$} "${script}" dummy num;
			lappend scripts(calllines${num}) ${script};
			puts "CALLLINES (${num}) FILE: ${script}";
		}
		{^.*calllines.*$} {
			lappend scripts(calllines1) ${script};
			puts "CALLLINES (1) FILE: ${script}";
		}
		{^.*prov_linefeats([2-9][0-9]*).*$} {
			regexp {^.*prov_linefeats([0-9][0-9]*).*$} "${script}" dummy num;
			lappend scripts(prov_linefeats${num}) ${script};
			puts "PROV_LINEFEATS (${num}) FILE: ${script}";
		}
		{^.*prov_linefeats.*$} {
			lappend scripts(prov_linefeats1) ${script};
			puts "PROV_LINEFEATS (1) FILE: ${script}";
		}
		{^.*users.*$} {
			lappend scripts(users) ${script};
			puts "USERS FILE: ${script}";
		}
		{^.*sigdbg.*$} {
			lappend scripts(sigdbg) ${script};
			puts "SIGDBG FILE: ${script}";
		}
		{^.*sigdebug.*$} {
			lappend scripts(sigdbg) ${script};
			puts "SIGDBG FILE: ${script}";
		}
		{^.*add.*$} {
			lappend scripts(addlinks) ${script};
			puts "ADDLINKS FILE: ${script}";
		}
		default {
			puts "UNKNOWN FILE: ${script}";
		}
		}
	}
	#
	# cycle over scripts
	#
	if {![info exists scripts(hardware1)]} {
		puts "\nWARNING: no hardware1 scripts found for ${labid}/${branch}.";
		return "0 - success";
	}
	#
	puts "";
	#
	initstack stack;
	#
	foreach script $scripts(hardware1) {
		if {[info exists paths]} {
			unset paths;
		}
		set childfound 0;
		#
		if {[regexp -nocase -- {.*auto.*} $script ignore] == 1} {
			set autozone 1;
		} else {
			set autozone 0;
		}
		#
		set status [traverse ${labid} ${branch} ${current} scripts paths stack ${script} hardware 1 "${suffix}" "" childfound ${autozone} ];
		if {[isNotOk $status]} {
			return "-1 - makebbb: traverse failed:\n${status}";
		}
		writepaths paths ${labid} ${current};
	}
	#
	return "0 - success";
} 
#
#########################################################################
#
puts "\nCreate BBB TL1 Scripts:";
#
# default values
#
set alllabids 0;
set allbranches 1;
set branches "";
set stracelevel 0;
set verbose 0;
set details 0;
set writebbb 1;
set suffix "";
#
global env;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-s.*} { getoptval $argval suffix arg; }
	{^-b.*} { 
		getoptval $argval branches arg; 
		set branches [string trim "${branches}"];
		set allbranches 0; 
	}
	{^-L} { set alllabids 1; }
	{^\+W} { set writebbb 0; }
	{^-V} { set verbose 1; }
	{^-D} { set details 1; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
if {$verbose > 0} {
	puts "\nLogging Enabled ...";
	log_user 1;
} else {
	log_user 0;
}
#
checkenv;
#
checkenvvar LCSBASESCRIPTS;
checkenvvar LCSSCRIPTS;
checkenvvar LCSBBBBASESCRIPTS;
#
# if {[id userid] != [id convert user lcstools]} {
	# puts "\nWARNING: UID is NOT lcstools. Disabling write (+W).";
	# set writebbb 0;
# }
#
cd $env(LCSBASESCRIPTS);
#
if {${alllabids}} {
	set labids [glob *];
} elseif {${arg}<${argc}} {
	set labids "";
	for { } {${arg}<${argc}} {incr arg} {
		set labid [lindex $argv $arg];
		append labids " ${labid}";
	}
	set labids [string trim "${labids}"];
} else {
	checkenvvar LABID;
	set labids $env(LABID);
}
#
puts "\nLabids: ${labids}";
#
foreach labid [split ${labids} " "] {
	cd $env(LCSBASESCRIPTS);
	if {![file isdirectory "${labid}"]} {
		# skip any file that is not a directory
		puts "\nWARNING: labid '${labid}' is not a directory. Skipping it.";
		continue;
	}
	#
	puts "\nProcessing Labid: ${labid}";
	#
	cd "${labid}";
	#
	if {${allbranches}} {
		set branches [glob *];
	} elseif {[string length "${branches}"] > 0} {
		regsub -all -- "," ${branches} " " branches;
		set branches [string trim "${branches}"];
	} else {
		puts "\nERROR: no branches given or found.";
		exit 2;
	}
	#
	foreach branch [split ${branches} " "] {
		set current ${branch}/current;
		if {![file isdirectory "${current}"]} {
			# does not exist or is not a directory
			puts "\nWARNING: ${current} is not a directory. Skipping it.";
			continue;
		}
		puts "\nScanning Branch Directory: ${current}";
		set status [makebbb ${labid} ${branch} ${current} ${suffix} ];
		if {[isNotOk $status]} {
			puts "\nERROR: makebbb failed:\n${status}";
			exit 2;
		}
	}
}
#
exit 0;
