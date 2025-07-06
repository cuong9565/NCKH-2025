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

struct item{
    vector<int>itemSet;
    vector<int>tid;
    item() = default;
    item(int item){itemSet.pb(item);}

    string get_item(){
        string str = "{";
        int n = itemSet.size();
        if(n>0){
            str += to_string(itemSet[0]);
            for(int i=1; i<n; i++) str += "," + to_string(itemSet[i]);
        }
        str += "} => ";
        for(int &x: tid) str += to_string(x+1) + " ";
        return str;
    }
};

struct TransactionInfo{
    float twu = 0;
    vector<int>quantities;
    TransactionInfo() = default;
};

struct ItemInfo{
    int parent = 0;
    float weights = 0;
    ItemInfo() = default;
};

float sum_twu, min_wus = 0.6; // Tong twu, nguong toi thieu wus
map<int,vector<int>>mp_hierarchy; // Mot map luu quan he cha con trong cay phan cap
vector<ItemInfo> itemTable;
vector<TransactionInfo>transactionTable;
vector<item>F, freq;

vector<string>split(const string &str){
    vector<string>result;
    stringstream ss(str);
    string s;
    while(getline(ss, s, ' ')) if(s!="") result.pb(s);
    return result;
}

void doc_cay_hierachy(){
    int n;
    cin >> n; cin.ignore(); // Doc so dong
    f1(i,n){
        string line; getline(cin, line);
        vector<string> vt_str = split(line);
        int m = vt_str.size();
        int parent = stoi(vt_str[0]);
        for(int i=1; i<m; i++){
            int child = stoi(vt_str[i]);
            mp_hierarchy[parent].pb(child);
        }
    }
}

void doc_weights(){
    int n;
    float value;
    cin >> n; cin.ignore();
    itemTable.resize(n + mp_hierarchy.size() + 1);
    f1(i,n) cin >> value, itemTable[i].weights = value;
}

void doc_transaction(){
    int n; cin >> n; cin.ignore(); // So giao dich trong csdl
    f1(i,n){
        string line; getline(cin, line);
        vector<int>transaction;
        vector<string>vt = split(line);
        transaction.pb(0);
        for(string &x: vt) transaction.pb(stoi(x));
        transactionTable.pb({0, transaction});
    }
}

void calculate_weights(int node){
    if(itemTable[node].weights==0){
        float sum = 0;
        for(int &child_node: mp_hierarchy[node]){
            if(itemTable[child_node].weights==0)
                calculate_weights(child_node);
            sum += itemTable[child_node].weights;
            itemTable[child_node].parent = node;
        }
        itemTable[node].weights = sum;
    }
}

void calculate_twu(){
    for(auto &inf: transactionTable){
        float tu = 0, mau = 0;
        f1(i,int(inf.quantities.size()-1)){
            tu += itemTable[i].weights * inf.quantities[i];
            mau += inf.quantities[i];
        }
        inf.twu = tu/mau;
        sum_twu += inf.twu;
    }
}

float calculate_wus(item it){
    float s = 0;
    for(int &x: it.tid) s += transactionTable[x].twu;
    return s/sum_twu;
}

item intersection(item a, item b){
    int m = a.tid.size(), n = b.tid.size(), i = 0, j = 0;
    item c;
    // tim itemSet
    c.itemSet = a.itemSet;
    c.itemSet.pb(b.itemSet.back());

    // tim tid
    while(i<m && j<n){
        if(a.tid[i]==b.tid[j]) {
            c.tid.pb(a.tid[i]);
            i++;
            j++;
        }
        else if(a.tid[i]>b.tid[j]) j++;
        else i++;
    }
    return c;
}

void add_parent(){
    for(auto &inf: transactionTable){
        vector<int>tr(itemTable.size(), 0);
        tr[0] = 0;
        for(int i=1; i<int(inf.quantities.size()); i++){
            int curr = i;
            int quantity = inf.quantities[i];
            while(curr!=0){
                tr[curr] += quantity;
                curr = itemTable[curr].parent;
            }
        }
        inf.quantities = tr;
    }
}

void eclat(vector<item> F){
    int n = F.size();
    for(int i=0; i<n; i++){
        if(calculate_wus(F[i])>=min_wus){
            freq.pb(F[i]);
            vector<item>new_F;
            for(int j=i+1; j<n; j++){
                item new_it = intersection(F[i], F[j]);
                if(calculate_wus(new_it)>=min_wus)
                    new_F.pb(new_it);
            }
            eclat(new_F);
        }
    }
}

void mine_fwuis(){
    // Tinh trong so node cha
    for(auto &[u,v]: mp_hierarchy) calculate_weights(u);
    add_parent(); // Tinh quantity cua node cha
    calculate_twu(); // Tinh twu cua tung transaction

    // Them item vao F
    int n = transactionTable[0].quantities.size();
    for(int i=1; i<n; i++){
        item it(i);
        for(int j=0; j<int(transactionTable.size()); j++){
            if(transactionTable[j].quantities[i])
                it.tid.pb(j);
        }
        if(calculate_wus(it)>=min_wus)
            F.pb(it);
    }

    // Goi de quy eclat
    eclat(F);
}

void print_result(){
    for(auto &x: freq) cout << x.get_item(), el;
    cout << freq.size(), el;
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
//    freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    doc_cay_hierachy(); // Doc cay phan cap
    doc_weights();
    doc_transaction();
    mine_fwuis();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    print_result();
    cout << duration.count() << " ms" , el;
}


