#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//***************
//||ACTOR CLASS||
//***************
Actor::Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir, int depth, double size):GraphObject(imageID, startX, startY, dir, depth, size){
    world = whereAmI;
}
StudentWorld* Actor::getWorld(){
    return world;
}

//****************
//||PLAYER CLASS||
//****************
Player::Player(StudentWorld* whereAmI, int imageID, int startX, int startY)
:Actor(whereAmI,imageID,startX,startY,right,0,1), dieRoll(0), walkDir(right), state(WAITING), tTMove(0) ,nCoins(0), nStars(0), hasVortex(false), landed(false){}

void Player::doSomething(){
    //If Player to Move
    if(state==WAITING){
        //Check if avatar is facing valid direction and fix if not
        //If user decides to roll die
        if(getWorld()->getAction(P1)==ACTION_ROLL){
            dieRoll = randInt(1,10); //Generate random die roll
            tTMove = dieRoll*8; //Change ticks to move
            state = WALKING; //Player is now walking
            landed = false; //Player move away from square
        }
        else return; //User doesn't press key, or presses another key
    }
    //If user is walking
    if(state==WALKING){
        //Check if next tile is empty, and if so, adjust walk direction appropriately
        if(getX()%16==0 && getY()%16==0 && !canMove(walkDir)){
            if(walkDir == right || walkDir == left) canMove(up) ? walkDir = up : walkDir = down;
            else if(walkDir == up || walkDir == down) canMove(right) ? walkDir = right : walkDir = left;
        }//if
        (walkDir == left) ? setDirection(left) : setDirection(right); //Set sprite direction to 180 deg if walking left otherwise set sprite direction to 0 deg
    }//if
    moveAtAngle(walkDir, 2); //Move 2 pixels in walk direction
    tTMove--; //Decrement ticks to move count by 1
    if(tTMove == 0){
        state = WAITING; //If ticks to move equals 0 then change state to waiting to roll
        landed = true; //Player landed on square
    }
};

bool Player::canMove(int direction){
    int newX, newY;
    getPositionInThisDirection(direction, 16, newX, newY);
    newX /=16;
    newY /=16;
    return !getWorld()->isEmpty(newX,newY);
}

bool Player::hasLanded(){
    return landed;
}

int Player::getCoins(){
    return nCoins;
}

int Player::getStars(){
    return nStars;
}

bool Player::vortex(){
    return hasVortex;
}

int Player::getRoll(){
    return dieRoll;
}


//****************
//||BADDIE CLASS||
//****************
Baddie::Baddie(StudentWorld* whereAmI, int imageID, int startX, int startY):Actor(whereAmI,imageID,startX,startY,right,0,1), walkDir(right), state(PAUSED), travelDist(0), pauseCounter(0){}

void Baddie::doSomething(){}


//****************
//||BOWSER CLASS||
//****************
Bowser::Bowser(StudentWorld* whereAmI, int imageID, int startX, int startY): Baddie(whereAmI, imageID, startX, startY){}

void Bowser::doSomething(){}

//*************
//||BOO CLASS||
//*************
Boo::Boo(StudentWorld* whereAmI, int imageID, int startX, int startY): Baddie(whereAmI, imageID, startX, startY){}

void Boo::doSomething(){}

//****************
//||Square CLASS||
//****************
Square::Square(StudentWorld* whereAmI, int imageID, int startX, int startY):Actor(whereAmI, imageID,startX,startY,right,1,1), aliveStatus(true){}

void Square::doSomething(){
    if(!isActive()){
        return;
    }
}

bool Square::isActive(){
    return aliveStatus;
}

//**************
//||COIN CLASS||
//**************
Coin::Coin(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY), nCoins(0){}

void Coin::doSomething(){}

int Coin::getCoins(){return nCoins;}

void Coin::setCoins(int amt){nCoins=amt;}

//*********
//BLUE COIN
//*********
Blue::Blue(StudentWorld* whereAmI, int imageID, int startX, int startY):Coin(whereAmI,imageID, startX, startY){
    setCoins(3);
}

void Blue::doSomething(){}

//********
//RED COIN
//********
Red::Red(StudentWorld* whereAmI, int imageID, int startX, int startY):Coin(whereAmI,imageID, startX, startY){
    setCoins(-3);
}

void Red::doSomething(){}

//**************
//||STAR CLASS||
//**************
Star::Star(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Star::doSomething(){}

//*********************
//||DIRECTIONAL CLASS||
//*********************
Directional::Directional(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Directional::doSomething(){}

//**************
//||BANK CLASS||
//**************
Bank::Bank(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY), balance(0){}

void Bank::doSomething(){}

//***************
//||EVENT CLASS||
//***************
Event::Event(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Event::doSomething(){}

//******************
//||DROPPING CLASS||
//******************
Dropping::Dropping(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Dropping::doSomething(){}
