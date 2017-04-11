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
map<char, set<char> > first, follow;
set<char> nTerm, Term;

bool non_terminal(char x)
{
    return (x >= 'A' && x <= 'Z');
}

void first_c(char x)
{
    if(!non_terminal(x)) {
        first[x].insert(x);
        return;
    }

    if(rule[x].find("?") != rule[x].end())
        first[x].insert('?');

    string s;
    int i;
    Rep(it, rule[x]) {
        s = *it;
        int f = 0;
        For(i, 0, s.size()) {
            first_c(s[i]);
            if(first[s[i]].find('?') ==  first[s[i]].end()) {
                f = 1;
            }
            Rep(it1, first[s[i]])
                if(*it1 != '?')
                    first[x].insert(*it1);
            if(f)
                break;
        }
        if(!f)
            first[x].insert('?');
    }
}

void follow_c(char x)
{
    Rep(it, rule) {
        char p = it->fi;
        Rep(it1, rule[p]) {
            string s = *it1;
            int i,j;
            For(i, 0, s.size()) {
                if(s[i] == x) {
                    int f = 0;
                    For(j, i+1, s.size()) {

                        if(first[s[j]].find('?') == first[s[j]].end())
                            f = 1;
                        Rep(it2, first[s[j]])
                            if(*it2 != '?')
                                follow[x].insert(*it2);
                        if(f)
                            break;
                    }
                    if(!f && (x != p)) {
                        if(follow[p].empty())
                            follow_c(p);
                        Rep(it2, follow[p])
                            follow[x].insert(*it2);
                    }
                }
            }
        }
    }
}

void print_first()
{
    cout << "First - \n";
    Rep(it, first) {
        cout << it->fi << " - {";
        int cnt = (it->se).size() - 1;
        Rep(it1, it->se) {
            cout << *it1;
            if(cnt--)
                cout << ", ";
        }
        cout << "}\n";
    }
    cout << "\n";
}

void print_follow()
{
    cout << "Follow - \n";
    Rep(it, follow) {
        cout << it->fi << " - {";
        int cnt = (it->se).size() - 1;
        Rep(it1, it->se) {
            cout << *it1;
            if(cnt--)
                cout << ", ";
        }
        cout << "}\n";
    }
    cout << "\n";
}

map<pair<char, char>, pair<char, string> > ll1;

void calc_ll1()
{
    int i;
    Rep(it, rule) {
        char x = it->fi;
        Rep(it1, rule[x]) {
            string s = *it1;
            int f = 0;
            For(i, 0, s.size()) {
                Rep(it2, first[s[i]]) {
                    if(*it2 != '?')
                        ll1[mp(x,*it2)] = mp(x, s);
                }

                if(first[s[i]].find('?') == first[s[i]].end()) {
                    f = 1;
                    break;
                }
             }
             if(!f) {
                Rep(it2, follow[x])
                    ll1[mp(x, *it2)] = mp(x, s);
             }
        }
    }
}

void print_ll1()
{
    Rep(it, ll1) {
        cout << "m[" << (it->fi).fi << ", " << (it->fi).se << "] = {" << (it->se).fi << " -> " << (it->se).se << "}" << endl;
    }
    cout << endl;
}

bool parser_match(string s)
{
    stack<char> st;
    st.push('$');
    st.push('S');
    s = s + '$';

    int j,i = 0, n = s.size();

    while(i < n) {
        if(st.empty())
            return false;
      //  cout << st.top() << " " << s[i] << endl;
        if(s[i] == '$' && st.top() == '$')
            return true;

        if(!non_terminal(st.top())) {
            if(st.top() != s[i])
                return false;
            st.pop();
            i++;
            continue;
        }

        if(ll1.find(mp(st.top(), s[i])) == ll1.end())
            return false;

        string p = ll1[mp(st.top(), s[i])].se;
        st.pop();

        //cout << "  " << p << endl;
        if(p == "?")
            continue;
        Ford(j, p.size()-1, 0)
            st.push(p[j]);
    }
    if(st.size() >= 2)
        return false;
    if(st.empty())
        return false;
    if(st.top() == s[n-1] && s[n-1] == '$')
        return true;
    return false;
}

int main()
{
    //freopen("temp.txt", "r", stdin);
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

    Rep(it, nTerm)
        first_c(*it);
    Rep(it, Term)
        first_c(*it);

    print_first();

    follow['S'].insert('$');
    follow_c('S');
    Rep(it, nTerm)
        if(follow[*it].empty())
            follow_c(*it);
    print_follow();

    calc_ll1();
    print_ll1();

    cout << "Enter number of Queries : ";
    int q;
    cin >> q;

    while(q--) {
        string s;
        cin >> s;
        cout << s.size() << endl;
        if(parser_match(s))
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
