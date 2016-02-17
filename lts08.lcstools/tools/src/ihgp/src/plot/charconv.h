#ifndef __CHARCONV_H
#define __CHARCONV_H
/*
 * some basic macros.
 */
#define is_digit(x) ((((x) >= '0') && ((x) <= '9')) ? 1 : 0)
#define is_lowercase(x) ((((x) >= 'a') && ((x) <= 'z')) ? 1 : 0)
#define is_uppercase(x) ((((x) >= 'A') && ((x) <= 'Z')) ? 1 : 0)
#define is_blank(x) (((x) == ' ') ? 1 : 0)

#define to_uppercase(x) ((is_lowercase(x)) ? ((x) - 'a' + 'A') : (x))
#define to_lowercase(x) ((is_uppercase(x)) ? ((x) + 'a' - 'A') : (x))
#define to_decimal(x) ((x) - '0')
#define to_ascii(x) ((((x) >= 0) && ((x) <= 9)) ? ((x) + '0') : -1)

#define ASCIINULL (char) '\0'
#define NDIG 80
#define NULLADDR ((char *) 0L)
#define MAXSIGDIG 14	/* maximum significant digits */
#define MAXDIG 60	/* maximum digits for F format */
#define MAXEXPDIG 2	/* maximum digits in exponent, E format */
#define MAXLONGDIG 10	/* maximum digits in long integer */

#define GOODCONV 1L
#define BADCONV 2L
#define BADFORMAT 4L

extern long a2l();
extern char *l2a();
extern double a2d();
extern char *d2a();
extern long char_a2l();
extern long char_l2a();
extern long char_a2d();
extern long char_d2a_e();
extern long char_d2a_f();

#endif
