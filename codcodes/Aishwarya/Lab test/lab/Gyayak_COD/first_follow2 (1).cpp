#include<bits/stdc++.h>
#include<string.h>
#define SIZE 10

using namespace std;

int num;
string production[10];
map<char, set<char> > first, follow;
map<char, int> non_terminal;
map<char, int> index_no;

void find_first(char ch);

void print_first(char ch) {
    cout << " { ";
    for (set<char>::iterator it = first[ch].begin(); it != first[ch].end(); it++) {
        cout << *it << ", ";
    }
    cout << " } " << endl;
    return;
}

void find_follow(char ch) {
	for (int i = 0; i < num; i++) {
		int j;
		for (j = 3; j < production[i].length(); j++) {
			if (production[i][j] == ch) {
				int k = j+1;
				int flag = 1;
				if (k == production[i].length() || production[i][k] == '|') {
					if (production[i][k-1] == production[i][0]) {
						continue;
					}
					else {
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

int main () {
	printf("Enter Number of Production : ");
	scanf("%d",&num);
	printf("Enter the grammar as E->E-A :\n");
	for(int i=0;i<num;i++){
		cin >> production[i];
		char rt = production[i][0];
		non_terminal[rt] = 1;
		index_no[rt] = i;
	}
	for (int i = 0; i < num; i++) {
		char rt = production[i][0];
		find_first(rt);
		cout << "First of " << rt << " = { ";
		for (set<char>::iterator it = first[rt].begin(); it != first[rt].end();) {
			cout << *it;
			it++;
			if (it != first[rt].end()) {
                cout << ", ";
			}
		}
		cout << " }" << endl;
	}
	cout << endl;
	follow[production[0][0]].insert('$');
	for (int i = 0; i < num; i++) {
		char rt = production[i][0];
		find_follow(rt);
		cout << "Follow of " << rt << " = { ";
		for (set<char>::iterator it = follow[rt].begin(); it != follow[rt].end();) {
			cout << *it;
			it++;
			if (it != follow[rt].end()) {
                cout << ", ";
			}
		}
		cout << " }" << endl;
	}
	return 0;
}
