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

map<char, set<string> > rule;
set<char> nTerm, Term, sym;

#define item set<pair<char, string> >

map< pair<item, char >, pair<char, item > > lr0;
map< pair<int, char >, pair<char, int> > lr0_tab;
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

void calc_lr0()
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
                if(non_terminal(x))
                    lr0[mp(cur, x)] = mp('G', next);
                else
                    lr0[mp(cur, x)] = mp('S', next);
            }

            if(next.size() > 0 && vis.find(next) == vis.end()) {
                vis.insert(next);
                qu.push(next);
            }

            if(next.size() == 0 && cur.size() == 1) {
                string tmp_s = (*(cur.begin())).se;
                char tmp_c = (*(cur.begin())).fi;
                tmp_s = tmp_s.substr(0, tmp_s.size()-1);
                Rep(it2, Term) {
                    item red_item;
                    red_item.insert(mp(tmp_c, tmp_s));
                    lr0[mp(cur, *it2)] = mp('R', red_item);
                }
            }
        }
    }
}

bool check_lr0()
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

    int sr = 0, rr = 0;
    Rep(it, vis) {
        item t1 = *it;
        int c1 = 0, c2 = 0;
        Rep(it1, t1) {
            int pos = find_pos((*it1).se);

            if(pos == ((*it1).se).size()-1)
                c2++;
            else
                c1++;
        }
        if(c2 > 1)
            rr = 1;
        if(c2 && c1)
            sr = 1;
    }

    if(rr)
        cout << "The Grammer has reduce-reduce conflict.\n";
    if(sr)
        cout << "The Grammer has shift-reduce conflict.\n";

    if(sr || rr)
        return false;
    return true;
}


void gen_lr0_table()
{
    Term.insert('$');

    Rep(it, lr0) {
        char x1, x2;
        item t1, t2;

        t1 = (it->fi).fi;
        x1 = (it->fi).se;
        t2 = (it->se).se;
        x2 = (it->se).fi;

        if(x2 == 'R') {
            if((*(t2.begin())).fi == 'X') {
                lr0_tab[mp(itemid[t1], '$')] = mp('&', 1000);
                continue;
            }
            Rep(it1, Term) {
                string tmp_s = (*(t2.begin())).se;
                char tmp_c = (*(t2.begin())).fi;
                //cout << "    " << tmp_c << " " << tmp_s << endl;
                lr0_tab[mp(itemid[t1], *it1)] = mp(x2, red_has[mp(tmp_c, tmp_s)]);
            }
        } else {
            lr0_tab[mp(itemid[t1], x1)] = mp(x2, itemid[t2]);
        }
    }
}

void print_lr0()
{
    int t1, t2;
    char x1, x2;
    Rep(it, lr0_tab) {
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

bool lr0_parser(string s)
{
    s = s + '$';
    stack<int> st;
    st.push(1);

    int i = 0;
    while(i < s.size()) {
        //cout << s[i] << " - ";
        //print_stack(st);
        if(lr0_tab.find(mp(st.top(), s[i])) == lr0_tab.end())
            return false;

        if(st.size() == 3 && lr0_tab[mp(st.top(), s[i])] == mp('&', 1000))
            return true;
        pair<char, int> cur = lr0_tab[mp(st.top(), s[i])];

        if(cur.fi == 'R') {
            string tmp_s = ired_has[cur.se].se;
            char tmp_c = ired_has[cur.se].fi;

            int x = 2*tmp_s.size();
            while(x--)
                st.pop();

            int cur_e = st.top();
            if(lr0_tab.find(mp(cur_e, tmp_c)) == lr0_tab.end())
                return false;

            st.push(Fchar[tmp_c]);
            st.push(lr0_tab[mp(cur_e, tmp_c)].se);
        }
        else if(cur.fi == 'S') {
            int cur_e = st.top();
            st.push(Fchar[s[i]]);
            st.push(lr0_tab[mp(cur_e, s[i])].se);
            i++;
        }
    }
}

int main()
{
    int i,t;

    scanf("%d", &t);
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

    int xx = 100;
    Rep(it, sym) {
        Fchar[*it] = xx;
        iFchar[xx++] = *it;
    }

    Rep(it, nTerm)
        calc_closure(*it);
    //print_closure();


    calc_lr0();
    if(check_lr0()) {
        gen_lr0_table();
        print_lr0();

        cin >> t;
        while(t--) {
            string inp;
            cin >> inp;

            if(lr0_parser(inp))
                cout << "Accepted\n";
            else
                cout << "Not Accepted\n";
        }
    }

}
