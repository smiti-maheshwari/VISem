
from sklearn.datasets import make_moons
import math
import operator
import random

def euclidean(a,b):
	d = 0
	for i in range(len(a)-1):
		d+=(a[i]-b[i])*(a[i]-b[i])
	#print(d)
	return math.sqrt(d)
	
def predict(train, test, kval):
	predicted = []
	for i in range(len(test)):
		eudist = []
		knn = []
		class1 = 0
		class2 = 0
		for j in range(len(train)):
			eu = euclidean(test[i], train[j])
			#print(i,j,eudist)
			tmp = []
			tmp.append(train[j][2])
			tmp.append(eu)
			eudist.append(tmp)
		eudist.sort(key = operator.itemgetter(1))
		knn = eudist[:kval]
		#print(knn)
		for k in range(len(knn)):
			if (knn[k][0]==0):
				class1+=1
			else:
				class2+=1
		#print(class1, class2)
		if class1 > class2:
			predicted.append(0)
		elif class2 > class1:
			predicted.append(1)
		else:
			predicted.append(-1)
	return predicted

def accuracy(predicted, test):
	correct = 0
	for i in range(len(test)):
		if test[i][2] == predicted[i]:
			correct+=1
	acc = (correct*1.0*100)/len(test)
	return acc
	
X,y = make_moons()
# print(X)
# print(y)

X = X.tolist()
y = y.tolist()
# print(X)
# print(y)
dataset = []
for i in range(len(X)):
	tmp = []
	tmp.append(X[i][0])
	tmp.append(X[i][1])
	tmp.append(y[i])
	dataset.append(tmp)
random.shuffle(dataset)
train = dataset[:70]
test = dataset[70:]
#print(len(test))
#print(dataset)
for k in range(1,16):
	predicted = predict(train, test, k)
#print (predicted)
	acc = accuracy(predicted, test)
	print(k)
	print(acc)
