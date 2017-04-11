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
set<char> nTerm, Term, sym;

#define item set<pair<char, string> >

map< pair<item, char >, pair<char, item > > slr1;
map< pair<int, char >, pair<char, int> > slr1_tab;
map<char, item > closure;
map<item, int > itemid;
map<pair<char, string>,int >red_has;
map<int, pair<char, string> > ired_has;
int id = 1, red_id = 1;
set< item > vis;
map<char, int> Fchar;
map<int, char> iFchar;

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
            if(s[i] == x) {
                f = 1;
                break;
            }
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

void calc_closure(char x)
{
    Rep(it, rule[x]) {
        string s = *it;
        closure[x].insert(mp(x, "." + s));

        if(non_terminal(s[0]) && s[0] != x) {
            calc_closure(s[0]);
            Rep(it1, closure[s[0]])
                closure[x].insert(*it1);
        }
    }
}

void print_closure()
{
    Rep(it, nTerm) {
        Rep(it1, closure[*it]) {
            cout << (*it1).fi << " " << (*it1).se << endl;
        }
        cout << endl;
    }
}

int find_pos(string s)
{
    int i;

    For(i, 0, s.size())
        if(s[i] == '.')
            return i;
}

string nextString(string s, int pos)
{
    int i;

    s.erase(s.begin() + pos);
    s.insert(s.begin() + pos + 1, '.');

    return s;
}

void check_correct(item a, char b, char c, item d)
{
    if(slr1.find(mp(a,b)) != slr1.end() && slr1[mp(a, b)] != mp(c, d)) {
        cout << "Grammar has "<< c << "-" << slr1[mp(a,b)].fi << " conflict\n";
        exit(0);
    }
}

void calc_slr1()
{
    item st = closure['S'];
    st.insert(mp('X', ".S"));

    queue< item > qu;

    qu.push(st);
    vis.insert(st);

    while(!qu.empty()) {
        item cur = qu.front();
        qu.pop();

        itemid[cur] = id++;

        Rep(it, sym) {
            char x = *it;

            item next;

            Rep(it1, cur) {
                string s = (*it1).se;

                int pos = find_pos(s);

                if(pos < (s.size()-1) && s[pos+1] == x) {
                    string s1 = nextString(s, pos);
                    next.insert(mp((*it1).fi, s1));
                }
            }

            Rep(it1, next) {
                string s = (*it1).se;

                int pos = find_pos(s);

                if(pos < (s.size()-1) && non_terminal(s[pos+1])) {
                    char y = s[pos+1];

                    Rep(it2, closure[y])
                        next.insert(*it2);
                }
            }

            if(next.size() > 0) {
                if(non_terminal(x)) {
                    check_correct(cur, x, 'G', next);
                    slr1[mp(cur, x)] = mp('G', next);
                }
                else {
                    check_correct(cur, x, 'S', next);
                    slr1[mp(cur, x)] = mp('S', next);
                }
            }

            if(next.size() > 0 && vis.find(next) == vis.end()) {
                vis.insert(next);
                qu.push(next);
            }

            Rep(it1, cur) {
                char tmp_c = (*it1).fi;
                string tmp_s = (*it1).se;

                if(find_pos(tmp_s) == tmp_s.size()-1) {
                    tmp_s = tmp_s.substr(0, tmp_s.size()-1);
                    item tmp_i;
                    tmp_i.insert(mp(tmp_c, tmp_s));

                    Rep(it2, follow[tmp_c])
                        check_correct(cur, *it2, 'R', tmp_i), slr1[mp(cur, *it2)] = mp('R', tmp_i);
                }
            }
        }
    }
}

bool check_slr1()
{
    cout << "itemset id --- \n";
    Rep(it, vis) {
        item t1 = *it;
        int cnt = t1.size()-1;
        cout << "{";
        Rep(it1, t1) {
            cout << "(" << (*it1).fi << "," << (*it1).se << ")";
            if(cnt--)
                cout << ", ";
        }
        cout << "} -> " << itemid[t1] << endl;
    }

    cout << endl << endl;
    cout << "Rule id --\n";

    Rep(it, nTerm) {
        char x = *(it);

        Rep(it1, rule[x]) {
            cout << "(" << x << ", " << *it1 << ") - " << red_has[mp(x, *it1)] << endl;
        }
    }
    cout << endl << endl;
    return true;

}

void gen_slr1_table()
{
    Rep(it, slr1) {
        char x1, x2;
        item t1, t2;

        t1 = (it->fi).fi;
        x1 = (it->fi).se;
        t2 = (it->se).se;
        x2 = (it->se).fi;

        if(x2 == 'R') {
            if((*(t2.begin())).fi == 'X') {
                slr1_tab[mp(itemid[t1], '$')] = mp('&', 1000);
                continue;
            }
                string tmp_s = (*(t2.begin())).se;
                char tmp_c = (*(t2.begin())).fi;
                slr1_tab[mp(itemid[t1], x1)] = mp(x2, red_has[mp(tmp_c, tmp_s)]);
        } else {
            slr1_tab[mp(itemid[t1], x1)] = mp(x2, itemid[t2]);
        }
    }
}

void print_slr1()
{
    int t1, t2;
    char x1, x2;
    Rep(it, slr1_tab) {
        t1 = (it->fi).fi;
        x1 = (it->fi).se;
        t2 = (it->se).se;
        x2 = (it->se).fi;
        cout << "{" <<t1 << ", " << x1 << "} -> {" <<x2<<", "<< t2 << "}" << endl;

    }
}

void print_stack(stack<int> st)
{
    while(!st.empty()) {
        if(st.top() >= 100)
            cout << iFchar[st.top()] << " ";
        else
            cout << st.top() << " ";
        st.pop();
    }
    cout << endl;
}

bool slr1_parser(string s)
{
    s = s + '$';
    stack<int> st;
    st.push(1);

    int i = 0;
    while(i < s.size()) {
        //cout << s[i] << " - ";
        //print_stack(st);
        if(slr1_tab.find(mp(st.top(), s[i])) == slr1_tab.end())
            return false;

        if(slr1_tab[mp(st.top(), s[i])] == mp('&', 1000))
            return true;
        pair<char, int> cur = slr1_tab[mp(st.top(), s[i])];

        if(cur.fi == 'R') {
            string tmp_s = ired_has[cur.se].se;
            char tmp_c = ired_has[cur.se].fi;

            int x = 2*tmp_s.size();
            while(x--)
                st.pop();

            int cur_e = st.top();
            if(slr1_tab.find(mp(cur_e, tmp_c)) == slr1_tab.end())
                return false;

            st.push(Fchar[tmp_c]);
            st.push(slr1_tab[mp(cur_e, tmp_c)].se);
        }
        else if(cur.fi == 'S') {
            int cur_e = st.top();
            st.push(Fchar[s[i]]);
            st.push(slr1_tab[mp(cur_e, s[i])].se);
            i++;
        }
    }
}

int main()
{
    //freopen("temp.txt", "r", stdin);
    int i,t;
    //cout << "Enter number of rules: \n";
    scanf("%d", &t);
    //cout << "\nEnter grammer: \n";

    while(t--) {
        string s;
        char x;
        cin >> x >> s;

        red_has[mp(x, s)] = red_id;
        ired_has[red_id++] = mp(x, s);

        if(non_terminal(x))
            nTerm.insert(x);
        else
            Term.insert(x);
        sym.insert(x);

        For(i, 0, s.size()) {
            if(non_terminal(s[i]))
                nTerm.insert(s[i]);
            else
                Term.insert(s[i]);
            sym.insert(s[i]);
        }

        rule[x].insert(s);
    }
    Rep(it, nTerm)
        first_c(*it);
    Rep(it, Term)
        first_c(*it);

    print_first();


    follow['X'].insert('$');
    follow['S'].insert('$');
    follow_c('S');
    Rep(it, nTerm)
        if(follow[*it].empty())
            follow_c(*it);
    print_follow();

    int xx = 100;
    Rep(it, sym) {
        Fchar[*it] = xx;
        iFchar[xx++] = *it;
    }

    Rep(it, nTerm)
        calc_closure(*it);
    //print_closure();


    calc_slr1();
    if(check_slr1()) {
        gen_slr1_table();
        print_slr1();

        cin >> t;
        while(t--) {
            string inp;
            cin >> inp;

            if(slr1_parser(inp))
                cout << "Accepted\n";
            else
                cout << "Not Accepted\n";
        }
    }
    return 0;
}
