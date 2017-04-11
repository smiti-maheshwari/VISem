//Done by: K Ashwin

#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

#define REP(i, a, b) \
for (int i = int(a); i <= int(b); i++) // a to b, and variable i is local!
#define TRvi(c, it) \
for (auto it = (c).begin(); it != (c).end(); it++)

#define s(x) scanf("%d", &x)
#define sl(x) scanf("%lld", &x)
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define set0(a) memset(a, 0, sizeof(a))
#define setdp(a) memset(a, -1, sizeof(a))
#define INF 2000000000
#define MOD 1000000007

/*
Sample Input 1:
3
E 2
E+T
T
T 2
T*F
F
F 2
(E)
a

Sample Input 2:
4
A 2
Bxy
x
B 1
CD
C 2
A
c
D 1
d
*/

struct srt
{
    int nop;
    set <string> prod;
} symbol[30];

int has[30];
set <string> s1, s2;
set <string> ::iterator it, it2;

int main()
{
    int n, idx;
    char ch;
    string s;

    s(n);

    REP (i, 1, n) {
        cin >> ch;

        cin >> symbol[ch - 'A'].nop;

        has[ch - 'A'] = 1;
        REP (j, 1, symbol[ch - 'A'].nop) {
            cin >> s;

            symbol[ch - 'A'].prod.insert(s);
        }
    }

    REP (i, 0, 25) {
        if (!has[i])
            continue ;

        REP (j, 0, 25) {
            if (i == j || !has[j])
                continue ;

            cout << i << " " << j << endl;

            s1.clear();
            s2.clear();
            for (it = symbol[i].prod.begin(); it != symbol[i].prod.end(); it++) {
                //cout << " " << *it << endl;
                if ((*it).size() > 1 && (*it)[0] - 'A' == j) {
                    s = (*it).substr(1, (*it).size() - 1);

                    //cout << s << " suf" << endl;
                    for (it2 = symbol[j].prod.begin(); it2 != symbol[j].prod.end(); it2++)
                        s2.insert((*it2) + s);

                    s1.insert(*it);
                    //it = symbol[i].prod.begin();
                }
            }

            for (it = s1.begin(); it != s1.end(); it++)
                symbol[i].prod.erase(*it);

            for (it = s2.begin(); it != s2.end(); it++)
                symbol[i].prod.insert(*it);
        }

        REP (j, 0, 25)
            if (!has[j]) {
                idx = j;

                break ;
            }

        s1.clear();
        s2.clear();
        for (it = symbol[i].prod.begin(); it != symbol[i].prod.end(); it++) {
            if ((*it)[0] - 'A' == i)
                s1.insert((*it).substr(1, (*it).size() - 1));

            else
                s2.insert((*it));
        }

        if (s1.size() == 0)
            continue ;

        cout << "Yay\n";

        has[idx] = 1;
        symbol[i].prod.clear();
        for (it = s2.begin(); it != s2.end(); it++) {
            symbol[i].prod.insert((*it) + (char)(idx + 'A'));
        }

        symbol[idx].prod.insert("?");
        for (it = s1.begin(); it != s1.end(); it++) {
            symbol[idx].prod.insert((*it) + (char)(idx + 'A'));
        }
    }

    REP (i, 0, 25) {
        if (has[i]) {
            cout << (char)(i + 'A') << " -> ";

            for (it = symbol[i].prod.begin(); it != symbol[i].prod.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
    }

    return 0;
}
