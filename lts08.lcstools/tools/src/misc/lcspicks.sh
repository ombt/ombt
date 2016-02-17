#!/usr/bin/ksh
#
if [[ $# == 0 ]]
then
	set -- ${LABID}
elif [[ "${1}" == "-a" ]]
then
	set -- $(lcslabs)
elif [[ "${1}" == "-?" ]]
then
	echo "usage: $(basename ${0}) [ -? | -a | [labid [labid2 ...]] ]"
	exit 0
fi
#
cd ~lcstools/tools/logfiles
#
for labid in ${*}
do
	echo "\n==== LCSRELOAD LOGFILE FOR ${labid} ====\n"
	cgrep -1 "END: reloading ${labid}" reload | tail -2
	#
	echo "\n==== BUPP/LCSBU LOGFILE FOR ${labid} ====\n"
	cgrep -1 "STARTING RELOAD:.*${labid}" buppreload | tail -2
	cgrep -1 "ENDING RELOAD:.*${labid}" buppreload | tail -2
done
#
exit 0
