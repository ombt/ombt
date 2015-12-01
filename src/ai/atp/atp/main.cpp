// automated theorem prover - uses A-ordering resolution. supports
// equality via paramodulation or by axiomatizing equality.
//

// headers
#include "pr.h"
#include "trace.h"

// externs
extern int optind;
extern char *optarg;
extern int syntaxErrors;
extern FILE *yyin;
extern int yyparse();

// globals
DEFINEDATA();
DEFINETRACE();

// help messages
void
usage(const char *cmd)
{
	cout << "usage: " << cmd << " [-?vepud]" << endl;
	cout << "\t[-t temp_base_directory]" << endl;
	cout << "\t[-m maximum_depth] [-M maximum_clauses]" << endl;
	cout << "\t[-W weight]" << endl;
	cout << "\t[-S solutions_to_find]" << endl;
	cout << "\t[-L maximum_literals_in_clause]" << endl;
	cout << "\t[-P phase[, ...] ] [-N phase[, ...] ]" << endl;
	cout << "\t[-s [on|off]] [-T [on|off]] [-U [on|off]]" << endl;
	cout << "\t[-c [on|off]] [-D [on|off]] [-C [on|off]]" << endl;
	cout << "\t[-n [on|off]]" << endl;
	cout << "\t[-r [sat|bfs|dfs|iter|best|dfshc]" << endl;
	cout << "\t[-R [none|parent|stack|both]" << endl;
	cout << "\t[-I include_dir [-I ...] ] file ..." << endl;
	cout << endl;
	cout << "where:" << endl;
	cout << endl;
	cout << "\t-? = print usage message," << endl;
	cout << "\t-v = enable verbose mode," << endl;
	cout << "\t-d = enable debug mode," << endl;
	cout << "\t-e = echo input to stdout," << endl;
	cout << "\t-R = reporting type: none, parent, stack, both," << endl;
	cout << "\t-p = use paramodulation," << endl;
	cout << "\t-u = enable memory usage reporting," << endl;
	cout << "\t-t = set path to temp directory," << endl;
	cout << "\t-M = maximum number of clauses (-1 to disable)," << endl;
	cout << "\t-m = maximum depth for search (-1 to disable, default=12)," << endl;
	cout << "\t-S = number of solutions to find (-1 for all, default=1)," << endl;
	cout << "\t-L = maximum literals in a clause (disabled by default)," << endl;
	cout << "\t-r = search type: sat, bfs, dfs, best, iter, dfshc, (default = dfs)," << endl;
	cout << "\t-I = include directory," << endl;
	cout << "\t-P = phases to run: exp, prs, xax, cnv, prv," << endl;
	cout << "\t-N = phases NOT to run: exp, prs, xax, cnv, prv," << endl;
	cout << "\t-T = turn tautology test on/off (default=on)," << endl;
	cout << "\t-s = turn subsumption test on/off (default=on)," << endl;
	cout << "\t-U = turn unit-literal preference on/off, (default=on)," << endl;
	cout << "\t-D = turn depth preference on/off, (default=on)," << endl;
	cout << "\t-C = turn clause complexity on/off, (default=on)," << endl;
	cout << "\t-W = relative weight of g vs. h, 0<=W<=10, (default=5)," << endl;
	cout << "\t-c = turn redundant clause checks on/off (default=off)," << endl;
	cout << "\tfile = list of input files." << endl;
	cout << endl;
	cout << "\texp = expand file." << endl;
	cout << "\tprs = parse file." << endl;
	cout << "\txax = extra axioms." << endl;
	cout << "\tcnv = convert to CNF form." << endl;
	cout << "\tprv = run prover." << endl;
	cout << endl;
	cout << "\tsequence of steps are: exp --> prs --> xax --> cnv --> prv" << endl;
	cout << endl;
	return;
}

// signal handlers
extern "C" void
handler(int sig)
{
	ERRORD("signal caught. aborting.", sig, errno);
	finaledata = (unsigned long)sbrk(0);
	cout << endl;
	cout << "Initial Data Address: " << initialedata << endl;
	cout << "Final Data Address: " << finaledata << endl;
	cout << "Growth in Data Section: " << (finaledata-initialedata) << endl;
	exit(0);;
}

// run theorem prover on a file
int
runatp(const List<Tuple<String, String> > &expandedfiles)
{
	// check phases
	if (!phases[EXPANDFILE] || !phases[PARSEFILE])
	{
		cout << endl;
		cout << "Skipping parsing ..." << endl;
	}

	// run prover on expanded files
	ListIterator<Tuple<String, String> > xfIter(expandedfiles);
	for ( ; !xfIter.done(); xfIter++)
	{
		// get input and expanded file names
		String infile(xfIter().key);
		String exfile(xfIter().data);
		cout << endl;
		cout << "Parsing input file ... " << infile << endl;
		if (verbose)
		{
			cout << "Parsing expanded file ... " << exfile << endl;
		}
		programstatistics[TotalFilesRead] += 1;

		// reset syntax errors counter for this file
		syntaxErrors = 0;

		// open file to read
		if ((yyin = fopen((const char *)exfile, "r")) == NULL)
		{
			ERRORD("unable to read.", exfile, errno);
			return(NOTOK);
		}

		// check syntax of program
		if (yyparse() != OK)
		{
			ERRORD(String("yyparse failed for ...")+infile, 
				syntaxErrors, errno);
			fclose(yyin);
			yyin = NULL;
			return(NOTOK);
		}

		// close file.
		fclose(yyin);
		yyin = NULL;

		// clear out semantic structures
		scope.clear();
		symbols.clear();
		ptrees.clear();
	}

	// all done
	return(OK);
}

// initialize statistics
void
initAllStatistics()
{
	statistics[AttemptedSubsumptionTests] = 0;
	statistics[AttemptedStillmanSubsumptionTests] = 0;
	statistics[SubsumedClausesRemoved] = 0;
	statistics[AttemptedTautologyTests] = 0;
	statistics[TautologiesRemoved] = 0;
	statistics[ClausesGenerated] = 0;
	statistics[ResolutionsAttempted] = 0;
	statistics[RedundantClauseTestsAttempted] = 0;
	statistics[RedundantClausesRejected] = 0;
	statistics[SolutionsFound] = 0;
	statistics[MaximumLiteralsClausesRejected] = 0;
	totalstatistics[TotalAttemptedSubsumptionTests] = 0;
	totalstatistics[TotalAttemptedStillmanSubsumptionTests] = 0;
	totalstatistics[TotalSubsumedClausesRemoved] = 0;
	totalstatistics[TotalAttemptedTautologyTests] = 0;
	totalstatistics[TotalTautologiesRemoved] = 0;
	totalstatistics[TotalClausesGenerated] = 0;
	totalstatistics[TotalResolutionsAttempted] = 0;
	totalstatistics[TotalRedundantClauseTestsAttempted] = 0;
	totalstatistics[TotalRedundantClausesRejected] = 0;
	totalstatistics[TotalSolutionsFound] = 0;
	totalstatistics[TotalMaximumLiteralsClausesRejected] = 0;
	programstatistics[TotalFilesRead] = 0;
	programstatistics[TotalProgramsExecuted] = 0;
	programstatistics[TotalValidPrograms] = 0;
	programstatistics[TotalNotProvenPrograms] = 0;
	programstatistics[TotalMaximumClauseExceededPrograms] = 0;
	programstatistics[TotalMaximumDepthExceededPrograms] = 0;
	return;
}

// print option value
void
dumpoptval(const String &optnm, int optval)
{
	cout << optnm << " = ";
	if (optval)
		cout << "ON";
	else
		cout << "OFF";
	cout << endl;
	return;
}

// start of theorem prover
main(int argc, char **argv)
{
	// current end of data
	SETINITEDATA();
	DISABLETRACE();

	// turn off buffering
	setbuf(stdout, NULL);

	// initialize some variables
	phases[EXPANDFILE] = 1;
	phases[PARSEFILE] = 1;
	phases[ADDEXTRAAXIOMS] = 1;
	phases[CONVERT2CNF] = 1;
	phases[RUNPROVER] = 1;

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?L:W:M:D:U:c:S:s:T:C:N:P:m:I:vpt:r:eudR:")) != EOF; )
	{
		switch (c)
		{
		case 'D':
			// turn depth-calculation off
			if (String(optarg) == String("on"))
				usedepth = 1;
			else
				usedepth = 0;
			break;

		case 'U':
			// turn unit-literal option on or off for BFS
			if (String(optarg) == String("on"))
				unitliteral = 1;
			else
				unitliteral = 0;
			break;

		case 'c':
			// turn best-first checks on or off
			if (String(optarg) == String("on"))
				bfswithchecks = 1;
			else
				bfswithchecks = 0;
			break;

		case 's':
			// turn subsumption test on or off
			if (String(optarg) == String("on"))
				subsumptiontest = 1;
			else
				subsumptiontest = 0;
			break;

		case 'T':
			// turn tautolgy test on or off
			if (String(optarg) == String("on"))
				tautologytest = 1;
			else
				tautologytest = 0;
			break;

		case 'C':
			// turn complexity test on or off
			if (String(optarg) == String("on"))
				complexity = 1;
			else
				complexity = 0;
			break;

		case 'M':
			// maximum clause
			maxclause = atoi(optarg);
			if (maxclause <= 0)
				maxclause = INT_MAX;
			break;

		case 'S':
			// number of solutions to find
			solutionsrequired = atoi(optarg);
			if (solutionsrequired <= 0)
				solutionsrequired = INT_MAX;
			break;

		case 'L':
			// number of literals in a clause
			maxliterals = atoi(optarg);
			if (maxliterals <= 0)
				maxliterals = INT_MAX;
			break;

		case 'e':
			// echo input to stdout
			echo = 1;
			break;

		case 'd':
			// enable debug mode
			ENABLETRACE();
			break;

		case 'u':
			// enable memory usage reporting
			reportmemoryusage = 1;
			break;

		case 'R':
		{
			// set type of report
			String rtype(optarg);
			if (rtype == String("none"))
				reporttype = ReportNone;
			else if (rtype == String("both"))
				reporttype = ReportBoth;
			else if (rtype == String("parent"))
				reporttype = ReportParent;
			else if (rtype == String("stack"))
				reporttype = ReportStack;
			else 
			{
				ERRORD("unknown report type.", rtype, EINVAL);
				usage(argv[0]);
				return(2);
			}
			break;
		}

		case 'r':
		{
			// set type of search
			String stype(optarg);
			if (stype == String("sat"))
				searchtype = Saturation;
			else if (stype == String("bfs"))
				searchtype = BreadthFirst;
			else if (stype == String("dfshc"))
				searchtype = DepthFirstHillClimb;
			else if (stype == String("dfs"))
				searchtype = DepthFirst;
			else if (stype == String("best"))
				searchtype = BestFirst;
			else if (stype == String("iter"))
				searchtype = IterativeDeepening;
			else 
			{
				ERRORD("unknown search type.", stype, EINVAL);
				usage(argv[0]);
				return(2);
			}
			break;
		}


		case 'N':
		{
			// turn off phases to run
			String Popts(optarg);
			StringTokens Poptst(Popts, ",");
			for ( ; !Poptst.done(); Poptst++)
			{
				String Popt = Poptst();
				if (!phases.isInMap(Popt))
				{
					ERRORD("unknown phase.", Popt, EINVAL);
					return(2);
				}
				else
				{
					phases[Popt] = 0;
				}
			}
			break;
		}

		case 'P':
		{
			// turn off all phases
			phases[EXPANDFILE] = 0;
			phases[PARSEFILE] = 0;
			phases[ADDEXTRAAXIOMS] = 0;
			phases[CONVERT2CNF] = 0;
			phases[RUNPROVER] = 0;

			// turn on phases to run
			String Popts(optarg);
			StringTokens Poptst(Popts, ",");
			for ( ; !Poptst.done(); Poptst++)
			{
				String Popt = Poptst();
				if (!phases.isInMap(Popt))
				{
					ERRORD("unknown phase.", Popt, EINVAL);
					return(2);
				}
				else
				{
					phases[Popt] = 1;
				}
			}
			break;
		}

		case 'm':
			// maximum depth for search
			maxdepth = atoi(optarg);
			if (maxdepth <= 0)
				maxdepth = INT_MAX;
			break;

		case 'W':
			// weight factor 0<=weight<=10
			weight = atoi(optarg);
			if (weight < 0 || weight > 10)
			{
				ERRORD("weight out of range, 0<=W<=10.",
					weight, EINVAL);
				return(2);
			}
			break;

		case 'v':
			// verbose mode
			verbose = 1;
			break;

		case 'p':
			// use paramodulation 
			paramodulation = 1;
			ERROR("paramodulation (-p) not implemented.", EINVAL);
			return(2);
			// break;

		case 't':
			// base directory for temp
			tempbase = optarg;
			break;

		case 'I':
			// include directories
			if (includedirs.insertAtEnd(String(optarg)) != OK)
			{
				ERRORD("unable to save an include directory.",
					optarg, EINVAL);
				return(2);
			}
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// copy list of files to a list
	if (optind >= argc)
	{
		ERROR("no input files were given.", EINVAL);
		usage(argv[0]);
		return(2);
	}
	List<String> inputfiles;
	for (int arg = optind; arg < argc; arg++)
	{
		if (inputfiles.insertAtEnd(String(argv[arg])) != OK)
		{
			ERRORD("unable to save file.", argv[arg], errno);
			return(2);
		}
	}

	// command line options
	cout << "================== Program Options =============== " << endl;
	cout << "phases to RUN: " << phases << endl;
	cout << "temp base directory: " << tempbase << endl;
	cout << "extra include-directories: " << includedirs << endl;
	dumpoptval("echo input", echo);
	dumpoptval("debug mode (trace flag)", GETTRACE());
	dumpoptval("use verbose", verbose);
	dumpoptval("report memory usage", reportmemoryusage);
	cout << "maximum depth: ";
	if (maxdepth == INT_MAX)
		cout <<  "DISABLED";
	else
		cout << maxdepth;
	cout << endl;
	cout << "maximum clause: ";
	if (maxclause == INT_MAX)
		cout <<  "DISABLED";
	else
		cout << maxclause;
	cout << endl;
	cout << "solutions to find: ";
	if (solutionsrequired == INT_MAX)
		cout <<  "ALL SOLUTIONS";
	else
		cout << solutionsrequired;
	cout << endl;
	cout << "maximum literals in a clause: ";
	if (maxliterals == INT_MAX)
		cout <<  "NO LIMIT";
	else
		cout << maxliterals;
	cout << endl;
	cout << "best-first search g vs. h weight: " << weight << endl;
	dumpoptval("use paramodulation", paramodulation);
	dumpoptval("subsumption test", subsumptiontest);
	dumpoptval("tautology test", tautologytest);
	cout << "search type: ";
	switch (searchtype)
	{
	case Saturation:
		cout << "Saturation";
		break;
	case BreadthFirst:
		cout << "BreadthFirst";
		break;
	case DepthFirst:
		cout << "DepthFirst";
		break;
	case DepthFirstHillClimb:
		cout << "DepthFirstHillClimb";
		break;
	case BestFirst:
		cout << "BestFirst";
		break;
	case IterativeDeepening:
		cout << "IterativeDeepening";
		break;
	default:
		cout << "UNKNOWN !!!";
		break;
	}
	cout << endl;
	cout << "report type: ";
	switch (reporttype)
	{
	case ReportNone:
		cout << "ReportNone";
		break;
	case ReportBoth:
		cout << "ReportBoth";
		break;
	case ReportParent:
		cout << "ReportParent";
		break;
	case ReportStack:
		cout << "ReportStack";
		break;
	default:
		cout << "UNKNOWN !!!";
		break;
	}
	cout << endl;
	dumpoptval("depth for gvalue (bfs-only)", usedepth);
	dumpoptval("unit-preference for hvalue (bfs-only)", unitliteral);
	dumpoptval("children-redundancy removal (bfs-only)", bfswithchecks);
	dumpoptval("complexity-preference for hvalue (bfs-only)", complexity);
	cout << "================================================== " << endl;

	// expand any include files
	List<Tuple<String, String> > expandedfiles;
	if (verbose)
	{
		cout << endl;
		cout << "Input files are ... " << endl;
		ListIterator<String> ifIter(inputfiles);
		for ( ; !ifIter.done(); ifIter++)
		{
			cout << ifIter() << endl;
		}
	}
	if (expandFiles(inputfiles, expandedfiles) != OK)
	{
		ERROR("failed to expand files.", EINVAL);
		return(2);
	}
	if (verbose)
	{
		cout << endl;
		cout << "Expanded files are ... " << endl;
		ListIterator<Tuple<String, String> > xfIter(expandedfiles);
		for ( ; !xfIter.done(); xfIter++)
		{
			cout << "input file " << xfIter().key << " ===>> ";
			cout << "expanded file " << xfIter().data << endl;
		}
	}

	// create statistics entries
	initAllStatistics();

	// set signal handler
	(void) signal(SIGINT, handler);
	(void) signal(SIGHUP, handler);
	(void) signal(SIGQUIT, handler);

	// set current end of data
	SETFINALEDATA();
	DUMPDATA(cout);

	// parse and run theorem prover
	if (runatp(expandedfiles) != OK)
	{
		ERROR("failed to run theorem prover.", EINVAL);
		return(2);
	}

	// dump out statistics
	cout << endl;
	cout << "Total Run Time Statistics ..." << endl;
	cout << totalstatistics << endl;
	cout << endl;
	cout << "Total Program Statistics ..." << endl;
	cout << programstatistics << endl;

	// current end of data
	SETFINALEDATA();
	DUMPDATA(cout);

#ifdef MEMORYLEAK
	// dump memory leak data
	cout << endl;
	cout << "BFSNode Memory Leak Data: " << endl;
	cout << BFSNode::memoryleak << endl;
	cout << endl;
	cout << "Clause Memory Leak Data: " << endl;
	cout << Clause::memoryleak << endl;
	cout << endl;
	cout << "Literal Memory Leak Data: " << endl;
	cout << Literal::memoryleak << endl;
	cout << endl;
	cout << "Terms Memory Leak Data: " << endl;
	cout << Terms::memoryleak << endl;
#endif

	// all done
	return(0);
}




