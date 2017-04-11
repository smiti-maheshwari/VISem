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
int reduce[1005];                   // if node is reducing then stores the production number else stores -1

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

int err;

bool check(int x)
{
    bksi base = ulta[x];
    int last, mid;
    last = mid = 0;

    bksi :: iterator p1;
    for(p1 = base.begin(); p1 != base.end(); ++p1) {
        if(p1-> se == (p1 -> fi).size() && (p1 -> fi)[0] != 'S' && (p1 -> fi)[3] != 'e')
            last++;
        else if(p1 ->se < (p1 -> fi).size())
            mid++;
    }
    if(last > 1) {
        err = 3;
        return 0;
    }
    if(last && mid) {
        err = 5;
        return 0;
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
        if(!check(idx)) {
            notValid = 1;
        }

        if(hg)
            fnc(nodes[t1]);
    }
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

    FOR(i, 0, 1000)
        reduce[i] = -1;


    FOR(i, 1, n + 1) {
        cin >> inp[i];
        separate(inp[i]);
    }
    string dollar2 = dollar;

    production[1] = "S->"+dollar;
    anti_production["S->"+dollar] = 1;



    /*SS :: iterator b1;
    for(b1 = ma["E"].begin(); b1 != ma["E"].end(); ++ b1)
        cout <<" *** "<< *b1 << endl;
*/

    initialise();
    fnc(1);

    FOR(i, 1, id) {
        bksi tmp = ulta[i];
        bksi :: iterator p1;
        cout <<i <<"  - >" <<endl;
        for(p1 = tmp.begin(); p1 != tmp.end(); ++p1)
            cout <<"         "<< p1->fi<<" "<<p1->se <<endl;

        if(tmp.size() == 1) {
            string bn = tmp.begin() -> fi;
            //if(bn[0] == 'S')
              //  cout<<" -- >  "<<bn <<" "<<reduce[i]<<endl;



            if(bn.size() == tmp.begin() -> se && bn[0] != 'S')
                reduce[i] = anti_production[bn];
        }

    }
    cout << endl << endl;

    FOR(i, 1, prd) {
        cout <<"production  -> "<< i <<" "<<production[i] << endl;
    }
    cout << endl << endl;

    cout << endl << endl;

    FOR(i, 1, id)
        cout <<"reduce -> "<< i <<" "<<reduce[i] << endl;
    cout << endl << endl;




        cout <<endl << endl << endl;
    map< pair<int, string> , int> :: iterator f1;

    for(f1 = link.begin(); f1 != link.end(); ++ f1) {
        cout <<" ( "<< (f1->fi).fi <<" "<<(f1->fi).se<<" )   ->  "<<(f1->se)<<endl;
    }

    if(notValid) {
        cout <<"NOT VALID\n\n";
        if(err == 3)
            cout <<"REDUCE REDUCE\n";
        else
            cout <<"SHIFT REDUCE\n";
    }
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
            Head = str.substr(cur, 1);
            Top = parser.top();


            bksi bkk = ulta[Top];
            st1 = (bkk.begin()) -> fi;
            t1 = (bkk.begin()) -> se;
             if((bkk.find(mp("S->"+dollar2, 4)) != bkk.end()) &&  str[cur] == '$' ) {                           //TERMINATING CONDITION
                break;
            }


            if(ma.find(Head) == ma.end() && reduce[Top] != -1) {            // check for reduction
                t1 = production[reduce[Top]].size() - 3;
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
                parser.push(production[reduce[Top]][0]);
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

        if(!ans) {
            cout <<"STRING ACCEPTED\n";
        } else {
            cout <<"STRING NOT ACCEPTED\n";
        }
    }
    return 0;




    return 0;
}


