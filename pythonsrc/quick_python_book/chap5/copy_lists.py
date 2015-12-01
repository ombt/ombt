#!/usr/bin/python3.2
#
m = [[0,1,2],[10,11,12],[20,21,22]]
#
print("m[0] is ... ", m[0])
print("m[0][1] is ... ", m[0][1])
#
print("m[2] is ... ", m[2])
print("m[2][1] is ... ", m[2][1])
#
nested = [0]
print("nested is ... ", nested)
#
original = [nested, 1]
print("original is ... ", original)
#
# directly reassign nested ...
nested[0] = 'zero'
print("nested is ... ", nested)
print("original is ... ", original)
#
# reassign via original
original[0][0] = 0
print("nested is ... ", nested)
print("original is ... ", original)
#
# assign a new list to nested, then the link between
# original and nested is broken. it uses copy-on-write
# semantics.
#
nested = [ "i'm done mr. duck" ]
print("nested is ... ", nested)
print("original is ... ", original)
#
level10 = ["hi, mr. duck"]
level11 = ["hi, weavil"]
level0 = [level10, level11]
print("level0 is ... ", level0)
#
shallow = level0[:]
print("shallow is ... ", shallow)
#
level11.append("i'm done, mr. duck")
print("level0 is ... ", level0)
print("shallow is ... ", shallow)
#
import copy
deep = copy.deepcopy(level0)
print("before level0 is ... ", level0)
print("before shallow is ... ", shallow)
print("before deep is ... ", deep)
#
level10.append("mr. duck?")
print("after level0 is ... ", level0)
print("after shallow is ... ", shallow)
print("after deep is ... ", deep)
#
quit()
