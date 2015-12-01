#!/usr/bin/ksh
#
CMD=${0##*/}
CMD=${CMD#lcs}
#
if [[ -z "${LCSTOOLSBIN}" ]]
then
	echo "LCSTOOLSBIN is not set." >&2
	exit 2
fi
#
if [[ ! -d "${LCSTOOLSBIN}" ]]
then
	echo "$LCSTOOLSBIN is not directory." >&2
	exit 2
fi
#
if [[ ! -x "${LCSTOOLSBIN}/suid" ]]
then
	echo "$LCSTOOLSBIN/suid is not executable or does not exist." >&2
	exit 2
fi
#
if [[ ! -x "${LCSTOOLSBIN}/suidtools/${CMD}" ]]
then
	echo "$LCSTOOLSBIN/suidtools/${CMD} is not executable or does not exist." >&2
	exit 2
fi
#
exec ${LCSTOOLSBIN}/suid ${LCSTOOLSBIN}/suidtools/${CMD} "${@}"
