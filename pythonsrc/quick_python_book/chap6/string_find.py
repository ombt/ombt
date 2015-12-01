#!/usr/bin/python3.2
#
x = "mississippi"
#
print("x ... ", x)
print("x.find(ss) ... ", x.find("ss"))
print("x.find(zz) ... ", x.find("zz"))
print("x.find(ss,3) ... ", x.find("ss",3))
print("x.find(ss,0,3) ... ", x.find("ss",0,3))
print("x.rfind(ss,3) ... ", x.rfind("ss",3))
print("x.count(ss) ... ", x.count("ss"))
print("x.startswith(Miss) ... ", x.startswith("Miss"))
print("x.endswith(ppi) ... ", x.endswith("ppi"))
print("x.replace(pp,++) ... ", x.replace("ss","++"))
#
y = "1.2"
print("y ... ", y)
print("y.isdigit() ... ", y.isdigit())
print("y.isalpha() ... ", y.isalpha())
#
z = "AB"
print("z ... ", z)
print("z.isdigit() ... ", z.isdigit())
print("z.isalpha() ... ", z.isalpha())
print("z.isupper() ... ", z.isupper())
print("z.islower() ... ", z.islower())
#
quit()
