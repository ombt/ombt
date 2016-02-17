#
# tool to grow/degrow cards based on the info in the official sysconf
# files or in backup versions. 
#
############################################################################
#
# local variables
#
sysconf="/var/opt/LSS/sysconf"
cpus="/tmp/cpus$$"
cards="/tmp/cards$$"
HEXP='[0-9a-f][0-9a-f]'
MACRE="${HEXP}:${HEXP}:${HEXP}:${HEXP}:${HEXP}:${HEXP}"
#
# usage message
#
usage() {
	cat >&2 <<EOF
usage: ${0} [-x] [-s directory] 

where:
	-x - enable debug mode.
	-s - sysconf directory, default is /var/opt/LSS/sysconf.
EOF
}
#
# miscellaneous functions
#
checkdir() {
	path=${1}
	#
	if [[ -d "${path}" ]]
	then
		return 0
	else
		echo "${path} is not a directory ..."
		return 2
	fi
}
#
checkfiles() {
	notfound=
	#
	for f in ${*}
	do
		if [[ ! -r "${f}" ]]
		then
			notfound="${notfound} ${f}"
		fi
	done
	#
	if [[ -z "${notfound}" ]]
	then
		return 0
	else
		echo "files not found: ${notfound} ..."
		return 2
	fi
}
#
checkreturn() {
	status=${1}
	if [[ "${status}" == 0 ]]
	then
		return 0
	else
		echo "FAILED, return code is ${status}."
		echo "continue [y/n/cr=n] ?"
		read ans
		case "${ans}" in
		y*)
			return 0
			;;
		*)
			return 2
		esac
	fi
	return 2
}
#
IsMacOK() {
	macaddr=${1}
	macaddr=$(echo "${macaddr}" | sed -n 's/^\('${MACRE}'\)$/\1/p' )
	if [[ -n "${macaddr}" ]]
	then
		return 0
	else
		echo "invalid MAC address form."
		return 2
	fi
}
#
# perform a backup before doing operations
#
backupDHCP() {
	backupdirprefix=${1}
	#
	app=$(uname -n | sed 's/mi/app/; s/-[0-9]*$/-/')
	#
	for mach in 1 2
	do
		appmach="${app}${mach}"
		ping ${appmach}
		if [[ $? != 0 ]]
		then
			echo "unable to ping ${appmach} ..."
			return 2
		fi
		#
		backupdir=${backupdirprefix}/${appmach}
		[[ ! -d "${backupdir}" ]] && mkdir -p ${backupdir}
		#
		echo "rcp -r root@${appmach}:/var/dhcp ${backupdir}"
		rcp -r root@${appmach}:/var/dhcp ${backupdir}
		if [[ $? != 0 ]]
		then
			echo "rcp from ${appmach} failed ..."
			return 2
		fi
	done
	#
	return 0
}
#
backupPKGCONF() {
	backupdirprefix=${1}
	mi=$(uname -n )
	backupdir=${backupdirprefix}/${mi}/pkgconf
	[[ ! -d "${backupdir}" ]] && mkdir -p ${backupdir}
	echo "cp /etc/pkgconf/* ${backupdir}"
	cp /etc/pkgconf/* ${backupdir}
	return 0
}
#
backupSYSCONF() {
	backupdirprefix=${1}
	mi=$(uname -n )
	backupdir=${backupdirprefix}/${mi}/sysconf
	[[ ! -d "${backupdir}" ]] && mkdir -p ${backupdir}
	echo "cp /var/opt/LSS/sysconf/* ${backupdir}"
	cp /var/opt/LSS/sysconf/* ${backupdir}
	return 0
}
#
# operations
#
dhcp_delete_all() {
	echo "executing dhcp_delete_all  ..."
	echo "this cmd will will delete ALL cards from both APPL hosts."
	echo "do you wish to continue? [yes/no/cr=no] \c"
	read ans
	case "${ans}" in
	y*)
		;;
	*)
		echo "aborting dhcp_delete_all ..."
		return 2
		;;
	esac
	#
	echo "calling ... dhcpconf_adm --action del_all --hostside 1"
	dhcpconf_adm --action del_all --hostside 1
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... dhcpconf_adm --action del_all --hostside 2"
	dhcpconf_adm --action del_all --hostside 2
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "success, exiting dhcp_delete_all ..."
	return 0
}
#
dhcp_add_all() {
	echo "executing dhcp_add_all  ..."
	echo "this cmd will will add ALL cards to both APPL hosts."
	echo "do you wish to continue? [yes/no/cr=no] \c"
	read ans
	case "${ans}" in
	y*)
		;;
	*)
		echo "aborting dhcp_add_all ..."
		return 2
		;;
	esac
	#
	echo "calling ... dhcpconf_adm --action add_all --hostside 1"
	dhcpconf_adm --action add_all --hostside 1
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... dhcpconf_adm --action add_all --hostside 2"
	dhcpconf_adm --action add_all --hostside 2
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "success, exiting dhcp_add_all ..."
	return 0
}
#
update_personality_files () {
	echo "executing update_personality_files  ..."
	#
	echo "calling ... hostconf_adm --action generate_all"
	hostconf_adm --action generate_all
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... miconf_adm --action install_hostconf"
	miconf_adm --action install_hostconf
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... miconf_adm --action generate_pkgmap"
	miconf_adm --action generate_pkgmap
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... hostconf_adm --action install_file --hostfile lss_ips"
	hostconf_adm --action install_file --hostfile lss_ips
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "calling ... hostconf_adm --action install_file --hostfile hosts"
	hostconf_adm --action install_file --hostfile hosts
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "Run 'hostconf_adm --action install_file --hostfile pkgmap' ???"
	read ans
	case "${ans}" in
	y*)
		echo "calling ... hostconf_adm --action install_file --hostfile pkgmap"
		hostconf_adm --action install_file --hostfile pkgmap
		checkreturn ${?}
		[[ ${?} != 0 ]] && return 2 
		;;
	esac
	#
	echo "success, exiting update_personality_files ..."
	return 0
}
#
delete_card() {
	echo "executing delete_card ..."
	# verify sysconf directory and required files exist.
	checkdir ${sysconf}
	if [[ ${?} != 0 ]]
	then
		echo "'checkdir' failed, exiting delete_card ..."
		return 2
	fi
	#
	checkfiles ${sysconf}/card.conf ${sysconf}/macaddr.conf ${sysconf}/ipaddr.conf
	if [[ ${?} != 0 ]]
	then
		echo "'checkfiles' failed, exiting delete_card ..."
		return 2
	fi
	# required files
	cardconf="${sysconf}/card.conf"
	macaddrconf="${sysconf}/macaddr.conf"
	ipaddrconf="${sysconf}/ipaddr.conf"
	# select a shelf
	shelfs=$(cat ${cardconf} | grep ^cardtype | cut -d';' -f2 | sort -u )
	set -- ${shelfs}
	#
	if [[ "${#}" == 1 ]]
	then
		shelf=${shelfs}
		echo "only shelf ${shelf} exists ..."
	else
		PS3="choose a shelf: "
		select shelf in QUIT ${shelfs} 
		do
			[[ -n "${shelf}" ]] && break
			REPLY=
		done
		if [[ "${shelf}" == QUIT ]] 
		then
			echo "QUITING delete_card ..."
			return 2
		fi
		echo "using shelf ${shelf} ..."
	fi
	# get cpu and cards for given shelf
	cat ${cardconf} | grep ^cardtype | cut -d';' -f2,3,4,5 | 
	grep "^${shelf};" | cut -d';' -f2,3,4 | grep "^cpu;" |
	cut -d';' -f2,3 | sort -t';' -n +0 -1 | paste -d";" - - >${cpus}
	#
	cat ${cardconf} | grep ^cardtype | cut -d';' -f2,3,4,5 | 
	grep "^${shelf};" | cut -d';' -f2,3,4 | grep -v "^cpu;" |
	cut -d';' -f2,3 | sort -t';' -n +0 -1 | paste -d";" - - >${cards}
	# select what to delete
	PS3="select card pair to delete: "
	select pair in QUIT $(cat ${cpus}) $(cat ${cards}) 
	do
		[[ -n "${pair}" ]] && break
		REPLY=
	done
	if [[ "${pair}" == QUIT ]] 
	then
		echo "QUITING delete_card ..."
		return 2
	fi
	echo "pair of cards to delete ... ${pair}"
	echo "\n\nARE YOU SURE? [yes/no/cr=no] \c"
	read ans
	case "${ans}" in
	y*)
		;;
	*)
		echo "QUITING deleting_card ..."
		return 2
		;;
	esac
	# generate the command to delete cards
	slot1=$(echo ${pair} | cut -d';' -f1)
	type1=$(echo ${pair} | cut -d';' -f2)
	slot2=$(echo ${pair} | cut -d';' -f3)
	type2=$(echo ${pair} | cut -d';' -f4)
	#
	dhcpconf_adm --action del_card --cardtype ${type1} --shelfnum ${shelf} --slot ${slot1} --hostside 1
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	dhcpconf_adm --action del_card --cardtype ${type2} --shelfnum ${shelf} --slot ${slot2} --hostside 1
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	dhcpconf_adm --action del_card --cardtype ${type1} --shelfnum ${shelf} --slot ${slot1} --hostside 2
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	dhcpconf_adm --action del_card --cardtype ${type2} --shelfnum ${shelf} --slot ${slot2} --hostside 2
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	netconf_adm --action del_routable_card --cardtype ${type1} --shelfnum ${shelf} --slot ${slot1} --mateshelf ${shelf} --mateslot ${slot2}
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	macaddr_adm --action del_card --cardtype ${type1} --shelfnum ${shelf} --slot ${slot1}
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	macaddr_adm --action del_card --cardtype ${type2} --shelfnum ${shelf} --slot ${slot2}
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2
	#
	hwconf_adm --action del_card --cardtype ${type1} --shelfnum ${shelf} --slot ${slot1} --mateshelf ${shelf} --mateslot ${slot2}
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	echo "success, exiting delete_card ..."
	return 0
	#
}
#
insert_card() {
	echo "executing insert_card ..."
	#
	PS3="select a shelf: "
	select shelf in 1 2
	do
		[[ -n "${shelf}" ]] && break
		REPLY=
	done
	#
	PS3="select a card type: "
	select cardtype in cpu scs isdn h248 sip 
	do
		if [[ -n "${cardtype}" ]] 
		then
			if [[ "${cardtype}" == cpu && "${shelf}" != 2 ]]
			then
				echo "shelf must be 2 for cpu."
			else
				break
			fi
		fi
		REPLY=
	done
	#
	if [[ "${cardtype}" == cpu ]]
	then
		slot1=1
		echo "choosing slot 1 for CPU 1."
	else
		PS3="select a slot for first card: "
		select slot1 in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
		do
			[[ -n "${slot1}" ]] && break
			REPLY=
		done
	fi
	#
	while [ 1 ]
	do
		echo "enter MAC address for ${cardtype} in slot ${slot1}: "
		echo "MAC address format: XX:XX:XX:XX:XX:XX"
		read mac1
		mac1=$(echo "${mac1}" | tr '[A-Z]' '[a-z]')
		IsMacOK ${mac1}
		[[ $? == 0 ]] && break
	done
	#
	if [[ "${cardtype}" == cpu ]]
	then
		slot2=2
		echo "choosing slot 2 for CPU 2."
	else
		#
		PS3="select a slot for second card, first card slot is $slot1: "
		select slot2 in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
		do
			if [[ -n "${slot2}" ]] 
			then
				if [[ "${slot1}" == "${slot2}" ]] 
				then
					echo "slot1 cannot equal slot2."
				else
					break
				fi
			fi
			REPLY=
		done
	fi
	#
	while [ 1 ]
	do
		echo "enter MAC address for ${cardtype} in slot ${slot2}: "
		echo "MAC address format: XX:XX:XX:XX:XX:XX"
		read mac2
		mac2=$(echo "${mac2}" | tr '[A-Z]' '[a-z]')
		IsMacOK ${mac2}
		[[ $? == 0 ]] && break
	done
	#
	echo "inserting cards ... "
	echo "(shelf,slot,type,mac)=($shelf,$slot1,$cardtype,$mac1)"
	echo "(shelf,slot,type,mac)=($shelf,$slot2,$cardtype,$mac2)"
	ans=
	echo "\n\nARE YOU SURE? [yes/no/cr=no] \c"
	read ans
	case "${ans}" in
	y*)
		;;
	*)
		echo "QUITING insert_card ..."
		return 2
		;;
	esac
	# 
	hwconf_adm --action add_card --cardtype $cardtype --shelfnum $shelf --slot $slot1 --mateshelf $shelf --mateslot $slot2 
	checkreturn ${?} 
	[[ ${?} != 0 ]] && return 2 
	#
	macaddr_adm --action add_card --cardtype $cardtype --shelfnum $shelf --slot $slot1 --addr $mac1
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2
	#
        macaddr_adm --action add_card --cardtype $cardtype --shelfnum $shelf --slot $slot2 --addr $mac2
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2 
	#
	netconf_adm --action add_routable_card --cardtype $cardtype --shelfnum $shelf --slot $slot1 --mateshelf $shelf --mateslot $slot2 
	checkreturn ${?}
	[[ ${?} != 0 ]] && return 2
	#
	echo "success, exiting insert_card ..."
	return 0
}
#
show_cards() {
	echo "executing show_cards ..."
	#
	# verify sysconf directory and required files exist.
	checkdir ${sysconf}
	if [[ ${?} != 0 ]]
	then
		echo "'checkdir' failed, exiting delete_card ..."
		return 2
	fi
	#
	checkfiles ${sysconf}/card.conf ${sysconf}/macaddr.conf ${sysconf}/ipaddr.conf
	if [[ ${?} != 0 ]]
	then
		echo "'checkfiles' failed, exiting delete_card ..."
		return 2
	fi
	# required files
	cardconf="${sysconf}/card.conf"
	macaddrconf="${sysconf}/macaddr.conf"
	ipaddrconf="${sysconf}/ipaddr.conf"
	# select a shelf
	shelfs=$(cat ${cardconf} | grep ^cardtype | cut -d';' -f2 | sort -u )
	#
	PS3="choose data type to display: "
	select datatype in QUIT dhcp network macaddr hardware
	do
		case "${datatype}" in
		dhcp)
			echo "DHCP: "
			echo "SIDE 1: "
			dhcpconf_adm --action show_all --hostside 1
			echo "SIDE 2: "
			dhcpconf_adm --action show_all --hostside 2
			;;
		network)
			echo "NETWORK:"
			netconf_adm --action show_routable_card
			;;
		macaddr)
			echo "MACADDR:"
			macaddr_adm --action show_all
			;;
		hardware)
			echo "HARDWARE:"
			for shelf in ${shelfs}
			do
				echo "SHELF ${shelf}:"
				hwconf_adm --action show_shelf --shelfnum ${shelf}
				REPLY=
			done
			;;
		QUIT)
			break 2
			;;
		esac
		REPLY=
	done
	#
	echo "success, exiting show_card ..."
	return 0
}
#
backup_cards() {
	dia=$(date '+%m%d%y%H%M')
	ORIGINALbackupdir="/storage/su_backup/LSS/GROWTH/${dia}/"
	echo "BACK UP DIR: ${ORIGINALbackupdir}"
	backupDHCP ${ORIGINALbackupdir}
	backupPKGCONF ${ORIGINALbackupdir}
	backupSYSCONF ${ORIGINALbackupdir}
	return 0
}
#
save_sim_log() {
	dia=$(date '+%m%d%y%H%M')
	backupdirprefix="/storage/su_backup/LSS/GROWTH/${dia}/"
	echo "BACK UP DIR: ${backupdirprefix}"
	mi=$(uname -n )
	backupdir=${backupdirprefix}/${mi}/sim
	[[ ! -d "${backupdir}" ]] && mkdir -p ${backupdir}
	echo "cp /var/opt/LSS/log/sim/sim.log ${backupdir}"
	cp /var/opt/LSS/log/sim/sim.log ${backupdir}
	return 0
}
#
# scan cmd line arguments
#
set -- $(getopt 'xs:' ${*} 2>${tfile})
if [[ ${?} != 0 || -s "${tfile}" ]]
then
	usage
	exit 2
fi
#
for opt in ${*}
do
	case "${opt}" in
	-x)
		set -x
		shift 1
		;;
	-s)
		sysconf="${2}"
		shift 2
		;;
	--)
		shift 1
		break
		;;
	esac
		
done
#
# choose an operation
#
PS3="choose an operation: "
select oper in QUIT delete_card insert_card update_personality_files dhcp_delete_all dhcp_add_all show_cards backup_cards save_sim_log 
do
	if [[ -n "${oper}" ]] 
	then
		# all done?
		[[ "${oper}" == QUIT ]] && break
		# call operation
		eval ${oper}
		status=${?}
		if [[ "${status}" != 0 ]]
		then
			echo "'${oper}' return code is ${status}."
		else
			echo "'${oper}' returned success."
		fi
	fi
	PS3="choose an operation: "
	REPLY=
done
#
# all done
#
exit 0
