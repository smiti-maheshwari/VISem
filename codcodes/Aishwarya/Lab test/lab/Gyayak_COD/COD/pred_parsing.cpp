#include <bits/stdc++.h>
#define REP(i,a,b) for (int i = a; i <= b; ++i)
#define TRvi(c,it) for (auto it = c.begin(); it != c.end(); it++)
#define mp make_pair
#define fi first
#define se second

/*Sample Input
5
E 1
TX
X 2
+TX ?
T 1
FY
Y 2
*FY ?
F 2
(E) a
*/


using namespace std;

struct srt
{
    int nop;
    char ch;
    set <string> prod;
} symbol[100];

map <char, set <char> > first, follow, vv;
map <char, int> has;
map <pair <char, char>, set <string> > M;

set <char> fnc(char sym)
{
    if (first.find(sym) != first.end())
        return first[sym];

    if (sym <= 'A' || sym >= 'Z') {
        first[sym].insert(sym);

        return first[sym];
    }

    int idx = has[sym];

    set <char> tf, ts;
    bool flag;
    char ch;

    TRvi (symbol[idx].prod, it) {
        REP (i, 0, (*it).size() - 1) {
            ch = (*it)[i];

            if (ch == '?' && (*it).size() == 1) {
                tf.insert('?');

                continue ;
            }

            if (first.find(ch) == first.end())
                fnc(ch);

            ts = fnc(ch);

            flag = 0;
            TRvi (ts, it2) {
                if (*it2 == '?') {
                    flag = 1;

                    continue ;
                }

                tf.insert(*it2);
            }

            if (!flag)
                break ;
        }

        if (flag)
            tf.insert('?');
    }

    first[sym] = tf;

    return tf;
}

int main()
{
    int n, i, j, flag, lp = 5;
    string s;
    char ch, A, B;

    cin >> n;

    for (i = 1; i <= n; i++) {
        cin >> ch;

        cin >> symbol[i].nop;

        cout << symbol[i].nop << " " << ch;

        symbol[i].ch = ch;
        has[ch] = i;
        //rhas[i] = s;

        for (j = 0; j < symbol[i].nop; j++) {
            cin >> s;

            REP (k, 0, s.size() - 1) {
                if ((s[k] <= 'A' || s[k] >= 'Z') && s[k] != '?' && first.find(s[k]) == first.end())
                    first[s[k]].insert(s[k]);
            }

            symbol[i].prod.insert(s);
        }
    }

    REP (i, 1, n) {
        if (first.find(symbol[i].ch) == first.end())
            fnc(symbol[i].ch);
    }

    TRvi (first, it) {
        cout << "First ( " << it->first << " ) = { ";

        TRvi (it->second, it2) {
            cout << *it2 << " ";
        }
        cout << "}\n";
    }

    follow[symbol[1].ch].insert('$');
    while (lp--) {
        REP (i, 1, n) {
            A = symbol[i].ch;
            TRvi (symbol[i].prod, it) {
                B = (*it)[(*it).size() - 1];

                if (B != A && B >= 'A' && B <= 'Z') {
                    TRvi (follow[A], it2) {
                        follow[B].insert(*it2);
                    }
                }

                if ((*it).size() == 1)
                    continue ;

                ch = B;
                B = (*it)[(*it).size() - 2];

                //cout << A << " " << B << " " << ch << endl;
                if (B != A && B >= 'A' && B <= 'Z') {
                    flag = 1;
                    TRvi (first[ch], it2) {
                        if (*it2 == '?') {
                            flag = 0;

                            continue ;
                        }

                        follow[B].insert(*it2);
                    }

                    if (!flag) {
                        follow[B].insert(follow[A].begin(), follow[A].end());
                    }
                }
            }
        }
    }


    /*TRvi (vv, it) {
        cout << it->first << " " << (it->second).size() << endl;
        TRvi (it->second, it2) {
            follow[it->first].insert(follow[*it2].begin(), follow[*it2].end());
        }
    }*/

    TRvi (follow, it) {
        cout << "Follow ( " << it->first << " ) = { ";
        TRvi (it->second, it2)
            cout << *it2 << " ";
        cout << "}\n";
    }


    REP (i, 1, n) {
        A = symbol[i].ch;

        TRvi (symbol[i].prod, it) {
            ch = (*it)[0];

            flag = 0;

            if (ch == '?') {
                flag = 1;

                goto l1;
            }

            TRvi (first[ch], it2) {
                if (*it2 == '?') {
                    flag = 1;

                    continue ;
                }

                M[mp(A, *it2)].insert(*it);
            }

            l1:
            if (flag) {
                flag = 0;
                TRvi (follow[A], it2) {
                    if (*it2 == '$') {
                        flag = 1;

                        continue ;
                    }

                    M[mp(A, *it2)].insert(*it);
                }

                if (flag)
                    M[mp(A, '$')].insert(*it);
            }
        }
    }

    TRvi (M, it) {
        cout << "M[ " << (it->fi).fi << ", " << (it->fi).se << " ] = ";
        TRvi (it->second, it2)
            cout << (it->fi).fi << " -> " << *it2 << "  ";
        cout << endl;
    }

    return 0;
}
