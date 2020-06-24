'\" Compile: nroff -mm -rN4 $1.mm > $1.man
.br
.tr
.SA 0
.PH ""
.PF ""
.OH ""
.EH ""
.ds Dt "(printed \n(mo/\n(dy/\n(yr)"
.de tH
.nr mP 0 1
.tl \fB'\\$1(\\$2)'UNIX 5.0 (\\$3)'\\$1(\\$2)'\fP
.OH "\fB'\\$1(\\$2)'UNIX 5.0 (\\$3)'\\$1(\\$2)'\fP"
.EH "\fB'\\$1(\\$2)'UNIX 5.0 (\\$3)'\\$1(\\$2)'\fP"
.PF "\fB'Page \\\\\\\\n+(mP''\*(Dt'\fP"
..
.tH SCCSPR 1 "5ESS RC/V TOOLS"
.VL 5
.LI "NAME"
.br
sccspr \- print CMS SCCS files in a useful format
.LI "SYNOPSIS"
.br
.B sccspr 
[ -d | -p ]
[ -e ]
[ -h ]
.LI "DESCRIPTION"
.br
.I Sccspr
expects a CMS SCCS file as standard input, and will format it in a
somewhat more useful format on the standard output.  A table of all
meaningful SCCS deltas will be printed, followed by a listing of the
file.  The listing will include all lines that were ever added to or
deleted from the file (as long as the deltas still exist), with each
line prefixed by the MR that inserted it (or ``[addsrc]'' if line was
inserted when the file was created) and the MR that deleted it if it
no longer exists.  Note that only those lines with an "adding" MR and
no "deleting" MR are still in the file.
.P
The following options are recognized, and may be used in combination
with the expected results:
.VL 5
.LI -d
.I Sccspr
will prefix output lines with SCCS delta numbers instead of MR
numbers.
.LI -p
.I Sccspr
will prefix output lines with the programmer making the delta instead
of the MR.
.LI -e
.I Sccspr
will only print existing lines in the file.  Lines that were deleted
will not be displayed.
.LI -h
.I Sccspr
will not display the delta table as part of the output heading.
.LE
.ig
.LI "SHELL VARIABLES"
.br
None.
.LI "RESTRICTIONS"
.br
None.
..
.LI "EXAMPLE"
.br
cms vlines.c | sccspr
.LI "DIAGNOSTICS"
.br
Unknown argument: <argument>
.sp
Unknown option: <option>
.sp
malloc():  unable to allocate needed memory
.sp
Delta <delta> not found. SCCS file corruption suspected!!
.ig
.LI "CAVEATS"
.br
What's a ``caveat''???
..
.LI "BUGS"
.br
.I Sccspr
does practically no error checking.  It expects a CMS style SCCS
file, and if it doesn't get one it doesn't complain but may give
unexpected results.
.P
Deltas which have been moved from the original MR to another with
.I chgmrs
will be attributed to the original MR, making it confusing about
what changes the new MR will cause to the file.
.LI "SEE ALSO"
.br
cms(rcapp),
sccsfile(4).
.LI "AUTHOR"
.br
Merlin J Mathesius
.LI "PROVIDER"
.br
RC Subsystem Engineer
.LE
.OH ""
.EH ""
