#!/usr/bin/ksh
#
# driver file for automated theorem proving (atp) program. it calls
# the following files;
#
#	atppp - preprocessor
#	atpfe - front-end
#	atpsplit - heuristic splitting
#	atppr - resolution/paramodulation theorem prover 
#
# execution atppr is controlled by the ANDOR file and a simple
# logic parser written in awk. the logic parser evaluates the 
# logic expression in the ANDOR file using the short-circuit 
# technique. it stops as soon as it knows the results for a 
# specific expression.
#
##############################################################################
#
# default values for atp programs
#
cmd=${0}
verboseopt=
echoopt=
includeopt=
paramodulationopt=
strategyopt="-S all"
#
# temporary files
#
TmpFile="/tmp/TF$$"
ProbDirs="/tmp/PD$$"
StrDirs="/tmp/SD$$"
#
# functions
#
# remove temp files
#
rmTmpFiles() {
	rm -rf /tmp/*$$ 1>/dev/null 2>&1
}
#
# print usage message
#
usage() {
	echo "usage: ${cmd} [-xVEP] [-I includedir [-I ...]] " >&2
	echo "	[-S strategy [, ...]] file.t ..." >&2
	echo "" >&2
	echo "	-x = debug mode" >&2
	echo "	-V = verbose mode" >&2
	echo "	-E = echo mode" >&2
	echo "	-P = use paramodulation to handle equality" >&2
	echo "	-I = include directory, can have more than one." >&2
	echo "	-S = list of strategies: none, bledsoe, schaum, all." >&2
	echo "" >&2
	echo "where file.t are logic program files." >&2
}
#
# run automated theorem prover for a problem
#
runprover() {
	#
	# goto strategy directory
	#
	cd ${1}
	#
	# check if ANDOR file exists;
	#
	if [[ ! -r "ANDOR" ]]
	then
		echo "ANDOR file does NOT exist." >&2
		echo "SKIPPING this strategy." >&2
		return 2
	fi
	#
	# parse ANDOR file and call prover
	#
	nawk 'BEGIN {
		# options for provers
		popt = "'"${paramodulationopt}"'";
		vopt = "'"${verboseopt}"'";
		opts = popt " " vopt;
		#
		problem = "'"${PWD}"'";
		match(problem, /[^\/]+\/[^\/]+\/[^\/]+$/);
		problem = substr(problem, RSTART, RLENGTH);
		# read in ANDOR record
		if (getline andorrec < "ANDOR" <= 0)
		{
			printf("unable to read ANDOR file.\n") | "cat 1>&2";
			retval = 2;
			exit 2;
		}
		#
		inittokenizer();
		# 
		if (parse() != "ok")
		{
			printf("parse failed.\n") | "cat 1>&2";
			retval = 2;
			exit 2;
		}
		#
		if (evaluate() != "ok")
		{
			printf("evaluate failed.\n") | "cat 1>&2";
			retval = 2;
			exit 2;
		}
		#
		exit retval;
	}
	END {
		exit retval
	}
	function nextlabel() {
		labelno++;
		label = "label" labelno ":";
		return(label);
	}
	function nextpc() {
		lastpc++;
		return(lastpc);
	}
	function inittokenizer() {
		itoken = 1;
		andorreclen = length(andorrec);
		token = "";
		tokenstring = andorrec;
		retval = 0;
		lastpc = 0;
		labelno = 0;
		label = "label" labelno;
		gettoken();
	}
	function skipws( iwsp, temp) {
		if (itoken <= andorreclen)
		{
			temp = substr(tokenstring, itoken, 1);
			while (temp == " ")
			{
				itoken++;
				temp = substr(tokenstring, itoken, 1);
			}
		}
		return;
	}
	function gettoken( token2) {
		# check if we are done
		if (itoken > andorreclen)
			return("");
		# skip white space
		skipws();
		# get next token
		token = substr(tokenstring, itoken, 1);
		if (token == "(" || token == ")")
		{
			itoken++;
			return(token);
		}
		else if (token == "p")
		{
			itoken++;
			token2 = substr(tokenstring, itoken, 1);
			while (token2 ~ /^[0-9]$/)
			{
				token = token token2;
				itoken++;
				token2 = substr(tokenstring, itoken, 1);
			}
			return(token);
		}
		else if (token == "|")
		{
			itoken++;
			token2 = substr(tokenstring, itoken, 1)
			if (token2 == "|")
			{
				token = token token2;
				itoken++;
				return(token);
			}
			else
			{
				printf("unexpected character after \"|\" in ANDOR file: %s\n", token) | "cat 1>&2";
				retval = 2;
				exit 2;
			}
		}
		else if (token == "&")
		{
			itoken++;
			token2 = substr(tokenstring, itoken, 1)
			if (token2 == "&")
			{
				token = token token2;
				itoken++;
				return(token);
			}
			else
			{
				printf("unexpected character after \"&\" in ANDOR file: %s\n", token) | "cat 1>&2";
				retval = 2;
				exit 2;
			}
		}
		else
		{
			printf("unexpected character in ANDOR file: %s\n", token) | "cat 1>&2";
			retval = 2;
			exit 2;
		}
	} 
	#
	# the language is as follows:
	#
	# start --> or
	# or --> and | 
	#	or OR and
	# and --> pid | 
	#	and AND pid
	# pid --> PID |
	#	LPAREN expr RPAREN
	#
	# where:
	#
	# OR = "||"
	# AND = "&&"
	# LPAREN = "("
	# RPAREN = ")"
	# PID = "p[0-9]+"
	#
	# translated to remove left-recursion gives:
	#
	# or --> and ortail
	# ortail --> OR and ortail | empty
	# and --> pid andtail
	# andtail --> AND pid andtail | empty
	# pid --> LPAREN or RPAREN | PID
	#
	function parse() {
		if (or() != "ok")
		{
			retval = 2;
			return("notok");
		}
		else
		{
			retval = 0;
			program[nextpc()] = "stop";
			return("ok");
		}
	}
	# or --> and ortail
	function or(orlabel) {
		if (and() != "ok")
		{
			return("notok");
		}
		orlabel = nextlabel();
		if (ortail(orlabel) != "ok")
		{
			return("notok");
		}
		if (orlabel in labelused)
		{
			program[nextpc()] = orlabel;
			labels[orlabel] = lastpc;
		}
		return("ok");
	}
	# ortail --> OR and ortail | empty
	function ortail(orlabel) {
		if (token == "||")
		{
			# code for short-circuit
			labelused[orlabel] = orlabel;
			program[nextpc()] = "iftrue " orlabel;
			# accept ||
			gettoken();
			# continue with rule
			if (and() != "ok")
			{
				return("notok");
			}
			# code
			return(ortail(orlabel));
		}
		else
		{
			# null transition
			return("ok");
		}
	}
	# and --> pid andtail
	function and(andlabel) {
		if (pid() != "ok")
		{
			return("notok");
		}
		andlabel = nextlabel();
		if (andtail(andlabel) != "ok")
		{
			return("notok");
		}
		if (andlabel in labelused)
		{
			program[nextpc()] = andlabel;
			labels[andlabel] = lastpc;
		}
		return("ok");
	}
	# andtail --> AND pid andtail | empty
	function andtail(andlabel) {
		if (token == "&&")
		{
			# code for short-circuit
			program[nextpc()] = "iffalse " andlabel;
			labelused[andlabel] = andlabel;
			# accept &&
			gettoken();
			# continue with rule
			if (pid() != "ok")
			{
				return("notok");
			}
			# code
			return(andtail(andlabel));
		}
		else
		{
			# null transition
			return("ok");
		}
	}
	# pid --> LPAREN or RPAREN | PID
	function pid() {
		if (token ~ /^p[0-9]+/)
		{
			# write code
			program[nextpc()] = "eval " token;
			# accept token
			gettoken();
			return("ok");
		}
		else if (token == "(")
		{
			# accept token
			gettoken()
			# expect an expression
			if (or() != "ok")
			{
				return("notok");
			}
			# check for closing parenthesis
			if (token != ")")
			{
				return("notok");
			}
			# accept token
			gettoken();
			return("ok");
		}
		else
		{
			printf("unexpected token %s.\n", token) | "cat 1>&2";
			return("notok");
		}
	}
	function evaluate() {
		# print program and labels
		printf("ANDOR record:\n");
		printf("%s\n", andorrec);
		printf("program:\n");
		for (pc = 1; pc <= lastpc; pc++)
		{
			printf("%03d: %s\n", pc, program[pc]);
		}
		printf("labels:\n");
		for (label in labels)
		{
			printf("%s ==> pc: %d\n", label, labels[label]);
		}
		# execute the program
		accumulator = 0;
		for (pc = 1; program[pc] != "stop"; )
		{
			# get parts of line
			nopc = split(program[pc], opcodes, " ");
			# get opcode
			opcode = opcodes[1];
			# execute the opcode
			if (opcode ~ /^eval$/)
			{
				subprobdir = opcodes[2];
				cmd = "atppr " opts " " subprobdir "/axioms_conclusion";
				print "executing command  ... " cmd;
				status = system(cmd);
				if (status == 0)
				{
					accumulator = 1;
					pc++;
				}
				else if (status == 1)
				{
					accumulator = 0;
					pc++;
				}
				else
				{
					printf("system failed.\n") | "cat 1>&2";
					retval = 2;
					return("notok");
				}
			}
			else if (opcode ~ /^iftrue$/)
			{
				label = opcodes[2];
				if (accumulator == 1)
				{
					pc = labels[label];
				}
				else
				{
					pc++;
				}
			}
			else if (opcode ~ /^iffalse$/)
			{
				label = opcodes[2];
				if (accumulator == 0)
				{
					pc = labels[label];
				}
				else
				{
					pc++;
				}
			}
			else if (opcode ~ /^label[0-9]+:$/)
			{
				pc++;
			}
			else
			{
				printf("unknown opcode %s.\n", opcode) | "cat 1>&2";
				retval = 2;
				return("notok");
			}
		}
		# check if problem was proved
		if (accumulator == 1)
		{
			printf("%s PROVED !!!\n", problem);
		}
		else
		{
			printf("%s UNPROVEN !!!\n", problem);
		}
		return("ok");
	} '
	#
	# all done
	#
	return ${?}
}
#
# trap exits and errors
#
trap 'rmTmpFiles; exit 0;' 0 1 2 15
#
# get command line options
#
set -- $(getopt 'xPVEI:S:' ${*} 2>${TmpFile} )
if [[ ${?} -ne 0 || -s "${TmpFile}" ]]
then
	usage
	exit 2
fi
#
for opt in ${*}
do
	case "${opt}" in
	-P)
		paramodulationopt="-P"
		shift 1
		;;
	-V)
		verboseopt="-V"
		shift 1
		;;
	-I)
		includeopt="${includeopt} -I ${2}"
		shift 2
		;;
	-E)
		echoopt="-E"
		shift 1
		;;
	-S)
		strategyopt="-S ${2}"
		shift 2
		;;
	-x)
		set -x
		shift 1
		;;
	--)
		shift 1
		break
		;;
	esac
done
#
# process input files
#
for tfile in ${*}
do
	#
	# preprocess the file 
	#
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "preprocessing of file ${tfile} ..." 
	#
	echo "atppp ${includeopt} ${verboseopt} ${tfile}"
	#
	atppp ${includeopt} ${verboseopt} ${tfile}
	if [[ ${?} -ne 0 ]]
	then
		echo "atppp failed for file ${tfile}. skipping the file."
		continue
	fi
	#
	# check syntax
	#
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "syntax checking of file ${tfile} ..." 
	#
	echo "atpfe ${verboseopt} ${echoopt} ${tfile}"
	#
	atpfe ${verboseopt} ${echoopt} ${tfile}
	if [[ ${?} -ne 0 ]]
	then
		echo "atpfe failed for file ${tfile}. skipping the file."
		continue
	fi
	#
	# heuristic splitting of problems
	#
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "heuristic splitting of file ${tfile} ..." 
	#
	echo "atpsplit -O ${strategyopt} ${verboseopt} ${echoopt} ${tfile}"
	#
	atpsplit -O ${strategyopt} ${verboseopt} ${echoopt} ${tfile}
	if [[ ${?} -ne 0 ]]
	then
		echo "atpsplit failed for file ${tfile}. skipping the file."
		continue
	fi
	#
	# run prover on problems for this file
	#
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "running prover on file ${tfile} ..." 
	# 
	# get list of strategies
	#
	sopts="$(echo ${strategyopt} | sed 's/^-S //g; s/,/ /g;' )"
	#
	case "${sopts}" in
	*all*)
		sopts="bledsoe schaum none"
		;;
	esac
	#
	# get directories
	#
	oldpwd="$(pwd)"
	#
	find atp_temp/${tfile}.d -type d -print |
	grep '/axioms_conclusion[0-9][0-9]*$' >${ProbDirs}
	#
	for pdir in $(cat ${ProbDirs} )
	do
		# 
		echo "processing problem directory ${pdir} ..." 
		#
		# goto to the problem directory
		#
		cd ${pdir}
		#
		# check if strategy directory exists
		#
		for sopt in ${sopts}
		do
			if [[ ! -d "${sopt}" ]]
			then
				echo "strategy ${sopt} for ${pdir} does NOT exist." >&2
				echo "skipping strategy ${sopt} ..." >&2
				continue
			else
				echo "processing strategy ${sopt} ..." 
				#
				runprover ${sopt}
				if [[ ${?} != 0 ]]
				then
					echo "prover failed for problem ${pdir} and strategy ${sopt}." >&2
				else
					echo "prover succeeded for problem ${pdir} and strategy ${sopt}." >&2
				fi
			fi
			# 
			# return to old directory
			#
			cd ..
		done
		# 
		# return to old directory
		#
		cd ${oldpwd}
	done
done
#
# all done
# 
exit 0
