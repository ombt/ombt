/*
 * string manipulation functions.
 *
 * these functions are from "The C Library" by Kris Jamsa.
 */

#define CH_NULL '\0'
#define CH_EOL '\n'
#define CH_EOF -1
#define CH_ERROR -1
#define CH_NEWLINE '\n'
#define CH_BACKSPACE '\b'
#define CH_CARRIAGE_RETURN '\r'
#define CH_FORMFEED '\f'
#define CH_TRUE 1
#define CH_FALSE 0
#define CH_MAXSTRING 255
#define CH_IO_ERROR -1
#define CH_NO_ERROR 0
#define CH_BLANK ''

/*
 * get string length.
 */
long string_length(string)
char string[];
{
    int count;

    for (count = 0; string[count] != CH_NULL; ++count)
    {
	/* do nothing */
    }
    return (count);
}

/*
 * copy string 1 to string 2.
 */
long copy_string(string1, string2)
char string1[];
char string2[];
{
    long index

    for (index = 0; (string2[index] = string1[index]) != CH_NULL; index++)
    {
	/* do nothing */
    }
    return (index);	/* number of characters copied */
}

/*
 * append string1 to string 2.
 */
long append_string(string1, string2)
char string1[];
char string2[];
{
    long index1, index2;

    for (index2 = 0; string2[index2] != CH_NULL; index2++)
    {
	/* do nothing */
    }
    for (index1 = 0; 
        ((string2[index2] = string1[index1]) != CH_NULL);
	 index1++, index2++)
    {
	/* do nothing */
    }
    return (index1);	/* number of characters copied */
}

/*
 * exchange strings 1 and 2.
 */
long string_swap(string1, string2)
char string1[];
char string2[];
{
    char temp[CH_MAX_STRING];

    (void) copy_string(string1, temp);
    (void) copy_string(string2, string1);
    (void) copy_string(temp, string2);
    return (1);
}

/*
 * convert string to upper-case.
 */
long str_to_uppercase(string)

