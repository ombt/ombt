#!/usr/bin/ksh
#
# create HUG nodes
#
#########################################################################
#
CMD=${0}
PATH=$PATH:/opt/exp/lib/unity/bin
#
trap 'rm -rf /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 3 4 5 6 15
#
usage() {
cat <<EOF
usage: ${CMD} [-?] [-x] [branch [branch ...]]

where: 
	-x - debug 
	-? - usage message 

EOF
}
#
set -- `getopt ?x ${*}`
if [[ ${?} -ne 0 ]]
then
	echo "\nERROR: invalid option" >&2
	usage >&2
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
	--)
		shift
		break
		;;
	esac
done
#
if [[ $# == 0 ]]
then
	uselect -q from ${LCSHUGDATA}/hugbase where type req ^scripts$ >/tmp/bases$$
else
	for branch in ${*}
	do
		uselect -q from ${LCSHUGDATA}/hugbase where tree req ^${branch}$ and type req ^scripts$
	done >/tmp/bases$$
fi
#
if [[ ! -s "/tmp/bases$$" ]]
then
	echo "\nERROR: no HUG base directories found." >&2
	exit 2
fi
#
cat /tmp/bases$$ |
while read branch type base
do
	echo 
	echo "Processing ..."
	echo "Branch: ${branch}"
	echo "Base  : ${base}"
	#
	uselect -q subtypes from ${LCSHUGDATA}/hugtypes where branch req ^${branch}$ and type req ^chassistypes$ >/tmp/chassistypes$$
	#
	if [[ ! -s "/tmp/chassistypes$$" ]]
	then
		echo "\nWARNING: no chassis types found for ${branch}. Skipping." >&2
		continue
	fi
	#
	uselect -q subtypes from ${LCSHUGDATA}/hugtypes where branch req ^${branch}$ and type req ^hugsections$ >/tmp/hugsections$$
	#
	if [[ ! -s "/tmp/hugsections$$" ]]
	then
		echo "\nWARNING: no HUG sections found for ${branch}. Skipping." >&2
		continue
	fi
	#
	uselect -q subtypes from ${LCSHUGDATA}/hugtypes where branch req ^${branch}$ and type req ^iomtypes$ >/tmp/iomtypes$$
	#
	if [[ ! -s "/tmp/iomtypes$$" ]]
	then
		echo "\nWARNING: no IOM types found for ${branch}. Skipping." >&2
		continue
	fi
	#
	uselect -q subtypes from ${LCSHUGDATA}/hugtypes where branch req ^${branch}$ and type req ^sptypes$ >/tmp/sptypes$$
	#
	if [[ ! -s "/tmp/sptypes$$" ]]
	then
		echo "\nWARNING: no SP types found for ${branch}. Skipping." >&2
		continue
	fi
	#
	cat /tmp/chassistypes$$ |
	tr ' ' '\012' |
	while read chassistype
	do
		cat /tmp/hugsections$$ |
		tr ' ' '\012' |
		while read hugsection
		do
			if [[ "${hugsection}" == "iomhug" ]]
			then
				cat /tmp/iomtypes$$ |
				tr ' ' '\012' |
				while read iomtype
				do
					dir=${base}/${chassistype}/${hugsection}/${iomtype}
					echo "Directory: ${dir}"
					[ ! -d "${dir}" ] && mkdir -p ${dir}
				done
			elif [[ "${hugsection}" == "sphug" ]]
			then
				cat /tmp/sptypes$$ |
				tr ' ' '\012' |
				while read sptype
				do
					dir=${base}/${chassistype}/${hugsection}/${sptype}
					echo "Directory: ${dir}"
					[ ! -d "${dir}" ] && mkdir -p ${dir}
				done
			else
				dir=${base}/${chassistype}/${hugsection}
				echo "Directory: ${dir}"
				[ ! -d "${dir}" ] && mkdir -p ${dir}
			fi
		done
	done
done
#
exit 0
