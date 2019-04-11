def newTerm(a):
     cl = 1
     for i in range(4,a):
             cl = cl*((i-2)/i)
     cl = (a-3)*cl*(2/a)
     return cl

totSum = 0
for i in range(4,10000):
	totSum = totSum+newTerm(i)

print(totSum)