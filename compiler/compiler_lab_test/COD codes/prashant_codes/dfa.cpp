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

string s;

struct node
{
    char c;
    bool nullable;
    set<int> firstpos, lastpos;
    int lchild, rchild, id;
} tree[10000];

int headId;
set<int> followpos[10000];

bool isOperand(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '#';
}

struct node makeNode(char tmp, int idx, int l, int r, int cnt)
{
    //cout << tmp << " " << idx << " " << l << " " << r << " " << cnt << endl;
    tree[idx].c = tmp;
    tree[idx].lchild = l;
    tree[idx].rchild = r;
    tree[idx].id = idx;

    if(l == 0 && r == 0) {
        tree[idx].nullable = false;
        tree[idx].firstpos.insert(cnt);
        tree[idx].lastpos.insert(cnt);
    } else if(tmp == '*') {
        tree[idx].nullable = true;
        tree[idx].firstpos = tree[l].firstpos;
        tree[idx].lastpos = tree[l].lastpos;
    } else if(tmp == '.') {
        tree[idx].nullable = tree[l].nullable & tree[r].nullable;
        tree[idx].firstpos = tree[l].firstpos;
        if(tree[l].nullable)
            Rep(it, tree[r].firstpos)
                tree[idx].firstpos.insert(*it);
        tree[idx].lastpos = tree[r].lastpos;
        if(tree[r].nullable) {
            Rep(it, tree[l].lastpos)
                tree[idx].lastpos.insert(*it);
        }
    } else if(tmp == '|') {
        tree[idx].nullable = tree[l].nullable | tree[r].nullable;
        tree[idx].firstpos = tree[l].firstpos;
        Rep(it, tree[r].firstpos)
            tree[idx].firstpos.insert(*it);
        tree[idx].lastpos = tree[l].lastpos;
        Rep(it, tree[r].lastpos)
            tree[idx].lastpos.insert(*it);
    }

    return tree[idx];
}

void build_tree()
{
    stack<node> st;

    node t1, t2;
    int i;
    int cnt = 1, idx = 1;
    For(i, 0, s.size()) {
        if(isOperand(s[i])) {
            st.push(makeNode(s[i], idx++, 0, 0, cnt++));
        } else if(s[i] == '*') {
            t1 = st.top();
            st.pop();
            st.push(makeNode(s[i], idx++, t1.id, 0, 0));
        } else {
            t1 = st.top(), st.pop();
            t2 = st.top(), st.pop();

            st.push(makeNode(s[i], idx++, t2.id, t1.id, 0));
        }
    }
    headId = st.top().id;
}

void calc_follow(node cur)
{
    int l = cur.lchild, r = cur.rchild;
    if(cur.c == '.') {
        Rep(it, tree[l].lastpos) {
            Rep(it1, tree[r].firstpos)
                followpos[*it].insert(*it1);
        }
    }
    else if(cur.c == '*') {
        Rep(it, cur.lastpos) {
            Rep(it1, cur.firstpos)
                followpos[*it].insert(*it1);
        }
    }
    if(cur.lchild)
        calc_follow(tree[cur.lchild]);
    if(cur.rchild)
        calc_follow(tree[cur.rchild]);
}

set<set<int> > vis;
map<int, char> fn;
map<pair<set<int>,char>, set<int> > trans;
set<char> inp;
map<set<int>, int> setid;
int setcnt = 1;

void build_dfs()
{
    stack<set<int> > stk;

    stk.push(tree[headId].firstpos);

    while(!stk.empty()) {
        vis.insert(stk.top());

        set<int> cur = stk.top();
        stk.pop();

        Rep(it, inp) {
            set<int> U;
            Rep(it1, cur) {
                if(fn[*it1] == *it) {
                    Rep(it2, followpos[*it1]) {
                        U.insert(*it2);
                    }
                }
            }
            if(!U.empty() && vis.find(U) == vis.end())
                stk.push(U);
            trans[mp(cur, *it)] = U;
        }
    }
}

void printTrans()
{
    Rep(it, trans) {
        if(setid.find((*it).fi.fi) == setid.end())
            setid[(*it).fi.fi] = setcnt++;
        if(setid.find((*it).se) == setid.end())
            setid[(*it).se] = setcnt++;
    }

    Rep(it, trans) {
        cout << "[" << setid[(*it).fi.fi] << ", " << (*it).fi.se << "] -> " << setid[(*it).se] << endl;;
    }
}

void preorder(node cur)
{
    cout << cur.c << " - ";
    cout << "First: ";
    Rep(it, cur.firstpos)
        cout << *it << " ";
    cout << ", Last: ";
    Rep(it, cur.lastpos)
        cout << *it << " ";
    cout << endl;
    if(cur.lchild)
        preorder(tree[cur.lchild]);
    if(cur.rchild)
        preorder(tree[cur.rchild]);
}

void printfollowpos(int len)
{
    cout << "follow pos\n";
    int i;
    For(i, 1, len+1) {
        cout << i << " - ";
        Rep(it, followpos[i])
            cout << *it << " ";
        cout << endl;
    }
    cout << endl;
}

void preprocess()
{
    int i;
    string ret = "";
    ret = ret + s[0];
    For(i, 1, s.size()) {
        if(s[i] == '|' || s[i] == ')' || s[i] == '*') {
            ret += s[i];
            continue;
        }
        if(s[i-1] != '|' && s[i-1] != '(')
            ret = ret + '.' + s[i];
        else
            ret = ret + s[i];
    }

    s = ret;
}

int prec(char c)
{
    switch (c) {
    case '|':
        return 1;
    case '.':
        return 2;

    case '*':
        return 3;
    }
    return -1;
}

void infixToPostfix()
{
    string ret = "";
    stack<char> st;
    int i;

    For(i, 0, s.size()) {
        if(isOperand(s[i])) {
            ret = ret + s[i];
        } else if(s[i] == '(') {
            st.push('(');
        } else if(s[i] == ')') {
            while(!st.empty() && st.top() != '(') {
                ret = ret + st.top();
                st.pop();
            }
            if(!st.empty() && st.top() != '(') {
                cout << "wa in regular expresssion\n";
                exit(0);
            }
            st.pop();
        } else {
            while(!st.empty() && prec(s[i]) <= prec(st.top())) {
                ret = ret + st.top();
                st.pop();
            }
            st.push(s[i]);
        }
    }

    while(!st.empty())
        ret = ret + st.top(), st.pop();

    s = ret + "#.";
}


int main()
{
    cin >> s;
    int len = 0, i;

    For(i, 0, s.size()) {
        if(isOperand(s[i])) {
            fn[++len] = s[i];
            inp.insert(s[i]);
        }
    }

    preprocess();

    cout << s << endl;

    infixToPostfix();

    cout << s << endl;

    build_tree();
    calc_follow(tree[headId]);

    preorder(tree[headId]);
    cout << endl;

    printfollowpos(len);
    build_dfs();
    printTrans();
    return 0;
}
