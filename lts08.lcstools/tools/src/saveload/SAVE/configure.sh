#!/usr/bin/ksh
#
# edit parameters for a load.
#
###############################################################################
#
CMD=${0}
#
labids=/tmp/labids$$
loads=/tmp/loads$$
#
usage() {
	echo "usage: $CMD [-x?]"
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
}
#
function add_labid_for_reload {
	typeset PS3
	PS3="Choose labid to add to $branch/$load: "
	#
	uprintf -q -f"%s\n" labid in chassis | 
	sort -u >$labids
	#
	select labid in QUIT $(cat $labids)
	do
		case "${labid}" in
		QUIT)
			echo "Quitting add_labid_for_reload"
			break
			;;
		*)
			echo "Adding load $branch/$load for reload of $labid"
			break
			;;
		esac
	done
	#
	return 0
}
#
function delete_labid_for_reload {
	typeset PS3
	PS3="Choose labid to delete from $branch/$load: "
	#
	uprintf -q -f"%s\n" labid in chassis | 
	sort -u >$labids
	#
	select labid in QUIT $(cat $labids)
	do
		case "${labid}" in
		QUIT)
			echo "Quitting delete_labid_for_reload"
			break
			;;
		*)
			echo "Deleting load $branch/$load for reload of $labid"
			break
			;;
		esac
	done
	#
	return 0
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
echo "Starting LCS Configure Load"
#
if [[ -z "${LCSTOOLSDATA}" ]]
then
	echo
	echo "LCSTOOLSDATA not defined." >&2
	exit 2
fi
#
while [ 1 ]
do
	# available loads to edit
	cd ${LCSTOOLSDATA}
	#
	uprintf -q -f"%s %s\n" branch cpuload in loads | 
	sed 's/[	 ]/__/' |
	sort -u >$loads
	#
	echo
	#
	PS3="Choose branch/load to edit: "
	#
	select branchload in QUIT_CONFIGURE $(cat $loads)
	do
		case "${branchload}" in
		QUIT_CONFIGURE)
			break 2
			;;
		*)
			branchload=$(echo $branchload | sed 's/__/ /')
			set -- ${branchload}
			branch=${1}
			load=${2}
			break
			;;
		esac
	done
	#
	echo 
	echo "Branch/Load to edit: $branch $load"
	echo
	#
	PS3="Choose operation for $branch/$load: "
	#
	select operation in QUIT_CONFIGURE QUIT_OPERATION ADD_LABID_FOR_RELOAD DELETE_LABID_FOR_RELOAD
	do
		case "${operation}" in
		QUIT_CONFIGURE)
			break 2
			;;
		QUIT_OPERATION)
			break
			;;
		ADD_LABID_FOR_RELOAD)
			add_labid_for_reload
			;;
		DELETE_LABID_FOR_RELOAD)
			delete_labid_for_reload
			;;
		*)
			echo "Unknown operation: $operation"
			;;
		esac
		#
		PS3="Choose operation for $branch/$load: "
	done
	#
done
#
rm -rf /tmp/*$$ 1>/dev/null 2>&1
#
exit 0
# remove all tuples referring to this branch and load.
#
grep -v "${branch}	${load}	" loads >$newloads
grep -v "${branch}	${load}	" images >$newimages
grep -v "	${branch}	${load}$" labloads >$newlabloads
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
		cat ${filestoremove} | xargs rm -f 1>/dev/null 2>&1
		# cat ${filestoremove} | sed 's?/text/?/toc/?; s?gz?\.gz.toc?;' | xargs rm -f 1>/dev/null 2>&1
		#
		# back up files before updating
		#
		backupdir="BACKUP/$(date '+%y%m%d%H%M%S')"
		[ ! -d ${backupdir} ] && mkdir -p ${backupdir};
		cp loads images labloads ${backupdir}
		#
		mv $newloads loads
		mv $newimages images
		mv $newlabloads labloads
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
#
rm -rf /tmp/*$$ 1>/dev/null 2>&1
#
exit 0
