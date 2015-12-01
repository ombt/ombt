#!/usr/bin/python3.2
#
try:
    age = input("Enter anything: ")
    print("My age is ... ", age)
except EOFError:
    print("you just had to say, good bye")
else:
    if age == None:
        print("you don't say.")
    else:
        print("you're an old fart")
#
quit()
