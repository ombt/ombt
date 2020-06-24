/*
 *	@(#)sccspr.c	1.6	C Source
 *	Date: 92/03/06	Time: 15:19:14
 *
 *	sccspr - print CMS SCCS files in a useful format
 *
 *	Written By:     Merlin J. Mathesius
 *	Date Written:	1/28/87
 *
 *	Updated by:	L. K. Temple	10/16/91 - Insert prolog, tool_use
 *
 */

#define SCCS_Version    "1.6"
#define LastEditDate    "91/10/16 19:29:19"

#include	<stdio.h>
#include	<malloc.h>
#include	<string.h>
#include	<pwd.h>

void exit();

/* Local function declarations. */
char	*basename();

#define	CTLCH	'\001'

#define SZBUF	1024
#define SZMR	16
#define SZUSER	9
#define SZDATE	9
#define SZTIME	9

#define YES	1
#define	NO	0

#define	NODELTA	-1

struct delta {
	unsigned short	valid : 1;
	char		mr[SZMR];
	char		user[SZUSER];
	char		date[SZDATE];
	char		time[SZTIME];
};

struct stack {
	struct stack	*next;
	int		delta;
	char		op;
};


FILE		*infile, *outfile;
char		buf[SZBUF], *bufptr;
struct delta	*d_table = NULL;
struct stack	*d_stack = NULL;
struct stack	*d_free;
int		numdeltas = 0;

char		dflag = 'm';	/* delta output type 'm'r, 'd'elta, 'p'rogrammer */
int		eflag = NO;
int		hflag = YES;

main( argc, argv )
int	argc;
char	*argv[];
{
	struct passwd	*getpwnam();	/* Find "rcapp" in /etc/passwd. */
	struct passwd	*pwent;		/* Password entry structure. */
	void	endpwent();		/* Close /etc/passwd. */
	char	*progname;		/* Minimal name of this program. */
	char	tool_use[40];		/* Path to the tool_use command. */
	char	log_cmd[80];		/* Logging command to system. */
	int	i;
	char	*arg;

	infile = stdin;
	outfile = stdout;
	progname = basename( argv[0]);

	/*
	** analyze the options
	*/

	for ( i = 1; i < argc; i++ )
	{
		arg = argv[i];

		if ( *arg == '-' )
		{
			switch ( arg[1] )
			{
			case 'd':	/* -d: print delta number */
				dflag = 'd';
				break;
			case 'p':	/* -p: print delta programmer */
				dflag = 'p';
				break;
			case 'e':	/* -e: print existing lines only */
				eflag = YES;
				break;
			case 'h':	/* -h: don't print delta table */
				hflag = NO;
				break;
			default:
				(void)fprintf( stderr, "Unknown option: %s\n", arg );
				exit( 1 );
			}
		}
		else
		{
			(void)fprintf( stderr, "Unknown argument: %s\n", arg );
			exit( 1 );
		}
	}

	/*
	** Log the invocation of this tool, if possible.
	*/
	pwent = getpwnam( "rcapp");
	if (pwent != NULL)
	{
		(void)sprintf( tool_use, "%s/lib/tool_use", pwent->pw_dir);
		/* Is there an executable tool_use file? */
		if ( access( tool_use, 1) == 0)
		{
			(void)sprintf( log_cmd, "%s %s/log/tool_use/%s",
				tool_use, pwent->pw_dir, progname);
			(void)system( log_cmd);
		}
	}
	endpwent();

	/*
	** scan the header
	*/

	scan_hdr();

	/*
	** print the delta table and header
	*/

	print_hdr();

	/*
	** now scan the rest of the file
	*/

	scan_file();

	/*
	** exit gracefully
	*/

	return( 0 );
}

/*
** scan_hdr()
**
**	Scan the SCCS header and extract the needed delta table information
*/

scan_hdr()
{
	int	delta;
	int	got_mr;
	char	mr[SZMR];
	char	user[SZUSER];
	char	date[SZDATE];
	char	time[SZTIME];

	while ( ( bufptr = fgets( buf, 1024, infile ) ) != NULL )
	{
		/* if this is not the SCCS control char, break */

		if ( *bufptr != CTLCH ) break;

		++bufptr;

		switch ( *bufptr )
		{
		case 's':	/* start of delta table entry */
			got_mr = NO;
			delta = NODELTA;
			break;

		case 'd':	/* delta info */
			/* ^Ad <type> <SCCSid> yr/mo/da hr:mi:se <pgrm> DDDDD DDDDD */
			(void)sscanf( bufptr, "d %*s %*s %s %s %s %d %*d",
				     date, time, user, &delta );
			break;

		case 'c':	/* comment */
			if ( sscanf( bufptr, "c change for mr %s", mr ) == 1 )
			{
				got_mr = YES;
			}
			else if ( strncmp( bufptr, "c date and time created", 23 ) == 0 )
			{
				(void)strcpy( mr, "[addsrc]" );
				got_mr = YES;
			}
			break;

		case 'e':	/* end of delta table entry */
			if ( got_mr == YES && delta != NODELTA )
			{
				/* allocate the delta table if necessary */

				if ( d_table == NULL ) alloc_tbl( delta );

				/* store the delta information */

				(void)strcpy( d_table[delta].mr, mr );
				(void)strcpy( d_table[delta].user, user );
				(void)strcpy( d_table[delta].date, date );
				(void)strcpy( d_table[delta].time, time );
				d_table[delta].valid = YES;
			}

		case 't':	/* start of comment section */
			break;

		case 'T':	/* end of comment section */
			/* this is the end of the header section */
			return;
		}
	}
}

/*
** scan_file()
**
**	Scan the file, keeping track of what delta has inserted
**	and deleted each line.
*/

scan_file()
{
	int	ins, del;
	int	i;

	ins = del = NODELTA;

	/*
	** read lines through the end of file
	*/

	while ( ( bufptr = fgets( buf, 1024, infile ) ) != NULL )
	{

		/*
		** if this is an SCCS control line, process as such
		*/

		if ( *bufptr == CTLCH )
		{
			++bufptr;

			switch ( *bufptr )
			{
			case 'I':	/* following lines inserted by ... */
				(void)sscanf( bufptr, "I %d", &i );
				push_delta( 'i', i );
				ins = i;
				continue;

			case 'D':	/* following lines deleted by ... */
				(void)sscanf( bufptr, "D %d", &i );
				push_delta( 'd', i );
				del = i;
				continue;

			case 'E':	/* end of insertion/deletion */
				(void)sscanf( bufptr, "E %d", &i );
				pop_delta( i, &ins, &del );
				continue;

			default:
				break;
			}
		}

		/*
		** just print the line
		*/

		pr_line( ins, del, bufptr );
	}
}

/*
** pr_line()
**
**	prints a line of output
*/

pr_line( ins, del, line )
int	ins, del;
char	*line;
{
	/*
	** if only printing existing lines, return if line has been deleted
	*/

	if ( eflag == YES && del != NODELTA ) return;

	/*
	** print the MR that inserted the line
	*/

	if ( ins == NODELTA )
		(void)fprintf( outfile, "                " );
	else if ( ins > numdeltas || d_table[ins].valid == NO )
		(void)fprintf( outfile, "+?%-14d", ins );
	else
	{
		switch ( dflag )
		{
		case 'd':
			(void)fprintf( outfile, "+%-15d", ins );
			break;
		case 'p':
			(void)fprintf( outfile, "+%-15.15s", d_table[ins].user );
			break;
		default:
			(void)fprintf( outfile, "+%-15.15s", d_table[ins].mr );
			break;
		}
	}

	/*
	** print the MR that deleted the line
	**  (if eflag = YES, no need to print delete info line)
	*/

	if ( eflag == NO )
	{
		if ( del == NODELTA )
			(void)fprintf( outfile, "                " );
		else if ( del > numdeltas || d_table[del].valid == NO )
			(void)fprintf( outfile, "-?%-14d", del );
		else
		{
			switch ( dflag )
			{
			case 'd':
				(void)fprintf( outfile, "-%-15d", del );
				break;
			case 'p':
				(void)fprintf( outfile, "-%-15.15s",
					      d_table[del].user );
				break;
			default:
				(void)fprintf( outfile, "-%-15.15s",
					      d_table[del].mr );
				break;
			}
		}
	}

	/*
	** now print the line
	*/

	if ( line != NULL )
		(void)fprintf( outfile, "%s", line );
	else
		(void)fprintf( outfile, "\n" );
}

/*
** alloc_tbl
**
**	Allocates memory to store the delta table and delta stack
**
**
*/

alloc_tbl( ndelta )
int	ndelta;
{
	int	i;

	++ndelta;

	d_table = (struct delta *) malloc( ndelta * sizeof( struct delta ) );

	if ( d_table == (struct delta *)NULL )
	{
		(void)fprintf( stderr,
			      "malloc():  unable to allocate needed memory\n" );
		exit( 1 );
	}

	/*
	** save the size of the table
	*/

	numdeltas = ndelta;

	/*
	** initially mark all entries as "invalid"
	*/

	for ( i = 0; i < ndelta; ++i )
		d_table[i].valid = NO;


	/*
	** initialize the free delta stack
	*/

	d_free = (struct stack *) malloc( ndelta * sizeof( struct stack ) );

	if ( d_free == (struct stack *)NULL )
	{
		(void)fprintf( stderr,
			      "malloc():  unable to allocate needed memory\n" );
		exit( 1 );
	}

	for ( i = 0; i < ndelta - 1; ++i )
		d_free[i].next = &d_free[i+1];

	d_free[ndelta-1].next = (struct stack *)NULL;
}

/*
** print_hdr()
**
**	Prints the delta table and a file header
*/

print_hdr()
{
	int i;


	if ( hflag == YES )
	{
		(void)fprintf( outfile, "================================\n" );
		(void)fprintf( outfile, "DELTA TABLE:\n" );
		(void)fprintf( outfile, "\tdelta  mr              user      date\n" );
		(void)fprintf( outfile, "\t-----  --------------  --------  -----------------\n" );

		for ( i = 0; i < numdeltas; ++i )
		{
			if ( d_table[i].valid == NO )
				continue;

			(void)fprintf( outfile,
				      "\t%-5d  %-14.14s  %-8.8s  %8s %8s\n",
				      i,
				      d_table[i].mr,
				      d_table[i].user,
				      d_table[i].date, d_table[i].time );
		}
	}

	(void)fprintf( outfile, "================================\n" );


	switch ( dflag )
	{
	case 'd':
		(void)fprintf( outfile, "+Adding Delta+  " );
		break;
	case 'p':
		(void)fprintf( outfile, "+Adding User+   " );
		break;
	default:
		(void)fprintf( outfile, "+Adding MR+     " );
		break;
	}

	if ( eflag == NO )
	{
		switch ( dflag )
		{
		case 'd':
			(void)fprintf( outfile, "-Deleting Delta-" );
			break;
		case 'p':
			(void)fprintf( outfile, "-Deleting User-" );
			break;
		default:
			(void)fprintf( outfile, "-Deleting MR-" );
			break;
		}
	}

	(void)fprintf( outfile, "\n" );

	(void)fprintf( outfile, "=============== " );
	if ( eflag == NO )
		(void)fprintf( outfile, "===============" );
	(void)fprintf( outfile, "\n");
}

/*
** push_delta()
**
*/

push_delta( op, delta )
char	op;
int	delta;
{
	struct stack	*s;

	/* unlink an entry from the free list */
	s = d_free;
	d_free = d_free->next;

	/* fill in delta stack entry */
	s->op = op;
	s->delta = delta;

	/* link entry onto head of delta stack */
	s->next = d_stack;
	d_stack = s;
}

/*
** pop_delta()
**
*/

pop_delta( delta, ins_ptr, del_ptr )
int	delta, *ins_ptr, *del_ptr;
{
	struct stack	*s, *prev;
	char		op;
	int		odelta;

	prev = NULL, s = d_stack;
	while ( s != (struct stack *)NULL && s->delta != delta )
		prev = s, s = s->next;

	if ( s == NULL )
	{
		(void)fprintf( stderr,
			      "Delta %d not found. SCCS file corruption suspected!!\n",
			      delta );
		exit( 1 );
	}

	/* save the delta operation */
	op = s->op;

	/* unlink entry from stack */
	if ( prev != (struct stack *)NULL )
		prev->next = s->next;
	else
		d_stack = s->next;
		

	/* link the entry back on the free list */
	s->next = d_free;
	d_free = s;

	/* scan down stack to find the delta to replace this one */
	s = d_stack;
	while ( s != (struct stack *)NULL && s->op != op )
		s = s->next;

	if ( s != (struct stack *)NULL )
		odelta = s->delta;
	else
		odelta = NODELTA;

	if ( op == 'i' )
		*ins_ptr = odelta;
	else
		*del_ptr = odelta;
}

/*
 * Function:	basename
 *
 * Abstract:	Return a pointer to the last node in a path.
 *
 * Usage:	char	*path;
 *		char	*last_node;
 *		last_node = basename( path);
 *
 * Parameters:	char	*path - Ptr to string containing a nodal path
 *
 * Returns:	char *        - Ptr to first char in path following the
 *				the last '/'.
 *
 * Description:	Use strrchr() to find the last '/' in the string pointed
 *		to by path.  If a '/' was found, return the address of
 *		the next character; otherwise, return the address of path.
 *
 * Calls:	strrchr()
 */

char *
basename( path)
char	*path;	/* String containing a nodal path. */
{
	char	*s;	/* Utility pointer variable. */

	s = strrchr( path, '/' );
	if( s == NULL )
		return( path );
	else
		return( s + 1 );

} /* End of function basename. */
