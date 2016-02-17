#!/usr/bin/ksh
#
# make hardare TL1 scripts using data from chassis and ioms relations.
#
###########################################################################
#
export usagecmd=$(basename ${0})
#
usage="\n
for creating scripts: \n
\n
usage: ${usagecmd} [-?x] branch cpuload labid [labid ...] \n
\n
or for listing available load data: \n
\n
usage: ${usagecmd} [-?x] -L [-l labid ... | -c load ... | -b branch ...] \n
\n
or for listing chassis data: \n
\n
usage: ${usagecmd} [-?x] -C [-l labid ... | -b branch ...] \n
\n
or for listing IOM data: \n
\n
usage: ${usagecmd} [-?x] -I [-l labid ... | -b branch ...] \n
\n
or for listing ENET data: \n
\n
usage: ${usagecmd} [-?x] -E [-l labid ... | -b branch ...] \n
\n
or for listing TDM (T1,E1,T3,STS1,OC3,OC12) data: \n
\n
usage: ${usagecmd} [-?x] -T [-l labid ... | -b branch ...] \n
\n
where \n
\t-? - print usage message \n
\t-x - debug \n
\t-L - list load data \n
\t-C - list chassis data \n
\t-I - list IOM data \n
\t-l - filter data by labid \n
\t-c - filter data by cpuload \n
\t-b - filter data by branch \n
\n
"
#
function make_ed_chassis_eqpt {
	typeset labid branch cpuload scriptdir tl1template
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	# get ed-chassis-template for this branch and load
	#
	tl1template=${LCSBASETEMPLATES}/${branch}/ed-chassis-eqpt
	if [[ ! -r "${tl1template}" ]]
	then
		echo
		echo "ED-CHASSIS-EQPT TL1 template ${tl1template} is not readable." 
		exit 2
	fi
	tl1cmd=$(cat ${tl1template})
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating ED-CHASSIS-EQPT for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current Chassis Data:"
	uprint hwchassis where labid req "^${labid}$" and branch req "^${branch}$" 
	#
	uselect -q fieldname fieldvalue from hwchassis where labid req "^${labid}" and branch req "^${branch}" | 
	while read fldnm fldval
	do
		newtl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)${fldnm}=<value>/\1${fldnm}=${fldval}/")
		if [[ "${newtl1cmd}" == "${tl1cmd}" ]]
		then
			# we have a field that was not found. the 
			# tl1 cmd string should always change for 
			# each entry in the hwchassis relation.
			#
			echo 
			echo "WARNING: Field ${fldnm}=${fldval} NOT found in ed-chassis-eqpt template." 
		else
			tl1cmd="${newtl1cmd}"
		fi
		#
	done
	tl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)[^=]*=<value>/\1/g")
	tl1cmd=$(echo $tl1cmd | sed "s/<tid>//; s/<aid>/com/; s/<ctag>//;")
	#
	echo "ED-CHASSIS-EQPT: ${tl1cmd}"
	echo "${tl1cmd}" >${scriptdir}/ed-chassis-eqpt
	#
	return 0
}
#
function make_ent_eqpt {
	typeset labid branch cpuload scriptdir
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating ENT-EQPT for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current IOM Data:"
	#
	uprint hwioms where labid req "^${labid}$" and branch req "^${branch}$" and tl1cmd req "^ent-eqpt$" 
	#
	# read data from hwioms relation and create ent-eqpt commands
	#
	rm -f ${scriptdir}/ent-eqpt 1>/dev/null 2>&1
	#
	uselect -q iom type clei redundancy initialstate threshold1 threshold2 threshold3 from hwioms where labid req "^${labid}" and branch req "^${branch}" and tl1cmd req "^ent-eqpt$" | 
	while read iom type clei redundancy initialstate threshold1 threshold2 threshold3 
	do
		filename=$(uselect -q name from images where branch req "^${branch}$" and cpuload req "^${cpuload}$" and type req "^${type}")
		if [[ -z "${filename}" ]]
		then
			echo 
			echo "unable to get IOM filename for $branch, $cpuload, $type" 
			exit 2
		fi
		#
		swversion=$(echo $filename | sed 's/^\([^_]*\)_.*$/\1/')
		if [[ -z "${swversion}" ]]
		then
			echo 
			echo "unable to get software version for $branch, $cpuload, $type, $filename." 
			exit 2
		fi
		#
		tl1cmd="ent-eqpt::iom-${iom}:::${clei},${redundancy},${threshold1},${threshold2},${threshold3},${swversion}:${initialstate}"
		#
		echo "ENT-EQPT: ${tl1cmd}"
		echo "${tl1cmd}" >>${scriptdir}/ent-eqpt
	done
	#
	return 0
}
#
function make_ent_eqpt_all {
	typeset labid branch cpuload scriptdir
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating ENT-EQPT-ALL for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current IOM Data:"
	#
	uprint hwioms where labid req "^${labid}$" and branch req "^${branch}$" and tl1cmd req "^ent-eqpt-all$" 
	#
	# read data from hwioms relation and create ent-eqpt-all commands
	#
	rm -f ${scriptdir}/ent-eqpt-all 1>/dev/null 2>&1
	#
	uselect -q iom type clei rearmodclei redundancy initialstate threshold1 threshold2 threshold3 from hwioms where labid req "^${labid}" and branch req "^${branch}" and tl1cmd req "^ent-eqpt-all$" | 
	while read iom type clei rearmodclei redundancy initialstate threshold1 threshold2 threshold3 
	do
		filename=$(uselect -q name from images where branch req "^${branch}$" and cpuload req "^${cpuload}$" and type req "^${type}")
		if [[ -z "${filename}" ]]
		then
			echo 
			echo "unable to get IOM filename for $branch, $cpuload, $type" 
			exit 2
		fi
		#
		swversion=$(echo $filename | sed 's/^\([^_]*\)_.*$/\1/')
		if [[ -z "${swversion}" ]]
		then
			echo 
			echo "unable to get software version for $branch, $cpuload, $type, $filename." 
			exit 2
		fi
		#
		# tl1cmd="ent-eqpt-all::iom-${iom}:::${clei},${redundancy},${threshold1},${threshold2},${threshold3},${swversion}:${initialstate}"
		tl1cmd="ent-eqpt-all::iom-${iom}:::iomoduletype=${clei}"
		tl1cmd="${tl1cmd},redundancy=${redundancy}"
		tl1cmd="${tl1cmd},memthreshold=${threshold1}"
		tl1cmd="${tl1cmd},busythreshold=${threshold2}"
		tl1cmd="${tl1cmd},nvmemthreshold=${threshold3}"
		tl1cmd="${tl1cmd},swversion=${swversion}"
		tl1cmd="${tl1cmd},rearmoduletype=${rearmodclei}"
		tl1cmd="${tl1cmd}:${initialstate}"
		#
		echo "ENT-EQPT-ALL: ${tl1cmd}"
		echo "${tl1cmd}" >>${scriptdir}/ent-eqpt-all
	done
	#
	return 0
}
#
function make_cpy_mem {
	typeset labid branch cpuload scriptdir
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating CPY-MEM for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current IOM Data:"
	#
	uprint labid branch iom type in hwioms where labid req "^${labid}$" and branch req "^${branch}$"
	#
	# read data from hwioms relation and create cpy-mem commands
	#
	rm -f ${scriptdir}/cpy-mem 1>/dev/null 2>&1
	#
	uselect -q iom type clei from hwioms where labid req "^${labid}" and branch req "^${branch}" | 
	while read iom type clei
	do
		tl1cmd="cpy-mem::iom-${iom}"
		#
		echo "CPY-MEM: ${tl1cmd}"
		echo "${tl1cmd}" >>${scriptdir}/cpy-mem
	done
	#
	return 0
}
#
function make_ent_enet {
	typeset labid branch cpuload scriptdir tl1template
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	# get ent-enet-template for this branch and load
	#
	tl1template=${LCSBASETEMPLATES}/${branch}/ent-enet
	if [[ ! -r "${tl1template}" ]]
	then
		echo 
		echo "ENT-ENET TL1 template ${tl1template} is not readable." 
		exit 2
	fi
	tl1cmd=$(cat ${tl1template})
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating ENT-ENET for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current Chassis Data:"
	uprint hwenet where labid req "^${labid}$" and branch req "^${branch}$" and tl1cmd req "^ent-enet$" 
	#
	uselect -q fieldname fieldvalue from hwenet where labid req "^${labid}" and branch req "^${branch}" and tl1cmd req "^ent-enet$" | 
	while read fldnm fldval
	do
		newtl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)${fldnm}=<value>/\1${fldnm}=${fldval}/")
		if [[ "${newtl1cmd}" == "${tl1cmd}" ]]
		then
			# we have a field that was not found. the 
			# tl1 cmd string should always change for 
			# each entry in the hwenet relation. try replacing
			# name of the field with the known value, then check
			# again.
			#
			newtl1cmd=$(echo $newtl1cmd | sed "s/<${fldnm}>/${fldval}/")
			if [[ "${newtl1cmd}" == "${tl1cmd}" ]]
			then
				echo 
				echo "WARNING: Field ${fldnm}=${fldval} NOT found in ent-enet template." 
			else
				tl1cmd="${newtl1cmd}"
			fi
		else
			tl1cmd="${newtl1cmd}"
		fi
		#
	done
	tl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)[^=,:]*=<value>/\1/g")
	tl1cmd=$(echo $tl1cmd | sed "s/<tid>//; s/<aid>/com/; s/<ctag>//;")
	#
	echo "ENT-ENET: ${tl1cmd}"
	echo "${tl1cmd}" >${scriptdir}/ent-enet
	#
	return 0
}
#
function make_ent_enet_endptvoip {
	typeset labid branch cpuload scriptdir tl1template
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	scriptdir=${4}
	#
	# get ent-enet-endptvoip-template for this branch and load
	#
	tl1template=${LCSBASETEMPLATES}/${branch}/ent-enet-endptvoip
	if [[ ! -r "${tl1template}" ]]
	then
		echo 
		echo "ENT-ENET-ENDPTVOIP TL1 template ${tl1template} is not readable." 
		exit 2
	fi
	tl1cmd=$(cat ${tl1template})
	#
	cd $LCSTOOLSDATA
	#
	echo
	echo "Creating ENT-ENET_ENDPTVOIP for:"
	echo
	echo "Labid     : ${labid}"
	echo "Branch    : ${branch}"
	echo "CPU Load  : ${cpuload}"
	echo "Script Dir: ${scriptdir}"
	echo
	echo "Current Chassis Data:"
	uprint hwenet where labid req "^${labid}$" and branch req "^${branch}$" and tl1cmd req "^ent-enet-endptvoip$" 
	#
	uselect -q fieldname fieldvalue from hwenet where labid req "^${labid}" and branch req "^${branch}" and tl1cmd req "^ent-enet-endptvoip$" | 
	while read fldnm fldval
	do
		newtl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)${fldnm}=<value>/\1${fldnm}=${fldval}/")
		if [[ "${newtl1cmd}" == "${tl1cmd}" ]]
		then
			# we have a field that was not found. the 
			# tl1 cmd string should always change for 
			# each entry in the hwenet relation. try replacing
			# name of the field with the known value, then check
			# again.
			#
			newtl1cmd=$(echo $newtl1cmd | sed "s/<${fldnm}>/${fldval}/")
			if [[ "${newtl1cmd}" == "${tl1cmd}" ]]
			then
				echo 
				echo "WARNING: Field ${fldnm}=${fldval} NOT found in ent-enet template." 
			else
				tl1cmd="${newtl1cmd}"
			fi
		else
			tl1cmd="${newtl1cmd}"
		fi
		#
	done
	tl1cmd=$(echo $tl1cmd | sed "s/\([,:]\)[^=,:]*=<value>/\1/g")
	tl1cmd=$(echo $tl1cmd | sed "s/<tid>//; s/<aid>/com/; s/<ctag>//;")
	#
	echo "ENT-ENET-ENDPTVOIP: ${tl1cmd}"
	echo "${tl1cmd}" >${scriptdir}/ent-enet-endptvoip
	#
	return 0
}
#
function makehwscripts {
	typeset labid branch cpuload
	#
	labid=${1}
	branch=${2}
	cpuload=${3}
	#
	echo 
	echo "Labid   : ${labid}"
	echo "Branch  : ${branch}"
	echo "CPU Load: ${cpuload}"
	#
	if [[ -z "${labid}" ]]
	then
		echo 
		echo "LABID is unknown." 
		exit 2
	fi
	#
	scriptdir=${LCSHWSCRIPTS}/${branch}/${cpuload}/${labid}
	if [[ ! -d "${scriptdir}" ]]
	then
		echo 
		echo "Creating HW script directory ${scriptdir} ..."
		mkdir -p ${scriptdir}
	else
		echo 
		echo "HW script directory ${scriptdir} ..."
	fi
	#
	make_ed_chassis_eqpt $labid $branch $cpuload $scriptdir
	#
	make_ent_eqpt $labid $branch $cpuload $scriptdir
	#
	make_ent_eqpt_all $labid $branch $cpuload $scriptdir
	#
	make_ent_enet $labid $branch $cpuload $scriptdir
	#
	make_ent_enet_endptvoip $labid $branch $cpuload $scriptdir
	#
	make_cpy_mem $labid $branch $cpuload $scriptdir
	#
	return 0
}
#
function listdata {
	#
	typeset oper field
	oper=${1}
	field=${2}
	#
	if [[ $# -lt 2 ]]
	then
		echo 
		echo "Missing 'oper' and/or 'field' in listdata." 
		exit 2
	fi
	shift 2
	#
	cd $LCSTOOLSDATA
	#
	case "${oper}" in
	loads)
		if [[ $# == 0 ]]
		then
			uprint labloads
			return 0
		fi
		#
		for value in ${*}
		do
			echo 
			echo "uprint labloads where ${field} req \"^${value}$\""
			uprint labloads where ${field} req "^${value}$"
		done
		;;
	chassis)
		if [[ $# == 0 ]]
		then
			uprint labid branch fieldname fieldvalue in hwchassis
			return 0
		fi
		#
		for value in ${*}
		do
			echo 
			echo "uprint labid branch fieldname fieldvalue in hwchassis where ${field} req \"^${value}$\""
			uprint labid branch fieldname fieldvalue in hwchassis where ${field} req "^${value}$"
		done
		;;
	enet)
		if [[ $# == 0 ]]
		then
			uprint labid branch fieldname fieldvalue in hwenet
			return 0
		fi
		#
		for value in ${*}
		do
			echo 
			echo "uprint labid branch fieldname fieldvalue in hwenet where ${field} req \"^${value}$\""
			uprint labid branch fieldname fieldvalue in hwenet where ${field} req "^${value}$"
		done
		;;
	tdm)
		if [[ $# == 0 ]]
		then
			uprint labid branch fieldname fieldvalue in hwtdm
			return 0
		fi
		#
		for value in ${*}
		do
			echo 
			echo "uprint labid branch fieldname fieldvalue in hwtdm where ${field} req \"^${value}$\""
			uprint labid branch fieldname fieldvalue in hwtdm where ${field} req "^${value}$"
		done
		;;
	ioms)
		if [[ $# == 0 ]]
		then
			uprint labid branch iom type clei in hwioms
			return 0
		fi
		#
		for value in ${*}
		do
			echo 
			echo "uprint labid branch iom type clei in hwioms where ${field} req \"^${value}$\""
			uprint labid branch iom type clei in hwioms where ${field} req "^${value}$"
		done
		;;
	*)
		echo 
		echo "Unknown 'oper' in listdata: ${oper}." 
		exit 2
		;;
	esac
	
	#
	#
	#
	return 0
}
#
#######################################################################
#
echo 
echo "Starting mkhwscripts:"
#
oper=scripts
filter=none
#
set -- $(getopt ?TELCIxlbc ${*})
if [[ ${?} -ne 0 ]]
then
	echo $usage 
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
	-b)
		filter=branch
		shift
		;;
	-c)
		filter=cpuload
		shift
		;;
	-l)
		filter=labid
		shift
		;;
	-L)
		oper=loads
		shift
		;;
	-C)
		oper=chassis
		shift
		;;
	-I)
		oper=ioms
		shift
		;;
	-E)
		oper=enet
		shift
		;;
	-T)
		oper=tdm
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ "${oper}" != "scripts" ]]
then
	listdata ${oper} ${filter} ${*}
	exit 0
fi
#
if [[ -z "${LCSHWSCRIPTS}" ]]
then
	echo 
	echo "LCSHWSCRIPTS is not set, run '. setup_lcs'." 
	exit 2
fi
if [[ -z "${LCSBASETEMPLATES}" ]]
then
	echo 
	echo "LCSBASETEMPLATES is not set, run '. setup_lcs'." 
	exit 2
fi
#
# at this point, we are creating scripts. we need at least
# a branch, a load and, at least, one labid.
#
if [[ $# -lt 3 ]]
then
	echo 
	echo "Require branch, load and, at least, one labid." 
	echo $usage 
	exit 2
fi
#
branch=${1}
cpuload=${2}
shift 2
#
if [[ -z "${branch}" ]]
then
	echo 
	echo "BRANCH was not given." 
	echo $usage 
	exit 2
fi
if [[ -z "${cpuload}" ]]
then
	echo 
	echo "CPU load was not given." 
	echo $usage 
	exit 2
fi
#
for labid in ${*}
do
	makehwscripts $labid $branch $cpuload
done
#
exit 0
#
