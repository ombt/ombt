#!/usr/bin/python3.2
#
# creating a list
#
#     0  1  2  3
#    -4 -3 -2 -1
x = [ 1, 2, 3, 4 ]
print("x is ... ", x);
print("len(x) is ... ", len(x));
#
print("x[:-1] is ... ", x[:-1]);
print("x[2:-1] is ... ", x[2:-1]);
#
y = [ 1, 3 ]
print("y is ... ", y);
print("len(y) is ... ", len(y));
#
z = [ 1, x, y, 2 ]
print("z = ", z)
print("len(z) is ... ", len(z));
#
# copy z to t
t = z[:]
print("t = ", t)
print("len(t) is ... ", len(t));
#
origtlen = len(t);
#
# append t to t
t[len(t):] = t
print("t = ", t)
print("len(t) is ... ", len(t));
#
# prepend t to t
t[:0] = t
print("t = ", t)
print("len(t) is ... ", len(t));
#
# remove from t
t[origtlen:3*origtlen] = []
print("t = ", t)
print("len(t) is ... ", len(t));
#
# add a, b, c to list t
print("t = ", t)
t.append("a")
print("add a to t = ", t)
t.append("c")
print("add c to t = ", t)
t.append("b")
print("add b to t = ", t)
#
# adding lists to lists
t.extend(z);
print("add list z to list t = ", t)
#
# delete some junk
t[0:-origtlen-3] = []
print("clean up t = ", t)
#
# insert at front
t.insert(0, 'front')
print("insert at front of t = ", t)
t.insert(len(t), 'end')
print("insert at end of t = ", t)
#
# delete at end
print("before t = ", t)
del t[len(t)-1]
print("after t = ", t)
print("before t = ", t)
del t[0]
print("after t = ", t)
#
print("before remove t = ", t)
t.remove(1)
print("after remove t = ", t)
print("before remove t = ", t)
try:
	t.remove(1)
except:
	print("oops, catching all errors ... ")
print("after remove t = ", t)
#
# sort lists
a = t[0:3]
print("before sort a = ", a)
a.sort()
print("after sort a = ", a)
#
x = [[3,5],[2,9],[2,3],[4,3],[3,2]]
print("before sort x = ", x)
x.sort()
print("after sort x = ", x)

#
quit()
