#!/usr/bin/python3.2
#
x = "hello, world"
print(x);
#
x = 1
print(x);
#
try:
	del x
	print(x);
except NameError as error:
	print("caught name error: %s" % error)
else:
	print("No error caught")
finally:
	print("all done")
#
quit()
