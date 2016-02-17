# set billing server IP.
#
PATH=~lcstools/tools/bin:~exptools/lib/unity/bin:$PATH
#
if [[ -z "${LABID}" ]] 
then
	echo "LABID is not set." >&2
	exit 2
fi
#
if [[ -z "${LCSTOOLSDATA}" ]] 
then
	echo "LCSTOOLSDATA is not set." >&2
	exit 2
fi
#
if [[ $# == 0 ]]
then
	echo "usage: ${0} bill-system-ip-address [tl1exec options ...]" >&2
	exit 2
elif [[ "${1}" == "-?" ]]
then
	echo "usage: ${0} bill-system-ip-address [tl1exec options ...]" >&2
	exit 0
else
	BTSIP=${1}
	shift 1
fi
#
cd $LCSTOOLSDATA
#
spaip=$(uprintf -q cpu_a_ip in chassis where labid req $LABID| sed -n '1,1p')
echo "SP-A-IP=$spaip"
spbip=$(uprintf -q cpu_b_ip in chassis where labid req $LABID| sed -n '1,1p')
echo "SP-B-IP=$spbip"
#
cat >/tmp/amaon.$$ <<EOF
rtrv-dataserver-hostcfg
set-dataserver-hostcfg:::::dataserveranet1=${BTSIP}
rtrv-dataserver-hostcfg
EOF
#
tl1exec -i "${spaip}" "${@}" /tmp/amaon.$$
#
tl1exec -i "${spbip}" "${@}" /tmp/amaon.$$
#
rm -f /tmp/amaon.$$ 1>/dev/null 2>&1
#
exit 0
