#define demo
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

const ll maxN = 1e4+7;

ll n;
map<string,ll> a[maxN]; // De bai
set<item>se_all_item, L, L_k, L_temp;
vector<pair<item,item>>Law;
map<item,ll>C, mp_support;
double min_sup, min_conf;


// Bo bien de sinh
vector<ll>vt_sinh;
vector<vector<ll>>vt_sinh_list;
ll n_sinh;

vector<item> convert_to_vector(set<item>se){
    vector<item>vt;
    for(auto x: se) vt.pb(x);
    return vt;
}
vector<string> convert_to_vector(item it){
    vector<string>vt;
    for(auto x: it.se) vt.pb(x);
    return vt;
}

item merge_item(item a, item b){
    item i;
    for(string x: a.se) i.se.insert(x);
    for(string x: b.se) i.se.insert(x);
    return i;
}

ll cal_support(item it){
    if(mp_support.find(it)==mp_support.end()){
        ll cnt = 0;
        f1(i,n){
            ll res = 1;
            for(string x: it.se){
                if(a[i][x]==0) {
                    res=0;
                    break;
                }
            }
            cnt+=res;
        }
        mp_support[it] = cnt;
    }
    return mp_support[it];
}

void dfs(ll u){
    for(ll i=0; i<=1; i++) {
        vt_sinh.pb(i);
        if(u==n_sinh){
            ll cnt = 0;
            for(ll x: vt_sinh) cnt+=x;
            if(cnt>0 && cnt<n_sinh) {
                vt_sinh_list.pb(vt_sinh);
            }
        }
        else dfs(u+1);
        vt_sinh.pop_back();
    }
}

bool is_frequently(item it){
    vector<string> vt_item = convert_to_vector(it);
    n_sinh = it.se.size();
    vt_sinh_list.clear();
    vt_sinh.clear();

    dfs(1);

    for(auto x: vt_sinh_list){
        item i_temp;
        f0(i,n_sinh) if(x[i]==1) {
            i_temp.se.insert(vt_item[i]);
        }
        if(L.find(i_temp)==L.end()) return false;
    }

    return true;
}

void doc(){
    cin >> n >> min_sup >> min_conf; cin.ignore();
    f1(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        while(getline(ss,s,',')) if(s!=""){
            a[i][s]=1;
            se_all_item.insert(item(s));
        }
    }
}

void Buoc1(){
    // Tim L_k;
    for(auto x: se_all_item){
        double db = cal_support(x)*1.0/n;
//        cout << x.display() << ' ' << cal_support(x) << ' ' << db, el;
        if(db>=min_sup) L_k.insert(x);
    }
    //

    L = L_k;
}

void Buock(){
    C.clear();
    vector<item>vt = convert_to_vector(L_k);
    L_k.clear();

    ll n_vt = vt.size();
    for(ll i=0; i<n_vt-1; i++){
        for(ll j=i+1; j<n_vt; j++){
            item i_temp = merge_item(vt[i], vt[j]);
            if(!is_frequently(i_temp)) continue;

            // Tim L_k
            double db = cal_support(i_temp)*1.0/n;
            if(db>=min_sup) L_k.insert(i_temp);
            //
        }
    }

    for(auto x: L_k) L.insert(x);
}

void SinhLuat(){
    for(auto it: L) if(it.se.size()>=2){
        ll it_support = cal_support(it); // support(A U B)
        vector<string> vt_item = convert_to_vector(it);
        n_sinh = it.se.size();
        vt_sinh_list.clear();
        vt_sinh.clear();

        dfs(1);

        for(auto x: vt_sinh_list){
            item i_temp_a, i_temp_b;
            f0(i,n_sinh) switch(x[i]){
                case 0: i_temp_a.se.insert(vt_item[i]); break;
                case 1: i_temp_b.se.insert(vt_item[i]); break;
            }
            ll a_support = cal_support(i_temp_a);

            // Kiem tra dieu kien luat
            double db = it_support*1.0/a_support;
            if(db>=min_conf) Law.pb({i_temp_a, i_temp_b});
            //
        }

    }
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc(); // Doc trong file
    Buoc1(); // Thuc hien buoc 1
    while(L_k.size()>0){
        Buock(); // Thuc hien buoc k tim L
    }
    cout << "Danh sach L (Tap muc pho bien): ", el;
    for(auto x: L)
        cout << x.display(), el;
    el;

    // Tao luat
    SinhLuat();
    cout << "Luat ket hop: ", el;
    for(auto [u,v]: Law){
        cout << u.display() << " => " << v.display(), el;
    }
}
