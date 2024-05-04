#ifndef MOVE
#define MOVE
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <framework.h>
#include <math.h>
bool dis();
//bool action(string s);
bool action(std::string s);
bool in(double x,double y,double d_x,double d_y);
std::string inte(double d_x,double d_y,int op);
std::string movement(double d_x,double d_y,int id);
#endif