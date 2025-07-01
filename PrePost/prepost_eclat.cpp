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

struct PPCode{
    int pre, post, cnt;
};

struct NList{
    vector<int>item;
    vector<PPCode>nl;
    int cnt;

    NList(){cnt=0;}

    string get_item(){
        string str = "{";
        if(item.size()>0){
            str+=to_string(item[0]);
            for(int i=1; i<item.size(); i++) str += "," + to_string(item[i]);
        }
        str+="}";
        return str;
    }
};

struct Node{
    int id, h, cnt, pre, post;
    unordered_map<int, Node*>children;

    Node(){
        h = id = pre = post = 0;
        cnt = 1;
    }

    Node(int id, int h): id(id), h(h), cnt(1), pre(0), post(0){}

    Node *insert_by_id(int id){
        if(children.find(id)==children.end()){
            Node *new_node = new Node(id, h+1);
            children[id] = new_node;
        }
        else{
            children[id]->cnt++;
        }
        return children[id];
    }
};

int n, minsup, idPre, idPos; // So dong, nguong ho tro toi thieu, gan Pre, gan Post
float min_sup = 0.08;
vector<vector<int>>transactions; // Tap giao dich
vector<vector<int>>freq;
Node *root = new Node();

unordered_map<int,int>mp_count_support; // dem support trong transaction
vector<NList>vt_nlists;

void dfsPrePost(Node *curr){
    curr->pre=idPre++;
    for(auto [u,v]: curr->children) dfsPrePost(v);
    curr->post=idPos++;
}

void buildNLists(Node *curr){
    if(curr!=root){
        if(vt_nlists[curr->id].cnt==0) vt_nlists[curr->id].item.pb(curr->id);
        vt_nlists[curr->id].nl.pb({curr->pre, curr->post, curr->cnt});
        vt_nlists[curr->id].cnt += curr->cnt;
    }
    for(auto [u,v]: curr->children) buildNLists(v);
}

NList intersection(const NList &a, const NList &b){
    NList c;
    int i = 0, j = 0;
    while(i<a.nl.size() && j<b.nl.size()){
        if(a.nl[i].pre < b.nl[j].pre){
            if (a.nl[i].post > b.nl[j].post){
                if(c.nl.size() && c.nl.back().pre == a.nl[i].pre){
                    c.nl.back().cnt += b.nl[j].cnt;
                }
                else{
                    c.nl.pb({a.nl[i].pre, a.nl[i].post, b.nl[j].cnt});
                }
                c.cnt += b.nl[j].cnt;
                j++;
            } else i++;
        } else j++;
    }
    c.item = a.item;
    c.item.pb(b.item.front());
    return c;
}

void doc(){
    // Doc tung dong
    string str, s;
    while(getline(cin, str)){
        stringstream ss(str);
        vector<int>transaction;
        while(getline(ss, s, ' ')) {
            int val = stoi(s);
            transaction.pb(val);
            mp_count_support[val]++;
        }
        transactions.pb(transaction);
        n++;
    }
    minsup = min_sup * n;

    // Loc va sap xep transaction theo thu tu support
    for(vector<int> &transaction: transactions){
        vector<int>tr;

        // Loc cac phan tu trong transaction co support >= minsup
        for(int x: transaction) if(mp_count_support[x]>=minsup) tr.pb(x);

        // Sap xep thu tu cac transaction
        sort(all(tr), [&](const int a, const int b){
            if(mp_count_support[a]!=mp_count_support[b]) return mp_count_support[a]>mp_count_support[b];
            return a<b;
        });

        if(tr.size()) transaction = tr;
    }
}

void eclat(vector<NList>vt_nlists){
    int n = vt_nlists.size();
    for(int i=0; i<n; i++){
        freq.pb(vt_nlists[i].item);
        vector<NList>new_vt_nlists;
        for(int j=i+1; j<n; j++){
            NList nl = intersection(vt_nlists[j], vt_nlists[i]);
            if(nl.cnt>=minsup) new_vt_nlists.pb(nl);
        }
        eclat(new_vt_nlists);
    }
}

void solve(){
    // Chen du lieu vao cay PPC
    for(vector<int> &transaction: transactions){
        Node *curr = root;
        for(int x: transaction)
            curr = curr->insert_by_id(x);
    }

    // Tim PrePost
    idPre = idPos = 0;
    dfsPrePost(root);

    // Tao NList
    vt_nlists.resize(mp_count_support.size()+1);
    buildNLists(root);

    // Loc lai NList
    vector<NList>vt_fill;
    for(NList &x: vt_nlists) if(x.cnt>=minsup) vt_fill.pb(x);
    vt_nlists = vt_fill;

    // Sap xep lai vt_nlists
    sort(all(vt_nlists), [&](const NList &a, const NList &b){
        if(mp_count_support[a.item[0]]!=mp_count_support[b.item[0]])
            return mp_count_support[a.item[0]]<mp_count_support[b.item[0]];
        return a.item[0] > b.item[0];
    });

    // Goi Eclat
    eclat(vt_nlists);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("mushroom.txt", "r", stdin);
//    freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    doc();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;
}


