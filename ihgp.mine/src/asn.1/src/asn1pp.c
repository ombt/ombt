// asn1 preprocessor, removes comments.

// headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// local headers
#include "returns.h"
#include "debug.h"

// local data
enum States { DoubleDashComment, SlashStarComment, NotInComment, Unknown };

// remove all comments from the file
void
removeComments(FILE *ifd, FILE *ofd)
{
	unsigned int c, c2;
	int commentLevel = 0;
	States state = NotInComment;

	for ( ; (c = fgetc(ifd)) != EOF; )
	{
		switch (state)
		{
		case DoubleDashComment:
			if (c == '-')
			{
				if ((c2 = fgetc(ifd)) == EOF)
					return;
				if (c2 == '-')
				{
					// end of comments
					state = NotInComment;
				}
			}
			else if (c == '\n')
			{
				// end of comments
				state = NotInComment;
				fputc(c, ofd);
			}
			break;
		case SlashStarComment:
			if (c == '/')
			{
				if ((c2 = fgetc(ifd)) == EOF)
					return;
				if (c2 == '*')
				{
					// another layer of comments
					commentLevel++;
				}
				else if (c2 == '\n')
				{
					fputc(c, ofd);
				}
			}
			else if (c == '*')
			{
				if ((c2 = fgetc(ifd)) == EOF)
					return;
				if (c2 == '/')
				{
					// exiting a layer of comments
					if (--commentLevel == 0)
					{
						state = NotInComment;
					}
				}
				else if (c2 == '\n')
				{
					fputc(c, ofd);
				}
			}
			else if (c == '\n')
			{
				fputc(c, ofd);
			}
			break;
		case NotInComment:
			if (c == '/')
			{
				if ((c2 = fgetc(ifd)) == EOF)
				{
					fputc(c, ofd);
					return;
				}
				if (c2 == '*')
				{
					// start a comment
					commentLevel++;
					state = SlashStarComment;
				}
				else
				{
					fputc(c, ofd);
					fputc(c2, ofd);
				}
			}
			else if (c == '-')
			{
				if ((c2 = fgetc(ifd)) == EOF)
				{
					fputc(c, ofd);
					return;
				}
				if (c2 == '-')
				{
					// start a comment
					state = DoubleDashComment;
				}
				else
				{
					fputc(c, ofd);
					fputc(c2, ofd);
				}
			}
			else
				fputc(c, ofd);
			break;
		default:
			MustBeTrue(state == DoubleDashComment ||
				state == SlashStarComment ||
				state == NotInComment);
		}
	}
	
}

// main entry
main(int argc, char **argv)
{
	FILE *ifd, *ofd;
	char ofile[BUFSIZ];

	for (int arg=1; arg<argc; arg++)
	{
		// open files
		sprintf(ofile, "%s.i", argv[arg]);
		MustBeTrue((ifd = fopen(argv[arg], "r")) != NULL);
		MustBeTrue((ofd = fopen(ofile, "w")) != NULL);

		// remove all comments
		removeComments(ifd, ofd);

		// clean up
		fclose(ifd);
		fclose(ofd);
	}
	return(0);
}
