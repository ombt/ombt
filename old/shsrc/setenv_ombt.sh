if [[ $# -ne 0 ]]
then
	cd ${1}
	ROOT=$(pwd)
	cd -
else
	ROOT=$(pwd)
	echo $ROOT
fi
export ROOT
#
LD_LIBRARY_PATH="${ROOT}/lib:${LD_LIBRARY_PATH}"
export LD_LIBRARY_PATH
#
PATH="${ROOT}/bin:${PATH}"
export PATH
