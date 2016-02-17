#!/usr/bin/ksh
#
if  [[ "${1}" == "-?" ]]
then
	echo "usage: getloads [branch [branch ...]]"
	exit 0
fi
#
rm -f /autosaveload/queue/*
#
logdir="/autosaveload/logs/$(date '+%y%m%d%H%M')"
#
[[ ! -d "${logdir}}" ]] && mkdir -p ${logdir};
#
cd ${logdir}
#
>requiredfiles
#
if [[ $# == 0 ]]
then
	set -- Main BP-6-2-0-3 BP-6-2-0-2 BP-6-2-0-1 BP-6-2-1-3 BP-6-2-1-2 BP-6-2-1-1 BP-5-3-0-6 BP-5-3-0-5 BP-5-3-0-4 BP-5-1-2-3 BP-5-1-2-2 BP-6-3-0-1 BRANCH-DEV-6-4 BRANCH-6-1-0 BRANCH-6-1-1 BRANCH-6-1-2 BRANCH-6-1-3 BRANCH-6-2-0 BRANCH-6-2-1 BRANCH-6-3-0 BP-6-3-0-2
fi
#
for branch in ${*}
do
	/autosaveload/bin/autosaveload -r requiredfiles -b /telica/repository -n 15 ${branch} 2>&1 | tee ${branch}.out
done | tee getloads.out
#
exit 0

