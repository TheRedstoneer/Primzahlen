import math
import sys

def findNext(start, mod):
	x = start
	while(x % mod > 0):
		x += 2
	return mod-(x-start)%mod

def crossOut(l):
	for i in range(9):
		if l[i][0] == 0:
			return "X"
	return ""


smallPrimes = list()
xList = list()
start = 15

for i in range(3,200,2):
    isPrime = True
    for mod in range(3,int(math.sqrt(i))+1,2):
        if(i % mod == 0):
            isPrime = False
            break
    if(isPrime):
        #print(i,end=", ")
        smallPrimes.append(i)
#print()
ch = [1,3,6,0,0,13,13,13,13]
for i in range(9):
	p = smallPrimes[i]
	#xList.append([ch[i],p])
	xList.append([findNext(start-2,p),p])
print(xList)
#sys.exit(0)
"""
	 3  5  7  11 13 17 19 23 29
15:  0  0  1   2  2 15 15 15 15
"""

print(f"Found {len(smallPrimes)} Primes!")
print("P",end="\t")
for i in range(9):
	print(smallPrimes[i],end="\t")
print("X ?\n---------------------------------------------------------------------------------")
for p in range(start,70,2):
	print(p,end="\t")
	for i in range(9):
		xList[i][0] +=2
		if(xList[i][0] >= xList[i][1]):
			xList[i][0] -= xList[i][1]
		print(xList[i][0],end="\t")
	print(crossOut(xList))
