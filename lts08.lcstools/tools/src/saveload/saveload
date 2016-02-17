#!/opt/exp/bin/expect
#
# save a load from a build machine to the local repository.
#
#########################################################################
#
# library functions
#
source $env(LCSTOOLSLIB)/checkenv
source $env(LCSTOOLSLIB)/getoptval
source $env(LCSTOOLSLIB)/chooseone
source $env(LCSTOOLSLIB)/db
source $env(LCSTOOLSLIB)/lcsftp
source $env(LCSTOOLSLIB)/lcstelnet
source $env(LCSTOOLSLIB)/logging
source $env(LCSTOOLSLIB)/lock
source $env(LCSTOOLSLIB)/local
source $env(LCSTOOLSLIB)/checkretval
#
# exit handler
#
exit -onexit {
	global LABID;
	if {[info exists LABID] && [llength LABID] > 0} {
		V saveload $LABID;
	}
}
#
trap {
	global LABID;
	puts "";
	set signame [trap -name];
	set signo [trap -number];
	puts "Got signal <$signame,$signo>";
	if {[info exists LABID] && [llength LABID] > 0} {
		V saveload $LABID;
		unset LABID;
	}
	exit 2;
} {SIGHUP SIGINT SIGQUIT SIGPIPE SIGTERM SIGBUS};
#
proc eofexit { status } {
	global LABID;
	if {[info exists LABID] && [llength LABID] > 0} {
		quietV saveload $LABID;
		unset LABID;
	}
	exit $status;
}
#
# functions
#
proc usage { } {
	global argv0;
	global username;
	global userpasswd;

	puts "\nusage: [file tail $argv0] \[-\?] \[-x] \[-T] \[-B] \[-R] \[-U] \[-V] \[-O] \[-S trace level]";
	puts "	\[-d] \[-G] \[-b branch_to_save] \[-u username] \[-p passwd] ";
	puts "	\[-s] \[-t status] \[-F filename] \[labid \[labid ...] ]";
	puts "where:";
	puts "	-? - print usage message";
	puts "	-x - enable TCL debugger";
	puts "	-T - test-mode.";
	puts "	-U - unconditional installation; remove local file when it exists.";
	puts "	-V - enable verbose output";
	puts "	-s - save old file instead of overwriting or deleting it";
	puts "	-R - remove saveload lock";
	puts "	-B - run saveload in the background.";
	puts "	-G - get the latest file for CPU and IOMs.";
	puts "	-b branch - BRANCH load to save.";
	puts "	-t status - default load status, 'oper' or 'test'.";
	puts "	-O - allow over-write when CPU load has NOT changed, but IOMs have changed";
	puts "	-d - save CPU DEBUG files locally.";
	puts "	-r remote_machine!full_path - expand CPU DEBUG files on ";
	puts "		remote machine at the given path.";
	puts "	-F filename - file contains a list of files to save, ";
	puts "		the list must contain a CPU tar file. the file format ";
	puts "		is the same as the images relation.";
	puts "";
	puts "The load will be turned on for any labid listed in the command line.";
	puts "The default status when assigning a load to a labid is 'oper' if";
	puts "the -t option is not used.";
	puts "";
	puts "When using the list the files-to-save, you must know that it is";
	puts "NOT guaranteed that a given file-to-save is either found in the";
	puts "chosen branch, or that the file even exists. The algorithm for";
	puts "saving files is this. First, remotely generate a list of all "
	puts "available files for the chosen branch. Then, for each type of ";
	puts "tar file, compare the files for each type against the given ";
	puts "list of files-to-save. If there is a match, then save the file.";
	puts "Repeat this process for each type of file. The files are saved";
	puts "under the CPU load from the CPU file-to-save.";
}
#
proc getimages { branch imagesname { debugflag "" } } {
	global env;
	upvar $imagesname images;
	# get remaining data
	if {[dbselect record buildmachines "branch req ^${branch}$" "branch machine directory type"] != 0} {
		puts "getimages: dbselect of relation 'buildmachines' failed.";
		logmsg saveload "END: getimages: dbselect of relation 'buildmachines' failed.";
		exit 2;
	}
	# get branch for BTS.
	if {[regexp -- {^BP-} ${branch}]} {
		# beta branch, look for parent branch
		if {[dbselect basebuf basebranch "branch req ^$branch\$ and type req ^tl1\$" "base" ] != 0} {
			puts "getimages: dbselect of relation 'basebranch' failed.";
			logmsg saveload "END: getimages: dbselect of relation 'basebranch' failed.";
			exit 2;
		}
		if {![info exists basebuf] || [llength $basebuf] == 0} {
			puts "getimages: UNKNOWN BTS branch, unable to get parent branch for ${branch}";
			set btsbranch "UNKNOWN";
		} else {
			set btsbranch [lindex $basebuf 0];
			puts "getimages: BETA BTS branch is ${btsbranch}";
		}
	} else {
		# dev branch, bts branch is the same.
		set btsbranch ${branch};
		puts "getimages: DEV BTS branch is ${btsbranch}";
	}
	#
	set record [lindex $record 0];
	set bdata [split $record " \t"];
	set machine [lindex $bdata 1];
	set directory [lindex $bdata 2];
	set btype [lindex $bdata 3];
	set toolsbin $env(LCSTOOLSBIN);
	#
	puts "";
	puts "Branch    : ${branch}";
	puts "Machine   : ${machine}";
	puts "Directory : ${directory}";
	puts "Type      : ${btype}";
	puts "BTS Branch: ${btsbranch}";
	puts "";
	#
	set mypid [pid];
	# copy tool to determine the list of loads.
	ftp_put_binary_file $machine "${toolsbin}/load2images" "/tmp/load2images${mypid}";
	# run it remotely
	set outfile "/tmp/load2images.out.${mypid}";
	#
	telnet_to $machine remote_ip;
	remote_exec $remote_ip "/bin/chmod 777 /tmp/load2images${mypid}";
	#
	# if debug flag is set to -d, then the file of imagetype-to-files
	# will contain a list of image types that start with debug_XXX.
	# as of now, only debug_cpu is expected and supported.
	# 2/1/2008 - adding support for cm debug files.
	#
	remote_exec $remote_ip "/tmp/load2images${mypid} ${debugflag} ${branch} $btype all  ${btsbranch} 1>${outfile} 2>&1";
	# get results
	ftp_get_binary_file $machine "${outfile}" "${outfile}";
	# delete old files
	remote_exec $remote_ip "/bin/rm -f /tmp/load2images${mypid} ${outfile} 1>/dev/null 2>&1";
	close_telnet $remote_ip;
	# read results
	set infd [open $outfile "r"];
	while {[gets $infd line] != -1} {
		# store images data. remove "NONE" entries.
		switch -regexp -- $line {
		{^.*NONE *0$} {
			#skip this;
		}
		{^.*ERROR:} {
			puts "load2images: $line";
			logmsg saveload "END: load2images: $line";
			exit 2;
		}
		default {
			lappend images $line;
		}
		}
	}
	close $infd;
	#
	if {[file exists $outfile]} {
		file delete -force -- $outfile;
	}
}
#
proc shipdebugfile { imagetype tofile } {
	global remotemach;
	global rmach;
	global rdir;
	global global_cpuload;
	global global_branch;
	global global_deleteremotedir;
	#
	if {[string length "${remotemach}"] <= 0} {
		# no remote machine to send debug files.
		return 0;
	}
	#
	# we have a remote machine and a file to send and expand.
	#
	set tarfile [file tail "${tofile}"];
	#
	# clean up any previous tar files or directories for 
	# for this load.
	#
	telnet_to ${rmach} remote_id;
	remote_exec $remote_id "/bin/rm -f ${rdir}/${global_cpuload}/${tarfile} 2>/dev/null 1>&2";
	if {${global_deleteremotedir}} {
		remote_exec $remote_id "/bin/rm -rf ${rdir}/${global_cpuload} 2>/dev/null 1>&2";
		remote_exec $remote_id "/bin/mkdir -p ${rdir}/${global_cpuload} 2>/dev/null 1>&2";
		set global_deleteremotedir 0;
	}
	close_telnet $remote_id;
	#
	# ship the new debug tar file over.
	#
	ftp_put_binary_file "${rmach}" "${tofile}" "${rdir}/${global_cpuload}/${tarfile}";
	#
	# expand the remote tar file.
	#
	telnet_to ${rmach} remote_id;
	remote_exec $remote_id "cd ${rdir}/${global_cpuload}";
	remote_exec $remote_id "echo ${global_branch} >branch";
	remote_exec $remote_id "/bin/tar xzvf ${rdir}/${global_cpuload}/${tarfile}" 1500;
	# remove tar file once it is expanded.
	remote_exec $remote_id "/bin/rm -f ${rdir}/${global_cpuload}/${tarfile} 2>/dev/null 1>&2";
	close_telnet $remote_id;
	#
	return 0;
}
#
proc saveimages { branch ilist savecntname filesysname imagesizesname } {
	global env;
	global expect_out;
	global ucond;
	global branch2save;
	global saveoldfiles;
	global now;
	#
	upvar $ilist images;
	upvar $savecntname savecnt;
	upvar $filesysname filesys;
	upvar $imagesizesname imagesizes;
	#
	# get remaining data
	if {[dbselect record buildmachines "branch req ^${branch}$" "branch machine directory type"] != 0} {
		puts "saveimages: dbselect of relation 'buildmachines' failed.";
		logmsg saveload "END: saveimages: dbselect of relation 'buildmachines' failed.";
		exit 2;
	}
	#
	set record [lindex $record 0];
	#
	set bdata [split $record " \t"];
	set machine [lindex $bdata 1];
	set directory [lindex $bdata 2];
	set btype [lindex $bdata 3];
	#
	set savecnt 0;
	#
	# text file system
	#
	if {[dbselect fsobuf filesystems "branch req ^$branch\$ and type req text" "path" ] != 0} {
		puts "saveimages: dbselect of relation 'filesystems' failed.";
		logmsg saveload "END: saveimages: dbselect of relation 'filesystems' failed.";
		exit 2;
	}
	if {![info exists fsobuf] || [llength fsobuf] == 0} {
		puts "saveimages: no filesystem found for branch $branch.";
		logmsg saveload "END: saveimages: no filesystem found for branch $branch.";
		exit 2;
	}
	set choices $fsobuf;
	if {$branch2save == ""} {
		set choices [linsert $choices 0 "QUIT"];
		chooseone "Choose a file system: " choices rootfilesys;
		if {$rootfilesys == "QUIT"} {
			puts "exiting saveload.";
			logmsg saveload "END: exiting saveload.";
			exit 0;
		}
	} else {
		# default to the first one for now. later we
		# can check file usage and choose the least
		# used file system.
		set rootfilesys [lindex $choices 0]
		puts "\nUsing default file system $rootfilesys.";
	}
	set filesys $rootfilesys;
	#
	# at this point, if we are doing the work in the 
	# background, then disconnect.
	#
	global background;
	if {$background != 0} {
		if {[fork] != 0} {
			puts "Backgrounding. Parent saveload exiting.";
			logmsg saveload "END: Backgrounding. Parent saveload exiting.";
			global LABID;
			unset LABID;
			exit 0;
		}
		disconnect;
	}
	#
	foreach imagetype [array names images] {
		#
		set fromfile $images($imagetype);
		set tofilepath "$rootfilesys/$branch/$imagetype";
		set image [file tail $images($imagetype)];
		set tofile "$rootfilesys/$branch/$imagetype/$image";
		#
		if {![file isdirectory $tofilepath]} {
			file mkdir $tofilepath;
		}
		#
		if {[info exists data]} {
			unset data;
		}
		#
		puts "\nNew File: ${tofile}";
		set newfilesize $imagesizes(${fromfile});
		puts "New file size in bytes: ${newfilesize}";
		set newfilesize [expr $imagesizes(${fromfile})/1000];
		puts "New file size in kilobytes: ${newfilesize}";
		#
		set status [local_df data "${tofilepath}"];
		if {[isNotOk $status]} {
			puts "saveimages: unable to df file system ${tofilepath}.";
			logmsg saveload "END: saveimages: unable to df file system ${tofilepath}.";
			exit 2;
		}
		set localfs $data(localfs);
		set localfsfree $data(${localfs},fsavail);
		#
		puts "\nSpace left in ${localfs}    : ${localfsfree} (kbytes)";
		puts "Space required in ${localfs}: ${newfilesize} (kbytes)";
		#
		if {${newfilesize} >= ${localfsfree}} {
			global global_branch;
			global global_cpuload;
			#
			puts "\nsaveimages: RAN OUT OF SPACE IN ${tofilepath} - local file space (${localfsfree}) is less than file size ${newfilesize} for ${tofile}.";
			logmsg saveload "END: saveimages: RAN OUT OF SPACE IN ${tofilepath} - local file space (${localfsfree}) is less than file size ${newfilesize} for ${tofile} for $global_branch/$global_cpuload.";
			sendemail $global_branch "saveload" "success" "SAVELOAD: RAN OUT OF SPACE IN ${tofilepath} for load $global_branch/$global_cpuload. Check saveload log file for details.";
			exit 2;
		}
		#
		if {[file exists $tofile]} {
			puts "\n$tofile exists locally.";
			if {$ucond} {
				puts "===>>> deleting local file ...";
				logmsg saveload "===>>> deleting local file ...";
				file delete -force -- $tofile;
			} elseif {$saveoldfiles} {
				# compare sizes of files. if the sizes
				# are different, then rename the local
				# old file and save the new one.
				set newfilesize $imagesizes(${fromfile});
				set oldfilesize [file size ${tofile}];
				#
				puts "===>>> size of new file ${newfilesize}";
				puts "===>>> size of old file ${oldfilesize}";
				#
				logmsg saveload "===>>> size of new file ${newfilesize}";
				logmsg saveload "===>>> size of old file ${oldfilesize}";
				#
				if {$oldfilesize == $newfilesize} {
					# the same file
					puts "===>>> sizes are the same, skipping local file ...";
					logmsg saveload "===>>> sizes are the same, skipping local file ...";
					continue;
				} else {
					# rename the old file, add time stamp to name.
					puts "===>>> sizes are the different, rename local file and save new file ...";
					logmsg saveload "===>>> sizes are the different, rename local file and save new file ...";
					#
					set savetofile "${tofile}_${now}";
					puts "===>>> renaming ${tofile} ...";
					logmsg saveload "===>>> renaming ${tofile} ...";
					file rename -force -- ${tofile} ${savetofile};
					puts "===>>> new file name ${savetofile}";
					logmsg saveload "===>>> new file name ${savetofile}";
				}
			} else {
				puts "===>>> skipping local file ...";
				logmsg saveload "===>>> skipping local file ...";
				continue;
			}
		}
		#
		puts "\nSAVING IMAGE TYPE $imagetype:"
		puts "FROM FILE: $machine!$fromfile";
		puts "TO FILE  : $tofile";
		#
		logmsg saveload "SAVING IMAGE TYPE $imagetype:"
		logmsg saveload "FROM FILE: $machine!$fromfile";
		logmsg saveload "TO FILE  : $tofile";
		#
		ftp_get_binary_file $machine $fromfile $tofile
		#
		# check for debug files ..
		#
		if {[regexp -- {^debug_} "${imagetype}"] == 1} {
			# check if we should ship debug file to a 
			# remote machine for expansion.
			shipdebugfile ${imagetype} ${tofile};
		}
		#
		incr savecnt +1;
	}
}
#
proc DoesCpuLoadExist { branch cpuload } {
	#
	if {[dbselect cpuloadlist loads "branch req ^${branch}$ and cpuload req ^${cpuload}$" cpuload] != 0} {
		puts "saveload: DoesCpuLoadExist: dbselect of relation 'loads' failed.";
		logmsg saveload "END: saveload: DoesCpuLoadExist: dbselect of relation 'loads' failed.";
		exit 2;
	}
	#
	if {[info exists cpuloadlist]} {
		puts "\nbranch/cpuload $branch/$cpuload exists.";
		logmsg saveload "DoesCpuLoadExist: branch/cpuload $branch/$cpuload exists.";
		return 1;
	} else {
		puts "\nbranch/cpuload $branch/$cpuload does NOT exist.";
		logmsg saveload "DoesCpuLoadExist: branch/cpuload $branch/$cpuload does NOT exist.";
		return 0;
	}
}
#
proc getcpuload { branch cpuloadname } {
	upvar $cpuloadname cpuload;
	#
	if {[dbselect cpuloadlist loads "branch req ^${branch}$" cpuload] != 0} {
		puts "saveload: dbselect of relation 'loads' failed.";
		logmsg saveload "END: saveload: dbselect of relation 'loads' failed.";
		exit 2;
	}
	#
	if {![info exists cpuloadlist]} {
		puts "no cpuloads found for branch $branch.";
		logmsg saveload "END: no cpuloads found for branch $branch.";
		exit 2;
	} else {
		set choices $cpuloadlist;
		set choices [linsert $choices 0 "QUIT"];
		chooseone "Choose a cpu-load: " choices answer;
		if {$answer == "QUIT"} {
			puts "exiting saveload.";
			logmsg saveload "END: exiting saveload.";
			exit 0;
		}
		set cpuload $answer;
	}
}
#
proc insertimages { branch cpuload imagetype image } {
	# check if the image tuple already exists. the tuple
	# must match exactly what is about to be inserted.
	#
	if {[dbselect names images "branch req ^${branch}$ and cpuload req ^${cpuload}$ and type req ^${imagetype}$ and name req ^${image}$" "name"] != 0} {
		puts "insertimages: dbselect of relation 'images' failed.";
		logmsg saveload "END: insertimages: dbselect of relation 'images' failed.";
		exit 2;
	}
	if {[info exists names] && [llength names] > 0} {
		# tuple exists. don't reinsert, just return ok.
		puts "insertimages: tuple <$branch,$cpuload,$imagetype,$image> exists.";
		return 0;
	}
	# now for the real problem. check if a tuple for the given load
	# and image type exists, but don't query using name. if a tuple exists,
	# then it is from a previous saveload on this same cpuload. the
	# current image type version is assumed to be newer or, at least,
	# preferred. remove the old tuple so reload does not get confused
	# and cause cpy-mem to puke. possibly, with the images tuple deleted
	# but the file still existing, this could leave a file which does not
	# have a cpuload assigned to it. you can audit this situation using
	# the tool audittext. it will list any tar files that are not listed
	# in the images relation under any cpu load. use the -r option of 
	# audittext to remove these orphan files.
	# 
	if {[dbdelete images "branch req ^${branch}$ and cpuload req ^${cpuload}$ and type req ^${imagetype}$" ] != 0} {
		puts "insertimages: dbdelete of relation 'images' failed: $branch,$cpuload,$imagetype";
		logmsg saveload "END: insertimages: dbdelete of relation 'images' failed: $branch,$cpuload,$imagetype";
		exit 2;
	}
	#
	# now insert new tuple with the new data.
	#
	set namevalue(branch) $branch;
	set namevalue(cpuload) $cpuload;
	set namevalue(type) $imagetype;
	set namevalue(name) $image;
	if {[dbinsert images namevalue] != 0} {
		puts "insertimages: dbinsert of relation 'images' failed.";
		logmsg saveload "END: insertimages: dbinsert of relation 'images' failed.";
		exit 2;
	}
	return 0;
}
#
proc insertloads { branch cpuload rootfilesys } {
	if {[dbselect filesys loads "branch req ^${branch}$ and cpuload req ^${cpuload}$ and basedir req ^${rootfilesys}$" "basedir"] != 0} {
		puts "insertloads: dbselect of relation 'loads' failed.";
		logmsg saveload "END: insertloads: dbselect of relation 'loads' failed.";
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
		logmsg saveload "END: insertloads: dbinsert of relation 'loads' failed.";
		exit 2;
	}
	return 0;
}
#
proc createdbentries { branch ilist rootfilesys cpuload } {
	global env;
	global expect_out;
	#
	upvar $ilist images;
	#
	puts "\ncreating database entries ...";
	#
	puts "branch: $branch";
	puts "cpu-load: $cpuload";
	#
	# insert any new images tuples
	#
	foreach imagetype [array names images] {
		set image [file tail $images($imagetype)];
		insertimages $branch $cpuload $imagetype $image;
	}
	#
	# insert any new loads tuple
	#
	insertloads $branch $cpuload $rootfilesys;
}
#
proc sendemail { branch operation status subject { msg "" } } {
	global env;
	global testmode;
	#
	if {$msg == ""} {
		set msg $subject;
	}
	if {$testmode != 0} {
		set status "test";
	}
	set cmd "/usr/bin/ksh $env(LCSTOOLSBIN)/lcssendemail $branch $operation $status \"$subject\" \"$msg\" ";
	# puts "$cmd";
	catch { system "$cmd"; } ignore;
	catch { wait -nowait; } ignore;
	return 0;
}
#
proc saveload { savecountnm } {
	upvar $savecountnm savecount;
	#
	global env;
	global expect_out;
	global spawn_id;
	global getlatest;
	global branch2save;
	global overwrite;
	global debugflag;
	global files2save;
	#
	# get list of supported releases
	#
	if {$branch2save == ""} {
		if {[dbselect branches buildmachines "" "branch"] != 0} {
			puts "saveload: dbselect of relation 'buildmachines' failed.";
			logmsg saveload "END: saveload: dbselect of relation 'buildmachines' failed.";
			exit 2;
		}
		#
		# ask user to choose a release
		#
		set choices $branches;
		set choices [linsert $choices 0 "QUIT"];
		chooseone "Choose a branch: " choices branch;
		if {$branch == "QUIT"} {
			puts "exiting saveload.";
			logmsg saveload "END: saveload NORMAL termination";
			exit 0;
		}
		unset choices;
	} else {
		if {[dbselect branches buildmachines "branch req ^${branch2save}$" "branch"] != 0} {
			puts "saveload: dbselect of relation 'buildmachines' failed.";
			logmsg saveload "END: saveload: dbselect of relation 'buildmachines' failed.";
			exit 2;
		}
		if {![info exists branches]} {
			puts "saveload: branch-to-save $branch2save does not exist in relation 'buildmachines'.";
			logmsg saveload "END: saveload: branch-to-save $branch2save does not exist in relation 'buildmachines'.";
			exit 2;
		}
		set branch [lindex $branches 0]
		if {[string length $branch] <= 0} {
			puts "saveload: branch-to-save $branch2save does not exist in relation 'buildmachines'.";
			logmsg saveload "END: saveload: branch-to-save $branch2save does not exist in relation 'buildmachines'.";
			exit 2;
		}
	}
	#
	global global_branch;
	set global_branch $branch;
	#
	# get files for this release.
	#
	getimages $branch images $debugflag;
	#
	foreach image $images {
		# puts "\nimage tuple: <$image>";
		#
		set data [split $image " \t"];
		set imagetype [lindex $data 0];
		set image [lindex $data 1];
		set imagesize [lindex $data 2];
		#
		# puts "type: <$imagetype>";
		# puts "image: <$image>";
		# puts "size: <$imagesize>";
		#
		lappend imagesbytype($imagetype) $image;
		set imagesizes(${image}) $imagesize;
		# puts "imagesizes\($image\): $imagesizes($image)";
	}
	#
	set imagessaved 0;
	#
	foreach imagetype [array names imagesbytype] {
		puts "\nimagesubtype is <$imagetype>.";
		#
		set prompt "Choose an $imagetype image to save: ";
		lappend choices "QUIT" "NONE";
		# puts "choices are: <${choices}>";
		#
		foreach imagefile $imagesbytype($imagetype) {
			# puts "imagefile is <$imagefile>.";
			lappend choices $imagefile;
		}
		# puts "choices are: <${choices}>";
		#
		set image2save "";
		if {[llength $files2save] > 1} {
			puts "scanning files-to-save list:";
			set image2save "NONE";
			foreach imagefile $imagesbytype($imagetype) {
				set imagefilename [file tail ${imagefile}];
				if {[lsearch -exact ${files2save} ${imagefilename}] != -1} {
					puts "==>> found file-to-save: ${imagefilename}";
					set image2save ${imagefile};
					break;
				}
			}
			if {${image2save} == "NONE"} {
				puts "==>> WARNING: NO file-to-save found for image type: ${imagetype}";
			}
		} elseif {$getlatest == 0} {
			# ask user to choose file.
			chooseone "${prompt}" choices image2save;
		} else {
			# choose the latest file, if it exists.
			# remember, the files are listed newest first.
			set image2save [lindex $choices 2]
			if {![info exists image2save] || \
			    [string length $image2save] <= 0} {
				set image2save "NONE";
			} else {
				puts "==>> defaulting to newest file.";
			}
		}
		if {$image2save == "QUIT"} {
			puts "exiting saveload.";
			logmsg saveload "END: saveload NORMAL termination";
			exit 0;
		}
		if {$image2save != "NONE"} {
			lappend images2save($imagetype) $image2save;
			puts "";
			puts "Will save image:";
			puts "$images2save($imagetype).";
			puts "";
			incr imagessaved +1;
		}
		unset choices;
	}
	if {$imagessaved <= 0} {
		puts "No images saved. Exiting saveload.";
		logmsg saveload "END: No images saved. Exiting saveload.";
		exit 0;
	}
	# puts "PHONY exiting saveload.";
	# logmsg saveload "END: saveload NORMAL termination";
	# exit 0;
	#
	# get swCPU load
	#
	if {(![info exists images2save(cpu)]) || ([llength images2save(cpu)] == 0)} {
		# we need a cpu load to store the data. ask the user.
		getcpuload $branch cpuload;
	} elseif {[regexp -nocase -- "/(\[0-9A-Z\.]*)_cpu\.tar\.gz$" $images2save(cpu) ignore cpuload] != 1 } {
		puts "saveload: unable to get cpu-load.";
		logmsg saveload "END: saveload: unable to get cpu-load.";
		exit 2;
	}
	puts "cpu-load: $cpuload";
	global global_cpuload;
	set global_cpuload $cpuload;
	#
	# we have a cpu load. does this cpu load already exist? if it
	# exists, then we can't save on top of it, unless we were
	# told it was ok. 
	#
	if {${overwrite} == 0} {
		puts "\nsaveload: overwrite option is OFF.";
		logmsg saveload "saveload: overwrite option is OFF.";
		#
		if {[DoesCpuLoadExist $branch $cpuload] != 0} {
			puts "saveload: $cpuload exists; will NOT overwrite.";
			logmsg saveload "END: saveload: $cpuload exists; will NOT overwrite.";
			sendemail $global_branch "saveload" "success" "SAVELOAD: NEW IOMs, but existing CPU load, will NOT overwrite existing IOMs for load $global_branch/$global_cpuload.";
			exit 2;
		} else {
			puts "saveload: $cpuload does NOT exist; will save load";
			logmsg saveload "saveload: $cpuload does NOT exist; will save load";
		}
	} else {
		puts "\nsaveload: overwrite option is ON.";
		logmsg saveload "saveload: overwrite option is ON.";
	}
	#
	set savecount 0;
	logmsg saveload "START: saveimages for $branch.";
	saveimages $branch images2save savecount filesys imagesizes;
	logmsg saveload "END: saveimages - $savecount $branch images saved";
	#
	puts "$savecount images saved; will create DB entries.";
	createdbentries $branch images2save $filesys $cpuload;
}
#
#########################################################################
#
# default values
#
set username "lcstools";
set userpasswd "plexus9000";
set verbose 0;
set ucond 0;
set stracelevel -1;
set removelock 0;
set background 0;
set testmode 0;
set getlatest 0;
set overwrite 0;
set branch2save "";
set defaultstatus "oper";
set debugflag "";
set remotemach "";
set saveoldfiles 0;
set files2save "";
#
global env;
#
set global_branch "";
set global_cpuload "";
set global_deleteremotedir 1;
#
# get cmd line options
#
for {set arg 0} {$arg<$argc} {incr arg} {
	set argval [lindex $argv $arg];
	switch -regexp -- $argval {
	{^-x} { debug -now; }
	{^-T} { set testmode 1; }
	{^-G} { set getlatest 1; }
	{^-O} { set overwrite 1; }
	{^-V} { set verbose 1; }
	{^-B} { set background 1; }
	{^-d} { set debugflag "-d"; }
	{^-U} { set ucond 1; }
	{^-R} { set removelock 1; }
	{^-s} { 
		set saveoldfiles 1;
		set now [clock format [clock seconds] -format "%Y%m%d%H%M%S"];
	}
	{^-S.*} { getoptval $argval stracelevel arg; }
	{^-u.*} { getoptval $argval username arg; }
	{^-p.*} { getoptval $argval userpasswd arg; }
	{^-b.*} { getoptval $argval branch2save arg; }
	{^-t.*} { getoptval $argval defaultstatus arg; }
	{^-r.*} { getoptval $argval remotemach arg; }
	{^-F.*} { getoptval $argval files2save arg; }
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
set logfile "/lcsl100/tmp/saveload.[pid]";
puts "\nSaveload started:\n\nDefault log file is $logfile.\n";
if {[catch {log_file -noappend -a $logfile; } status]} {
	puts "\nWARNING: Logging failed for file $logfile:\n$status";
}
#
checkenv;
#
switch -regexp -- $defaultstatus {
{^oper$} {
	#ok
	set defaultstatus " -t oper ";
}
{^test$} {
	set defaultstatus " -t test ";
	#ok
}
default {
	puts "\nInvalid status value $defaultstatus, only 'oper' and 'test' are allowed.";
	exit 2;
}
}
#
if {[string length "${remotemach}"] > 0} {
	if {[regexp -- {^([^! ]+)!([^! ]+)$} ${remotemach} ignore rmach rdir] == 1} {
		puts "\nRemote machine for DEBUG files  : ${rmach}";
		puts "Remote directory for DEBUG files: ${rdir}\n";
		logmsg saveload "SAVELOAD: remote machine and directory: $rmach, $rdir.";
	} else {
		puts "ERROR: unrecognized format for remote machine and directory.";
		logmsg saveload "END: ERROR: unrecognized format for remote machine and directory.";
		exit 2;
	}
}
#
logusage saveload;
#
set LABID "all";
if {$removelock != 0} {
        puts "WARNING: Forced removal of saveload LOCK file.";
        logmsg saveload "WARNING: Forced removal of saveload LOCK.";
        set status [V saveload $LABID];
        logmsg saveload "WARNING: status of removal of saveload LOCK: $status";
}
#
# was a list of files-to-save given?
#
if {[string length $files2save] > 0} {
	puts "\nList of Files to Save:";
	if {![file readable $files2save]} {
		puts "ERROR: unable to read files2save ${files2save}.";
		logmsg saveload "END: ERROR: unable to read files2save ${files2save}.";
		exit 2;
	}
	set infd [open $files2save "r"];
	unset files2save;
	lappend files2save "JUNK";
	set cpugiven 0;
	while {[gets $infd line] != -1} {
		set tuple [split $line " \t"];
		set tbranch [lindex $tuple 0];
		set tcpuload [lindex $tuple 1];
		set tfiletype [lindex $tuple 2];
		set tfilename [lindex $tuple 3];
		#
		puts "\nBranch   : ${tbranch}";
		puts "CpuLoad  : ${tcpuload}";
		puts "File Type: ${tfiletype}";
		puts "File Name: ${tfilename}";
		#
		if {"${tfiletype}" == "cpu"} {
			set cpugiven 1;
		}
		#
		lappend files2save "${tfilename}";
	}
	catch { close $infd; } ignore;
	#
	if {!${cpugiven}} {
		puts "ERROR: No CPU file included in files2save list.";
		logmsg saveload "END: ERROR: No CPU file included in files2save list.";
		exit 2;
	}
}
#
set status [P saveload $LABID];
if {$status == 0} {
	puts "Another saveload in progress.";
	logmsg saveload "END: another saveload in progress.";
	unset LABID;
	exit 2;
}
#
# return actual number of images saved.
set savecount 0;
#
saveload savecount;
#
if {[string length $global_branch] > 0 && [string length $global_cpuload] > 0} {
	set alllabids "";
	#
	for { } {$arg<$argc} {incr arg} {
		set argval [lindex $argv $arg];
		#
		puts "Enabling $global_branch/$global_cpuload for lab $argval.";
		set cmd "/usr/bin/ksh $env(LCSTOOLSBIN)/lcsconfigure ${defaultstatus} -a $global_branch $global_cpuload $argval";
		puts "\nLCS CONFIGURE CMD: $cmd";
		if {[catch { system "$cmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nlcsconfigure: $errmsg";
				logmsg saveload "ERROR: lcsconfigure: $errmsg";
			} else {
				puts "\nlcsconfigure: unknown error.";
				logmsg saveload "ERROR: lcsconfigure: unknown error.";
			}
		}
		catch { wait -nowait; } ignore;
		#
		puts "Copying $global_branch/$global_cpuload scripts for lab $argval.";
		set cmd "/usr/bin/ksh $env(LCSTOOLSBIN)/lcscopytl1 -b $global_branch $global_branch $global_cpuload $argval";
		puts "\nLCS COPY TL1 CMD: $cmd";
		if {[catch { system "$cmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nlcscopytl1: $errmsg";
				logmsg saveload "ERROR: lcscopytl1: $errmsg";
			} else {
				puts "\nlcscopytl1: unknown error.";
				logmsg saveload "ERROR: lcscopytl1: unknown error.";
			}
		}
		catch { wait -nowait; } ignore;
		# HUG scripts
		set cmd "/usr/bin/ksh $env(LCSTOOLSBIN)/lcshugcopytl1 -b $global_branch $global_branch $global_cpuload $argval";
		puts "\nLCS HUG COPY TL1 CMD: $cmd";
		if {[catch { system "$cmd"; } errmsg] != 0} {
			if {[string length $errmsg] > 0} {
				puts "\nlcscopytl1: $errmsg";
				logmsg saveload "ERROR: lcshugcopytl1: $errmsg";
			} else {
				puts "\nlcscopytl1: unknown error.";
				logmsg saveload "ERROR: lcshugcopytl1: unknown error.";
			}
		}
		catch { wait -nowait; } ignore;
		#
		append alllabids " " $argval;
	}
	#
	logmsg saveload "SAVELOAD: load $global_branch/$global_cpuload saved for reload: $alllabids";
	# 
	if {$savecount > 0} {
		sendemail $global_branch "saveload" "success" "SAVELOAD: load $global_branch/$global_cpuload saved for reload - load status=${defaultstatus}: $alllabids";
	} else {
		sendemail $global_branch "saveload" "success" "SAVELOAD: no new files saved for load $global_branch/$global_cpuload: $alllabids";
	}
}
#
logmsg saveload "END: saveload NORMAL termination";
#
exit 0;

