#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <math.h>
#include <move.h>
#include <stack>
#include <cmath>
#include <queue>

extern Player Players[2+5];
extern int width, height;
extern char Map[20 + 5][20 + 5];
extern int IngredientCount;
extern struct Ingredient Ingredient[20 + 5];
extern int recipeCount;
extern struct Recipe Recipe[20 + 5];
extern int totalTime, randomizeSeed, totalOrderCount;
extern struct Order totalOrder[20 + 5];
extern int orderCount;  
extern struct Order Order[20 + 5];
extern int entityCount;
extern struct Entity Entity[20 + 5];
extern int status;

char bfs_map[20+5][20+5];
bool lable[20+5][20+5];
int father[20+5][20+5];

//double error_d=0.06;//允许的误差范围
//double error_u=0.96;//NUM=1,2226 points //NUM=2,2747
//double error_d=0.05;//允许的误差范围
//double error_u=0.989;//NUM=3,2681 points
double error_d1=0.4;//允许的误差范围
double error_u1=0.6;//NUM=3,2681 points
//double error_d=0.3;//允许的误差范围
//double error_u=0.7;//NUM=4,2103
double error_d=0.2;//允许的误差范围
double error_u=0.8;//NUM=4,2103
//double error_d=0.2;//允许的误差范围
//double error_u=0.954;//NUM=4,2103

int f_x[4]={0,1,0,-1};
int f_y[4]={1,0,-1,0};

std::string last_1="Move";
std::string last_2="Move";


bool dis(){
    double x1=Players[0].x;
    double x2=Players[1].x;
    double y1=Players[0].y;
    double y2=Players[1].y;
    return ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)<0.6);
}
bool action(std::string s){
    if(s=="Move U"||s=="Move D"||s=="Move L"||s=="Move R"||s=="Move RU"||s=="Move RD"||s=="Move LU"||s=="Move LD")
    return 1;
    return 0;
}

bool id_move(int id){
    return (fabs(Players[id].X_Velocity)<=0.001&&fabs(Players[id].Y_Velocity)<=0.001);
}

void map_copy(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
        bfs_map[i][j]=Map[i][j];
        lable[i][j]=0;
        father[i][j]=0;
        }
    }
    return;
}
//地图的遍历为y，x；player0优先使用地图，player1遍历的时候留下0的位置，给1让路
std::string bfs(int x,int y,int* dx,int* dy,int id){
    map_copy();
    bfs_map[y][x]=id+'0';
    std::queue<int>q_x;
    std::queue<int>q_y;
    q_x.push(x);
    q_y.push(y);
    int find=0;
    while(!q_x.empty()&&!find){
        int tmp_x=q_x.front();
        int tmp_y=q_y.front();
        q_x.pop();
        q_y.pop();
        lable[tmp_y][tmp_x]=1;
        for(int i=0;i<4;i++){
            if(tmp_x+f_x[i]==*dx&&tmp_y+f_y[i]==*dy){
                father[*dy][*dx]=i;
                find=1;
                break;
            }
            else{
                if(bfs_map[tmp_y+f_y[i]][tmp_x+f_x[i]]=='.'&&!lable[tmp_y+f_y[i]][tmp_x+f_x[i]]){
                    father[tmp_y+f_y[i]][tmp_x+f_x[i]]=i;
                    lable[tmp_y+f_y[i]][tmp_x+f_x[i]]=1;
                    q_x.push(tmp_x+f_x[i]);
                    q_y.push(tmp_y+f_y[i]);
                }
            }
        }
    }
    if(find){
        int tmp_x=*dx;
        int tmp_y=*dy;
        int f=father[*dy][*dx];
        while(tmp_x!=x||tmp_y!=y){
            f=father[tmp_y][tmp_x];
            //std::cerr<<tmp_y<<" "<<tmp_x<<"->";
            int f_1=f_x[father[tmp_y][tmp_x]];
            int f_2=f_y[father[tmp_y][tmp_x]];
            tmp_x-=f_1;
            tmp_y-=f_2;
        }
        //fix ther destination
        std::cerr<<"fix"<<x<<" "<<y<<" "<<x+f_x[f]<<" "<<y+f_y[f]<<std::endl;
        *dx=x+f_x[f];
        *dy=y+f_y[f];
    }
    else return "Move";
    return "Move";
}

bool in(double x,double y,double d_x,double d_y){
    if(d_x>x+error_d && d_x<x+error_u && d_y>y+error_d && d_y<y+error_u){
        return 1;
    }
    return 0;
}//检查是否抵达目的地

bool hard_in(double x,double y,double d_x,double d_y){
    if(d_x>x+error_d1 && d_x<x+error_u1 && d_y>y+error_d1 && d_y<y+error_u1){
        return 1;
    }
    return 0;
}//检查是否抵达目的地

std::string inte(double d_x, double d_y,  int op){
    if(d_x==0){
            if(op==1){
                return "PutOrPick L";
            }
            else if(op==2){
                return "Interact L";
            }
        }
    else if(d_x==9){
            if(op==1){
                return "PutOrPick R";
            }
            else if(op==2){
                return "Interact R";
            }
        }
    else if(d_y==0){
            if(op==1){
                return "PutOrPick U";
            }
            else if(op==2){
                return "Interact U";
            }
        }
    else if(d_y==9){
            if(op==1){
                return "PutOrPick D";
            }
            else if(op==2){
                return "Interact D";
            }
        } 
    return "Move";
}

std::string movement(double d_x,double d_y,int id){
    int x=(int)(Players[id].x-0.2);
    int y=(int)(Players[id].y-0.2);
    int des_xx=lround(d_x);
    int des_yy=lround(d_y);
    //std::cerr<<Players[id].x<<" "<<Players[id].y<<std::endl;
    if(des_xx!=x||des_yy!=y){bfs(x,y,&des_xx,&des_yy,id);
    d_x=des_xx;
    d_y=des_yy;
    /*
     for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
        std::cerr<<bfs_map[i][j];
        }
        std::cerr<<std::endl;
    }*/

    std::string ret = "Move ";
    if (Players[id].x < d_x + error_d)
        ret += "R";
    else if (Players[id].x > d_x+error_u)
        ret += "L";
    if (Players[id].y < d_y + error_d)
        ret += "D";
    else if (Players[id].y > d_y+error_u)
        ret += "U";
    if(ret=="Move ")ret="Move";
    //if(Players[id].y>6.7 && Players[id].y<9 && Players[id].x>6.5 && (ret=="Move LD"||ret=="Move D"))ret="Move L";
    //if(Players[id].x>6.7 && Players[id].x<9 && Players[id].y>6.7 && (ret=="Move RU"||ret=="Move R"))ret="Move U";
    if(id==0&&ret!=last_1&&!id_move(0)){
        //last_1=ret;
        ret="Move";
    }
    else if(id==0&&ret!=last_1&&id_move(0)){
        last_1=ret;
        //ret="Move";
    }
    if(id==1&&ret!=last_2){
        last_2=ret;
        ret="Move";
    }
    else if(id==1&&ret!=last_2&&id_move(1)){
        last_2=ret;
        //ret="Move";
    }
    return ret;
    }
    else{
        std::string ret = "Move ";
    if (Players[id].x < d_x + error_d1)
        ret += "R";
    else if (Players[id].x > d_x+error_u1)
        ret += "L";
    if (Players[id].y < d_y + error_d1)
        ret += "D";
    else if (Players[id].y > d_y+error_u1)
        ret += "U";
    if(ret=="Move ")ret="Move";
    return ret;
    }
}//移动策略


std::string hard_movement(double d_x,double d_y,int id){
    std::string ret = "Move ";
    if (Players[id].x < d_x + error_d1)
        ret += "R";
    if (Players[id].x > d_x+error_u1)
        ret += "L";
    if (Players[id].y < d_y + error_d1)
        ret += "D";
    if (Players[id].y > d_y+error_u1)
        ret += "U";
    if(ret=="Move ")ret="Move";
    if(Players[id].y>7 && Players[id].y<8 && Players[id].x>7.8 && ret=="Move LD")ret="Move L";
    if(Players[id].x>7 && Players[id].x<8 && Players[id].y>7.8 && ret=="Move RU")ret="Move U";
    return ret;
}//移动策略