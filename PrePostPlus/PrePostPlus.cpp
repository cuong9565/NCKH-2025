#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using namespace std::chrono;

struct PPCNode{
    int id, pre, post, cnt;
    PPCNode *parent;
    unordered_map<int,PPCNode*>children;

    PPCNode():id(-1), pre(0), post(0), cnt(1), parent(nullptr){}
    PPCNode(int id, PPCNode *parent): id(id), parent(parent), pre(0), post(0), cnt(1){}

    PPCNode *insert_child_by_id(int id){
        if(children.count(id)){
            children[id]->cnt++;
            return children[id];
        }

        PPCNode *new_child = new PPCNode(id, this);
        children[id] = new_child;
        return new_child;
    }
};

struct NListNode{
    int pre, post, cnt;
};

typedef vector<NListNode>Nlist;


int n, minsup, idPre, idPos; // So dong, nguong ho tro toi thieu, gan Pre, gan Post
float min_sup = 0.7;
vector<vector<int>>transactions; // Tap giao dich
vector<pair<vector<int>,int>>freq; // Ket qua tap muc thuong xuyen

PPCNode *root = new PPCNode();
unordered_map<int,int>mp_count_item; // dem item trong transaction
unordered_map<int,int>mp_order_item; // tim thu tu item
unordered_map<int,Nlist>mp_nlists; // Danh sach nlist tu
vector<pair<int,int>>vt_count_item; // mp da chuyen thanh vt
vector<pair<int,Nlist>>vt_nlists; // Chuyen tu mp_nlists sang vt_nlists


void dfsPrePost(PPCNode *curr){
    curr->pre = idPre++;
    for(auto [u,v]: curr->children) dfsPrePost(v);
    curr->post = idPos++;
}

void build_nlists(PPCNode *curr){
    for(auto [u,v]: curr->children){
        mp_nlists[u].pb({v->pre, v->post, v->cnt});
        build_nlists(v);
    }
}

int cal_support(const Nlist &nlists){
    int s = 0;
    for(const NListNode &x: nlists) s+=x.cnt;
    return s;
}

Nlist intersect(const Nlist &a, const Nlist &b){
    Nlist c;
    int i = 0, j = 0;
    while(i<a.size() && j<b.size()){
        if(a[i].pre<b[j].pre && b[j].post<a[i].post){
            c.pb({b[j].pre, b[j].post, min(a[i].cnt, b[j].cnt)});
            j++;
        }
        else if(b[j].post >= a[i].post) i++;
        else j++;
    }
    return c;
}

void dfsPrePostPlus(vector<int> prefix, vector<pair<int,Nlist>>suffix) {
    int n = suffix.size();
    for(int i=0; i<n; i++){
        vector<int>new_prefix = prefix; new_prefix.pb(suffix[i].first);
        int sup = cal_support(suffix[i].second);
        if(sup>=minsup){
            freq.pb({new_prefix, sup});

            vector<pair<int,Nlist>>new_suffix;
            for(int j=i+1; j<n; j++){
                Nlist inter = intersect(suffix[i].second, suffix[j].second);
                if(cal_support(inter)>=minsup)
                    new_suffix.pb({suffix[j].first, inter});
            }

            dfsPrePostPlus(new_prefix, new_suffix);
        }
    }
}


void solve(){
    string str;
    while(getline(cin, str)){
        stringstream ss(str);
        string s;
        vector<int>transaction;

        while(getline(ss, s, ' ')) {
            int x = stoi(s);
            transaction.pb(x);
            mp_count_item[x]++;
        }

        transactions.pb(transaction);
        n++;
    }
    minsup = min_sup * n;

    // Chuyen mp_count_item => vt_count_item va kiem tra minsup
    for(auto &[u,v]: mp_count_item) if(v>=minsup)
        vt_count_item.pb({u,v});

    // Sap xep vt_count_item theo v tang dang
    sort(all(vt_count_item), [&](const pair<int,int> a, const pair<int,int> b){
        if(a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    // Tao thu tu cac muc
    f0(i,int(vt_count_item.size()))
        mp_order_item[vt_count_item[i].first] = i;

    // Loc item co support >= minsup
    for(auto &transaction: transactions){
        vector<int>tr;
        for(auto x: transaction) if(mp_count_item[x]>=minsup) tr.pb(x);
        transaction = tr;
    }

    // Sap xep cac transaction theo thu tu
    for(auto &transaction: transactions){
        vector<int>sorted_transaction = transaction;

        sort(all(sorted_transaction), [&](const int a, const int b){
            return mp_order_item[a] < mp_order_item[b];
        });

        transaction = sorted_transaction;
    }

    // Xay dung cay PPC
    for(auto &transaction: transactions){
        PPCNode *curr = root;
        for(auto &x: transaction)
            curr = curr->insert_child_by_id(x);
    }

    // Gan Pre Post
    dfsPrePost(root);

    // Xay dung N-list
    build_nlists(root);

    // Tra ve vt_nlists
    for(auto &[u,v]: mp_nlists) if(cal_support(v)>=minsup){
        vt_nlists.pb({u,v});
    }

    // Sap xep vt_nlists theo do ho tro giam
    sort(all(vt_nlists), [&](auto a, auto b){
        return mp_order_item[a.first] < mp_order_item[b.first];
    });

    // PrePost+
    dfsPrePostPlus({}, vt_nlists);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("chess.txt", "r", stdin);
//    freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;

}


