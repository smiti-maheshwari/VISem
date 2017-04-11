#include <bits/stdc++.h>

using namespace std;

map <char, vector <string> > prod;
set <char> term;
vector <char> nonTerm;
map <char, set <char> > first;
map <char, set <char> > follow;
map <char, vector <string> > table;
map <char, vector <int> > table1;
vector <string> rules;
vector <int> r1;

void computeFirst(vector <string> v, char x) {
	int i, j;
	string str;
	set <char>::iterator it;
	bool flag;

	for (i = 0; i < v.size(); i++) {
		str = v[i];
		flag = false;
		for (j = 0; j < str.length(); j++) {
			if (term.find(str[j]) != term.end())
				flag = true;

			if (flag == true) {
				first[x].insert(str[j]);
				break;
			} else {
				computeFirst(prod[str[j]], str[j]);
				set <char> tmp = first[str[j]];
				it = tmp.begin();
				bool f = false;
				while (it != tmp.end()) {
					if (*it == '?') f = true;
					else first[x].insert(*it);
					it++;
				}
				if (f == false) break;
				if (f == true && j == str.length() - 1) first[x].insert('?');
			}
		}
	}
}

int computeFollow(char s, set <char>& m) {
	int x, oSize, fSize;
	bool hasEpsilon;
	set <char>::iterator it;

	if (follow[s].empty() == false) {
		m = follow[s];
		oSize = m.size();
	} else oSize = 0;

	for (int i = 0; i < nonTerm.size(); i++) {
		vector <string> l = prod[nonTerm[i]];

		for (int j = 0; j < l.size(); j++) {
			string temp = l[j];

			for (int k = 0; k < temp.length(); k++) {
				if (s == temp[k]) {	// rule A -> aBb
					if (k < temp.length() - 1) {// if terminal

						if (term.find(temp[k + 1]) != term.end()) m.insert(temp[k + 1]);
						else { // if non-terminal
							hasEpsilon = true;
							x = k + 1;

							while (x < temp.length() && hasEpsilon == true) {
								set <char> tt = first[temp[x]];
								hasEpsilon = false;

								for (it = tt.begin(); it != tt.end(); it++) {
									if (*it == '?') hasEpsilon = true;
									else m.insert(*it);
								}

								if (hasEpsilon == true) x++;
							}

							if (hasEpsilon == true && x == temp.length()) {
								if (follow[nonTerm[i]].size() > 0) {
									for (it = follow[nonTerm[i]].begin(); it != follow[nonTerm[i]].end(); it++) {
										m.insert(*it);
									}
								}
							}
						}
					} else {
						if (follow[nonTerm[i]].size() > 0) {
							for (it = follow[nonTerm[i]].begin(); it != follow[nonTerm[i]].end(); it++) {
								m.insert(*it);
							}
						}
					}
				}
			}
		}
	}
	follow[s] = m;
	fSize = m.size();

	if (fSize > oSize) return 1;
	else return 0;
}

void parseTable() {
	cout << "\nParsing table rules:-\n";
	int k = 0;
	vector <char>::iterator it;

	for (int i = 0; i < nonTerm.size(); i++) {
		vector <string> l = prod[nonTerm[i]];
		for (int j = 0; j < l.size(); j++) {
			printf("%d : %c -> ", k++, nonTerm[i]);
			cout << prod[nonTerm[i]][j] << endl;
			rules.push_back(prod[nonTerm[i]][j]);
			r1.push_back(i);
		}
	}

	for (int i = 0; i < nonTerm.size(); i++) {
		vector <string> l = prod[nonTerm[i]];
		set <char> l1 = first[nonTerm[i]];
		set <char> l2 = follow[nonTerm[i]];

		vector <string> m;
		vector <int> m1;
		set <char>::iterator j;

		k = 0;
		for (j = term.begin(); j != term.end(); j++) {
			if (k >= l.size()) {
				k = 0;
			}

			if (l1.find('?') != l1.end()) {
				if (l1.find(*j) != l1.end()) {
					m.push_back(l[k]);
					k++;
				} else if (l2.find(*j) != l2.end()) {
					m.push_back("?");
				} else {
					m.push_back("-");
				}
			} else {
				if (l1.find(*j) != l1.end()) {
					m.push_back(l[k]);
					k++;
				} else {
					m.push_back("-");
				}
			}

			if (m[m.size() - 1] == "-") {
				m1.push_back(-1);
			} else {
				for (int ii = 0; ii < rules.size(); ii++) {
					if (r1[ii] == i && m[m.size() - 1] == rules[ii]) {
						m1.push_back(ii);
						break;
					}
				}
			}
		}

		/*
		cout << "\n\n================\nTerms:\t";
		for (j = term.begin(); j != term.end(); j++) {
			cout << *j << "\t";
		}
		cout << "\n-----------------------------------";

		cout << "\nRow " << nonTerm[i] << "\n";
		cout << "m:\t";
		for (int ii = 0; ii < m.size(); ii++) {
			cout << m[ii] << "\t";
		}

		cout << "\nm1:\t";
		for (int ii = 0; ii < m1.size(); ii++) {
			cout << m1[ii] << "\t";
		}
		cout << "\n================\n\n";
		*/

		table[nonTerm[i]] = m;
		table1[nonTerm[i]] = m1;
	}
}

int main() {
	int n, i;
	char c;
	string str;
	int flg;

	cin>>n;

	for (i = 0; i < n; i++) {
		cin>>c;
		cin>>str;
		prod[c].push_back(str);
		if (i == 0) follow[c].insert('$');
	}

	map <char, vector <string> >::iterator it;
	set <char>::iterator it1;

	for (it = prod.begin(); it != prod.end(); it++) {
		nonTerm.push_back(it->first);
	}

	for (it = prod.begin(); it != prod.end(); it++) {

		for (i = 0; i < it->second.size(); i++) {
			str = it->second[i];

			for (int j = 0; j < str.length(); j++) {
				c = str[j];
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

	for (i = 0; i < nonTerm.size(); i++) computeFirst(prod[nonTerm[i]], nonTerm[i]);

	map <char, set <char> >::iterator it2;
	cout<<"\nFirst"<<endl;

	for (it2 = first.begin(); it2 != first.end(); it2++) {
		cout<<it2->first<<"\t";
		for (it1 = it2->second.begin(); it1 != it2->second.end(); it1++) cout<<(*it1)<<" ";
		cout<<endl;
	}

	int ctr = 0;
	while (true) {
		flg = 0;
		for (i = 0; i < nonTerm.size(); i++) {
			set <char> m;
			flg += computeFollow(nonTerm[i], m);
		}
		if (flg == 0) {
			break;
		}
	}

	cout << endl << endl;
	cout<<"Follow"<<endl;

	for (it2 = follow.begin(); it2 != follow.end(); it2++) {
		cout<<it2->first<<"\t";
		for (it1 = it2->second.begin(); it1 != it2->second.end(); it1++) cout<<(*it1)<<" ";
		cout<<endl;
	}

	term.insert('$');
	term.erase('?');
	parseTable();

	// PARSING TABLE
	set <char>::iterator it9;
	vector <char>::iterator it8;

	cout << endl << "Parsing Table" << endl << "\t";
	for (it9 = term.begin(); it9 != term.end(); it9++) {
		cout << *it9 << "\t";
	}
	cout << endl << "\t-------------------------------------------------" << endl;
	for (it8 = nonTerm.begin(); it8 != nonTerm.end(); it8++) {
		cout << *it8 << ":\t";
		vector <string> l = table[*it8];
		for (int j = 0; j < l.size(); j++) {
			cout << l[j] << "\t";
		}
		cout << endl;
	}

	// INDEX

	cout << endl << "\t";
	for (it9 = term.begin(); it9 != term.end(); it9++) {
		cout << *it9 << "\t";
	}
	cout << endl << "\t-------------------------------------------------" << endl;
	for (it8 = nonTerm.begin(); it8 != nonTerm.end(); it8++) {
		cout << *it8 << ":\t";
		vector <int> l = table1[*it8];
		for (int j = 0; j < l.size(); j++) {
			if (l[j] != -1) cout << l[j] << "\t";
			else cout << "-\t";
		}
		cout << endl;
	}


	return 0;
}



/*
8
E TA
A +TA
A ?
T FB
B *FB
B ?
F (E)
F i

10
S ABCDE
A a
A ?
B b
B ?
C c
D d
D ?
E e
E ?

6
S Bb
S Cd
B aB
B ?
C cC
C ?

9
S ACB
S CbB
S Ba
A da
A BC
B g
B ?
C h
C ?

3
S AB
A a
B b

5
S aAB
S Ba
A S
B b
B ?

7
S A
A Bb
A Cd
B aB
B ?
C cC
C ?

6
S aB
S ?
B bC
B ?
C cS
C ?

4
S AaAb
S BbBa
A ?
B ?

3
S aAB
B d
A bc

*/

