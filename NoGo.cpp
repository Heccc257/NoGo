#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<set>
#include<conio.h>
#include<ctime>
#include<map>
#include<fstream>
#include<iomanip>
#include<windows.h>
#include<io.h>
#include "Move.cpp"

using namespace std;

int Input_Mode=1;
int chx=1,chy=1;
void Set_xy() {
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(mp[i][j]==-1) {
                chx=i;
                chy=j;
                return ;
            }
        }
    }
}
void Set_xy(int x,int y) {
    chx = x;
    chy = y;
}
int blk = 16;
void Blank() {
    for(int i=1;i<=blk;i++) cout<<" ";
}

void Print() {
    system("cls");
    puts("");

    Blank();

    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
    for(int i=1;i<=N;i++) {

        Blank();

        for(int j=1;j<=N;j++) {
            if(Turn==0&&Input_Mode&&i==chx&&j==chy) printf("╳ ");
            else if(mp[i][j]==1) printf("●");
            else if(mp[i][j]==0) printf("○");
            else if(i==1&&j==1) {
                printf("┏ ");
            } else if(i==1&&j==N) {
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

            Blank();

            for(int j=1;j<=N;j++) printf("｜  ");
            puts("");
        }
    }
    Blank();
    for(int i=1;i<=N;i++) cout<<i<<"   ";cout<<"\n";
}

int Cursor_flag;

void HideCursor(int flag)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = flag; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

namespace Get_File {
    vector<string> files;  
    char buffer[MAX_PATH];   
    char str[30];  
    void getFiles( string path, string path2, vector<string>& files )  
    {  
        //文件句柄  
        long   hFile   =   0;  
        //文件信息  
        struct _finddata_t fileinfo;  
        string p,p2;  
        if((hFile = _findfirst(p.assign(path).append(path2).append("*").c_str(),&fileinfo)) !=  -1)  
        {  
            do  
            {  
                //如果是目录,迭代之  
                //如果不是,加入列表  
                if((fileinfo.attrib &  _A_SUBDIR))  
                {  
                    if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
                        getFiles( p.assign(path).append("\\"),p2.assign(fileinfo.name).append("\\"), files );  
                }  
                else  
                {  
                    files.push_back(p.assign(path2).append(fileinfo.name) );  //这一行可以给出相对路径
                    //files.push_back(p.assign(fileinfo.name) );  //这一行可以给出文件名 
                }
            }while(_findnext(hFile, &fileinfo)  == 0);  
            _findclose(hFile);  
        }  
    }

    void Show_Files() {
        string tem;
        files.clear();
        getcwd(buffer, MAX_PATH);
        string filePath;
        filePath.assign(buffer).append("\\");
        ////获取该路径下的所有文件  
        getFiles(filePath,"", files );  
        int size = files.size();  
        for (int i = 0;i < size;i++)  
        {  
            tem = files[i].c_str();
            if(tem.substr(tem.find_last_of('.')+1)=="nogo") cout<<"     "<<tem<<"\n";
            // cout<<files[i].c_str()<<endl;  
        }
    }
};

bool Read(char *name) {
    string Del = name;
    if(Del.substr(Del.find_last_of('.')+1)!="nogo") strcat(name,".nogo");
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
    cerr<<"Success!\n";
    return 1;
}

void Write(char *name) {
  
    strcat(name,".nogo");
    ofstream File;
    File.open(name,ios::out);
    File<<List.size()<<"\n";
    for(int i=0;i<List.size();i++) {
        File<<List[i].x<<" "<<List[i].y<<"\n";
    }
    for(int i=1;i<=N;i++,File<<"\n")
        for(int j=1;j<=N;j++)
            File<<mp[i][j]<<" ";
}


int Robot_Turn=-1;


namespace Menu {
    int init_flag=0,tag;
    vector<string>cmd;
    void Withdraw() {
        static node tem;
        if(List.size()<=0) {
            cout<<"你不能悔棋!!\n";
        } else {
            tem=List.back();
            printf("悔棋: (%d,%d)\n",tem.x,tem.y);
            Turn^=1;
            mp[tem.x][tem.y]=-1;
            List.pop_back();
            Check_with_ufs::Trace_back();
        }
        // system("pause");
    }
    
    void Init() {
        cmd.push_back("新游戏");
        cmd.push_back("存盘");
        cmd.push_back("读取");
        cmd.push_back("悔棋");
        cmd.push_back("提示");
        cmd.push_back("更改输入方式");
    }

    void open_menu() {
        if(!init_flag) init_flag=1,Init();
        tag=0;

        while(1) {
            system("cls");

            Blank();
            for(int i=1;i<=5;i++) cout<<" ";
            printf("按 ESC 回到棋盘\n");
            puts("");
            // cout<<"tag="<<tag<<"\n";
            for(int i=0;i<cmd.size();i++) {
                Blank();
                if(tag==i) {
                    cout<<setw(10)<<"→ ";
                } else cout<<setw(10)<<" ";
                cout<<cmd[i]<<"  ";
                cout<<"\n";
                if(i==tag) {
                    Blank();
                    cout<<setw(10)<<"--";
                    for(int j=0;j<cmd[i].length();j++) cout<<"-";
                    cout<<"--";
                    cout<<"\n";
                }
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
                        if(cmd[tag]=="新游戏") {
                            ::Init();
                            return ;
                        } else if(cmd[tag]=="悔棋") {
                            Withdraw();
                            system("pause");
                        } else if(cmd[tag]=="存盘") {

                            HideCursor(1);
  
                            printf("input the name of the file:");
                            scanf("%s",str);
                            Write(str);
                            printf("存盘 success\n");

                            HideCursor(Cursor_flag);

                            system("pause");

                        } else if(cmd[tag]=="读取") {

                            HideCursor(1);

                            printf("Files are below:\n");
                            puts("-------------------");
                            Get_File::Show_Files();
                            puts("-------------------");
                            printf("input the name of the file:");
                            scanf("%s",str);

                            if(Read(str)) {
                                
                                
                                HideCursor(Cursor_flag);

                                system("pause");
                                return ;
                            }
                            HideCursor(Cursor_flag);

                        } else if(cmd[tag]=="提示") {
                            int x=0,y=0;
                            Robot::Move(x,y);
                            if(Input_Mode) Set_xy(x,y);
                            printf("Maybe (%d,%d) is OK!!\n",x,y);
                            system("pause");
                            return ;
                        } else if(cmd[tag]=="更改输入方式") {
                            Cursor_flag ^= 1;
                            HideCursor(Cursor_flag);
                            Input_Mode^=1;
                            Set_xy();
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

map<int,int>DX,DY;

int Left(int x,int y) {
    //(x,y)以及其左边第一个空点
    for(int j=y;j>=1;j--) if(mp[x][j]==-1) return j;
    return 0;
}

int Right(int x,int y) {
    //(x,y)以及其右边第一个空点
    for(int j=y;j<=N;j++) if(mp[x][j]==-1) return j;
    return 0;
}

int Find_nearest(int x,int y) {
    if(Left(x,y)) return Left(x,y);
    if(Right(x,y)) return Right(x,y);
    return 0;
}

int Get_cmd_arrow(int &x,int &y) {
    DX[72]=-1,DX[75]=0,DX[77]=0,DX[80]=1;
    DY[72]=0,DY[75]=-1,DY[77]=1,DY[80]=0;
    char c;
    bool zero=0;
    while(1) {
        // cerr<<"233\n";
        if(kbhit()) {
            c=_getch();
            if(c==27) {
                Menu::open_menu();
                return 1;
            }
            if(c==13) {
                //回车
                x=chx,y=chy;
                return 10;
            }
            if(!c||c==224||c==-32) {
                zero=1;
                continue ;
            }
            if(zero) {
                int tx=0,ty=0;
                if(c==72) {
                    for(int i=chx-1;i>=1;i--) {
                        if(Find_nearest(i,chy)) {
                            tx=i;
                            ty=Find_nearest(i,chy);
                            break;
                        }
                    }
                } else if(c==80) {
                    for(int i=chx+1;i<=N;i++) {
                        if(Find_nearest(i,chy)) {
                            tx=i;
                            ty=Find_nearest(i,chy);
                            break;
                        }
                    }
                } else if(c==75) {
                    tx=chx;
                    ty=Left(chx,chy-1);
                } else if(c==77){
                    tx=chx;
                    ty=Right(chx,chy+1);
                }
                if(tx&&ty) {
                    chx=tx,chy=ty;
                    return 2;                    
                }
                zero=0;
            }
        }
    }
    return 0;
}

char str[2233];
int main() {
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,BACKGROUND_RED |BACKGROUND_GREEN |BACKGROUND_BLUE );

    HideCursor(Cursor_flag);

    srand(233);
    Init();
    int Win=0;
    int x=-1,y;

    Menu::open_menu();

    while(1) {
        Print();
        if(!Check_with_ufs::Check_all()) {
            sprintf(str,"Lastgame");
            Write(str);
            if(Turn==1) printf("White Win!!");
            else printf("Black Win!!");
            system("pause");
            return 0;
        }
        Blank();
        if(Input_Mode) {
            printf("按 ESC 打开菜单\n");
        } else {
            printf("输入 'M' 打开菜单\n");
        }
        Blank();
        printf("It's your turn!: ");
        if(Turn) cout<<"●\n";
        else cout<<"○\n";
        int cmd;
        if(Turn==0) {
            // Robot::Rand(x,y);
            if(Input_Mode==1) {
                cmd=Get_cmd_arrow(x,y);
                if(cmd==1) continue ;//菜单
                if(cmd==2) continue ;//改变方向
            } else {
                cmd=Get_cmd(x,y);
                if(cmd) continue ;
            }
        } else {
            Robot::Move(x,y);
        }

        Blank();
        printf("(%d,%d)\n",x,y);
        Sleep(600);
        int ck=Check_with_ufs::Check_xy(x,y,Turn);
        if(ck==1) {
            //success
            move(x,y);
            Set_xy();
            Turn^=1;
            // Sleep(200);
            continue ;
        }

        if(!ck) {
            Blank();
            cout<<"下在该点会导致失败!\n";
        } else if(ck==-1) {
            Blank();
            cout<<"该点已被占据\n";
        } else if(ck==-2) {
            Blank();
            cout<<"不能下在棋盘外!\n";
        }
        Blank();

        printf("再下一次\n");

        Blank();
        system("pause");
    }
    return 0;
}
