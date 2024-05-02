#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <math.h>
#include <move.h>
using namespace std;

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
double error_d=0.1;//允许的误差范围
double error_u=0.9;

double dis(double x1,double y1,double x2,double y2){
    return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
}
bool action(int id){
    return 1;
}

bool in(double x,double y,double d_x,double d_y){
    if(d_x>=x+error_d && d_x<=x+error_u && d_y>=y+error_d && d_y<=y+error_u){
        return 1;
    }
    return 0;
}//检查是否抵达目的地

string inte(double d_x, double d_y, int id, int op){
    if(8.99<d_y && d_y<9.01 &&(d_y - Players[id].y <= center + 0.3) && (Players[id].x >= d_x && (Players[id].x - d_x) <= center + radius)){
        if(op==1){
            return "PutOrPick D";
        }
        else if(op==2){
            return "Interact D";
        }
    }
    else if(d_y == 0 && (Players[id].y - d_y <=  + center + 1.3) && (Players[id].x >= d_x && (Players[id].x - d_x) <= center + radius)){
         if(op==1){
            return "PutOrPick U";
        }
        else if(op==2){
            return "Interact U";
        }
    }
     else if(d_x == 0 && (Players[id].x - d_x <= center + 1.3) && (Players[id].y >= d_y && (Players[id].y - d_y) <= center + radius)){
         if(op==1){
            return "PutOrPick L";
        }
        else if(op==2){
            return "Interact L";
        }
    }
}


string movement(double d_x,double d_y,int id){
    /*
    if(Players[id].X_Velocity==0 &&Players[id].Y_Velocity==0 && Players[id].x - center <= d_x + 0.3
    && Players[id].x >= d_x && Players[id].y - center <= d_y + 0.3 && Players[id].y >= d_y){
        //return inte(d_x,d_y,id,op);
    }


     if(8.99<d_y && d_y<9.01 &&(d_y - Players[id].y <= center + 0.3) && (Players[id].x >= d_x && (Players[id].x - d_x) <= center + radius)){
        if(op==1){
            return "PutOrPick D";
        }
        else if(op==2){
            return "Interact D";
        }
    }
    else if(d_y == 0 && (Players[id].y - d_y <=  + center + 1.3) && (Players[id].x >= d_x && (Players[id].x - d_x) <= center + radius)){
         if(op==1){
            return "PutOrPick U";
        }
        else if(op==2){
            return "Interact U";
        }
    }
     else if(d_x == 0 && (Players[id].x - d_x <= center + 1.3) && (Players[id].y >= d_y && (Players[id].y - d_y) <= center + radius)){
         if(op==1){
            return "PutOrPick L";
        }
        else if(op==2){
            return "Interact L";
        }
    }*/

    string ret = "Move ";
    if (Players[id].x < d_x + error_d)
        ret += "R";
    if (Players[id].x > d_x+error_u)
        ret += "L";
    if (Players[id].y < d_y + error_d)
        ret += "D";
    if (Players[id].y > d_y+error_u)
        ret += "U";
    if(ret=="Move ")ret="Move";
    return ret;
}//移动策略