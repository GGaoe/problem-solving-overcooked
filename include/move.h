#ifndef MOVE
#define MOVE
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <queue>
bool dis();
bool action(std::string s);
bool in(double x,double y,double d_x,double d_y);
bool hard_in(double x,double y,double d_x,double d_y);
std::string inte(double d_x,double d_y,int op);
std::string movement(double d_x,double d_y,int id);
std::string hard_movement(double d_x,double d_y,int id);
#endif