#!/usr/bin/python3.2
#
def compare_len(string1):
	return len(string1)
#
a = []
a.append("abcdefghijklmnopq")
a.append("bcdefghijklmnopq")
a.append("cdefghijklmnopq")
a.append("defghijklmnopq")
a.append("efghijklmnopq")
a.append("fghijklmnopq")
a.append("ghijklmnopq")
#
print("before sort ... a is ... ", a)
a.sort(key=compare_len)
print("after sort ... a is ... ", a)
#
print('check if "ghijklmnopq" in a', "ghijklmnopq" in a)
print('check if "ghijklmnopq" not in a', "ghijklmnopq" not in a)
#
# concatenation
b = a+a
print("b=a+a ... ", b)
#
print("4b is ... ", b*4)
#
# init a list to predetermined size
t = [None]*10;
print("t is ... ", t)
#
print("min of a ... ", min(a))
print("max of a ... ", max(a))
#
print("location of '%s' is at %d" % (
      "defghijklmnopq", 
      b.index("defghijklmnopq")))
#
print("'%s' occurs in b %d times" % (
      "defghijklmnopq", 
      b.count("defghijklmnopq")))
#
shallow = a[:]
print("\nshallow before del a[1] ... ", shallow)
print("\na before del a[1] ... ", a)
del a[1]
print("\na after del a[1] ... ", a)
print("\nshallow after del a[1] ... ", shallow)
#
quit()
