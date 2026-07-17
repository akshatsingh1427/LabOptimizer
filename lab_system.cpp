
#include <bits/stdc++.h>
using namespace std;

// chrono for real timing
using namespace chrono;

// CONSOLE COLORS
#define RESET   "\033[0m"
#define CYAN    "\033[96m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define RED     "\033[91m"
#define MAGENTA "\033[95m"
#define BLUE    "\033[94m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define WHITE   "\033[97m"

void printBanner() {
    cout << CYAN << BOLD;
    cout << "\n  +================================================================+\n";
    cout <<   "  |    SMART LAB MANAGEMENT & OPTIMIZATION SYSTEM  v2.0            |\n";
    cout <<   "  |    8 CP Algorithms  |  Timing  |  Stress Test  |  Swap Sim     |\n";
    cout <<   "  +================================================================+\n";
    cout << RESET << DIM;
    cout << "\n  Topo Sort | Binary Search | Sliding Window | Two Pointer\n";
    cout <<   "  Prefix Sum | DSU | Tarjan Bridges | Bit Manipulation\n";
    cout << RESET << "\n";
}

void printSection(const string& t) { cout<<"\n"<<YELLOW<<BOLD<<"  ==="<<t<<"==="<<RESET<<"\n"; }
void printOk  (const string& m) { cout<<GREEN  <<"  [OK] "<<RESET<<m<<"\n"; }
void printInfo(const string& m) { cout<<CYAN   <<"  [>>] "<<RESET<<m<<"\n"; }
void printWarn(const string& m) { cout<<YELLOW <<"  [!!] "<<RESET<<m<<"\n"; }
void printErr (const string& m) { cout<<RED    <<"  [XX] "<<RESET<<m<<"\n"; }
void printTime(const string& algo, long long us) {
    cout << MAGENTA << "  [T]  " << RESET << left << setw(28) << algo
         << CYAN << setw(8) << us << " us" << RESET;
    if      (us < 10)   cout << GREEN  << "  [blazing fast]";
    else if (us < 100)  cout << GREEN  << "  [very fast]";
    else if (us < 1000) cout << YELLOW << "  [fast]";
    else                cout << RED    << "  [moderate]";
    cout << RESET << "\n";
}


// TIMER HELPER
struct Timer {
    time_point<high_resolution_clock> start_;
    void start() { start_ = high_resolution_clock::now(); }
    long long us() {
        return duration_cast<microseconds>(high_resolution_clock::now() - start_).count();
    }
};


// DSU  O(alpha*N)
struct DSU {
    vector<int> parent, rnk, sz;
    int components;
    DSU(int n): parent(n), rnk(n,0), sz(n,1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x]!=x) parent[x]=find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        x=find(x); y=find(y);
        if (x==y) return false;
        if (rnk[x]<rnk[y]) swap(x,y);
        parent[y]=x; sz[x]+=sz[y];
        if (rnk[x]==rnk[y]) rnk[x]++;
        components--;
        return true;
    }
    int getSize(int x) { return sz[find(x)]; }
};

// TOPOLOGICAL SORT  O(V+E)
vector<int> topoSort(int S, vector<vector<int>>& adj) {
    vector<int> indeg(S,0);
    for (int u=0;u<S;u++) for (int v:adj[u]) indeg[v]++;
    queue<int> q;
    for (int i=0;i<S;i++) if (indeg[i]==0) q.push(i);
    vector<int> order;
    while (!q.empty()) {
        int u=q.front(); q.pop();
        order.push_back(u);
        for (int v:adj[u]) if (--indeg[v]==0) q.push(v);
    }
    return order;
}

// BINARY SEARCH - Min Labs  O(N log N)
int minLabsRequired(vector<int>& caps, int N) {
    vector<int> s=caps;
    sort(s.rbegin(),s.rend());
    vector<int> pre((int)s.size()+1,0);
    for (int i=0;i<(int)s.size();i++) pre[i+1]=pre[i]+s[i];
    int lo=1,hi=(int)s.size(),ans=(int)s.size();
    while (lo<=hi) {
        int mid=(lo+hi)/2;
        if (pre[mid]>=N) { ans=mid; hi=mid-1; } else lo=mid+1;
    }
    return ans;
}


// PREFIX SUM  O(N)
vector<int> prefixSum(vector<int>& arr) {
    vector<int> p((int)arr.size()+1,0);
    for (int i=0;i<(int)arr.size();i++) p[i+1]=p[i]+arr[i];
    return p;
}

// CHEATING DETECTION  O(N)
struct CheatingCluster { int labId, branchId, studentCount; vector<int> ids; };

vector<CheatingCluster> detectCheating(
    vector<tuple<int,int,int>>& students, vector<int>& labAlloc, int thr=3)
{
    map<pair<int,int>, vector<int>> groups;
    for (int i=0;i<(int)students.size();i++) {
        if (labAlloc[i]<0) continue;
        groups[make_pair(labAlloc[i], get<2>(students[i]))].push_back(get<0>(students[i]));
    }
    vector<CheatingCluster> out;
    for (auto it=groups.begin();it!=groups.end();++it) {
        if ((int)it->second.size()>=thr) {
            CheatingCluster c;
            c.labId=it->first.first; c.branchId=it->first.second;
            c.studentCount=(int)it->second.size(); c.ids=it->second;
            out.push_back(c);
        }
    }
    sort(out.begin(),out.end(),
         [](const CheatingCluster& a,const CheatingCluster& b){ return a.studentCount>b.studentCount; });
    return out;
}

// TWO POINTER - Overlaps  O(N log N)
vector<pair<int,int>> detectOverlaps(vector<pair<int,int>>& iv) {
    vector<pair<pair<int,int>,int>> v;
    for (int i=0;i<(int)iv.size();i++) v.push_back(make_pair(iv[i],i));
    sort(v.begin(),v.end());
    vector<pair<int,int>> res;
    int l=0;
    for (int r=1;r<(int)v.size();r++) {
        if (v[l].first.second>v[r].first.first) res.push_back(make_pair(v[l].second,v[r].second));
        else l=r;
    }
    return res;
}


// TARJAN BRIDGES  O(V+E)
struct Tarjan {
    int n, timer_;
    vector<vector<int>>& adj;
    vector<int> disc,low;
    vector<bool> vis;
    vector<pair<int,int>> bridges;
    Tarjan(int n_,vector<vector<int>>& a)
        : n(n_),timer_(0),adj(a),disc(n_,-1),low(n_,0),vis(n_,false) {}
    void dfs(int u,int par) {
        vis[u]=true; disc[u]=low[u]=timer_++;
        for (int v:adj[u]) {
            if (!vis[v]) {
                dfs(v,u); low[u]=min(low[u],low[v]);
                if (low[v]>disc[u]) bridges.push_back(make_pair(u,v));
            } else if (v!=par) low[u]=min(low[u],disc[v]);
        }
    }
    vector<pair<int,int>> run() {
        for (int i=0;i<n;i++) if (!vis[i]) dfs(i,-1);
        return bridges;
    }
};


// BIT ENCODER  O(1)
struct BitEncoder {
    vector<long long> mask;
    BitEncoder(int L): mask(L,0LL) {}
    void set(int lab,int slot)  { mask[lab]|=(1LL<<(slot%32)); }
    bool get(int lab,int slot)  { return (mask[lab]>>(slot%32))&1; }
    int  cnt(int lab)           { return __builtin_popcountll(mask[lab]); }
};


// ALLOCATION ENGINE  (shared by greedy & optimal)
struct AllocResult {
    vector<int> labAlloc;   // student i -> lab index
    vector<int> labLoad;    // how many in each lab
    double wastedPct;       // wasted capacity %
    int cheatingClusters;
    int maxLoad, minLoad;
    string strategy;
};

// GREEDY: assign student to first lab with space (sorted by capacity desc)
AllocResult allocGreedy(vector<tuple<int,int,int>>& students,
                        vector<int>& caps, vector<int>& execOrder,
                        map<int,vector<int>>& subjStudents, int L)
{
    vector<int> labOrder(L); iota(labOrder.begin(),labOrder.end(),0);
    sort(labOrder.begin(),labOrder.end(),[&](int a,int b){ return caps[a]>caps[b]; });
    vector<int> labLoad(L,0), labAlloc((int)students.size(),-1);
    vector<vector<int>> stepLog; // step-by-step log

    for (int subj : execOrder) {
        auto it=subjStudents.find(subj);
        if (it==subjStudents.end()) continue;
        for (int si : it->second) {
            for (int li : labOrder) {
                if (labLoad[li]<caps[li]) {
                    labAlloc[si]=li; labLoad[li]++;
                    break;
                }
            }
        }
    }

    int totalCap=0; for (int c:caps) totalCap+=c;
    int N=(int)students.size();
    auto clusters=detectCheating(students,labAlloc);
    int mx=*max_element(labLoad.begin(),labLoad.end());
    int mn=*min_element(labLoad.begin(),labLoad.end());

    AllocResult r;
    r.labAlloc=labAlloc; r.labLoad=labLoad;
    r.wastedPct=(totalCap-N)*100.0/totalCap;
    r.cheatingClusters=(int)clusters.size();
    r.maxLoad=mx; r.minLoad=mn;
    r.strategy="GREEDY";
    return r;
}

// OPTIMAL: round-robin balanced allocation (minimizes max load)
AllocResult allocOptimal(vector<tuple<int,int,int>>& students,
                         vector<int>& caps, vector<int>& execOrder,
                         map<int,vector<int>>& subjStudents, int L)
{
    // Use priority queue: always assign to lab with most remaining capacity
    // This is optimal for minimizing maximum load (load balancing)
    typedef pair<int,int> pii; // (remaining_cap, lab_idx)
    priority_queue<pii> pq;
    for (int i=0;i<L;i++) pq.push(make_pair(caps[i],i));

    vector<int> labLoad(L,0), labAlloc((int)students.size(),-1);

    for (int subj : execOrder) {
        auto it=subjStudents.find(subj);
        if (it==subjStudents.end()) continue;
        for (int si : it->second) {
            if (pq.empty()) break;
            pii top=pq.top(); pq.pop();
            int li=top.second;
            int rem=top.first;
            if (rem>0) {
                labAlloc[si]=li; labLoad[li]++;
                pq.push(make_pair(rem-1, li));
            }
        }
    }

    int totalCap=0; for (int c:caps) totalCap+=c;
    int N=(int)students.size();
    auto clusters=detectCheating(students,labAlloc);
    int mx=*max_element(labLoad.begin(),labLoad.end());
    int mn=*min_element(labLoad.begin(),labLoad.end());

    AllocResult r;
    r.labAlloc=labAlloc; r.labLoad=labLoad;
    r.wastedPct=(totalCap-N)*100.0/totalCap;
    r.cheatingClusters=(int)clusters.size();
    r.maxLoad=mx; r.minLoad=mn;
    r.strategy="OPTIMAL";
    return r;
}


struct SwapResult {
    int labA, labB;
    int before_cheating, after_cheating;
    int before_imbalance, after_imbalance;
    bool beneficial;
};

SwapResult simulateSwap(vector<tuple<int,int,int>>& students,
                        vector<int> labAlloc, // copy intentional
                        vector<int>& caps, int labA, int labB, int L)
{
    // Count before
    auto clustersBefore = detectCheating(students, labAlloc);
    int beforeCheat=(int)clustersBefore.size();
    vector<int> loadBefore(L,0);
    for (int x:labAlloc) if (x>=0) loadBefore[x]++;
    int beforeImb = loadBefore[labA] - loadBefore[labB];
    if (beforeImb<0) beforeImb=-beforeImb;

    // Perform swap: all students in labA go to labB and vice versa
    for (int i=0;i<(int)labAlloc.size();i++) {
        if (labAlloc[i]==labA) labAlloc[i]=labB;
        else if (labAlloc[i]==labB) labAlloc[i]=labA;
    }

    // Count after
    auto clustersAfter = detectCheating(students, labAlloc);
    int afterCheat=(int)clustersAfter.size();
    vector<int> loadAfter(L,0);
    for (int x:labAlloc) if (x>=0) loadAfter[x]++;
    int afterImb = loadAfter[labA] - loadAfter[labB];
    if (afterImb<0) afterImb=-afterImb;

    SwapResult r;
    r.labA=labA; r.labB=labB;
    r.before_cheating=beforeCheat; r.after_cheating=afterCheat;
    r.before_imbalance=beforeImb;  r.after_imbalance=afterImb;
    r.beneficial=(afterCheat<beforeCheat) || (afterImb<beforeImb);
    return r;
}

// STRESS TEST  - generates random data and times it
struct StressResult {
    int N;
    long long totalUs;
    long long topoUs, bsearchUs, dsuUs, slidingUs, tarjanUs, prefixUs;
    int minLabsFound;
};

StressResult runStressTest(int N, int L, int S, int D_req) {
    // Generate random data
    srand(42); // fixed seed for reproducibility
    vector<tuple<int,int,int>> students(N);
    for (int i=0;i<N;i++)
        students[i]=make_tuple(i+1, rand()%S, rand()%5);

    vector<int> caps(L);
    for (int i=0;i<L;i++) caps[i]=max(10, N/L + rand()%100 - 50);

    vector<vector<int>> adj(S);
    int D=min(D_req, S-1);
    // Build random DAG (no cycles)
    for (int i=0;i<D&&i<S-1;i++) adj[i].push_back(i+1);

    map<int,vector<int>> subjStudents;
    for (int i=0;i<N;i++) subjStudents[get<1>(students[i])].push_back(i);

    Timer t;
    StressResult r; r.N=N;

    // 1. Topo sort
    t.start(); auto topo=topoSort(S,adj); r.topoUs=t.us();

    // 2. Binary search
    t.start(); r.minLabsFound=minLabsRequired(caps,N); r.bsearchUs=t.us();

    // 3. Prefix sum
    t.start(); auto pre=prefixSum(caps); r.prefixUs=t.us();

    // 4. DSU
    t.start();
    DSU dsu(N);
    for (auto it=subjStudents.begin();it!=subjStudents.end();++it) {
        const vector<int>& idx=it->second;
        for (int i=1;i<(int)idx.size();i++) dsu.unite(idx[0],idx[i]);
    }
    r.dsuUs=t.us();

    // 5. Allocation + sliding window
    t.start();
    vector<int> labOrder(L); iota(labOrder.begin(),labOrder.end(),0);
    sort(labOrder.begin(),labOrder.end(),[&](int a,int b){ return caps[a]>caps[b]; });
    vector<int> labLoad(L,0), labAlloc(N,-1);
    for (int subj: (topo.size()==(size_t)S ? topo : vector<int>())) {
        auto it=subjStudents.find(subj);
        if (it==subjStudents.end()) continue;
        for (int si:it->second)
            for (int li:labOrder)
                if (labLoad[li]<caps[li]) { labAlloc[si]=li; labLoad[li]++; break; }
    }
    auto clusters=detectCheating(students,labAlloc);
    r.slidingUs=t.us();

    // 6. Tarjan
    t.start();
    vector<vector<int>> labAdj(L);
    set<pair<int,int>> edgeSet;
    for (int i=0;i<N&&i<5000;i++) { // sample for stress test
        int br1=get<2>(students[i]);
        for (int j=i+1;j<min(N,i+20);j++) {
            int br2=get<2>(students[j]);
            if (br1==br2&&labAlloc[i]!=labAlloc[j]&&labAlloc[i]>=0&&labAlloc[j]>=0) {
                pair<int,int> e=make_pair(min(labAlloc[i],labAlloc[j]),max(labAlloc[i],labAlloc[j]));
                if (!edgeSet.count(e)) {
                    edgeSet.insert(e);
                    labAdj[labAlloc[i]].push_back(labAlloc[j]);
                    labAdj[labAlloc[j]].push_back(labAlloc[i]);
                }
            }
        }
    }
    Tarjan tarjan(L,labAdj);
    tarjan.run();
    r.tarjanUs=t.us();

    r.totalUs=r.topoUs+r.bsearchUs+r.prefixUs+r.dsuUs+r.slidingUs+r.tarjanUs;
    return r;
}

// ══════════════════════════════════════════════════
// STEP-BY-STEP ALLOCATION LOG
// Records each student assignment for animation
// ══════════════════════════════════════════════════
struct AllocStep {
    int studentId, subjectId, branchId, labId, labLoadAfter;
};

vector<AllocStep> buildStepLog(vector<tuple<int,int,int>>& students,
                               vector<int>& labAlloc, vector<int>& labLoad,
                               map<int,vector<int>>& subjStudents,
                               vector<int>& execOrder, vector<int>& caps, int L)
{
    vector<int> labOrder(L); iota(labOrder.begin(),labOrder.end(),0);
    sort(labOrder.begin(),labOrder.end(),[&](int a,int b){ return caps[a]>caps[b]; });
    vector<int> load(L,0);
    vector<AllocStep> steps;

    for (int subj : execOrder) {
        auto it=subjStudents.find(subj);
        if (it==subjStudents.end()) continue;
        for (int si : it->second) {
            for (int li : labOrder) {
                if (load[li]<caps[li]) {
                    load[li]++;
                    AllocStep s;
                    s.studentId=get<0>(students[si]);
                    s.subjectId=get<1>(students[si]);
                    s.branchId =get<2>(students[si]);
                    s.labId=li;
                    s.labLoadAfter=load[li];
                    steps.push_back(s);
                    break;
                }
            }
        }
    }
    return steps;
}

// ══════════════════════════════════════════════════
// INPUT HELPER
// ══════════════════════════════════════════════════
int getInt(const string& p, int lo, int hi) {
    int v;
    while (true) {
        cout << CYAN << "  > " << RESET << p;
        if (cin>>v && v>=lo && v<=hi) return v;
        cin.clear(); cin.ignore(10000,'\n');
        cout << RED << "  [XX] Invalid! " << lo << " to " << hi << RESET << "\n";
    }
}

// ══════════════════════════════════════════════════
// JSON HELPERS
// ══════════════════════════════════════════════════
void writeIntArray(ofstream& out, const string& key, const vector<int>& arr) {
    out << "  \"" << key << "\": [";
    for (int i=0;i<(int)arr.size();i++) { out<<arr[i]; if(i+1<(int)arr.size()) out<<","; }
    out << "],\n";
}

void writeDoubleArray(ofstream& out, const string& key, const vector<double>& arr) {
    out << "  \"" << key << "\": [";
    for (int i=0;i<(int)arr.size();i++) { out<<fixed<<setprecision(1)<<arr[i]; if(i+1<(int)arr.size()) out<<","; }
    out << "],\n";
}

// ══════════════════════════════════════════════════
// MAIN
// ══════════════════════════════════════════════════
int main() {
    printBanner();

    // ── STEP 1: Configuration ────────────────────
    printSection(" STEP 1 — System Configuration ");
    int N = getInt("Number of students  (1-200000) : ", 1, 200000);
    int L = getInt("Number of labs      (1-100)    : ", 1, 100);
    int S = getInt("Number of subjects  (1-50)     : ", 1, 50);
    int D = getInt("Number of dependencies (0-500) : ", 0, 500);

    // ── STEP 2: Students ─────────────────────────
    printSection(" STEP 2 — Student Data ");
    printInfo("Format: student_id  subject_id  branch_id");
    printInfo("subject_id: 0 to " + to_string(S-1) + "  |  branch_id: 0 to 9");
    cout << "\n";

    vector<tuple<int,int,int>> students(N);
    for (int i=0;i<N;i++) {
        cout << DIM << "  [" << setw(4) << (i+1) << "/" << N << "] " << RESET;
        int id,subj,branch;
        while (true) {
            cout << CYAN << "id subj branch: " << RESET;
            if (cin>>id>>subj>>branch && subj>=0&&subj<S&&branch>=0&&branch<=9) {
                students[i]=make_tuple(id,subj,branch); break;
            }
            cin.clear(); cin.ignore(10000,'\n');
            cout << RED << "  Invalid!\n" << RESET;
            cout << DIM << "  [" << setw(4) << (i+1) << "/" << N << "] " << RESET;
        }
    }
    printOk("Student data recorded.");

    // ── STEP 3: Dependencies ─────────────────────
    vector<vector<int>> adjDep(S);
    if (D>0) {
        printSection(" STEP 3 — Subject Dependencies ");
        printInfo("subjectA must complete BEFORE subjectB");
        cout << "\n";
        for (int i=0;i<D;i++) {
            int a,b;
            cout << DIM << "  [" << (i+1) << "/" << D << "] " << RESET;
            while (true) {
                cout << CYAN << "subjectA subjectB: " << RESET;
                if (cin>>a>>b&&a>=0&&a<S&&b>=0&&b<S&&a!=b) { adjDep[a].push_back(b); break; }
                cin.clear(); cin.ignore(10000,'\n');
                cout << RED << "  Invalid!\n" << RESET;
                cout << DIM << "  [" << (i+1) << "/" << D << "] " << RESET;
            }
        }
        printOk("Dependencies recorded.");
    }

    // ── STEP 4: Lab Capacities ───────────────────
    printSection(" STEP 4 — Lab Capacities ");
    vector<int> caps(L);
    for (int i=0;i<L;i++)
        caps[i]=getInt("Capacity of Lab " + to_string(i) + " : ", 1, 10000);
    printOk("Capacities recorded.");

    // ── SAVE input.txt ───────────────────────────
    {
        ofstream fin("input.txt");
        fin<<N<<" "<<L<<" "<<S<<" "<<D<<"\n";
        for (int i=0;i<N;i++)
            fin<<get<0>(students[i])<<" "<<get<1>(students[i])<<" "<<get<2>(students[i])<<"\n";
        for (int u=0;u<S;u++) for (int v:adjDep[u]) fin<<u<<" "<<v<<"\n";
        for (int i=0;i<L;i++) fin<<caps[i]<<(i+1<L?" ":"\n");
    }
    printOk("input.txt saved!");

    // ════════════════════════════════════════════
    // RUN ALL 8 ALGORITHMS WITH REAL TIMING
    // ════════════════════════════════════════════
    printSection(" PROCESSING — Running All Algorithms with Timing ");
    cout << "\n";

    Timer t;
    map<string,long long> timings;

    // Build subject->students map
    map<int,vector<int>> subjStudents;
    for (int i=0;i<N;i++) subjStudents[get<1>(students[i])].push_back(i);

    // 1. Topological Sort
    cout << "  " << MAGENTA << "[1/8]" << RESET << " Topological Sort...          ";
    cout.flush();
    t.start(); auto topoOrder=topoSort(S,adjDep); timings["topo"]=t.us();
    bool hasCycle=(int)topoOrder.size()<S;
    printTime("", timings["topo"]);

    // 2. Binary Search
    cout << "  " << MAGENTA << "[2/8]" << RESET << " Binary Search (min labs)...  ";
    cout.flush();
    t.start(); int minL=minLabsRequired(caps,N); timings["bsearch"]=t.us();
    printTime("", timings["bsearch"]);

    // 3. Prefix Sum
    cout << "  " << MAGENTA << "[3/8]" << RESET << " Prefix Sum...                ";
    cout.flush();
    t.start(); auto prefix=prefixSum(caps); int totalCap=prefix[L]; timings["prefix"]=t.us();
    printTime("", timings["prefix"]);

    // 4. DSU + Allocation
    cout << "  " << MAGENTA << "[4/8]" << RESET << " DSU (grouping)...            ";
    cout.flush();
    t.start();
    DSU dsu(N);
    for (auto it=subjStudents.begin();it!=subjStudents.end();++it) {
        const vector<int>& idx=it->second;
        for (int i=1;i<(int)idx.size();i++) dsu.unite(idx[0],idx[i]);
    }
    timings["dsu"]=t.us();
    printTime("", timings["dsu"]);

    // ── GREEDY vs OPTIMAL ALLOCATION ────────────
    cout << "  " << MAGENTA << "[4b]" << RESET << " Greedy Allocation...         ";
    cout.flush();
    vector<int> execSeq = hasCycle ? vector<int>() : topoOrder;
    if (hasCycle) for (int i=0;i<S;i++) execSeq.push_back(i);

    t.start();
    AllocResult greedyRes = allocGreedy(students,caps,execSeq,subjStudents,L);
    timings["greedy"]=t.us();
    printTime("", timings["greedy"]);

    cout << "  " << MAGENTA << "[4c]" << RESET << " Optimal Allocation...        ";
    cout.flush();
    t.start();
    AllocResult optimalRes = allocOptimal(students,caps,execSeq,subjStudents,L);
    timings["optimal"]=t.us();
    printTime("", timings["optimal"]);

    // Use greedy as primary allocation
    vector<int>& labAlloc = greedyRes.labAlloc;
    vector<int>& labLoad  = greedyRes.labLoad;

    // 5. Sliding Window Cheating
    cout << "  " << MAGENTA << "[5/8]" << RESET << " Sliding Window (cheating)... ";
    cout.flush();
    t.start();
    auto clusters=detectCheating(students,labAlloc);
    timings["sliding"]=t.us();
    printTime("", timings["sliding"]);

    // 6. Two Pointer
    cout << "  " << MAGENTA << "[6/8]" << RESET << " Two Pointer (overlaps)...    ";
    cout.flush();
    t.start();
    vector<pair<int,int>> intervals;
    for (int i=0;i<L;i++) intervals.push_back(make_pair(i*2, i*2+1+labLoad[i]/10));
    auto overlaps=detectOverlaps(intervals);
    timings["twoptr"]=t.us();
    printTime("", timings["twoptr"]);

    // 7. Tarjan
    cout << "  " << MAGENTA << "[7/8]" << RESET << " Tarjan (critical labs)...    ";
    cout.flush();
    t.start();
    vector<vector<int>> labAdj(L);
    set<pair<int,int>> edgeSet;
    for (int i=0;i<N;i++) {
        int br1=get<2>(students[i]);
        for (int j=i+1;j<min(N,i+80);j++) {
            int br2=get<2>(students[j]);
            if (br1==br2&&labAlloc[i]!=labAlloc[j]&&labAlloc[i]>=0&&labAlloc[j]>=0) {
                pair<int,int> e=make_pair(min(labAlloc[i],labAlloc[j]),max(labAlloc[i],labAlloc[j]));
                if (!edgeSet.count(e)) {
                    edgeSet.insert(e);
                    labAdj[labAlloc[i]].push_back(labAlloc[j]);
                    labAdj[labAlloc[j]].push_back(labAlloc[i]);
                }
            }
        }
    }
    Tarjan tarjan(L,labAdj);
    auto bridges=tarjan.run();
    set<int> critSet;
    for (int bi=0;bi<(int)bridges.size();bi++) {
        critSet.insert(bridges[bi].first);
        critSet.insert(bridges[bi].second);
    }
    timings["tarjan"]=t.us();
    printTime("", timings["tarjan"]);

    // 8. Bit Manipulation
    cout << "  " << MAGENTA << "[8/8]" << RESET << " Bit Manipulation...          ";
    cout.flush();
    t.start();
    BitEncoder enc(L);
    for (int slot=0;slot<(int)topoOrder.size()&&slot<32;slot++) {
        auto it=subjStudents.find(topoOrder[slot]);
        if (it==subjStudents.end()) continue;
        for (int si:it->second) if (labAlloc[si]>=0) enc.set(labAlloc[si],slot);
    }
    timings["bits"]=t.us();
    printTime("", timings["bits"]);

    long long totalUs=0;
    for (auto& kv:timings) totalUs+=kv.second;

    // ── STEP-BY-STEP LOG ─────────────────────────
    auto stepLog=buildStepLog(students,labAlloc,labLoad,subjStudents,execSeq,caps,L);

    // ── LAB SWAP SIMULATIONS ─────────────────────
    vector<SwapResult> swaps;
    for (int a=0;a<L&&a<4;a++)
        for (int b=a+1;b<L&&b<4;b++)
            swaps.push_back(simulateSwap(students,labAlloc,caps,a,b,L));

    // ── STRESS TEST ───────────────────────────────
    printSection(" STRESS TEST — Performance at Scale ");
    vector<StressResult> stressResults;
    vector<int> stressSizes = {100, 1000, 10000, 50000, 100000, 200000};
    cout << "\n";
    cout << DIM << "  N          | Topo   | BSearch| DSU    | Sliding| Tarjan | TOTAL\n" << RESET;
    cout << DIM << "  -----------+--------+--------+--------+--------+--------+----------\n" << RESET;
    for (int sz : stressSizes) {
        StressResult sr=runStressTest(sz, min(20, sz/5+1), min(20, sz/10+2), min(10, sz/20+1));
        stressResults.push_back(sr);
        cout << "  N=" << setw(8) << left << sz << "| "
             << setw(7) << sr.topoUs << "| "
             << setw(7) << sr.bsearchUs << "| "
             << setw(7) << sr.dsuUs << "| "
             << setw(7) << sr.slidingUs << "| "
             << setw(7) << sr.tarjanUs << "| "
             << GREEN << sr.totalUs << " us" << RESET << "\n";
    }

    // ════════════════════════════════════════════
    // RESULTS
    // ════════════════════════════════════════════
    printSection(" RESULTS SUMMARY ");
    cout << "\n";
    cout << BOLD << "  +--------------------------------------------------+\n" << RESET;
    cout << BOLD << "  |                SYSTEM SUMMARY                    |\n" << RESET;
    cout << BOLD << "  +--------------------------------------------------+\n" << RESET;
    cout << BOLD << "  |" << RESET << "  Students        : " << CYAN    << setw(6) << N          << RESET << "                       " << BOLD << "|\n" << RESET;
    cout << BOLD << "  |" << RESET << "  Labs            : " << BLUE    << setw(6) << L          << RESET << "                       " << BOLD << "|\n" << RESET;
    cout << BOLD << "  |" << RESET << "  Total Capacity  : " << GREEN   << setw(6) << totalCap   << RESET << "                       " << BOLD << "|\n" << RESET;
    cout << BOLD << "  |" << RESET << "  Min Labs Needed : " << GREEN   << setw(6) << minL       << RESET << "  [Binary Search]       " << BOLD << "|\n" << RESET;
    cout << BOLD << "  |" << RESET << "  DSU Groups      : " << YELLOW  << setw(6) << dsu.components << RESET << "  [DSU O(a*N)]         " << BOLD << "|\n" << RESET;
    cout << BOLD << "  |" << RESET << "  Cheating Clusters: "<<(clusters.size()>0?RED:GREEN)<<setw(5)<<clusters.size()<<RESET<<"  [Sliding Window]      "<<BOLD<<"|\n"<<RESET;
    cout << BOLD << "  |" << RESET << "  Critical Labs   : " <<(critSet.size()>0?RED:GREEN)<<setw(6)<<critSet.size()<<RESET<<"  [Tarjan]              "<<BOLD<<"|\n"<<RESET;
    cout << BOLD << "  |" << RESET << "  Cycle in Deps   : " <<(hasCycle?RED:GREEN)<<setw(6)<<(hasCycle?"YES":"NO")<<RESET<<"  [Topo Sort]           "<<BOLD<<"|\n"<<RESET;
    cout << BOLD << "  +--------------------------------------------------+\n" << RESET;

    // Greedy vs Optimal comparison
    cout << "\n" << YELLOW << BOLD << "  Greedy vs Optimal Allocation:" << RESET << "\n";
    cout << "  Strategy  | MaxLoad | MinLoad | Wasted% | CheatingClusters\n";
    cout << "  ----------+---------+---------+---------+-----------------\n";
    cout << "  GREEDY    | " << setw(7) << greedyRes.maxLoad  << " | " << setw(7) << greedyRes.minLoad
         << " | " << setw(7) << fixed << setprecision(1) << greedyRes.wastedPct
         << " | " << greedyRes.cheatingClusters << "\n";
    cout << "  OPTIMAL   | " << setw(7) << optimalRes.maxLoad << " | " << setw(7) << optimalRes.minLoad
         << " | " << setw(7) << optimalRes.wastedPct
         << " | " << optimalRes.cheatingClusters << "\n";

    // Lab utilization bars
    cout << "\n" << YELLOW << BOLD << "  Lab Utilization:" << RESET << "\n";
    for (int i=0;i<L;i++) {
        double pct=caps[i]>0?(100.0*labLoad[i]/caps[i]):0;
        int fill=(int)(pct/5.0);
        string bar(fill,'#'), empty(20-fill,'.');
        string col=pct>80?RED:pct>50?YELLOW:GREEN;
        cout<<"  Lab "<<setw(2)<<i<<"  ["<<col<<bar<<RESET<<DIM<<empty<<RESET<<"]  "
            <<setw(5)<<fixed<<setprecision(1)<<pct<<"%"
            <<"  ("<<labLoad[i]<<"/"<<caps[i]<<")\n";
    }

    // Timing summary
    cout << "\n" << YELLOW << BOLD << "  Algorithm Timing Summary:" << RESET << "\n";
    printTime("  Topological Sort",   timings["topo"]);
    printTime("  Binary Search",      timings["bsearch"]);
    printTime("  Prefix Sum",         timings["prefix"]);
    printTime("  DSU",                timings["dsu"]);
    printTime("  Greedy Alloc",       timings["greedy"]);
    printTime("  Optimal Alloc",      timings["optimal"]);
    printTime("  Sliding Window",     timings["sliding"]);
    printTime("  Two Pointer",        timings["twoptr"]);
    printTime("  Tarjan Bridges",     timings["tarjan"]);
    printTime("  Bit Manipulation",   timings["bits"]);
    cout << GREEN << "  Total: " << totalUs << " us (" << (totalUs/1000.0) << " ms)\n" << RESET;

    // Subject execution order
    cout << "\n" << YELLOW << BOLD << "  Subject Execution Order:" << RESET << "\n  ";
    if (hasCycle) cout << RED << "  Cycle detected!\n" << RESET;
    else { for (int i=0;i<(int)topoOrder.size();i++) { cout<<CYAN<<"S"<<topoOrder[i]<<RESET; if(i+1<(int)topoOrder.size()) cout<<" -> "; } cout<<"\n"; }

    // Swap simulations
    if (!swaps.empty()) {
        cout << "\n" << YELLOW << BOLD << "  Lab Swap Simulations:" << RESET << "\n";
        cout << "  LabA-LabB | Before Cheat | After Cheat | Beneficial?\n";
        for (auto& sw:swaps) {
            cout << "  Lab" << sw.labA << " <-> Lab" << sw.labB
                 << " | " << setw(12) << sw.before_cheating
                 << " | " << setw(11) << sw.after_cheating
                 << " | " << (sw.beneficial ? GREEN"YES" : RED"NO") << RESET << "\n";
        }
    }

    // ════════════════════════════════════════════
    // SAVE output.txt
    // ════════════════════════════════════════════
    {
        ofstream out("output.txt");
        out << "{\n";

        // Basic info
        out<<"  \"N\": "<<N<<",\n";
        out<<"  \"L\": "<<L<<",\n";
        out<<"  \"S\": "<<S<<",\n";
        out<<"  \"D\": "<<D<<",\n";
        out<<"  \"minLabsRequired\": "<<minL<<",\n";
        out<<"  \"totalCapacity\": "<<totalCap<<",\n";
        out<<"  \"hasCycle\": "<<(hasCycle?"true":"false")<<",\n";
        out<<"  \"studentGroups\": "<<dsu.components<<",\n";

        writeIntArray(out,"subjectOrder",topoOrder);
        writeIntArray(out,"labLoads",labLoad);
        writeIntArray(out,"labCapacities",caps);

        // Cheating clusters
        out<<"  \"cheatingClusters\": [\n";
        for (int i=0;i<(int)clusters.size();i++) {
            out<<"    {\"labId\":"<<clusters[i].labId<<",\"branchId\":"<<clusters[i].branchId<<",\"count\":"<<clusters[i].studentCount<<"}";
            if (i+1<(int)clusters.size()) out<<",";
            out<<"\n";
        }
        out<<"  ],\n";

        // Critical labs
        out<<"  \"criticalLabs\": [";
        bool first=true;
        for (int x:critSet) { if(!first) out<<","; out<<x; first=false; }
        out<<"],\n";

        // Bit masks
        vector<int> maskVec(enc.mask.begin(), enc.mask.end());
        writeIntArray(out,"labSlotMasks",maskVec);

        // Timings (microseconds - REAL measured values)
        out<<"  \"timings\": {\n";
        out<<"    \"topoSort\": "    <<timings["topo"]    <<",\n";
        out<<"    \"binarySearch\": "<<timings["bsearch"] <<",\n";
        out<<"    \"prefixSum\": "   <<timings["prefix"]  <<",\n";
        out<<"    \"dsu\": "         <<timings["dsu"]     <<",\n";
        out<<"    \"slidingWindow\":"<<timings["sliding"] <<",\n";
        out<<"    \"twoPointer\": "  <<timings["twoptr"]  <<",\n";
        out<<"    \"tarjan\": "      <<timings["tarjan"]  <<",\n";
        out<<"    \"bitManip\": "    <<timings["bits"]    <<",\n";
        out<<"    \"greedyAlloc\": " <<timings["greedy"]  <<",\n";
        out<<"    \"optimalAlloc\": "<<timings["optimal"] <<",\n";
        out<<"    \"totalUs\": "     <<totalUs            <<"\n";
        out<<"  },\n";

        // Greedy vs Optimal
        out<<"  \"greedyAlloc\": {";
        out<<"\"maxLoad\":"<<greedyRes.maxLoad<<",\"minLoad\":"<<greedyRes.minLoad;
        out<<",\"wastedPct\":"<<fixed<<setprecision(1)<<greedyRes.wastedPct;
        out<<",\"cheatingClusters\":"<<greedyRes.cheatingClusters;
        out<<",\"loads\":[";
        for (int i=0;i<L;i++) { out<<greedyRes.labLoad[i]; if(i+1<L) out<<","; }
        out<<"]},\n";

        out<<"  \"optimalAlloc\": {";
        out<<"\"maxLoad\":"<<optimalRes.maxLoad<<",\"minLoad\":"<<optimalRes.minLoad;
        out<<",\"wastedPct\":"<<fixed<<setprecision(1)<<optimalRes.wastedPct;
        out<<",\"cheatingClusters\":"<<optimalRes.cheatingClusters;
        out<<",\"loads\":[";
        for (int i=0;i<L;i++) { out<<optimalRes.labLoad[i]; if(i+1<L) out<<","; }
        out<<"]},\n";

        // Swap simulations
        out<<"  \"swapSimulations\": [\n";
        for (int i=0;i<(int)swaps.size();i++) {
            auto& sw=swaps[i];
            out<<"    {\"labA\":"<<sw.labA<<",\"labB\":"<<sw.labB
               <<",\"beforeCheating\":"<<sw.before_cheating
               <<",\"afterCheating\":"<<sw.after_cheating
               <<",\"beforeImbalance\":"<<sw.before_imbalance
               <<",\"afterImbalance\":"<<sw.after_imbalance
               <<",\"beneficial\":"<<(sw.beneficial?"true":"false")<<"}";
            if (i+1<(int)swaps.size()) out<<",";
            out<<"\n";
        }
        out<<"  ],\n";

        // Stress test results
        out<<"  \"stressTest\": [\n";
        for (int i=0;i<(int)stressResults.size();i++) {
            auto& sr=stressResults[i];
            out<<"    {\"N\":"<<sr.N
               <<",\"topoUs\":"<<sr.topoUs
               <<",\"bsearchUs\":"<<sr.bsearchUs
               <<",\"dsuUs\":"<<sr.dsuUs
               <<",\"slidingUs\":"<<sr.slidingUs
               <<",\"tarjanUs\":"<<sr.tarjanUs
               <<",\"totalUs\":"<<sr.totalUs<<"}";
            if (i+1<(int)stressResults.size()) out<<",";
            out<<"\n";
        }
        out<<"  ],\n";

        // Step-by-step allocation log (first 200 steps max)
        int maxSteps=min((int)stepLog.size(), 200);
        out<<"  \"allocationSteps\": [\n";
        for (int i=0;i<maxSteps;i++) {
            auto& s=stepLog[i];
            out<<"    {\"studentId\":"<<s.studentId
               <<",\"subjectId\":"<<s.subjectId
               <<",\"branchId\":"<<s.branchId
               <<",\"labId\":"<<s.labId
               <<",\"labLoad\":"<<s.labLoadAfter<<"}";
            if (i+1<maxSteps) out<<",";
            out<<"\n";
        }
        out<<"  ],\n";

        // Lab usage %
        vector<double> usagePct;
        for (int i=0;i<L;i++) usagePct.push_back(caps[i]>0?(100.0*labLoad[i]/caps[i]):0);
        writeDoubleArray(out,"labUsagePct",usagePct);

        out<<"  \"version\": \"2.0\"\n";
        out<<"}\n";
        out.close();
    }

    cout << "\n  ====================================================\n";
    printOk("input.txt  saved");
    printOk("output.txt saved  (includes timings, greedy/optimal, stress test, swap sim)");
    printInfo("Open dashboard.html in browser and load output.txt!");
    cout << "  ====================================================\n\n";

    cout << DIM << "  Press Enter to exit..." << RESET;
    cin.ignore(10000,'\n'); cin.get();
    return 0;
}