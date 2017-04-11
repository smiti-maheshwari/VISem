#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <vector>

using namespace std;

set <char> findFill(set <string>);
set <string> findCross(set <char>, set <char>);
void fillTable();
void makeUnion(set <char>&, set <char>);
void showSet(set <char>);
void showSet(set <string>);
void showVec(vector <set <char> >);
void showTable();

map <char, vector <string> > prod;
set <char> term;
vector <char> nonTerm;
vector <vector <set <char> > > table;
string str;
int sLen;

int main() {
	int n;
	char c;
	string s;

	// input the no of productions
	cin>>n;

	// input the production
	for (int i = 0; i < n; i++) {
		cin>>c;
		cin>>s;
		prod[c].push_back(s);
	}

	// input the string to be checked
	cin >> str;
	sLen = str.length();

	map <char, vector <string> >::iterator it;
	set <char>::iterator it1;

	for (it = prod.begin(); it != prod.end(); it++) {
		nonTerm.push_back(it->first);
	}

	for (it = prod.begin(); it != prod.end(); it++) {
		for (int i = 0; i < it->second.size(); i++) {
			s = it->second[i];
			for (int j = 0; j < s.length(); j++) {
				c = s[j];
				bool flag = false;

				for (int k = 0; k < nonTerm.size(); k++) {
					if (nonTerm[k] == c) {
						flag = true;
						break;
					}
				}

				if (flag == false) term.insert(c);
			}
		}
	}

	fillTable();
	showTable();

	if (table[sLen - 1][0].find(nonTerm[0]) != table[sLen - 1][0].end()) {
		printf("\nString is accepted\n");
	} else {
		printf("\nString is rejected\n");
	}

	return 0;
}

set <char> findFill(set <string> sSet) {
	string sProd;
	set <char> cSet;
	set <string>::iterator itSet;
	map <char, vector <string> >::iterator itMap;

	// iterate through set of strings
	for (itSet = sSet.begin(); itSet != sSet.end(); itSet++) {
		// iterate through set of non-terminals
		for (itMap = prod.begin(); itMap != prod.end(); itMap++) {
			// iterate through set of productions of non-terminal itMap -> first
			for (int i = 0; i < itMap -> second.size(); i++) {
				// read ith production rule of non-terminal itMap -> first
				sProd = itMap -> second[i];
				if (sProd.find(*itSet) != string::npos && sProd.length() == (*itSet).length()) {
					// if ith production matches the string, add non-terminal to set of chars
					cSet.insert(itMap -> first);
				}
			}
		}
	}

	return cSet;
}

set <string> findCross(set <char> setA, set <char> setB) {
	set <string> sSet;

	if (setA.empty() == false && setB.empty() == false) {
		set <char>::iterator itA;
		set <char>::iterator itB;
		string sStr;

		for (itA = setA.begin(); itA != setA.end(); itA++) {
			for (itB = setB.begin(); itB != setB.end(); itB++) {
				sStr.clear();
				sStr += *itA;
				sStr += *itB;
				sSet.insert(sStr);
			}
		}
	}

	return sSet;
}

void makeUnion(set <char>& dSet, set <char> sSet) {
	set <char>::iterator it;
	for (it = sSet.begin(); it != sSet.end(); it++) {
		dSet.insert(*it);
	}
}

void fillTable() {
	string sStr;
	set <char> cSetTmp;
	set <char> cSetEle;
	set <string> sSet;
	vector <set <char> > cSetVec;

	// insert first row in table
	for (int i = 0; i < sLen; i++) {
		sStr.clear();
		cSetTmp.clear();
		sSet.clear();

		sStr += str[i];
		sSet.insert(sStr);

		cSetTmp = findFill(sSet);
		cSetVec.push_back(cSetTmp);
	}
	table.push_back(cSetVec);

	// insert remaining rows in table
	for (int i = 1; i < sLen; i++) {
		cSetVec.clear();
		for (int j = 0; j < sLen - i; j++) {
			int r1 = 0;
			int c1 = j;
			int r2 = i - 1;
			int c2 = j + 1;

			cSetEle.clear();
			for (int k = 0; k < i; k++) {
				cSetTmp.clear();
				sSet.clear();

				sSet = findCross(table[r1++][c1], table[r2--][c2++]);
				cSetTmp = findFill(sSet);
				makeUnion(cSetEle, cSetTmp);
			}
			cSetVec.push_back(cSetEle);
		}
		table.push_back(cSetVec);
	}
}

void showTable() {
	printf("\nThe table is:-\n");
	cout << "\t  ";
	for (int j = 0; j < sLen; j++) {
		printf("j = %d\t", j);
	}

	printf("\n\t-------------------------------------------\n");
	for (int i = sLen - 1; i >= 0; i--) {
		printf("i = %d\t| ", i);
		for (int j = 0; j < sLen - i; j++) {
			showSet(table[i][j]);
		}
		cout << endl;
	}

	cout << "\t-------------------------------------------\n\t  ";
	for (int j = 0; j < sLen; j++) {
		cout << str[j] << "\t";
	}
	cout << endl;
}

void showSet(set <char> cSet) {
	set <char>::iterator it;
	for (it = cSet.begin(); it != cSet.end(); it++) {
		cout << *it << " ";
	}
	cout << "\t";
}

void showSet(set <string> cSet) {
	set <string>::iterator it;
	for (it = cSet.begin(); it != cSet.end(); it++) {
		cout << *it << " ";
	}
	cout << "\t";
}

void showVec(vector <set <char> > cSetVec) {
	for (int i = 0; i < cSetVec.size(); i++) {
		cout << "(";
		showSet(cSetVec[i]);
		cout << ") ";
	}
	cout << endl;
}

/*

9
S aBDh
B cC
C bC
C ?
D EF
E g
E ?
F f
F ?

8
S AB
S BC
A BA
A a
B CC
B b
C AB
C a
baaba

8
S AB
S BC
A BA
A a
B CC
B b
C AB
C a
baaa

5
S AB
A BB
A a
B AB
B b
aabba

8
S ?
S AB
S XB
T AB
T XB
X AT
A a
B b
aaabb

12
S AB
S BW
S BX
W AB
X a
B aa
B XW
B WX
B WW
B b
A a
A BA
bbabaa

8
S ?
S AB
S XB
T AB
T XB
X AT
A a
B b
aaabbb

*/
