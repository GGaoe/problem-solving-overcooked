#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <move.h>

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
extern int status;//0：取原料 1：找盘子 2：拿盘子和食材 3：去上菜

double windows_x;//交菜窗口
double windows_y;
double sink_x;//洗碗池
double sink_y;
double plate_x=-1;//盘子
double plate_y=-1;
double pot_x;//锅
double pot_y;
double pan_x;//煎锅
double pan_y;
double cliff_x;//悬崖
double cliff_y;
double cut_x;//切菜
double cut_y;
std::string current_ingredient="";
std::string derection="";
std::string derection1="";
double des_x=0;
double des_y=0;
double o0_x=0;
double o1_x=0;
double o0_y=0;
double o1_y=0;
double des1_x=0;
double des1_y=0;
bool washing=0;

void init(){
    status=0;
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
    //行为策略
    //player0负责做菜及上菜
    //检查当前食谱上的原料
    //fix原料位置
    //取原料
    //拿盘子
    //std::cerr<<"step0 into"<<std::endl;
    if(finished){
        finished=0;
        current_ingredient=Order[0].recipe[count1];
        //std::cerr<<Order[0].recipe[count1]<<std::endl;
    }
     //std::cerr<<"step0 into"<<std::endl;
    if(status==0){//取食材
        //std::cerr<<"step1 into"<<std::endl;
        for (int i = 0; i < IngredientCount; i++)
            {
                //std::cerr<<"step2 into"<<std::endl;
                if (Ingredient[i].name == current_ingredient)
                {   
                    //std::cerr<<"step3 into"<<std::endl;
                    o0_x=Ingredient[i].x;
                    o0_y=Ingredient[i].y;
                    fix(&des_x,&des_y,Ingredient[i].x,Ingredient[i].y);
                    break;
                }
            }
        if(!in(des_x,des_y,Players[0].x,Players[0].y)){
            player0_Action=movement(des_x,des_y,0);//移动操作
        }
        else{
            if(o0_x==0){
                player0_Action="PutOrPick L";
            }
            else if(o0_x==9){
                player0_Action="PutOrPick R";
            }
            else if(o0_y==0){
                player0_Action="PutOrPick U";
            }
            else if(o0_y==9){
                player0_Action="PutOrPick D";
            }
            else{
                std::cerr<<"No match"<<std::endl;
            }
            status=1;//去取盘子
        }//取食材操作
    }
    else if(status==1){//取盘子
        bool find_plate=0;
        if(plate_x==-1)
        for(int i=0;i<entityCount;i++){
            if(Entity[i].containerKind==ContainerKind::Plate){
                plate_x=Entity[i].x;
                plate_y=Entity[i].y;
                find_plate=1;
                break;
            }
        }
        else find_plate=1;
        fix(&des_x,&des_y,plate_x,plate_y);
        if(find_plate){
            if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                player0_Action=movement(des_x,des_y,0);//移动操作
            }
            else {
                if(plate_x==0){
                    player0_Action="PutOrPick L";
                    derection="L";
                }
                else if(plate_x==9){
                    player0_Action="PutOrPick R";
                    derection="R";
                }
                else if(plate_y==0){
                    player0_Action="PutOrPick U";
                    derection="U";
                }
                else if(plate_y==9){
                    player0_Action="PutOrPick D";
                    derection="D";
                }
                else{
                    std::cerr<<"No match"<<std::endl;
                }
                count1++;
                if(count1==Order[0].recipe.size())status=2;//拿盘子和食材 
            }
        }
    }
    else if(status==2){//拿起做好的食材
        player0_Action="PutOrPick "+derection;
        plate_x=-1;
        plate_y=-1;
        status=3;
    }
    else if(status==3){//送菜
        fix(&des_x,&des_y,windows_x,windows_y);
        if(!in(des_x,des_y,Players[0].x,Players[0].y)){
                player0_Action=movement(des_x,des_y,0);//移动操作
            }
        else {
            if(windows_x==0){
                player0_Action="PutOrPick L";
            }
            else if(windows_x==9){
                player0_Action="PutOrPick R";
            }
            else if(windows_y==0){
                player0_Action="PutOrPick U";
            }
            else if(windows_y==9){
                player0_Action="PutOrPick D";
            }
            finished=1;
            status=0; 
            count1=0;
        }
    }
    //player1负责拿脏盘子以及洗盘子
    if(Players[1].containerKind==ContainerKind::DirtyPlates){//player1拿着脏盘子去洗碗池
        fix(&des1_x,&des1_y,sink_x,sink_y);
        if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
            player1_Action=movement(des1_x,des1_y,1);//移动操作
        }
        else{
            if(sink_x==0){
                player1_Action="PutOrPick L";
                derection1="L";
            }
            else if(sink_x==9){
                player1_Action="PutOrPick R";
                derection1="R";
            }
            else if(sink_y==0){
                player1_Action="PutOrPick U";
                derection1="U";
            }
            else if(sink_y==9){
                player1_Action="PutOrPick D";
                derection1="D";
            }
            washing=1;
        }
    }
    else{
        if(washing){
            player1_Action="Interact "+derection1;
            int check=0;
            for(int i=0;i<entityCount;i++){
                if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                    {   
                        if(Entity[i].x==sink_x&&Entity[i].y==sink_y)
                            {
                                check = 1;
                                break;
                            }
                    }
            }
            if(!check)washing=0;
        }
        else{
            int check=0;
            for(int i=0;i<entityCount;i++){
                if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                    {   
                        o1_x=Entity[i].x;
                        o1_y=Entity[i].y;
                        fix(&des1_x,&des1_y,Entity[i].x,Entity[i].y);
                        check=1;
                        break;
                }
            }
            if(!check){
            
            }
            else{
                if(!in(des1_x,des1_y,Players[1].x,Players[1].y)){
                    player1_Action=movement(des1_x,des1_y,1);//移动操作
                }
                else{
                    if(o1_x==0){
                        player1_Action="PutOrPick L";
                    }
                    else if(o1_x==9){
                        player1_Action="PutOrPick R";
                    }
                    else if(o1_y==0){
                        player1_Action="PutOrPick U";
                    }
                    else if(o1_y==9){
                        player1_Action="PutOrPick D";
                    }
                }
            }
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
