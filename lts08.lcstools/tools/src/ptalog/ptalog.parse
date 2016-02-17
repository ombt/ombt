#!/usr/bin/ksh
#
# parse a PTA log file.
#
######################################################################
#
CMD=${0}
dbdir=/tmp
#
usage() {
	echo "usage: ${CMD} [-x?] [-d directory] db_name pta_log_html_file "
	return
}
#
export PATH=/usr/bin:${PATH}:/opt/exp/lib/unity/bin
#
set -- $(getopt ?xd: ${*})
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
	-d)
		dbdir=${2}
		if [[ ! -d "${dbdir}" ]]
		then
			echo
			echo "ERROR: ${dbdir} is not a directory."
			exit 2
		fi
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
echo
echo "Creating PTA LOG Unity Database:"
#
if [[ $# != 2 ]]
then
	echo
	echo "Incorrect number of files were given."
	usage
	exit 2
fi
#
dbname=${1}
htmlfname=${2}
#
# write descriptor files
#
dbname=$(basename ${dbname})
#
htmlrel=${dbdir}/${dbname}
htmldrel=${dbdir}/D${dbname}
htmlrel2=${dbdir}/${dbname}.min
htmldrel2=${dbdir}/D${dbname}.min
htmlrel3=${dbdir}/${dbname}.testresults
htmldrel3=${dbdir}/D${dbname}.testresults
#
echo
echo "Writing Unity descriptor file: ${htmldrel}"
#
cat >${htmldrel} <<EOF
recordno	t\\t	l
time	t\\t	l
thread	t\\t	l
level	t\\t	l
category	t\\t	l
type	t\\t	l
name	t\\t	l
message	t\\n	l
EOF
chmod 777 ${htmldrel}
#
echo
echo "Writing Unity descriptor file: ${htmldrel2}"
#
cat >${htmldrel2} <<EOF
recordno	t\\t	l
time	t\\t	l
level	t\\t	l
type	t\\t	l
name	t\\t	l
message	t\\n	l
EOF
chmod 777 ${htmldrel2}
#
echo
echo "Writing Unity descriptor file: ${htmldrel3}"
#
cat >${htmldrel3} <<EOF
srecordno	t\\t	l
stime	t\\t	l
slevel	t\\t	l
stype	t\\t	l
sname	t\\t	l
smessage	t\\t	l
erecordno	t\\t	l
etime	t\\t	l
elevel	t\\t	l
etype	t\\t	l
ename	t\\t	l
emessage	t\\t	l
status	t\\t	l
fail	t\\t	l
success	t\\t	l
inprog	t\\t	l
other	t\\n	l
EOF
chmod 777 ${htmldrel3}
#
echo
echo "Writing Unity relation: ${htmlrel}"
#
cp ${htmlfname} ${dbdir}/$(basename ${htmlfname})
#
cat ${htmlfname} |
nawk 'BEGIN {
	recordno = 0;
	#
	time = "";
	thread = "";
	level = "";
	category = "";
	type = "";
	name = "";
	message = "";
	#
	timeslen = length("<td title=\"Time\">");
	threadslen = length("<td title=\"Thread\">");
	levelslen = length("<td title=\"Level\">");
	categoryslen = length("<td title=\"Category\">");
	typeslen = length("<td title=\"Type\">");
	nameslen = length("<td title=\"Name\">");
	messageslen = length("<td title=\"Message\">");
	#
	tdelen = length("<\/td>");
}
$0 ~ /^<tr>/ {
	# beginning of a new record
	time = "";
	thread = "";
	level = "";
	category = "";
	type = "";
	name = "";
	message = "";
	# print ">>>>>>>>>>>>> START ROW DATA <<<<<<<<<<<<";
	next;
}
$0 ~ /^<td title="Time">/, $0 ~ /<\/td>/ {
	# print "TIME RAW RECORD - " $0;
	if ($0 ~ /^<td title="Time">/ && $0 ~ /<\/td>/) {
		# we have the entire record. clear all old data.
		time = "";
		thread = "";
		level = "";
		category = "";
		type = "";
		name = "";
		message = "";
		# get all data
		time = substr($0, timeslen+1, length($0)-timeslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Time">/) {
		# we have the beginning of the record. clear all data.
		time = "";
		thread = "";
		level = "";
		category = "";
		type = "";
		name = "";
		message = "";
		# get tail of data
		time = substr($0, timeslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		time = time " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		time = time " " $0;
	}
} 
$0 ~ /^<td title="Thread">/, $0 ~ /<\/td>/ {
	# print "THREAD RAW RECORD - " $0;
	if ($0 ~ /^<td title="Thread">/ && $0 ~ /<\/td>/) {
		# get all data
		thread = substr($0, threadslen+1, length($0)-threadslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Thread">/) {
		# get tail of data
		thread = substr($0, threadslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		thread = thread " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		thread = thread " " $0;
	}
} 
$0 ~ /^<td title="Level">/, $0 ~ /<\/td>/ {
	# print "LEVEL RAW RECORD - " $0;
	if ($0 ~ /^<td title="Level">/ && $0 ~ /<\/td>/) {
		# get all data
		level = substr($0, levelslen+1, length($0)-levelslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Level">/) {
		# get tail of data
		level = substr($0, levelslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		level = level " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		level = level " " $0;
	}
} 
$0 ~ /^<td title="Category">/, $0 ~ /<\/td>/ {
	# print "CATEGORY RAW RECORD - " $0;
	if ($0 ~ /^<td title="Category">/ && $0 ~ /<\/td>/) {
		# get all data
		category = substr($0, categoryslen+1, length($0)-categoryslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Category">/) {
		# get tail of data
		category = substr($0, categoryslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		category = category " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		category = category " " $0;
	}
} 
$0 ~ /^<td title="Type">/, $0 ~ /<\/td>/ {
	# print "TYPE RAW RECORD - " $0;
	if ($0 ~ /^<td title="Type">/ && $0 ~ /<\/td>/) {
		# get all data
		type = substr($0, typeslen+1, length($0)-typeslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Type">/) {
		# get tail of data
		type = substr($0, typeslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		type = type " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		type = type " " $0;
	}
} 
$0 ~ /^<td title="Name">/, $0 ~ /<\/td>/ {
	# print "NAME RAW RECORD - " $0;
	if ($0 ~ /^<td title="Name">/ && $0 ~ /<\/td>/) {
		# get all data
		name = substr($0, nameslen+1, length($0)-nameslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Name">/) {
		# get tail of data
		name = substr($0, nameslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		name = name " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		name = name " " $0;
	}
} 
$0 ~ /^<td title="Message">/, $0 ~ /<\/td>/ {
	# print "MESSAGE RAW RECORD - " $0;
	if ($0 ~ /^<td title="Message">/ && $0 ~ /<\/td>/) {
		# get all data
		message = substr($0, messageslen+1, length($0)-messageslen-tdelen);
		next;
	} else if ($0 ~ /^<td title="Message">/) {
		# get tail of data
		message = substr($0, messageslen+1);
	} else if ($0 ~ /<\/td>/) {
		# we have the end of the record. get the beginning of the data.
		message = message " " substr($0, 1, length($0)-tdelen);
		next;
	} else {
		# we have the middle of the record. just add to data.
		message = message " " $0;
	}
} 
$0 ~ /^<\/tr>/ {
	# end of a record
	# print ">>>>>>>>>>>>> DUMP ROW DATA <<<<<<<<<<<<";
	# print "\tTIME - " time;
	# print "\tTHREAD - " thread;
	# print "\tLEVEL - " level;
	# print "\tCATEGORY - " category;
	# print "\tTYPE - " type;
	# print "\tNAME - " name;
	# print "\tMESSAGE - " message;
	# print ">>>>>>>>>>>>> END ROW DATA <<<<<<<<<<<<";
	recordno += 1;
	print recordno "\t" time "\t" thread "\t" level "\t" category "\t" type "\t" name "\t" message;
	# clear data
	time = "";
	thread = "";
	level = "";
	category = "";
	type = "";
	name = "";
	message = "";
	next;
}
END {
} ' >${htmlrel}
chmod 777 ${htmlrel}
#
echo
echo "Writing Unity relation: ${htmlrel2}"
#
rm -f ${htmlrel2} 1>/dev/null 2>&1
uprintf -q -f"%s\t%s\t%s\t%s\t%s\t%s\n" recordno time level type name message in ${htmlrel} into ${htmlrel2}
chmod 777 ${htmlrel2}
#
echo
echo "Writing Unity relation: ${htmlrel3}"
#
cat ${htmlrel2} |
nawk 'BEGIN {
	FS = "\t";
	recordno = 0;
	session = "";
	#
	tssucc = 0;
	tsinprog = 0;
	tsfail = 0;
	tsother = 0;
	#
	tcsucc = 0;
	tcinprog = 0;
	tcfail = 0;
	tcother = 0;
}
$4 ~ /^SESSION START/ {
	ssrecord = $1;
	sstime = $2;
	sslevel = $3;
	sstype = $4;
	ssname = $5;
	ssmsg = $6;
	sstuple =  ssrecord "\t" sstime "\t" sslevel "\t" sstype "\t" ssname "\t" ssmsg;
	# print "RAW DATA: " sstuple;
	next;
}
$4 ~ /^SESSION END/ {
	serecord = $1;
	setime = $2;
	selevel = $3;
	setype = $4;
	sename = $5;
	semsg = $6;
	setuple = serecord "\t" setime "\t" selevel "\t" setype "\t" sename "\t" semsg;
	testresults = tsfail "\t" tssucc "\t" tsinprog "\t" tsother;
	# print "RAW DATA: " setuple;
	print sstuple "\t" setuple "\t" selevel "\t" testresults;
	next;
}
$4 ~ /^TEST CASE START/ {
	tcsucc = 0;
	tcinprog = 0;
	tcfail = 0;
	tcother = 0;
	#
	tcsrecord = $1;
	tcstime = $2;
	tcslevel = $3;
	tcstype = $4;
	tcsname = $5;
	tcsmsg = $6;
	tcstuple = tcsrecord "\t" tcstime "\t" tcslevel "\t" tcstype "\t" tcsname "\t" tcsmsg;
	# print "RAW DATA: " tcstuple;
	next;
}
$4 ~ /^TEST CASE END/ {
	tcerecord = $1;
	tcetime = $2;
	tcelevel = $3;
	tcetype = $4;
	tcename = $5;
	tcemsg = $6;
	tcetuple = tcerecord "\t" tcetime "\t" tcelevel "\t" tcetype "\t" tcename "\t" tcemsg;
	# print "RAW DATA: " tcetuple;
	testresults = tcfail "\t" tcsucc "\t" tcinprog "\t" tcother;
	print tcstuple "\t" tcetuple "\t" tcelevel "\t" testresults;
	next;
}
$4 ~ /^TEST ACTION START/ {
	tasrecord = $1;
	tastime = $2;
	taslevel = $3;
	tastype = $4;
	tasname = $5;
	tasmsg = $6;
	tastuple = tasrecord "\t" tastime "\t" taslevel "\t" tastype "\t" tasname "\t" tasmsg;
	# print "RAW DATA: " tastuple;
	next;
}
$4 ~ /^TEST ACTION END/ {
	taerecord = $1;
	taetime = $2;
	taelevel = $3;
	taetype = $4;
	taename = $5;
	taemsg = $6;
	taetuple = taerecord "\t" taetime "\t" taelevel "\t" taetype "\t" taename "\t" taemsg;
	#
	tafail = 0;
	tainprog = 0;
	tasucc = 0;
	taother = 0;
	#
	if (taemsg ~ /FAILURE/) {
		tcfail += 1;
		tsfail += 1;
		tafail = 1;
	} else if (taemsg ~ /IN PROGRESS/) {
		tcinprog += 1;
		tsinprog += 1;
		tainprog = 1;
	} else if (taemsg ~ /SUCCESS/) {
		tcsucc += 1;
		tssucc += 1;
		tasucc = 1;
	} else {
		tcother += 1;
		tsother += 1;
		taother = 1;
	}
	# print "RAW DATA: " taetuple;
	testresults = tafail "\t" tasucc "\t" tainprog "\t" taother;
	print tastuple "\t" taetuple "\t" taelevel "\t" testresults;
	next;
}
END {
} ' >${htmlrel3}
chmod 777 ${htmlrel3}
#
echo
echo "Done."
#
exit 0
