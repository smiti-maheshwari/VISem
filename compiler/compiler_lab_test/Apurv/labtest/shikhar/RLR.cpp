#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include<stack>
#include<set>
#include<limits.h>
#include <queue>
#include <map>
#include <cstdlib>
#include <algorithm>
//psyduck
#define ll long long
#define S(x) scanf("%d",&x)
#define Sf(x) scanf("%f",&x)
#define Slf(x) scanf("%lf",&x)
#define Sl(x) scanf("%lld",&x)
#define P(x)  printf("%d\n", x)
#define Pf(x)  printf("%f\n", x)
#define Plf(x)  printf("%lf\n", x)
#define Pl(x)  printf("%lld\n", x)
#define mem(x,i) memset(x,i,sizeof(x))
#define mod 1000000007
using namespace std;
struct node{
    char ch;
    vector<string>v;
};
node a[100];
int main()
{
    int n,i,j;
    cin >> n;
    string s,tmp;
    for (i = 1; i <= n; i++){
        cin >> s;
        j = 0;
        a[i].ch = s[0];
        while(s[j] != '-'){
            j++;
        }
        j = j+2;
        while(j < s.size()){
         //   cout << j<<" "<<s[j]<<endl;
            tmp = "";
            while(s[j] != '|' && j < s.size()){
                tmp.push_back(s[j]);
                j++;
            }
            j++;
          //  cout << tmp<<endl;
            a[i].v.push_back(tmp);
        }
    }
    /*for(i = 1; i <= n; i++){
        cout << a[i].ch<<" ";
        for (j = 0; j < a[i].v.size(); j++){
            cout << a[i].v[j]<<" ";
        }
        cout << endl;
    }*/
    char tr;
    string z;
    int id = 9;
    for (i = 1; i <= n; i++){
        for (j = 1; j <= i-1; j++){
            tr = a[j].ch;
            for (int k = 0; k < a[i].v.size(); k++){
                if(a[i].v[k][0] == tr){
                    z = a[i].v[k];
                    //cout <<z<<endl;
                    a[i].v.erase(a[i].v.begin()+k);
                    z.erase(z.begin()+0);
                    for (int l = 0; l < a[j].v.size(); l++){
                        a[i].v.push_back(a[j].v[l]+z);
                    }
                    k = 0;
                }
            }
        }
        int ff = 0;
        for (int j = 0; j < a[i].v.size(); j++){
            if(a[i].v[j][0] == a[i].ch){
                ff = 1;
            }
        }
        if(ff){
            id++;
            a[id].ch = 64+id;
            string uu = "";
            uu.push_back(64+id);
           // cout << uu;
            a[id].v.push_back("$");
            int sz = a[i].v.size();
            cout << sz<<endl;
            vector<string>rec;
            vector<string>nonrec;
            for (j = 0; j < sz; j++){
                //cout << a[i].v[j]<<" ";
                if(a[i].v[j][0] == a[i].ch){
                    z = a[i].v[j];
                    //a[i].v.erase(a[i].v.begin()+j);
                    z.erase(z.begin()+0);
                    z = z+uu;
                    nonrec.push_back(z);
                }
                else if(a[i].v[j] == "$"){
                    continue;
                }
                else {
                    z = a[i].v[j];
                   // a[i].v.erase(a[i].v.begin()+j);
                    z = z+uu;
                    rec.push_back(z);
                }
            }
            a[i].v.clear();
            for (int k = 0; k < nonrec.size(); k++){
                a[id].v.push_back(nonrec[k]);
            }
            cout << endl;
            for (int k = 0; k < rec.size(); k++){
                a[i].v.push_back(rec[k]);
            }
        }
    }
   for(i = 1; i <= id; i++){
        if( a[i].v.size() != 0){
            cout << a[i].ch<<" ";
            for (j = 0; j < a[i].v.size(); j++){
                cout << a[i].v[j]<<" ";
            }
            cout << endl;
        }
    }
}
