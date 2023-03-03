#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//***************
//||ACTOR CLASS||
//***************
Actor::Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir, int depth, double size):GraphObject(imageID, startX, startY, dir, depth, size){
    world = whereAmI;
}
StudentWorld* Actor::getWorld() const{
    return world;
}

//****************
//||PLAYER CLASS||
//****************
Player::Player(StudentWorld* whereAmI, int imageID, int startX, int startY)
:Actor(whereAmI,imageID,startX,startY,right,0,1), dieRoll(0), walkDir(right), state(WAITING), tTMove(0) ,nCoins(0), nStars(0), hasVortex(false), landed(false), here(false){}

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
            here = false;
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

//Getters
bool Player::hasLanded() const{
    return landed;
}

bool Player::isHere() const{
    return here;
}

int Player::getCoins() const{
    return nCoins;
}

int Player::getStars() const{
    return nStars;
}

bool Player::vortex() const{
    return hasVortex;
}

int Player::getRoll() const{
    return dieRoll;
}

//Setters
void Player::setHere(){
    here = true;
}

void Player::addCoins(int amt){
    nCoins += amt;
}

void Player::addStars(int amt){
    nStars+=amt;
}

void Player::setWalkDir(int dir){
    walkDir = dir;
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
    //Check if square is alive
    if(!isActive()){
        return;
    }
    //Check is Peach traverses a square
    if(getX()==getWorld()->getPeach()->getX() && getY()==getWorld()->getPeach()->getY()){
        traverseAction(getWorld()->getPeach());
        //Check if Peach lands on a square
        if(getWorld()->getPeach()->hasLanded()){
            //Checks if Peach is new
            if(!getWorld()->getPeach()->isHere()){
                landAction(getWorld()->getPeach());
                //Marks Peach as having activated square already
                getWorld()->getPeach()->setHere();
            }
        }
    }
    //Check is Yoshi traverses a square
    if(getX()==getWorld()->getYoshi()->getX() && getY()==getWorld()->getYoshi()->getY()){
        traverseAction(getWorld()->getYoshi());
        if(getWorld()->getYoshi()->hasLanded()){
            //Checks if Yoshi is new
            if(!getWorld()->getYoshi()->isHere()){
                landAction(getWorld()->getYoshi());
                //Marks Yoshi as having activated the square already
                getWorld()->getYoshi()->setHere();
            }
        }
    }
}

bool Square::isActive() const{
    return aliveStatus;
}

//**************
//||COIN CLASS||
//**************
Coin::Coin(StudentWorld* whereAmI, int imageID, int startX, int startY, int amt):Square(whereAmI,imageID, startX, startY), coinModifier(amt){}

void Coin::landAction(Player* plyr){
    //If blue coin square, add coins to player
    if(coinModifier>0){
        plyr->addCoins(coinModifier);
        //Play Blue Square Sound
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    else{
        //If red coin square, make sure coins don't go below 0
        int playerCoins = plyr->getCoins();
        playerCoins < 3 ? plyr->addCoins(-playerCoins) : plyr->addCoins(coinModifier);
        //Play Red Square Sound
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
}

void Coin::traverseAction(Player* plyr){return;}

//**************
//||STAR CLASS||
//**************
Star::Star(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Star::landAction(Player* plyr){
    //If player cannot purchase star immediately return
    if(plyr->getCoins()<20) return;
    //Otherwise deduct 20 coins from player and give player a star
    plyr->addCoins(-20);
    plyr->addStars(1);
    //Play sound for given star
    getWorld()->playSound(SOUND_GIVE_STAR);
}

void Star::traverseAction(Player* plyr){landAction(plyr);}


//*********************
//||DIRECTIONAL CLASS||
//*********************
Directional::Directional(StudentWorld* whereAmI, int imageID, int startX, int startY, int fDir):Square(whereAmI,imageID, startX, startY), forcingDirection(fDir){
    setDirection(fDir);
}

void Directional::landAction(Player* plyr){
    //Change walk direction
    plyr->setWalkDir(forcingDirection);
    //Adjust sprite direction accordingly
    (forcingDirection == left) ? plyr->setDirection(left) : plyr->setDirection(right); //Set sprite direction to 180 deg if walking left otherwise set sprite direction to 0 deg
}

void Directional::traverseAction(Player* plyr){landAction(plyr);}


//**************
//||BANK CLASS||
//**************
Bank::Bank(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY), balance(0){}

void Bank::landAction(Player* plyr){}

void Bank::traverseAction(Player* plyr){return;}


//***************
//||EVENT CLASS||
//***************
Event::Event(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Event::landAction(Player* plyr){}

void Event::traverseAction(Player* plyr){return;}


//******************
//||DROPPING CLASS||
//******************
Dropping::Dropping(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Dropping::landAction(Player* plyr){}

void Dropping::traverseAction(Player* plyr){return;}

