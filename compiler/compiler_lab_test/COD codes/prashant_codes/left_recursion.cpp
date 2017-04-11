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
set<char> nTerm, Term, tnTerm;

bool non_terminal(char x)
{
    return (x >= 'A' && x <= 'Z');
}

void remove_immediate(char x)
{
    int f = 0;
    Rep(it, rule[x])
        if((*it)[0] == x)
            f = 1;

    if(!f)
        return;

    char t1 = *tnTerm.begin();
    tnTerm.erase(t1);
    rule[t1].insert("?");
    set<string> tmp;
    Rep(it, rule[x]) {
        string s = *it;

        if(s[0] == x) {
            rule[t1].insert(s.substr(1, s.size()-1) + t1);
        } else {
            tmp.insert(s + t1);
        }
    }
    rule[x] = tmp;

}

int main()
{
    freopen("inp_rec1.txt", "r", stdin);
    int t,i;
    For(t, 0, 26)
        tnTerm.insert((char)(t + 'A'));

    cout << "Enter number of rules: \n";
    scanf("%d", &t);
    cout << "\nEnter grammer: \n";

    while(t--) {
        string s;
        char x;
        cin >> x >> s;

        if(non_terminal(x)) {
            nTerm.insert(x);
            if(tnTerm.find(x) != tnTerm.end())
                tnTerm.erase(x);
        }
        else
            Term.insert(x);
        For(i, 0, s.size()) {
            if(non_terminal(s[i])) {
                nTerm.insert(s[i]);
                if(tnTerm.find(x) != tnTerm.end())
                    tnTerm.erase(x);
            }
            else
                Term.insert(s[i]);
        }

        rule[x].insert(s);
    }
    ///left recursion starts from here
    Rep(it, nTerm) {

        Rep(it1, nTerm) {
            if(*it1 == *it)
                break;
            set<string> es, is;
            Rep(it2, rule[*it]) {
                string s = *it2;
                if(s[0] == *it1) {
                    es.insert(*it2);

                    Rep(it3, rule[s[0]])
                        is.insert(*it3 + s.substr(1, s.size()-1));
                }
            }
            Rep(it2, es)
                rule[*it1].erase(*it2);
            Rep(it2, is)
                rule[*it1].insert(*it2);
        }
        remove_immediate(*it);
    }

    Rep(it, rule) {
        cout << it->fi << " -> ";
        Rep(it1, it->se) {
            cout << *it1 << " | ";
        }
        cout << endl;
    }
}
