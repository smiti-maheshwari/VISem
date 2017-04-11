#include <bits/stdc++.h>
#define pb push_back

using namespace std;

struct node
{
    int no;
    int type;
    bool nul;
    set <int> fpos;
    set <int> lpos;
    node *left;
    node *right;
};

typedef set <int> state;

vector <state> vs;
map <state, state> trans[10];
map <state, int> vis;
map <char, int> has;

int cnt = 1;
stack <char> st1;
stack <node*> st;
set <int> fol_pos[100];
set <int> ::iterator it, it2;

node * create_node(int lf, int tp, int nl)
{
    node *tmp = (node *)malloc(sizeof(struct node));

    tmp->no = lf;
    tmp->type = tp;
    tmp->nul = nl;
    tmp->fpos.clear();
    tmp->lpos.clear();
    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

void traverse(node * nn)
{
    if (nn->left != NULL)
        traverse(nn->left);

    cout << "--------------------\n";
    cout << "Leaf = " << nn->no << endl;
    cout << "Operator = " << nn->type << endl;
    cout << "Nullable = " << nn->nul << endl;
    cout << "firstpos = { ";
    for (it = nn->fpos.begin(); it != nn->fpos.end(); it++)
        cout << *it << " ";
    cout << "}\n";
    cout << "lastpos = { ";
    for (it = nn->lpos.begin(); it != nn->lpos.end(); it++)
        cout << *it << " ";
    cout << "}\n";

    if (nn->right != NULL)
        traverse(nn->right);
}

int main()
{
    string s, s1, tmp;
    int i;

    cin >> s1;

    for (i = 0; i < s1.size() - 1; i++) {
        if (s1[i] >= 'a' && s1[i] <= 'z') {
            s.pb(s1[i]);

            if ((s1[i + 1] >= 'a' && s1[i + 1] <= 'z') || s1[i + 1] == '#' || s1[i + 1] == '(')
                s.pb('.');
        }
        else if (s1[i] == '(' || s1[i] == ')') {
            s.pb(s1[i]);


            if (s1[i] == ')' && ((s1[i + 1] >= 'a' && s1[i + 1] <= 'z') || s1[i + 1] == '#'))
                s.pb('.');
        }

        else if (s1[i] == '*') {
            s.pb(s1[i]);
            s.pb('.');
        }

        else
            s.pb(s1[i]);
    }
    s.pb('#');

    cout << s << endl;

    for (i = 0; i < s.size(); i++) {
        if ((s[i] >= 'a' && s[i] <= 'z') || s[i] == '#' || s[i] == '*')
            tmp.pb(s[i]);

        else if (s[i] == ')') {
            while (!st1.empty() && st1.top() != '(') {
                tmp.pb(st1.top());
                st1.pop();
            }

            if (!st1.empty())
                st1.pop();
        }

        else if (s[i] == '(')
            st1.push('(');

        else {
            while (!st1.empty() && st1.top() != '(')
                 tmp.pb(st1.top()), st1.pop();

            if (!st1.empty())
                st1.pop();

            st1.push(s[i]);
        }
    }

    while (!st1.empty()) {
        tmp.pb(st1.top());
        st1.pop();
    }

    s = tmp;

    cout << s << endl;

    node *nn, *n1, *n2;

    for (i = 0; i < s.size() ; i++) {
        if ((s[i] >= 'a' && s[i] <= 'z') || s[i] == '#') {
            nn = create_node(cnt, -1, 0);

            has[cnt] = s[i];
            nn->fpos.insert(cnt);
            nn->lpos.insert(cnt);
            cnt++;

            st.push(nn);
        }

        else if (s[i] == '*') {
            nn = create_node(-1, 1, 1);

            n1 = st.top();
            st.pop();

            nn->fpos = n1->fpos;
            nn->lpos = n1->lpos;

            nn->left = n1;
            nn->right = NULL;

            for (it = nn->lpos.begin(); it != nn->lpos.end(); it++) {
                for (it2 = nn->fpos.begin(); it2 != nn->fpos.end(); it2++)
                    fol_pos[*it].insert(*it2);
            }

            st.push(nn);
        }

        else if (s[i] == '|') {
            nn = create_node(-1, 2, 0);

            n2 = st.top();
            st.pop();
            n1 = st.top();
            st.pop();

            nn->nul = n1->nul | n2->nul;

            for (it = n1->fpos.begin(); it != n1->fpos.end(); it++)
                nn->fpos.insert(*it);

            for (it = n2->fpos.begin(); it != n2->fpos.end(); it++)
                nn->fpos.insert(*it);


            for (it = n1->lpos.begin(); it != n1->lpos.end(); it++)
                nn->lpos.insert(*it);

            for (it = n2->lpos.begin(); it != n2->lpos.end(); it++)
                nn->lpos.insert(*it);

            nn->left = n1;
            nn->right = n2;

            st.push(nn);
        }

        else if (s[i] == '.') {
            nn = create_node(-1, 3, 0);

            n2 = st.top();
            st.pop();
            n1 = st.top();
            st.pop();

            nn->nul = n1->nul & n2->nul;

            nn->fpos = n1->fpos;
            nn->lpos = n2->lpos;

            if (n1->nul) {
                for (it = n2->fpos.begin(); it != n2->fpos.end(); it++)
                    nn->fpos.insert(*it);
            }

            if (n2->nul) {
                for (it = n1->lpos.begin(); it != n1->lpos.end(); it++)
                    nn->lpos.insert(*it);
            }

            for (it = n1->lpos.begin(); it != n1->lpos.end(); it++) {
                for (it2 = n2->fpos.begin(); it2 != n2->fpos.end(); it2++)
                    fol_pos[*it].insert(*it2);
            }

            nn->left = n1;
            nn->right = n2;

            st.push(nn);
        }
    }

    node *root = st.top();
    st.pop();

    assert(st.empty());

    traverse(root);

    cout << "Followpos:\n";
    for (i = 0; i < cnt; i++) {
        cout << endl << "Followpos of " << i << ":\n";
        for (it = fol_pos[i].begin(); it != fol_pos[i].end(); it++)
            cout << *it << " ";
    }

    int nos = 1;
    state ts;

    ts = root->fpos;

    stack <state> ss;
    set <int> u;
    string sigma = "ab";

    ss.push(ts);
    while (!ss.empty()) {
        ts = ss.top();
        ss.pop();

        //cout << "hi " << endl;

        if (vis[ts])
            continue ;

        vis[ts] = 1;

        cout << "State: { ";
        for (it = ts.begin(); it != ts.end(); it++)
            cout << *it << " ";
        cout << "}\nTransitions:\n";

        for (i = 0; i < sigma.size(); i++) {
            u.clear();
            for (it = ts.begin(); it != ts.end(); it++) {
                if (has[*it] == sigma[i]) {
                    for (it2 = fol_pos[*it].begin(); it2 != fol_pos[*it].end(); it2++)
                        u.insert(*it2);
                }
            }

            if (u.size() && vis[u] == 0) {
                state ns;

                ns = u;
                ss.push(u);
            }

            cout << sigma[i] << " -> { ";
            for (it = u.begin(); it != u.end(); it++)
                cout << *it << " ";
            cout << "}\n";
            trans[sigma[i] - 'a'][ts] = u;
        }
    }



    return 0;
}
