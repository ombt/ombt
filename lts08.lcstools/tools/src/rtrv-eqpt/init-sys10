#!/usr/bin/ksh
#
labid=${1:-${LABID}}
#
case "${labid}" in
*wg[0-9]*)
	labid="$(echo ${labid} | sed 's/wg\([0-9]\)/wag\1/')"
	;;
*g[0-9]*)
	labid="$(echo ${labid} | sed 's/g\([0-9]\)/gw\1/')"
	;;
*)
	;;
esac
#
exec tl1cmd "init-sys:${labid}::::10"
