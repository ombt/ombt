#
# create a process tree graph
#
# need dt tool to run. check for dt in path.
#
dtloc="$(whence -v dt 2>&1)"
case "${dtloc}" in
"dt not found")
	echo "dt (draw tree) tool not found; should be under exptools." >&2
	exit 2
	;;
esac
#
# check for options and parameters.
#
if [[ "${1}" == "-?" ]]
then
	echo "usage: myptree [-?] [-q] [login]" >&2
	exit 2
fi
#
if [[ "${1}" == "-q" ]]
then
	PS3="choose a user: "
	select user in $(who | cut -d' ' -f1 | sort -u)
	do
		[[ -n "${user}" ]] && break
	done
	#
	shift 1
else
	user=${1:-$(logname)}
fi
#
if [[ -z "${user}" ]]
then
	echo "no user was given or determined." >&2
	echo "usage: myptree [-?] [-q] [login]" >&2
	exit 2
fi
#
# take a snap shot.
#
ps -fu ${user} >/tmp/psfu.$$
if [[ $? != 0 ]]
then
	echo "no processes found for user ${user}." >&2
	echo "usage: myptree [-?] [-q] [login]" >&2
	exit 2
fi
#
echo "user ${user} has $(cat /tmp/psfu.$$ | wc -l | sed 's/ //g;' ) processes."
sleep 1
#
# generate pid, ppid file.
#
cat /tmp/psfu.$$ |
sed -n '2,$p' | 
sed 's/  */ /g; s/^ //g;' | 
cut -d' ' -f3,2 | 
nawk '{ print $2 " " $1; }' >/tmp/edges.$$
#
# generate pid-to-cmd translation file.
#
cat /tmp/psfu.$$ |
sed -n '2,$p' | 
cut -c10-14 >/tmp/labels1.$$
#
cat /tmp/psfu.$$ |
sed -n '2,$p' | 
cut -c48- |
sed 's/ /_/g;' >/tmp/labels2.$$
#
paste /tmp/labels1.$$ /tmp/labels2.$$ |
nawk '{ print $1 " (" $1 ")_" $2 }' >/tmp/labels.$$
#
# draw tree using edges and labels files.
#
dt -a -x /tmp/labels.$$ /tmp/edges.$$
#
# remove tmp files.
#
rm -f /tmp/psfu.$$ /tmp/labels*.$$ /tmp/edges.$$
#
# all done.
#
exit 0
