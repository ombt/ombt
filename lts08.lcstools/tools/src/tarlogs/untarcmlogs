#!/usr/bin/ksh
#
# untar a CM tar logfile created using tarcmlogs.
#
###############################################################################
#
CMD=${0}
#
usage() {
	echo "usage: $CMD [-x?] cmtarfile [...] "
	echo
	echo "where:"
	echo "	-x - enable debug mode"
	echo "	-? - print usage message"
	echo
	echo "Tar files are expanded in the current directory."
}
#
set -- $(getopt ?x ${*})
if [[ ${?} -ne 0 ]]
then
	usage
	exit 0
fi
#
for opt in ${*}
do
	case "${opt}" in
	-x)
		set -x
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
echo
echo "Expanding CM TAR files: "
#
if [[ $# == 0 ]]
then
	echo 
	echo "No tar files were given."
	exit 2
fi
#
for tarfile in ${*}
do
	echo
	echo "Extracting $tarfile ..."
	#
	case "$(file $tarfile)" in
	*tar*archive*)
		tar xvf $tarfile
		;;
	*)
		echo "Skipping $tarfile ..."
		;;
	esac
done
#
cd home
#
for zipfile in *.gz
do
	echo
	echo "Unzipping $zipfile ..."
	case "$(file $zipfile)" in
	#
	*gzip*compressed*data*)
		gunzip $zipfile
		;;
	*)
		echo "Skipping $zipfile ..."
		;;
	esac
done
#
for tarfile in *log.tar
do
	echo
	echo "Extracting $tarfile ..."
	#
	case "$(file $tarfile)" in
	*tar*archive*)
		set -- $(echo $tarfile | sed 's/_/ /g')
		export cmcpudir="${1}/${2}/${3}"
		[ ! -d "${cmcpudir}" ] && mkdir -p ${cmcpudir}
		cd ${cmcpudir}
		tar xf ../../../$tarfile
		cd -
		;;
	*)
		echo
		echo "Skipping $tarfile ..."
		;;
	esac
done
#
exit 0
