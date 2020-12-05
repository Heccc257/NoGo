#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<set>
#include<conio.h>
#include<ctime>
#include<fstream>
#include<iomanip>
#include<windows.h>
#include "Move.cpp"

using namespace std;

void Print() {
    system("cls");
    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(mp[i][j]==1) printf("○");
            else if(mp[i][j]==0) printf("●");
            else if(i==1&&j==1) {
                printf("┏ ");
            }
            else if(i==1&&j==N) {
                printf("┓ ");
            } else if(i==N&&j==1) {
                printf("┗ ");
            } else if(i==N&&j==N) {
                printf("┛ ");
            }
            else if(i==1) {
                printf("┳ ");
            } else if(i==N) {
                printf("┻ ");
            } else if(j==1) {
                printf("┣ ");
            } else if(j==N) {
                printf("┫ ");
            } else {
                printf("╋ ");
            }
            if(j!=N) printf("—");
        }
        printf(" %d\n",i);
        if(i!=N) {
            for(int j=1;j<=N;j++) printf("｜  ");
            puts("");
        }
    }
    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
}

void Write(char *name) {
    ofstream File;
    File.open(name,ios::out);
    cerr<<"name: "<<name<<'\n';
    cerr<<"size="<<List.size()<<"\n";
    File<<List.size()<<"\n";
    for(int i=0;i<List.size();i++) {
        File<<List[i].x<<" "<<List[i].y<<"\n";
    }
    for(int i=1;i<=N;i++,File<<"\n")
        for(int j=1;j<=N;j++)
            File<<mp[i][j]<<" ";
}

bool Read(char *name) {
    ifstream File;
    File.open(name,ios::in);
    if(!File) {
        printf("No such file");
        system("pause");
        return 0;
    }
    Init();
    int Size;
    File>>Size;
    node tem;
    for(int i=1;i<=Size;i++) {
        File>>tem.x>>tem.y;
        List.push_back(tem);
        Check_with_ufs::Fill(tem.x,tem.y,Turn);
        Turn^=1;
    }
    return 1;
}

int Robot_Turn=-1;
namespace Menu {
    int init_flag=0,tag;
    vector<string>cmd;
    
    void Withdraw() {
        static node tem;
        if(List.size()<=0) {
            cout<<"you can't withdraw!!\n";
        } else {
            tem=List.back();
            printf("withdraw: (%d,%d)\n",tem.x,tem.y);
            Turn^=1;
            mp[tem.x][tem.y]=-1;
            List.pop_back();
            Check_with_ufs::Trace_back();
        }
        // system("pause");
    }
    
    void Init() {
        cmd.push_back("New game");
        cmd.push_back("Save");
        cmd.push_back("Read File");
        cmd.push_back("Withdraw");
        cmd.push_back("Help");
    }

    void open_menu() {
        if(!init_flag) init_flag=1,Init();
        tag=0;

        while(1) {
            system("cls");
            printf("press Esc to go back to the baord\n");
            // cout<<"tag="<<tag<<"\n";
            for(int i=0;i<cmd.size();i++) {
                if(tag==i) cout<<setw(9)<<"→ ";
                else cout<<setw(10)<<" ";
                cout<<cmd[i]<<"  ";
                cout<<"\n";
            }
            int c;
            int zero=0;
            while(1) {
                //
                //75  77
                if(kbhit()) {
                    c=_getch();
                    if(c==27) return ;
                    if(zero) {
                        //??????
                        if(c==75&&tag>0) tag--;
                        if(c==77&&tag<cmd.size()-1) tag++;
                        break;
                    }

                    if(!c||c==224) zero=1;
                    static char str[233];
                    if(c==13) {
                        system("cls");
                        if(cmd[tag]=="New game") {
                            ::Init();
                            return ;
                        } else if(cmd[tag]=="Withdraw") {
                            cout<<"233\n";
                            Withdraw();
                            system("pause");
                        } else if(cmd[tag]=="Save") {
                            printf("input the name of the file:");
                            scanf("%s",str);
                            Write(str);
                            printf("Save success\n");
                            system("pause");
                        } else if(cmd[tag]=="Read File") {
                            printf("input the name of the file:");
                            scanf("%s",str);
                            if(Read(str)) {
                                system("pause");
                                return ;
                            }
                        } else if(cmd[tag]=="Help") {
                            int x=0,y=0;
                            Robot::Move(x,y);
                            printf("Maybe (%d,%d) is OK!!\n",x,y);
                            system("pause");
                            return ;
                        }
                        break;
                    }
                }
            }
        }
    }
}

int Get_cmd(int &x,int &y) {
    x=y=0;
    int c;
    while(1) {
        c=getchar();
        // cout<<"c="<<c<<"\n";
        if(c=='M'||c=='m') {
            cout<<"CCCC\n";
            cout<<"********";
            Menu::open_menu();
            return 1;
        }
        if('0'<=c&&c<='9') {
            x=c-'0';
            break;
        }
    }
    while(1) {
        c=getchar();
        if('0'<=c&&c<='9') {
            y=c-'0';
            break;
        }
    }
    return 0;
}

char str[2233];
int main() {

//     Init();
//     mp[1][1]=1;
//     mp[2][2]=mp[3][3]=mp[4][4]=mp[5][4]=-1;
//     Print();
//     cout<<"○●〇●—- —";
//     system("pause");
// return 0;

    srand(233);
    Init();
    int Win=0;
    int x=-1,y;
    Menu::open_menu();
    while(1) {
        Print();
        if(!Check_with_ufs::Check_all()) {
            if(Turn==1) printf("White Win!!");
            else printf("Black Win!!");
            return 0;
        }

        printf("input 'M' to see the menu\n");
        printf("it's your turn!: %c\n",Turn?'X':'O');

        int cmd;
        if(Turn==0   ) {
            cmd=Get_cmd(x,y);
            if(cmd) continue ;
        } else Robot::Move(x,y);

        printf("(%d,%d)\n",x,y);
        Sleep(400);
        int ck=Check_with_ufs::Check_xy(x,y,Turn);
        if(ck==1) {
            //success
            move(x,y);
            Turn^=1;
            // Sleep(200);
            continue ;
        }

        if(!ck) {
            cout<<"you can't place here!\n";
        } else if(ck==-1) {
            cout<<"-1\n";
        } else if(ck==-2) {
            cout<<"out of the board!\n";
        }
        printf("place again\n");
        system("pause");
    }
    return 0;
}
