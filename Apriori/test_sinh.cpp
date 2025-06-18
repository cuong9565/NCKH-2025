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

const ll maxN = 2e5+7;

ll n;
vector<ll>vt;

void dfs(ll u){
    for(ll i=0; i<=1; i++) {
        vt.pb(i);
        if(u==n){
            for(ll x: vt) cout << x;
            el;
        }
        else dfs(u+1);
        vt.pop_back();
    }
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("temp.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    cin >> n;
    dfs(1);
}


