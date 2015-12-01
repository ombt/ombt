#!/usr/bin/python3.2
#
x = "hello"
#
print("x is ... ", x)
print("x[0] is ... ", x[0])
print("x[1] is ... ", x[1])
print("x[2] is ... ", x[2])
print("x[3] is ... ", x[3])
print("x[4] is ... ", x[4])
#
y = "good bye, sucker!\n"
print("y is ... ", y)
print("y[:-1] is ... ", y[:-1])
print("len(y) is ... ", len(y))
#
print("x + y is ... ", x+y)
print("len(x + y) is ... ", len(x+y))
#
z = "ab"
print("z is ... ", z)
print("8*z is ... ", 8*z)
#
bell = '\a'
print(bell, bell, bell, bell)
#
print("newline <\n>")
print("tab <\t>")
print("vertical tab <\v>")
#
print("'m' in hex is 6D ... <\x6d>")
print("'m' in oct is 155 ... <\155>")
#
unicode_a = '\N{LATIN SMALL LETTER A}'
print("unicode a ... ", unicode_a)
unicode_a_with_acute = '\N{LATIN SMALL LETTER A WITH ACUTE}'
print("unicode a with acute ... ", unicode_a)
print("\u00E1")
#
print("abc\n")
print("abc\n", end="")
#
quit()
