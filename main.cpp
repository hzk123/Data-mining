#include <bits/stdc++.h>
#include <time.h>
#define ll long long
#define clr(x,y) memset(x,y,sizeof(x))
#define pii pair<int,int>
#define pb(x) push_back(x)
#define mkii(x,y) make_pair(x,y)
#define pb(x) push_back(x)

using namespace std;
const int N = 200010;
const int p = 2333333;
const int seed = 1331;

int cnt = 0 , tot = 0 , mx = 0;

vector<int> tail[N];
struct Fp_tree
{

    int label , num;
    map<int,int> go;

    int pre;
    void Fp_tree_init( int _label, int _num) ;
    void Insert(int r , int fa) ;
} FP[400010];


void Fp_tree::Fp_tree_init( int _label, int _num)
    {
        label = _label;
        num = _num;
        go.clear();
    }

void Fp_tree::Insert(int r , int fa)
{
    if ( go.find(r) != go.end() )
            FP[go[r]].num++;
    else
    {
        go[r] = ++cnt;
        tail[r].push_back(cnt);
        FP[go[r]].Fp_tree_init( r , 1);
        FP[go[r]].pre = fa;
    }
}

vector<int> st[100000];
int Count[100010] , rk[100010];
bool cmp1(pii a,pii b) {return a.second > b.second ; }
bool cmp2(int a,int b) {return rk[a] < rk[b] ; };
void input()
{
    //#define filein
    #ifdef filein
    freopen("test.txt","r",stdin);
    freopen("out.txt","w",stdout);
    #endif // filein
    clr(Count , 0 );
    int a,b;
    while( scanf("%d%d",&a,&b)!=EOF)
         swap(a,b),st[a].pb(b) , Count[b]++ , mx = max(mx,b) , tot = max(tot,a);
    vector<pii> p;
    for (int i = 0 ; i <= mx ; i++)
        p.push_back( mkii(i,Count[i]) );
    sort ( p.begin() , p.end() ,  cmp1 );
    for (int i = 0 ; i <= mx ; i++)
        rk[ p[i].first ] = i + 1;
}

void Insert_Data( int minsup )
{
    cnt = 0 ;
    FP[0].Fp_tree_init( -1 , 1);
    FP[0].pre = -1;
    int now , r;
    for (int i = 1 ; i <= tot; i++)
    {
        now = 0;
        sort( st[i].begin() , st[i].end() , cmp2);
        for (int j = 0 ; j < st[i].size(); j++)
        {
            if ( Count[st[i][j]] >= minsup )
            FP[now].Insert(st[i][j] , now);
            now = FP[now].go[st[i][j]];
        }
    }
}

pii par[2333] ;
int rear = 0;

const int Minsupport = 0.;
const int Minconfidence = 0.;

void showFrequentItemset()
{
    double support = par[rear].second * 1.0 / tot ;
    if ( support < Minsupport || rear == 1) return ;
    printf("[");
    for (int i = 2 ; i <= rear ; i++)
       printf("%d ",par[i].first);
    printf("]  support = %f \n",support);
}

void calc()
{
    for (int i = 2 ; i <= rear ; ++i)
        printf("[%d,%d] ",par[i].first,par[i].second);
    printf("\n");
}
void dfs(int now)
{
    par[++rear] =  mkii(FP[now].label , FP[now].num);
    calc();
   // showFrequentItemset();
    if (FP[now].go.size() != 0)
        for (const auto &r : FP[now].go ) dfs(r.second);
    rear--;
}

vector<set<pii> > vs[1010];
set<pii> get(int id , int minsup)
{
    int now = id;
    set<pii> st;
    while ( FP[now].pre != -1 )
    {
        st.insert(mkii(FP[now].label,FP[now].num));
        now = FP[now].pre;
    }
    return st;
}

set<pii> st2;
void Finditem(int minsup)
{
    for (int i = 1 ; i <= mx ; i++)
        {
            for (const auto &r : tail[i])
            {
                    st2 = get(r,minsup);
                    if (!st2.empty())
                    vs[i].pb(st2);
            }
        }
//
//    for (int i = 1 ; i <= mx ; i++)
//        {
//            if (!vs[i].empty())
//            {
//                printf("%d :",i);
//                for (const auto &r : vs[i])
//                {
//                    printf("[ ");
//                    for (const auto &t : r)
//                    printf("%d,%d ",t.first,t.second);
//                    printf("] ");
//                }
//                printf("\n");
//            }
//        }
}


void gen()
{

}
int main()
{
    input();
    Insert_Data( 10 );
    //dfs(0);
    cout << "Build tree success" << endl;
    Finditem( 10 );

    return 0;
}


/*
1 1
1 2
1 3
2 1
2 3
3 4
3 5


*/
