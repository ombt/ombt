#!/usr/bin/ksh
#
CMD=${0}
#
usage() {
	echo "usage: $CMD [-x?] sp_log_tar_file [...] "
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
echo "Expanding SP Log TAR files: "
#
if [[ $# == 0 ]]
then
	echo 
	echo "No tar files given."
	exit 2
fi
#
for tarfile in ${*}
do
	echo
	echo "Extracting $tarfile ..."
	#
	case "${tarfile}" in
	*log.tar.gz*)
		case "$(file $tarfile)" in
		*gzip*compressed*)
			/opt/exp/gnu/bin/tar xzvf $tarfile
			;;
		*)
			echo "Skipping file $tarfile ..."
			;;
		esac
		;;
	*)
		echo "Skipping file, not an SP Log TAR file."
		;;
	esac
	#
done
#
exit 0
