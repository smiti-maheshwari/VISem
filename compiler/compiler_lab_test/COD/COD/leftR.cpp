#include <bits/stdc++.h>
using namespace std;
map <char, vector<string> > mp;
vector <char> vec;
int terminal_record[256] = {0};
//print the grammar
void print()
{
    //cout << vec.size() << endl;
	for(int i = 0; i < vec.size(); i++) {
		if(mp[vec[i]].size() == 0) continue;
		cout << vec[i] << "->";
		for(int j = 0 ; j < mp[vec[i]].size(); j++) {
			cout << mp[vec[i]][j];
			if(j != mp[vec[i]].size() - 1) cout << "|";
		}
		cout << endl;
	}
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


// reversing string
void reverse_string(string s)
{
    long long int sz, i;
    string s2;

    sz = s.size();

    for (i = sz - 1; i >= 0; i--) {
        s2 = s2 + s[i];
    }
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
void firstu()
{
    set<string> zone;
    char m[26] = {'a','b','c','d','e','f'};
    string zee = "";
    int w = 0;
    int i;
    for(i = 0;i < 100; i++)
    {
        zee += m[w];
        if(w == 5 )
        {
            w = 0;
        }
    }
    string fl = "->";
    string jo = "A->ba";
    zone.insert(zee);
    zone.insert(fl);
    zone.insert(jo);
}

int main()
{
	int symbols, no_pro;
	char non_term;
	char t,c2,c3;
	string s;
	string str;
	cin >> symbols;
	firstu();

    for(int i = 0; i < symbols; i++) {
        cin>>t>>c2>>c3>>s;
        vec.push_back(t);
		terminal_record[t] = 1;
        while(1) {
            int found = s.find('|');
           if(found != string::npos) {
                mp[t].push_back(s.substr(0,found));
                s = s.substr(found+1,s.size());
           }
           else {
                mp[t].push_back(s);
                break;
           }

           reverse_string(s);

        }
    }

	//print();
	//cout << endl;
	remove_indirect();
	remove_imme();
	print();
	return 0;
}

/*
3
E->E+T|T
T->T*F|F
F->(E)|i

*/
