#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <move.h>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <stack>

extern Player Players[2+5];//Player0负责做菜上菜，Player1负责拿脏盘子、洗盘子
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
extern int status;//0：取原料 1：找盘子 2：拿盘子和食材 3：去上菜 4：找工作台放菜 5：做菜 6：拿菜 7：还锅

double windows_x;//交菜窗口
double windows_y;
double sink_x;//洗碗池
double sink_y;
double plate_x=-1;//盘子
double plate_y=-1;
double pot_x;//蒸锅
double pot_y;
double pan_x;//煎锅
double pan_y;
double cliff_x;//悬崖
double cliff_y;
double cut_x;//切菜
double cut_y;
std::string current_ingredient="";
std::stack<std::string>Op;
std::stack<std::string>O_ingredient;
int pointer_o=0;
double des_x=0;
double des_y=0;
double des1_x=0;
double des1_y=0;
double o0_x=0;
double o1_x=0;
double o0_y=0;
double o1_y=0;
bool washing=0;
double clean_plate_x=0;
double clean_plate_y=0;
double dirty_plate_x=0;
double dirty_plate_y=0;

std::string random_walk(){
    srand((unsigned)time(0));
    int ran_num=rand()%4;
    if(ran_num==0)return "Move U";
    else if(ran_num==1)return "Move D";
    else if (ran_num==2)return "Move L";
    else return "Move R";
    }

bool exist_plate(double *x_1,double *y_1,ContainerKind cont){
    for(int i=0;i<entityCount;i++){
                if(Entity[i].containerKind==cont){
                    *x_1=Entity[i].x;
                    *y_1=Entity[i].y;
                    return 1;
                }
    }
    return 0;
}

bool exist_Entity(double *x_1,double *y_1,std::string s){
    for(int i=0;i<entityCount;i++){
                if(Entity[i].entity.back()==s){
                    *x_1=Entity[i].x;
                    *y_1=Entity[i].y;
                    return 1;
                }
    }
    return 0;
}

bool exist_Ingredient(double *x_1,double *y_1,std::string s){
   for (int i = 0; i < IngredientCount; i++)
            {
                if (Ingredient[i].name == s)
                {   
                    *x_1=Ingredient[i].x;
                    *y_1=Ingredient[i].y;
                    return 1;
                }
            }
    return 0;
}

void init(){
    status=0;
    exist_Entity(&pot_x,&pot_y,"Pot");
    exist_Entity(&pan_x,&pan_y,"Pan");
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(Map[i][j]=='$'){
                windows_x=j;
                windows_y=i;
            }
            else if(Map[i][j]=='k'){
                sink_x=j;
                sink_y=i;
            }
            else if(Map[i][j]=='_'){
                cliff_x=j;
                cliff_y=i;
            }
            else if(Map[i][j]=='c'){
                cut_x=j;
                cut_y=i;
            }
            else if(Map[i][j]=='r'){
                clean_plate_x=j;
                clean_plate_y=i;
            }
            else if(Map[i][j]=='p'){
                dirty_plate_x=j;
                dirty_plate_y=i;
            }
        }
    }
}//初始化

void fix(double *x,double *y,double x1,double y1){
    *x=x1,*y=y1;
    if(x1==0)*x+=1;
    if(x1==9)*x-=1;
    if(y1==0)*y+=1;
    if(y1==9)*y-=1;
    return;
}//修正目标地址

int main()
{
    std::ios::sync_with_stdio(false);
    std::cerr.tie(nullptr);
    std::cerr << std::nounitbuf;
    std::string s;
    std::stringstream ss;
    int frame;
    init_read();

    /*
        你可以在读入后进行一些相关预处理，时间限制：5秒钟
        init();
    */

   init();
    
    bool finished=1;
    int count1=0;
    int totalFrame = 14400;
    for (int i = 0; i < totalFrame; i++)
    {
        bool skip = frame_read(i);
        if (skip) continue;
        std::string player0_Action="Move";
        std::string player1_Action = "Move";
        std::cerr<<"status:"<<status<<std::endl;
        //std::cerr<<"Pot:"<<pot_x<<" "<<pot_y<<std::endl;


    //行为策略
    //player0负责做菜及上菜
    //检查当前食谱上的原料
    //fix原料位置
    //取原料
    //拿盘子
    if(finished){
        finished=0;
        count1=0;
        current_ingredient=Order[0].recipe[0];
        while(!exist_Ingredient(&o0_x,&o0_y,current_ingredient)){
            for (int i = 0; i < recipeCount; i++)
                {
                    if(current_ingredient==Recipe[i].nameAfter){
                        current_ingredient=Recipe[i].nameBefore;
                        Op.push(Recipe[i].kind);
                        O_ingredient.push(Recipe[i].nameAfter);
                    }
                }
        }
        status=0;
        //找到了处理方法和每次处理后的食材，用数组模拟栈，转移到status=1，取食材，栈非空转移到4 对食材进行处理
    }
    if(status==0){//取食材
        exist_Ingredient(&o0_x,&o0_y,current_ingredient);
        fix(&des_x,&des_y,o0_x,o0_y);
        if(!in(des_x,des_y,Players[0].x,Players[0].y)){
            player0_Action=movement(des_x,des_y,0);//移动操作
        }
        else{
            player0_Action=inte(o0_x,o0_y,1);
            if(Op.empty())status=1;//去取盘子
            else{
                status=4;//先拿食材，再去操作台
            }
        }//取食材操作
    }
    else if(status==4){//找工作台放菜
        if(Op.top()=="-chop->"){
            o0_x=cut_x;
            o0_y=cut_y;
            fix(&des_x,&des_y,cut_x,cut_y);
            if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                    player0_Action=movement(des_x,des_y,0);
            }
            else{
                player0_Action=inte(cut_x,cut_y,1);//放菜
                status=5;
            }            
        }
        else if(Op.top()=="-pot->"){
            o0_x=pot_x;
            o0_y=pot_y;
            fix(&des_x,&des_y,pot_x,pot_y);
            if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                    player0_Action=movement(des_x,des_y,0);
            }
            else{
                player0_Action=inte(pot_x,pot_y,1);//放菜
                status=5;
            }
        }
        else if(Op.top()=="-pan->"){
            o0_x=pan_x;
            o0_y=pan_y;
            fix(&des_x,&des_y,pan_x,pan_y);
            if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                    player0_Action=movement(des_x,des_y,0);
            }
            else{
                player0_Action=inte(pan_x,pan_y,1);//放菜
                status=5;
            }
        }
    }
    else if(status==5){
        //player0_Action="Move";
        for (int i = 0; i < entityCount; i++)
            {
                //std::cerr<<"o0_x:"<<o0_x<<" "<<o0_y<<std::endl;
                if(o0_x==Entity[i].x && o0_y==Entity[i].y){
                    //std::cerr<<"current"<<Entity[i].currentFrame<<" "<<Entity[i].totalFrame<<std::endl;
                    if((Entity[i].entity.front()==O_ingredient.top()&&Entity[i].totalFrame==0)||Entity[i].currentFrame>Entity[i].totalFrame){
                    //if(Entity[i].entity.front()==O_ingredient.top()){
                        player0_Action=inte(Entity[i].x,Entity[i].y,1);//拿做好的菜
                        current_ingredient=O_ingredient.top();
                        O_ingredient.pop();
                        Op.pop();
                        if(Op.empty()){
                            status=1;
                            //拿盘子
                        }
                        else{
                            status=4;
                        }
                    }
                    else{
                        player0_Action=inte(Entity[i].x,Entity[i].y,2);//做菜
                        //player0_Action="Move";
                    }
                    
                }
            }
    }//做菜
    else if(status==6){
        if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                player0_Action=movement(des_x,des_y,0);//移动操作
            }
        else {
            fix(&des_x,&des_y,plate_x,plate_y);
            player0_Action=inte(o0_x,o0_y,1);
            status=2;
        }
    }//还锅
    else if(status==1){//取盘子
        bool find_plate=0;
        if(plate_x==-1){
            if(!exist_plate(&plate_x,&plate_y,ContainerKind::Plate)){
                fix(&des_x,&des_y,clean_plate_x,clean_plate_y);
                if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                    //player0_Action=movement(des_x,des_y,0);//提前移动到干净盘子的地方//优化后比没优化还低？
                }
            }
            else find_plate=1;
        }
        else find_plate=1;
        fix(&des_x,&des_y,plate_x,plate_y);
        if(find_plate){
            if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                player0_Action=movement(des_x,des_y,0);//移动操作
            }
            else {
                player0_Action=inte(plate_x,plate_y,1);
                count1++;
                
                //放下做好的东西到盘子里，如果手里拿着锅，去还锅
                if(Players[0].containerKind==ContainerKind::Pan){
                    o0_x=pan_x;
                    o0_y=pan_y;
                    fix(&des_x,&des_y,pan_x,pan_y);
                    status=6;
                }
                else if(Players[0].containerKind==ContainerKind::Pot){
                    o0_x=pot_x;
                    o0_y=pot_y;
                    fix(&des_x,&des_y,pot_x,pot_y);
                    status=6;
                }
                else{//手里空空如也
                    if(count1==Order[0].recipe.size())status=2;//已经完成菜谱，拿起做好的食材 
                    else{
                        player0_Action="Move";
                        //还有别的菜，继续找原材料。。。。。。待补充
                    }
                }
            }
        }
    }
    else if(status==2){//拿起做好的食材和盘子
    if(!in(des_x,des_y,Players[0].x,Players[0].y)){
        player0_Action=movement(des_x,des_y,0);//移动操作
    }
    else{
        player0_Action=inte(plate_x,plate_y,1);
        plate_x=-1;
        plate_y=-1;
        status=3;
        }
    }
    else if(status==3){//送菜
        fix(&des_x,&des_y,windows_x,windows_y);
        if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                player0_Action=movement(des_x,des_y,0);//移动操作
            }
        else {
            player0_Action=inte(windows_x,windows_y,1);
            finished=1;
            status=0; 
        }
    }



    //player1负责拿脏盘子以及洗盘子
    if(Players[1].containerKind==ContainerKind::DirtyPlates){//player1拿着脏盘子去洗碗池
        fix(&des1_x,&des1_y,sink_x,sink_y);
        if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
            player1_Action=movement(des1_x,des1_y,1);//移动操作
        }
        else{
            player1_Action=inte(sink_x,sink_y,1);
            washing=1;
        }
    }
    else{
        if(washing){
            player1_Action=inte(sink_x,sink_y,2);
            if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
                    //player1_Action=movement(des1_x,des1_y,1);//移动操作
                }//防止被撞偏移
            int check=0;
            for(int i=0;i<entityCount;i++){
                if (Entity[i].containerKind==ContainerKind::DirtyPlates)
                    {   
                        if(Entity[i].x==sink_x&&Entity[i].y==sink_y)//在洗碗池里，没洗完
                            {
                                check = 1;
                                break;
                            }
                    }
            }
            if(!check)washing=0;
        }
        else{
            int check=exist_plate(&o1_x,&o1_y,ContainerKind::DirtyPlates);
            fix(&des1_x,&des1_y,o1_x,o1_y);
            if(!check){
                fix(&des1_x,&des1_y,dirty_plate_x,dirty_plate_y);
                if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
                    //player1_Action=movement(des1_x,des1_y,1);//移动操作
                }
            }
            else{
                if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
                    player1_Action=movement(des1_x,des1_y,1);//移动操作
                }
                else{
                    player1_Action=inte(o1_x,o1_y,1);
                }
            }
        }
    }

    if(dis()){
        if(action(player0_Action)){
            //player0_Action=random_walk();
            player0_Action="Move RD";
            //按照地图选取策略，防止卡死可以小概率随机，大概率选取固定策略
            //卡死时启用随机游走，否则按照默认运行
        }
    }
        /* 输出当前帧的操作，此处仅作示例 */
        std::cout << "Frame " << i << "\n";

        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
