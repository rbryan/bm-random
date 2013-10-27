import math

bits = 32
#bins = int(raw_input("How many bins do you want to use?>"))
bins = 10
f = open("output/ascii/internal.txt", "r")
lines = f.readlines()
array = []
counts = []
chis = []
for line in lines:
	try:
		array.append(float(line))
	except:
		print "Could not cast to float: " + line
		pass
print "Now converting to floats"
	
for i in range(1,bins+1,1):
	count = 0
	for num in array:
		if num <= i*((2.0**bits)/bins) and num > i*2.0**bits/bins - 2.0**bits/bins:
			count += 1
	counts.append(count)
for count in counts:
	chis.append(((count - (2.0**bits/bins*1.0))**2)/(2.0**bits/(bins*1.0)))
	print str(count)
#print array
chi2 = sum(chis)
print "chi squared value:"
print chi2
print "Sum of the counts:"
print sum(counts)
print "Average of the counts:"
print sum(counts)/(len(counts)*1.0)
print "Average of the generated numbers:"
print sum(array)/len(array)/1.0
