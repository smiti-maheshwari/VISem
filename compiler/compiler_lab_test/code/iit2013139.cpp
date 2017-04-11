/// APURV KHATRI
/// IIT2013139
/// C++11 ISO C++ LANGUAGE STANDARD [-std=c++11]

#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define S(x) scanf("%d",&x)
#define mem(x,i) memset(x,i,sizeof(x))
#define pb push_back
#define mp make_pair
#define fi first
#define se second

#define item set<pair<char,string> >

map<char,set<string> >inp;
map<char,item>closure;
set<char>var;
map<item,int>state;
map<pair<item,char>,item >table;
map<item,int>item_no;
map<pair<int,char>,int>gt;
map<pair<int,char>,pair<char,int> >action;
map<pair<char,string>,int >pro_no;
map<char,vector<string> >m;
pair<char,string>pr1[100004];
map<char,set<char> >firstset, followset;
map<char,set<char> >::iterator pos;
map<char,vector<string> > :: iterator it;

int isNT(char ch)
{
    if(ch >= 65 && ch <= 90)
        return 1;

    return 0;
}

int first(char ch)
{
    if(ch >= 65 && ch <= 90)
        return 1;

    return 0;
}

void findCLO(char ch)
{
    set<string>st,st1;
    string tmp;
    st = inp[ch];
    if(closure.find(ch) == closure.end()){
        for(auto it = st.begin(); it != st.end(); it++){
            tmp = *it;
            tmp.insert(tmp.begin(),'.');
            if(tmp[1] == '?'){
                tmp.erase(tmp.begin()+1);
            }
            //cout << tmp<<" ";
            closure[ch].insert(mp(ch,tmp));
            if(isNT(tmp[1])){
                findCLO(tmp[1]);
                set<pair<char,string> > st1 = closure[tmp[1]];
                for (auto it1 = st1.begin(); it1 != st1.end(); it1++){
                    closure[ch].insert(mp((*it1).fi,(*it1).se));
                }
            }
        }
    }
}

void bfs()
{
    int i;
    string s;
    set<pair<char,string> > cur,pro;
    queue<item>q;
    cur.insert(mp('X',".S"));
    pro = closure['S'];
    for (auto it = pro.begin(); it != pro.end(); it++){
        cur.insert(*it);
    }
    state[cur] = 1;
    item_no[cur] = 0;
    q.push(cur);
    while(!q.empty()){
        cur = q.front();
        q.pop();
        for (auto it = var.begin(); it != var.end(); it++){
            pro.clear();
            for (auto it1 = cur.begin(); it1 != cur.end(); it1++){
                for (i = 0; i+1 < (*it1).se.size(); i++){
                    if((*it1).se[i] == '.' && (*it1).se[i+1] == (*it)){
                        s = (*it1).se;
                        s.insert(s.begin()+i+2,'.');
                        s.erase(s.begin()+i);
                        pro.insert(mp((*it1).fi,s));
                        if(i+2 < s.size()){
                            if(isNT(s[i+2])){
                                for (auto it2 = closure[s[i+2]].begin(); it2 != closure[s[i+2]].end(); it2++){
                                    pro.insert(*it2);
                                }
                            }
                        }
                    }
                }
            }
            if(pro.size()){
                if (state[pro] == 0) {
                    q.push(pro);
                    state[pro] = 1;
                }
                table[mp(cur,(*it))] = pro;
            }
        }
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
                if(ch == s[j]){
                    break;
                }
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
    int n,i,sz;
    char ch;
    string tmp,mys;

    set <string> st;

    st.insert("S");
    inp['X'] = st;

    var.insert('X');
    var.insert('$');

    int id = 1;
    pro_no[mp('X',"S")] = id;
    id++;

    n = 6;
    for (i = 1; i <= n; i++){
        cin >> mys;
        ch = mys[0];
        if(ch == 'E')
            ch = 'S';
        var.insert(ch);
        int ff = 0;
        if(!isNT(ch)){
            cout << "Missing operand\n";
            ff = 1;
        }
        int op,ob=0,cb=0;
        for(op = 0; op < mys.size(); ++op){
            if(mys[op] == '(')
                ++ob;
            if(mys[op] == ')')
                ++cb;
        }
        //cout << ob << " " << cb << endl;
        if(ob > cb){
            cout << "Missing right parenthesis\n";
            ff = 1;
        }
        if(cb > ob){
            cout << "Missing left parenthesis\n";
            ff = 1;
        }

        if(ff)
            return 0;

        tmp.clear();
        for(op = 3; op < mys.size(); ++op){
            tmp.pb(mys[op]);
        }

        for (int j = 0; j < tmp.size(); j++){
            if(tmp[j] == 'E')
                tmp[j] = 'S';
            var.insert(tmp[j]);
        }
        pro_no[mp(ch,tmp)] = id;
        pr1[id] = mp(ch,tmp);
        id++;
        inp[ch].insert(tmp);
        m[ch].pb(tmp);
    }

    for (it = m.begin(); it != m.end(); it++){
        rec(it->first);
    }
    followset['S'].insert('$');

    for (it = m.begin(); it != m.end(); it++){
        calc_follow(it->first);
    }
    set<char>ans;
    set<char>tmp_set;
    tmp_set.insert('$');
    followset['X'] = tmp_set;

    for (auto it = inp.begin(); it != inp.end(); it++){
        findCLO(it->fi);
    }


    bfs();


    id = 1;
    for (auto it = state.begin(); it != state.end(); it++){
        if(item_no.find(it->fi) == item_no.end()){
            item_no[it->fi] = id++;
        }
    }

    for (auto it = table.begin(); it != table.end(); it++){
        //cout << item_no[it->fi.fi] << " " << it->fi.se << " " << item_no[it->se] << endl;
        if(isNT(it->fi.se)){
             gt[mp(item_no[it->fi.fi],it->fi.se)] = item_no[it->se];
        }
        else{
             action[mp(item_no[it->fi.fi],it->fi.se)] = mp('S',item_no[it->se]);
        }
    }
    for (auto it = state.begin(); it != state.end(); it++){
         for (auto it1 = it->fi.begin(); it1 != it->fi.end(); it1++){
            ch = (*it1).fi;
            string s = (*it1).se;
            if(s[s.size()-1] == '.'){
                s.erase(s.end()-1);
                if(s.size() == 0){
                    s = "?";
                }
                int num = pro_no[mp(ch,s)];
                for (auto it2 = followset[ch].begin(); it2 != followset[ch].end(); it2++){
                    if(!isNT(*it2)){
                        auto it3 = action.find(mp(item_no[it->fi],*it2));
                        if(it3 == action.end()){
                            if(ch == 'X' && *it2 == '$')
                                action[mp(item_no[it->fi],*it2)] = mp('A',num);
                            else{
                                action[mp(item_no[it->fi],*it2)] = mp('R',num);
                            }
                        }
                        else{
                            if(it3->se.fi == 'R'){
                                cout << "Reduce Reduce Conflict" << endl;
                            }
                            else{
                                cout << "Shift Reduce Conflict" << endl;
                            }
                        }
                    }
                }
            }
         }
    }

    cout << "\nGOTO TABLE:-\n\n";
    for (auto it = gt.begin(); it != gt.end(); it++){
        cout << it->fi.fi << " -> " << it->fi.se << " -> " << it->se << endl;
    }

    cout << "\nACTION TABLE:-\n\n";
    for (auto it = action.begin(); it != action.end(); it++){
        cout << it->fi.fi << " -> " << it->fi.se << " -> " << it->se.fi << it->se.se << endl;
    }

    return 0;
}


