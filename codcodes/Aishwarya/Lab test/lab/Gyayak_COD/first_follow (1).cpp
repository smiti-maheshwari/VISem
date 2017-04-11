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
					cout << "LL";
					if (production[i][k-1] == production[i][0]) {
						continue;
					}
					else {
						cout << "follow called from 1" << endl;
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
					cout << "follow called from here 2" << endl;
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
	//cout << "find_first called " << ch << endl;
	set<char> ret;
	if (non_terminal[ch] != 1) {
		first[ch].insert(ch);
		return;
	}
	int gr_no = index_no[ch];
	find_first(production[gr_no][3]);
	for (set<char>::iterator it = first[production[gr_no][3] ].begin(); it != first[production[gr_no][3] ].end(); it++) {
		first[ch].insert(*it);
	}
	for (int i = 4; i < production[gr_no].length(); i++) {
		if (production[gr_no][i] == '|') {
			find_first(production[gr_no][i+1]);
			for (set<char>::iterator it = first[production[gr_no][i+1] ].begin(); it != first[production[gr_no][i+1] ].end(); it++) {
				first[ch].insert(*it);
			}
		}	
	}
	return;
}
 
int main () {  
	
	
	
	/* starting of the string following "->" */  
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
		cout << "First of " << rt << " = {";
		for (set<char>::iterator it = first[rt].begin(); it != first[rt].end(); it++) {
			cout << *it << ", ";
		}
		cout << "}" << endl;
	}
	cout << "Mohtit";
	follow[production[0][0]].insert('$');
	
//int k;
//find_first(')');
//set<char> xyz = first[')'];
//cout << "first of ) = ";
//for (set<char>::iterator it= xyz.begin(); it!= xyz.end(); it++) {
//	cout << *it << " ";					
						
//}
//cout << endl;

	//find_follow(production[0][0]);
	for (int i = 0; i < num; i++) {	
		char rt = production[i][0];
cout << "Gyayak";
		find_follow(rt);
		cout << "Follow of " << rt << " = {";
		for (set<char>::iterator it = follow[rt].begin(); it != follow[rt].end(); it++) {
			cout << *it << ", ";
		}
		cout << "}" << endl;
	}

	
	return 0;
}   
