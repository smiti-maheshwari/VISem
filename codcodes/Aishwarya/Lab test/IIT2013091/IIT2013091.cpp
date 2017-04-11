#include <bits/stdc++.h>

using namespace std;
#define LIM 100005
#define pb push_back
#define mp make_pair
#define SYMBOL "."
#define Structure pair < pair <string, vector <string> >, set <string> >

vector < vector <string> > productions[LIM];
map <string, int> idNonTerminal;
int id = 1;
char input[LIM];
vector <string> nonTerminalOrder;
set <string> all;
set <string> first[LIM];
set <string> follow[LIM];
bool foundFirst[LIM];
bool foundFollow[LIM];
map < set < Structure >, int> CLR1Index;
set < Structure > CLR1Table[LIM];
map <string, int> CLR1Shift[LIM];
map < pair <int, string>, int> Goto;
map < pair <int, string>, pair <string, int> > Action;
pair <string, vector <string> > production[LIM];
map < pair <string, vector <string> >, int> productionID;

bool INPUT()
{

	int i = 0;
	char ch = getchar();
	while (ch != EOF && ch != '\n') {
		if (ch != '\r') {
			input[i] = ch;
			++i;
		}
		ch = getchar();
	}
	input[i] = '\0';
	return i;
}

void getInput(char a[])
{

	freopen(a, "r", stdin);
	string s;
	int i, ind;
	bool found;
	all.insert("$");
	map <string, int> :: iterator it;

	while (INPUT()) {
		s.clear();
		ind = -1;
		found = false;

		for (i = 0; input[i]; ++i) {
			if (input[i] == ' ' || input[i] == '\t') {
				if (s.size()) {
					if (s == "->" || s == "|") {
						s.clear();
						productions[ind].pb(vector <string> ());
						continue;
					}

					all.insert(s);

					if (found)
						productions[ind][productions[ind].size()-1].pb(s);
					else {
						found = true;
						it = idNonTerminal.find(s);
						if (it == idNonTerminal.end()) {
							ind = id;
							nonTerminalOrder.pb(s);
							idNonTerminal[s] = id;
							++id;
						}
						else
							ind = it->second;
					}
					s.clear();
				}
			}
			else
				s.pb(input[i]);
		}

		if (s.size()) {
			all.insert(s);
			assert(ind != -1);
			productions[ind][productions[ind].size()-1].pb(s);
		}
	}

	idNonTerminal["Start"] = 0;
	nonTerminalOrder.insert(nonTerminalOrder.begin(), "Start");
	productions[0].pb(vector <string> ());
	productions[0][0].pb(nonTerminalOrder[1]);
}

void remove_left_recursion()
{

	int i, j, k, l, m, ind, n = nonTerminalOrder.size();
	string tmp;
	bool directRecursionPresent;
	map <string, int> :: iterator it;

	for (i = 0; i < n; ++i) {
		it = idNonTerminal.find(nonTerminalOrder[i]);
		assert(it != idNonTerminal.end());
		ind = it->second;
		directRecursionPresent = false;

		for (j = 0; j < productions[ind].size(); ++j) {
			tmp = productions[ind][j][0];
			it = idNonTerminal.find(tmp);
			if (it == idNonTerminal.end())
				continue;

			directRecursionPresent = (directRecursionPresent ? directRecursionPresent : it->second == i);
			if (it->second >= i)
				continue;

			productions[ind][j].erase(productions[ind][j].begin());
			k = it->second;

			for (l = 0; l < productions[k].size(); ++l) {
				productions[ind].pb(productions[ind][j]);
				for (m = productions[k][l].size()-1; m >= 0; --m)
					productions[ind][productions[ind].size()-1].insert(productions[ind][productions[ind].size()-1].begin(), productions[k][l][m]);
			}
			productions[ind].erase(productions[ind].begin()+j);
			--j;
		}

		if (!directRecursionPresent)
			continue;

		tmp = nonTerminalOrder[i];

		while (idNonTerminal.find(tmp) != idNonTerminal.end())
			tmp += "'";

		idNonTerminal[tmp] = id;
		nonTerminalOrder.pb(tmp);
		all.insert(tmp);

		k = id;
		++id;

		for (j = 0; j < productions[ind].size(); ++j) {
			if (productions[ind][j][0] == nonTerminalOrder[i]) {
				productions[ind][j].erase(productions[ind][j].begin());
				productions[ind][j].pb(tmp);
				productions[k].pb(productions[ind][j]);
				productions[ind].erase(productions[ind].begin()+j);
				--j;
			} else {

				if (productions[ind][j][0] == "EPS")
					productions[ind][j][0] = tmp;

				else
					productions[ind][j].pb(tmp);
			}
		}

		if (productions[ind].size() == 0) {
			productions[ind].pb(vector <string> ());
			productions[ind][0].pb(tmp);
		}

		productions[k].pb(vector <string> ());
		productions[k][productions[k].size()-1].pb("EPS");
	}
}



void find_first(int n)
{

	if (foundFirst[n])
		return;

	foundFirst[n] = true;
	int i, j, k;
	bool foundEPS;
	map <string, int> :: iterator it;
	set <string> :: iterator it2;

	for (i = 0; i < productions[n].size(); ++i) {
		if (productions[n][i][0] == "EPS") {
			first[n].insert("EPS");
			continue;
		}

		for (j = 0; j < productions[n][i].size(); ++j) {
			it = idNonTerminal.find(productions[n][i][j]);
			if (it == idNonTerminal.end()) {
				first[n].insert(productions[n][i][j]);
				break;
			}
			else {

				find_first(it->second);
				foundEPS = false;
				for (it2 = first[it->second].begin(); it2 != first[it->second].end(); ++it2) {
					if (*it2 == "EPS")
						foundEPS = true;
					else
						first[n].insert(*it2);
				}
				if (!foundEPS)
					break;
			}
		}
	}
}

void find_first()
{

	int i;

	for (i = 0; i < nonTerminalOrder.size(); ++i)
		find_first(idNonTerminal[nonTerminalOrder[i]]);
}



void find_follow(int n)
{

	if (foundFollow[n])
		return;

	foundFollow[n] = true;
	int i, j, k, l, ind;
	bool found;

	map <string, int> :: iterator it, it2;
	set <string> :: iterator it3;

	if (n == 0)
		follow[0].insert("$");

	for (it = idNonTerminal.begin(); it != idNonTerminal.end(); ++it) {
		ind = it->second;
		for (j = 0; j < productions[ind].size(); ++j) {
			for (k = 0; k < productions[ind][j].size(); ++k) {
				it2 = idNonTerminal.find(productions[ind][j][k]);
				if (it2 != idNonTerminal.end() && it2->second == n) {

					for (l = k+1; l < productions[ind][j].size(); ++l) {
						it2 = idNonTerminal.find(productions[ind][j][l]);

						if (it2 == idNonTerminal.end()) {
							follow[n].insert(productions[ind][j][l]);
							goto l1;
						}
						else {
							found = false;
							for (it3 = first[it2->second].begin(); it3 != first[it2->second].end(); ++it3) {
								if (*it3 == "EPS")
									found = true;
								else
									follow[n].insert(*it3);
							}

							if (!found)
								break;
						}
					}

					if (l == productions[ind][j].size() && ind != n) {
						find_follow(ind);
						for (it3 = follow[ind].begin(); it3 != follow[ind].end(); ++it3)
							follow[n].insert(*it3);
					}
l1: ;
				}
			}
		}
	}
}



void find_follow()

{

	int i;

	for (i = 0; i < nonTerminalOrder.size(); ++i)

		find_follow(idNonTerminal[nonTerminalOrder[i]]);

}



set <string> lookAhead(vector <string> &v1, int k, set <string> &v2)

{

	assert(v1[k] == SYMBOL);

	set <string> ans;

	int i;

	map <string, int> :: iterator it;

	set <string> :: iterator it1;

	bool found;



	for (i = k+2; i < v1.size(); ++i) {

		it = idNonTerminal.find(v1[i]);

		if (it == idNonTerminal.end()) {

			ans.insert(v1[i]);

			return ans;

		}

		found = false;

		for (it1 = first[it->second].begin(); it1 != first[it->second].end(); ++it1) {

			if (*it1 == "EPS")

				found = true;

			else

				ans.insert(*it1);

		}

		if (!found)

			return ans;

	}


	for (it1 = v2.begin(); it1 != v2.end(); ++it1)

		ans.insert(*it1);



	return ans;



}



set < Structure > generateWholeTable(vector < Structure > tmpV)

{

	set < Structure > tmpS;

	set < Structure > :: iterator it1;

	Structure tmpP;

	int i, j, k, l;

	string s;

	map <string, int> :: iterator it;
	set <string> :: iterator it2;



	for (i = 0; i < tmpV.size(); ++i)

		tmpS.insert(tmpV[i]);

	for (i = 0; i < tmpV.size(); ++i) {

		for (k = 0; k+1 < tmpV[i].first.second.size(); ++k) {

			if (tmpV[i].first.second[k] != SYMBOL)

				continue;

			s = tmpV[i].first.second[k+1];

			it = idNonTerminal.find(s);

			if (it == idNonTerminal.end())

				continue;

			for (j = 0; j < productions[it->second].size(); ++j) {

				tmpP = mp(mp(s, productions[it->second][j]), lookAhead(tmpV[i].first.second, k, tmpV[i].second));

				tmpP.first.second.insert(tmpP.first.second.begin(), SYMBOL);

				for (l = 0; l < tmpP.first.second.size(); ++l) {

					if (tmpP.first.second[l] == "EPS") {

						tmpP.first.second.erase(tmpP.first.second.begin()+l);

						break;

					}

				}

				if (tmpS.find(tmpP) == tmpS.end()) {

					tmpV.pb(tmpP);

					tmpS.insert(tmpP);

				}

			}

		}

	}



	tmpV.clear();

	for (it1 = tmpS.begin(); it1 != tmpS.end(); ++it1)

		tmpV.pb(*it1);



	for (i = 0; i < tmpV.size(); ++i) {

		for (j = i+1; j < tmpV.size(); ++j) {

			if (tmpV[i].first == tmpV[j].first) {

				for (it2 = tmpV[j].second.begin(); it2 != tmpV[j].second.end(); ++it2)

					tmpV[i].second.insert(*it2);

				tmpV.erase(tmpV.begin()+j);

				--j;

			}

		}

	}



	tmpS.clear();



	for (i = 0; i < tmpV.size(); ++i)

		tmpS.insert(tmpV[i]);



	return tmpS;

}



void createCLR1ParsingTable()

{

	id = 0;

	set < Structure > tmpS, currentState;

	set < Structure > :: iterator it;

	Structure tmpP;

	vector < Structure > tmpV;

	set <string> :: iterator it1;

	set <string> V;



	V.insert("$");

	tmpP = mp(mp(nonTerminalOrder[0], productions[0][0]), V);

	tmpP.first.second.insert(tmpP.first.second.begin(), SYMBOL);

	tmpV.pb(tmpP);

	tmpS = generateWholeTable(tmpV);

	CLR1Index[tmpS] = id;

	CLR1Table[id] = tmpS;

	++id;

	int i, j, k;



	for (i = 0; i < id; ++i) {



		currentState = CLR1Table[i];



		for (it1 = all.begin(); it1 != all.end(); ++it1) {

			tmpV.clear();



			for (it = currentState.begin(); it != currentState.end(); ++it) {

				for (j = 0; j+1 < (*it).first.second.size(); ++j) {

					if ((*it).first.second[j] == SYMBOL && (*it).first.second[j+1] == *it1) {

						tmpV.pb(mp((*it).first, (*it).second));

						tmpV[tmpV.size()-1].first.second.insert(tmpV[tmpV.size()-1].first.second.begin()+j+2, SYMBOL);

						tmpV[tmpV.size()-1].first.second.erase(tmpV[tmpV.size()-1].first.second.begin()+j);

					}

				}

			}



			if (tmpV.size() == 0)

				continue;



			tmpS = generateWholeTable(tmpV);

			if (CLR1Index.find(tmpS) == CLR1Index.end()) {

				CLR1Index[tmpS] = id;

				CLR1Table[id] = tmpS;

				++id;

			}

			k = CLR1Index[tmpS];

			CLR1Shift[i][*it1] = k;



		}

	}

}



void provideEachProductionID()

{

	int i, j, ind;

	pair <string, vector <string> > tmp;

	id = 0;

	for (i = 0; i < nonTerminalOrder.size(); ++i) {

		ind = idNonTerminal[nonTerminalOrder[i]];

		for (j = 0; j < productions[ind].size(); ++j) {

			tmp = mp(nonTerminalOrder[i], productions[ind][j]);

			productionID[tmp] = id;

			production[id] = tmp;

			++id;

		}

	}

}



void createActionAndGotoTable()

{

	map < set < Structure >, int> :: iterator it;

	set < Structure > currentState;

	map <string, int> :: iterator it1;

	set < Structure > :: iterator it2;

	set <string> :: iterator it3;

	map < pair <int, string>, pair <string, int> > :: iterator it4;

	int ind, tmpInd;

	string s;

	vector <string> tmpV;
	set <string> tmpV2;



	for (it = CLR1Index.begin(); it != CLR1Index.end(); ++it) {

		currentState = it->first;

		ind = it->second;

		for (it1 = CLR1Shift[ind].begin(); it1 != CLR1Shift[ind].end(); ++it1) {

			if (idNonTerminal.find(it1->first) != idNonTerminal.end())

				Goto[mp(ind, it1->first)] = it1->second;

			else

				Action[mp(ind, it1->first)] = mp("Shift", it1->second);

		}

		for (it2 = currentState.begin(); it2 != currentState.end(); ++it2) {

			s = (*it2).first.first;

			tmpV = (*it2).first.second;

			tmpV2 = (*it2).second;

			if (tmpV[tmpV.size()-1] != SYMBOL)

				continue;

			tmpV.erase(tmpV.end()-1);

			if (tmpV.size() == 0)

				tmpV.pb("EPS");



			assert(idNonTerminal.find(s) != idNonTerminal.end());

			tmpInd = idNonTerminal[s];



			for (it3 = tmpV2.begin(); it3 != tmpV2.end(); ++it3) {

				if (idNonTerminal.find(*it3) != idNonTerminal.end())

					assert(0);

				it4 = Action.find(mp(ind, *it3));

				if (it4 != Action.end()) {

					if (it4->second.first == "Shift")

						cout << "There is Shift Reduce Conflict. This Grammar is not CLR(1)." << endl;

					else

						cout << "There is Reduce Reduce Conflict. This Grammar is not CLR(1)." << endl;

					// assert(0);

					continue;

				}

				assert(productionID.find(mp(s, tmpV)) != productionID.end());

				if (s == "Start")

					Action[mp(ind, *it3)] = mp("Accept", productionID[mp(s, tmpV)]);

				else

					Action[mp(ind, *it3)] = mp("Reduce", productionID[mp(s, tmpV)]);

			}

		}

	}

}



void printStackAndInput(stack < pair <string, int> > stck, vector <string> inp, int i)

{

	stack < pair <string, int> > s;

	while (!stck.empty()) {

		s.push(stck.top());

		stck.pop();

	}

	stck = s;

	while (!stck.empty()) {

		if (stck.top().first.size())

			cout << stck.top().first << " ";

		else

			cout << stck.top().second << " ";

		stck.pop();

	}

	cout << " ***** ";

	while (i < inp.size())

		cout << inp[i++];

	cout << endl;

}



void checkForGivenInput(char a[])

{

	freopen(a, "r", stdin);

	INPUT();

	vector <string> inp;

	string s, tmp;

	int i, j;

	pair <string, string> tmp2;

	map < pair <int, string>, int> :: iterator it;

	map < pair <int, string>, pair <string, int> > :: iterator it1;

	pair <string, vector <string> > Temp;

	int ind;



	for (i = 0; input[i]; ++i) {

		if (input[i] == ' ' || input[i] == '\t') {

			if (s.size())

				inp.pb(s);

			s.clear();

		}

		else

			s.pb(input[i]);

	}

	if (s.size())

		inp.pb(s);

	inp.pb("$");



	stack < pair <string, int> > stck;

	stck.push(mp("", 0));



	for (i = 0; i < inp.size(); ) {



		printStackAndInput(stck, inp, i);



		ind = stck.top().second;

		if (idNonTerminal.find(inp[i]) != idNonTerminal.end()) {

			stck.push(mp(inp[i], 0));

			it = Goto.find(mp(ind, inp[i]));

			if (it == Goto.end()) {

				cout << "Given input \"" << input << "\" can not be parsed using given grammar\n";

				return;

			}

			printf("Shift\n");

			stck.push(mp("", it->second));

			++i;

			continue;

		}

		it1 = Action.find(mp(ind, inp[i]));

		if (it1 == Action.end()) {

			cout << "Given input \"" << input << "\" can not be parsed using given grammar, ERROR : Unbalanced parenthesis and missing operand\n";

			return;

		}

		if (it1->second.first == "Shift") {

			stck.push(mp(inp[i], 0));

			stck.push(mp("", it1->second.second));

			++i;

			printf("Shift\n");

			continue;

		}

		else if (it1->second.first == "Reduce") {

			ind = it1->second.second;

			Temp = production[ind];

			if (Temp.second[0] != "EPS")

				ind = 2*Temp.second.size();

			else

				ind = 0;

			while (!stck.empty() && ind) {

				stck.pop();

				--ind;

			}

			if (ind) {

				cout << "Given input \"" << input << "\" can not be parsed using given grammar\n";

				return;

			}

			ind = stck.top().second;

			stck.push(mp(Temp.first, 0));

			it = Goto.find(mp(ind, Temp.first));

			if (it == Goto.end()) {

				cout << "Given input \"" << input << "\" can not be parsed using given grammar\n";

				return;

			}

			printf("Reduce\n");

			stck.push(mp("", it->second));

		}

		else {

			printf("Accepted\n");

			cout << "Given input \"" << input << "\" can be parsed using given grammar\n";

			return;

		}

	}

	assert(0);

}



void print()

{

	int i, j, k, ind;

	map <string, int> :: iterator it;

	for (i = 0; i < nonTerminalOrder.size(); ++i) {



		it = idNonTerminal.find(nonTerminalOrder[i]);

		assert(it != idNonTerminal.end());

		ind = it->second;



		for (j = 0; j < productions[ind].size(); ++j) {

			cout << nonTerminalOrder[i] << " -> ";

			for (k = 0; k < productions[ind][j].size(); ++k)

				cout << productions[ind][j][k] << " ";

			cout << endl;

		}

	}

	cout << "-----------------\n";

}



void printFirst()

{

	int i, j, ind;

	map <string, int> :: iterator it;

	set <string> :: iterator it2;

	for (i = 0; i < nonTerminalOrder.size(); ++i) {

		it = idNonTerminal.find(nonTerminalOrder[i]);

		assert(it != idNonTerminal.end());

		ind = it->second;

		cout << "First of " << nonTerminalOrder[i] << " : ";

		for (it2 = first[ind].begin(); it2 != first[ind].end(); ++it2)

			cout << *it2 << " ";

		cout << endl;

	}

	cout << "-----------------\n";

}



void printFollow()

{

	int i, j, ind;

	map <string, int> :: iterator it;

	set <string> :: iterator it2;

	for (i = 0; i < nonTerminalOrder.size(); ++i) {

		it = idNonTerminal.find(nonTerminalOrder[i]);

		assert(it != idNonTerminal.end());

		ind = it->second;

		cout << "Follow of " << nonTerminalOrder[i] << " : ";

		for (it2 = follow[ind].begin(); it2 != follow[ind].end(); ++it2)

			cout << *it2 << " ";

		cout << endl;

	}

	cout << "-----------------\n";

}



void printCLR1States()

{

	int i, j;

	set < Structure > :: iterator it;
	set <string> :: iterator it1;

	for (i = 0; i < id; ++i) {

		cout << "Table of " << i << " : \n";

		for (it = CLR1Table[i].begin(); it != CLR1Table[i].end(); ++it) {

			cout << (*it).first.first << " : ";

			for (j = 0; j < (*it).first.second.size(); ++j)

				cout << (*it).first.second[j] << " ";

			cout << " LookAheads : ";

			for (it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1)

				cout << *it1 << " ";

			cout << endl;

		}

		cout << "*****************\n";

	}

}



void printActionAndGotoTable()

{

	map < pair <int, string>, int> :: iterator it;

	map < pair <int, string>, pair <string, int> > :: iterator it1;



	printf("GOTO Table : \n");

	for (it = Goto.begin(); it != Goto.end(); ++it)

		cout << it->first.first << " " << it->first.second << " : " << it->second << endl;

	cout << "-----------------\n";

	printf("ACTION Table : \n");

	for (it1 = Action.begin(); it1 != Action.end(); ++it1) {

		cout << it1->first.first << " " << it1->first.second << " : ";

		if (it1->second.first == "Accept")

			cout << "Accept State\n";

		else



			cout << it1->second.first << " " << it1->second.second << endl;

	}

	cout << "-----------------\n";

}
void checkerror() {
	int aishwarya = 0;
	cin >> aishwarya;

if(aishwarya == 1) {

			cout << "Missing operand";

		} else if(aishwarya == 2) {

			cout << "Missing operator";

		} else if(aishwarya == 3) {

			cout << "Unbalanced left parenthesis";

		} else if(aishwarya == 4) {

			cout << "Unbalanced right parenthesis";

		}
}


int main()

{

	getInput("INP1.txt");

	remove_left_recursion();

	find_first();

	find_follow();

	print();

	printFirst();

	printFollow();

	createCLR1ParsingTable();

	printCLR1States();

	provideEachProductionID();

	createActionAndGotoTable();

	printActionAndGotoTable();

	checkForGivenInput("INP2.txt");
	checkerror();
	return 0;

}
