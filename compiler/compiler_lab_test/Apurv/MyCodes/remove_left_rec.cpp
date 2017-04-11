#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define mem(a,x) memset(a,x,sizeof(a))

vector <string> ans;
vector <string> vv[100];

void remove_left_rec(string s)
{
    int i,j,l,r,flag;
    l = 3;
    string str, ss, ss_new, tmp, tmp2, x, y, xx, res, yy;
    vector <string> v,V,nv;

    ss.pb(s[0]);
    x = ss;
    ss.pb('-'); ss.pb('>');
    ss_new = x;
    ss_new.pb('`');
    y = ss_new;
    ss_new.pb('-'); ss_new.pb('>');

    //cout << ss << " " << ss_new << " " << x << " " << y << "\n\n";

    for(i = 3; i < s.size(); ++i){
        if(s[i] == '|'){
            r = i - 1;
            str = s.substr(l, r-l+1);
            v.pb(str);
            l = i + 1;
        }
    }
    str = s.substr(l, s.size()-1-l+1);
    v.pb(str);

    /*for(i = 0; i < v.size(); ++i){
        cout << v[i] << " ";
    }
    cout << endl;*/

    for(i = 0; i < v.size(); ++i){
        vv[s[0]].pb(v[i]);
    }
    //cout << vv[s[0]][0] << " " << vv[s[0]][1] << endl;
    for(i = 0; i < v.size(); ++i){
        tmp2 = v[i][0];
        if(vv[tmp2[0]].size() && tmp2[0] != s[0]){
            xx = v[i];
            xx.erase(0,1);
            v.erase(v.begin()+i);
            for(j = 0; j < vv[tmp2[0]].size(); ++j){
                yy = vv[tmp2[0]][j];
                if(yy[0] == '$')
                    res = xx;
                else
                    res = yy + xx;
                nv.pb(res);
            }
        }
    }
    for(i = 0; i < nv.size(); ++i){
        v.pb(nv[i]);
    }
    /*for(i = 0; i < v.size(); ++i){
        cout << v[i] << " ";
    }
    cout << endl;*/

    flag = 0;
    for(i = 0; i < v.size(); ++i){
        if(v[i][0] == s[0]){
            flag = 1;
            break;
        }
    }
    for(i = 0; i < v.size(); ++i){
        tmp = v[i];
        if(tmp[0] == s[0]){
            tmp.erase(0,1);
            tmp = ss_new + tmp + y + "|$";
            //cout << tmp << "\n\n\n\n";
            V.pb(tmp);
        }
        else{
            if(flag && tmp[0] != '$'){
                tmp = ss + tmp + y;
                V.pb(tmp);
            }
            else{
                tmp = ss + tmp;
                V.pb(tmp);
            }
        }
    }

    for(i = V.size()-1; i >= 0; --i){
        ans.pb(V[i]);
    }
}

int main()
{
    int n,i;
    string s;

    cin >> n;
    for(i = 0; i < n; ++i){
        cin >> s;
        remove_left_rec(s);
    }
    cout << endl;

    //cout << vv['S'][0] << " " << vv['A'][1] << endl;

    for(i = 0; i < ans.size(); ++i){
        cout << ans[i] << endl;
    }
    return 0;
}
