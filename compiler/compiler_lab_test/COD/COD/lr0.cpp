#include <bits/stdc++.h>
#define f first
#define s second
#define prod pair < char, string >

using namespace std;

map < char, vector < string > > grammar;
map < prod , int > prod_num;
map < int, prod > rprod_num;

map <char, int> ter_map;
map <char, int> non_ter_map;
map <int, char> rter_map;
map <int, char> rnon_ter_map;
int sz = 0;

char nonterminal[100];
char cannmat[100][100];
char aug_start_sym = 'Z';
int cnt;
int symbols;

map < int, vector < prod > > cann_mp;
map < vector < prod > , int > rcann_mp;

int parsing_table[100][100];

bool terminal(char ch) {
    if (ch >= 'A' && ch <= 'Z')
        return false;
    return true;
}
void print(vector < prod > state) {
    for (int i = 0; i < state.size(); i++) {
        cout << state[i].f << " " << state[i].s << endl;
    }
}
vector < prod > closure(prod p) {

    vector < prod > state;
    string tmp = p.second;
    int ind = tmp.find('.');

    state.push_back(make_pair(p.f, tmp));

    if (ind == tmp.length() - 1) {
        return state;
    }

    map < prod, bool > visited;
    for (int i = 0; i < state.size(); i++) {
        tmp = state[i].s;
        ind = tmp.find('.');
        ind++;

        if (ind == tmp.length())
            continue;

        char sym = tmp[ind];
        if (terminal(sym)) {
            continue;
        }

        vector < string > temp = grammar[sym];

        for (int j = 0; j < temp.size(); j++) {

            string pd = "";
            if (temp[j].compare("e") != 0)
                pd = "." + temp[j];
            else
                pd = ".";
            if (visited.find(make_pair(sym, pd)) == visited.end()) {
                state.push_back(make_pair(sym, pd));
                visited[make_pair(sym, pd)] = true;
            }
        }

    }

    return state;
}

void create_canonical_collection() {

    queue < vector < prod > > q;
    string temp = "." + grammar[aug_start_sym][0];

    vector < prod > tmp;

    tmp = closure(make_pair(aug_start_sym, temp));

    cnt = 0;
    cann_mp[cnt] = tmp;
    rcann_mp[tmp] = cnt;
    q.push(tmp);

    while (!q.empty()) {
        tmp = q.front();
        q.pop();
       // cout << "FROM QUEUE : " << rcann_mp[tmp] << endl;
        //print(tmp);
        //cout << endl;
        map < char, vector < prod > > mp;
        vector <char> trans_sym;
        for (int i = 0; i < tmp.size(); i++) {
            prod pt = tmp[i];
            string dtmp = pt.s;
            int ind = dtmp.find('.');
            if (ind == dtmp.length() - 1)
                continue;

            swap(dtmp[ind], dtmp[ind + 1]);
            vector <prod> state = closure(make_pair(pt.f, dtmp));
            trans_sym.push_back(dtmp[ind]);
            for (int j = 0; j < state.size(); j++)
                  mp[dtmp[ind]].push_back(state[j]);
        }
       // map < char, vector < prod > > :: iterator it;
       // it = mp.begin();
       // cout << "States\n";
        //while (it != mp.end()) {
        for (int i = 0; i < trans_sym.size(); i++) {
            //char sym = it->first;
            char sym = trans_sym[i];
            //vector <prod> state = it->second;
            vector <prod> state = mp[sym];
            if (rcann_mp.find(state) == rcann_mp.end()) {
                cnt++;
                rcann_mp[state] = cnt;
                cann_mp[cnt] = state;
                q.push(state);
            }
            cannmat[rcann_mp[tmp]][rcann_mp[state]] = sym;
            //cout << sym << " " << rcann_mp[state] << endl;
            //print(state);
            //cout << endl << endl;
            //it++;
            bool flg = false;
            for (int j = 0; j < state.size(); j++) {
                string str = state[j].s;
                int id = str.find('.');
                if (id == str.length() - 1)
                    flg = true;
            }

            if (flg && state.size() >= 2) {
                cout << "Grammar is not LR(0)\n";
                return ;
            }

           if (flg) {
                prod pr = state[0];
                int id = pr.s.find('.');
                pr.s.erase(id, 1);

                int val = prod_num[pr];

                if (val == 0) {
                    parsing_table[rcann_mp[state]][ter_map['$']] = INT_MIN;
                } else {
                    map < char,  int > :: iterator it;
                    it = ter_map.begin();
                    while (it != ter_map.end()) {
                        parsing_table[rcann_mp[state]][it->s] = val * (-1);
                        it++;
                    }
                }
                //parsing_table[rcann_mp[tmp]][]
           }
            int idc;
            if (terminal(sym)) {
                idc = ter_map[sym];
            } else {
                idc = non_ter_map[sym];
            }
            parsing_table[rcann_mp[tmp]][idc] = rcann_mp[state];

        }
    }
    sz = ter_map.size() + non_ter_map.size();
    //cout << sz << endl;
    for (int i = 0; i <= cnt; i++) {
        for (int j = 0; j < sz; j++) {
            cout << parsing_table[i][j] << " ";
        }
        cout << endl;
    }
}

void parse_input_string(string str) {
    stack <int> stk;
    stk.push(0);
    int i = 0;
    str = str + "$";

    while (true) {
        int tp = stk.top();
        int val = parsing_table[tp][ter_map[str[i]]];
        if (val > 0) {

            stk.push(val);
            i++;
        } else if (val < 0 && val != INT_MIN) {
            val = val * (-1);
            prod pr = rprod_num[val];
            cout << pr.f << " -> " << pr.s << endl;
            int l = pr.s.length();

            while (l--) stk.pop();
            tp = stk.top();
            stk.push(parsing_table[tp][non_ter_map[pr.f]]);
        } else if (val == INT_MIN) {
            cout << "ACCEPTED" << endl;
            return;
        } else {
            cout << "ERROR";
            return ;
        }
    }
}

int main() {

    cin >> symbols;
    nonterminal[0] = aug_start_sym;
    set <char> terminals;
    int pr_num = 1;
    for(int i = 1; i <= symbols; ++i) {
        cin >> nonterminal[i];
        prod b;
        int productions;
        cin >> productions;
        b.f = nonterminal[i];
        for(int j = 0; j < productions; ++j) {
            string str;
            cin >> str;

            grammar[nonterminal[i]].push_back(str);
            b.s = str;
            prod_num[b] = pr_num;
            rprod_num[pr_num] = b;
            pr_num++;
            for (int l = 0; l < str.length(); l++) {
                if (str[l] != 'e' && terminal(str[l])) {
                    terminals.insert(str[l]);
                }
            }
        }
    }

    string str(1, nonterminal[1]);
    grammar[aug_start_sym].push_back(str);
    terminals.insert('$');
    int c = 0;
    set <char> :: iterator it;
    it = terminals.begin();
    cout << endl;

    while (it != terminals.end()) {
        ter_map[*it] = c;
        rter_map[c] = *it;

        cout << ter_map[*it] << " -> " << rter_map[c] << endl;
        c++;
        it++;

    }
    for(int i = 1; i <= symbols; ++i) {
        non_ter_map[nonterminal[i]] = c;
        rnon_ter_map[c] = nonterminal[i];
        cout << non_ter_map[nonterminal[i]] << " -> " << rnon_ter_map[c] << endl;
        c++;

    }
    create_canonical_collection();
    cout << "Input a string : \n";
    string input_string;
    cin >> input_string;
    parse_input_string(input_string);
    return 0;
}
/*
E' + E
E -+ E + T (T
T + T*F 1 F
E -+ (E) I id
*/

/*
3
E 2
E+T T
T 2
T*F F
F 2
(E) x

2
S 2
(L) a
L 2
L,S S

3
V 1
S
S 1
AA
A 2
aA b

2
S 1
AA
A 2
aA b

2
S 2
(L) a
L 2
L,S S

1
S 2
(S) x
*/
