#include <bits/stdc++.h>
using namespace std;
map<char,vector<string> > m;
map<char,int> hash1;
map<char,int> hash2;
map<char,set<char> > first;
map<char,set<char> > follow;
map <char, vector<string> > mp;
vector <char> vec;
int terminal_record[256] = {0};
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
				if(tmp == "$") tmp = po;
				else tmp += po;
				//cout << tmp << endl;
				tpm.push_back(tmp);
			}
		}
		mp[vec[i]].clear();
		mp[vec[i]] = tpm;
		mp[(char)po].push_back("$");
	}
}

int terminal(char c)
{
    if(!isupper(c) && c != '$') {
        return 1;
    }
    return 0;
}
void First(char nt)
{
    int i;
    int j;
    int k;
    char ch;
    int flag;
    int cnt;
    set<char> :: iterator it;

    for(i = 0; i < m[nt].size(); i++) {
        cnt = 0;
        for(j = 0; j < m[nt][i].size(); j++) {
            ch = m[nt][i][j];
            flag = 0;
            if(terminal(ch) || ch == '$') {
                first[nt].insert(ch);
                break;
            }
            else if(hash1[ch] == 0) {
                First(ch);
                hash1[ch] = 1;
            }
            if(hash1[ch] == 1) {
                for(it = first[ch].begin(); it != first[ch].end(); it++) {
                    if(*it == '$') {
                        flag = 1;
                        cnt++;
                    }
                    else {
                        first[nt].insert(*it);
                    }
                }
            }
            if(flag == 0) {
                break;
            }
            if(cnt == m[nt][i].size()) {
                first[nt].insert('$');
            }
        }

    }
    hash1[nt] = 1;
}
void Follow(char nt)
{
    int i;
    int j;
    int flag;
    string s;
    int k;
    int flag1;
    map<char,vector<string> > :: iterator it;
    set<char> :: iterator ti;
    vector<string> v;
    for(it = m.begin(); it != m.end(); it++) {
        v = it->second;
        for(i = 0; i < v.size(); i++) {
            s = v[i];
            flag1 = 0;
            for(j = 1; j < s.size(); j++) {
                flag = 0;
                if(s[j-1] == nt || flag1) {
                    flag1 = 0;
                    if(terminal(s[j])) {
                        follow[nt].insert(s[j]);
                        break;
                    }
                    else {
                        for(ti = first[s[j]].begin(); ti != first[s[j]].end(); ti++) {
                            if(*ti == '$') {
                                flag1 = 1;
                                if(it->first != nt && j == s.size()-1) {
                                    flag = 1;
                                }
                            }
                            if(*ti != '$') {
                                follow[nt].insert(*ti);
                            }
                        }
                    }
                }
                if(flag == 1 || (s[j] == nt && (j == s.size()-1) && (it->first != nt))) {
                    if(hash2[it->first] == 0) {
                        Follow(it->first);
                        hash2[it->first] = 1;
                    }
                    if(hash2[it->first] == 1) {
                        for(ti = follow[it->first].begin(); ti != follow[it->first].end(); ti++) {
                            follow[nt].insert(*ti);
                        }
                    }
                }
            }
        }
    }
    hash2[nt] = 1;

}

int main()
{
    int n;
    int i;
    string s;
    int found;
    char c2,c3;
    char t;
    string c1;
    string t1;
    int j;
    char st;
    map<char,vector<string> > :: iterator it1;
    set<char> :: iterator it;

    cin>>n;
    for(int i = 0; i < n; i++) {
        cin>>t>>c2>>c3>>s;
        if(i == 0) {
            st = t;
        }
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
        }
    }


    //print();
    remove_indirect();
	remove_imme();
	print();
	m = mp;
    for(it1 = m.begin(); it1 != m.end(); it1++) {
        if(hash1[it1->first] == 0) {
            First(it1->first);
        }
        if(first[it1->first].size()) {
            cout<<it1->first<<"->";
        }
        for(it = first[it1->first].begin(); it != first[it1->first].end(); it++) {
            cout<<*it<<' ';
        }
        cout<<endl;
    }
    cout<<endl;
    t1.clear();
    t1.push_back('@');
    follow[st].insert('@');
    for(it1 = m.begin(); it1 != m.end(); it1++) {
        if(hash2[it1->first] == 0) {
            Follow(it1->first);
        }
        if(follow[it1->first].size()) {
            cout<<it1->first<<"->";
        }

        for(it = follow[it1->first].begin(); it != follow[it1->first].end(); it++) {
            cout<<*it<<' ';
        }
        cout<<endl;
    }

}

/*
3
E->E+T|T
T->T*F|F
F->(E)|i

*/

