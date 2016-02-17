#!/usr/bin/ksh
addresses="${1} mrumore@alcatel-lucent.com lcstools@lts08.ih.lucent.com"
labid="${2}"
shift 2
msg="${*}"
#
if [[ -z "${addresses}" ]]
then
	exit 0
else
	addresses="$(echo ${addresses} | tr ',' ' ')"
fi
if [[ -z "${labid}" ]]
then
	exit 0
fi
if [[ -z "${msg}" ]]
then
	exit 0
fi
#
echo "${msg}" | /usr/bin/mailx -s "FINAL ${labid} RELOAD STATUS:" $addresses
exit 0
