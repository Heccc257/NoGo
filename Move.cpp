#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<set>
#include<stdlib.h>
#include<algorithm>
#include<ctime>
// #include "jsoncpp/json.h" // C++编译时默认包含此库

using namespace std;


const int N = 9;
int mp[15][15];
int dx[] = {-1,0,1,0},dy[]= {0,1,0,-1};
bool Turn = 1;
int x=5,y=5;
struct node {
    int x,y;
    node(int _x,int _y):x(_x),y(_y){};
    node() {x=y=0;}
};



void Print() {
    system("cls");
    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(mp[i][j]==1) printf("X");
            else if(mp[i][j]==0) printf("O");
            else printf("+");
            if(j!=N) printf("---");
        }
        printf(" %d\n",i);
        if(i!=N) {
            for(int j=1;j<=N;j++) printf("|   ");
            puts("");
        }
    }
    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
}



namespace Check_with_ufs {
    //???????????check????

    int op_Time=1;
    struct node {
        // trace back
        int *p,v,t;
        node(int *_p,int _v,int _t):p(_p),v(_v),t(_t) {}
        node() {}
    };

    vector<node>st;

    int fa[90],q[90];
    inline int id(int x,int y) {return (x-1)*N+y;}
    inline int Getf(int v) {
        while(v!=fa[v]) v=fa[v];
        return v;
    }


    inline void Change(int &x,int v) {
        st.push_back(node(&x,x,op_Time));
        x=v;
    }

    void Trace_back() {
        static node tem;
        while(st.back().t==op_Time) {
            tem=st.back();
            *tem.p=tem.v;
            st.pop_back();
        }
        op_Time--;
    }
    void Union(int x,int y) {
        x=Getf(x),y=Getf(y);
        if(x==y) return ;
        if(rand()&1) swap(x,y);

        Change(fa[x],y);
        Change(q[y],q[x]+q[y]);
    }

    void Fill(int x,int y,int color) {

        //??????????trace back
        op_Time++;  //????????
        mp[x][y]=color;

        int now=id(x,y);

        //???????????????
        q[now]=0;
        for(int k=0;k<4;k++) q[now]+=(mp[x+dx[k]][y+dy[k]]==-1);

        for(int k=0;k<4;k++) {
            int tx=x+dx[k],ty=y+dy[k];
            if(mp[tx][ty]==2||mp[tx][ty]==-1) continue ;
            
            int nxt=Getf(id(tx,ty));
            Change(q[nxt],q[nxt]-1);
            if(mp[tx][ty]==color) {
                Union(now,nxt);
            }
        }
    }

    inline int Check_xy(int x,int y,int color) { //???????????(x,y)
        //1 ????
        //-1 ?????
        //-2 ???
        //0 ????

        if(x<1||x>N||y<1||y>N) return -2;
        if(mp[x][y]!=-1) return -1;

        int flag=1;
        static vector<int>st[2];

        st[0].clear(),st[1].clear();
        int qxy=0;//xy????
        for(int k=0;k<4;k++) {
            int tx=x+dx[k],ty=y+dy[k];
            if(mp[tx][ty]==-1) qxy++;
            else if(mp[tx][ty]==0||mp[tx][ty]==1) st[mp[tx][ty]].push_back(Getf(id(tx,ty)));
        }
        
        int tem;


        //?????????????
        for(int i=0;i<st[color^1].size();i++) {
            tem=st[color^1][i];
            q[tem]--;
            // cout<<"tem="<<tem<<" "<<q[tem]<<"\n";
            if(!q[tem]) flag=0;
        }
        for(int i=0;i<st[color^1].size();i++) q[st[color^1][i]]++;
        if(!flag) return 0;

        if(qxy) return 1;//x y 如果有气则一定成立
        if(st[color].size()==0) return 0; //????????????

        for(int i=0;i<st[color].size();i++) {
            tem=st[color][i];
            q[tem]--;
        }

        //相同棋子
        flag=0;
        for(int i=0;i<st[color].size();i++) if(q[st[color][i]]) flag=1;
        for(int i=0;i<st[color].size();i++) q[st[color][i]]++;
        
        return flag;

    }

    bool Check_all() {
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_xy(i,j,Turn)==1) return 1;
            }
        }
        return 0;
    }
    void Init() {
        for(int i=1;i<=N*N;i++) fa[i]=i,q[i]=0;
        st.clear();
        op_Time=0;
        int a;
        st.push_back(node(&a,0,0));
    }
    int QI(int x,int y) {
        return q[Getf(id(x,y))];
    }
}


vector<node>List;
void Withdraw() {
    static node tem;
    if(List.size()<=0) {
        cout<<"???????!!\n";
    } else {
        cout<<"withdraw succeed\n";
        tem=List.back();
        Turn^=1;
        mp[tem.x][tem.y]=-1;
        List.pop_back();
        Check_with_ufs::Trace_back();
    }
    // system("pause");
}

void move(int x,int y) {
    Check_with_ufs::Fill(x,y,Turn);
    List.push_back(node(x,y));
}


char ts[20000];
namespace Robot {
    inline bool Pattern1(int x,int y,int color) {
        //只有一个气，其他的地方是color的棋子或者边界外
        //
        if(Check_with_ufs::Check_xy(x,y,color)!=1) return 0;

        int qi=0;
        for(int k=0;k<4;k++) {
            int tx=x+dx[k],ty=y+dy[k];
            if(mp[tx][ty]==(color^1)) return 0;
            else if(Check_with_ufs::Check_xy(x,y,color)==1) qi++;
        }
        return (qi==1);
    }
    inline bool Eye(int x,int y,int color) {
        return Check_with_ufs::Check_xy(x,y,color)==1&&Check_with_ufs::Check_xy(x,y,color^1)!=1;     
    }
    bool ok[N][N];
    int Count_hk(int color) {

        int ans=0;
        for(int i=1;i<=N;i++)
            for(int j=1;j<=N;j++)
                ans+=Pattern1(i,j,color);
        return ans;

        memset(ok,0,sizeof(ok));
        for(int i=1;i<=N;i++)
            for(int j=1;j<=N;j++)
                ok[i][j]=(Check_with_ufs::Check_xy(i,j,color)==1);
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(ok[i][j]) {
                    for(int k=0;k<4;k++) {
                        
                    }
                }
            }
        }
    }

    int Value1(int color) {
        int ans=0;
        int qi=0;
        int flag=0;
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(mp[i][j]==-1) {
                    if(Check_with_ufs::Check_xy(i,j,Turn)==1) {
                        ans++;
                        flag|=(color==Turn);
                    }
                    if(Check_with_ufs::Check_xy(i,j,Turn^1)==1) {
                        ans--;
                        flag|=(color!=Turn);
                    }
                }  else if(mp[i][j]==Turn) {
                    for(int k=0;k<4;k++) {
                        qi+=(mp[i+dx[k]][j+dy[k]]==-1);
                    }
                } else {
                    for(int k=0;k<4;k++) {
                        qi-=(mp[i+dx[k]][j+dy[k]]==-1);
                    }
                }
            }
        }
        if(!flag) {
            if(color==Turn) return -1e9;
            else return 1e9;
        }
        return ans*100+qi;
    }

    int global_ans,Time,Limit;
    int res;

    vector<int>st[3][10];    
    int d[]={0,1,-1};
    int dfs_value1(int &x,int &y,int dep,int color,int lst) {

        if(dep==Limit) return Value1(color);

        int ans;
        if(color==Turn) ans=-1e9;
        else ans=1e9;

        ++Time;
        if(Time>3e5) return ans;
        st[0][dep].clear();
        st[1][dep].clear();
        st[2][dep].clear();

        int pre_hk=Count_hk(color^1);

        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_with_ufs::Check_xy(i,j,color)!=1) continue ;
                
                if(Eye(i,j,color)) {//先不堵眼
                   st[0][dep].push_back((i-1)*N+j);
                   continue ;
                }

                int flag=0;
                for(int k=0;k<4;k++) {
                    int tx=i+dx[k],ty=j+dy[k];
                    if(Pattern1(tx,ty,color)||Pattern1(tx,ty,color^1)) flag=1;
                }

                if(Pattern1(i,j,color^1)) flag=1;
                if(!flag) {
                    Check_with_ufs::Fill(i,j,color^1);
                    int now=Count_hk(color^1);
                    if(now-pre_hk>=1) flag=1;
                    mp[i][j]=-1;
                    Check_with_ufs::Trace_back();
                }

                if(flag) {
                    st[2][dep].push_back((i-1)*N+j);
                    continue ;
                }
                bool t=0;
                for(int x=0;x<3;x++)
                    for(int y=0;y<3;y++)
                        t|=(mp[i+d[x]][j+d[y]]==0||mp[i+d[x]][j+d[y]]==1);
                st[t][dep].push_back((i-1)*N+j);
            }
        }
        random_shuffle(st[0][dep].begin(),st[0][dep].end());
        random_shuffle(st[1][dep].begin(),st[1][dep].end());
        random_shuffle(st[2][dep].begin(),st[2][dep].end());

        for(int e=2;e>=0;e--) {
            for(int t=0;t<st[e][dep].size();t++) {
                int id=st[e][dep][t];
                int j=(id-1)%N+1;
                int i=(id-j)/N+1;

                Check_with_ufs::Fill(i,j,color);
                int tem=dfs_value1(x,y,dep+1,color^1,ans);
                
                if(e==2&&dep==1&&tem>ans) {
                    ans = tem;
                    Limit++;
                    tem=dfs_value1(x,y,dep+1,color^1,ans);
                    Limit--;
                }

                mp[i][j]=-1;
                Check_with_ufs::Trace_back();
                
                if(color==Turn) ans=max(ans,tem);
                else ans=min(ans,tem);
                
                if(dep==1) {
                    if(global_ans<ans) x=i,y=j,global_ans=ans;
                } else if(color!=Turn&&ans<=global_ans) {
                    return ans;
                }
                if((color!=Turn&&ans<=lst) || (color==Turn&&ans>=lst)) return ans;
                
            }
        }
        return ans;
    }

    bool dfs_all(int color,int &x,int &y,int dep) {
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_with_ufs::Check_xy(i,j,color)!=1) continue ;

                Check_with_ufs::Fill(i,j,color);
                int result=dfs_all(color^1,x,y,dep+1);
                mp[i][j]=-1;
                Check_with_ufs::Trace_back();

                if(!result) {
                    if(dep==1) x=i,y=j;
                    return 1;
                }
            }
        }
        return 0;
    }

    void Rand(int &x,int &y) {
        int sx=rand()%N+1;
        int sy=rand()%N+1;
        for(int i=sx,t1=1;t1<=N;i=i%N+1,t1++) {
            for(int j=sy,t2=1;t2<=N;j=j%N+1,t2++) {
                if(Check_with_ufs::Check_xy(i,j,Turn)==1) {
                    x=i,y=j;
                    return ;
                }
            }
        }
    }

    void Move(int &x,int &y) {
        x=y=0;
        res=0;
        
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_with_ufs::Check_xy(i,j,0)==1||Check_with_ufs::Check_xy(i,j,1)==1) res++;
            }
        }

        if(res<=16) {
            if(dfs_all(Turn,x,y,1)) {
                if(x&&y) return ;
            } else {
                
            }
        }


        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(mp[i][j]!=-1) continue ;
                if(Pattern1(i,j,Turn^1)&&Check_with_ufs::Check_xy(i,j,Turn)==1) {
                    x=i,y=j;
                    return ;
                }
            }
        }

        if(Turn==0 || 1  ) {
            Limit=5;
            if(res<=35&&res>=15&&rand()%3==0) Limit=7;
            Limit = 4;
            Time=0;
            global_ans=-1e9;
            dfs_value1(x,y,1,Turn,1e9);
            if(List.size()>=30&&Time<=1e4) {
                Limit++;
                global_ans=-1e9;
                dfs_value1(x,y,1,Turn,1e9);
            }
            if(x&&y) {
                // cerr<<"x="<<x<<" y="<<y<<"\n";
                // cerr<<"global_ans="<<global_ans<<"\n";
                // cerr<<"val="<<Value1(Turn)<<" "<<Count_hk(Turn)<<" "<<Count_hk(Turn^1) <<"\n";
                // system("pause");
                return ;
            }
        }
        Rand(x,y);
        return ;
    }
}

void Init() {
    memset(mp,-1,sizeof(mp));
    for(int i=0;i<=N+1;i++) mp[0][i]=mp[i][0]=mp[i][N+1]=mp[N+1][i]=2;
    List.clear();
    Turn=1;
    Check_with_ufs::Init();
}

int main() {
    // srand(233);
    // Init();
    // while(1) {
    //     Print();
    //     if(!Check_with_ufs::Check_all()) {
    //         if(Turn==1) printf("White Win!!");
    //         else printf("Black Win!!");
    //         return 0;
    //     }
    //     int x,y;
    //     printf("input 'M' to see the menu\n");
    //     printf("it's your turn!: %c\n",Turn?'X':'O');
    //     int cmd;
    //     if(Turn==0  ) {
    //         cin>>x>>y;
    //      } else Robot::Move(x,y);
    //     // if(cmd==1) continue ;

    //     printf("(%d,%d)\n",x,y);
    //     int ck=Check_with_ufs::Check_xy(x,y,Turn);
    //     if(ck==1) {
    //         //success
    //         move(x,y);
    //         Turn^=1;
    //         continue ;
    //     }
    //     printf("place again\n");
    //     system("pause");
    // }

	srand((unsigned)time(0));
    Init();
	string str;
    str.clear();
	int x=-1,y;
	// 读入JSON
	getline(cin,str);
	//getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input); 
	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	for (int i = 0; i < turnID; i++)
	{
		x=input["requests"][i]["x"].asInt(), y=input["requests"][i]["y"].asInt();
		if (x!=-1) {
            Check_with_ufs::Fill(x+1,y+1,Turn);
            Turn^=1;
        }
		x=input["responses"][i]["x"].asInt(), y=input["responses"][i]["y"].asInt();
		if (x!=-1) {
            Check_with_ufs::Fill(x+1,y+1,Turn);
            Turn^=1;
        }
	}
	x=input["requests"][turnID]["x"].asInt(), y=input["requests"][turnID]["y"].asInt();
	if (x!=-1) {
        Check_with_ufs::Fill(x+1,y+1,Turn);
        Turn^=1;
    }
	// 输出决策JSON
	Json::Value ret;
	Json::Value action;
	
	x=y=0;
    Robot::Move(x,y);

	action["x"]=x-1; action["y"]=y-1;
	ret["response"] = action;
	Json::FastWriter writer;
	
	cout << writer.write(ret) << endl;
	return 0;
}
/*
1   2   3   4   5   6   7   8   9
O---+---+---+---O---+---O---+---O 1 
|   |   |   |   |   |   |   |   |   
+---X---+---+---+---O---X---O---X 2 
|   |   |   |   |   |   |   |   |   
+---+---X---+---+---+---+---X---+ 3 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---+---X---+---+ 4 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---X---+---+---+ 5 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---+---+---+---+ 6 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---+---+---+---+ 7 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---+---+---+---+ 8 
|   |   |   |   |   |   |   |   |   
+---+---+---+---+---+---+---+---+ 9 
1   2   3   4   5   6   7   8   9  
*/