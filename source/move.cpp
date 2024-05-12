#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <math.h>
#include <move.h>

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
//double error_d=0.06;//允许的误差范围
//double error_u=0.96;//NUM=1,2226 points //NUM=2,2747
//double error_d=0.05;//允许的误差范围
//double error_u=0.989;//NUM=3,2681 points
double error_d=0.2;//允许的误差范围
double error_u=0.8;//NUM=4,2103

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

bool in(double x,double y,double d_x,double d_y){
    if(d_x>x+error_d && d_x<x+error_u && d_y>y+error_d && d_y<y+error_u){
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
    std::string ret = "Move ";
    if (Players[id].x < d_x + error_d)
        ret += "R";
    if (Players[id].x > d_x+error_u)
        ret += "L";
    if (Players[id].y < d_y + error_d)
        ret += "D";
    if (Players[id].y > d_y+error_u)
        ret += "U";
    if(ret=="Move ")ret="Move";
    if(Players[id].y>7 && Players[id].y<8 && Players[id].x>7.8 && ret=="Move LD")ret="Move L";
    if(Players[id].x>7 && Players[id].x<8 && Players[id].y>7.8 && ret=="Move RU")ret="Move U";
    return ret;
}//移动策略