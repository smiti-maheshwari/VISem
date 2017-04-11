#include <bits/stdc++.h>

using namespace std;
map<char, vector <string> > prodction;
map<char, set<string> > prod3;
set <char> nonterm;
set <char> term;
int ctr = 0;
void checkcon(char noterm2, char naterm2) {
    set <string> vv = prod3[noterm2];
    int flag = 0;
    set <string> v2 = prod3[naterm2];
    set <string> toberemoved;
    //we iterate over the entire set of noterm2 to check if naterm2 is first term in any of its productions
    for(set <string>::iterator it = vv.begin(); it!= vv.end(); it++) {
        cout << "*it = " << *it << endl;
        string s= *it;
        //if naterm2 is the first term of one of noterm2's production
        if(s[0] == naterm2) {
            //the production containing naterm2 as its first term should be removed from production of noterm2 so it is added to toberemoved set
            toberemoved.insert(s);
            string tobeadded = "";
            //copy the entire production in string tobeadded ignoring the first term that is naterm2
            for(int k = 1; k < s.length(); k++) {
                tobeadded = tobeadded + s[k];
            }
            //the string tobeadded+all the productions of naterm2 is added to the production of noterm2
            for(set<string>::iterator it2 = v2.begin(); it2 != v2.end(); it2++) {
                prod3[noterm2].insert(*it2+tobeadded);
            }
        }
    }
    //remove the productions needed to be removed;
    for(set <string>::iterator it = toberemoved.begin();it!= toberemoved.end();it++) {
        set<string>::iterator it2 = prod3[noterm2].find(*it);

        prod3[noterm2].erase(it2);
    }
}
void directleftrecursion(char terminal) {
    set <string> vec = prod3[terminal]; //all the productions of nonterminal copied in vec
    set <string> alpha,beta;//A->Aalpha|beta----------> A->betaQ,Q->alphaQ|#
    set <string> toberemoved;//the productions to be replaced for ex A->Aalpha and A->beta;
    int flag = 0;
    for(set<string>::iterator i = vec.begin();i != vec.end(); i++) {
        string ss = *i;
        //if the first nonterminal of the production is equal to the nonterminal in the left side i.e production is of form A->A.alpha
        if(ss[0] == terminal) {
            
            cout << "erasing production " << terminal << "->" << *i <<endl; 
                flag++;//to keep a track of the presence of left recusion in the production
            string t = "";
            for(int j = 1; j < ss.length(); j++) {
                t = t+ss[j];
            }
            //cout << "t = " << t << endl;
            toberemoved.insert(ss);//inserting the production to be removed from the nonterminal
            alpha.insert(t);//finding all the alphas
        }
        else {
            toberemoved.insert(ss);//finding all the betas;
            beta.insert(ss);
        }
    }
    if(beta.size() == 0) {
        beta.insert("#");// if beta is empty then we insert epsilon i.e. # in beta
    }
    //if flag == 0. that means no left recursion present in the production
    if(flag == 0) {
        cout << "nodirect left recursion is present in the terminal " << terminal << endl;
        prod3[terminal] = beta;//if no left recursion present then no need to remove anything..we just insert enitre beta set in the production of the expression
    }
    else {//if left recursion is present then
        char addedterm;
        cout << "THE TERMINAL " <<terminal << " HAS LEFT RECURSION ...SO ENTER THE VARIABLE TO BE USED\n";
        //Q= addedterm
        cin >> addedterm;//we take the input of the nonterminal to be added
        if(nonterm.find(addedterm)== nonterm.end()) {//check if nonterminal is already present in the list of non terminals

           for(set <string> ::iterator it = beta.begin();it!= beta.end();it++) {
                if(beta.size() == 1 && *it == "#") {
                    //if size of beta set is only 1 and epsilon is that single element then we add A->Q to production of A
                    string s2 = "";
                    s2 = s2+addedterm;
                    prod3[terminal].insert(s2);
                    break;
                }
                //else we add the beta*addedterm to the production of A
                string ss = *it+addedterm;
                
                prod3[terminal].insert(ss);
           }
           //removing the productions needed to be removed from A
           for(set <string>::iterator it = toberemoved.begin(); it!= toberemoved.end(); it++) {
                set <string>::iterator it2= prod3[terminal].find(*it);
                prod3[terminal].erase(it2);
           }
           //for Q..Q->alphaQ|epsilon
           for(set <string>::iterator it = alpha.begin(); it!= alpha.end(); it++) {
                string newv = *it+addedterm;
                prod3[addedterm].insert(newv);
           }
           prod3[addedterm].insert("#");
        }
    }
}

void removeepsilonproduction() {
    set<char> containsepsilon;
    for(set<char>::iterator it= nonterm.begin(); it!= nonterm.end(); it++) {

        for(set<string>::iterator it2 = prod3[*it].begin(); it2!=prod3[*it].end(); it2++) {
            string ss = *it2;
            if(ss[0] == '#') {
                containsepsilon.insert(*it);
                break;
            }
        }
    }
    for(set<char>::iterator it = nonterm.begin(); it!= nonterm.end(); it++) {
        for(set<string>::iterator it2 = prod3[*it].begin();it2!= prod3[*it].end(); it2++) {
            string ss = *it2;
            string tmp = ss;
            while(true) {
                if(containsepsilon.find(tmp[0])!= containsepsilon.end()) {
                
                    string rsv="";
                    for(int i =1; i <tmp.length(); i++) {
                        rsv = rsv+tmp[i];
                    }
                    cout << "inserting productions " << *it << " - > " << rsv << endl; 
                    prod3[*it].insert(rsv);
                    tmp = rsv;
                }
                else {
                    break;
                }
            }
        }
    }
    for(set<char>::iterator it = containsepsilon.begin();it!= containsepsilon.end();it++) {
        set<string> ::iterator it2 = prod3[*it].find("#");
        prod3[*it].erase(it2);
    }
}

void removecycle() {
    cout << "AARAHA HU ME\n";
    map <char, set<char> > trackcycle;

    for(set<char>::iterator i = nonterm.begin(); i != nonterm.end(); i++) {
        for(set<string>::iterator it = prod3[*i].begin(); it!= prod3[*i].end(); it++) {
            string ss = *it;
            if(ss.length()==1) {
                if(nonterm.find(ss[0])!= nonterm.end()) {
                    trackcycle[*i].insert(ss[0]);
                }
            }
        }
    }

    for(set<char>::iterator it = nonterm.begin(); it!= nonterm.end(); it++) {
        cout << "YAHA BHI AA RHA HU ME\n";
        set<char> temp = trackcycle[*it];
        for(set<char>::iterator it2 = temp.begin(); it2 != temp.end(); it2++) {
            char w = *it2;
            set<char> temp2 = trackcycle[w];
            for(set<char>::iterator it3 = temp2.begin(); it3 != temp2.end(); it3++) {
                if(*it3 == *it) {
                    string qw = "";
                    qw = qw+w;
                    set<string>::iterator tr = prod3[*it].find(qw);
                    
                    string qww="";
                    qww=qww+(*it);
                    set<string>::iterator tr2 = prod3[*it2].find(qww);

                    if(tr2!= prod3[*it2].end() && tr!= prod3[*it].end()) {
                        cout << "REMOVING CYCLES IN \n";
                        cout << *it <<" -> " << qw << " and " << *it2 << " -> " << qww << endl;
                        prod3[*it2].erase(tr2);
                        prod3[*it].erase(tr);
                    }
                    //flag = 1;
                    break;
                }
            }
        }
    }
}

int main() {
    //freopen("/home/nalin/Desktop/code/input.txt","r", stdin);
    int n;
    cout << "Enter the total number of productions in your grammar\n";
    cin >> n;
    string s;
    char c;
    for(int i =0; i < n; i++) {
        cin >> c;
       // cout << "->";
        cin >> s;
        prodction[c].push_back(s);
        nonterm.insert(c);
    }
    set <char>::iterator it,it2,it3;
    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        vector <string> ss = prodction[*it];
        for(int j = 0; j < ss.size(); j++) {
            string q = ss[j];
            for(int k = 0; k < q.length(); k++) {
                if(nonterm.find(q[k])!= nonterm.end()) {
                    continue;
                }
                term.insert(q[k]);
            }
        }
    }


    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        for(int i =0; i <prodction[*it].size(); i++) {
                prod3[*it].insert(prodction[*it][i]);
        }
    }
   // removecycle();
    removeepsilonproduction();
    //removecycle();
    //remove indirect left recursion --->

    for(it =nonterm.begin(); it != nonterm.end(); it++) {
        if(it == nonterm.begin()) {
            directleftrecursion(*it);
            continue;
        }
        for(it2 = nonterm.begin(); it2 != it; it2++) {
                //cout << "present nonterminal = " << *it << " in " << *it2 << endl;
                checkcon(*it,*it2);
        }
        directleftrecursion(*it);
    }

    // end of removing indirect left recursion -->
    for(map<char, set<string> >::iterator it = prod3.begin(); it != prod3.end(); it++) {
        //cout << "NALIN\n";
        set<string> ss = it->second;
        cout << it->first << "->";
        for(set <string>::iterator it =ss.begin(); it != ss.end(); it++) {
            cout << *it << " | ";
        }
        cout << endl;
    }

}