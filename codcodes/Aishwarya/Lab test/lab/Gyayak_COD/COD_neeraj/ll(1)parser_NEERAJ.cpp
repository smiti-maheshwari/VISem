#include <bits/stdc++.h>

#define s(x) scanf("%d", &x)

using namespace std;

string s[100005];
map<string, set<string> > first, fol;
map<string, int> nont;
int n;
map<pair<string, string>, string> tab;

void fun(int id, string cur)
{
    id--;
    int i, j, cnt = 0;
    string tmp, lol = "";
    string vec[100];

    for (i = 3; i < s[id].size(); i++) {
        if (s[id][i] == '|') {
            vec[cnt] = lol;
            cnt++;
            lol = "";
        }
        else {
            lol +=  s[id][i];
        }
    }

    if (lol.size() != 0)
        vec[cnt] = lol;

    for (i = 0; i <= cnt; i++) {
        //cout << vec[i] << endl;
        for (j = 0; j < vec[i].size(); j++) {
            tmp = vec[i][j];
            if (nont.find(tmp) == nont.end()) {
                //cout << "ddgd " << tmp << " " << cur << endl;
                first[cur].insert(tmp);
                break;
            }
            else {
                fun(nont[tmp], tmp);
                string fir;
                int f = 0;
                set<string> :: iterator it;
                for (it = first[tmp].begin(); it != first[tmp].end(); it++) {
                    fir = (*it);
                    if (fir == "e")
                        f = 1;
                    else
                        first[cur].insert(fir);
                }
                if (!f)
                    break;
                }
        }
    }
}

void follow(string strr)
{
    int i, j, cnt = 0, k;
    string tmp, lol = "", use;

    for (int id = 0; id < n; id++) {
        string vec[100];
        lol = "";
        for (i = 3; i < s[id].size(); i++) {
            if (s[id][i] == '|') {
                vec[cnt] = lol;
                cnt++;
                lol = "";
            }
            else {
                lol +=  s[id][i];
            }
        }

        if (lol.size() != 0)
            vec[cnt] = lol;

        lol = "";

        for (i = 0; i <= cnt; i++) {
            for (j = 0; j < vec[i].size(); j++) {
                tmp = vec[i][j];
                if (tmp == strr) {
                    int f = 0;
                    for (k = j+1; k < vec[i].size(); k++) {
                        lol = vec[i][k];
                        if (nont.find(lol) == nont.end()) {
                            fol[tmp].insert(lol);
                            break;
                        }
                        else {
                            set<string> :: iterator it;
                            for (it = first[lol].begin(); it != first[lol].end(); it++) {
                                use = (*it);
                                if (use == "e") {
                                    f = 1;
                                    continue;
                                }
                                else {
                                    fol[tmp].insert(use);
                                }
                            }
                        }
                        if (!f) {
                            break;
                        }
                    }

                    if (k == vec[i].size()) {
                        lol = s[id][0];
                        if (lol != tmp) {
                            follow(lol);
                            set<string> :: iterator it;
                            for (it = fol[lol].begin(); it != fol[lol].end(); it++)
                                fol[tmp].insert(*it);
                        }
                    }
                }
            }
        }
    }
}

void table(int id, string cur)
{
    id--;
    int i, j, cnt = 0;
    string tmp, lol = "";
    string vec[100];

    for (i = 3; i < s[id].size(); i++) {
        if (s[id][i] == '|') {
            vec[cnt] = lol;
            cnt++;
            lol = "";
        }
        else {
            lol +=  s[id][i];
        }
    }

    if (lol.size() != 0)
        vec[cnt] = lol;

    for (i = 0; i <= cnt; i++) {
        for (j = 0; j < vec[i].size(); j++) {
            tmp = vec[i][j];
            if (nont.find(tmp) == nont.end() && tmp != "e") {
                lol = cur + " -> ";
                lol = lol + vec[i];
                tab[make_pair(cur, tmp)] = lol;
                break;
            }
            else if (tmp == "e") {
                set<string> :: iterator it;
                for (it = fol[cur].begin(); it != fol[cur].end(); it++) {
                    lol = cur + " -> ";
                    lol = lol + vec[i];
                    tab[make_pair(cur, (*it))] = lol;
                }
            }
            else {
                string fir;
                int f = 0;
                set<string> :: iterator it;
                for (it = first[tmp].begin(); it != first[tmp].end(); it++) {
                    fir = (*it);
                    if (fir == "e") {
                        f = 1;
                    }
                    else {
                        lol = cur + " -> ";
                        lol = lol + vec[i];
                        tab[make_pair(cur, fir)] = lol;
                    }
                }
                if (!f)
                    break;

            }
        }
    }
}

int main()
{
    int  i, j;
    s(n);

    for (i = 0; i < n ;i++)
        cin >> s[i];

    string tmp = "", lol = "";
    for (i = 0; i < n; i++) {
        tmp = "";
        for (j = 0; j < s[i].size(); j++) {
            if (s[i][j] == '-')
                break;
            tmp = tmp + s[i][j];
        }
        nont[tmp] = i+1;
    }

    map<string, int> :: iterator it;
    for (it = nont.begin(); it != nont.end(); it++) {
        string cur = (it->first);
        int id = it->second;
        //cout << cur <<  " " << id << endl;
        fun(id, cur);
    }
    map<string, set<string> > :: iterator it1;
    //set<string> :: iterator sit;

    for (it = nont.begin(); it != nont.end(); it++) {
        cout << (it->first) << " ------ ";
        set<string> :: iterator sit;
            for (sit = first[it->first].begin(); sit != first[it->first].end(); sit++)
                cout << (*sit) << " ";
        cout << endl;
    }


    /** FOLLOW **/

    string use;

    use = s[0][0];
    fol[use].insert("$");

     for (it = nont.begin(); it != nont.end(); it++) {
        follow(it->first);
    }

    cout << endl << endl;

    for (it = nont.begin(); it != nont.end(); it++) {
        cout << (it->first) << " ------ ";
        set<string> :: iterator sit;
            for (sit = fol[it->first].begin(); sit != fol[it->first].end(); sit++)
                cout << (*sit) << " ";
        cout << endl;
    }
    cout << endl;


    /**LL(1) PARSER **/

    for (it = nont.begin(); it != nont.end(); it++) {
        string cur = (it->first);
        int id = it->second;
        //cout << cur <<  " " << id << endl;
        table(id, cur);
    }

    map<pair<string, string>, string> :: iterator tabit;
    for (tabit = tab.begin(); tabit != tab.end(); tabit++) {
        cout << "M[" << tabit->first.first << ", " << tabit->first.second << "]  =  " << tabit->second << endl;
    }


    /**LL(1) JUDGE **/

    string inp;

    cin >> inp;
    inp.push_back('$');
    stack<string> st;

    lol = "$";
    st.push(lol);

    lol = s[0][0];
    st.push(lol);

    string buff = "";
    i = 0;
    int flag = 0;
    while (!st.empty()) {
        buff = inp[i];
        lol = st.top();
        st.pop();

        //cout <<st.size() << " " << lol << "-----\n" << endl;

        if (lol == "$" && buff == "$") {
            flag = 1;
            break;
        }

        if (lol == buff) {
            i++;
            continue;
        }
        pair<string, string> sp;
        sp = make_pair(lol, buff);
        if (tab.find(sp) != tab.end()) {
            string bla = tab[sp];

            for (j = bla.size()-1; j > 4; j--) {
                tmp = bla[j];
                //cout << tmp << endl;;
                if (tmp != "e")
                    st.push(tmp);
            }
        }
        else {
            break;
        }
    }

    if (flag)
        cout << "YES\n";
    else
        cout << "NO\n";

}
