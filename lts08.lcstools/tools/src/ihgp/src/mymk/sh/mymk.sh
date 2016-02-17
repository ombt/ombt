#!/usr/bin/ksh
#
# script to generate makefiles using an input file containing
# a list of targets and dependencies.
#
###############################################################################
#
# functions and aliases
#
unset ENV
PS4='[${LINENO}]+ '
#
alias vecho='[ "${verbose}" = "yes" ] && print -u2 "[${LINENO}] "'
#
isreadable() {
	if [ ! -r "${1}" ]
	then
		echo "WARNING: FILE ${1} NOT READABLE." >&2
		echo "         ASSUMING IT IS GENERATED." >&2
	fi
	return 0
}
#
vcat() {
	if [ "${verbose}" = "yes" ]
	then
		cat ${1} >&2
	fi
	#
	return 0
}
#
retrieve() {
	relation="${1}"
	key="${2}"
	shift 2
	#
	grep "^${key};" ${relation}
	#
	return ${?}
}
#
insert() {
	relation="${1}"
	key="${2}"
	shift 2
	#
	echo "${key};${*}" >>${relation}
	#
	return ${?}
}
#
delete() {
	relation="${1}"
	key="${2}"
	shift 2
	#
	newrelation="/tmp/newrel.$$"
	grep -v "^${key};" ${relation} >> ${newrelation}
	mv ${newrelation} ${relation}
	#
	return ${?}
}
#
update() {
	relation="${1}"
	key="${2}"
	shift 2
	#
	data="${*}"
	delete ${relation} ${key}
	insert ${relation} ${key} ${data}
	#
	return ${?}
}
#
initialize() {
	> ${targets}
	> ${deps}
	> ${targetdeps}
	> ${depdeps}
	> ${depspaths}
	> ${variables}
	> ${install}
	> ${filters}
	> ${tmpfile}
	#
	return 0
}
#
cleanup() {
	rm -f ${targets}
	rm -f ${deps}
	rm -f ${targetdeps}
	rm -f ${depdeps}
	rm -f ${depspaths}
	rm -f ${variables}
	rm -f ${install}
	rm -f ${filters}
	rm -f ${tmpfile}
	#
	rm -rf ${depdir}
	#
	return 0
}
#
# remove / from names
#
rmvslash() {
	echo "$(echo ${1} | tr '/' '_')"
	return 0
}
#
# defaults
#
usage="usage: ${0} [-xv] [make.in.file]"
verbose=no
makeinfile="make.in"
includedirs="."
makefile=makefile
#
# tmp files and relations
#
tmpfile="/tmp/tmpfile.$$"
targets="/tmp/targets.$$"
deps="/tmp/deps.$$"
targetdeps="/tmp/targetdeps.$$"
depdeps="/tmp/depdeps.$$"
depspaths="/tmp/depspaths.$$"
variables="/tmp/variables.$$"
install="/tmp/install.$$"
filters="/tmp/filters.$$"
#
depdir="/tmp/depdir$$"
#
# set up trap handler
#
trap 'cleanup 1>/dev/null 2>&1; trap 0; exit 0;' 0 1 2 15
# 
# get command line options
#
set -- `getopt ?o:xv $*`
#
if [ $? -ne 0 ]
then
	echo ${usage} >&2
	exit 2
fi
#
for i in ${*}
do
	case $i in
	-x)
		set -x
		shift
		;;
	-o)
		makefile=${2}
		shift 2
		;;
	-v)
		verbose=yes
		shift
		;;
	--)
		shift
		break
		;;
	esac
done
#
# check if make.in file was given.
#
if [ $# -ne 0 ]
then
	makeinfile="${1}"
	echo "using make.in file '${makeinfile}' ..." >&2
else
	echo "using default make.in file ..." >&2
fi
#
if [ ! -r "${makeinfile}" ]
then
	echo "file ${makeinfile} is not readable." >&2
	exit 2
fi
#
# output file
#
exec 1>${makefile}
#
# create data file
#
initialize
#
# generate default variables
#
insert ${variables} "AR" "/usr/ccs/bin/ar"
insert ${variables} "ARFLAGS" "-r -v"
insert ${variables} "MAKE" "make"
insert ${variables} "YACC" "/usr/ccs/bin/yacc"
insert ${variables} "YACCFLAGS" "-d -v"
insert ${variables} "LEX" "/usr/ccs/bin/lex"
insert ${variables} "LEXFLAGS" "-n"
insert ${variables} "LD" "cc"
insert ${variables} "LDFLAGS"
insert ${variables} "CC" "cc"
insert ${variables} "CCFLAGS" "-g"
insert ${variables} "INCLDIRS"
insert ${variables} "SHELLBIN" '$(HOME)/bin'
insert ${variables} "BACKUPDIR" 'BACKUP'
insert ${variables} "LPR" 'mycpr2on1'
insert ${variables} "LPRFLAGS" 
#
# create relations
#
vecho "RELATION ${variables} IS"
vcat ${variables}
#
target=
targettype=
#
cat ${makeinfile}  |
grep -v '^[ 	]*#' |
while read inline
do
	vecho "PROCESSING LINE: ${inline}"
	#
	case "${inline}" in
	filterout=*)
		data=${inline#*=}
		insert ${filters} "${target}" "${data}"
		;;
	*.o)
		target=${inline}
		targettype="OBJECT"
		insert ${targets} "${target}" "${targettype}"
		;;
	*.a)
		target=${inline}
		targettype="ARCHIVE"
		#
		insert ${targets} "${target}" "${targettype}"
		;;
	*.sh)
		isreadable ${inline}
		#
		targettype="SHELLSCRIPT"
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "SHELLFILE"
		update ${targets} "${target}" "${targettype}"
		;;
	*.c|*.cpp|*.cxx|*.C|*.CPP)
		isreadable ${inline}
		#
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "CFILE"
		;;
	*.l)
		isreadable ${inline}
		#
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "LEXFILE"
		;;
	*.y)
		isreadable ${inline}
		#
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "YACCFILE"
		;;
	*.h|*.hxx|*.HPP)
		isreadable ${inline}
		#
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "HDRFILE"
		;;
	-l*)
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "LIBFILE"
		;;
	-*)
		insert ${targetdeps} "${target}" "${inline}"
		update ${deps} "${inline}" "OPTION"
		;;
	install[0-9]*=*)
		key=${inline%%=*}
		keyno=${key#install}
		data=${inline#*=}
		update ${install} "${target}${keyno}" "${data}"
		;;
	*=*)
		key=${inline%%=*}
		data=${inline#*=}
		update ${variables} "${key}" "${data}"
		;;
	\#*)
		;;
	*)
		target=${inline}
		targettype="BINARY"
		#
		insert ${targets} "${target}" "${targettype}"
		;;
	esac
done
#
vecho "RELATION ${targets} IS"
vcat ${targets}
vecho "RELATION ${deps} IS"
vcat ${deps}
vecho "RELATION ${targetdeps} IS"
vcat ${targetdeps}
vecho "RELATION ${variables} IS"
vcat ${variables}
vecho "RELATION ${install} IS"
vcat ${install}
vecho "RELATION ${filters} IS"
vcat ${filters}
#
# set include directories
#
export includedirs
#
set -- $(retrieve ${variables} INCLDIRS | cut -d';' -f2-100)
set -- $(getopt I: ${*})
#
if [ $? -ne 0 ]
then
	echo "unable to parse include directories." >&2
	exit 2
fi
#
includedirs="."
vecho "DEFAULT INCLUDE DIRECTORIES: ${includedirs}"
#
for i in ${*}
do
	case "${i}" in
	-I)
		vecho "ADDING INCLUDE DIRECTORY: ${2}"
		includedirs="${includedirs} ${2}"
		shift 2
		;;
	--)
		shift
		break
		;;
	esac
done
#
vecho "FINAL INCLUDE DIRECTORIES: ${includedirs}"
#
# clear dependencies file
#
>${depdeps}
#
# create repository for dependencies files
#
mkdir ${depdir}
#
# create initial jobs in queue
#
vecho "CREATING INITIAL JOBS"
#
cat ${deps} |
sed 's/;/ /' |
while read file filetype
do
	#
	# create entry of file to be scanned
	#
	case "${filetype}" in
	OPTION|ARCHIVE|LIBFILE|SHELLFILE)
		# skip these entries
		;;
	*)
		vecho "CREATING SCAN JOB FOR: ${file}"
		touch ${depdir}/$(rmvslash ${file}.scan)
		#
		# create entry of header file to have path determined
		#
		if [ "${filetype}" = "HDRFILE" ]
		then
			vecho "CREATING PATH JOB FOR: ${file}"
			touch ${depdir}/$(rmvslash ${file}.path)
		fi
		;;
	esac
done
#
# process all jobs in queue, i.e., all files with zero length.
#
vecho "PROCESSING JOBS"
#
Done=no
#
while [ "${Done}" = "no" ]
do
	#
	Done=yes
	#
	# determine jobs to process
	#
	pathjobs="$(ls ${depdir} | grep '\.path$')"
	scanjobs="$(ls ${depdir} | grep '\.scan$')"
	#
	vecho "ALL SCAN JOBS: ${scanjobs}"
	vecho "ALL PATH JOBS: ${pathjobs}"
	#
	for pathjob in ${pathjobs}
	do
		#
		# check if length is zero
		#
		if [ -s "${depdir}/${pathjob}" ]
		then
			vecho "SKIPPING PATH JOB: ${pathjob}"
			continue
		else
			vecho "PROCESSING PATH JOB: ${pathjob}"
		fi
		#
		Done=no
		#
		# found a job to process, determine path to header file
		#
		found=no
		hfile="${pathjob%.path}"
		#
		for idir in ${includedirs}
		do
			if [ -r "${idir}/${hfile}" ]
			then
				vecho "FOUND PATH ${pathjob}: ${idir}/${hfile}"
				echo "${idir}/${hfile}" > ${depdir}/${pathjob}
				found=yes
				break
			fi
		done
		#
		# for the case of generated headers, i.e., yacc headers
		#
		if [ "${found}" = "no" ]
		then
			vecho "DEFAULT PATH ${pathjob}: ${hfile}"
			echo "WARNING: FILE ${hfile} NOT READABLE." >&2
			echo "         ASSUMING IT IS GENERATED." >&2
			echo "./${hfile}" > ${depdir}/${pathjob}
		fi
	done
	#
	for scanjob in ${scanjobs}
	do
		#
		# check if length is zero
		#
		if [ -s "${depdir}/${scanjob}" ]
		then
			vecho "SKIPPING SCAN JOB: ${scanjob}"
			continue
		else
			vecho "PROCESSING SCAN JOB: ${scanjob}"
		fi
		#
		Done=no
		#
		# scan for dependent files
		#
		file="${scanjob%.scan}"
		#
		# is path known for header file?
		#
		case "${file}" in
		*.h)
			vecho "HEADER FILE: ${file}"
			if [ -s "${depdir}/${file}.path" ]
			then
				filepath="$(cat ${depdir}/${file}.path)"
				vecho "HEADER FILEPATH: ${filepath}"
			else
				echo "PATH NOT KNOWN FOR ${file}" >&2
				exit 2
			fi
			;;
		*)
			# all other files
			filepath="${file}"
			vecho "FILEPATH: ${filepath}"
			;;
		esac
		#
		# check for created headers, like yacc headers.
		#
		if [ -s "${filepath}" ]
		then
			#
			# 2/5/03 NOTE: need to grep for *.h and *.c
			# since the g++ compiler expects the source 
			# to be in the header for templates and the 
			# easiest way to include a separate c-file 
			# in a header is to simply #include the c-file.
			#
			cat ${filepath} |
			sed -n 's/^.*#[	 ]*include[	 ]*"\([^"]*\.[ch]\)".*$/\1/p' |
			sort -u >${depdir}/${scanjob}
		else
			> ${depdir}/${scanjob}
		fi
		#
		#
		vecho "FILE DEPENDENCIES FOR ${filepath}"
		#
		# there may files with NO include files. check for them.
		#
		if [ ! -s "${depdir}/${scanjob}" ]
		then
			echo "NO INCLUDE FILES" > ${depdir}/${scanjob}
			vcat ${depdir}/${scanjob}
		else
			vcat ${depdir}/${scanjob}
			#
			# any new jobs to queue
			#
			vecho "CHECKING FOR NEW JOBS: ${filepath}"
			#
			cat ${depdir}/${scanjob} |
			while read dfile
			do
				if [ ! -s "${depdir}/${dfile}.path" ] 
				then
					touch ${depdir}/$(rmvslash ${dfile}.path)
					vecho "NEW PATH JOB: ${depdir}/${dfile}.path"
				fi
				if [ ! -s "${depdir}/${dfile}.scan" ]
				then
					touch ${depdir}/$(rmvslash ${dfile}.scan)
					vecho "NEW SCAN JOB: ${depdir}/${dfile}.scan"
				fi
			done
		fi
	done
done
#
# create depdeps and depspaths relations
#
vecho "CREATING DEPDEPS AND DEPSPATHS RELATIONS"
#
cd ${depdir}
#
ls | nawk 'BEGIN {
	verbose = "'"${verbose}"'";
	depspathsrel = "'"${depspaths}"'";
	depdepsrel = "'"${depdeps}"'";
}
$1 ~ /^.*\.path$/ {
	fname = substr($1, 1, (length($1)-length(".path")));
	depspaths[fname] = fname;
	getline tuple < $1;
	close($1);
	depspaths[fname] = depspaths[fname] ";" tuple;
	trace("pathpattern 10", "depspaths[" fname "] is " depspaths[fname]);
	next;
}
$1 ~ /^.*\.scan$/ {
	fname = substr($1, 1, (length($1)-length(".scan")));
	depdeps[fname] = fname;
	while (getline tuple < $1 > 0)
	{
		depdeps[fname] = depdeps[fname] ";" tuple;
	}
	close($1);
	trace("scanpattern 10", "depdeps[" fname "] is " depdeps[fname]);
	next;
}
END {
	trace("END 10", "depdeps is ...");
	for (dep in depdeps)
	{
		trace("END 15", "depdeps[" dep "] is " depdeps[dep]);
		print depdeps[dep] >> depdepsrel;
	}
	close(depdepsrel);
	#
	trace("END 20", "depspaths is ...");
	for (dep in depspaths)
	{
		trace("END 25", "depspaths[" dep "] is " depspaths[dep]);
		print depspaths[dep] >> depspathsrel;
	}
	close(depspathsrel);
}
function trace(lineno, msg) {
	if (verbose == "yes")
	{
		printf("nawk [%s] %s\n", lineno, msg) | "cat 1>&2"
	}
	return;
}'
#
cd - 1>/dev/null
#
vecho "RELATION ${depdeps} IS"
vcat ${depdeps}
vecho "RELATION ${depspaths} IS"
vcat ${depspaths}
#
# write make file
#
vecho "CREATING MAKE FILE"
#
ls |
nawk 'BEGIN {
	#
	verbose = "'"${verbose}"'";
	#
	# relations file names
	#
	targetsrel = "'"${targets}"'";
	targetdepsrel = "'"${targetdeps}"'";
	depsrel = "'"${deps}"'";
	depdepsrel = "'"${depdeps}"'";
	depspathsrel = "'"${depspaths}"'";
	variablesrel = "'"${variables}"'";
	installrel = "'"${install}"'";
	#
	trace("BEGIN 10", "targetsrel is ... " targetsrel);
	trace("BEGIN 10", "targetdepsrel is ... " targetdepsrel);
	trace("BEGIN 10", "depsrel is ... " depsrel);
	trace("BEGIN 10", "depdepsrel is ... " depdepsrel);
	trace("BEGIN 10", "depspathsrel is ... " depspathsrel);
	trace("BEGIN 10", "variablesrel is ... " variablesrel);
	#
	# read in list of targets
	#
	trace("BEGIN 20", "reading in targets ...");
	while (getline tuple < targetsrel > 0)
	{
		split(tuple, fields, ";");
		targets[fields[1]] = "";
		trace("BEGIN 25", "target is ..." fields[1]);
		targettype[fields[1]] = fields[2];
		trace("BEGIN 26", "targettype[" fields[1] "] is " targettype[fields[1]]);
		if (fields[2] == "COLLECTOR")
			collectors[fields[1]] = fields[1];
	}
	close(targetsrel);
	#
	# read in list of target dependencies
	#
	trace("BEGIN 30", "reading in targetdeps ...");
	while (getline tuple < targetdepsrel > 0)
	{
		split(tuple, fields, ";");
		targets[fields[1]] = targets[fields[1]] " " fields[2];
		trace("BEGIN 35", "targetdeps[" fields[1] "] is " targets[fields[1]]);
	}
	close(targetdepsrel);
	#
	# read in list of dependent files
	#
	trace("BEGIN 40", "reading in depsrel ...");
	while (getline tuple < depsrel > 0)
	{
		split(tuple, fields, ";");
		deps[fields[1]] = fields[2];
		trace("BEGIN 45", "deps[" fields[1] "] is " deps[fields[1]]);
	}
	close(depsrel);
	#
	# read in list of dependent files dependencies
	#
	trace("BEGIN 50", "reading in depdepsrel ...");
	while (getline tuple < depdepsrel > 0)
	{
		nfld = split(tuple, fields, ";");
		for (fld = 2; fld <= nfld; fld++)
		{
			if (fld > 2)
				depdeps[fields[1]] = depdeps[fields[1]] ";" fields[fld];
			else
				depdeps[fields[1]] = fields[fld];

		}
		trace("BEGIN 55", "depdeps[" fields[1] "] is " depdeps[fields[1]]);
	}
	close(depdepsrel);
	#
	# read in dependent files paths
	#
	trace("BEGIN 60", "reading in depspathsrel ...");
	while (getline tuple < depspathsrel > 0)
	{
		split(tuple, fields, ";");
		depspaths[fields[1]] = fields[2];
		trace("BEGIN 65", "depspaths[" fields[1] "] is " depspaths[fields[1]]);
	}
	close(depspathsrel);
	#
	# read in variables
	#
	trace("BEGIN 70", "reading in variablesrel ...");
	while (getline tuple < variablesrel > 0)
	{
		split(tuple, fields, ";");
		variables[fields[1]] = fields[2];
		trace("BEGIN 75", "variables[" fields[1] "] is " variables[fields[1]]);
	}
	close(variablesrel);
	#
	# read in install lines
	#
	trace("BEGIN 80", "reading in installrel ...");
	while (getline tuple < installrel > 0)
	{
		split(tuple, fields, ";");
		install[fields[1]] = fields[2];
		trace("BEGIN 85", "install[" fields[1] "] is " install[fields[1]]);
	}
	close(installrel);
	#
	# start writing makefile
	#
	written["NULL"] = "";
	#
	trace("BEGIN 90", "writing make file ...");
	#
	writesuffixes();
	writevariables();
	writesuffixrules();
	writealltarget();
	writetargets();
	writedeps();
	writecleanup();
	writebackup();
	writeprint();
	#
	trace("BEGIN 100", "all done ...");
}
function trace(lineno, msg) {
	if (verbose == "yes")
	{
		printf("nawk [%s] %s\n", lineno, msg) | "cat 1>&2"
	}
	return;
}
function writesuffixes() {
	trace("writesuffixes 10 ", "entry ...");
	printf("\n");
	printf(".SUFFIXES:\t\t.o .C .CPP .c .cpp\n");
	# printf(".SUFFIXES:\t\t.o .c .l .y\n");
	return;
}
function writevariables() {
	trace("writevariables 10 ", "entry ...");
	printf("\n");
	for (variable in variables)
	{
		printf("%s = %s\n", variable, variables[variable]);
	}
	printf("ENDLIST = \n");
	return;
}
function writesuffixrules() {
	trace("writesuffixrules 10 ", "entry ...");
	#
	# C files
	#
	printf("\n");
	printf(".c.o:\n");
	printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $<\n");
	printf(".C.o:\n");
	printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $<\n");
	printf(".cpp.o:\n");
	printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $<\n");
	printf(".CPP.o:\n");
	printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $<\n");
	#
	# yacc files
	#
	# printf("\n");
	# printf(".y.o:\n");
	# printf("\t\t$(YACC) $(YACCFLAGS) $<\n");
	# printf("\t\tmv y.tab.h $(<:.y=yacc.h)\n");
	# printf("\t\tmv y.tab.c $(<:.y=yacc.c)\n");
	# printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $(<:.y=yacc.c)\n");
	#
	# lex files
	#
	# printf("\n");
	# printf(".l.o:\n");
	# printf("\t\t$(LEX) $(LEXFLAGS) $<\n");
	# printf("\t\tmv lex.yy.c $(<:.l=lex.c)\n");
	# printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c $(<:.l=lex.c)\n");
	#
	return;
}
function writealltarget() {
	trace("writealltarget 10 ", "entry ...");
	#
	# write all target.
	#
	printf("\nall:");
	#
	for (target in targets)
	{
		printf("\t\t%s \\\n", target);
	}
	printf("\t\t$(ENDLIST)\n");
	printf("\t\ttouch all\n");
	return;
}
function writetargets() {
	trace("writetargets 10 ", "entry ...");
	#
	# write builds for targets.
	#
	for (target in targets)
	{
		if (targettype[target] == "BINARY")
		{
			printf("\n%s:", target);
			#
			targetno = split(targets[target], targetlist);
			for (itarget = 1; itarget <= targetno; itarget++)
			{
				if (targetlist[itarget] ~ /^-.*$/)
				{
					# skip options in dependency list
					continue;
				}
				printf("\t\t%s \\\n", t2o(targetlist[itarget]));
			}
			printf("\t\t$(ENDLIST)\n");
			#
			printf("\t\t$(LD) $(LDFLAGS) -o %s \\\n", target);
			for (itarget = 1; itarget <= targetno; itarget++)
			{
				printf("\t\t\t%s \\\n", t2o(targetlist[itarget]));
			}
			# moreccopts();
			printf("\t\t\t$(ENDLIST)\n");
		}
		else if (targettype[target] == "OBJECT")
		{
			# skip this entry
			continue;
		}
		else if (targettype[target] == "SHELLSCRIPT")
		{
			printf("\n%s:", target);
			#
			split(targets[target], targetlist);
			printf("\t\t%s \\\n", targetlist[1]);
			printf("\t\t$(ENDLIST)\n");
			#
			printf("\t\t-rm -f $(SHELLBIN)/%s\n", target);
			printf("\t\tcp %s %s\n", targetlist[1], target);
			printf("\t\tchmod +x %s\n", target);
			printf("\t\tcp %s $(SHELLBIN)/%s\n", target, target);
		}
		else if (targettype[target] == "ARCHIVE")
		{
			printf("\n%s:", target);
			#
			targetno = split(targets[target], targetlist);
			for (itarget = 1; itarget <= targetno; itarget++)
			{
				printf("\t\t%s \\\n", t2o(targetlist[itarget]));
			}
			printf("\t\t$(ENDLIST)\n");
			#
			printf("\t\t$(AR) $(ARFLAGS) %s \\\n", target);
			for (itarget = 1; itarget <= targetno; itarget++)
			{
				printf("\t\t\t%s \\\n", t2o(targetlist[itarget]));
			}
			printf("\t\t\t$(ENDLIST)\n");
			for (itarget = 1; itarget <= targetno; itarget++)
			{
				printf("\t\trm -f %s\n", t2o(targetlist[itarget]));
			}
		}
		itarget = 1;
		installtarget = target itarget;
		while (installtarget in install)
		{
			printf("\t\t%s\n", install[installtarget]);
			itarget += 1;
			installtarget = target itarget;
		}
	}
	return;
}
function writedeps(idep, ndeps, dep, depslist) {
	trace("writedeps 10 ", "entry ...");
	for (dep in deps)
	{
		# check for shell scripts
		if ((deps[dep] == "SHELLFILE") || (deps[dep] == "LIBFILE") ||
		    (deps[dep] == "OPTION"))
		{
			# nothing to do
			continue;
		}
		else
		{
			#
			# default case of object files
			#
			printf("\n%s:", t2o(dep));
			printf("\t%s \\\n", dep);
			ndeps = split(depdeps[dep], depslist, ";");
			for (idep=1; idep <= ndeps; idep++)
			{
				tracedeps(depslist[idep]);
			}
			#
			# check if there is a matching yacc file.
			#
			if ((deps[dep] == "LEXFILE") && (l2y(dep) in deps))
			{
				printf("\t\t%s \\\n", l2y(dep));
			}
			printf("\t\t$(ENDLIST)\n");
			#
			# check for yacc or lex files
			#
			if (deps[dep] == "YACCFILE")
			{
				printf("\t\t$(YACC) $(YACCFLAGS) %s\n", dep);
				printf("\t\tmv y.tab.h %s\n", y2h(dep));
				printf("\t\tmv y.tab.c %s\n", y2c(dep));
				printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c %s\n", y2c(dep));
			}
			else if (deps[dep] == "LEXFILE")
			{
				printf("\t\t$(LEX) $(LEXFLAGS) %s\n", dep);
				printf("\t\tmv lex.yy.c %s\n", l2c(dep));
				printf("\t\t$(CC) $(CCFLAGS) $(INCLDIRS) -c %s\n", l2c(dep));
			}
			clearwritten();
		}
	}
	return;
}
function y2h(yfile, hfile) {
	trace("y2h 10 ", "entry ... " yfile);
	hfile = yfile;
	sub("\.y$", "yacc.h", hfile);
	return(hfile);
}
function y2c(yfile, cfile) {
	trace("y2c 10 ", "entry ... " yfile);
	cfile = yfile;
	sub("\.y$", "yacc.c", cfile);
	return(cfile);
}
function l2c(lfile, cfile) {
	trace("l2c 10 ", "entry ... " lfile);
	cfile = lfile;
	sub("\.l$", "lex.c", cfile);
	return(cfile);
}
function l2y(lfile, yfile) {
	trace("l2y 10 ", "entry ... " lfile);
	yfile = lfile;
	sub("\.l$", ".y", yfile);
	return(yfile);
}
function tracedeps(dep, idep, ndeps, depslist) {
	trace("tracedeps 10 ", "entry ... " dep);
	if ( ! (dep in written))
	{
		written[dep] = dep;
		if (dep ~ /^.*yacc\.h$/ || dep ~ /NO INCLUDE FILE/)
			continue;
		printf("\t\t%s \\\n", depspaths[dep]);
		if (depdeps[dep] !~ "NO INCLUDE FILE")
		{
			ndeps = split(depdeps[dep], depslist, ";");
			for (idep=1; idep <= ndeps; idep++)
			{
				tracedeps(depslist[idep]);
			}
		}
	}
	return;
}
function clearwritten(fname) {
	trace("clearwritten 10 ", "entry ...");
	for (fname in written)
	{
		delete written[fname];
	}
	return;
}
function t2o(infile) {
	trace("t2o 10 ", "entry ... " infile);
	outfile = infile;
	if (deps[infile] == "CFILE")
	{
		sub("\.c$", ".o", outfile);
	}
	else if (deps[infile] == "LEXFILE")
	{
		sub("\.l$", "lex.o", outfile);
	}
	else if (deps[infile] == "YACCFILE")
	{
		sub("\.y$", "yacc.o", outfile);
	}
	return(outfile);
}
function moreccopts() {
	trace("moreccopts 10 ", "entry ...");
	#
	yaccflag = "no";
	lexflag = "no";
	#
	for (itarget = 1; itarget <= targetno; itarget++)
	{
		if (deps[targetlist[itarget]] == "YACCFILE")
		{
			yaccflag = "yes";
		}
		else if (deps[targetlist[itarget]] == "LEXFILE")
		{
			lexflag = "yes";
		}
	}
	if (lexflag == "yes")
		printf("\t\t\t-ll \\\n");
	if (yaccflag == "yes")
		printf("\t\t\t-ly \\\n");
	return;
}
function writecleanup() {
	trace("writecleanup 10 ", "entry ...");
	#
	# write cleanup for targets.
	#
	printf("\n");
	printf("clean:\n");
	#
	# if C++, then remove templates repository
	#
	if ((variables["CC"] ~ /^.*CC$/) || 
	    (variables["CC"] ~ /^.*c\+\+$/) ||
	    (variables["CC"] ~ /^.*g\+\+$/))
	{
		# printf("\t\t-rm -rf Templates.DB\n");
		printf("\t\t-([ -d \"Templates.DB\" ] && rm -rf Templates.DB;)\n");
		printf("\t\t-([ -d \"SunWS_cache\" ] && rm -rf SunWS_cache;)\n");
	}
	#
	printf("\t\t-([ -f \"ir.out\" ] && rm -rf ir.out;)\n");
	#
	for (target in targets)
	{
		if ((targettype[target] != "OBJECT") &&
		    (target !~ /^[ 	]*$/))
			printf("\t\t-rm -f %s\n", target);
	}
	for (dep in deps)
	{
		if ((deps[dep] == "SHELLFILE") || (deps[dep] == "LIBFILE") ||
		    (deps[dep] == "OPTION"))
		{
			# do not delete the shell script or libraries
			continue;
		}
		else
		{
			# delete the object file
			if (t2o(dep) !~ /^[ 	]*$/)
					printf("\t\t-rm -f %s\n", t2o(dep));
		}
	}
}
function writebackup() {
	trace("writebackup 10 ", "entry ...");
	#
	# write backup for targets.
	#
	printf("\n");
	printf("backup:");
	#
	for (dep in deps)
	{
		if ((deps[dep] != "LIBFILE") && (deps[dep] != "OPTION"))
			printf("\t\t%s \\\n", dep);
	}
	#
	backupdirs["."] = ".";
	#
	for (dep in depspaths)
	{
		printf("\t\t%s \\\n", depspaths[dep]);
		backupdirs[dirname(depspaths[dep])] = dirname(depspaths[dep]);
	}
	printf("\t\t$(ENDLIST)\n");
	#
	# create backup directories
	#
	for (backupdir in backupdirs)
	{
		printf("\t\t-([ ! -d \"%s/$(BACKUPDIR)\" ] && mkdir %s/$(BACKUPDIR);)\n", backupdir, backupdir);
	}
	#
	# copy dependent files files to backup directory
	#
	for (dep in deps)
	{
		if ((deps[dep] != "LIBFILE") && (deps[dep] != "OPTION"))
		{
			printf("\t\tcp %s $(BACKUPDIR)/%s\n", dep, dep);
		}
	}
	for (dep in depspaths)
	{
		printf("\t\tcp %s %s/$(BACKUPDIR)/%s\n", depspaths[dep], dirname(depspaths[dep]), dep);
	}
	printf("\t\ttouch backup\n");
	return;
}
function writeprint() {
	trace("writeprint 10 ", "entry ...");
	#
	# write backup for targets.
	#
	printf("\n");
	printf("print:");
	#
	for (dep in deps)
	{
		if ((deps[dep] != "LIBFILE") && (deps[dep] != "OPTION"))
			printf("\t\t%s \\\n", dep);
	}
	#
	for (dep in depspaths)
	{
		printf("\t\t%s \\\n", depspaths[dep]);
	}
	printf("\t\t$(ENDLIST)\n");
	#
	# print dependent files 
	#
	printf("\t\t$(LPR) $(LPRFLAGS) \\\n");
	#
	for (dep in deps)
	{
		if ((deps[dep] != "LIBFILE") && (deps[dep] != "OPTION"))
			printf("\t\t\t%s \\\n", dep);
	}
	#
	for (dep in depspaths)
	{
		printf("\t\t\t%s \\\n", depspaths[dep]);
	}
	printf("\t\t\t$(ENDLIST)\n");
	return;
}
function basename(base) {
	trace("basename 10 ", "entry ... " base);
	match(base, /[^\/]+$/);
	base = substr(base, RSTART, RLENGTH);
	return(base);
}
function dirname(dir) {
	trace("dirname 10 ", "entry ... " dir);
	if (match(dir, /^.*\//) > 0)
		dir = substr(dir, RSTART, RLENGTH-1);
	else
		dir = ".";
	return(dir);
} '
#
# close stdout
#
exec 1>/dev/tty
#
# kludge - filter out unwanted headers files.
#
if [[ -s "${filters}" ]]
then
	cut -f2 -d';' ${filters} >${tmpfile}
	mv ${tmpfile} ${filters}
	egrep -v -f ${filters} ${makefile} >${tmpfile}
	mv ${tmpfile} ${makefile}
fi
#
echo "makefile '${makefile}' created ..."
#
# clean up mess
#
cleanup
#
# all done
#
exit 0
#
