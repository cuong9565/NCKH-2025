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
    // Variable
    set<string>se;

    // Constructor
    item(){}
    item(string str){ add(str); }
    item(vector<string>vt){ for(string x: vt) add(x); }

    // Operator
    bool operator<(const item &o)const{ return se < o.se; }

    // Function
    void add(string str){ se.insert(str); }
    string display(){
        string str="{";
        if(se.size()>0){
            for(string x: se) str+= x+",";
            str.pop_back();
        }
        str+="}";
        return str;
    }

};

struct tree{
    string str;
    int cnt;
    tree *parent;
    map<string, tree*> child;

    tree(string str, tree *parent = nullptr): str(str), cnt(1), parent(parent){}

    tree *add(string str){
        if(child.count(str)) child[str]->cnt++;
        else child[str] = new tree(str, this);
        return child[str];
    }
};

const ll maxN = 1e4+7;

ll n;
set<string>a[maxN];
map<string,ll>items;
map<string,vector<tree*>>mp_pos_item;
vector<pair<string,ll>>vt_items;
set<item>L;
tree *head = new tree("");
double min_sup;

// Bo sinh
vector<ll>vt_sinh;
vector<vector<ll>>sinh_list;
ll n_sinh;

bool compare(pair<string,ll>a, pair<string,ll>b){
    if(a.second != b.second) return a.second > b.second;
    return a.first < b.first;
}

void doc(){
    cin >> n >> min_sup; cin.ignore();
    f1(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        while(getline(ss,s,',')) if(s!=""){
            a[i].insert(s);
            items[s]++;
        }
    }
    for(auto it = items.begin(); it!=items.end(); ){
        double db = it->second*1.0/n;
        if(db<min_sup) it = items.erase(it);
        else it++;
    }
    for(auto [u,v]: items) vt_items.pb({u,v});
    sort(all(vt_items), compare);
}

void solve(){
    f1(i,n){
        tree *curr = head;
        for(auto [u,v]: vt_items) if(a[i].find(u)!=a[i].end()){
            curr = curr->add(u);
        }
    }
}

void dfs(tree *curr){
    for(auto [u,v]: curr->child){
        mp_pos_item[u].pb(v);
        dfs(v);
    }
}

void sinh_xau(ll u){
    if (u > n_sinh) {
        vt_sinh.pb(0);
        sinh_list.pb(vt_sinh);
        return;
    }
    for(ll i=0; i<=1; i++){
        vt_sinh.pb(i);
        if(u==n_sinh) sinh_list.pb(vt_sinh);
        else sinh_xau(u+1);
        vt_sinh.pop_back();
    }
}

void tao_danh_sach(){
    for(auto [u,v]: mp_pos_item){
        map<string,ll>mp;
        for(tree *x: v){
            tree *curr = x;
            ll cnt = curr->cnt;
            while(true){
                curr = curr->parent;
                if(curr->str == "") break;
                else mp[curr->str]+=cnt;
            }
        }
        for(auto it = mp.begin(); it!=mp.end(); ){
            double db = it->second*1.0/n;
            if(db<min_sup) it = mp.erase(it);
            else it++;
        }

        // Tao danh sach
        vector<string>vt;
        for(auto[x,y]: mp) vt.pb(x);

        n_sinh = vt.size();
        sinh_list.clear();
        vt_sinh.clear();

        sinh_xau(1);
        for(auto x: sinh_list){
            item it(u);
            ll n_size = x.size();
            f0(i,n_size) if(x[i]==1) it.add(vt[i]);
            L.insert(it);
        }
    }
}

void print_result(){
    cout << "Danh sach tap L: ", el;
    for(item x: L) cout << x.display(), el;
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc();
    solve();
    dfs(head);
    tao_danh_sach();
    print_result();
}


