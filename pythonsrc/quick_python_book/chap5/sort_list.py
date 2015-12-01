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
x = [ 4,3,1,2 ]
print("x is ... ", x)
x = sorted(x)
print("x is ... ", x)
#
quit()
