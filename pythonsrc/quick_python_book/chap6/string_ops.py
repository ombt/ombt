#!/usr/bin/python3.2
#
x = " "
words = [ "I'm", "done", "Mr.", "Duck" ]
print("words is ... ", words)
print("' '.join(...) is ... ", x.join(words))
print("split(' ') is ... ", x.join(words).split(" "))
#
r = "124.456"
print("r is ... ", float(r))
o = "777777"
print("octal 777777 is ... ", int(o))
#
try:
	r = "1.4"
	int(r)
except:
	print("exception thrown since '1.4' is NOT an int")
#
x = "   xx yy zz   "
print("x is ... <", x, ">, len(x) is ... ", len(x))
print("x.lstrip() is ... <", x.lstrip(), ">")
print("x.rstrip() is ... <", x.rstrip(), ">")
print("x.strip() is ... <", x.strip(), ">, len(x) is ... ", len(x.strip()), "\n",  end="")
#
x = "xxxxyyyyzzzztttt"
print("x is ... <", x, ">, len(x) is ... ", len(x))
print("x.lstrip('x') is ... <", x.lstrip('x'), ">")
print("x.rstrip('t') is ... <", x.rstrip('t'), ">")
print("x.strip('xt') is ... <", x.strip('xt'), ">")
#
text = "hello, world"
print("text ... ", text)
wordList = list(text);
print("wordList ... ", wordList)
wordList[6:] = []
wordList.reverse()
text = "".join(wordList)
print("reversed text is ... ", text)
#
quit()
