#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor Base Class
Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size):GraphObject(imageID, startX, startY, dir, depth, size){}

//Peach Class
Peach::Peach(int startX, int startY):Actor(IID_PEACH,startX,startY,right,0,1){}
void Peach::doSomething(){};

//Coin Class
Coin::Coin(int startX, int startY):Actor(IID_BLUE_COIN_SQUARE,startX,startY,right,1,1){}
void Coin::doSomething(){}
bool Coin::isActive(){
    return false;
}
