#include<iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include<bits/stdc++.h>
using namespace std;
string prod[100][100];
//epsilon is represented by #
map<string,pair<int,int> > mp;
int k=0;
void find_all_prod(string s) {
	int i=0,j,z;
	string st="";

  	while(1) {
	
		if(i+1 < s.size() && s[i]=='-' && s[i+1]=='>')
		break;
		st += s[i];
		i++;
	}

	int p = k;
	i=i+2;
	int f = 0;
	for(j=0;j<k;j++) {
		if(prod[j][0].compare(st)==0) {
			f = 1;
			p = j;
			break;
		}
	}
	if(f == 0) {

		prod[p][0]=st;
		z = 1;
		mp[st].first = k;
		k++;
	} else {
		z = mp[st].second;
	}
	string st1 = "";
	while(i<s.size()) {
		if(s[i]=='|') {
			prod[p][z] = st1;
			z++;
			st1="";
		}
		else 
		st1 += s[i];
		i++;
	}
	prod[p][z] = st1;
	mp[st].second = z+1;	
}

int main() {
	freopen("tab6.txt","r",stdin);

	int prod_rule,i,j;
	int no_of_variables;
	printf("no of non-terminal symbol = ");
	cin >> no_of_variables;
	cout << no_of_variables << endl;
	string var[100];
	for(i=0;i<no_of_variables;i++)
	cin >> var[i];

	int no_of_terminal;
	printf("no of terminal symbol = ");
	cin >> no_of_terminal;
	cout << no_of_terminal << endl;
	string ter[100];
	for(i=0;i<no_of_terminal;i++)
	cin >> ter[i];

	printf("no of production rules = ");
	cin >> prod_rule;
	cout << prod_rule << endl;
	string s;
	for(i=0;i<prod_rule;i++) {
		cin >> s;
		find_all_prod(s);
	}

	for(i=0;i<no_of_variables;i++) {
		if(mp.find(var[i])==mp.end())
		continue;
		int q,p = mp[var[i]].first;
		vector<string> match,unmatch;

		for(j=0;j<=i-1;j++) {

			int p1 = mp[var[j]].first;
			vector<string> temp;
			for(int l=1;l<mp[var[i]].second;l++) {

				for(q=0;q<var[j].size();q++) {
					if(var[j][q]!=prod[p][l][q])
					break;
				}
				
				if(q==var[j].size()) {

					string st3="";
					for(;q<prod[p][l].size();q++)
					st3 += prod[p][l][q];

					for(q=1;q<mp[var[j]].second;q++) 
					temp.push_back(prod[p1][q]+st3);
				} else 
				temp.push_back(prod[p][l]);
			}

			for(q=0;q<temp.size();q++)
			prod[p][q+1] = temp[q];
			mp[var[i]].second = temp.size()+1; 
		}


		for(int l=1;l<mp[var[i]].second;l++) {
			for(q=0;q<var[i].size();q++) {
				if(var[i][q]!=prod[p][l][q])
				break;
			}

			if(q == var[i].size())  {
				string st3="";
				for(;q<prod[p][l].size();q++)
				st3 += prod[p][l][q];
				match.push_back(st3);
			}
			else 
			unmatch.push_back(prod[p][l]);
		}


		if(match.size() == 0)
		continue;
		char ch1=39;
		string st1 = var[i]+ch1;
		for(q=0;q<unmatch.size();q++) {
			string st2 = unmatch[q]+st1;
			prod[p][q+1] = st2; 
		}
		mp[var[i]].second = q+1;
		mp[st1].first = k;
		prod[k][0] = st1;
		for(q=0;q<match.size();q++) {
			string st2 = match[q]+st1;
			prod[k][q+1] = st2; 
		}
		char ch='!';
		string st10;
		st10+=ch;
		prod[k][q+1]=st10;
		mp[st1].second =q+2;
		k++;

	}
	ofstream myfile;
	myfile.open ("example.txt");
	for(int q=0;q<k;q++) {
		//ofstream myfile;
		myfile << prod[q][0]<<" = "<<prod[q][1];
		cout<<prod[q][0]<<"->"<<prod[q][1];
		for(int q1=2;q1<mp[prod[q][0]].second;q1++) {
		myfile << "|"<<prod[q][q1]; 		
		cout <<"|"<<prod[q][q1];
		}
		myfile << "\n";
		cout <<"\n";
	}
	myfile.close();
	return 0;
}
/*
4
E
T
F
K
5
+
*
(
)
id
4
E->E+T|T
T->T*F|K
K->K*E
F->(E)|idw
*/
