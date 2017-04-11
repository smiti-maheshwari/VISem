#include <bits/stdc++.h>

using namespace std;

int main()
{
	int prod, i, ind, j;
	string g[100], str;
	char T, NT;
	string a, b;
	vector <string> alpha;
	vector <string> beta;
	
	cin>>prod;
	
	for (i = 0; i < prod; i++) cin>>g[i];
			
	
	cout<<endl;
	
	for (i = 0; i < prod; i++) {
		str = g[i];
		NT = str[0];
		ind = 3;
		a = "";
		b = "";
		j = ind;
		alpha.clear();
		beta.clear();
		bool flag = false;
		
		if (str[0] == str[3]) flag = true;
		
		if (flag == false) cout<<str<<endl;
		else {
		
			while (j < str.length()) {
				a = "";
				ind = j;
				if (NT == str[ind]) {
					ind++;
					while (str[ind] != 0 && str[ind] != '|') a += str[ind++];
					ind++;
				 	alpha.push_back(a);
					j = ind;
				} else j++;
			}
			
			ind = 3;
			j = ind;
			
			while (j < str.length()) {
				b = "";
				if (NT == str[ind]) {
					while (str[ind] != 0 && str[ind] != '|') ind++;
					ind++;
				}
			
				if (NT != str[ind] && flag == true && ind < str.size()) {
					while (str[ind] != 0 && str[ind] != '|') b += str[ind++];
					ind++;
					//cout<<b<<endl;
					beta.push_back(b);
					j = ind;
				} else j++;
			}
			
			if (flag == false) cout<<str<<endl;
			else {
				if (beta.size() == 0) cout<<"Error in Grammar"<<endl;
				else {
					cout<<NT<<"->";
					for (j = 0; j < beta.size(); j++) {
						if (j > 0) cout<<"|";
						cout<<beta[j]<<NT<<"'";
					}
					
					cout<<endl;
					
					cout<<NT<<"'->";
					for (j = 0; j < alpha.size(); j++) {
						if (j > 0) cout<<"|";
						cout<<alpha[j]<<NT<<"'";
					}
					cout<<"|ep";
				}
			}
			cout<<endl;
		}
	}
	
	return 0;
}

/* Test Cases
3
E->E+T|T
T->T*F|F
F->(E)|id

3
E->E+T|E+Z|T
T->T*F|F
F->(E)|id

3
E->E+T|T
T->T*F|F
F->F*S|id

3
E->E+T|T|a|b|c
T->T*F|F
F->(E)|id

3
E->P|E+T|L|E/T
F->L|F*X|lg|F*S
X->id|S|re
*/
