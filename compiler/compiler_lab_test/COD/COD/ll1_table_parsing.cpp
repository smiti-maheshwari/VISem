/*---------------------------QUES 6-------------------------------*/
#include <bits/stdc++.h>
using namespace std;
map <char, vector<string> > mp;
vector <char> vec;
int terminal_record[256] = {0};
//print the grammar


map< char, vector< string > > grammar;

map < char , set < char > > first;
map < char , set < char > > follow;

int symbols;
char startSym[100];
map <int, char> ter_i_map;
set <char> terminals;
map <char, int> ter_map;
map <char, int> non_ter_map;
map <string, set <char> > prod_first;

string parsing_table[100][100];

int find_all(int arr[], string str, char sym) {
    int c = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == sym) {
            arr[c++] = i;
        }
    }

    return c;
}
/*
inline void print(char ch) {
    vector<string > temp = grammar[ch];
    for(int i = 0; i < temp.size(); ++i) cout << temp[i] << " "; cout << endl;
} */

bool terminal(char ch) {
    if (ch >= 'A' && ch <= 'Z')
        return false;
    return true;
}

void computeFirst(char sym) {
    vector < string > temp = grammar[sym];

    for (int i = 0; i < temp.size(); i++) {
        string str = temp[i];
        for (int j = 0; j < str.length(); j++) {
            if (terminal(str[j])) {
                first[sym].insert(str[j]);
                prod_first[str].insert(str[j]);
                break;
            } else {
                computeFirst(str[j]);
                set < char > tmp = first[str[j]];
                set < char > :: iterator it;
                it = tmp.begin();
                bool fg = true;
                while (it != tmp.end()) {
                    char ch = *it;
                    if (ch == 'e') {
                        fg = false;
                    } else {
                        first[sym].insert(ch);
                        prod_first[str].insert(ch);
                    }
                    it++;
                }
                if (fg)
                    break;
                if (!fg && j == str.length() - 1) {
                    first[sym].insert('e');
                    prod_first[str].insert('e');
                }
            }
        }
    }
}

void computeFollow(char sym) {
    vector <string > temp;
    int arr[100];
    for(int i = 0; i < symbols; ++i) {
        temp = grammar[startSym[i]];
        for(int j = 0; j < temp.size(); ++j) {
            string str = temp[j];
            int c = find_all(arr, str, sym);
            if (c == 0)
                continue;
            int l = str.length();
            for (int p = 0; p < c; p++) {
                if (arr[p] == l - 1) {
                    if (sym != startSym[i]) {
                        computeFollow(startSym[i]);
                        set<char > followTemp = follow[startSym[i]];
                        set < char > :: iterator it = followTemp.begin();
                        while (it != followTemp.end()) {
                            follow[sym].insert(*it);
                            it++;
                        }
                    }
                } else {
                    int ind = arr[p];
                    while (ind < l) {
                        ind++;
                        set<char > firstTemp;
                        if(terminal(str[ind])) {
                            follow[sym].insert(str[ind]);
                            break;
                        }

                        firstTemp = first[str[ind]];
                        set < char > :: iterator it = firstTemp.begin();
                        bool flag = 0;

                        while(it != firstTemp.end()) {
                            char ch = *it;
                            if(ch == 'e') {
                                flag = 1;
                            } else {
                                follow[sym].insert(ch);
                            }
                            it++;
                        }

                        if (!flag)
                            break;

                        if (ind == l - 1) {
                            if (sym != startSym[i]) {
                                computeFollow(startSym[i]);
                                set<char > followTemp = follow[startSym[i]];
                                set < char > :: iterator it = followTemp.begin();
                                while (it != followTemp.end()) {
                                    follow[sym].insert(*it);
                                    it++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

inline void goFirst() {
    cout << endl << "First : " << endl;
    for (int i = 0; i < symbols; i++) {
        computeFirst(startSym[i]);
        set < char > tmp = first[startSym[i]];
        set < char > :: iterator it;

        cout << startSym[i] << " ---> ";
        it = tmp.begin();
        while (it != tmp.end()) {
            char ch = *it;
            cout << ch << " ";
            it++;
        }
        cout << endl;
    }
}

void goFollow() {
    follow[startSym[0]].insert('$');
    cout << endl;
    cout << "Follow : " << endl;
    for(int i = 0; i < symbols; ++i) {
        computeFollow(startSym[i]);
        set < char > tmp = follow[startSym[i]];
        set < char > :: iterator it;

        cout << startSym[i] << " ---> ";
        it = tmp.begin();
        while (it != tmp.end()) {
            char ch = *it;
            if ((int) ch != 0)
                cout << ch << " ";
            else
                follow[startSym[i]].erase(follow[startSym[i]].find(ch));
            it++;
        }
        cout << endl;
    }
}

void print_parse_table() {
    cout << "Parsing Table :" << endl;
    for (int i = 0; i < symbols; i++) {

        for (int j = 0; j < ter_map.size(); j++) {
            string str = parsing_table[i][j];
            if (str.length() != 0) {
                //cout << str << " ";
                cout << "M[" << startSym[i] << ", " << ter_i_map[j] << "] = " << startSym[i] << " -> " << str << endl;
            }
        }
        //cout << endl;
    }
}

bool create_parsing_table(int c) {
    for (int i = 0; i < symbols; i++) {
        vector <string> temp = grammar[startSym[i]];
        for (int j = 0; j < temp.size(); j++) {
            string str = temp[j];
            set <char> tmp = prod_first[str];
            set <char> :: iterator it;
            it = tmp.begin();
            bool flg = false;
            while (it != tmp.end()) {
                if (*it == 'e') {
                    flg = true;
                } else {
                    string tmps = parsing_table[i][ter_map[*it]];
                   if (tmps.length() != 0) {
                        cout << "Grammer is not LL(1)"<< endl;
                        return false;
                   }
                    parsing_table[i][ter_map[*it]] = str;
                }
                it++;
            }
            if (flg) {
                tmp = follow[startSym[i]];
                set <char> :: iterator it;
                it = tmp.begin();
                while (it != tmp.end()) {
                    char ch = *it;
                    if ((int)ch == 0) {
                        it++;
                        continue;
                    }

                    string tmps = parsing_table[i][ter_map[*it]];

                    if (tmps.length() != 0) {
                        cout << "Grammer is not LL(1)"<< endl;
                        return false;
                    }
                    parsing_table[i][ter_map[*it]] = str;
                    it++;

                }
            }
        }


    }
    cout << endl;
  /* cout << " ";
    map <char, int> :: iterator itr;
    itr = ter_map.begin();
    while (itr != ter_map.end()) {
        cout << itr->first << " ";
        itr++;
    }
    cout << endl; */
    print_parse_table();
    return true;
}

bool parse_input_string(string inputstr) {
    stack <char> stk;
    stk.push('$');
    stk.push(startSym[0]);
    int i = 0;
    while (stk.top() != '$') {
        char ch = stk.top();
        if (terminal(ch)) {
            if (ch == inputstr[i]) {
                stk.pop();
                i++;
                cout << endl;
            } else {
                cout << "Error " << endl;
                return 0;
            }
        } else {
            string temp = parsing_table[non_ter_map[ch]][ter_map[inputstr[i]]];
            if (temp.length() != 0) {
                stk.pop();
                cout << ch << " -> " << temp << endl;
                if (temp[0] != 'e') {
                    for (int j = temp.length() - 1; j >= 0; j--)
                        stk.push(temp[j]);
                }
            } else {
                cout << "Error " << endl;
                return 0;
            }
        }
    }

    return stk.top() == inputstr[i];
}
void prod_first_print() {
    map <string, set < char > > :: iterator it;
    it = prod_first.begin();
    cout << endl;
    while (it != prod_first.end()) {
        cout << it->first << " ---> ";
        set <char> temp = it->second;
        set <char> :: iterator i;
        i = temp.begin();
        while (i != temp.end()) {
            cout << *i << " ";
            i++;
        }
        it++;
        cout << endl;
    }
}



void print()
{
    cout << endl << "Grammer after left recursion removal : " << endl;
    int c = 0;
	for(int i = 0; i < vec.size(); i++) {
		if(mp[vec[i]].size() == 0) continue;
		cout << vec[i] << "->";
        startSym[c] = vec[i];
		non_ter_map[startSym[c]] = c;

		for(int j = 0 ; j < mp[vec[i]].size(); j++) {
			cout << mp[vec[i]][j];
			grammar[startSym[c]].push_back(mp[vec[i]][j]);
			if(j != mp[vec[i]].size() - 1) cout << "|";

			string str = mp[vec[i]][j];
			for (int l = 0; l < str.length(); l++) {
                if (str[l] != 'e' && terminal(str[l])) {
                    terminals.insert(str[l]);
                }
            }

		}
		cout << endl;
		c++;
	}
	symbols = c;
}

//check for indirect left recur
int check(char a, int i)
{
	for(int j = 0; j <= i; j++) {
		if(vec[j] == a) return j;
	}
	return -1;
}

bool removeduplicate(vector <string> v, string str)
{
	for(int i = 0; i < v.size(); i++)
		//if already present production
		if(v[i] == str) return true;
	return false;
}
//remove indirect recur
void remove_indirect()
{
	//start from index 2nd production
	for(int i = 1; i < vec.size(); i++) {
		//check if any indirect left recur in A-> Sd|e
		for(int j = 0; j < mp[vec[i]].size(); j++) {
			string as_str = mp[vec[i]][j];
			//cout << as_str << endl;
			if(check(as_str[0], i - 1) >= 0) {
				int index = check(as_str[0], i - 1);
				//cout << index << endl;
				//cout << mp[vec[index]].size() << endl;
				for(int k = 0; k < mp[vec[index]].size(); k++) {
					string new_str = mp[vec[index]][k];
					for(int l = 1; l < mp[vec[i]][j].length(); l++) {
						string temp = mp[vec[i]][j];
						new_str += temp[l];
						//new production added to the non-terminals
						if(!removeduplicate(mp[vec[i]], new_str)) mp[vec[i]].push_back(new_str);
					}
				}
				mp[vec[i]].erase(mp[vec[i]].begin() + j);

			}

		}
	}
}

//check for immediate left recur
bool check2(char a, int i)
{
	return (vec[i] == a) ? true : false;
}


//remove immediate recur
void remove_imme()
{
    	int sz = vec.size();
	for(int i = 0; i < sz; i++) {
		int po;
		for(po = 65; po <= 90; po++) {
			if(terminal_record[po] == 0) {
				break;
			}
		}

		int y = mp[vec[i]].size();
		int flag = 0;
		for(int j = 0; j < y; j++) if(check2(mp[vec[i]][j][0], i)) {vec.push_back((char)po);flag = 1;break;}
		if(!flag) continue;
		int op = 1;
		vector<string> tpm;
        	for(int j = 0; j < y; j++) {
			string as_str = mp[vec[i]][j];
			//cout << as_str << endl;
			if(check2(as_str[0], i)) {
				terminal_record[po] = 1;
				string temp = "";
				for(int k = 1; k < as_str.length(); k++) {
					temp += as_str[k];
				}
				temp += po;
				mp[(char)po].push_back(temp);
				//cout << mp[vec[i]].size() << endl;
			} else {
				terminal_record[po] = 1;
				string tmp = as_str;
				if(tmp == "e") tmp = po;
				else tmp += po;
				//cout << tmp << endl;
				tpm.push_back(tmp);
			}
		}
		mp[vec[i]].clear();
		mp[vec[i]] = tpm;
		mp[(char)po].push_back("e");
	}
}


int main()
{
	int no_pro;
	char non_term;
	string str;
	cin >> symbols;

	for(int i = 0; i < symbols; i++) {
		cin >> non_term >> no_pro;
		vec.push_back(non_term);
		terminal_record[non_term] = 1;
		while(no_pro--) {
			cin >> str;
			mp[non_term].push_back(str);

            for (int l = 0; l < str.length(); l++) {
                if (str[l] != 'e' && terminal(str[l])) {
                    terminals.insert(str[l]);
                }
            }
		}
	}

	//print();
	//cout << endl;
	remove_indirect();
	remove_imme();
	print();
    goFirst();
    goFollow();
    int c = 0;

    terminals.insert('$');
    set <char> :: iterator it;
    it = terminals.begin();
    while (it != terminals.end()) {
        ter_i_map[c] = *it;
        ter_map[*it] = c++;
       // cout << *it << " ";
        it++;
    }

    map <char, int> :: iterator itr;
    itr = ter_map.begin();
    /*while (itr != ter_map.end()) {
        cout << itr->first << " " << itr->second << endl;
        itr++;
    }*/

    //prod_first_print();
    if (create_parsing_table(c)) {
        cout << "Enter string you want to check " << endl;
        string inputstring;
        cin >> inputstring;
        cout << endl;
        inputstring = inputstring + "$";
        if (parse_input_string(inputstring))
            cout <<  "String Accepted "<< endl;
        else
            cout <<  "String Not Accepted "<< endl;
    }
    return 0;
}






/*
2
S 3
aA b cS
A 2
Sd e

1
Q 2
q QED

3
E 2
E+T T
T 2
T*F
F
F 2
(E) id

3
E 2
E+T T
T 2
T*F F
F 2
(E) i
E->TA
T->FB
F->(E)|i
A->+TA|e
B->*FB|e

1
S 2
SOSIS
OI
S->OIA
A->OSISA|e

1
E 2
E+T T
E->TA
A->+TA|e


2
S 2
Aa b
A 4
Ac Aad bd e
S->Aa|b
A->bdB|B
B->cB|adB|e


*/
