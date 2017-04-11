#include <bits/stdc++.h>
#define prod pair < char, string >

using namespace std;

map <char, vector <string> > grammar;	// input grammar
vector <char> nonTerm;				    // set of non terminals
set <char> nTSet;
set <char> terminals;
char cannmat[100][100];				   	//symbol table b/w two states
char augStartSymbol = 'Z';				// augmented start symbol
int cnt;								// count for state number
map < vector < prod > , int > rcann_mp;	// visted map for states
map <prod, int> prodNum;
map <int, prod> rprodNum;

map <char, int> ter_map;				// get index from terminal char
map <char, int> non_ter_map;			// get index from non-terminal char
map <int, char> rter_map;				// get terminal char from index
map <int, char> rnon_ter_map;			// get non-terminal char from index

int parsing_table[100][100];

bool terminal(char ch) {
	if (ch >= 'A' && ch <= 'Z')
		return false;
	return true;
}

void print(vector < prod > state) {
	for (int i = 0; i < state.size(); i++) {
		cout << state[i].first << " " << state[i].second << endl;
	}
}

vector < prod > closure(prod p) {
	vector < prod > state;
	string tmp = p.second;
	int ind = tmp.find('.');

	state.push_back(make_pair(p.first, tmp));

	if (ind == tmp.length() - 1) {
		return state;
	}

	map < prod, bool > visited;
	for (int i = 0; i < state.size(); i++) {
		tmp = state[i].second;
		ind = tmp.find('.');
		ind++;
		char sym = tmp[ind];
		if (terminal(sym)) {
			continue;
		}

		vector < string > temp = grammar[sym];

		for (int i = 0; i < temp.size(); i++) {
			string pd = "." + temp[i];
			if (visited.find(make_pair(sym, pd)) == visited.end()) {
				state.push_back(make_pair(sym, pd));
				visited[make_pair(sym, pd)] = true;
			}
		}
	}

	return state;
}

// function for creating item sets

void createItemSet() {
	queue < vector < prod > > q;
	string temp = "." + grammar[augStartSymbol][0];
	vector <prod> tmp;	// collections of closure
	cnt = 0;
	tmp = closure(make_pair(augStartSymbol, temp));
	//cann_mp[cnt] = tmp;
	rcann_mp[tmp] = cnt;
	q.push(tmp);

	while (q.empty() != true) {
		tmp = q.front();
		q.pop();
		cout << "FROM QUEUE : " << rcann_mp[tmp] << endl;
		print(tmp);
		cout << endl;

		map < char, vector < prod > > mp;	// visted map for check same closure
		vector <char> trans_sym;			// collections of all transition symbols
		for (int i = 0; i < tmp.size(); i++) {
			prod pt = tmp[i];
			string dtmp = pt.second;
			int ind = dtmp.find('.');
			if (ind == dtmp.length() - 1) continue; // condition to check the end of states

			swap(dtmp[ind], dtmp[ind + 1]);
			vector <prod> state = closure(make_pair(pt.first, dtmp));
			trans_sym.push_back(dtmp[ind]);
			for (int j = 0; j < state.size(); j++)
				mp[dtmp[ind]].push_back(state[j]);
		}

		cout << "States\n";
		for (int i = 0; i < trans_sym.size(); i++) {
			char sym = trans_sym[i];
			vector <prod> state = mp[sym];
			if (rcann_mp.find(state) == rcann_mp.end()) {
				cnt++;
				rcann_mp[state] = cnt;
				//cann_mp[cnt] = state;
				q.push(state);
			}

			cannmat[rcann_mp[tmp]][rcann_mp[state]] = sym;

			bool flg = false;
			for (int j = 0; j < state.size(); j++) {
				string str = state[j].second;
				int id = str.find('.');
				if (id == str.length() - 1)
					flg = true;
			}

			if (flg && state.size() >= 2) {
				cout << "Grammar is not LR(0)\n";
				return ;
			}

			if (flg) {
				prod pr = state[0];
				int id = pr.second.find('.');
				pr.second.erase(id, 1);

				int val = prodNum[pr];

				if (val == 0) {
					parsing_table[rcann_mp[state]][ter_map['$']] = INT_MIN;
				} else {
					map < char,  int > :: iterator it;
					it = ter_map.begin();
					while (it != ter_map.end()) {
						parsing_table[rcann_mp[state]][it->second] = val * (-1);
						it++;
					}
				}
				//parsing_table[rcann_mp[tmp]][]
			}
			int idc;
			if (terminal(sym)) {
				idc = ter_map[sym];
			} else {
				idc = non_ter_map[sym];
			}
			parsing_table[rcann_mp[tmp]][idc] = rcann_mp[state];
		}
	}

	int sz = ter_map.size() + non_ter_map.size();
	//cout << sz << endl;
	for (int i = 0; i <= cnt; i++) {
		for (int j = 0; j < sz; j++) {
			cout << parsing_table[i][j] << " ";
		}
		cout << endl;
	}

	// print headings
	set <char>::iterator it;
	printf("\nThe parsing table is:-\n\t");
	for (it = terminals.begin(); it != terminals.end(); it++) {
		printf("%c\t", *it);
	}
	for (int i = 1; i < nonTerm.size(); i++) {
		printf("%c\t", nonTerm[i]);
	}
	printf("\n");

	for (int i = 0; i <= cnt; i++) {
		printf("st%d\t", i);
		int j;
		for (j = 0; j < terminals.size(); j++) {
			if (parsing_table[i][j] > 0) {
				printf("s%d\t", parsing_table[i][j]);
			} else if (parsing_table[i][j] == INT_MIN) {
				printf("acc\t");
			} else if (parsing_table[i][j] < 0) {
				printf("r%d\t", (-1 * parsing_table[i][j]));
			} else if (parsing_table[i][j] == 0) {
				printf("_\t");
			}
		}
		//printf("_\t");
		for (; j < terminals.size() + nonTerm.size() - 1; j++) {
			if (parsing_table[i][j] == 0) {
				printf("_\t");
			} else if (parsing_table[i][j] > 0) {
				printf("%d\t", parsing_table[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n\n");

	for (int i = 0; i <= cnt; i++) {
		for (int j = 0; j <= cnt; j++) {
			if (cannmat[i][j] != '\0')
				cout << cannmat[i][j] << " ";
			else
				cout << "-" << " ";
		}
		cout << endl;
	}
}

void parse_input_string(string str) {
	stack <int> stk;
	stk.push(0);
	int i = 0;
	str = str + "$";

	while (true) {
		int tp = stk.top();
		int val = parsing_table[tp][ter_map[str[i]]];

		if (val > 0) {
			stk.push(val);
			i++;
		} else if (val < 0 && val != INT_MIN) {
			val = val * (-1);
			prod pr = rprodNum[val];
			cout << pr.first << " -> " << pr.second << endl;
			int l = pr.second.length();

			while (l--) stk.pop();

			tp = stk.top();
			stk.push(parsing_table[tp][non_ter_map[pr.first]]);
		} else if (val == INT_MIN) {
			cout << "ACCEPTED" << endl;
			return;
		} else {
			cout << "ERROR";
			return ;
		}
	}
}

int main() {
	int n, i;
	char c, c1, c2;
	string str;
	int prod_ctr = 1;

	cin>>n;
	nonTerm.push_back(augStartSymbol);

	for (int i = 0; i < n; i++) {
		cin>>c>>c1>>c2>>str;

		if (nTSet.find(c) == nTSet.end()) {
			nonTerm.push_back(c);
			nTSet.insert(c);
		}

		grammar[c].push_back(str);
		prod b;

		b.first = c;
		b.second = str;
		prodNum[b] = prod_ctr;
		rprodNum[prod_ctr] = b;
		prod_ctr++;

		for (int j = 0; j < str.length(); j++) {
			if (terminal(str[j]) == true && str[j] != '?') terminals.insert(str[j]);
		}
	}


	set <char> :: iterator it;

	string str1(1, nonTerm[1]);
	grammar[augStartSymbol].push_back(str1);
	terminals.insert('$');

	int cnt = 0;
	it = terminals.begin();
	cout << endl;

	while (it != terminals.end()) {
		ter_map[*it] = cnt;
		rter_map[cnt] = *it;

		cout << ter_map[*it] << " -> " << rter_map[cnt] << endl;
		cnt++;
		it++;

	}
	cout << endl;

	for(int i = 1; i <= nonTerm.size() - 1; ++i) {
		non_ter_map[nonTerm[i]] = cnt;
		rnon_ter_map[cnt] = nonTerm[i];
		cout << non_ter_map[nonTerm[i]] << " -> " << rnon_ter_map[cnt] << endl;
		cnt++;
	}
	cout << endl;

	createItemSet();

	printf("Enter string to be checked:\t");
	string strChk;
	cin >> strChk;
	parse_input_string(strChk);

	return 0;
}

/*
6
E->E+T
E->T
T->T*F
T->F
F->(E)
F->i

4
S->(L)
S->a
L->L,S
L->S

4
V->S
S->AA
A->aA
A->b

3
S->AA
A->aA
A->b

2
S->(S)
S->x
*/
