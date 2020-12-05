#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<set>
#include<stdlib.h>
#include<algorithm>
#include<ctime>
#include "jsoncpp/json.h" // C++编译时默认包含此库

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
vector<node>List;

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

    int fa[100],q[100];
    //q?????????????????????????????????????????
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

    int Check_xy(int x,int y,int color) { //???????????(x,y)
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


namespace Robot {

    bool Pattern1(int x,int y,int color) {
        if(mp[x][y]!=-1) return 0;
        //只有一个地方可以放子，其他的地方是color的棋子或者边界外
        //
        int qi=0;
        for(int k=0;k<4;k++) {
            int tx=x+dx[k],ty=y+dy[k];
            if(mp[tx][ty]==(color^1)) return 0;
            else if(Check_with_ufs::Check_xy(tx,ty,color)==1) qi++;
        }
        return (qi==1);
    }

    bool Pattern2(int x,int y,int color) {
        //中空
        int num=0;
        for(int k=0;k<4;k++) {
            int tx=x+dx[k],ty=y+dy[k];
            num+=(mp[tx][ty]==color);
        }
        return (num==4);
    }

    int Value1(int color) {
        int ans=0;
        int flag=0;
        int h1=0,h2=0;
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(mp[i][j]==-1) {
                    if(Check_with_ufs::Check_xy(i,j,Turn)==1) {
                        ans+=100;
                        flag|=(color==Turn);
                    }
                    if(Check_with_ufs::Check_xy(i,j,Turn^1)==1) {
                        ans-=100;
                        flag|=(color!=Turn);
                    }
                    if(Pattern1(i,j,Turn)) h1++;
                    if(Pattern1(i,j,Turn^1)) h2++;
                }
            }
        }
        if(!flag) {
            if(color==Turn) return -1e9;
            else return 1e9;
        }
        return ans+10*(h1*h1-h2*h2);
    }

    int global_ans,Time,Limit;
    int res;

    vector<int>st[3][8];
    int d[]={0,1,-1};
    double start_time;
    int dfs_value1(int &x,int &y,int dep,int color,int lst) {

        if(dep==Limit) return Value1(color);

        int ans;
        if(color==Turn) ans=-1e9-1;
        else ans=1e9+1;

        st[0][dep].clear();
        st[1][dep].clear();
        st[2][dep].clear();
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_with_ufs::Check_xy(i,j,color)!=1) continue ;
                int flag=0;
                for(int k=0;k<4;k++) {
                    int tx=i+dx[k],ty=j+dy[k];
                    if(Pattern1(tx,ty,color)) {
                        flag=1;
                    }
                }
                if(Pattern1(i,j,color^1)) flag=1;
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

        for(int e=2;e>=0;e--) {
            for(int t=0;t<st[e][dep].size();t++) {
                int id=st[e][dep][t];
                int j=(id-1)%N+1;
                int i=(id-j)/N+1;

                ++Time;
                if(Time>2e4&&clock()-start_time>850000) return ans;


                Check_with_ufs::Fill(i,j,color);
                int tem=dfs_value1(x,y,dep+1,color^1,ans);
                mp[i][j]=-1;
                Check_with_ufs::Trace_back();
                
                if(color==Turn) ans=max(ans,tem);
                else ans=min(ans,tem);
                
                if(dep==1) {
                    if(global_ans<ans) x=i,y=j,global_ans=ans;
                }
                if(color==Turn) {
                    //取max
                    if(ans>=lst) return ans;
                } else {
                    //去min
                    if(ans<=min(lst,global_ans)) return ans;
                }
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
            }
        }
        
        if(res>=60) {
            for(int i=1;i<=N;i++) {
                for(int j=1;j<=N;j++) {
                    if(mp[i][j]!=-1) continue ;
                    if(Pattern1(i,j,Turn^1)) {
                        if(Check_with_ufs::Check_xy(i,j,Turn)==1) {
                            x=i,y=j;
                        } else {
                            for(int k=0;k<4;k++) {
                                int tx=i+dx[k],ty=j+dy[k];
                                if(Check_with_ufs::Check_xy(tx,ty,Turn)==1) {
                                    x=tx,y=ty;
                                }
                            }
                        }
                        return ;
                    }
                }
            }
        }


        if(Turn==0 || 1  ) {
            start_time=clock();
            Limit = 4;
            if(res<=55) Limit=5;
            else if(res<=40) Limit=6;
            else if(res<=20) Limit=7;
            Time=0;
            global_ans=-1e9;
            dfs_value1(x,y,1,Turn,2e9);
            if(x&&y) {
                // cerr<<"x="<<x<<" y="<<y<<"\n";
                // cerr<<"global_ans="<<global_ans<<"\n";
                // system("pause");
                return ;
            }
        }
        Rand(x,y);
        return ;
    }
}

namespace Robot_uct {
    //UCT
    int tot_id = 1;
    const int M = 3000005;
    int lose[M],tot[M];
    double I[M];
    int access[M];//v节点已经访问的孩子数量
    bool end[M];
    const double c=sqrt(2);
    struct node {
        int id;
        short nxt;
        node() {}
        node(int _id,short _nxt):id(_id),nxt(_nxt) {}
    };
    vector<node>son[M];

    void Init() {
        for(int i=1;i<=tot_id;i++) son[i].clear();
        tot_id=1;
        memset(lose,0,sizeof(lose));
        memset(tot,0,sizeof(tot));
        memset(I,0,sizeof(I));
        memset(access,0,sizeof(access));
        memset(end,0,sizeof(end));
    }

    void Build(int now,int color) {
        //找到所有子节点
        if(end[now]||son[now].size()) return ;
        access[now]=0;
        for(int i=1;i<=N;i++) {
            for(int j=1;j<=N;j++) {
                if(Check_with_ufs::Check_xy(i,j,color)==1) son[now].push_back(node(++tot_id,i*100+j));

            }
        }
        random_shuffle(son[now].begin(),son[now].end());
        if(!son[now].size()) end[now]=1;
    }

    node Best_child(int now) {
        node ans;
        double mx=-1e9;
        for(int i=0;i<son[now].size();i++) {
            int sn=son[now][i].id;
            I[sn]=1.0*(lose[sn]/tot[sn]+c*sqrt(2*log(1.0*tot[now])/tot[sn]));
            if(mx<I[sn]) {
                mx=I[sn];
                ans=son[now][i];
            }
        }
        return ans;
    }

    node Expand(int now) {
        if(access[now]<son[now].size()) {
            access[now]++;
            return son[now][access[now]-1];
        } else {
            return Best_child(now);
        }
    }


    void Rand(int &x,int &y,int color) {
        x=y=0;
        int sx=rand()%N,sy=rand()%N;
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if(Check_with_ufs::Check_xy((sx+i)%N+1,(sy+j)%N+1,color)==1) {
                    x=(sx+i)%N+1,y=(sy+j)%N+1;
                    return ;
                }
            }
        }
    }

    int simulation(int color) {
        //返回失败的次数
        int x,y;
        x=y=0;
        Rand(x,y,color);
        if(!x) return 1;
        Check_with_ufs::Fill(x,y,color);
        int ans=-simulation(color^1);
        mp[x][y]=-1;
        Check_with_ufs::Trace_back();
        return ans;
    }

    vector<node>path;
    void UCT(int &x,int &y) {
        int Time=0;
        int simt=5;
        double start_time=clock();
        node tem;
        while(1) {
            if(Time>=3e4&&clock()-start_time>900000) break;
            if(M-tot_id<4000) break;
            int color=Turn;
            path.clear();
            path.push_back(node(1,0));
            while(1) {
                //找到叶子
                int now=path.back().id;
                if(son[now].size()==0) break;
                tem=Expand(now);
                
                Check_with_ufs::Fill(tem.nxt/100,tem.nxt%100,color);
                color^=1;

                path.push_back(tem);
            }

            //simulation 10次
            
            int ans=0;
            
            Time+=simt*40;

            for(int i=1;i<=simt;i++) {
                ans+=simulation(color);
            }

            Time+=path.size();

            for(int i=path.size()-1;i>=0;i--) {
                node ver=path[i];
                lose[ver.id] += ans;
                ans *= -1;
                tot[ver.id] += simt;
                if(i>0) {
                    Check_with_ufs::Trace_back();
                    mp[ver.nxt/100][ver.nxt%100] = -1;
                }
            }
            //拓展叶子节点
            tem=path.back();
            Build(tem.id,color);
        }

        // system("pause");
        double rate=-1e9;
        for(int i=0;i<son[1].size();i++) {
            int now=son[1][i].id;
            // cerr<<"id="<<now<<" x="<<son[1][i].nxt/100<<" y="<<son[1][i].nxt%100 <<" lose="<<lose[now]<<" tot="<<tot[now] <<" rate="<<1.0*lose[now]/tot[now]<<"\n";
            if(rate<1.0*lose[now]/tot[now]) {
                rate=1.0*lose[now]/tot[now];
                x=son[1][i].nxt/100,y=son[1][i].nxt%100;
            }
        }
        // cerr<<"ansx="<<x<<" ansy="<<y<<"\n";
        // system("pause");
    }
}

void move(int x,int y) {
    Check_with_ufs::Fill(x,y,Turn);
    List.push_back(node(x,y));
}

void Init() {
    memset(mp,-1,sizeof(mp));
    for(int i=0;i<=N+1;i++) mp[0][i]=mp[i][0]=mp[i][N+1]=mp[N+1][i]=2;
    List.clear();
    Turn=1;
    Check_with_ufs::Init();
}

int main()
{
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
    //     if(Turn==1  ) {
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
            move(x+1,y+1);
            Turn^=1;
        }
		x=input["responses"][i]["x"].asInt(), y=input["responses"][i]["y"].asInt();
		if (x!=-1) {
            move(x+1,y+1);
            Turn^=1;
        }
	}
	x=input["requests"][turnID]["x"].asInt(), y=input["requests"][turnID]["y"].asInt();
	if (x!=-1) {
        move(x+1,y+1);
        Turn^=1;
    }
	// 输出决策JSON
	Json::Value ret;
	Json::Value action;
	
	x=y=0;
    
    if(List.size()<=40) {
        Robot::Move(x,y);
    } else {
        Robot_uct::UCT(x,y);
    }

	action["x"]=x-1; action["y"]=y-1;
	ret["response"] = action;
	Json::FastWriter writer;
	
	cout << writer.write(ret) << endl;
	return 0;
}