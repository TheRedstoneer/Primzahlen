import math
import sys

def findStart(start, mod):
	x = start
	while(x % mod > 0):
		x += 2
	return mod-int((x-start)/2)%mod

def crossOut(l):
	for i in range(9):
		if l[i][0] == 0:
			return "X"
	return ""

def calcWithSieve(end):
	wurzel = int(math.sqrt(end))
	low_primes = getSmallPrimes(wurzel)
	erg = 0

	countArr = list()

	for p in low_primes:
		countArr.append(findStart(1, p))
		print(f"\t{p}",end="")


	for i in range(wurzel+1,end,2):
		isPrime = True
		print(f"\n{i}\t",end="")
		for j in range(len(countArr)):
			countArr[j] += 1
			if(countArr[j] == low_primes[j]): countArr[j] = 0
			print(countArr[j],end="\t")

			if(countArr[j] == 0 and isPrime): #array[] = 0 AND isPrime = 1
				isPrime = False

		if(isPrime):
			erg += 1
	return erg+len(low_primes)

def getSmallPrimes(max):
	smallPrimes = list()
	for i in range(3,max,2):
	    isPrime = True
	    for mod in range(3,int(math.sqrt(i))+1,2):
	        if(i % mod == 0):
	            isPrime = False
	            break
	    if(isPrime):
	        smallPrimes.append(i)
	return smallPrimes

x = calcWithSieve(1000)
print(f"\n\n:{x}")
sys.exit(1)

xList = list()
start = 15


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
		xList[i][0] +=1
		if(xList[i][0] == xList[i][1]):
			xList[i][0] = 0
		print(xList[i][0],end="\t")
	print(crossOut(xList))
