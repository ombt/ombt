#
# copy scripts to individual directories for reload.
#
#############################################################################
#
cmd=${0}
#
if [[ -z "${1}" ]]
then
	echo "No directory given. Exit."
	exit 2
elif [[ ! -d "${1}" ]]
then
	echo "Path ${1} is not a directory or is not readable. Exit."
	exit 2
fi
#
cd ${1}
#
currdir=$(pwd)
distdir=$(echo $currdir | sed 's?/scripts/?/distscripts/?')
#
set -- $(echo ${distdir} | sed 's?/? ?g;')
branch=${3}
cpuload=${4}
labid=${5}
#
echo
echo "Distribute Scripts for:"
echo
echo "Labid  : $labid"
echo "Branch : $branch"
echo "Cpuload: $cpuload"
echo
echo "Distributing scripts:"
echo
echo "From: ${currdir}"
echo "To  : ${distdir}"
#
if [[ -d "${distdir}" ]]
then
	echo
	echo "Removing old directory ${distdir}."
	rm -rf ${distdir}
fi
#
echo
echo "Creating new directory ${distdir}."
mkdir -p ${distdir}
#
# copy scripts to new directory and work from there.
#
echo
echo "Copy scripts to ${distdir}"
#
filecopy=no
#
for script in users *hardware* *calldata* *iproute* *linefeat*
do
	if [[ -f "${script}" ]]
	then
		echo
		echo "Copying ${script} to ${distdir}."
		cp ${script} ${distdir}
		filecopy=yes
	fi
done
#
if [[ "${filecopy}" == no ]]
then
	echo
	echo "No files copied to ${distdir}. Exit."
	exit 2
fi
#
cd ${distdir}
#
# get chassis type
#
chassistype=$(grep -i chassis-eqpt * | grep -v '^[	 ]*#' | tr '[A-Z]' '[a-z]' | sed 's/^.*chassistype=\([^,]*\),.*$/\1/' | tail -1)
#
if [[ -z "${chassistype}" ]]
then
	echo
	echo "Unable to determine chassis type."
	exit 2
fi
#
case "${chassistype}" in
both)
	prefix=
	;;
sg)
	prefix=sg
	;;
mg)
	prefix=mg
	;;
mgc)
	prefix=mgc
	;;
sg_mgc)
	prefix=mgcsg
	;;
sg_mgc_mg)
	prefix=
	;;
*)
	echo
	echo "Unknown chassis type: <$chassistype>"
	exit 2
esac
#
# use the official users file.
#
users=$LCSBASESCRIPTS/commonfiles/users
#
if [[ ! -f users ]]
then
	users=
fi
#
# get hardware scripts.
#
for hfile in *hardware*
do
	
done
#
# get list of IOMs
#
iomlist=$(grep -i ent-eqpt * | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:\(iom-[0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
#
if [[ -z "${iomlist}" ]]
then
	echo
	echo "Unable to determine list of IOMs."
	exit 2
fi
#
echo
echo "Chassis type: $chassistype"
echo "Prefix      : $prefix"
echo "IOM list    : $iomlist"
#
exit 0









for script in *
do
	if [[ -d "${script}" ]]
	then 
		echo
		echo "Skipping directory ${f} ..."
	fi
	#
	case "${script}" in
	*calldata*)
		echo
		echo "Call Data script: $script"
		grep -i '^[^#]*init-sys:' $script 1>/dev/null
		if [[ ${?} == 0 ]]
		then
			echo
			echo "Script $script contains an init-sys cmd."
		fi
		;;
	*hardware*)
		echo
		echo "Hardware script: $script"
		grep -i '^[^#]*init-sys:' $script 1>/dev/null
		if [[ ${?} == 0 ]]
		then
			echo
			echo "Script $script contains an init-sys cmd."
		fi
		;;
	*prov_linefeats*)
		echo
		echo "Provision Line Features script: $script"
		grep -i '^[^#]*init-sys:' $script 1>/dev/null
		if [[ ${?} == 0 ]]
		then
			echo
			echo "Script $script contains an init-sys cmd."
		fi
		;;
	*iproute*)
		echo
		echo "IP Route script: $script"
		grep -i '^[^#]*init-sys:' $script 1>/dev/null
		if [[ ${?} == 0 ]]
		then
			echo
			echo "Script $script contains an init-sys cmd."
		fi
		;;
	users)
		echo
		echo "Users script: $script"
		;;
	*cpymem*|*copymem*)
		echo
		echo "Copy Mem script: $script"
		;;
	*)
		echo
		echo "Unknown type of script: $script"
		;;
	esac
done
#
exit 0
