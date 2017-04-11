#include<bits/stdc++.h>  
#include<string.h>  
#define SIZE 10 

using namespace std; 

string production[20];
map<string, set<char> > mp_set;
string str;

set<char> cyk(int start, int finish) {
	if (start == finish) {
		string zx = "";
		zx += str[start-1];
		return mp_set[zx];	
	}
	set<char> ret;
	for (int k = 0; k < finish-start; k++) {
		set<char> set1 = cyk(start, start+k);
		set<char> set2 = cyk(start+k+1, finish);
		
		for (set<char>::iterator it = set1.begin(); it != set1.end(); it++) {
			for (set<char>::iterator it2 = set2.begin(); it2 != set2.end(); it2++) {
				string fg = "";				
				fg += *it;
				fg += *it2;
				
				set<char> xy = mp_set[fg];
				for (set<char>::iterator it3 = xy.begin(); it3 != xy.end(); it3++) {
					ret.insert(*it3);
				}
			}
		}	
	}
	return ret;
}
int main () {  
	int num;
	printf("Enter Number of Production : ");  
	scanf("%d",&num); 
	printf("Enter the grammar as E->E-A :\n");  
	for(int i=0;i<num;i++){
		cin >> production[i];
		string new_str;
		for (int j = 3; j < production[i].length(); j++) {
			if (production[i][j] == '|') {
				mp_set[new_str].insert(production[i][0]);				
				new_str = "";
				continue;
			}
			new_str += production[i][j];
		}
		mp_set[new_str].insert(production[i][0]);
	}
	int q;
	cin >> q;

	for (int i = 0; i < q; i++) {
		
		cin >> str;	
		
		set<char> ans = cyk(1, str.length());
		for (set<char>::iterator it = ans.begin(); it != ans.end(); it++) {
			cout << *it << " "; 		
		}
		cout << " => ";
		if (ans.find('S') != ans.end()) {
			cout << "YES" << endl;		
		}
		else{
			cout << "NO" << endl;
		}
	}
	/*for (map<string, set<char> >::iterator it = mp_set.begin(); it != mp_set.end(); it++) {
		set<char> myset = it->second;
		cout << it->first << " ----> ";
		for (set<char>::iterator it2 = myset.begin(); it2 != myset.end(); it2++) {
			cout << *it2 << ", ";
		}
		cout << endl;
	}*/
	
	//cyk(0, )
	return 0;
 }   
