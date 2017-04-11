/**
Prashant Gupta(GHOST_YO)
IIITA
**/
#include <bits/stdc++.h>
using namespace std;
#define For(i,a,b) for(i=a;i<b;i++)
#define Ford(i,a,b) for(i=a;i>=b;i--)
//#define Rep(i,c) for(__typeof(c.begin()) it(c.begin());i!=c.end();i++)
#define Rep(i,c) for(auto i=c.begin();i!=c.end();i++)
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define modul 1000000007
#define infi 999999999999999999.0
#define BUG(x) {cout<<#x<<" = "<<x<<endl;}
#define PR(x,a,b) {For(i,a,b+1)cout<<x[i]<<' ';cout<<endl;}
#define FILL(a,x) memset(a,x,sizeof(a));
#define sc(a) scanf("%d", &a)
#define pii pair<int,int>
#define ll long long
#define PI acos(-1)
#define gc getchar
#define pc putchar
#define TIN {cout<<"Time started\n";time_t __t_v=clock();
#define TOUT cout<<"Time Taken : "<<(clock()-__t_v)/(double)CLOCKS_PER_SEC<<"\n";}

inline void scanint(int &x){register int c=0;x=0;int flag=0;for(;((c!=45)&&(c<48||c>57));c=gc());
for(;((c==45)||(c>47&&c<58));c=gc()){if(c==45)flag=1;else x=(x<<1)+(x<<3)+c-48;}if(flag)x=-x;}

map<char, set<string> > rule;
set<char> nTerm, Term, startSymbol;
map<pair<int, pair<int, char> >, bool>P, eP;

bool non_terminal(char x)
{
    return (x >= 'A' && x <= 'Z');
}

void cyk(string s)
{
    P = eP;
    int i,j,k,l;

    For(i, 0, s.size()) {
        Rep(it, nTerm) {
            string t1 = "";
            t1 = t1 + s[i];
            if(rule[*it].find(t1) != rule[*it].end())
                P[mp(1, mp(i, *it))] = true;
        }
    }
    //cout << P[mp(1, mp(0, 'A'))] << endl;
    For(i, 2, s.size()+1) {
        For(j, 0, s.size()-i+1) {
            For(k, 1, i) {
                Rep(it, nTerm) {
                    Rep(it1, rule[*it]) {
                        string x = *it1;
                        if(x.size() == 2) {
                            if(P[mp(k, mp(j, x[0]))] && P[mp(i-k,mp(j+k,x[1]))])
                                P[mp(i, mp(j, *it))] = true;
                        }
                    }
                }
            }
        }
    }

    Rep(it, nTerm) {
       // cout << "*it " << *it << endl;
        if(P[mp(s.size(), mp(0, *it))]) {
            cout << "YES\n";
            return;
        }
    }
    cout << "NO\n";
}

int main()
{
    //freopen("cyk.txt", "r", stdin);
    int i,t;
    cout << "Enter number of rules: \n";
    scanf("%d", &t);
    cout << "\nEnter grammer: \n";

    while(t--) {
        string s;
        char x;
        cin >> x >> s;

        if(non_terminal(x))
            nTerm.insert(x);
        else
            Term.insert(x);
        For(i, 0, s.size()) {
            if(non_terminal(s[i]))
                nTerm.insert(s[i]);
            else
                Term.insert(s[i]);
        }

        rule[x].insert(s);
    }
    int q;
    cout << "Enter Number of Start Symbols : ";
    cin >> q;
    while(q--) {
        char x;
        cin >> x;
        startSymbol.insert(x);
    }

    cout << "Enter number of Queries : ";

    cin >> q;

    while(q--) {
        string inp;
        cin >> inp;

        cyk(inp);
    }

    return 0;
}


/*
2
A  a
B  b
ab
*/

/*
8
S AB
S BC
A BA
A a
B CC
B b
C AB
C a
baaba

*/

/*
16
S EF
S AF
S EB
S AB
X AY
X BY
X a
X b
Y AY
Y BY
Y a
Y b
E AX
F BX
A a
B b


3
S AB
A a
B b

S->AB
A->a
B->b

7
S 4
EF AF EB AB
X 4
AY BY a b
Y 4
AY BY a b
E 1
AX
F 1
BX
A 1
a
B 1
b



*/
