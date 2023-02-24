#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor Base Class
Actor::Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir, int depth, double size):GraphObject(imageID, startX, startY, dir, depth, size){
    world = whereAmI;
}
StudentWorld* Actor::getWorld(){
    return world;
}

//Peach Class
Peach::Peach(StudentWorld* whereAmI, int startX, int startY)
:Actor(whereAmI,IID_PEACH,startX,startY,right,0,1), walkDirection(right), pNum(1), state(WAITING), tTMove(0){}
void Peach::doSomething(){
    //If Player to Move
    if(state==WAITING){
        //Check if avatar is facing valid direction and fix if not
        //Check if user pressed key
        if(getWorld()->getAction(PEACHID)){
            
        }
    }
};

//Coin Class
Coin::Coin(StudentWorld* whereAmI, int startX, int startY):Actor(whereAmI, IID_BLUE_COIN_SQUARE,startX,startY,right,1,1){}
void Coin::doSomething(){}
bool Coin::isActive(){
    return false;
}
