#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <algorithm>

// ab|*a.b.b.#.
// 0*1.0.0*.1.0*.10*.E|.#.
// 01|*0.0.01|*.#.

#define MAX 100

struct node {
	char sym;
	int fPos[MAX];
	int lPos[MAX];
	int fLim;
	int lLim;
	bool null;
	struct node * lChild;
	struct node * rChild;
};

using namespace std;
typedef struct node link;

link * createNode(char, int, bool);
link * createNode(char, link *, link *);
void inOrder(link *, bool);
link * createTree(char [MAX], char[MAX], map <char, int>&, int *, char [MAX], int *);
link * pop(link * [MAX], int *);
void push(link * [MAX], int *, link *);
void inOrder(link *, bool);
void preOrder(link *, bool);
void postOrder(link *, bool);
void findFirstAndLastPos(link *);
int makeUnion(int [MAX], int, int [MAX], int, int [MAX]);
int makeCopy(int *, int, int [MAX]);
void showFirstAndLastPos(link *);
void showArray(int *, int);
int removeDuplicates(int *, int);
void fillIndices(int [][MAX], int);
void findFollowPos(link *, int [][MAX]);
void addToFollowPos(int [][MAX], int, int [MAX], int);
void showFollowPos(int [][MAX], int);
void bubbleSort(int *, int);
void swapEle(int *, int, int);
int alreadyPresent(int [][MAX], int [MAX], int, int [MAX], int);
int makeTransTable(int [][MAX], int [][MAX], int [MAX], char [MAX], int, char [MAX], int, int [][MAX]);
bool isUnique(char [MAX], int, char);
bool isFState(int [MAX], int, int);
void showStates(int [][MAX], int [MAX], int, int [MAX], int, int);
void showTransTable(int [][MAX], int [][MAX], int [MAX], int, char [MAX], int);
int findFinalStates(int [][MAX], int [MAX], int, int [MAX], int);
void showFinalStates(int [][MAX], int [MAX], int [MAX], int);

int main() {
	char postfix[MAX];
	link * root;
	int stateCount;
	int fStateCount;
	int leafCount;
	int symCount;
	char numToSym[MAX];
	map <char, int> symToNum;
	int stateLen[MAX];
	int states[MAX][MAX];
	int fStates[MAX];
	int trans[MAX][MAX];
	char symList[MAX];

	printf("Enter postfix form of regex:\t");
	scanf("%s", postfix);

	root = createTree(postfix, numToSym, symToNum, &leafCount, symList, &symCount);
	int followPos[root -> rChild -> fPos[0]][MAX];

	printf("\nThe inOrder tree is:-\n");
	inOrder(root, false);
	//printf("\n");
	//inOrder(root, true);

	printf("\n\nThe preOrder tree is:-\n");
	preOrder(root, false);
	//printf("\n");
	//preOrder(root, true);

	printf("\n\nThe postOrder tree is:-\n");
	postOrder(root, false);
	//printf("\n");
	//postOrder(root, true);

	findFirstAndLastPos(root);

	printf("\n\nThe firstPos & lastPos are:-\nNode\tfPos\tlPos\n");
	showFirstAndLastPos(root);

	fillIndices(followPos, root -> rChild -> fPos[0]);
	findFollowPos(root, followPos);

	for (int i = 0; i < root -> rChild -> fPos[0]; i++) {
		bubbleSort(&followPos[i][2], followPos[i][1] - 2);
		//sort(&followPos[i][2], &followPos[i][followPos[i][1] - 2]);
	}
	showFollowPos(followPos, root -> rChild -> fPos[0]);

	printf("List of symbols:-\n");
	for (int i = 0; i < symCount; i++) {
		printf("symList[%d] = %c\n", i, symList[i]);
	}
	printf("\nList of leaf nodes:-\n");
	for (int i = 0; i < leafCount; i++) {
		printf("numToSym[%d] = %c\n", i, numToSym[i]);
	}
	/*
	printf("\nnumToSym map:-\n");
	for (int i = 0; i < leafCount; i++) {
		printf("symToNum[%c] = %d\n", numToSym[i], symToNum[numToSym[i]]);
	}
	*/
	printf("\n");

	stateLen[0] = makeCopy(root -> fPos, root -> fLim, states[0]);
	stateCount = makeTransTable(trans, states, stateLen, numToSym, leafCount, symList, symCount, followPos);
	fStateCount = findFinalStates(states, stateLen, stateCount, fStates, leafCount);
	showStates(states, stateLen, stateCount, fStates, fStateCount, leafCount);
	showTransTable(trans, states, stateLen, stateCount, symList, symCount);

	return 0;
}

link * createNode(char sym, int ind, bool null) {
	link * newNode;

	newNode = (link *) malloc(sizeof(link));
	memset(newNode -> fPos, -1, MAX * sizeof(int));
	memset(newNode -> lPos, -1, MAX * sizeof(int));

	newNode -> lChild = NULL;
	newNode -> rChild = NULL;
	newNode -> fLim = 0;
	newNode -> lLim = 0;
	newNode -> sym = sym;
	newNode -> null = null;

	if (ind > 0) {
		newNode -> fPos[newNode -> fLim++] = ind;
		newNode -> lPos[newNode -> lLim++] = ind;
	}

	return newNode;
}

link * createNode(char sym, link * lChild, link * rChild) {
	link * newNode;

	switch (sym) {
		case '|'	:
			newNode = createNode(sym, 0, lChild -> null || rChild -> null);
			break;

		case '.'	:
			newNode = createNode(sym, 0, lChild -> null && rChild -> null);
			break;

		case '*'	:
			newNode = createNode(sym, 0, true);
			break;
	}

	newNode -> lChild = lChild;
	newNode -> rChild = rChild;

	return newNode;
}

void inOrder(link * root, bool showInd) {
	if (root != NULL) {
		inOrder(root -> lChild, showInd);
		if (showInd) {
			printf("%d ", root -> fPos[0]);
		} else {
			printf("%c ", root -> sym);
		}
		inOrder(root -> rChild, showInd);
	}
}

void preOrder(link * root, bool showInd) {
	if (root != NULL) {
		if (showInd) {
			printf("%d ", root -> fPos[0]);
		} else {
			printf("%c ", root -> sym);
		}
		preOrder(root -> lChild, showInd);
		preOrder(root -> rChild, showInd);
	}
}

void postOrder(link * root, bool showInd) {
	if (root != NULL) {
		postOrder(root -> lChild, showInd);
		postOrder(root -> rChild, showInd);
		if (showInd) {
			printf("%d ", root -> fPos[0]);
		} else {
			printf("%c ", root -> sym);
		}
	}
}

link * createTree(char postfix[MAX], char numToSym[MAX], map <char, int>& symToNum, int * leafCount, char symList[MAX], int * symCount) {
	link * stak[MAX];
	link * root;
	link * newNode;
	link * lChild;
	link * rChild;
	int len;
	int i;
	int ind;
	int top;
	char c;

	ind = 1;
	top = -1;
	len = strlen(postfix);
	* leafCount = 0;
	* symCount = 0;

	for (i = 0; i < len; i++) {
		c = postfix[i];
		if (c == '.' || c == '|') {
			rChild = pop(stak, &top);
			lChild = pop(stak, &top);
			newNode = createNode(c, lChild, rChild);
			push(stak, &top, newNode);
		} else if (c == '*') {
			lChild = pop(stak, &top);
			newNode = createNode(c, lChild, NULL);
			push(stak, &top, newNode);
		} else {
			if (c == 'E') {
				newNode = createNode(c, 0, true);
				//numToSym[* leafCount] = c;
				//symToNum[c] = * leafCount;				(* leafCount)++;
			} else {
				newNode = createNode(c, ind++, false);
				if (c != '#') {
					numToSym[* leafCount] = c;
					symToNum[c] = * leafCount;
					(* leafCount)++;

					if (isUnique(symList, * symCount, c)) {
						symList[* symCount] = c;
						(* symCount)++;
					}
				}
			}
			push(stak, &top, newNode);
		}
	}

	return stak[top];
}

link * pop(link * stak[MAX], int * top) {
	link * newNode;

	newNode = stak[* top];
	(* top)--;

	return newNode;
}

void push(link * stak[MAX], int * top, link * newNode) {
	(* top)++;
	stak[* top] = newNode;
}

void findFirstAndLastPos(link * root) {
	if (root != NULL && (root -> lChild != NULL || root -> rChild != NULL)) {
		findFirstAndLastPos(root -> lChild);
		findFirstAndLastPos(root -> rChild);

		if (root -> sym == 'E') {
			// no firstPos and lastPos
		} else if (root -> sym == '|') {
			root -> fLim = makeUnion(root -> lChild -> fPos, root -> lChild -> fLim, root -> rChild -> fPos, root -> rChild -> fLim, root -> fPos);
			root -> lLim = makeUnion(root -> lChild -> lPos, root -> lChild -> lLim, root -> rChild -> lPos, root -> rChild -> lLim, root -> lPos);
		} else if (root -> sym == '.') {
			if (root -> lChild -> null) {
				root -> fLim = makeUnion(root -> lChild -> fPos, root -> lChild -> fLim, root -> rChild -> fPos, root -> rChild -> fLim, root -> fPos);
			} else {
				root -> fLim = makeCopy(root -> lChild -> fPos, root -> lChild -> fLim, root -> fPos);
			}
			if (root -> rChild -> null) {
				root -> lLim = makeUnion(root -> rChild -> lPos, root -> rChild -> lLim, root -> lChild -> lPos, root -> lChild -> lLim, root -> lPos);
			} else {
				root -> lLim = makeCopy(root -> rChild -> lPos, root -> rChild -> lLim, root -> lPos);
			}
		} else if (root -> sym == '*') {
			root -> fLim = makeCopy(root -> lChild -> fPos, root -> lChild -> fLim, root -> fPos);
			root -> lLim = makeCopy(root -> lChild -> lPos, root -> lChild -> lLim, root -> lPos);
		} else {
			// it's not an alphabet, no fPos / lPos needed
		}
	}
}

int makeUnion(int s1[MAX], int lim1, int s2[MAX], int lim2, int d[MAX]) {
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;

	for (; i < lim1 && j < lim2;) {
		if (s1[i] < s2[j]) {
			d[k++] = s1[i++];
		} else {
			d[k++] = s2[j++];
		}
	}

	while (i < lim1) {
		d[k++] = s1[i++];
	}

	while (j < lim2) {
		d[k++] = s2[j++];
	}

	k = removeDuplicates(&d[0], k);

	return k;
}

int makeUnion(int * s, int sLim, int d[MAX], int dLim) {
	if (dLim == 0) {
		makeCopy(s, sLim, d);
		//printf("makeUnion  returned %d\n", sLim);
		//printf("dest was empty, array copied\n");
		return sLim;
	} else {
		int i;
		int j;

		for (i = 0; i < sLim; i++) {
			for (j = 0; j < dLim; j++) {
				if (s[i] == d[j]) {
					//printf("comparing %d and %d\n", s[i], d[j]);
					// this number is already present
					break;
				}
			}

			if (j == dLim) {
				// if number wasn't already present, then include it
				d[dLim++] = s[i];
				//printf("not present, added\tnewLen = %d\n", dLim);
			} else {
				//printf("present, not added\n");
			}
		}

		//printf("makeUnion returned %d\n", dLim);
		return dLim;
	}
}

int makeCopy(int * s, int lim, int d[MAX]) {
	int i;

	for (i = 0; i < lim; i++) {
		d[i] = s[i];
	}

	//printf("makeCopy returned %d\n", lim);
	return lim;
}

void showFirstAndLastPos(link * root) {
	if (root != NULL) {
		showFirstAndLastPos(root -> lChild);

		printf("%c\t", root -> sym);
		showArray(root -> fPos, root -> fLim);
		showArray(root -> lPos, root -> lLim);
		printf("\n");

		showFirstAndLastPos(root -> rChild);
	}
}

void showArray(int * arr, int lim) {
	int i;

	for (i = 0; i < lim; i++) {
		printf("%d", arr[i]);
	}
	printf("\t");
}

int removeDuplicates(int * arr, int lim) {
	int i;
	int j;
	int temp[MAX];
	int prev;

	i = 0;
	j = 0;
	prev = -1;

	while (j < lim) {
		if (arr[j] == prev) {
			// do nothing
		} else {
			temp[i++] = arr[j];
			prev = arr[j];
		}
		j++;
	}

	for (j = 0; j < i; j++) {
		arr[j] = temp[j];
	}

	return i;
}

void fillIndices(int followPos[][MAX], int lim) {
	int i;

	for (i = 1; i <= lim; i++) {
		followPos[i - 1][0] = i;
		followPos[i - 1][1] = 2;
	}
}

void findFollowPos(link * root, int followPos[][MAX]) {
	if (root != NULL) {
		if (root -> sym == '.') {
			for (int i = 0; i < root -> lChild -> lLim; i++) {
				addToFollowPos(followPos, root -> lChild -> lPos[i], root -> rChild -> fPos, root -> rChild -> fLim);
			}
		} else if (root -> sym == '*') {
			for (int i = 0; i < root -> lLim; i++) {
				addToFollowPos(followPos, root -> lPos[i], root -> fPos, root -> fLim);
			}
		}

		findFollowPos(root -> lChild, followPos);
		findFollowPos(root -> rChild, followPos);
	}
}

void addToFollowPos(int followPos[][MAX], int ind, int arr[MAX], int lim) {
	int folPosCount;
	int ele;
	bool flag;
	int i;
	int j;

	ind--;
	folPosCount = followPos[ind][1];

	for (i = 0; i < lim; i++) {
		ele = arr[i];
		flag = false;

		for (j = 2; j < 2 + folPosCount; j++) {
			if (followPos[ind][j] == ele) {
				flag = true;
				break;
			}
		}

		if (flag) {
			// do nothing
		} else {
			followPos[ind][folPosCount++] = ele;
		}
	}
	followPos[ind][1] = folPosCount;
}

void showFollowPos(int followPos[][MAX], int lim) {
	int i;
	int j;
	int fPosCount;

	printf("\nFollow-Pos table is:-\nNode\tFollow-pos\n");
	for (i = 0; i < lim; i++) {
		fPosCount = followPos[i][1];
		printf("%d\t", followPos[i][0]);
		for (j = 2; j < fPosCount; j++) {
			printf("%d", followPos[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void bubbleSort(int * arr, int lim) {
	if (lim > 1) {
		int i;
		int j;

		for (i = 0; i < lim - 1; i++) {
			for (j = 0; j < lim - 1 - i; j++) {
				if (arr[j + 1] > 0 && arr[j + 1] < arr[j]) {
					swapEle(arr, j, j + 1);
				}
			}
		}
	}
}

void swapEle(int * arr, int pos1, int pos2) {
	int temp;

	temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}

int makeTransTable(int trans[][MAX], int states[][MAX], int stateLen[MAX], char numToSym[MAX], int leafCount, char symList[MAX], int symCount, int followPos[][MAX]) {
	int i;
	int j;
	int k;
	int stateIndex;
	int crrStateLen;
	int newStateLen;
	int stateCount;
	int newState[MAX];

	// ab|*a.b.b.#.
	// 0*1.0.0*.1.0*.10*.E|.#.
	// 01|*0.0.01|*.#.

	stateCount = 1;
	// Consider state i
	for (i = 0; i < stateCount; i++) {
		crrStateLen = stateLen[i];

		//printf("\n\nConsider state %d:\t", i);
		//showArray(states[i], stateLen[i]);
		//printf("\tlen = %d\n", crrStateLen);

		// Consider every symbol j
		for (j = 0; j < symCount; j++) {
			newStateLen = 0;
			//printf("For symbol %c, \n", symList[j]);

			// Consider every kth element of state i
			for (k = 0; k < crrStateLen; k++) {
				//printf("Before reading %d, (", states[i][k]);
				//showArray(&followPos[states[i][k] - 1][2], followPos[states[i][k] - 1][1] - 2);
				//printf(")newState = ", states[i][k]);
				//showArray(newState, newStateLen);
				// if the jth s0*1.0.0*.1.0*.10*.E|.#.ymbol is same as kth element of the ith state
				if (symList[j] == numToSym[states[i][k] - 1]) {
					// add followpos of jth symbol to new state
					newStateLen = makeUnion(&followPos[states[i][k] - 1][2], followPos[states[i][k] - 1][1] - 2, newState, newStateLen);
					//printf("makeTransTable received %d\n", newStateLen);
					//printf("makeTransTable lim was %d\n", followPos[j][1]);
				}
				//printf("After reading %d, newState = ", states[i][k]);
				//showArray(newState, newStateLen);
				//printf("\n");
			}

			bubbleSort(newState, newStateLen);
			if ((stateIndex = alreadyPresent(states, stateLen, stateCount, newState, newStateLen)) < 0 && newStateLen > 0) {
				// create a new state
				makeCopy(newState, newStateLen, states[stateCount]);
				stateLen[stateCount] = newStateLen;
				stateIndex = stateCount;
				stateCount++;

				//printf("new state %d created\t", stateIndex);
				//showArray(newState, newStateLen);
			} else if (newStateLen > 0) {
				//printf("no new state created\t");
			} else {
				//printf("dead state encountered\t");
			}

			// add transition to table
			if (newStateLen > 0) {
				trans[i][j] = stateIndex;
				//printf("trans to state %d added\n", stateIndex);
			} else {
				trans[i][j] = -1;
			}
		}
	}

	return stateCount;
}

int alreadyPresent(int states[][MAX], int stateLen[MAX], int stateCount, int newState[MAX], int newStateLen) {
	int i;
	int j;

	// For every state i
	for (i = 0; i < stateCount; i++) {
		// Check if length of ith state is equal to length of new state
		if (stateLen[i] == newStateLen) {
			// If length are equal, compare every element of ith state with corresponding element of newState
			for (j = 0; j < newStateLen; j++) {
				if (states[i][j] != newState[j]) {
					break;
				}
			}

			// if ith state completely matches with newstate, return i
			if (j == newStateLen) {
				return i;
			}
		}
	}

	// if no state matches newstate, return -1
	return -1;
}

bool isUnique(char numToSym[MAX], int lim, char c) {
	int i;

	for (i = 0; i < lim; i++) {
		if (c == numToSym[i]) {
			return false;
		}
	}

	return true;
}

int findFinalStates(int states[][MAX], int stateLen[MAX], int stateCount, int fStates[MAX], int leafCount) {
	int i;
	int j;
	int fStateCount;

	fStateCount = 0;
	for (i = 0; i < stateCount; i++) {
		if (states[i][stateLen[i] - 1] == leafCount + 1) {
			fStates[fStateCount++] = i;
		}
	}

	return fStateCount;
}

void showFinalStates(int states[][MAX], int stateLen[MAX], int fStates[MAX], int fStateCount) {
	int i;

	printf("\nThe final states are:-\n");
	for (i = 0; i < fStateCount; i++) {
		showArray(states[fStates[i]], stateLen[i]);
		printf("\n");
	}
}

bool isFState(int state[MAX], int stateLen, int leafCount) {
	if (state[stateLen - 1] == leafCount + 1) {
		return true;
	} else {
		return false;
	}
}

void showStates(int states[][MAX], int stateLen[MAX], int stateCount, int fStates[MAX], int fStateCount, int leafCount) {
	int i;

	printf("The states are:-\n");
	for (i = 0; i < stateCount; i++) {
		printf("%d.\t", i);
		showArray(states[i], stateLen[i]);
		if (isFState(states[i], stateLen[i], leafCount)) {
			printf("F");
		}
		printf("\n");
	}
}

void showTransTable(int trans[][MAX], int states[][MAX], int stateLen[MAX], int stateCount, char symList[MAX], int symCount) {
	int i;
	int j;

	printf("\nThe Transition Table is:-\n\t");
	for (i = 0; i < symCount; i++) {
		printf("%c\t", symList[i]);
	}
	printf("\n");

	for (i = 0; i < stateCount; i++) {
		printf("%d.\t", i);
		for (j = 0; j < symCount; j++) {
			printf("%d\t", trans[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

