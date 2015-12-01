#!/usr/bin/python3.2
#
p1 = "mr"
p2 = "duck"
p3 = "i'm"
p4 = "done"
p5 ="."
#
print("{0}. {1}, {2} {3}{4}".format(p1,p2,p3,p4,p5))
#
p1 = "mr. duck"
p2 = "i'm done"
print("{mrduck}. {imdone}".format(mrduck=p1,imdone=p2))
#
print("{0:{1}} is the food of the gods ...".format("Ambrosia", 15))
#
print("%s, %s." % ("mr. duck", "i'm done"))
#
quit()
