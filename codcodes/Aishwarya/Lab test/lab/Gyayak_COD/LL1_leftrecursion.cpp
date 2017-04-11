#include <bits/stdc++.h>

using namespace std;

int num;
string production[10];
map<char, set<char> > first, follow;
map<char, int> non_terminal;
map<char, int> index_no;
set<char> term, non_term;
map<char, map<char, string> > table;
map<char, vector <string> > prodction;
map<char, set<string> > prod3;
set <char> nonterm;
int ctr = 0;

void checkcon(char noterm2, char naterm2) {
    set <string> vv = prod3[noterm2];
    int flag = 0;
    set <string> v2 = prod3[naterm2];
    set <string> toberemoved;
    //we iterate over the entire set of noterm2 to check if naterm2 is first term in any of its productions
    for(set <string>::iterator it = vv.begin(); it!= vv.end(); it++) {
        cout << "*it = " << *it << endl;
        string s= *it;
        //if naterm2 is the first term of one of noterm2's production
        if(s[0] == naterm2) {
            //the production containing naterm2 as its first term should be removed from production of noterm2 so it is added to toberemoved set
            toberemoved.insert(s);
            string tobeadded = "";
            //copy the entire production in string tobeadded ignoring the first term that is naterm2
            for(int k = 1; k < s.length(); k++) {
                tobeadded = tobeadded + s[k];
            }
            //the string tobeadded+all the productions of naterm2 is added to the production of noterm2
            for(set<string>::iterator it2 = v2.begin(); it2 != v2.end(); it2++) {
                prod3[noterm2].insert(*it2+tobeadded);
            }
        }
    }
    //remove the productions needed to be removed;
    for(set <string>::iterator it = toberemoved.begin();it!= toberemoved.end();it++) {
        set<string>::iterator it2 = prod3[noterm2].find(*it);

        prod3[noterm2].erase(it2);
    }
}
void directleftrecursion(char terminal) {
    set <string> vec = prod3[terminal]; //all the productions of nonterminal copied in vec
    set <string> alpha,beta;//A->Aalpha|beta----------> A->betaQ,Q->alphaQ|#
    set <string> toberemoved;//the productions to be replaced for ex A->Aalpha and A->beta;
    int flag = 0;
    for(set<string>::iterator i = vec.begin();i != vec.end(); i++) {
        string ss = *i;
        //if the first nonterminal of the production is equal to the nonterminal in the left side i.e production is of form A->A.alpha
        if(ss[0] == terminal) {

            cout << "erasing production " << terminal << "->" << *i <<endl;
                flag++;//to keep a track of the presence of left recusion in the production
            string t = "";
            for(int j = 1; j < ss.length(); j++) {
                t = t+ss[j];
            }
            //cout << "t = " << t << endl;
            toberemoved.insert(ss);//inserting the production to be removed from the nonterminal
            alpha.insert(t);//finding all the alphas
        }
        else {
            toberemoved.insert(ss);//finding all the betas;
            beta.insert(ss);
        }
    }
    if(beta.size() == 0) {
        beta.insert("#");// if beta is empty then we insert epsilon i.e. # in beta
    }
    //if flag == 0. that means no left recursion present in the production
    if(flag == 0) {
        cout << "nodirect left recursion is present in the terminal " << terminal << endl;
        prod3[terminal] = beta;//if no left recursion present then no need to remove anything..we just insert enitre beta set in the production of the expression
    }
    else {//if left recursion is present then
        char addedterm;
        cout << "THE TERMINAL " <<terminal << " HAS LEFT RECURSION ...SO ENTER THE VARIABLE TO BE USED\n";
        //Q= addedterm
        cin >> addedterm;//we take the input of the nonterminal to be added
        if(nonterm.find(addedterm)== nonterm.end()) {//check if nonterminal is already present in the list of non terminals

           for(set <string> ::iterator it = beta.begin();it!= beta.end();it++) {
                if(beta.size() == 1 && *it == "#") {
                    //if size of beta set is only 1 and epsilon is that single element then we add A->Q to production of A
                    string s2 = "";
                    s2 = s2+addedterm;
                    prod3[terminal].insert(s2);
                    break;
                }
                //else we add the beta*addedterm to the production of A
                string ss = *it+addedterm;

                prod3[terminal].insert(ss);
           }
           //removing the productions needed to be removed from A
           for(set <string>::iterator it = toberemoved.begin(); it!= toberemoved.end(); it++) {
                set <string>::iterator it2= prod3[terminal].find(*it);
                prod3[terminal].erase(it2);
           }
           //for Q..Q->alphaQ|epsilon
           for(set <string>::iterator it = alpha.begin(); it!= alpha.end(); it++) {
                string newv = *it+addedterm;
                prod3[addedterm].insert(newv);
           }
           prod3[addedterm].insert("#");
        }
    }
}

void removeepsilonproduction() {
    set<char> containsepsilon;
    for(set<char>::iterator it= nonterm.begin(); it!= nonterm.end(); it++) {

        for(set<string>::iterator it2 = prod3[*it].begin(); it2!=prod3[*it].end(); it2++) {
            string ss = *it2;
            if(ss[0] == '#') {
                containsepsilon.insert(*it);
                break;
            }
        }
    }
    for(set<char>::iterator it = nonterm.begin(); it!= nonterm.end(); it++) {
        for(set<string>::iterator it2 = prod3[*it].begin();it2!= prod3[*it].end(); it2++) {
            string ss = *it2;
            string tmp = ss;
            while(true) {
                if(containsepsilon.find(tmp[0])!= containsepsilon.end()) {

                    string rsv="";
                    for(int i =1; i <tmp.length(); i++) {
                        rsv = rsv+tmp[i];
                    }
                    cout << "inserting productions " << *it << " - > " << rsv << endl;
                    prod3[*it].insert(rsv);
                    tmp = rsv;
                }
                else {
                    break;
                }
            }
        }
    }
    for(set<char>::iterator it = containsepsilon.begin();it!= containsepsilon.end();it++) {
        set<string> ::iterator it2 = prod3[*it].find("#");
        prod3[*it].erase(it2);
    }
}

void removecycle() {
    cout << "AARAHA HU ME\n";
    map <char, set<char> > trackcycle;

    for(set<char>::iterator i = nonterm.begin(); i != nonterm.end(); i++) {
        for(set<string>::iterator it = prod3[*i].begin(); it!= prod3[*i].end(); it++) {
            string ss = *it;
            if(ss.length()==1) {
                if(nonterm.find(ss[0])!= nonterm.end()) {
                    trackcycle[*i].insert(ss[0]);
                }
            }
        }
    }

    for(set<char>::iterator it = nonterm.begin(); it!= nonterm.end(); it++) {
        cout << "YAHA BHI AA RHA HU ME\n";
        set<char> temp = trackcycle[*it];
        for(set<char>::iterator it2 = temp.begin(); it2 != temp.end(); it2++) {
            char w = *it2;
            set<char> temp2 = trackcycle[w];
            for(set<char>::iterator it3 = temp2.begin(); it3 != temp2.end(); it3++) {
                if(*it3 == *it) {
                    string qw = "";
                    qw = qw+w;
                    set<string>::iterator tr = prod3[*it].find(qw);

                    string qww="";
                    qww=qww+(*it);
                    set<string>::iterator tr2 = prod3[*it2].find(qww);

                    if(tr2!= prod3[*it2].end() && tr!= prod3[*it].end()) {
                        cout << "REMOVING CYCLES IN \n";
                        cout << *it <<" -> " << qw << " and " << *it2 << " -> " << qww << endl;
                        prod3[*it2].erase(tr2);
                        prod3[*it].erase(tr);
                    }
                    //flag = 1;
                    break;
                }
            }
        }
    }
}

void find_first(char ch);

void printspaces(int x) {
	for (int i = 0; i < 9-x; i++) {
		cout << " ";
	}
	return;
}
void find_follow(char ch) {
	//cout << "follow called " << ch << endl;
	for (int i = 0; i < num; i++) {
		int j;
		for (j = 3; j < production[i].length(); j++) {
			if (production[i][j] == ch) {
				//cout << "i = " << i << " j = " << j << endl;
				int k = j+1;
				int flag = 1;
				if (k == production[i].length() || production[i][k] == '|') {
					//cout << "LL";
					if (production[i][k-1] == production[i][0]) {
						continue;
					}
					else {
						//cout << "follow called from 1" << endl;
						find_follow(production[i][0]);
						for (set<char>::iterator it=follow[production[i][0]].begin();it!=follow[production[i][0]].end(); it++)
						{
							follow[ch].insert(*it);
						}
						continue;
					}
				}
				while(k < production[i].length() && flag == 1 && production[i][k] != '|') {
					find_first(production[i][k]);
					set<char> xyz = first[production[i][k]];
					flag = 0;
					for (set<char>::iterator it= xyz.begin(); it!= xyz.end(); it++) {
						//follow[ch].insert(*it);
						if (*it == '#')
							flag = 1;
						else
							follow[ch].insert(*it);
					}
					k++;
				}
				if (flag == 1) {
					if (production[i][k-1] == production[i][0]) {
						continue;
					}
					//cout << "follow called from here 2" << endl;
					find_follow(production[i][0]);
					for (set<char>::iterator it=follow[production[i][0]].begin();it!=follow[production[i][0]].end(); it++)
					{
						follow[ch].insert(*it);
					}
				}
			}
		}
	}
}

void find_first(char ch) {
	set<char> ret;
	if (non_terminal[ch] != 1) {
		first[ch].insert(ch);
		return;
	}
	int gr_no = index_no[ch];
	int k = 3;
	int flag = 1;
	int counter = 0;
	while(flag == 1 && k < production[gr_no].length() && production[gr_no][k] != '|') {
        find_first(production[gr_no][k]);
        flag = 0;
        for (set<char>::iterator it = first[production[gr_no][k]].begin(); it != first[production[gr_no][k]].end(); ++it) {
            if (*it == '#') {
                flag = 1;
                if (k+1 >= production[gr_no].length() || production[gr_no][k+1] == '|') {
                     first[ch].insert(*it);
                }
            }
            else {
                    first[ch].insert(*it);
            }
        }
        k++;
	}
	for (int i = 4; i < production[gr_no].length(); i++) {
		if (production[gr_no][i] == '|') {
            k = i+1;
            flag = 1;
            while(flag == 1 && k < production[gr_no].length() && production[gr_no][k] != '|') {
                find_first(production[gr_no][k]);
                flag = 0;
                for (set<char>::iterator it = first[production[gr_no][k]].begin(); it != first[production[gr_no][k]].end(); ++it) {
                    if (*it == '#') {
                        flag = 1;
                        if (k+1 >= production[gr_no].length() || production[gr_no][k+1] == '|') {
                            first[ch].insert(*it);
                        }
                    }
                    else {
                            first[ch].insert(*it);
                    }
                }
                k++;
            }
		}
	}
	return;
}
void parser_table() {
	for (int i = 0; i < num; i++) {
		int k = 3;
		while(1) {
			if (k >= production[i].length())
				break;
			string str = "";
			str += production[i][0];
			str += "->";
			int x = k;
			int flag = 1;
			set<char> to_insert;
			while(1) {
				str += production[i][x];
				if (flag == 1) {

				flag = 0;
		                for (set<char>::iterator it = first[production[i][x]].begin(); it != first[production[i][x]].end();it++) {
					if (*it == '#') {
						flag = 1;
					}
					else {
						to_insert.insert(*it);
					}
				}
				}
				x++;
				if (production[i][x] == '|' || x == production[i].length()) {
					k = x+1;
					break;
				}
			}
			if (flag == 1) {
				for (set<char>::iterator it = follow[production[i][0]].begin(); it != follow[production[i][0]].end();it++){
					to_insert.insert(*it);
				}
			}
			for (set<char>::iterator it = to_insert.begin(); it != to_insert.end();it++) {
				table[production[i][0]][*it] = str;
				cout << "Table " << production[i][0] << " " << *it << " =>  " << str << endl;
			}
		}
	}
}

int main() {
    //freopen("/home/nalin/Desktop/code/input.txt","r", stdin);
    int n;
    cout << "Enter the total number of productions in your grammar\n";
    cin >> n;
    string s;
    char c;
    for(int i =0; i < n; i++) {
        cin >> c;
       // cout << "->";
        cin >> s;
        prodction[c].push_back(s);
        nonterm.insert(c);
    }
    set <char>::iterator it,it2,it3;
    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        vector <string> ss = prodction[*it];
        for(int j = 0; j < ss.size(); j++) {
            string q = ss[j];
            for(int k = 0; k < q.length(); k++) {
                if(nonterm.find(q[k])!= nonterm.end()) {
                    continue;
                }
                term.insert(q[k]);
            }
        }
    }


    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        for(int i =0; i <prodction[*it].size(); i++) {
                prod3[*it].insert(prodction[*it][i]);
        }
    }
   // removecycle();
    //removeepsilonproduction();
    //removecycle();
    //remove indirect left recursion --->

    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        if(it == nonterm.begin()) {
            directleftrecursion(*it);
            continue;
        }
        for(it2 = nonterm.begin(); it2 != it; it2++) {
                //cout << "present nonterminal = " << *it << " in " << *it2 << endl;
                checkcon(*it,*it2);
        }
        directleftrecursion(*it);
    }

    // end of removing indirect left recursion -->
    int kl = 0;
    for(map<char, set<string> >::iterator it = prod3.begin(); it != prod3.end(); it++) {
        //cout << "NALIN\n";
        string str = "";
        set<string> ss = it->second;
        cout << it->first << "->";
        str += it->first;
        str += "->";
        for(set <string>::iterator it =ss.begin(); it != ss.end(); ) {
            cout << *it << " | ";
            str += *it;
            it++;
            if (it != ss.end())
                str += "|";
        }
        cout << endl;
        production[kl] = str;
        char rt = production[kl][0];
		non_term.insert(rt);
		non_terminal[rt] = 1;
		index_no[rt] = kl;
        kl++;
    }
    num = nonterm.size();
    cout << "-----------------------Recieved production---------------------------" << endl;
    for (int i = 0; i < num; i++) {
        cout << production[i] << endl;
    }
    cout << endl << "---------------------------Printing First-----------------------------" << endl;
    for (int i = 0; i < num; i++) {
		char rt = production[i][0];
		find_first(rt);
		cout << "First of " << rt << " = {";
		for (set<char>::iterator it = first[rt].begin(); it != first[rt].end(); ) {
			cout << *it;
			it++;
			if (it != first[rt].end()) {
				cout << ", ";
			}
		}
		cout << "}" << endl;
	}
	follow[production[0][0]].insert('$');

	for (int i = 0; i < num; i++) {
		char rt = production[i][0];
		find_follow(rt);
		cout << "Follow of " << rt << " = {";
		for (set<char>::iterator it = follow[rt].begin(); it != follow[rt].end();) {
			cout << *it;
			it++;
			if (it != follow[rt].end()) {
				cout << ", ";
			}
		}
		cout << "}" << endl;
	}
	parser_table();
	term.insert('$');
	cout << "     ";
	for (set<char>::iterator it = term.begin(); it != term.end(); it++) {
		cout << "    ";
		cout << *it;
		printspaces(4);
		//cout << "|";
	}
	cout << endl;
	for (set<char>::iterator it = non_term.begin(); it != non_term.end(); it++) {
		cout << *it << " | ";
		for (set<char>::iterator it2 = term.begin(); it2 != term.end(); it2++)	{
			cout << table[*it][*it2];
			printspaces(table[*it][*it2].length());
			cout << "|";
		}
		cout << endl;
	}
    return 0;
}
