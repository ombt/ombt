# find any cpu cores
#
basename $(ls -l /Telica/swCPU/CurrRel 2>/dev/null | sed -n 's/^.*-> *\(,*\)$/\1/p') 2>/dev/null 1>/tmp/currel.$$
currrel=$(cat /tmp/currel.$$)
#
basedir=${1:-"/Telica /home1"}
corefiles=/tmp/corefiles.$$
#
find ${basedir} -type f -name core -print >${corefiles}
find ${basedir} -type f -name '*core' -print | grep Telica >>${corefiles}
find ${basedir} -type f -name '*oldCore*' -print | grep Telica >>${corefiles}
#
if [[ ! -s "${corefiles}" ]]
then
	echo
	echo "No CORE files found."
	exit 0
fi
#
cat ${corefiles} |
while read corefile
do
	echo
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "CORE FILE: $corefile"
	case "${corefile}" in
	*_CM*.core)
		app=$(basename ${corefile} | sed -n 's/^\([^_]*\).*$/\1/p')
		;;
	*oldCore*)
		app=$(basename ${corefile} | sed -n 's/^\([^\.]*\).*$/\1/p')
		;;
	*.core)
		app=$(basename ${corefile} | sed -n 's/^\([^\.]*\).*$/\1/p')
		;;
	*)
		echo "SKIPPING CORE FILE: $corefile"
		continue
		;;
	esac
	#
	echo "Application: ${app}"
	case "${app}" in
	signaling_sp2s)
		apppath=/Telica/swCPU/CurrRel/apps/signaling/signaling
		apppath2=/build/lcstools/Telica/swCPU/${currrel}_DEBUG/apps/signaling/signaling
		;;
	*)
		apppath=/Telica/swCPU/CurrRel/apps/${app}/${app}
		apppath2=//build/lcstools/Telica/swCPU/${currrel}_DEBUG/apps/${app}/${app}
		;;
	esac
	#
	if [[ -r "${apppath2}" ]]
	then
		echo "RUNNING GDB ON CORE FILE: $corefile"
		gdb -nw ${apppath2} ${corefile} <<EOF
bt
quit
EOF
	elif [[ -r "${apppath}" ]]
	then
		echo "RUNNING GDB ON CORE FILE: $corefile"
		gdb -nw ${apppath} ${corefile} <<EOF
bt
quit
EOF
	else
		echo "UNABLE TO FIND EXECUTABLE FOR CORE FILE: $corefile"
	fi
done
#
exit 2
