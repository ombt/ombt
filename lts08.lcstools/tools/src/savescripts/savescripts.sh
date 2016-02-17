#!/opt/exp/bin/expect
#
# save list of scripts to the local repository.
#
#########################################################################
#
# library functions
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/chooseone
source $env(LCSTOOLSLIB)/logging
source $env(LCSTOOLSLIB)/db
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-U] \[-V] \[-S trace level] script1 \[script2 ...]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-U - unconditional installation; remove local file when it exists.";
	puts "	-V - enable verbose output";
}
#
proc getcpuload { branch cpuloadname } {
	upvar $cpuloadname cpuload;
	#
	if {[dbselect cpuloadlist loads "branch req ^${branch}$" cpuload] != 0} {
		puts "saveload: dbselect of relation 'loads' failed.";
		exit 2;
	}
	#
	if {![info exists cpuloadlist]} {
		puts "no cpuloads found for branch $branch.";
		exit 2;
	} else {
		set choices $cpuloadlist;
		set choices [linsert $choices 0 "QUIT"];
		chooseone "Choose a cpu-load: " choices answer;
		if {$answer == "QUIT"} {
			puts "exiting saveload.";
			exit 0;
		}
		set cpuload $answer;
	}
}
#
proc insertimages { branch cpuload imagetype image } {
	if {[dbselect names images "branch req ^${branch}$ and cpuload req ^${cpuload}$ and type req ^${imagetype}$ and name req ^${image}$" "name"] != 0} {
		puts "insertimages: dbselect of relation 'images' failed.";
		exit 2;
	}
	if {[info exists names] && [llength names] > 0} {
		puts "insertimages: tuple <$branch,$cpuload,$imagetype,$image> exists.";
		return 0;
	} 
	set namevalue(branch) $branch;
	set namevalue(cpuload) $cpuload;
	set namevalue(type) $imagetype;
	set namevalue(name) $image;
	if {[dbinsert images namevalue] != 0} {
		puts "insertimages: dbinsert of relation 'images' failed.";
		exit 2;
	}
	return 0;
}
#
proc insertloads { branch cpuload rootfilesys } {
	if {[dbselect filesys loads "branch req ^${branch}$ and cpuload req ^${cpuload}$ and basedir req ^${rootfilesys}$" "basedir"] != 0} {
		puts "insertloads: dbselect of relation 'loads' failed.";
		exit 2;
	}
	if {[info exists filesys] && [llength filesys] > 0} {
		puts "insertloads: tuple <$branch,$cpuload,$rootfilesys> exists.";
		return 0;
	} 
	set namevalue(branch) $branch;
	set namevalue(cpuload) $cpuload;
	set namevalue(basedir) $rootfilesys;
	if {[dbinsert loads namevalue] != 0} {
		puts "insertloads: dbinsert of relation 'loads' failed.";
		exit 2;
	}
	return 0;
}
#
proc createdbentries { branch ilist rootfilesys } {
	global env;
	global expect_out;
	#
	upvar $ilist images;
	#
	puts "\ncreating database entries ...";
	#
	set bdata [split $branch " \t"];
	set branch [lindex $bdata 0];
	puts "branch: $branch";
	#
	# get swCPU load
	#
	if {$images(swCPU) == "NONE"} {
		# we need a cpu load to store the data. ask the user.
		getcpuload $branch cpuload;
	} elseif {[regexp -nocase -- "^(.*)_cpu\.tar\.gz$" $images(swCPU) ignore cpuload] != 1} {
		puts "createdbentries: unable to get cpu-load.";
		exit 2;
	}
	puts "cpu-load: $cpuload";
	#
	# insert any new images tuples
	#
	foreach imagetype [array names images] {
		foreach image [split $images($imagetype)] {
			if {$image == "NONE"} {
				continue;
			}
			insertimages $branch $cpuload $imagetype $image;
		}
	}
	#
	# insert any new loads tuple
	#
	insertloads $branch $cpuload $rootfilesys;
}
#
proc getimagesubtypes { switype ilistname stypelistname } {
	upvar $ilistname ilist;
	upvar $stypelistname subitypecnt;
	#
	if {[regexp "^sw(.*)$" $switype ignore itype] != 1} {
		puts "checkformultipleimages: invalid image type - $switype.";
		exit 2;
	}
	#
	foreach image $ilist {
		if {[regexp -nocase -- "(${itype}.*)\.tar\.gz$" $image ignore subitype] == 1} {
			if {[info exists subitypecnt($subitype)]} {
				incr subitypecnt($subitype) +1;
			} else {
				set subitypecnt($subitype) 1;
			}
		}
	}
	#
	return [array size subitypecnt];
}
#
proc savescripts { } {
	global env;
	global expect_out;
	global spawn_id;
	#
	if {[dbselect branches buildmachines "" "branch machine directory"] != 0} {
		puts "saveload: dbselect of relation 'buildmachines' failed.";
		exit 2;
	}
	#
	set choices $branches;
	set choices [linsert $choices 0 "QUIT"];
	chooseone "Choose a branch: " choices branch 
	if {$branch == "QUIT"} {
		puts "exiting saveload.";
		exit 0;
	}
	unset choices;
	#
	getimages $branch images
	# 
	foreach image [array names images] {
		getimagesubtypes $image images($image) imagesubtypes;
		#
		foreach imagesubtype [array names imagesubtypes] {
			puts "imagesubtype is <$imagesubtype>.";
			#
			set prompt "Choose an $image image to save: ";
			lappend choices "QUIT" "NONE";
			#
			foreach imagefile $images($image) {
				if {[regexp -nocase -- "(${imagesubtype}\.tar\.gz)$" $imagefile ignore ignore2] == 1} {
					lappend choices $imagefile;
				}
			}
			#
			set image2save "";
			chooseone "${prompt}" choices image2save;
			if {$image2save == "QUIT"} {
				puts "exiting saveload.";
				exit 0;
			}
			lappend images2save($image) $image2save;
			if {$image2save != "NONE"} {
				puts "Will save image $images2save($image).";
			}
			unset choices;
		}
		unset imagesubtypes;
	}
	#
	saveimages $branch images2save savecount filesys;
	#
	puts "$savecount images saved; will create DB entries.";
	createdbentries $branch images2save $filesys;
}
#
#########################################################################
#
# default values
#
set verbose 0;
set ucond 0;
set stracelevel -1;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-V} { set verbose 1; }
	{^-U} { set ucond 1; }
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-\?} { usage; exit 0; }
	{^--} { incr arg; break; }
	{^-.*} { puts "\nunknown option: $argval\n"; usage; exit 2 }
	default { break; }
	}
}
#
if {$arg >= $argc} {
	puts "[file tail $argv0]: no scripts were given.";
	usage;
	exit 2;
}
#
if {$stracelevel >= 0} {
	strace $stracelevel;
}
if {$verbose} {
	log_user 1;
} else {
	log_user 0;
}
#
checkenv
#
logusage savescripts
#
savescripts 
#
exit 0

