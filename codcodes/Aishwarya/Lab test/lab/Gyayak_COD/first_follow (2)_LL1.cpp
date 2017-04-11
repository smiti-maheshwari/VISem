#include<bits/stdc++.h>  
#include<string.h>  
#define SIZE 10 

using namespace std;

int num;  
string production[10];  
map<char, set<char> > first, follow;
map<char, int> non_terminal;
map<char, int> index_no;
set<char> term, non_term;
map<char, map<char, string> > table;

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
 
int main () {  
	
	
	
	/* starting of the string following "->" */  
	printf("Enter Number of Production : ");  
	scanf("%d",&num); 
	printf("Enter the grammar as E->E-A :\n");  
	for(int i=0;i<num;i++){
		cin >> production[i];
		char rt = production[i][0];
		non_term.insert(rt);
		non_terminal[rt] = 1;
		index_no[rt] = i; 
	}
	for(int i=0;i<num;i++){
		for (int l = 0; l < production[i].length(); l++) {
			if (non_terminal[production[i][l]] != 1 && production[i][l] != '|') {
				term.insert(production[i][l]);
			}
		}
	}
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
