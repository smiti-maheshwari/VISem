#include <bits/stdc++.h>
using namespace std;

map< char, vector< string > > grammar;

map < char , set < char > > first;
map < char , set < char > > follow;

int symbols;
char startSym[100];

inline void print(char ch) {
    vector<string > temp = grammar[ch];
    for(int i = 0; i < temp.size(); ++i) cout << temp[i] << " "; cout << endl;
}

bool terminal(char ch) {
    if (ch >= 'A' && ch <= 'Z')
        return false;
    return true;
}

int find_all(int arr[], string str, char sym) {
    int c = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == sym) {
            arr[c++] = i;
        }
    }

    return c;
}

void computeFirst(char sym) {
    vector < string > temp = grammar[sym];

    for (int i = 0; i < temp.size(); i++) {
        string str = temp[i];
        for (int j = 0; j < str.length(); j++) {
            if (terminal(str[j])) {
                first[sym].insert(str[j]);
                break;
            } else {
                computeFirst(str[j]);
                set < char > tmp = first[str[j]];
                set < char > :: iterator it;
                it = tmp.begin();
                bool fg = true;
                while (it != tmp.end()) {
                    char ch = *it;
                    if (ch == 'e') {
                        fg = false;
                    } else
                        first[sym].insert(ch);
                    it++;
                }
                if (fg)
                    break;
                if (!fg && j == str.length() - 1) {
                    first[sym].insert('e');
                }
            }
        }
    }
}

void computeFollow(char sym) {
    vector <string > temp;
    int arr[100];
    for(int i = 0; i < symbols; ++i) {
        temp = grammar[startSym[i]];
        for(int j = 0; j < temp.size(); ++j) {
            string str = temp[j];
            int c = find_all(arr, str, sym);
            if (c == 0)
                continue;
            int l = str.length();
            for (int p = 0; p < c; p++) {
                if (arr[p] == l - 1) {
                    if (sym != startSym[i]) {
                        computeFollow(startSym[i]);
                        set<char > followTemp = follow[startSym[i]];
                        set < char > :: iterator it = followTemp.begin();
                        while (it != followTemp.end()) {
                            follow[sym].insert(*it);
                            it++;
                        }
                    }
                } else {
                    int ind = arr[p];
                    while (ind < l) {
                        ind++;
                        set<char > firstTemp;
                        if(terminal(str[ind])) {
                            follow[sym].insert(str[ind]);
                            break;
                        }

                        firstTemp = first[str[ind]];
                        set < char > :: iterator it = firstTemp.begin();
                        bool flag = 0;

                        while(it != firstTemp.end()) {
                            char ch = *it;
                            if(ch == 'e') {
                                flag = 1;
                            } else {
                                follow[sym].insert(ch);
                            }
                            it++;
                        }

                        if (!flag)
                            break;

                        if (ind == l - 1) {
                            if (sym != startSym[i]) {
                                computeFollow(startSym[i]);
                                set<char > followTemp = follow[startSym[i]];
                                set < char > :: iterator it = followTemp.begin();
                                while (it != followTemp.end()) {
                                    follow[sym].insert(*it);
                                    it++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


inline void goFirst() {
    for (int i = 0; i < symbols; i++) {
        computeFirst(startSym[i]);
        set < char > tmp = first[startSym[i]];
        set < char > :: iterator it;

        cout << startSym[i] << " ---> ";
        it = tmp.begin();
        while (it != tmp.end()) {
            char ch = *it;
            cout << ch << " ";
            it++;
        }
        cout << endl;
    }
}

void goFollow() {
    follow[startSym[0]].insert('$');
    cout << endl;
    for(int i = 0; i < symbols; ++i) {
        computeFollow(startSym[i]);
        set < char > tmp = follow[startSym[i]];
        set < char > :: iterator it;

        cout << startSym[i] << " ---> ";
        it = tmp.begin();
        while (it != tmp.end()) {
            char ch = *it;
            if ((int) ch != 0)
            cout << ch << " ";
            it++;
        }
        cout << endl;
    }
}

int main() {
    cin >> symbols;

    for(int i = 0; i < symbols; ++i) {
        cin >> startSym[i];

        int productions;
        cin >> productions;
        for(int j = 0; j < productions; ++j) {
            string str;
            cin >> str;

            grammar[startSym[i]].push_back(str);
        }
    }
    goFirst();

    /////////////////////////
    goFollow();
    return 0;
}
/*
5
E 1
TA
A 2
+TA e
T 1
FB
B 2
*FB e
F 2
(E) x


6
S 1
ABCDE
A 2
a e
B 2
b e
C 1
c
D 2
d e
E 2
x e

3
S 2
Bb Cd
B 2
aB e
C 2
cC e

4
S 3
ACB CbB Ba
A 2
da BC
B 2
g e
C 2
h e

3
S 1
aABb
A 2
c e
B 2
d e

6
S 1
aBDh
B 1
cC
C 2
bC e
D 1
EF
E 2
g e
F 2
f e

2
S 2
A a
A 1
a
*/
