//Done by: K Ashwin

#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

#define REP(i, a, b) \
for (int i = int(a); i <= int(b); i++) // a to b, and variable i is local!
#define TR(c, it) \
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
7
S 4
EF
AF
EB
AB
X 4
AY
BY
a
b
Y 4
AY
BY
a
b
E 1
AX
F 1
BX
A 1
a
B 1
b
*/

set <string> prod[30];
bool dp[50][50][50];

int main()
{
    int n, r, nop;
    string inp, s;
    char ch, ch1, ch2;

    cin >> r;

    REP (i, 1, r) {
        cin >> ch;

        cin >> nop;

        REP (j, 1, nop) {
            cin >> s;

            prod[ch - 'A'].insert(s);
        }
    }


    cin >> inp;

    n = inp.size();

    set0(dp);

    //cout << inp << " " << n << endl;

    REP (i, 1, n) {
        REP (j, 0, 25) {
            TR (prod[j], it) {
                if ((*it).size() == 1 && (*it)[0] == inp[i - 1]) {
                    dp[1][i][j] = 1;

                    //cout << "f1\n";
                }
            }
        }
    }

    REP (i, 2, n) {
        REP (j, 1, n - i + 1) {
            REP (k, 1, i - 1) {
                REP (i1, 0, 25) {
                    TR (prod[i1], it) {
                        if ((*it).size() == 2) {
                            ch1 = (*it)[0];
                            ch2 = (*it)[1];


                            if (dp[k][j][ch1 - 'A'] && dp[i - k][j + k][ch2 - 'A']) {
                                dp[i][j][i1] = 1;

                                //cout << "f2\n";
                            }
                        }
                    }
                }
            }
        }
    }

    int flag = 0;
    REP (i, 0, 25) {
        if (dp[n][1][i])
            flag = 1;
    }

    if (flag)
        cout << "Yes\n";

    else
        cout << "No\n";

    return 0;
}
