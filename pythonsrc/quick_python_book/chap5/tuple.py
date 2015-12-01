#!/usr/bin/python3.2
#
# creating a tuple
#
#     0  1  2  3
#    -4 -3 -2 -1
x = ( 1, 2, 3, 4 )
print("x is ... ", x);
print("len(x) is ... ", len(x));
#
print("tuple x[0:-1] is ... ", x[0:-1]);
print("len(tuple x) is ... ", len(x));
#
# extra comma needed to disambiguate parsing
y = (1,)
print("y is ... ", y);
#
#
(one,two,three,four) = (1,2,3,4)
print("one is ... ", one)
print("four is ... ", four)
#
one,two,three,four = 2,4,6,8
print("one is ... ", one)
print("four is ... ", four)
#
one,four = four,one
print("one is ... ", one)
print("four is ... ", four)
#
# using wild cards to absorb extra elements.
x = (1,2,3,4)
a,b,*c = x
print(a)
print(b)
print(c)
a,*b,c = x
print(a)
print(b)
print(c)
a,b,c,d,*e = x
print(a)
print(b)
print(c)
print(d)
print(e)
#
# convert list to tuple, and vice-versa.
#
x = [1,2,3,4]
t = (2,4,6,8)
#
print(list(t))
print(tuple(x))
#
# interesting way to access individual chars in a string.
x = list("hello")
print("x is ... ", x)
#
quit()
