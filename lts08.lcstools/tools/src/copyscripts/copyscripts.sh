#!/usr/bin/ksh
# copy scripts from basescripts directory to scripts directory for 
# provisioning.
#
#########################################################################
#
export usagecmd=$(basename ${0})
#
makebbb ( ) {
	echo "Making BurnBabyBurn version of scripts"
	return 0
}
#
usage="\n
usage: ${usagecmd} [-?x] [-a] [-B] [labid [labid ...]]\n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-a - ask where to copy to \n
\t-B - create BurnBabyBurn version of scripts \n
\n
"
#
ask=no
bbb=no
#
set -- $(getopt ?Bxa ${*})
if [[ ${?} -ne 0 ]]
then
	echo $usage >&2
	exit 2
fi
#
for i in ${*}
do
	case "${i}" in
	-x)
		set -x
		shift
		;;
	-a)
		ask=yes
		shift
		;;
	-B)
		bbb=yes
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
CMD=${0}
cd ${LCSTOOLSDATA}
#
echo
echo "'From' Lab and Release data from ${LCSTOOLSDATA}/chassis file."
echo "'To' Lab, Release, Load data from ${LCSTOOLSDATA}/labloads file."
echo
#
cat chassis |
cut -d'	' -f1 |
sort -u >/tmp/fromlabids$$
#
PS3="Choose a Lab to copy base scripts from: "
#
if [[ $# == 0 ]]
then
	select fromlabids in QUIT $(cat /tmp/fromlabids$$)
	do
		case "${fromlabids}" in
		QUIT)
			echo "Quitting ${CMD}." >&2
			rm -f /tmp/*$$ 1>/dev/null 2>&1
			exit 0
			;;
		*)
			export fromlabids
			break
			;;
		esac
	done
else
	export fromlabids=${*}
fi
#
for fromlabid in ${fromlabids}
do
	# reset to data directory.
	cd ${LCSTOOLSDATA}
	#
	cat chassis |
	grep "^${fromlabid}	" |
	cut -d'	' -f2 |
	sort -u >/tmp/fromreleases$$
	#
	PS3="Choose a release for lab ${fromlabid} to copy base scripts from: "
	#
	select fromrelease in QUIT $(cat /tmp/fromreleases$$)
	do
		case "${fromrelease}" in
		QUIT)
			echo "Quitting ${CMD}." >&2
				rm -f /tmp/*$$ 1>/dev/null 2>&1
			exit 0
			;;
		*)
			export fromrelease
			break 
			;;
		esac
	done
	#
	# cat chassis |
	# cut -d'	' -f1,2 |
		# sed 's/[ 	]/__/' |
	# sort -u >/tmp/fromlabrelease$$
	# #
	# PS3="Choose a Lab and Release to copy base scripts from: "
	# #
	# select fromlabrelease in QUIT $(cat /tmp/fromlabrelease$$)
	# do
		# case "${fromlabrelease}" in
		# QUIT)
			# echo "Quitting ${CMD}." >&2
			# rm -f /tmp/*$$ 1>/dev/null 2>&1
			# exit 0
			# ;;
			# *)
		# fromlabrelease=$(echo ${fromlabrelease} | sed 's/__/ /')
			# break 2
			# ;;
		# esac
	# done
	#
		# set -- ${fromlabrelease}
	# fromlabid=${1}
	# fromrelease=${2}
	#
	echo
	echo "Copying from ${fromlabid}/${fromrelease} ..."
	echo
	#
	if [[ "${ask}" == yes ]]
	then
		cat labloads |
		cut -d'	' -f1 |
		sort -u >/tmp/tolab$$
		#
		PS3="Choose a Lab to copy base scripts To: "
		#
		select tolabid in QUIT $(cat /tmp/tolab$$)
		do
			case "${tolabid}" in
			QUIT)
				echo "Quitting ${CMD}." >&2
				exit 0
				;;
			*)
				break
				;;
			esac
		done
	else
		tolabid=${fromlabid}
	fi
	#
	grep "	${fromrelease}	" labloads >/tmp/loads$$
	if [[ ! -s "/tmp/loads$$" ]]
	then
		echo
		echo "WARNING: No $fromrelease loads found for lab ${tolabid}."
		echo
		sleep 2
	fi
	#
	cat labloads |
	grep "^${tolabid}	" | 
	cut -d'	' -f2,3 |
	sed 's/[ 	]/__/' |
	sort -u >/tmp/toreleaseload$$
	#
	PS3="Choose a ${tolabid} Release and Load to copy base scripts To: "
	#
	select toreleaseload in QUIT $(cat /tmp/toreleaseload$$)
	do
		case "${toreleaseload}" in
		QUIT)
			echo "Quitting ${CMD}." >&2
			rm -f /tmp/*$$ 1>/dev/null 2>&1
			exit 0
			;;
		*)
			toreleaseload=$(echo ${toreleaseload} | sed 's/__/ /')
			break
			;;
		esac
	done
	#
	set -- ${toreleaseload}
	torelease=${1}
	toload=${2}
	#
	# echo
	# echo "Copying to ${tolabid}/${torelease}/${toload} ..."
	# echo
	#
	echo
	echo "Base Scripts From : ${fromlabid}/${fromrelease}"
	echo "Copying Scripts To: ${torelease}/${toload}/${tolabid}"
	echo
	#
	from="${LCSBASESCRIPTS}/${fromlabid}/${fromrelease}/current"
	to="${LCSSCRIPTS}/${torelease}/${toload}/${tolabid}"
	#
	if [[ ! -d "${from}" ]]
	then
		echo "'From' directory ${from} does not exist." >&2
		# echo "${CMD} done."
		# rm -f /tmp/*$$ 1>/dev/null 2>&1
		# exit 2
		continue
	fi
	if [[ ! -d "${to}" ]]
	then
		echo >&2
		echo "'To' directory ${to} does not exist." >&2
		echo "Creating 'To' directory ${to}." >&2
		echo >&2
		mkdir -p ${to}
		#
		# echo "Do you want to create it? [y/n/cr=n] \c"
		# read YESNO
		# case "${YESNO}" in
		# y)
			# mkdir -p ${to}
			# ;;
		# *)
			# echo "${CMD} done."
			# rm -f /tmp/*$$ 1>/dev/null 2>&1
			# exit 2
			# ;;
		# esac
	fi
	#
	# check that scripts exist.
	#
	ls ${from}/* 2>/dev/null 1>/tmp/scripts$$
	if [[ ! -s "/tmp/scripts$$" ]]
	then
		echo "'From' directory ${from} is empty." >&2
		echo "${CMD} done."
		rm -f /tmp/*$$ 1>/dev/null 2>&1
		exit 2
	fi
	#
	echo "Exec'ing cp ${from}/* ${to} ..."
	cp ${from}/* ${to} 2>/dev/null
	#
	# copy common files.
	#
	echo "Exec'ing cp ${LCSBASESCRIPTS}/commonfiles/* ${to} ..."
	cp ${LCSBASESCRIPTS}/commonfiles/* ${to} 2>/dev/null
	#
	# update the hardware scripts
	#
	uprintf -q -f"<%s> %s\n" type name in images where branch req "^${torelease}$" and cpuload req "^${toload}$" |
	sed 's/ \([^_][^_]*\)_.*$/ \1/' |
	sed 's/\([0-9][0-9]*.P[0-9][0-9]*\)\.[0-9][0-9]*/\1/' |
	sed 's/\([0-9][0-9]*.SP\.[0-9][0-9]*\)\.[0-9][0-9]*/\1/' |
	sed 's?^?s/?; s? ?/?; s?$?/;?;' |
	tee /tmp/sedcmds$$
	#
	cd ${to}
	#
	for script in *
	do
		sed -f /tmp/sedcmds$$ ${script} >/tmp/new${script}
		mv /tmp/new${script} ${script}
		chmod 644 ${script}
	done
	#
	# distributescripts ${to}
	if [[ "${bbb}" == yes ]]
	then
		makebbb
	fi
done
#
echo "${CMD} done."
#
rm -f /tmp/*$$ 1>/dev/null 2>&1
exit 0

