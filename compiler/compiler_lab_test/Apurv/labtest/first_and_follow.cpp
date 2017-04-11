#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define S(x) scanf("%d", &x)
#define Sl(x) scanf("%lld", &x)
#define mem(a,x) memset(a,x,sizeof(a))

map<char,vector<string> >m;
map<char,set<char> >firstset, followset;
map<char,set<char> >::iterator pos;
map<char,vector<string> > :: iterator it;

int first(char ch)
{
    if(ch >= 65 && ch <= 90){
        return 1;
    }
    else{
        return 0;
    }
}

char rec(char ch)
{
    int i,j;
    for (i = 0; i < m[ch].size(); i++){
        string s = m[ch][i];
        int f = 0;
        for(j = 0; j < s.size(); j++){
            if(first(s[j]) == 0){
                firstset[ch].insert(s[j]);
                break;
            }
            else{
                rec(s[j]);
                for (set<char> :: iterator it2 = firstset[s[j]].begin(); it2 != firstset[s[j]].end(); it2++){
                    if(*it2 != '?'){
                        firstset[ch].insert(*it2);
                    }
                    else {
                        f++;
                    }
                }
                if(firstset[s[j]].find('?') == firstset[s[j]].end()){
                    break;
                }
            }
        }
        if(f == s.size()){
            firstset[ch].insert('?');
        }
    }
}

void calc_follow(char c)
{
    //cout << m.size() << endl;
    map<char,vector<string> > :: iterator itp;
    for(itp = m.begin(); itp != m.end(); ++itp) {
        char x = itp->first;
        //cout << x << endl;
        for(int i = 0; i < m[x].size(); i++) {
            string s = m[x][i];
            for(int j = 0; j < s.size(); j++) {
                if(s[j] == c) {
                    int f = 0;
                    for(int k = j+1; k < s.size(); ++k) {
                        if(first(s[k]) == 0) {
                            followset[c].insert(s[k]);
                            f = 1;
                            break;
                        } else {
                            for(set<char>::iterator itx = firstset[s[k]].begin(); itx != firstset[s[k]].end(); itx++) {
                                if(*itx != '?')
                                    followset[c].insert(*itx);
                            }
                            if(firstset[s[k]].find('?') == firstset[s[k]].end()) {
                                f = 1;
                                break;
                            }
                        }
                    }
                    if(!f) {
                        if(x != c) {
                            calc_follow(x);
                            for(set<char>::iterator itx = followset[x].begin(); itx != followset[x].end(); itx++) {
                                    followset[c].insert(*itx);
                            }
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    int n,i;
    char ch;
    string s;

    cin >> n;

    for (i = 1; i <= n; i++){
        cin >> ch;
        cin >> s;
        m[ch].push_back(s);
    }
    /*for (it = m.begin(); it != m.end(); it++){
        cout << it->first<<" ";
        for (i = 0; i < (it->second).size(); i++){
            cout << (it->second)[i]<<" ";
        }
        cout << endl;
    }*/
    cout << endl;
    for (it = m.begin(); it != m.end(); it++){
        rec(it->first);
    }
    followset['S'].insert('$');

    for (it = m.begin(); it != m.end(); it++){
        calc_follow(it->first);
    }

    set<char>ans;
    cout << "First -\n";
    for (pos = firstset.begin(); pos != firstset.end(); pos++){
        if(pos->second.size() != 0){
            cout << pos->first<<" ";
            ans = pos->second;
            for (set<char>::iterator it1 = ans.begin(); it1 != ans.end(); it1++){
                cout << (*it1)<<" ";
            }
            cout << endl;
        }
    }
    cout << endl << "Follow -\n";
    for (pos = followset.begin(); pos != followset.end(); pos++){
        if(pos->second.size() != 0){
            cout << pos->first<<" ";
            ans = pos->second;
            for (set<char>::iterator it1 = ans.begin(); it1 != ans.end(); it1++){
                cout << (*it1)<<" ";
            }
            cout << endl;
        }
    }
    return 0;
}

