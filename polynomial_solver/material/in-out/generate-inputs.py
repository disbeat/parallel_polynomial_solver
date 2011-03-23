import sys
import random

coefficientlim = 20
inputlim = 10

line = sys.stdin.readline()
vector = line.split()

#print "Degree of the polynomial? "
degree = int(vector[0])

#print "Number of inputs? "
numinputs = int(vector[1])

random.seed()

sys.stdout.write(str(degree) + " ")
for _ in xrange(degree + 1):
	nextcoeff = int(random.uniform(-coefficientlim - 1, coefficientlim + 1))
	sys.stdout.write(str(nextcoeff) + " ")
print

print numinputs	
for _ in xrange(numinputs):
	nextinput = int(random.uniform(-inputlim - 1, inputlim + 1))
	print nextinput
	
