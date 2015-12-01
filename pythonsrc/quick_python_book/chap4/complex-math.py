#!/usr/bin/python3.2
#
z = 3+4j;
print("z is ... ", z);
#
import cmath
#
x = 3.14159265j/4
print("x is ... ", x);
print("sqrt(x) is ", cmath.sqrt(x))
print("sin(x) is ", cmath.sin(x))
print("asin(sin(x)) is ", cmath.asin(cmath.sin(x)))
#
quit()
