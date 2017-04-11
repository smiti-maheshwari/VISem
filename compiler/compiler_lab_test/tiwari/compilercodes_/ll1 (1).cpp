// rishabhtwr07
#include<bits/stdc++.h>

using namespace std;

#define FOR(i,a,b) for(i=a;i<b;++i)
#define FORD(i,a,b) for(i=a;i>=b;--i)
#define ll long long
#define infi 1000000007
#define MOD 1000000007
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define s(x) scanf("%d", &x)
#define sl(x) scanf("%lld", &x)
#define sd(x) scanf("%lf", &x)
#define p(x) printf("%d",x)
#define pl(x) printf("%lld",x)
#define pn() printf("\n")
#define ps(x) printf("%s",x)


#define mapss map<string, set<string> >
#define mapsc map<string, set<char> >

int n;
string inp[100],dollar;
mapss ma;
mapsc frst, follow;
map<string , int> memoize, memoizef;
bool flag;
map< pair<string, char>, string > LL1;
bool notValid;


/*
 - code will work iff all terminals and non terminals are single character, no space between 2 adjacent terminals and non terminals change productions like A -> B a to A->Ba
 - productions like A -> B ; B -> A will end up into infinite loop in the follow section.
 - most probably code is wrong :p
 */


void separate(string str)
{
    int i, j, n = str.size();

    string fh = "", sh ="", tmp = "";


    FOR(i, 0, n) {
        if(str[i] == '-' && str[i + 1] == '>')
            break;
        fh.pb(str[i]);
    }
    if(!flag) {
        dollar = fh;
        flag = 1;
    }

    FOR(j, i + 2, n) {
        if(str[j] == '|') {
            ma[fh].insert(sh);
            sh = "";
        } else
            sh.pb(str[j]);
    }
    if(sh.size() != 0)
        ma[fh].insert(sh);

}





void compute_frst(string str)
{

    if(memoize[str] == 1)
        return;
    memoize[str] = 1;

    // terminals
    if(str.size() == 1 && ma.find(str) == ma.end()) {
        frst[str].insert(str[0]);
        return;
    }

    int j;
    string tmp1, tmp2;
    set<string> stmp1;
    set<string> :: iterator s1;
    set<char> :: iterator s2;

    if(str.size() == 1) {
        stmp1 = ma[str];
        for(s1 = stmp1.begin(); s1 != stmp1.end(); ++ s1) {
            compute_frst(*s1);

            for(s2 = frst[*s1].begin(); s2 != frst[*s1].end(); ++s2)
                frst[str].insert(*s2);
        }
    } else {
        int i;
        FOR(i, 0, str.size()) {
            tmp1 = str.substr(i, 1);
            compute_frst(tmp1);

            for(s2 = frst[tmp1].begin(); s2 != frst[tmp1].end(); ++s2)
                frst[str].insert(*s2);
            if(frst[tmp1].find('e') == frst[tmp1].end())
                break;
        }

    }
}


void compute_follow(string str)
{
    if(memoizef[str] == 1)
        return;
    memoizef[str] = 1;

    if(ma.find(str) == ma.end())
        return;

    mapss :: iterator p1;
    string tmp1, tmp2, tmp3;
    set<char> sc1;
    set<char> :: iterator pc1, pc2;
    set<string> :: iterator ps1;
    int i, j, k;


    for(p1 = ma.begin(); p1 != ma.end(); ++p1) {
        for(ps1 = p1->se.begin(); ps1 != p1->se.end(); ++ps1) {
            tmp1 = *ps1;
            FOR(j, 0, tmp1.size()) {
                if(tmp1.substr(j, 1) == str) {
                    //if(str == "X")
                       // cout <<" YES \n";
                    FOR(k, j + 1, tmp1.size()) {
                        tmp2 = tmp1.substr(k, 1);
                       // if(str == "X")
                         //       cout <<"                                  "<<tmp2 <<" "<<frst[tmp2].size()<< endl;
                         compute_frst(tmp2);
                        for(pc1 = frst[tmp2].begin(); pc1 != frst[tmp2].end(); ++pc1) {
                            //if(str == "E")
                              //  cout <<"                                 -"<<*pc1 << endl;
                            if(*pc1 != 'e')
                                follow[str].insert(*pc1);
                        }

                        if(frst[tmp2].find('e') == frst[tmp2].end())
                            break;

                    }
                   // if(str == "X")
                      //      cout <<"                                   "<<p1->fi <<" " <<" "<<k <<" "<<tmp1 <<" "<<endl;
                    if(k == tmp1.size() && p1->fi != str) {
                        compute_follow(p1->fi);
                        for(pc1 = follow[p1->fi].begin(); pc1 != follow[p1->fi].end(); ++ pc1)
                            follow[str].insert(*pc1);
                    }

                }
            }

        }

    }

}

int main()
{
    notValid = 0;
    flag = 0;
    int i, j, k;
    cout <<"ENTER NUMBER OF PRODUCTIONS\n";
    cin >> n;

    FOR(i,0 , n)
        cin >> inp[i];

    FOR(i, 0, n)
        separate(inp[i]);


    set<string> :: iterator b1;
    map< string , set<string> > :: iterator p1;
   /* cout <<"-----------------------\n\n";
    for(p1 = ma.begin(); p1 != ma.end(); ++p1) {
        cout << p1 -> fi <<"- >" <<endl;

        for(b1 = p1->second.begin(); b1 != p1 ->se.end(); ++b1)
                cout << *b1 << endl;
    }*/

    for(p1 = ma.begin(); p1 != ma.end(); ++p1)
        compute_frst(p1->first);

    set<char> :: iterator s1;
    mapsc :: iterator p12;
    cout <<"-------------FIRST ------------------------\n";
    cout << endl <<endl ;
    for(p12 = frst.begin(); p12 != frst.end(); ++p12) {
        string fh = p12 -> first;
        cout << fh <<" ->  "<<endl;
        for(s1 = frst[fh].begin(); s1 != frst[fh].end(); ++s1)
            cout << *s1 << " ";
        cout << endl<<endl;
    }





    // follow

    memoize.clear();
   //    memoize[dollar] = 1;

    follow[dollar].insert('$');

    for(p1 = ma.begin(); p1 != ma.end(); ++p1)
        compute_follow(p1 -> fi);
    cout <<"-----------------Follow------------\n";
    for(p1 = ma.begin(); p1 != ma.end(); ++p1) {
        cout << p1->fi <<" --- > \n";
        for(s1 = follow[p1->fi].begin(); s1 != follow[p1->fi].end(); ++s1)
            cout << *s1 <<" ";
        cout << endl  << endl;
    }



    mapss :: iterator np1;
    set<char> :: iterator np2;
    set<string> :: iterator nps1;

    for(np1 = ma.begin(); np1 != ma.end(); ++np1) {
        set<string> ns = np1 -> second;
        string fh = np1 -> first;
        for(nps1 = ns.begin(); nps1 != ns.end(); ++nps1) {
            string nstr = *nps1;
            bool nwflag = 0;
            for(int l1 = 0; l1 != nstr.size(); ++ l1) {
                string nstr2 = nstr.substr(l1, 1);
                set<char> tmpfirst = frst[nstr2];
                for(np2 = tmpfirst.begin(); np2 != tmpfirst.end(); ++np2) {
                    if(*np2 != 'e') {
                        if(LL1.find(mp(fh, *np2)) != LL1.end()) {
                            notValid = 1;
                        }
                        LL1[mp(fh, *np2)] = (fh +"->"+nstr);
                    }
                }
                if(tmpfirst.find('e') == tmpfirst.end()) {
                    nwflag = 1;
                    break;
                }
            }

            if(nwflag == 0) {
                set<char> tmpfollow = follow[fh];
                //cout <<"  - - -- - -- "<<fh << endl;
                for(np2 = tmpfollow.begin(); np2 != tmpfollow.end(); ++np2) {

                    if(LL1.find(mp(fh, *np2)) != LL1.end()) {
                        notValid = 1;
                    }
                    LL1[mp(fh, *np2)] = (fh + "->"+nstr);
                }
            }
        }

    }

    cout << endl << endl << endl;


    if(notValid)
            cout <<"Not accpeted by LL1 parser \n\n";
    else {

        map< pair<string, char>, string > :: iterator pr;
        for(pr = LL1.begin(); pr != LL1.end(); ++ pr)
            cout << "["<<(pr ->fi).fi <<","<<(pr->fi).se <<"]  "<<(pr->se) << endl;
    }


    string tmp1;


    cout <<"\n\nENTER THE STRING TO PARSE \n\n";
    cin >> tmp1;

    tmp1.pb('$');
    stack<string> parser;

    parser.push("$");
    parser.push(dollar);
    string Top, Head;
    bool status = 0;
    string production;

    int cur = 0;
    while(cur < tmp1.size()) {
        Top = parser.top();
        Head = tmp1.substr(cur, 1);
        //cout <<" -> "<< Top <<" "<<Head <<" ";
        if(ma.find(Top) == ma.end()) {
            //cout <<"sas "<<endl;
            if(Head == Top) {
                parser.pop();
                cur++;
            } else {
                status = 1;
                break;
            }

        } else if(LL1.find(mp(Top, Head[0])) == LL1.end()) {
            status = 1;

            break;
        } else {
            production = LL1[mp(Top, Head[0])];
            parser.pop();
            FORD(i, production.size() - 1, 3) {
                if(production.substr(i, 1) != "e")
                    parser.push(production.substr(i, 1));
                //cout << production.substr(i, 1) <<" ";
            //cur++;
            }
        }
        //cout << endl;
    }

    if(status) {
        cout <<"NOT Accepted \n\n";
    } else {
        cout <<"Accepted \n\n";
    }

    return 0;
}



