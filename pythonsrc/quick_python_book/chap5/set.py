#!/usr/bin/python3.2
#
# creating a set
#
x = ( 1, 2,2,3, 3, 4,4,4, 5 )
print("list x is ... ", x);
print("len(x) is ... ", len(x));
#
s = set(x)
print("set s is ... ", s);
print("len(s) is ... ", len(s));
#
print("set s is ... ", s);
print("is 1 in s ... ", 1 in s)
print("is 6 in s ... ", 6 in s)
#
s.remove(1);
s.add(6);
#
print("set s is ... ", s);
print("is 1 in s ... ", 1 in s)
print("is 6 in s ... ", 6 in s)
#
x = set([1,3,5,7])
print("x is ... ", x)
y = set([1,2,3,4,6])
print("y is ... ", y)
#
print("x&y (intersection) is ... ", x&y)
print("x|y (union) is ... ", x|y)
print("x^y (one or the other, but not both) is ... ", x^y)
#
z = frozenset(y)
print("z is ... ", z)
#
quit()
