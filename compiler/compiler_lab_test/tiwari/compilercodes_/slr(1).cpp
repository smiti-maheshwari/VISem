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
#define SS set<string>

#define bksi set < pair<string, int> >                           // define a node
#define voila map< bksi, int>                                    // define the index of the node
#define Mapping map< pair< int, string>, int >                   // mapping in  dfa
#define antivoila map<int, bksi>

#define mapss map<string, set<string> >
#define mapsc map<string, set<char> >
/* NOTE : DONOT use S as a non terminal in any productions*/


int n;
bool notValid;
string inp[100],dollar;
mapss ma;
bool flag;

voila nodes;
antivoila ulta;
Mapping relation;
int id;
map<pair<int, string> , int> link;
map<int, string> production;        // links production id to production
map<string, int> anti_production;   // links production to production id
int prd;
//int reduce[1005];                   // if node is reducing then stores the production number else stores -1
mapsc frst, follow;
map<string , int> memoize, memoizef;

map< pair<int, string>, int> reduce;
map< pair<int, string>, int> validity;


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



void separate(string str)
{
    string fh, sh;
    int i, j;
    fh.clear();

    FOR(i, 0, str.size()) {
        if(str[i] == '-' && str[i + 1] == '>')
            break;
        fh.pb(str[i]);
    }
    if(flag) {
        dollar = fh;
        cout <<"   -  #@$@$@$#@@#$$$$$$$$$$$$$$$$$$  "<<str <<" #@$@  "<<dollar << endl;
        flag = 0;
    }

    sh="";
    FOR(j, i + 2, str.size()) {
        if(str[j] == '|') {
            anti_production[(fh+"->"+sh)] = prd;
            production[prd] = (fh+"->"+sh);
            prd++;
            ma[fh].insert(sh);
            //cout <<fh <<"  %% "<<sh << endl;
            sh = "";
            continue;
        }
        sh.pb(str[j]);
    }
    if(sh.size() != 0) {
      //  cout <<fh <<" %% "<<sh << endl;
        anti_production[(fh+"->"+sh)] = prd;
        production[prd] = (fh+"->"+sh);
        prd++;
        ma[fh].insert(sh);
    }
}


bksi create_node(bksi ini)
{
    int i, j;
    bksi ret;

    bksi :: iterator p1;
    SS naya;
    string tmpstr, tmpstr2;
    SS :: iterator sp1;


    for(p1 = ini.begin(); p1 != ini.end(); ++p1) {
        ret.insert(*p1);

        tmpstr = (*p1).first;
        if(tmpstr.size() == (*p1).se)
            continue;

        tmpstr = tmpstr.substr((*p1).se, 1);
        //cout <<" ))   "<< tmpstr<<endl;

        if(ma.find(tmpstr) == ma.end())                     // if tmpstr is a terminal
            continue;

        naya = ma[tmpstr];

        for(sp1 = naya.begin(); sp1 != naya.end(); ++ sp1) {
            tmpstr2 = tmpstr +"->"+ *sp1;
            //cout << "  ^^ "<<tmpstr2 <<" "<<*sp1<<endl;
            if(tmpstr2[3] != 'e')
                ret.insert(mp(tmpstr2, 3));
            else
                ret.insert(mp(tmpstr2, 4)); // epsilon production
        }
    }

    if(ini.size() == ret.size())
        return ret;
    return create_node(ret);
}

int mark(bksi ini)
{
    if(nodes[ini] == 0) {
        nodes[ini] = id;
        ulta[id] = ini;
        id++;
        return 1;
    }
    return 0;
}

void initialise()
{
    //cout <<"askjsdafjkdsaf   "<<dollar<<endl;
    inp[0] = "S->"+dollar;
    dollar = "S";


    bksi ini;
    ini.insert(mp(inp[0], 3));

    ini = create_node(ini);
    mark(ini);


}

bool check()
{
    map< pair<int, string>, int> :: iterator p1;
    validity.clear();

    for(p1 = link.begin(); p1 != link.end(); ++ p1) {
        if(validity[p1->fi])
            return 0;
        validity[p1->fi] = 1;
    }

    for(p1 = reduce.begin(); p1 != reduce.end(); ++p1) {

        if(validity[p1->fi])
            return 0;
        validity[p1->fi] = 1;
    }
    return 1;
}


void fnc(int x)
{
    bksi t1, t2, t3, base;
    //cout << x << endl;

    bksi :: iterator bk1, bk2;

    map<string, int> tmpmap;
    tmpmap.clear();
    string tmpstr1, tmpstr2;
    base = ulta[x];


    for(bk1 = base.begin(); bk1 != base.end(); ++bk1) {
        t1.clear();
        if(bk1->se >= (bk1->fi).size())
            continue;
        tmpstr1 = (bk1->fi).substr(bk1->se, 1);
        if(tmpmap[tmpstr1])
            continue;
        tmpmap[tmpstr1] = 1;
        for(bk2 = bk1; bk2 != base.end(); ++bk2) {
            if(bk2->se >= (bk2->fi).size())
                continue;
            tmpstr2 = (bk2->fi).substr(bk2->se, 1);
            if(tmpstr2 == tmpstr1)
                t1.insert(mp(bk2->fi, (bk2->se + 1)));
        }
        t1 = create_node(t1);
        int hg = mark(t1);
        int idx = nodes[t1];
        link[mp(x, tmpstr1)] = idx;
       /* if(!check(idx)) {
            notValid = 1;
        }*/

        if(hg)
            fnc(nodes[t1]);
    }
}

void print(stack<int> st)
{
    cout << endl << endl;
    int id = 0;
        while(!st.empty()) {
            if(!id)
            cout << st.top() << " ";
            else
                cout <<(char)(st.top())<<" ";
            id = !id;
            st.pop();
        }
        cout << endl << endl;
}


int main()
{
    flag = 1;
    prd =  2;
    notValid = 0;
    id = 1;
    cout <<"Enter the number of productions do not use S as a non terminal \n";
    cin >> n;

    int i, j, k;
    string dollar2;




    FOR(i, 1, n + 1) {
        cin >> inp[i];
        separate(inp[i]);
    }
    separate("S->"+dollar);

    production[1] = "S->"+dollar;
    anti_production["S->"+dollar] = 1;
    prd--;
    dollar2 = dollar;

    mapss :: iterator p1;
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





    /*SS :: iterator b1;
    for(b1 = ma["E"].begin(); b1 != ma["E"].end(); ++ b1)
        cout <<" *** "<< *b1 << endl;
*/

    initialise();
    fnc(1);
    string dump = "1";


    set<char> ty;
    set<char> :: iterator tp1;

    int Accept = 3;
    FOR(i, 1, id) {
        bksi tmp = ulta[i];
        bksi :: iterator p1;
        cout <<i <<"  - >" <<endl;
        if(tmp.find(mp(("S->"+dollar2), 4)) != tmp.end())
            Accept = i;
        for(p1 = tmp.begin(); p1 != tmp.end(); ++p1)
            cout <<"         "<< p1->fi<<" "<<p1->se <<endl;

        //if(tmp.size() == 1) {
          //  string bn = tmp.begin() -> fi;
            //if(bn[0] == 'S')
              //  cout<<" -- >  "<<bn <<" "<<reduce[i]<<endl;
            bksi :: iterator p11;
            for (p11 = tmp.begin(); p11 != tmp.end(); ++ p11) {
                string bn = (p11->fi);
                if(bn.size() != p11->se)
                    continue;
                if(bn[0] != 'S') {
                    ty = follow[bn.substr(0, 1)];
                    for(tp1 = ty.begin(); tp1 != ty.end(); ++ tp1) {
                        dump[0] = *tp1;
                        reduce[mp(i, dump)] = anti_production[bn];
                    }
                }
        }

    }
    cout << endl << endl;

    FOR(i, 1, prd) {
        cout <<"production  -> "<< i <<" "<<production[i] << endl;
    }
    cout << endl << endl;

    cout << endl << endl;

    map<pair<int, string>, int> :: iterator p2;
    for(p2 = reduce.begin(); p2 != reduce.end(); ++p2)
        cout <<"reduce " <<(p2->fi).fi << " "<<(p2->fi).se<<" "<<(p2->se) << endl;
    cout << endl << endl;




        cout <<endl << endl << endl;
    map< pair<int, string> , int> :: iterator f1;

    for(f1 = link.begin(); f1 != link.end(); ++ f1) {
        cout <<" ( "<< (f1->fi).fi <<" "<<(f1->fi).se<<" )   ->  "<<(f1->se)<<endl;
    }

    cout <<endl << endl << endl;
    cout << "reduce -> \n";
    for(f1 = reduce.begin(); f1 != reduce.end(); ++f1) {
            cout <<" ( "<< (f1->fi).fi <<" "<<(f1->fi).se<<" )   ->  "<<(f1->se)<<endl;
    }
    cout << endl << endl;

    if(!check())
        cout <<"NOT VALID\n\n";
    else {
        cout <<" ENTER THE STRING TO PARSE \n\n";
        string str, dump;
        cin >> str;
        str.pb('$');
        dump = "1";

        stack<int> parser;

        parser.push(1);
        int cur = 0;
        string Head;
        int Top;
        string STop;
        int t1, t2, t3;
        string st1, st2;

        bool ans = 0;
        while(cur < str.size()) {
            print(parser);

            Head = str.substr(cur, 1);
            Top = parser.top();
            cout <<"        "<<Head <<" "<<Top << endl;

            bksi bkk = ulta[Top];
            st1 = (bkk.begin()) -> fi;
            t1 = (bkk.begin()) -> se;
            if(Top == Accept &&  str[cur] == '$') {                           //TERMINATING CONDITION
                break;
            }


            if(reduce.find(mp(Top, Head)) != reduce.end()) {            // check for reduction
                t1 = production[reduce[mp(Top,Head)]].size() - 3;
                t1 *= 2;
                while(t1--) {
                    if(parser.size() == 0) {
                        ans = 1;
                        break;
                    }
                    parser.pop();
                }
                if(ans)
                    break;
                t2 = parser.top();
                parser.push(production[reduce[mp(Top,Head)]][0]);
                dump[0] = (char)(parser.top());

                t3 = link[mp(t2, dump)];
                if(!t3) {
                    ans = 1;
                    break;
                }
                parser.push(t3);
            } else {                                                        // shift
                t1 = link[mp(Top, Head)];
                if(!t1) {
                    ans = 1;
                    break;
                }

                parser.push(Head[0]);
                parser.push(t1);
                cur++;

            }

        }
        cout << ("S->"+dollar2) << endl;
        cout <<"  - > accept - > "<<Accept<<endl;
        if(!ans) {
            cout <<"STRING ACCEPTED\n";
        } else {
            cout <<"STRING NOT ACCEPTED\n";
        }
    }




    return 0;
}


