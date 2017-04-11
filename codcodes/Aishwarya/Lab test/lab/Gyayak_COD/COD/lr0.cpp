#include <bits/stdc++.h>

using namespace std;

#define REP(i,a,b) for (int i = a; i <= b; ++i)
#define TR(c,it) for (auto it = c.begin(); it != c.end(); it++)
#define mp make_pair
#define fi first
#define se second
#define pb push_back


/*Sample Input

2
S 1
A
A 2
aAb
c


Another sample

3
S 1
E
E 3
T
E+T
E-T
T 2
i
(E)

*/

struct item_set
{
    set <string> prod;
    map <char, int> next;
} is[30];

struct inp
{
    char ch;
    set <string> prod;
} sym[30];

map <string, int> prod_set;
pair <set <string>, map <char, int> > cnv;
stack <int> st;
map <pair <set <string>, map <char, int> >, int> m;
map <char, int> has;
set <char> symbols;
map <pair <int, char>, string> table;
map <int, string> r_prod_set;

bool isEqual(item_set item1, item_set item2)
{
    if (item1.prod.size() != item2.prod.size() || item1.next.size() != item2.next.size())
        return 0;

    TR(item1.prod, it) {
        if (item2.prod.find(*it) == item2.prod.end())
            return 0;
    }

    TR (item1.next, it) {
        if (item2.next.find(it->first) == item2.next.end() || item2.next[it->first] != it->second)
            return 0;
    }

    return 1;
}

item_set closure(item_set ts)
{
    item_set ns;
    string s;

    ns = ts;

    symbols.insert('$');
    TR (ts.prod, it) {
        REP (i, 0, (*it).size() - 2) {
            if ((*it)[i] == '.' && (*it)[i + 1] >= 'A' && (*it)[i + 1] <= 'Z') {
                TR (sym[has[(*it)[i + 1]]].prod, it1) {
                    s = sym[has[(*it)[i + 1]]].ch;
                    s = s + "->.";
                    s = s + (*it1);

                    ns.prod.insert(s);
                }
            }
        }
    }

    if (isEqual(ns, ts))
        return ns;

    return closure(ns);
}

item_set init_itemset()
{
    item_set ns;
    string s;

    ns.prod.clear();

//    cout << sym[1].ch << " " << sym[]

    TR (sym[0].prod, it) {
        s = sym[0].ch;
        s = s + "->.";
        s = s + (*it);

        //cout << "here " << sym[1].ch << " " << (*it) << " " << s << endl;
        ns.prod.insert(s);
    }

    return closure(ns);
}

item_set next_item(item_set ts, char ch)
{
    item_set ns;
    string s, tmp;

    ns.prod.clear();

    TR (ts.prod, it) {
        tmp = *it;
        //cout << tmp << endl;

        REP (i, 0, tmp.size() - 2) {
            if (tmp[i] == '.' && tmp[i + 1] == ch) {
                s = tmp.substr(0, i) + tmp[i + 1] + "." + (i + 2 != tmp.size() ? tmp.substr(i + 2, tmp.size() - i - 2) : "");

                ns.prod.insert(s);
            }
        }
    }

    return closure(ns);
}

bool test(string str)
{
    stack <char> st;
    char tp;
    string pp, cpy, tmp;

    int state_num, lp = 0;

    st.push('0');
    while (!st.empty()) {
        tp = st.top();
        cout << "Top" << tp << " " << str[lp] << endl;

        state_num = tp - '0';

        if (table[mp(state_num, str[lp])] == "acc")
            return 1;

        if (table[mp(state_num, str[lp])] == "err") {
            cout << "Error\n";

            return 0;
        }

        tmp = table[mp(state_num, str[lp])];
        if (tmp[0] == 'S') {
            st.push(str[lp]);
            st.push(tmp[1]);
            ++lp;
        }

        else {
            pp = r_prod_set[tmp[1] - '0'];

            cpy = pp;
            cout << pp << " das\n";
            pp = pp.substr(3, pp.size() - 4);
            cout << " " << pp << endl;

            int len = 2 * pp.size();
            while (len--)
                st.pop();

            tp = st.top();
            state_num = tp - '0';
            if (table[mp(state_num, cpy[0])] == "err") {
                cout << "Error\n";

                return 0;
            }

            st.push(cpy[0]);
            st.push(table[mp(state_num, cpy[0])][0]);
        }
    }

    return 0;
}

int main()
{
    int n, nop, id, idx, flag;
    string s, tmp;

    cin >> n;

    int p_cnt = 1;
    REP (i, 1, n) {
        cin >> sym[i].ch;
        has[sym[i].ch] = i;

        symbols.insert(sym[i].ch);

        cin >> nop;

        REP (j, 1, nop) {
            cin >> s;

            sym[i].prod.insert(s);

            TR (s, it)
                symbols.insert(*it);

            tmp = sym[i].ch;
            tmp = tmp + "->";
            tmp = tmp + s;
            tmp = tmp + ".";

            prod_set[tmp] = p_cnt++;
            r_prod_set[p_cnt - 1] = tmp;
        }
    }

    string kk = "";
    kk.pb(sym[1].ch);

    sym[0].ch = 'K';
    has['K'] = 0;
    sym[0].prod.insert(kk);

    is[0] = init_itemset();

    //cout << is[0].prod.size() << " " << is[0].next.size() << endl;

    set <char> cs;

    cnv = mp(is[0].prod, is[0].next);
    id = 1;
    m[cnv] = 0;
    st.push(0);
    while (!st.empty()) {
        idx = st.top();
        st.pop();

        item_set cur = is[idx];

        cs.clear();

        TR (cur.prod, it) {
            s = *it;
            REP (i, 0, s.size() - 2) {
                if (s[i] == '.' && s[i + 1] != '?')
                    cs.insert(s[i + 1]);
            }
        }

        TR (cs, it) {
            item_set ts = next_item(cur, *it);

            cnv = mp(ts.prod, ts.next);
            if (m.find(cnv) != m.end())
                is[idx].next[*it] = m[cnv];

            else {
                is[id] = ts;
                m[cnv] = id;
                ++id;

                is[idx].next[*it] = id - 1;
                st.push(id - 1);
            }
        }
    }

    REP (i, 0, id - 1) {
        cout << "Item Set " << i << " " << is[i].prod.size() << " " << is[i].next.size() << endl;

        TR (is[i].prod, it)
            cout << (*it) << endl;
        cout << endl;
    }

    REP (i, 0, id - 1) {
        flag = 0;
        if (is[i].next.size() == 0) {
            tmp = *(is[i].prod.begin());

            //cout << i << " " << tmp << endl;
            if (prod_set.find(tmp) == prod_set.end()) {
                cout << "adlsfdk\n";

                TR (symbols, it)
                    table[mp(i, *it)] = "err";
                table[mp(i, '$')] = "acc";

                continue ;
            }
            flag = prod_set[tmp];
        }

        TR (symbols, it) {
            if (flag) {
                if ((*it) >= 'A' && (*it) <= 'Z')
                    table[mp(i, *it)] = "err";

                else {
                    tmp = "R";
                    tmp = tmp + (char)('0' + flag);
                    table[mp(i, *it)] = tmp;
                }
            }

            else {
                if (is[i].next.find(*it) == is[i].next.end())
                    table[mp(i, *it)] = "err";

                else {
                    if ((*it) >= 'A' && (*it) <= 'Z') {
                        tmp = (char)('0' + is[i].next[*it]);

                        table[mp(i, *it)] = tmp;
                    }

                    else {
                        tmp = "S";
                        tmp = tmp + (char)('0' + is[i].next[*it]);

                        table[mp(i, *it)] = tmp;
                    }
                }
            }
        }
    }

    TR (table, it) {
        cout << (it->first).fi << " " << (it->first).se << " " << it->second << endl;
    }

    string str;

    cin >> str;

    if (test(str))
        cout << "Accept\n";

    else
        cout << "Reject\n";

    return 0;
}
