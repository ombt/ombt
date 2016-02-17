#!/usr/bin/ksh
#
# remove an old load and update relations in a consistent manner.
#
###############################################################################
#
CMD=${0}
#
tmp=/tmp/tmp$$
loads=/tmp/loads$$
newloads=/tmp/newloads$$
newlabloads=/tmp/newlabloads$$
newimages=/tmp/newimages$$
filestoremove=/tmp/filestoremove$$
tocfilestoremove=/tmp/tocfilestoremove$$
remotecmds=/tmp/remotecmds$$
#
trap 'rm -f /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 15
#
usage() {
	echo "usage: $CMD [-x?]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
}
#
set -- $(getopt ?x ${*})
if [[ ${?} -ne 0 ]]
then
	usage
	exit 0
fi
#
for opt in ${*}
do
	case "${opt}" in
	-x)
		set -x
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
echo
echo "Starting LCS Remove Load"
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
# load to delete.
#
while [ 1 ]
do
	cd ${LCSTOOLSDATA}
	#
	uprintf -q -f"%s %s\n" branch cpuload in loads | 
	sed 's/[	 ]/__/' |
	sort -u >$loads
	#
	PS3="Choose branch/load to delete: "
	#
	select branchload in QUIT LCSDF LCSDU SHOWLOCKS $(cat $loads)
	do
		case "${branchload}" in
		QUIT)
			echo "Exit $CMD."
			rm -rf /tmp/*$$ 1>/dev/null 2>&1
			break 2
			# exit 0
			;;
		LCSDF)
			lcsdf
			continue
			;;
		LCSDU)
			lcsdu -t
			continue
			;;
		SHOWLOCKS)
			lcslocks
			continue
			;;
		*)
			branchload=$(echo $branchload | sed 's/__/ /')
			set -- ${branchload}
			branch=${1}
			load=${2}
			break 1
			;;
		esac
	done
	#
	echo "Branch/Load to remove: $branch $load"
	#
	# check if load is locked to prevent delete.
	#
	lcslockadm -P removeload ${load} 2>&1 | grep 'Creating.*failed'
	if [[ $? == 0 ]]
	then
		echo 
		echo "ERROR: Load $branch/$load is locked. Cannot delete."
		echo 
		continue
	fi
	lcslockadm -V removeload ${load} 1>/dev/null 2>&1
	#
	# check if a saveload is in progress.
	#
	lcslockadm -P saveload all 2>&1 | grep 'Creating.*failed'
	if [[ $? == 0 ]]
	then
		echo 
		echo "ERROR: saveload in progress. Cannot delete."
		echo 
		continue
	fi
	#
	# scripts directory
	#
	scriptsdir=${LCSSCRIPTS}/${branch}/${load}
	#
	# remove all tuples referring to this branch and load.
	#
	grep -v "${branch}	${load}	" loads >$newloads
	grep -v "${branch}	${load}	" images >$newimages
	grep -v "	${branch}	${load}	" labloads >$newlabloads
	#
	# now to remove tar files associated with this branch and load
	#
	>$filestoremove
	#
	uprintf -q -f"%s %s %s\n" branch cpuload basedir in loads where branch leq "$branch" and cpuload leq "$load" |
	while read br cpuld bdir
	do
		uprintf -q -f"%s %s %s %s\n" branch cpuload type name in images where branch leq "$br" and cpuload leq "$cpuld" |
		while read br2 cpuld2 type name
		do
			# check if this file is included in another 
			# load. if it is, then do not delete it.
			uprintf -q -f"%s %s %s %s\n" branch cpuload type name in images where branch leq "$br" and type leq "$type" and name leq "$name" and cpuload lne "$load" >${tmp}
			if [[ ! -s "${tmp}" ]]
			then
				# only this load uses this file, remove it
				echo "$bdir/$br/$type/$name" >>$filestoremove
			fi
		done
	done 
	#
	# ask if we commit the changes
	#
	if [[ -s "${filestoremove}" ]]
	then
		echo
		echo "Files to be removed are:"
		cat ${filestoremove}
		#
		echo 
		echo "Remove load? [y/n/cr=n] \c"
		read ans
		#
		case "${ans}" in
		y)
			# remove directories and files
			cat ${filestoremove} | xargs rm -f 1>/dev/null 2>&1
			#
			rm -rf ${scriptsdir} 1>/dev/null 2>&1
			#
			# cat ${filestoremove} | sed 's?/text/?/toc/?; s?gz?\.gz.toc?;' | xargs rm -f 1>/dev/null 2>&1
			#
			# back up files before updating
			#
			# backupdir="BACKUP/$(date '+%y%m%d%H%M%S')"
			# [ ! -d ${backupdir} ] && mkdir -p ${backupdir};
			# cp loads images labloads ${backupdir}
			#
			[ ! -d BACKUP ] && mkdir -p BACKUP ;
			#
			echo $(date '+%y%m%d%H%M%S') >>BACKUP/removeload.logfile
			echo 'diff newloads loads' >>BACKUP/removeload.logfile
			diff $newloads loads >>BACKUP/removeload.logfile
			echo 'diff newimages images' >>BACKUP/removeload.logfile
			diff $newimages images >>BACKUP/removeload.logfile
			echo 'diff newlabloads labloads' >>BACKUP/removeload.logfile
			diff $newlabloads labloads >>BACKUP/removeload.logfile
			#
			mv $newloads loads
			mv $newimages images
			mv $newlabloads labloads
			#
			# check if we have a remote DEBUG load to delete.
			# 
			grep "/debug_cpu/" ${filestoremove} 2>/dev/null 1>&2
			if [[ $? -eq  0 ]]
			then
				echo 
				echo "Remove remote debug file ..."
				# echo "rm -rf /build/lcstools/Telica/swCPU/${load}_DEBUG" >${remotecmds}
				echo "rm -rf /build/lcstools/${load}" >${remotecmds}
				telnetsp -A -u lcstools -p plexus9000 -F ${remotecmds} lcsbld1
			else
				echo
				echo "No remote debug files to remove ..."
			fi
			#
			echo
			echo "Load removed."
			echo "Dead. Buried. Gone."
			;;
		*)
			echo
			echo "Nothing removed."
			;;
		esac
	fi
	# release lock
	lcslockadm -V saveload all 1>/dev/null 2>&1
	#
done
#
rm -rf /tmp/*$$ 1>/dev/null 2>&1
#
exit 0
