#define demo
/*
    user: Cuong05.SGU
    name: Le Manh Cuong
    year: 2025
    face: https://www.facebook.com/le.manh.cuong.226886/
*/
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(ll i=0; i<n; i++)
#define f1(i,n) for(ll i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;

struct item{
    set<string>se;

    item(){}
    item(string str){
        se.insert(str);
    }
    item(vector<string>vt){
        for(string x: vt)
            se.insert(x);
    }
    string display(){
        string str="{";
        if(se.size()>0){
            for(string x: se)
                str+= x+",";
            str.pop_back();
        }
        str+="}";
        return str;
    }
    bool operator<(const item &o)const{
        return se < o.se;
    }
};

set<item>se;

signed main(){
    tm_opt;
    #ifdef demo
    freopen("temp.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo


    vector<string>vt = {"A", "B", "C"};

    item i(vt);
    item i1("Con Cho");
    item i2;

    item i3("123");
    item i4;
    item i5("Con Cho");

    se.insert(i);
    se.insert(i1);
    se.insert(i2);

    for(auto x: se){
        cout << x.display(), el;
    }
    cout << (se.find(i3)!=se.end()), el;
    cout << (se.find(i4)!=se.end()), el;
    cout << (se.find(i5)!=se.end()), el;
}


