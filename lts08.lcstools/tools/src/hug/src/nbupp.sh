#!/usr/bin/ksh
#
# create a script to reload and provision a lab.
#
#########################################################################
#
# get data for creating script.
#
CMD=${0}
PATH=$PATH:/opt/exp/lib/unity/bin
#
export CURRDIR=$(pwd)
#
trap 'rm -rf /tmp/*$$ 1>/dev/null 2>&1' 0 1 2 3 4 5 6 15
#
usage() {
cat <<EOF
usage: ${CMD} [-?] [-x] [-l] [-C] [-E] [-X] [-I] [-P|-p] [-G] [-S [MMDDhhmm | now]]
	[-b branch] [-c cpuload] [-R remoteuser] [-m email1[,email2,...]] 
	[-i [duplex | simplex [-A|-B] ] [-o outfile] [-U user TL1 scripts path]
	[-1 tl1login] [-2 tl1passwd] [-3 lynxlogin] [-4 lynxpasswd]
	[-T script1,script2,...] [-t] [-d] [-e] [-f] [-F] [-L] [-r] [labid] 

where: 
	-x - debug 
	-? - usage message 
	-l - list files for this labid (ls -ltr)
	-R remoteuser - remote user for logging 
	-C - cat log file to stdout 
	-b branch - branch used for reload 
	-c cpuload - cpu load used for reload 
	-i simplex|duplex - boot lab into simplex or duplex mode 
	-A - SP-A is active when running simplex mode 
	-B - SP-B is active when running simplex mode 
	-o filename - output file name 
	-X - execute script 
	-I - interactive mode 
	-E - edit script before executing 
	-P - provision lab 
	-p - ONLY provision lab (init-sys::30 done before provisioning)
	-T - Tl1 scripts to run in the given order 
	-U - path to user's TL1 scripts 
	-G - background the reload process 
	-t - tail the current reload log file for the given labid 
	-e - edit current reload log file for the given labid using \$EDITOR 
	-L - list current reload locks 
	-r - replace any init-sys10 cmds with 'reboot -aN'.
	-d - do SG pre-calldata init 
	-F - call updateioms before cpymems (if reloading to an older release)
	-f - list commands that have run 
	-m - comma-separated list of email addresses to notify 
	-u usermapfile - file mapping user products to chassis products 
	-S MMDDhhmm | now - start batch job at given time. The format is:
		(MM=01-12, DD=01-31, hh=00-23, mm=00-59). The special string
		'now' means to start immediately.

-G disables interactive (-I) and enables execute (-X). 
-I enables execute (-X). 
-T enables provisioning (-P).

If the shell variable USERTL1SCRIPTS is set and exists, then the TL1 
scripts are read from there. If the -U option is given, then the path 
given with the -U option takes precedence of USERTL1SCRIPTS. If RELOADEMAIL
is set, then email is sent to list of addresses contained in it. If the -m 
option is used, then the list of email addresses given with the -m option 
takes precedence. 
 
The '-u' option supports loading private executable files and private tar 
files. The user map file has the following format, it can contains as many 
entries as required: 

full_path_to_user_product1 full_path_chassis_product1 
full_path_to_user_product2 full_path_chassis_product2 
full_path_to_user_tarfile1 
full_path_to_user_tarfile2 
... 
The tar files and executables can be in any order in the user map file. 
Reload expands the tar files first, then it installs any private files 
using ftp. 
EOF
}
#
branch=
cpuload=
labid=
mode=
activesp=
suffix=${RANDOM}
useroutfile=
iomtypes=/tmp/iomtypes.$$
iomtypes2=/tmp/iomtypes2.$$
usertarfiles=/tmp/usertarfiles.$$
userexecfiles=/tmp/userexecfiles.$$
execscript=no
interactive=no
editscript=no
provision=no
provisiononly=no
replaceinitsys=no
#
tl1login=telica
tl1passwd=telica
lynxlogin=root
lynxpasswd=plexus9000
#
# maxcpymems=10 - change to lower value as a test (05/15/2007)
maxcpymems=5
#
tl1scripts=
background=no
taillogfile=no
catlogfile=no
editlogfile=no
filterlogfile=no
email=
dosginit=no
usermapfile=
remoteuser="UNKNOWN";
attimespec=now
listfiles=no
updateioms=no
loadhaslinuxcms=no
#
# time to wait for cpu to come up (changes if only privisioning).
provwaitforcpu=15
#
set -- `getopt ?1:2:3:4:rFlS:R:XCfetxb:o:c:i:T:U:u:LGPpIEABm:d ${*}`
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
	-L)
		exec lcslockadm -L
		;;
	-I)
		interactive=yes
		background=no	# interactive and background make no sense
		execscript=yes	# assume user wants to exec script.
		shift
		;;
	-G)
		background=yes
		interactive=no	# background and interactive makes no sense.
		execscript=yes	# assume user wants to exec script.
		shift
		;;
	-P)
		provision=yes
		provisiononly=no
		shift
		;;
	-p)
		provision=yes
		provisiononly=yes
		provwaitforcpu=1200
		shift
		;;
	-F)
		updateioms=yes
		shift
		;;
	-l)
		listfiles=yes
		shift
		;;
	-d)
		dosginit=yes
		shift
		;;
	-E)
		editscript=yes
		shift
		;;
	-X)
		execscript=yes
		shift
		;;
	-C)
		catlogfile=yes
		taillogfile=yes
		editlogfile=no
		filterlogfile=no
		shift
		;;
	-t)
		taillogfile=yes
		shift
		;;
	-r)
		replaceinitsys=yes
		shift
		;;
	-f)
		filterlogfile=yes
		editlogfile=no
		catlogfile=no
		taillogfile=yes
		shift
		;;
	-e)
		filterlogfile=no
		editlogfile=yes
		taillogfile=yes
		shift
		;;
	-1)
		tl1login=${2}
		shift 2
		;;
	-2)
		tl1passwd=${2}
		shift 2
		;;
	-3)
		lynxlogin=${2}
		shift 2
		;;
	-4)
		lynxpasswd=${2}
		shift 2
		;;
	-R)
		remoteuser=${2}
		shift 2
		;;
	-b)
		branch=${2}
		shift 2
		;;
	-m)
		RELOADEMAIL=${2}
		shift 2
		;;
	-S)
		attimespec="${2}"
		if [[ "${attimespec}" != "now" ]]
		then
			attimespec="-t ${2}"
		fi
		shift 2
		;;
	-c)
		cpuload=${2}
		shift 2
		;;
	-u)
		usermapfile=${2}
		if [[ ! -r "${usermapfile}" ]]
		then
			echo "\nERROR: User map file ${usermapfile} is not readable."
			exit 2
		fi
		shift 2
		;;
	-T)
		tl1scripts=${2}
		provision=yes
		shift 2
		;;
	-U)
		export USERTL1SCRIPTS=${2}
		provision=yes
		shift 2
		;;
	-o)
		useroutfile=${2}
		shift 2
		;;
	-i)
		mode=${2}
		shift 2
		;;
	-A)
		activesp=spa
		shift
		;;
	-B)
		activesp=spb
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
if [[ $# == 1 ]]
then
	labid=${1}
elif [[ $# > 1 ]]
then
	echo "\nERROR: too many command line arguments";
	usage >&2
	exit 2
fi
#
if [[ "$(id)" == "$(id lcstools)" ]]
then
	isowner=yes
	statusclause=""
	# just in case
	chmod 777 /lcsl100/tmp
else
	isowner=no
	statusclause=" and status req ^oper$ "
fi
#
# check if there are any private files to load.
#
if [[ -n "${usermapfile}" ]]
then
	# separate tar files from executable files. remove
	# comments and empty lines.
	errorfound=no
	>${usertarfiles}
	>${userexecfiles}
	#
	cat "${usermapfile}" |
	grep -v "#" |
	grep -v "^[ \t]*$" |
	while read inbuf
	do
		set -- ${inbuf}
		#
		case "${#}" in
		1)
			# we have a tar file
			if [[ ! -r "${1}" ]]
			then
				echo 
				echo "ERROR: private tar file ${1} is not readable."
				errorfound=yes
				continue
			fi
			#
			case "${1}" in
			*.tar.gz)
				echo ${1} >>${usertarfiles}
				;;
			*)
				echo
				echo "ERROR: tar file ${1} does not have .tar.gz suffix."
				errorfound=yes
				;;
			esac
			;;
		2)
			# we have a private file to install.
			if [[ ! -r "${1}" ]]
			then
				echo 
				echo "ERROR: private executable file ${1} is not readable."
				errorfound=yes
				continue
			fi
			#
			echo ${1} >>${userexecfiles}
			;;
		*)
			# we have junk. flag it.
			echo
			echo "ERROR: too many files names in line - ${inbuf}"
			errorfound=yes
			;;
		esac
		#
		if [[ "${errorfound}" == yes ]]
		then
			echo
			echo "ERROR: Errors found in user map file. Exit."
			exit 2
		fi
	done
	#
	if [[ -s "${usertarfiles}" ]]
	then
		echo
		echo "WARNING: these private tar files will be installed."
		cat ${usertarfiles}
	fi
	if [[ -s "${userexecfiles}" ]]
	then
		echo
		echo "WARNING: these private executable files will be installed."
		cat ${userexecfiles}
	fi
fi
#
# this is dangerous. you can mess up any open files, if the files are
# given relative paths. hardcode the paths (using LCSTOOLSDATA) where
# necessary.
#
# cd $LCSTOOLSDATA
#
if [[ -z "${labid}" ]]
then
	if [[ -n "${LCSLABID}" ]]
	then
		export labid=${LCSLABID}
	else 
		PS3="Choose LABID to reload:"
		select labid in QUIT $(cat ${LCSTOOLSDATA}/chassis | cut -d'	' -f1 | sort -u)
		do
			case "${labid}" in
			QUIT)
				echo "Quitting ${CMD}."
				exit 0
				;;
			*)
				export labid
				[[ -n "${labid}" ]] && break
				;;
			esac
		done
	fi
fi
#
# are we just listing files
#
if [[ "${listfiles}" == yes ]]
then
	cd /lcsl100/tmp
	exec ls -ltr *${labid}*
fi
#
# are we tailing a log file only?
#
if [[ "${taillogfile}" == yes ]]
then
	echo
	echo "Attempting to tail log file for lab ${labid}:"
	#
	cd /lcsl100/tmp
	#
	taillogfile="$(ls -t *.${labid}.log.* 2>/dev/null | sed -n '1,1p')"
	if [[ -z "${taillogfile}" ]]
	then
		echo
		echo "ERROR: Unable to find a log file for ${labid}."
		#
		exit 2
	fi
	if [[ ! -r "${taillogfile}" ]]
	then
		echo
		echo "ERROR: Log file for ${labid} is not readable."
		#
		exit 2
	fi
	#
	#
	if [[ "${editlogfile}" == yes ]]
	then
		echo
		echo "Editting log file ${taillogfile}:"
		echo
		[[ -z "${EDITOR}" ]] && export EDITOR=vi
		exec ${EDITOR} ${taillogfile}
	elif [[ "${filterlogfile}" == yes ]]
	then
		echo
		echo "Filtering log file ${taillogfile}:"
		echo
		exec cgrep -n +1 "^Calling" ${taillogfile}
	elif [[ "${catlogfile}" == yes ]]
	then
		echo
		echo "Cat log file ${taillogfile}:"
		echo
		exec cat ${taillogfile}
	else
		echo
		echo "Tailing log file ${taillogfile}:"
		echo
		exec tail -f ${taillogfile}
	fi
	#
	exit 0
fi
#
if [[ -z "${branch}" ]]
then
	PS3="Choose BRANCH for reload of ${labid}:"
	select branch in QUIT $(uprintf -q -f"%s\n" branch in ${LCSTOOLSDATA}/labloads where labid req ^${labid}$ ${statusclause} | sort -u )
	do
		case "${branch}" in
		QUIT)
			echo "Quitting ${CMD}."
			exit 0
			;;
		*)
			export branch
			[[ -n "${branch}" ]] && break
			;;
		esac
	done
fi
#
if [[ -z "${cpuload}" ]]
then
	PS3="Choose $branch CPU LOAD for reload of ${labid}:"
	select cpuload in QUIT $(uprintf -q -f"%s\n" cpuload in ${LCSTOOLSDATA}/labloads where labid req ^${labid}$ and branch req ^${branch}$ ${statusclause} | sort -u )
	do
		case "${cpuload}" in
		QUIT)
			echo "Quitting ${CMD}."
			exit 0
			;;
		*)
			export cpuload
			[[ -n "${cpuload}" ]] && break
			;;
		esac
	done
fi
#
if [[ -z "${mode}" ]]
then
	PS3="Choose boot mode for ${labid}:"
	select mode in QUIT simplex duplex
	do
		case "${mode}" in
		QUIT)
			echo "Quitting ${CMD}."
			exit 0
			;;
		*)
			export mode
			[[ -n "${mode}" ]] && break
			;;
		esac
	done
fi
#
case "${mode}" in
simplex)
	if [[ -z "${activesp}" ]]
	then
		PS3="Choose active SP for simplex ${labid}:"
		select activesp in QUIT spa spb 
		do
			case "${activesp}" in
			QUIT)
				echo "Quitting ${CMD}."
				exit 0
				;;
			*)
				export activesp
				[[ -n "${activesp}" ]] && break
				;;
			esac
		done
	fi
	;;
duplex)
	# ignore -A and -B choices
	activesp=both
	;;
*)
	echo "ERROR: Invalid boot mode: $mode" >&2
	usage >&2
	exit 2
	;;
esac
#
echo "\nReloading using the following data:\n"
echo "LABID      : $labid"
echo "BRANCH     : $branch"
echo "CPULOAD    : $cpuload"
echo "MODE       : $mode"
echo "ACTIVESP   : $activesp"
echo "USER OUTPUT FILE: $useroutfile"
#
outfile=/lcsl100/tmp/bupp.reload.${labid}.script.${suffix}
>${outfile}
chmod 777 ${outfile}
echo "SCRIPT FILE: $outfile"
#
# tl1 logfiles
#
inproglogfile=/lcsl100/tmp/bupp.reload.${labid}.tl1.inprog.${suffix}
denylogfile=/lcsl100/tmp/bupp.reload.${labid}.tl1.deny.${suffix}
misclogfile=/lcsl100/tmp/bupp.reload.${labid}.tl1.misc.${suffix}
#
############################################################################
#
# functions for writing script
#
writefile () {
	echo
	echo "Writing file section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION file ######
file script {
	scriptsdir .
}
###### END OF SECTION file ######
EOF
	#
	return 0
}
#
writeglobals () {
	echo
	echo "Writing globals section ..."
	#
	spa_ip=$(uprintf -q -f"%s\n" cpu_a_ip in ${LCSTOOLSDATA}/chassis where labid req ^${labid}$ | sort -u | sed -n '1,1p' )
	spb_ip=$(uprintf -q -f"%s\n" cpu_b_ip in ${LCSTOOLSDATA}/chassis where labid req ^${labid}$ | sort -u | sed -n '1,1p' )
	#
	if [[ -z "$spa_ip" ]]
	then
		echo
		echo "ERROR: SP-A IP is null." >&2
		exit 2
	elif [[ -z "$spb_ip" ]]
	then
		echo
		echo "ERROR: SP-B IP is null." >&2
		exit 2
	fi
	#
	spa_gw="${spa_ip%.*}.254"
	spa_bc="${spa_ip%.*}.255"
	spb_gw="${spb_ip%.*}.254"
	spb_bc="${spb_ip%.*}.255"
	#
	link_cpuload=$(echo $cpuload | sed 's/\([0-9][0-9]*.P[0-9][0-9]*\)\.[0-9][0-9]*/\1/' | sed 's/\([0-9][0-9]*.SP\.[0-9][0-9]*\)\.[0-9][0-9]*/\1/')
	#
	mknewparts="/Telica/swCPU/${link_cpuload}/system/scripts/partition_create_extended"
	mknewpartsout="/mknewparts.out"
	#
	cat >>$outfile <<EOF
###### START OF SECTION globals ######
globals data {
	labid ${labid}
	branch $branch
	last_reload $cpuload
	cpu_load $link_cpuload
	mknewparts $mknewparts
	mknewpartsout $mknewpartsout
	spa {
		ip $spa_ip
		gw $spa_gw
		bc $spa_bc
		login $lynxlogin
		passwd $lynxpasswd
		tl1login $tl1login
		tl1passwd $tl1passwd
	}
	spb {
		ip $spb_ip
		gw $spb_gw
		bc $spb_bc
		login $lynxlogin
		passwd $lynxpasswd
		tl1login $tl1login
		tl1passwd $tl1passwd
	}
	osfiles $LCSTOOLSLYNXBIN
	mode $mode
	activesp $activesp
	# maximum simultaneous cpy-mems allowed is 10
	maxcpymems ${maxcpymems}
	# tl1 log files
	inproglogfile ${inproglogfile}
	denylogfile ${denylogfile}
	misclogfile ${misclogfile}
	# kludge - to prevent not-valid-state error when rmv-eqpt of IOMs
	# SP init end < sleeptime < rmv-eqpt of IOMs
	sleepbeforermveqptiom 120
EOF
	#
	filesystem=$(uprintf -q -f"%s\n" path in ${LCSTOOLSDATA}/filesystems where branch req ^${branch}$ and type req text | sort -u)
	#
	if [[ -z "$filesystem" ]]
	then
		echo
		echo "ERROR: File system for branch $branch is unknown." >&2
		exit 2
	fi
	#
	textdir=${filesystem}/${branch}/
	#
	cat >>$outfile <<EOF
	ftp {
		textdir $textdir
		images {
EOF
	#
	uprintf -q -f"\t\t\t%s %s\n" type name in ${LCSTOOLSDATA}/images where branch req ^${branch}$ and cpuload req ^${cpuload}$ >>$outfile
	#
	cat >>$outfile <<EOF
		}
		scriptsdir /home/lcstools/tools/src/hug/scripts
	}
}
###### END OF SECTION globals ######
EOF
	#
	# do we have a linux cm?
	#
	loadhaslinuxcms=no
	textdir=$(sed -n 's/^.*textdir \(.*\)/\1/p;' ${outfile})
	cmrelpath=$(sed -n 's/^.*[	 ]\(cm .*\)/\1/p;' ${outfile} | tr ' ' '/')
	cmtarpath="${textdir}/${cmrelpath}"
	if [[ -r "${cmtarpath}" ]]
	then
		if [[ $(gnutar tzvf ${cmtarpath} | grep -c swCM_APPS) != 0 ]]
		then
			loadhaslinuxcms=yes
		fi
	fi
	if [[ "${loadhaslinuxcms}" == yes ]]
	then
		echo
		echo "Load contains Linux CMs (linuxcms=${loadhaslinuxcms})."
	else
		echo
		echo "Load contains Lynx-OS CMs (linuxcms=${loadhaslinuxcms})."
	fi
	#
	return 0
}
#
writepreload () {
	echo
	echo "Writing preload section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION preload ######
preload exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
	ftp put -b ftpspa <osfiles>/rm /bin/rm
	ftp put -b ftpspb <osfiles>/rm /bin/rm
	#
	ftp close ftpspa
	ftp close ftpspb
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspa /bin/chmod 755 /bin/rm
	telnet exec telnetspb /bin/chmod 755 /bin/rm
	#
	telnet close telnetspa
	telnet close telnetspb
}
###### END OF SECTION preload ######
###### START OF SECTION preload_spa ######
preload_spa exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	#
	ftp put -b ftpspa <osfiles>/rm /bin/rm
	#
	ftp close ftpspa
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec telnetspa /bin/chmod 755 /bin/rm
	#
	telnet close telnetspa
}
###### END OF SECTION preload_spa ######
###### START OF SECTION preload_spb ######
preload_spb exec {
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
	ftp put -b ftpspb <osfiles>/rm /bin/rm
	#
	ftp close ftpspb
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspb /bin/chmod 755 /bin/rm
	#
	telnet close telnetspb
}
###### END OF SECTION preload_spb ######
EOF
	#
	return 0
}
#
writeminmode () {
	#
	echo 
	echo "Writing minmode section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION minmode ######
minmode exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp put -a ftpspa <ftp.scriptsdir>/iprecover /net/rc.local
	ftp close ftpspa
	#
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	ftp put -a ftpspb <ftp.scriptsdir>/iprecover /net/rc.local
	ftp close ftpspb
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet exec telnetspa /bin/chmod 755 /net/rc.local
	telnet exec telnetspa /bin/chmod 644 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i telnetspa sync
	telnet exec -i telnetspa reboot -aN
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	telnet exec telnetspb /bin/chmod 755 /net/rc.local
	telnet exec telnetspb /bin/chmod 644 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i telnetspb sync
	telnet exec -i telnetspb reboot -aN
	#
	telnet close telnetspa
	telnet close telnetspb
	#
	waitforcpustate -t1200 <spa.ip> minmode <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> minmode <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION minmode ######
###### START OF SECTION minmode_spa ######
minmode_spa exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp put -a ftpspa <ftp.scriptsdir>/iprecover /net/rc.local
	ftp close ftpspa
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet exec telnetspa /bin/chmod 755 /net/rc.local
	telnet exec telnetspa /bin/chmod 644 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i telnetspa sync
	telnet exec -i telnetspa reboot -aN
	#
	telnet close telnetspa
	#
	waitforcpustate -t1200 <spa.ip> minmode <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION minmode_spa ######
###### START OF SECTION minmode_spb ######
minmode_spb exec {
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	ftp put -a ftpspb <ftp.scriptsdir>/iprecover /net/rc.local
	ftp close ftpspb
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	telnet exec telnetspb /bin/chmod 755 /net/rc.local
	telnet exec telnetspb /bin/chmod 644 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i telnetspb sync
	telnet exec -i telnetspb reboot -aN
	#
	telnet close telnetspb
	#
	waitforcpustate -t1200 <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> minmode <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION minmode_spb ######
EOF
	#
	return 0
}
#
writeappmode () {
	#
	echo
	echo "Writing appmode section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION appmode ######
appmode exec {
	# boot switch into duplex application mode
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspa
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION appmode ######
###### START OF SECTION appmode_spa ######
appmode_spa exec {
	# boot switch into duplex application mode
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet close appmode_telnetspa
	#
	waitforcpustate -t1800 <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION appmode_spa ######
###### START OF SECTION appmode_spb ######
appmode_spb exec {
	# boot switch into duplex application mode
	#
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
}
###### END OF SECTION appmode_spb ######
EOF
	#
	return 0
}
#
writeremovetarfiles () {
	#
	echo
	echo "Writing removetarfiles section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION removetarfiles ######
removetarfiles exec {
	#
	# clean up old tar files before sending new ones.
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspa /bin/mv /*tar.gz /tmp
	telnet exec -i telnetspa /bin/rm -f /tmp/*tar.gz
	telnet exec telnetspb /bin/mv /*tar.gz /tmp
	telnet exec -i telnetspb /bin/rm -f /tmp/*tar.gz
	telnet exec -i telnetspa /bin/find /home \( -type f -o -type l \) -print | /bin/xargs rm -f
	telnet exec -i telnetspb /bin/find /home \( -type f -o -type l \) -print | /bin/xargs rm -f
	telnet exec -i telnetspa /bin/find /home1 \( -type f -o -type l \) -print | /bin/xargs rm -f
	telnet exec -i telnetspb /bin/find /home1 \( -type f -o -type l \) -print | /bin/xargs rm -f
	#
	telnet close telnetspa
	telnet close telnetspb
}
###### END OF SECTION removetarfiles ######
###### START OF SECTION removetarfiles_spa ######
removetarfiles_spa exec {
	#
	# clean up old tar files before sending new ones.
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec telnetspa /bin/mv /*tar.gz /tmp
	telnet exec -i telnetspa /bin/rm -f /tmp/*tar.gz
	telnet exec -i telnetspa /bin/find /home \( -type f -o -type l \) -print | /bin/xargs rm -f
	telnet exec -i telnetspa /bin/find /home1 \( -type f -o -type l \) -print | /bin/xargs rm -f
	#
	telnet close telnetspa
}
###### END OF SECTION removetarfiles_spa ######
###### START OF SECTION removetarfiles_spb ######
removetarfiles_spb exec {
	#
	# clean up old tar files before sending new ones.
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspb /bin/mv /*tar.gz /tmp
	telnet exec -i telnetspb /bin/rm -f /tmp/*tar.gz
	telnet exec -i telnetspb /bin/find /home \( -type f -o -type l \) -print | /bin/xargs rm -f
	telnet exec -i telnetspb /bin/find /home1 \( -type f -o -type l \) -print | /bin/xargs rm -f
	#
	telnet close telnetspb
}
###### END OF SECTION removetarfiles_spb ######
EOF
	#
	return 0
}
#
writeremoveload () {
	#
	echo
	echo "Writing removeload section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION removeload ######
removeload exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
	ftp put ftpspa <ftp.scriptsdir>/remove_config_data /home/remove_config_data
	ftp put ftpspa <ftp.scriptsdir>/remove_timesten /home/remove_timesten
	ftp put ftpspa <ftp.scriptsdir>/restore_dbCurrent /home/restore_dbCurrent
	ftp put ftpspa <ftp.scriptsdir>/remove_all_extra_partitions /home/remove_all_extra_partitions
	ftp put ftpspb <ftp.scriptsdir>/remove_config_data /home/remove_config_data
	ftp put ftpspb <ftp.scriptsdir>/remove_timesten /home/remove_timesten
	ftp put ftpspb <ftp.scriptsdir>/restore_dbCurrent /home/restore_dbCurrent
	ftp put ftpspb <ftp.scriptsdir>/remove_all_extra_partitions /home/remove_all_extra_partitions
	#
	ftp close ftpspa
	ftp close ftpspb
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspa /bin/chmod 755 /home/remove_config_data
	telnet exec telnetspb /bin/chmod 755 /home/remove_config_data
	telnet exec telnetspa /bin/chmod 755 /home/remove_timesten
	telnet exec telnetspb /bin/chmod 755 /home/remove_timesten
	telnet exec telnetspa /bin/chmod 755 /home/restore_dbCurrent
	telnet exec telnetspb /bin/chmod 755 /home/restore_dbCurrent
	telnet exec telnetspa /bin/chmod 755 /home/remove_all_extra_partitions
	telnet exec telnetspb /bin/chmod 755 /home/remove_all_extra_partitions
	#
	# sleep 1200
	#
	telnet exec -i telnetspa /home/remove_config_data
	telnet exec -i telnetspb /home/remove_config_data
	#
	telnet exec -i -v telnetspa /home/remove_timesten
	telnet exec -i -v telnetspb /home/remove_timesten
	#
	telnet exec -t300 -i -v telnetspa /home/restore_dbCurrent a <spa.ip> <spa.gw>
	telnet exec -t300 -i -v telnetspb /home/restore_dbCurrent b <spb.ip> <spb.gw>
	#
	telnet exec -i -v telnetspa /home/remove_all_extra_partitions
	telnet exec -i -v telnetspb /home/remove_all_extra_partitions
	#
	# remove old CPU/IOM loads
	#
	telnet exec -i -v telnetspa rm -rf /Telica/sw*/*
	telnet exec -i -v telnetspb rm -rf /Telica/sw*/*
	# 
	# clean up old lost+found directories and old vi log files
	#
	telnet exec -i telnetspa rm -rf /lost+found/*
	telnet exec -i telnetspa rm -rf /usr/tmp/vi.recover
	telnet exec -i telnetspa mkdir -p /usr/tmp/vi.recover
	#
	telnet exec -i telnetspb rm -rf /lost+found/*
	telnet exec -i telnetspb rm -rf /usr/tmp/vi.recover
	telnet exec -i telnetspb mkdir -p /usr/tmp/vi.recover
	#
	# clean up the old data that is saved from boot to boot.
	#
	telnet exec -v -t60 telnetspa cd /Telica
	telnet exec -i telnetspa rm -rf SAVErestore 1>/dev/null 2>&1
	telnet exec telnetspa mv restore SAVErestore
	telnet exec telnetspa mkdir restore
	telnet exec -v -t60 telnetspa cd SAVErestore
	telnet exec telnetspa find . -type d -print | cpio -pduvm ../restore
	telnet exec telnetspa echo '# template rc.local file' >../restore/rc.local
	telnet exec -v -t60 telnetspa cd /Telica
	telnet exec -i telnetspa rm -rf SAVErestore 1>/dev/null 2>&1
	#
	telnet exec telnetspa mv /.odbc.ini /tmp 1>/dev/null 1>&2
	telnet exec -i telnetspa rm -f /tmp/.odbc.ini 1>/dev/null 1>&2
	telnet exec -i telnetspa rm -rf /opt/TimesTen* 1>/dev/null 1>&2
	telnet exec telnetspa echo '# over-written by reload' >/etc/resolv.conf
	telnet exec -i telnetspa rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2
	telnet exec -i telnetspa mkdir -p /home1/Telica/swCPU/core 1>/dev/null 2>&1
	telnet exec -i telnetspa [[ -L /localbackup ]] && rm -f /localbackup 1>/dev/null 2>&1
	telnet exec -i telnetspa rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i telnetspa rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	#
	telnet exec -v -t60 telnetspb cd /Telica
	telnet exec -i telnetspb rm -rf SAVErestore 1>/dev/null 2>&1
	telnet exec telnetspb mv restore SAVErestore
	telnet exec telnetspb mkdir restore
	telnet exec -v -t60 telnetspb cd SAVErestore
	telnet exec telnetspb find . -type d -print | cpio -pduvm ../restore
	telnet exec telnetspb echo '# template rc.local file' >../restore/rc.local
	telnet exec -v -t60 telnetspb cd /Telica
	telnet exec -i telnetspb rm -rf SAVErestore 1>/dev/null 2>&1
	#
	telnet exec telnetspb mv /.odbc.ini /tmp 1>/dev/null 1>&2
	telnet exec -i telnetspb rm -f /tmp/.odbc.ini 1>/dev/null 1>&2
	telnet exec -i telnetspb rm -rf /opt/TimesTen* 1>/dev/null 1>&2
	telnet exec telnetspb echo '# over-written by reload' >/etc/resolv.conf
	telnet exec -i telnetspb rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2
	telnet exec -i telnetspb mkdir -p /home1/Telica/swCPU/core 1>/dev/null 2>&1
	telnet exec -i telnetspb [[ -L /localbackup ]] && rm -f /localbackup 1>/dev/null 2>&1
	telnet exec -i telnetspb rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i telnetspb rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	#
	telnet close telnetspa
	telnet close telnetspb
}
###### END OF SECTION removeload ######
###### START OF SECTION removeload_spa ######
removeload_spa exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	#
	ftp put ftpspa <ftp.scriptsdir>/remove_config_data /home/remove_config_data
	ftp put ftpspa <ftp.scriptsdir>/remove_timesten /home/remove_timesten
	ftp put ftpspa <ftp.scriptsdir>/restore_dbCurrent /home/restore_dbCurrent
	ftp put ftpspa <ftp.scriptsdir>/remove_all_extra_partitions /home/remove_all_extra_partitions
	#
	ftp close ftpspa
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec telnetspa /bin/chmod 755 /home/remove_config_data
	telnet exec telnetspa /bin/chmod 755 /home/remove_timesten
	telnet exec telnetspa /bin/chmod 755 /home/restore_dbCurrent
	telnet exec telnetspa /bin/chmod 755 /home/remove_all_extra_partitions
	#
	telnet exec -i telnetspa /home/remove_config_data
	telnet exec -i -v telnetspa /home/remove_timesten
	telnet exec -t300 -i -v telnetspa /home/restore_dbCurrent a <spa.ip> <spa.gw>
	telnet exec -i -v telnetspa /home/remove_all_extra_partitions
	#
	# remove old CPU/IOM loads
	#
	telnet exec -i telnetspa rm -rf /Telica/sw*/*
	#
	# clean up the old data that is saved from boot to boot.
	#
	telnet exec -v -t60 telnetspa cd /Telica
	telnet exec -i telnetspa rm -rf SAVEdbCurrent 1>/dev/null 2>&1
	#
	telnet exec -v -t60 telnetspa cd /Telica
	telnet exec -i telnetspa rm -rf SAVErestore 1>/dev/null 2>&1
	telnet exec telnetspa mv restore SAVErestore
	telnet exec telnetspa mkdir restore
	telnet exec -v -t60 telnetspa cd SAVErestore
	telnet exec telnetspa find . -type d -print | cpio -pduvm ../restore
	telnet exec telnetspa echo '# template rc.local file' >../restore/rc.local
	telnet exec -v -t60 telnetspa cd /Telica
	telnet exec -i telnetspa rm -rf SAVErestore 1>/dev/null 2>&1
	#
	telnet exec telnetspa mv /.odbc.ini /tmp 1>/dev/null 1>&2
	telnet exec -i telnetspa rm -f /tmp/.odbc.ini 1>/dev/null 1>&2
	telnet exec -i telnetspa rm -rf /opt/TimesTen* 1>/dev/null 1>&2
	telnet exec telnetspa echo '# over-written by reload' >/etc/resolv.conf
	telnet exec -i telnetspa rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2
	telnet exec -i telnetspa mkdir -p /home1/Telica/swCPU/core 1>/dev/null 2>&1
	telnet exec -i telnetspa [[ -L /localbackup ]] && rm -f /localbackup 1>/dev/null 2>&1
	telnet exec -i telnetspa rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i telnetspa rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	#
	telnet close telnetspa
}
###### END OF SECTION removeload_spa ######
###### START OF SECTION removeload_spb ######
removeload_spb exec {
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
	ftp put ftpspb <ftp.scriptsdir>/remove_config_data /home/remove_config_data
	ftp put ftpspb <ftp.scriptsdir>/remove_timesten /home/remove_timesten
	ftp put ftpspb <ftp.scriptsdir>/restore_dbCurrent /home/restore_dbCurrent
	ftp put ftpspb <ftp.scriptsdir>/remove_all_extra_partitions /home/remove_all_extra_partitions
	#
	ftp close ftpspb
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec telnetspb /bin/chmod 755 /home/remove_config_data
	telnet exec telnetspb /bin/chmod 755 /home/remove_timesten
	telnet exec telnetspb /bin/chmod 755 /home/restore_dbCurrent
	telnet exec telnetspb /bin/chmod 755 /home/remove_all_extra_partitions
	#
	telnet exec -i telnetspb /home/remove_config_data
	telnet exec -i -v telnetspb /home/remove_timesten
	telnet exec -t300 -i -v telnetspb /home/restore_dbCurrent b <spb.ip> <spb.gw>
	telnet exec -i -v telnetspb /home/remove_all_extra_partitions
	#
	# remove old CPU/IOM loads
	#
	telnet exec -i telnetspb rm -rf /Telica/sw*/*
	#
	# clean up the old data that is saved from boot to boot.
	#
	telnet exec -v -t60 telnetspb cd /Telica
	telnet exec -i telnetspb rm -rf SAVErestore 1>/dev/null 2>&1
	telnet exec telnetspb mv restore SAVErestore
	telnet exec telnetspb mkdir restore
	telnet exec -v -t60 telnetspb cd SAVErestore
	telnet exec telnetspb find . -type d -print | cpio -pduvm ../restore
	telnet exec telnetspb echo '# template rc.local file' >../restore/rc.local
	telnet exec -v -t60 telnetspb cd /Telica
	telnet exec -i telnetspb rm -rf SAVErestore 1>/dev/null 2>&1
	#
	telnet exec telnetspb mv /.odbc.ini /tmp 1>/dev/null 1>&2
	telnet exec -i telnetspb rm -f /tmp/.odbc.ini 1>/dev/null 1>&2
	telnet exec -i telnetspb rm -rf /opt/TimesTen* 1>/dev/null 1>&2
	telnet exec telnetspb echo '# over-written by reload' >/etc/resolv.conf
	telnet exec -i telnetspb rm -rf /Telica/History/SavedLogs/* 1>/dev/null 1>&2
	telnet exec -i telnetspb mkdir -p /home1/Telica/swCPU/core 1>/dev/null 2>&1
	telnet exec -i telnetspb [[ -L /localbackup ]] && rm -f /localbackup 1>/dev/null 2>&1
	telnet exec -i telnetspb rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i telnetspb rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	#
	telnet close telnetspb
}
###### END OF SECTION removeload_spb ######
EOF
	#
	return 0
}
#
writeftpload () {
	#
	echo
	echo "Write ftpload section ..."
	#
	# determine which IOM images to ship to the switch.
	#
	uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where branch req ^${branch}$ and labid req ^${labid}$ and type rne ^bts$ and type rne ^tca$ and type rne '.*debug_.*' | 
	sort -u >${iomtypes}
	#
	if [[ -s "${iomtypes}" ]]
	then
		# add cpu to list of images to ship.
		#echo cpu >>${iomtypes}
		(echo cpu; cat ${iomtypes};) >${iomtypes2}
		mv ${iomtypes2} ${iomtypes}
	else
		echo 
		echo "No data found in chassis2ioms for branch ${branch}."
		echo "Try again using base branch."
		#
		basebranch="$(uprintf -q -f"%s" base in ${LCSTOOLSDATA}/basebranch where branch req ^${branch}$ and type req ^tl1$ )"
		if [[ -n "${basebranch}" ]]
		then
			echo
			echo "Base branch for branch ${branch} is ${basebranch}"
			echo "Check chassis2ioms again using base branch."
			#
			uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where branch req ^${basebranch}$ and labid req ^${labid}$ | 
			sort -u >${iomtypes}
			#
			if [[ -s "${iomtypes}" ]]
			then
				# add cpu to list of images to ship.
				# echo cpu >>${iomtypes}
				(echo cpu; cat ${iomtypes};) >${iomtypes2}
				mv ${iomtypes2} ${iomtypes}
				#
			else
				echo
				echo "No data found in chassis2ioms for base branch ${basebranch}. Using images relation."
				#
				uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/images where branch req ^${branch}$ and cpuload req ^${cpuload}$ and type rne ^bts$ and type rne ^tca$ and type rne '.*debug_.*' | 
				sort -u |
				grep -v cpu >${iomtypes}
				(echo cpu; cat ${iomtypes};) >${iomtypes2}
				mv ${iomtypes2} ${iomtypes}
			fi
		else
			echo
			echo "No data found in relation basebranch for branch ${branch}. Using images relation."
			uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/images where branch req ^${branch}$ and cpuload req ^${cpuload}$ and type rne ^bts$ and type rne ^tca$ and type rne '.*debug_.*' | 
			sort -u |
			grep -v cpu >${iomtypes}
			(echo cpu; cat ${iomtypes};) >${iomtypes2}
			mv ${iomtypes2} ${iomtypes}
		fi
	fi
	#
	echo
	echo "IOM Types are:"
	cat ${iomtypes}
	#
	cat >>$outfile <<EOF
###### START OF SECTION ftpload ######
ftpload exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
EOF
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	ftp put -t1800 -b ftpspa ${privtarfile} /home/$(basename ${privtarfile})"
				echo "	ftp put -t1800 -b ftpspb ${privtarfile} /home/$(basename ${privtarfile})"
			else
				echo "	ftp put -t1800 -b ftpspa <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
				echo "	ftp put -t1800 -b ftpspb <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
			fi
		else
			echo "	ftp put -t1800 -b ftpspa <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
			echo "	ftp put -t1800 -b ftpspb <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
		fi
	done >>$outfile
	#
	cat >>$outfile <<EOF
	#
	ftp close ftpspa 
	ftp close ftpspb 
}
###### END OF SECTION ftpload ######
EOF
	#
	cat >>$outfile <<EOF
###### START OF SECTION ftpload_spa ######
ftpload_spa exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	#
EOF
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	ftp put -t1800 -b ftpspa ${privtarfile} /home/$(basename ${privtarfile})"
			else
				echo "	ftp put -t1800 -b ftpspa <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
			fi
		else
			echo "	ftp put -t1800 -b ftpspa <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
		fi
	done >>$outfile
	#
	cat >>$outfile <<EOF
	#
	ftp close ftpspa 
}
###### END OF SECTION ftpload_spa ######
EOF
	#
	cat >>$outfile <<EOF
###### START OF SECTION ftpload_spb ######
ftpload_spb exec {
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	#
EOF
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	ftp put -t1800 -b ftpspb ${privtarfile} /home/$(basename ${privtarfile})"
			else
				echo "	ftp put -t1800 -b ftpspb <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
			fi
		else
			echo "	ftp put -t1800 -b ftpspb <ftp.textdir>/${iomtype}/<ftp.images.${iomtype}> /home/<ftp.images.${iomtype}>"
		fi
	done >>$outfile
	#
	cat >>$outfile <<EOF
	#
	ftp close ftpspb 
}
###### END OF SECTION ftpload_spb ######
EOF
	#
	return 0
}
#
writeinstallnewload () {
	#
	echo
	echo "Write installnewload section ..."
	#
	# determine which IOM images to ship to the switch.
	#
	uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where branch req ^${branch}$ and labid req ^${labid}$ and type rne ^bts$ and type rne ^tca$  | 
	sort -u >${iomtypes}
	#
	if [[ -s "${iomtypes}" ]]
	then
		# add cpu to list of images to ship
		# echo cpu >>${iomtypes}
		(echo cpu; cat ${iomtypes};) >${iomtypes2}
		mv ${iomtypes2} ${iomtypes}
	else
		echo 
		echo "No data found in chassis2ioms for branch ${branch}."
		echo "Try again using base branch."
		#
		basebranch="$(uprintf -q -f"%s" base in ${LCSTOOLSDATA}/basebranch where branch req ^${branch}$ and type req ^tl1$ )"
		if [[ -n "${basebranch}" ]]
		then
			echo
			echo "Base branch for branch ${branch} is ${basebranch}"
			echo "Check chassis2ioms again using base branch."
			#
			uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/chassis2ioms where branch req ^${basebranch}$ and labid req ^${labid}$ | 
			sort -u >${iomtypes}
			#
			if [[ -s "${iomtypes}" ]]
			then
				# add cpu to list of images to ship.
				(echo cpu; cat ${iomtypes};) >${iomtypes2}
				mv ${iomtypes2} ${iomtypes}
			else
				echo
				echo "No data found in chassis2ioms for base branch ${basebranch}. Using images relation."
				#
				uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/images where branch req ^${branch}$ and cpuload req ^${cpuload}$ and type rne ^bts$ and type rne ^tca$ and type rne '.*debug_.*' | 
				sort -u |
				grep -v cpu >${iomtypes}
				(echo cpu; cat ${iomtypes};) >${iomtypes2}
				mv ${iomtypes2} ${iomtypes}
			fi
		else
			echo
			echo "No data found in relation basebranch for branch ${branch}. Using images relation."
			uprintf -q -f"%s\n" type in ${LCSTOOLSDATA}/images where branch req ^${branch}$ and cpuload req ^${cpuload}$ and type rne ^bts$ and type rne ^tca$ and type rne '.*debug_.*' | 
			sort -u |
			grep -v cpu >${iomtypes}
			(echo cpu; cat ${iomtypes};) >${iomtypes2}
			mv ${iomtypes2} ${iomtypes}
		fi
	fi
	#
	echo
	echo "IOM Types are:"
	cat ${iomtypes}
	#
	cat >>$outfile <<EOF
###### START OF SECTION installnewload ######
installnewload exec {
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
EOF
	#
	# check if we need to create /dev/hd2f and /dev/hd2g for
	# the new linux CM products.
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# reset home directory
		home="/"
		if [[ "${iomtype}" == cm && "${loadhaslinuxcms}" == yes && $(grep -c '^cm' ${iomtypes}) != 0 ]]
		then
			echo "	telnet exec -t300 -v telnetspa <mknewparts> 2>&1 | /bin/tee <mknewpartsout>" >>${outfile}
			echo "	telnet exec -t300 -v telnetspb <mknewparts> 2>&1 | /bin/tee <mknewpartsout>" >>${outfile}
			home="/home_f"
		fi
		#
		echo "	telnet exec -v -t60 telnetspa cd ${home}" >>${outfile}
		echo "	telnet exec -v -t60 telnetspb cd ${home}" >>${outfile}
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/$(basename ${privtarfile})"
				echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/$(basename ${privtarfile})"
				#
				echo "	telnet exec -t1800 -v telnetspa echo $(basename ${privtarfile}) >/.last_reload_${iomtype}"
				echo "	telnet exec -t1800 -v telnetspb echo $(basename ${privtarfile}) >/.last_reload_${iomtype}"
			else
				echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/<ftp.images.${iomtype}>"
				echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/<ftp.images.${iomtype}>"
				#
				echo "	telnet exec -t1800 -v telnetspa echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
				echo "	telnet exec -t1800 -v telnetspb echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
			fi
		else
			echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/<ftp.images.${iomtype}>"
			echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/<ftp.images.${iomtype}>"
			#
			echo "	telnet exec -t1800 -v telnetspa echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
			echo "	telnet exec -t1800 -v telnetspb echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
		fi
	done >>$outfile
	#
	if [[ -s "${usertarfiles}" ]]
	then
		priv_cpu_load=$(cat ${usertarfiles} | sed -n '/#/d; s/^/\//; s/^.*\/\([^/]*\)_cpu.tar.gz$/\1/p;')
	else
		priv_cpu_load=
	fi
	#
	if [[ -z "${priv_cpu_load}" ]]
	then
		cat >>$outfile <<EOF
	#
	telnet exec telnetspa echo '<last_reload>' >/.last_reload
	telnet exec telnetspb echo '<last_reload>' >/.last_reload
	#
	telnet exec telnetspa /bin/ln -s /Telica/swCPU/<cpu_load> /Telica/swCPU/CurrRel
	telnet exec telnetspb /bin/ln -s /Telica/swCPU/<cpu_load> /Telica/swCPU/CurrRel
EOF
	else
		cat >>$outfile <<EOF
	#
	telnet exec telnetspa echo '<priv_cpu_load>' >/.last_reload
	telnet exec telnetspb echo '<priv_cpu_load>' >/.last_reload
	#
	telnet exec telnetspa /bin/ln -s /Telica/swCPU/${priv_cpu_load} /Telica/swCPU/CurrRel
	telnet exec telnetspb /bin/ln -s /Telica/swCPU/${priv_cpu_load} /Telica/swCPU/CurrRel
EOF
	fi
	#
	# do we have user products we need to install?
	#
	if [[ -s "${userexecfiles}" ]]
	then
		echo
		echo "Updating switch with private user products."
		#
		echo "	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>" >>$outfile
		echo "	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>" >>$outfile
		cat ${userexecfiles} | 
		while read userprod ofcprod
		do
			if [[ ! -r "${userprod}" ]]
			then
				echo
				echo "ERROR: User product is not readable: ${userprod}"
				exit 2
			fi
			echo "	telnet exec -t120 -i -v telnetspa /bin/mv ${ofcprod} /home1" >>$outfile
			echo "	telnet exec -t120 -i -v telnetspb /bin/mv ${ofcprod} /home1" >>$outfile
			echo "	ftp put -t1800 -b ftpspa ${userprod} ${ofcprod}" >>$outfile
			echo "	ftp put -t1800 -b ftpspb ${userprod} ${ofcprod}" >>$outfile
			echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 ${ofcprod}" >>$outfile
			echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 ${ofcprod}" >>$outfile
		done
		#
		echo "	ftp close ftpspa" >>$outfile
		echo "	ftp close ftpspb" >>$outfile
	fi
	#
	# START TEMP KLUDGE - 6/17/07
	#
	# echo "	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>" >>$outfile
	# echo "	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>" >>$outfile
	#
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/partition_create_extended /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/restore_dbCurrent /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/move_dbCurrent /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	#
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/partition_create_extended /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/restore_dbCurrent /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/move_dbCurrent /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	#
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	#
	# echo "	ftp close ftpspa" >>$outfile
	# echo "	ftp close ftpspb" >>$outfile
	#
	# END TEMP KLUDGE - 6/17/07
	#
	cat >>$outfile <<EOF
	#
	telnet exec -v -t60 telnetspa cd /Telica/swCPU/CurrRel/system/scripts
	telnet exec -t1200 -v telnetspa cat /etc/fstab
	telnet exec -t1200 -v telnetspa df
	telnet exec -t1200 -v telnetspa ./uprev.sh
	#
	telnet exec -v -t60 telnetspb cd /Telica/swCPU/CurrRel/system/scripts
	telnet exec -t1200 -v telnetspb cat /etc/fstab
	telnet exec -t1200 -v telnetspb df
	telnet exec -t1200 -v telnetspb ./uprev.sh
	#
	telnet close telnetspa
	telnet close telnetspb
}
###### END OF SECTION installnewload ######
EOF
	#
	cat >>$outfile <<EOF
###### START OF SECTION installnewload_spa ######
installnewload_spa exec {
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec -v -t60 telnetspa cd /
	#
EOF
	#
	# check if we need to create /dev/hd2f and /dev/hd2g for
	# the new linux CM products.
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# reset home directory
		home="/"
		if [[ "${iomtype}" == cm && "${loadhaslinuxcms}" == yes && $(grep -c '^cm' ${iomtypes}) != 0 ]]
		then
			echo "	telnet exec -t300 -v telnetspa <mknewparts> 2>&1 | /bin/tee <mknewpartsout>" >>${outfile}
			home="/home_f"
		fi
		#
		echo "	telnet exec -v -t60 telnetspa cd ${home}" >>${outfile}
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/$(basename ${privtarfile})"
				echo "	telnet exec -t1800 -v telnetspa echo $(basename ${privtarfile}) >/.last_reload_${iomtype}"
			else
				echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/<ftp.images.${iomtype}>"
				echo "	telnet exec -t1800 -v telnetspa echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
			fi
		else
			echo "	telnet exec -t1800 -v telnetspa /bin/tar xzvf /home/<ftp.images.${iomtype}>"
			echo "	telnet exec -t1800 -v telnetspa echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
		fi
	done >>$outfile
	#
	if [[ -s "${usertarfiles}" ]]
	then
		priv_cpu_load=$(cat ${usertarfiles} | sed -n '/#/d; s/^/\//; s/^.*\/\([^/]*\)_cpu.tar.gz$/\1/p;')
	else
		priv_cpu_load=
	fi
	#
	if [[ -z "${priv_cpu_load}" ]]
	then
		cat >>$outfile <<EOF
	#
	telnet exec telnetspa /bin/ln -s /Telica/swCPU/<cpu_load> /Telica/swCPU/CurrRel
EOF
	else
		cat >>$outfile <<EOF
	#
	telnet exec telnetspa /bin/ln -s /Telica/swCPU/${priv_cpu_load} /Telica/swCPU/CurrRel
EOF
	fi
	#
	# do we have user products we need to install?
	#
	if [[ -s "${userexecfiles}" ]]
	then
		echo
		echo "Updating switch with private user products."
		#
		echo "	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>" >>$outfile
		cat ${userexecfiles} | 
		while read userprod ofcprod
		do
			if [[ ! -r "${userprod}" ]]
			then
				echo
				echo "ERROR: User product is not readable: ${userprod}"
				exit 2
			fi
			echo "	telnet exec -t120 -i -v telnetspa /bin/mv ${ofcprod} /home1" >>$outfile
			echo "	ftp put -t1800 -b ftpspa ${userprod} ${ofcprod}" >>$outfile
			echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 ${ofcprod}" >>$outfile
		done
		#
		echo "	ftp close ftpspa" >>$outfile
	fi
	#
	# START TEMP KLUDGE - 6/17/07
	#
	# echo "	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>" >>$outfile
	#
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/partition_create_extended /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/restore_dbCurrent /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	ftp put -t1800 -b ftpspa /lcsl101/text/Main/cpu/move_dbCurrent /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspa /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	#
	# echo "	ftp close ftpspa" >>$outfile
	#
	# END TEMP KLUDGE - 6/17/07
	#
	cat >>$outfile <<EOF
	#
	telnet exec -v -t60 telnetspa cd /Telica/swCPU/CurrRel/system/scripts
	telnet exec -t1200 -v telnetspa cat /etc/fstab
	telnet exec -t1200 -v telnetspa df
	telnet exec -t1200 -v telnetspa ./uprev.sh
	#
	telnet close telnetspa
}
###### END OF SECTION installnewload_spa ######
EOF
	#
	cat >>$outfile <<EOF
###### START OF SECTION installnewload_spb ######
installnewload_spb exec {
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
EOF
	#
	# check if we need to create /dev/hd2f and /dev/hd2g for
	# the new linux CM products.
	#
	cat ${iomtypes} |
	while read iomtype
	do
		# reset home directory
		home="/"
		if [[ "${iomtype}" == cm && "${loadhaslinuxcms}" == yes && $(grep -c '^cm' ${iomtypes}) != 0 ]]
		then
			echo "	telnet exec -t300 -v telnetspb <mknewparts> 2>&1 | /bin/tee <mknewpartsout>" >>${outfile}
			home="/home_f"
		fi
		#
		echo "	telnet exec -v -t60 telnetspb cd ${home}" >>${outfile}
		# check for private tar files
		if [[ -s "${usertarfiles}" ]]
		then
			# see if we are overwriting a tar file.
			privtarfile=$(grep "_${iomtype}.tar.gz$" ${usertarfiles} | sed -n '1,1p') 
			if [[ -n "${privtarfile}" ]]
			then
				# we have a private tar file. use it.
				echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/$(basename ${privtarfile})"
				echo "	telnet exec -t1800 -v telnetspb echo $(basename ${privtarfile}) >/.last_reload_${iomtype}"
			else
				echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/<ftp.images.${iomtype}>"
				echo "	telnet exec -t1800 -v telnetspb echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
			fi
		else
			echo "	telnet exec -t1800 -v telnetspb /bin/tar xzvf /home/<ftp.images.${iomtype}>"
			echo "	telnet exec -t1800 -v telnetspb echo <ftp.images.${iomtype}> >/.last_reload_${iomtype}"
		fi
	done >>$outfile
	#
	if [[ -s "${usertarfiles}" ]]
	then
		priv_cpu_load=$(cat ${usertarfiles} | sed -n '/#/d; s/^/\//; s/^.*\/\([^/]*\)_cpu.tar.gz$/\1/p;')
	else
		priv_cpu_load=
	fi
	#
	if [[ -z "${priv_cpu_load}" ]]
	then
		cat >>$outfile <<EOF
	#
	telnet exec telnetspb /bin/ln -s /Telica/swCPU/<cpu_load> /Telica/swCPU/CurrRel
EOF
	else
		cat >>$outfile <<EOF
	#
	telnet exec telnetspb /bin/ln -s /Telica/swCPU/${priv_cpu_load} /Telica/swCPU/CurrRel
EOF
	fi
	#
	# do we have user products we need to install?
	#
	if [[ -s "${userexecfiles}" ]]
	then
		echo
		echo "Updating switch with private user products."
		#
		echo "	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>" >>$outfile
		cat ${userexecfiles} | 
		while read userprod ofcprod
		do
			if [[ ! -r "${userprod}" ]]
			then
				echo
				echo "ERROR: User product is not readable: ${userprod}"
				exit 2
			fi
			echo "	telnet exec -t120 -i -v telnetspb /bin/mv ${ofcprod} /home1" >>$outfile
			echo "	ftp put -t1800 -b ftpspb ${userprod} ${ofcprod}" >>$outfile
			echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 ${ofcprod}" >>$outfile
		done
		#
		echo "	ftp close ftpspb" >>$outfile
	fi
	#
	# START TEMP KLUDGE - 6/17/07
	#
	# echo "	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>" >>$outfile
	#
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/partition_create_extended /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/restore_dbCurrent /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	ftp put -t1800 -b ftpspb /lcsl101/text/Main/cpu/move_dbCurrent /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/partition_create_extended" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/restore_dbCurrent" >>$outfile
	# echo "	telnet exec -t120 -v telnetspb /bin/chmod 777 /Telica/swCPU/CurrRel/system/scripts/move_dbCurrent" >>$outfile
	#
	# echo "	ftp close ftpspb" >>$outfile
	#
	# END TEMP KLUDGE - 6/17/07
	#
	cat >>$outfile <<EOF
	#
	telnet exec -v -t60 telnetspb cd /Telica/swCPU/CurrRel/system/scripts
	telnet exec -t1200 -v telnetspb cat /etc/fstab
	telnet exec -t1200 -v telnetspb df
	telnet exec -t1200 -v telnetspb ./uprev.sh
	#
	telnet close telnetspb
}
###### END OF SECTION installnewload_spb ######
EOF
	#
	return 0
}
#
writepostreload () {
	#
	echo
	echo "Write postreload section ..."
	#
	# ftp files/tools we need.
	#
	cat >>$outfile <<EOF
###### START OF SECTION postreload ######
postreload exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp put -a ftpspa <ftp.scriptsdir>/iprecover /bin/iprecover
	ftp put -a ftpspa <ftp.scriptsdir>/removedefaultgw /bin/removedefaultgw
	ftp put -a ftpspa <ftp.scriptsdir>/getcorelog /bin/getcorelog
	ftp put -b ftpspa <osfiles>/rm /bin/rm
	ftp put -a ftpspa <ftp.scriptsdir>/lf /bin/lf
	ftp put -a ftpspa <ftp.scriptsdir>/postreload /bin/postreload
	ftp close ftpspa
	#
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	ftp put -a ftpspb <ftp.scriptsdir>/iprecover /bin/iprecover
	ftp put -a ftpspb <ftp.scriptsdir>/removedefaultgw /bin/removedefaultgw
	ftp put -a ftpspb <ftp.scriptsdir>/getcorelog /bin/getcorelog
	ftp put -b ftpspb <osfiles>/rm /bin/rm
	ftp put -a ftpspb <ftp.scriptsdir>/lf /bin/lf
	ftp put -a ftpspb <ftp.scriptsdir>/postreload /bin/postreload
	ftp close ftpspb
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet exec telnetspa /bin/chmod 755 /bin/iprecover
	telnet exec telnetspa /bin/chmod 755 /bin/removedefaultgw
	telnet exec telnetspa /bin/chmod 755 /bin/getcorelog
	telnet exec telnetspa /bin/chmod 755 /bin/rm
	telnet exec telnetspa /bin/chmod 755 /bin/lf
	telnet exec telnetspa /bin/chmod 755 /bin/postreload
	telnet exec telnetspa /bin/postreload -d -D -h -c <maxcpymems> -m "${labid} SP-A"
	telnet exec telnetspa grep -v 'set.*-o.*emacs' /.profile >/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet exec telnetspa grep -v 'set.*-o.*vi' /.profile >/.profile.$$
	telnet exec telnetspa echo 'set -o vi' >>/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet exec telnetspa grep -v 'date.*-d1.*-z360' /.profile >/.profile.$$
	telnet exec telnetspa echo '/bin/date -d1 -z360' >>/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet close telnetspa
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	telnet exec telnetspb /bin/chmod 755 /bin/iprecover
	telnet exec telnetspb /bin/chmod 755 /bin/removedefaultgw
	telnet exec telnetspb /bin/chmod 755 /bin/getcorelog
	telnet exec telnetspb /bin/chmod 755 /bin/rm
	telnet exec telnetspb /bin/chmod 755 /bin/lf
	telnet exec telnetspb /bin/chmod 755 /bin/postreload
	telnet exec telnetspb /bin/postreload -d -D -h -c <maxcpymems> -m "${labid} SP-B"
	telnet exec telnetspb grep -v 'set.*-o.*emacs' /.profile >/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet exec telnetspb grep -v 'set.*-o.*vi' /.profile >/.profile.$$
	telnet exec telnetspb echo 'set -o vi' >>/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet exec telnetspb grep -v 'date.*-d1.*-z360' /.profile >/.profile.$$
	telnet exec telnetspb echo '/bin/date -d1 -z360' >>/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet close telnetspb
	#
}
###### END OF SECTION postreload ######
###### START OF SECTION postreload_spa ######
postreload_spa exec {
	ftp open ftpspa <spa.ip> <spa.login> <spa.passwd>
	ftp put -a ftpspa <ftp.scriptsdir>/iprecover /bin/iprecover
	ftp put -a ftpspa <ftp.scriptsdir>/removedefaultgw /bin/removedefaultgw
	ftp put -a ftpspa <ftp.scriptsdir>/getcorelog /bin/getcorelog
	ftp put -b ftpspa <osfiles>/rm /bin/rm
	ftp put -a ftpspa <ftp.scriptsdir>/lf /bin/lf
	ftp put -a ftpspa <ftp.scriptsdir>/postreload /bin/postreload
	ftp close ftpspa
	#
	telnet open telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet exec telnetspa /bin/chmod 755 /bin/iprecover
	telnet exec telnetspa /bin/chmod 755 /bin/removedefaultgw
	telnet exec telnetspa /bin/chmod 755 /bin/getcorelog
	telnet exec telnetspa /bin/chmod 755 /bin/rm
	telnet exec telnetspa /bin/chmod 755 /bin/lf
	telnet exec telnetspa /bin/chmod 755 /bin/postreload
	telnet exec telnetspa /bin/postreload -s -D -h -c <maxcpymems> -m "${labid} SP-A"
	telnet exec telnetspa grep -v 'set.*-o.*emacs' /.profile >/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet exec telnetspa grep -v 'set.*-o.*vi' /.profile >/.profile.$$
	telnet exec telnetspa echo 'set -o vi' >>/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet exec telnetspa grep -v 'date.*-d1.*-z360' /.profile >/.profile.$$
	telnet exec telnetspa echo '/bin/date -d1 -z360' >>/.profile.$$
	telnet exec telnetspa mv /.profile.$$ /.profile
	telnet close telnetspa
}
###### END OF SECTION postreload_spa ######
###### START OF SECTION postreload_spb ######
postreload_spb exec {
	ftp open ftpspb <spb.ip> <spb.login> <spb.passwd>
	ftp put -a ftpspb <ftp.scriptsdir>/iprecover /bin/iprecover
	ftp put -a ftpspb <ftp.scriptsdir>/removedefaultgw /bin/removedefaultgw
	ftp put -a ftpspb <ftp.scriptsdir>/getcorelog /bin/getcorelog
	ftp put -b ftpspb <osfiles>/rm /bin/rm
	ftp put -a ftpspb <ftp.scriptsdir>/lf /bin/lf
	ftp put -a ftpspb <ftp.scriptsdir>/postreload /bin/postreload
	ftp close ftpspb
	#
	telnet open telnetspb <spb.ip> <spb.login> <spb.passwd>
	telnet exec telnetspb /bin/chmod 755 /bin/iprecover
	telnet exec telnetspb /bin/chmod 755 /bin/removedefaultgw
	telnet exec telnetspb /bin/chmod 755 /bin/getcorelog
	telnet exec telnetspb /bin/chmod 755 /bin/rm
	telnet exec telnetspb /bin/chmod 755 /bin/lf
	telnet exec telnetspb /bin/chmod 755 /bin/postreload
	telnet exec telnetspb /bin/postreload -s -D -h -c <maxcpymems> -m "${labid} SP-B"
	telnet exec telnetspb grep -v 'set.*-o.*emacs' /.profile >/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet exec telnetspb grep -v 'set.*-o.*vi' /.profile >/.profile.$$
	telnet exec telnetspb echo 'set -o vi' >>/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet exec telnetspb grep -v 'date.*-d1.*-z360' /.profile >/.profile.$$
	telnet exec telnetspb echo '/bin/date -d1 -z360' >>/.profile.$$
	telnet exec telnetspb mv /.profile.$$ /.profile
	telnet close telnetspb
}
###### END OF SECTION postreload_spb ######
EOF
	#
	return 0
}
#
writertrveqpt () {
	#
	echo
	echo "Writing rtrv-eqpt section ..."
	#
	cat >>$outfile <<EOF
###### START OF SECTION rtrveqpt ######
rtrveqpt exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 rtrveqpt_tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v rtrveqpt_tl1sp rtrv-eqpt
	tl1 close rtrveqpt_tl1sp
}
###### END OF SECTION rtrveqpt ######
###### START OF SECTION rtrveqpt_spa ######
rtrveqpt_spa exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 rtrveqpt_tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v rtrveqpt_tl1sp rtrv-eqpt
	tl1 close rtrveqpt_tl1sp
}
###### END OF SECTION rtrveqpt_spa ######
###### START OF SECTION rtrveqpt_spb ######
rtrveqpt_spb exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 rtrveqpt_tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v rtrveqpt_tl1sp rtrv-eqpt
	tl1 close rtrveqpt_tl1sp
}
###### END OF SECTION rtrveqpt_spb ######
EOF
	#
	return 0
}
#
writeprovisioning () {
	#
	if [[ "${provision}" == no ]]
	then
		echo
		echo "Skipping provisioning section ..."
		return 0
	fi
	#
	echo
	echo "Writing provisioning section ..."
	#
	# check if TL1 scripts exist.
	#
	if [[ -n "${USERTL1SCRIPTS}" ]]
	then
		echo
		echo "WARNING: Using user's TL1 script path: ${USERTL1SCRIPTS}"
		tl1dir=${USERTL1SCRIPTS}
	else
		tl1dir=${LCSSCRIPTS}/${branch}/${cpuload}/${labid}
	fi
	#
	if [[ ! -d "${tl1dir}" ]]
	then
		echo
		echo "TL1 scripts for ${branch}/${cpuload}/${labid} not found."

		echo "Skip provisioning ..."
		provision=no
		return 2
	else
		echo 
		echo "Scripts are:"
		ls ${tl1dir}
	fi
	#
	# check if scripts were given, or if user must choose.
	#
	if [[ -n "${tl1scripts}" ]]
	then
		#
		# goto scripts directory and verify scripts exist.
		#
		cd ${tl1dir}
		scripts2exec="$(echo ${tl1scripts} | tr ',' ' ')"
		#
		for script in ${scripts2exec}
		do
			echo
			echo "Checking script: $script"
			#
			if [[ ! -r "${script}" ]]
			then
				echo
				echo "ERROR: Script ${tl1dir}/${script} is not readable."
				exit 2
			fi
		done
	else
		#
		# let user choose which scripts to execute for now. 
		#
		cd ${tl1dir}
		#
		# let user choose scripts to run
		#
		integer scrnum=1
		availablescripts=/tmp/AVS.$$
		#
		ls | 
		while read script
		do
			if [[ -f "${script}" ]]
			then
				echo "${scrnum})\t${script}"
				scrnum=$((scrnum+1))
			fi
		done >${availablescripts}
		#
		if [[ ! -s "${availablescripts}" ]]
		then
			echo
			echo "No scripts to choose from."
			echo "Skip provisioning ..."
			provision=no
			return 2
		fi
		#
		scripts2exec=
		#
		PS3="Choose provisioning option:"
		select oper in QUIT SKIP_PROVISIONING CHOOSE_SCRIPTS DONE_CHOOSING
		do
			case "${oper}" in
			QUIT)
				echo
				echo "Quitting ${CMD}."
				exit 0
				;;
			DONE_CHOOSING)
				break
				;;
			SKIP_PROVISIONING)
				echo
				echo "Skip provisioning ..."
				provision=no
				return 2
				;;
			*)
				echo
				echo "Available scripts are:"
				echo
				cat ${availablescripts}
				echo
				echo "Enter list of scripts to execute by number and in order (space separated list):"
				echo "list example: 7 3 1 2"
				echo
				read script_list
				#
				if [[ -n "${script_list}" ]]
				then
					scripts2exec=
					for scrno in ${script_list}
					do
						script="$(grep "^${scrno})	" ${availablescripts} | cut -d'	' -f2 | sort -u)"
						scripts2exec="${scripts2exec} ${script}"
					done
				else
					echo
					echo "No list entered."
				fi
				;;
			esac
			#
			echo
			echo "Current list of scripts to run: ${scripts2exec}"
		done
	fi
	#
	echo
	echo "TL1 scripts chosen: $scripts2exec"
	#
	# get chassis type
	#
	chassistype=$(grep -i chassis-eqpt ${scripts2exec} | grep -v '^[	 ]*#' | tr '[A-Z]' '[a-z]' | sed 's/^.*chassistype=\([^,]*\),.*$/\1/' | tail -1)
#
	if [[ -z "${chassistype}" ]]
	then
		echo
		echo "Unable to determine chassis type."
		echo
		echo "Skip provisioning ..."
		provision=no
		return 2
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
	sg_mg)
		prefix=sgmg
		;;
	sg_mgc_mg)
		prefix=
		;;
	*)
		echo
		echo "Unknown chassis type: <$chassistype>"
		echo
		echo "Skip provisioning ..."
		provision=no
		return 2
	esac
	#
	# get list of IOMs
	#
	iomlist=$(grep -i ent-eqpt ${scripts2exec} | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:\(iom-[0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
	#
	if [[ -z "${iomlist}" ]]
	then
		echo
		echo "Unable to determine list of IOMs."
		echo
		echo "Skip provisioning ..."
		provision=no
		return 2
	fi
	#
	echo
	echo "Chassis Type: $chassistype"
	echo "Prefix: $prefix"
	echo "IOM List: $iomlist"
	echo
	#
	duplexscript="/tmp/duplex.$$"
	simplexspascript="/tmp/simplexspa.$$"
	simplexspbscript="/tmp/simplexspb.$$"
	#
	#
	cat >$duplexscript <<EOF
###### START OF SECTION provision ######
provision exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v tl1sp rtrv-eqpt
EOF
	#
	cat >$simplexspascript <<EOF
###### START OF SECTION provision_spa ######
provision_spa exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v tl1sp rtrv-eqpt
EOF
	#
	cat >$simplexspbscript <<EOF
###### START OF SECTION provision_spb ######
provision_spb exec {
	waitforcpustate -t${provwaitforcpu} <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
	tl1 exec -i -v tl1sp rtrv-eqpt
EOF
	#
	firstcalldata=yes
	#
	for script in ${scripts2exec}
	do
		# do we know this script?
		ignoreflag=""
		#
		case "${script}" in
		*user*)
			echo
			echo "User script: ${script}"
			echo 
			echo "WARNING: DENYs are ignored during users scripts."
			ignoreflag=" -i"
			;;
		*hardware*)
			echo
			echo "Hardware script: ${script}"
			;;
		*iproute*)
			echo
			echo "IP Route script: ${script}"
			;;
		*calldata*)
			echo
			echo "Call Data script: ${script}"
			echo 
			echo "WARNING: DENYs are ignored during calldata scripts."
			ignoreflag=" -i"
			#
			if [[ "${dosginit}" == yes && "${firstcalldata}" == yes && "${prefix}" == sg ]]
			then
				# turn this off !!!
				firstcalldata=no
				# we need a boot here
				cat >>${duplexscript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspa
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	#
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
				cat >>${simplexspascript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet close appmode_telnetspa
	#
	waitforcpustate -t1800 <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	#
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
				cat >>${simplexspbscript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	#
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
			fi
			;;
		*prov*line*)
			echo
			echo "Provision Line Features script: ${script}"
			;;
		*cpy*mem*|*copy*mem*)
			echo
			echo "CPY-MEM script: ${script}"
			# skip these. the cpy-mems are generated
			# below.
			continue
			;;
		*call*line*)
			echo
			echo "Call Line: ${script}"
			echo 
			echo "WARNING: DENYs are ignored during call line scripts."
			ignoreflag=" -i"
			;;
		*)
			echo
			echo "Unknown script: ${script}"
			echo "Skipping script."
			continue
			;;
		esac
		# check for inits
		grep -i '^[^#]*init-sys:' ${script} 1>/dev/null
		if [[ ${?} == 0 ]]
		then
			echo
			echo "Script $script contains an init-sys cmd."
			initsys=yes
		else
			initsys=no
		fi
		# run script
		if [[ "${initsys}" == no ]]
		then
			# no init command, just run the commands
			echo "	tl1 file -v ${ignoreflag} tl1sp ${tl1dir}/${script}" >>${duplexscript}
			echo "	tl1 file -v ${ignoreflag} tl1sp ${tl1dir}/${script}" >>${simplexspascript}
			echo "	tl1 file -v ${ignoreflag} tl1sp ${tl1dir}/${script}" >>${simplexspbscript}
		else
			# strip out init-sys commands for simplex cases.
			grep -v init-sys: ${script} >/lcsl100/tmp/${script}.$$
			# init-sys command and duplex
			if [[ "${replaceinitsys}" == yes ]]
			then
				echo "	tl1 file -v ${ignoreflag} tl1sp /lcsl100/tmp/${script}.$$" >>${duplexscript}
				cat >>${duplexscript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	#
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspa
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	#
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
			else
				echo "	tl1 file -v ${ignoreflag} tl1sp ${tl1dir}/${script}" >>${duplexscript}
				echo "	tl1 close tl1sp" >>${duplexscript}
				#
				echo "	waitforcpustate -t1800 <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>" >>${duplexscript}
				echo "	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>" >>${duplexscript}
			fi
			#
			# we have an init-sys command and simplex. init-sys 
			# does not work with a simplex lab, therefore we 
			# need to reboot via a unix command.
			#
			# run temp files with init-sys cmds removed.
			#
			echo "	tl1 file -v ${ignoreflag} tl1sp /lcsl100/tmp/${script}.$$" >>${simplexspascript}
			echo "	tl1 file -v ${ignoreflag} tl1sp /lcsl100/tmp/${script}.$$" >>${simplexspbscript}
			#
			cat >>${simplexspascript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspa sync
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet close appmode_telnetspa
	#
	waitforcpustate -t1800 <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
			#
			cat >>${simplexspbscript} <<EOF
	tl1 close tl1sp
	#
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	telnet exec -i appmode_telnetspb sync
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>
EOF
		fi
		# any additional processing?
		case "${script}" in
		*user*)
			;;
		*hardware*)
			# is time for cpy-mem?
			#
			# hasioms=$(grep -i ent-eqpt ${script} | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:iom-\([0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
			#
			enteqptall_sec=$(grep -i ent-eqpt ${script} | grep -i "redundancy=sec" | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:iom-\([0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
			enteqptall_pri=$(grep -i ent-eqpt ${script} | grep -i "redundancy=pri" | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:iom-\([0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
			enteqpt_sec=$(grep -i ent-eqpt ${script} | grep -i ",sec," | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:iom-\([0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
			enteqpt_pri=$(grep -i ent-eqpt ${script} | grep -i ",pri," | grep -v '[	 ]*#' | tr '[A-Z]' '[a-z]' | sed -n 's/^.*:iom-\([0-9][0-9]*\):.*$/\1/p' | sort -u | tr '\n' ' ')
			#
			hasioms="${enteqptall_sec} ${enteqpt_sec} ${enteqptall_pri} ${enteqpt_pri}"
			hasioms="$(echo $hasioms | sed -n 's/  */ /p; s/^  *//p; s/   *$//p;')"
			#
			echo
			echo "List and order of IOMs to remove before CPY-MEM: ${hasioms}"
			#
			if [[ -n "${hasioms}" ]]
			then
				# iomlist1=$(echo ${hasioms} | cut -d' ' -f1-${maxcpymems} )
				# iomlist2=$(echo ${hasioms} | cut -d' ' -f${maxcpymems}- )
				#
				# start removing and updating all IOMS 
				#
				# sleep before removing the IOMs, allow
				# them to reach a stable inoperative
				# state, not initializing ...
				#
				echo "	sleep <sleepbeforermveqptiom>" >>${duplexscript}
				echo "	sleep <sleepbeforermveqptiom>" >>${simplexspascript}
				echo "	sleep <sleepbeforermveqptiom>" >>${simplexspbscript}
				#
				# close existing TL1 streams
				#
				echo "	tl1 close tl1sp" >>${duplexscript}
				echo "	tl1 close tl1sp" >>${simplexspascript}
				echo "	tl1 close tl1sp" >>${simplexspbscript}
				#
				# remove all IOMs in batch
				#
				echo "	tl1 rmvallioms -i -v tl1sp" >>${duplexscript}
				echo "	tl1 rmvallioms -i -v tl1sp" >>${simplexspascript}
				echo "	tl1 rmvallioms -i -v tl1sp" >>${simplexspbscript}
				#
				# cpy-mem ioms
				#
				if [[ "${updateioms}" == yes ]]
				then
					echo "	tl1 updateioms -i -v tl1sp ${maxcpymems}" >>${duplexscript}
					echo "	tl1 updateioms -i -v tl1sp ${maxcpymems}" >>${simplexspascript}
					echo "	tl1 updateioms -i -v tl1sp ${maxcpymems}" >>${simplexspbscript}
				fi
				#
				echo "	tl1 cpymems -i -v tl1sp ${maxcpymems}" >>${duplexscript}
				echo "	tl1 cpymems -i -v tl1sp ${maxcpymems}" >>${simplexspascript}
				echo "	tl1 cpymems -i -v tl1sp ${maxcpymems}" >>${simplexspbscript}
				#
				# restore all IOMs in batch
				#
				echo "	tl1 rstallioms -i -v tl1sp" >>${duplexscript}
				echo "	tl1 rstallioms -i -v tl1sp" >>${simplexspascript}
				echo "	tl1 rstallioms -i -v tl1sp" >>${simplexspbscript}
				#
				# reopen TL1 streams
				#
				echo "	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>" >>${duplexscript}
				echo "	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>" >>${simplexspascript}
				echo "	tl1 open2 -t30 tl1sp <spa.ip> <spb.ip> <spa.tl1login> <spa.tl1passwd>" >>${simplexspbscript}
			fi
			;;
		*iproute*)
			;;
		*calldata*)
			;;
		*prov*line*)
			;;
		*)
			echo
			echo "Unknown script: ${script}"
			echo "Skipping script."
			;;
		esac
	done
	# close up shop
	cat >>$duplexscript <<EOF
	tl1 close tl1sp
}
###### END OF SECTION provision ######
EOF
	#
	cat >>$simplexspascript <<EOF
	tl1 close tl1sp
}
###### END OF SECTION provision_spa ######
EOF
	#
	cat >>$simplexspbscript <<EOF
	tl1 close tl1sp
}
###### END OF SECTION provision_spb ######
EOF
	# 
	# put all script together, but first return to original
	# directory in case the outfile path is relative.
	#
	cd ${CURRDIR}
	#
	cat $duplexscript >>$outfile 
	cat $simplexspascript >>$outfile 
	cat $simplexspbscript >>$outfile 
	#
	return 0
}
#
writeinitsys () {
	# always write this section, but it is only called
	# by hand or for "provision-only".
	echo
	echo "Writing init-sys section ..."
	#
	duplexscript="/tmp/duplex.$$"
	simplexspascript="/tmp/simplexspa.$$"
	simplexspbscript="/tmp/simplexspb.$$"
	#
	cat >$duplexscript <<EOF
###### START OF SECTION initsys ######
initsys exec {
EOF
	#
	cat >$simplexspascript <<EOF
###### START OF SECTION initsys_spa ######
initsys_spa exec {
EOF
	#
	cat >$simplexspbscript <<EOF
###### START OF SECTION initsys_spb ######
initsys_spb exec {
EOF
	#
	cat >>${duplexscript} <<EOF
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	#
	telnet exec -i appmode_telnetspa /Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed
	telnet exec -i appmode_telnetspa rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspa rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspa sync
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	#
	telnet exec -i appmode_telnetspb /Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed
	telnet exec -i appmode_telnetspb rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspb rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspb sync
	#
	telnet exec -i appmode_telnetspa reboot -aN
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspa
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> isact,isstbyh <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isact,isstbyh <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
EOF
	#
	cat >>${simplexspascript} <<EOF
	telnet open appmode_telnetspa <spa.ip> <spa.login> <spa.passwd>
	#
	telnet exec -v -t60 appmode_telnetspa cd /
	telnet exec -i appmode_telnetspa /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspa /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	#
	telnet exec -i appmode_telnetspa /Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed
	telnet exec -i appmode_telnetspa rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspa rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspa sync
	#
	telnet exec -i appmode_telnetspa reboot -aN
	#
	telnet close appmode_telnetspa
	#
	waitforcpustate -t1800 <spa.ip> isactnbk <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> ignore <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
EOF
	#
	cat >>${simplexspbscript} <<EOF
	telnet open appmode_telnetspb <spb.ip> <spb.login> <spb.passwd>
	#
	telnet exec -v -t60 appmode_telnetspb cd /
	telnet exec -i appmode_telnetspb /bin/rm -f /net/rc.local
	telnet exec appmode_telnetspb /bin/chmod 755 /Telica/swCPU/CurrRel/system/scripts/telica.rc.network
	#
	telnet exec -i appmode_telnetspb /Telica/swCPU/CurrRel/system/scripts/remove_configuration_data proceed
	telnet exec -i appmode_telnetspb rm -f /Telica/localbackups/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspb rm -f /Telica/localbackups.old/* 1>/dev/null 2>&1
	telnet exec -i appmode_telnetspb sync
	#
	telnet exec -i appmode_telnetspb reboot -aN
	#
	telnet close appmode_telnetspb
	#
	waitforcpustate -t1800 <spa.ip> ignore <spa.login> <spa.passwd> <spa.tl1login> <spa.tl1passwd> <spb.ip> isactnbk <spb.login> <spb.passwd> <spb.tl1login> <spb.tl1passwd>
EOF
	# close up shop
	cat >>$duplexscript <<EOF
}
###### END OF SECTION initsys ######
EOF
	#
	cat >>$simplexspascript <<EOF
}
###### END OF SECTION initsys_spa ######
EOF
	#
	cat >>$simplexspbscript <<EOF
}
###### END OF SECTION initsys_spb ######
EOF
	#
	# put all script together, but first return to original
	# directory in case the outfile path is relative.
	#
	cd ${CURRDIR}
	#
	cat $duplexscript >>$outfile 
	cat $simplexspascript >>$outfile 
	cat $simplexspbscript >>$outfile 
	#
	return 0
}
#
############################################################################ 
#
# write the script
#
writefile
#
writeglobals
#
writepreload
#
writeminmode
#
writeappmode
#
writeremovetarfiles
#
writeremoveload
#
writeftpload
#
writeinstallnewload
#
writepostreload
#
writertrveqpt
#
writeprovisioning
#
writeinitsys
#
############################################################################
#
# execute reload script, if requested
#
if [[ -n "${useroutfile}" ]]
then
	cp ${outfile} ${useroutfile}
fi
#
if [[ "${execscript}" == yes ]]
then
	if [[ "${editscript}" == yes ]]
	then
		$EDITOR $outfile
	fi
	#
	if [[ "${interactive}" == yes ]]
	then
		# check if reload is in progress
		lcslockadm -P reload ${labid} 2>&1 | grep "Creating.*failed"
		if [[ $? == 0 ]]
		then
			echo
			echo "ERROR: Another reload of $labid in progress."
			exit 2
		fi
		# interactive mode
		logfile=/lcsl100/tmp/bupp.reload.${labid}.log.${suffix}
		#
		echo
		echo "LOG FILE: ${logfile}"
		#
		lcssyslog buppreload "STARTING RELOAD: ${labid}/${branch}/${cpuload} - interactive reload, logfile=${logfile} remoteuser=${remoteuser}."
		upsh -I -U $outfile 2>&1 | tee ${logfile}
		lcslockadm -V reload ${labid}
		lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - interactive reload, logfile=${logfile}."
	else
		#
		# decide which sections to execute.
		#
		if [[ "${mode}" == duplex ]]
		then
			sections="preload minmode removetarfiles removeload ftpload installnewload postreload appmode rtrveqpt"
			if [[ "${provision}" == yes ]]
			then
				if [[ "${provisiononly}" == yes ]]
				then
					sections="preload minmode initsys provision"
				else
					sections="$sections provision"
				fi
			elif [[ "${provisiononly}" == yes ]]
			then
				echo
				echo "ERROR: duplex provision-only was requested, but provisioning was disabled."
				lcslockadm -V reload ${labid}
				exit 2
			fi
		elif [[ "${mode}" == simplex ]]
		then
			sections="preload_${activesp} minmode_${activesp} removetarfiles_${activesp} removeload_${activesp} ftpload_${activesp} installnewload_${activesp} postreload_${activesp} appmode_${activesp} rtrveqpt_${activesp}"
			if [[ "${provision}" == yes ]]
			then
				if [[ "${provisiononly}" == yes ]]
				then
					sections="preload_${activesp} minmode_${activesp} initsys_${activesp} provision_${activesp}"
				else
					sections="$sections provision_${activesp}"
				fi
			elif [[ "${provisiononly}" == yes ]]
			then
				echo
				echo "ERROR: simplex provision-only was requested, but provisioning was disabled."
				lcslockadm -V reload ${labid}
				exit 2
			fi
		else
			echo
			echo "ERROR: Invalid mode: $mode"
			lcslockadm -V reload ${labid}
			exit 2
		fi
		#
		# 
		# reload lab. execute each section in series.
		#
		if [[ "${background}" == yes ]]
		then
			# check if we starting the reload immediately.
			if [[ "${attimespec}" == now ]]
			then
				# check if reload is in progress
				lcslockadm -P reload ${labid} 2>&1 | grep "Creating.*failed"
				if [[ $? == 0 ]]
				then
					echo
					echo "ERROR: Another reload of $labid in progress."
					exit 2
				fi
				lcslockadm -V reload ${labid}
			fi
			#
			atfile="/lcsl100/tmp/bupp.atjob.${labid}.${suffix}"
			atfilelog="/lcsl100/tmp/bupp.atjob.${labid}.log.${suffix}"
			#
			echo
			echo "LOG FILE: ${atfilelog}"
			#
			cat >${atfile} <<EOF
. /home/lcstools/tools/bin/setup_lcs
lcssyslog buppreload "STARTING RELOAD: lab ${labid} with ${branch}/${cpuload} - logfile is ${atfilelog} remoteuser=${remoteuser}."
# check if reload is in progress
lcslockadm -P reload ${labid} 2>&1 | grep "Creating.*failed"
if [[ \$? == 0 ]]
then
	echo >${atfilelog};
	echo "ERROR: Another reload of $labid in progress." >${atfilelog};
	chmod 777 ${atfilelog}
	lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Another reload of $labid in progress - logfile is ${atfilelog}."
	sendemail "${RELOADEMAIL}" ${labid} "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Another reload of $labid in progress - logfile is ${atfilelog}."
	exit 2
fi
#
lcssyslog buppreload "RELOAD: ${labid}/${branch}/${cpuload} - sections to run are: ${sections}."
for section in ${sections}
do
	echo
	echo "Executing section: \$section ..."
	#
	lcssyslog buppreload "RELOAD: ${labid}/${branch}/${cpuload} - running section \$section"
	upsh -U -s \$section -c 1 $outfile
	if [[ \$? -ne 0 ]]
	then
		echo
		echo "ERROR: Section \$section failed."
		chmod 777 ${atfilelog}
		lcslockadm -V reload ${labid}
		lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Section: \$section failed - logfile is ${atfilelog}."
		sendemail "${RELOADEMAIL}" ${labid} "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Section: \$section failed - logfile is ${atfilelog}."
		exit 2
	fi
done 1>${atfilelog} 2>&1
chmod 777 ${atfilelog}
lcslockadm -V reload ${labid}
lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - SUCCESS - logfile is ${atfilelog}."
sendemail "${RELOADEMAIL}" ${labid} "ENDING RELOAD: ${labid}/${branch}/${cpuload} - SUCCESS - logfile is ${atfilelog}."
exit 0
EOF
			chmod 777 ${atfile}
			at -k -f ${atfile} ${attimespec}
		else
			# check if reload is in progress
			lcslockadm -P reload ${labid} 2>&1 | grep "Creating.*failed"
			if [[ $? == 0 ]]
			then
				echo
				echo "ERROR: Another reload of $labid in progress."
				exit 2
			fi
			#
			logfile=/lcsl100/tmp/bupp.reload.${labid}.log.${suffix}
			#
			echo
			echo "LOG FILE: ${logfile}"
			#
			>${logfile}
			chmod 777 ${logfile}
			#
			lcssyslog buppreload "STARTING RELOAD: lab ${labid} with ${branch}/${cpuload} - log file is ${logfile} remoteuser=${remoteuser}."
			lcssyslog buppreload "RELOAD: ${labid}/${branch}/${cpuload} - sections to run are: ${sections}."
			for section in ${sections}
			do
				echo
				echo "Executing section: $section ..."
				lcssyslog buppreload "RELOAD: ${labid}/${branch}/${cpuload} - running section ${section} - logfile is ${logfile}."
				#
				upsh -U -s $section -c 1 $outfile 2>&1
				if [[ $? -ne 0 ]]
				then
					echo
					echo "ERROR: Section $section failed."
					lcslockadm -V reload ${labid}
					lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Section: $section failed - logfile is ${logfile}."
					sendemail "${email}" "ENDING RELOAD: ${labid}/${branch}/${cpuload} - FAILURE - Section: $section failed - logfile is ${logfile}."
					exit 2
				fi
			done | tee ${logfile}
			lcslockadm -V reload ${labid}
			lcssyslog buppreload "ENDING RELOAD: ${labid}/${branch}/${cpuload} - SUCCESS - logfile is ${logfile}."
			sendemail "${RELOADEMAIL}" ${labid} "ENDING RELOAD: ${labid}/${branch}/${cpuload} - SUCCESS - logfile is ${logfile}."
		fi
	fi
else
	echo "Done."
fi
#
exit 0

