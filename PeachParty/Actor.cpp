#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//***************
//||ACTOR CLASS||
//***************
Actor::Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir, int depth, double size):GraphObject(imageID, startX, startY, dir, depth, size), aliveStatus(true){
    world = whereAmI;
}
StudentWorld* Actor::getWorld() const{
    return world;
}

bool Actor::isLiving(){
    return false;
}

bool Actor::isAlive() const{
    return aliveStatus;
}

void Actor::dead(){
    aliveStatus = false;
}

bool Actor::isImpactable() const{
    return false;
}

//Empty because by defualt actors are no impactable
void Actor::hit(){}

//***********
//MOVES CLASS
//***********
Moves::Moves(StudentWorld* whereAmI, int imageID, int startX, int startY):
    Actor(whereAmI,imageID,startX,startY,right,0,1), walkDir(right), state(PAUSED_OR_WAITING), tTMove(0){}

bool Moves::isLiving(){
    return true;
}

int Moves::moveInRandDir(){
    int newDir;
    do{
        newDir = randInt(0,3)*90;
    }while(!canMove(newDir));
    return newDir;
}

void Moves::teleport(){
    int x, y;
    do{
        x = randInt(0,15);
        y = randInt(0,15);
    }while(getWorld()->isEmpty(x,y));
    moveTo(x*16,y*16);
}

bool Moves::canMove(int direction){
    int newX, newY;
    getPositionInThisDirection(direction, 16, newX, newY);
    newX /=16;
    newY /=16;
    return !getWorld()->isEmpty(newX,newY);
}

void Moves::corner(){
    if(getWalkDir() == right || getWalkDir() == left) canMove(up) ? setWalkDir(up) : setWalkDir(down);
    else if(getWalkDir() == up || getWalkDir() == down) canMove(right) ? setWalkDir(right) : setWalkDir(left);
}

bool Moves::fork(){
    int nDirs = 0; //Number of valid directions
    if(canMove(up)) nDirs++;
    if(canMove(right)) nDirs++;
    if(canMove(down)) nDirs++;
    if(canMove(left)) nDirs++;
    if(nDirs>=3) return true;
    return false;
}

void Moves::move(){
    moveAtAngle(walkDir, 2); //Move 2 pixels in walk direction
    setTicks(getTicks()-1); //Decrement ticks to move count by 1
    if(getTicks() == 0){
        moveFunc();
    }
}

int Moves::getWalkDir() const{
    return walkDir;
}

int Moves::getState() const{
    return state;
}

int Moves::getTicks() const{
    return tTMove;
}

void Moves::setState(int newState){
    state = newState;
}

void Moves::setWalkDir(int newDir){
    walkDir = newDir;
    //Adjust sprite direction accordingly
    (newDir == left) ? setDirection(left) : setDirection(right); //Set sprite direction to 180 deg if walking left otherwise set sprite direction to 0 deg
}
void Moves::setTicks(int amt){
    tTMove = amt;
}

//****************
//||PLAYER CLASS||
//****************
Player::Player(StudentWorld* whereAmI, int imageID, int startX, int startY, int pNum)
:Moves(whereAmI, imageID, startX, startY), dieRoll(0), nCoins(0), nStars(0), vortex(false), landed(false), here(false), onDirSq(false), teleported(false), playerNum(pNum){}

void Player::doSomething(){
    //If Player to Move
    if(getState()==WAITING){
        //Check if avatar is facing invalid direction (due to being teleported) and turn to random valid direction if so
        if(teleported && !canMove(getWalkDir())){
            int newDir = moveInRandDir();
            setWalkDir(newDir);
            setTP(false);
        }
        int playerAction = getWorld()->getAction(playerNum);
        //If user decides to roll die
        if(playerAction==ACTION_ROLL){
            dieRoll = randInt(1,10); //Generate random die roll
            setTicks(dieRoll*8); //Change ticks to move
            setState(WALKING); //Player is now walking
            landed = false; //Player move away from square
            here = false; //Resets flag for square interaction
        }
        //If user decides to shoot vortex
        else if(hasVortex() && playerAction==ACTION_FIRE){
            //Creates vortex and plays sound
            getWorld()->createVortex(getX(), getY(), getWalkDir());
            //Updates player to no longer have vortex
            toggleVortex();
        }
        else return; //User doesn't press key, or presses another key
    }
    //If user is walking
    if(getState()==WALKING){
        //Only check if on a square
        if(getX()%16==0 && getY()%16==0){
            //Check for directional square and if true change direction done in directional square itself
            if(onDirSq){
                setOnDirSQ(false);
            }
            //If play is at a fork
            else if(fork()){
                //Check if user
                int newDir = getWorld()->getAction(playerNum);
                switch(newDir){
                    case(ACTION_UP):
                    {
                        newDir = up;
                        break;
                    }
                    case(ACTION_RIGHT):
                    {
                        newDir = right;
                        break;
                    }
                    case(ACTION_DOWN):
                    {
                        newDir = down;
                        break;
                    }
                    case(ACTION_LEFT):
                    {
                        newDir = left;
                        break;
                    }
                    //If user doesn't select direction
                    default:
                    {
                        newDir = -1;
                        break;
                    }
                }//Switch
                //If new valid direction selected, update walk dir and sprite dir
                if(newDir!=-1 && canMove(newDir)){
                    setWalkDir(newDir);
                }
                else return;
            }//Else if at fork
            //Check if next tile is empty, and if so, adjust walk direction appropriately
            else if(!canMove(getWalkDir())){
                corner();
            }
        }
        move();
    }//if
}

void Player::moveFunc(){
    setState(WAITING); //If ticks to move equals 0 then change state to waiting to roll
    landed = true; //Player landed on square
}

//Getters
int Player::isWaiting() const{
    return getState()==WAITING;
}

bool Player::hasLanded() const{
    return landed;
}

bool Player::isHere() const{
    return here;
}

int Player::getStars() const{
    return nStars;
}

int Player::getCoins() const{
    return nCoins;
}

bool Player::hasVortex() const{
    return vortex;
}

int Player::getRoll() const{
    return dieRoll;
}

//Setters
void Player::setHere(bool tf){
    here = tf;
}

void Player::addStars(int amt){
    nStars+=amt;
}

void Player::addCoins(int amt){
    nCoins += amt;
}

void Player::toggleVortex(){
    vortex = !vortex;
}

void Player::setRoll(int n){
    dieRoll = n;
}

void Player::setOnDirSQ(bool tf){
    onDirSq = tf;
}

void Player::setTP(bool tf){
    teleported = tf;
}

//****************
//||BADDIE CLASS||
//****************
Baddie::Baddie(StudentWorld* whereAmI, int imageID, int startX, int startY):Moves(whereAmI, imageID, startX, startY), travelDist(0), pauseCounter(180), sTMove(0), peach(false), yoshi(false){}

void Baddie::doSomething(){
    //If paused
    if(getState()==PAUSED){
        //If baddie on same square as peach, and peach is in a waiting state
        if(getX()==getWorld()->getPeach()->getX() && getY()==getWorld()->getPeach()->getY() && getWorld()->getPeach()->isWaiting()){
            //Checks if Peach is new
            if(!metPeach()){
                pausedAction(getWorld()->getPeach());
                //Marks Peach as having interacted with Baddie already
                setPeach(true);
            }//if new player
        }//if same square
        else setPeach(false); //Peach moves away
        
        //If baddie on same square as yoshi, and yoshi is in a waiting state
        if(getX()==getWorld()->getYoshi()->getX() && getY()==getWorld()->getYoshi()->getY() && getWorld()->getYoshi()->isWaiting()){
            //Checks if Yoshi is new
            if(!metYoshi()){
                pausedAction(getWorld()->getYoshi());
                //Marks Yoshi as having interacted with Baddie already
                setYoshi(true);
            }//if new player
        }//if same square
        else setYoshi(false); //Yoshi moves away
        
        //Decrement pause counter
        pauseCounter--;
        if(pauseCounter==0){
            //Set squares to move to random num [1,10]
            sTMove = nSquares();
            setTicks(sTMove*8);
            
            //Pick a new random direction to walk in, that is legal
            int newDir = moveInRandDir();
            setWalkDir(newDir);

            //Set baddie to walking state
            setState(WALKING);
            setPeach(false);
            setYoshi(false);
        }
    }//if paused state
    //If walking
    if(getState()==WALKING){
        //If on top of a square
        if(getX()%16==0 && getY()%16==0 && fork()){
            //Pick a random direction to walk in, that is legal
            int newDir = moveInRandDir();
            setWalkDir(newDir);
        }
        //Else if can't move forward
        else if(getX()%16==0 && getY()%16==0 && !canMove(getWalkDir())){
            corner();
        }
        //Move
        move();
    }
}

bool Baddie::metPeach() const{
    return peach;
}

bool Baddie::metYoshi() const{
    return yoshi;
}

bool Baddie::isImpactable() const{
    return true;
}

void Baddie::setPeach(bool tf){
    peach = tf;
}

void Baddie::setYoshi(bool tf){
    yoshi = tf;
}

void Baddie::setPC(int amt){
    pauseCounter = amt;
}

void Baddie::hit(){
    //Teleport bowser to another square on board
    teleport();
    //Set walking direction to right and sprite dir to 0 deg
    setWalkDir(right);
    //Transition to paused state
    setState(PAUSED);
    //Set pause ticks to 180
    setPC(180);
}

//****************
//||BOWSER CLASS||
//****************
Bowser::Bowser(StudentWorld* whereAmI, int imageID, int startX, int startY): Baddie(whereAmI, imageID, startX, startY){}
void Bowser::pausedAction(Player* plyr){
    //Simulate 50% chance
    int coinFlip = randInt(1,2);
    if(coinFlip==1){
        //Cause player to lose all their stars and coins
        plyr->addCoins(-plyr->getCoins());
        plyr->addStars(-plyr->getStars());
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }//if bowser causes player to lose all coins/stars
}
void Bowser::moveFunc(){
    setState(PAUSED);
    setPC(180);
    int chance = randInt(1,4); //Simulate 25% chance
    if(chance==1){
        getWorld()->convertSquare(getX()/16, getY()/16);
    }
}

int Bowser::nSquares(){
    return randInt(1,10);
}

//*************
//||BOO CLASS||
//*************
Boo::Boo(StudentWorld* whereAmI, int imageID, int startX, int startY): Baddie(whereAmI, imageID, startX, startY){}

void Boo::pausedAction(Player* plyr){
    
    Player* p1 = getWorld()->getPeach();
    Player* p2 = getWorld()->getYoshi();
    
    //Simulate 50/50 chance
    int coinFlip = randInt(1,2);
    switch(coinFlip){
        //Swap players coins
        case 1:
        {
            //Store p1 value
            int tempC = p1->getCoins();
            
            //Swap p1 coins w/ p2 coins
            p1->addCoins(-tempC+p2->getCoins());
            
            //Swap p2 coins w/ p1 coins
            p2->addCoins(-p2->getCoins()+tempC);
            break;
        }//case 1
        //Swap players stars
        case 2:
        {
            //Store p1 value
            int tempS = p1->getStars();
            
            //Swap p1 stars w/ p2 stars
            p1->addStars(-tempS+p2->getStars());
            
            //Swap p2 stars w/ p1 stars
            p2->addStars(-p2->getStars()+tempS);
            break;
        }//case 2
    }
}
void Boo::moveFunc(){
    setState(PAUSED);
    setPC(180);
}

//****************
//||Square CLASS||
//****************
Square::Square(StudentWorld* whereAmI, int imageID, int startX, int startY):Actor(whereAmI, imageID,startX,startY,right,1,1){}

void Square::doSomething(){
    //Check if square is alive
    if(!isAlive()){
        return;
    }
    //Check is Peach traverses a square
    if(getX()==getWorld()->getPeach()->getX() && getY()==getWorld()->getPeach()->getY()){
        //Check if Peach lands on a square
        if(getWorld()->getPeach()->hasLanded()){
            //Checks if Peach is new
            if(!getWorld()->getPeach()->isHere()){
                //Marks Peach as having activated square already
                getWorld()->getPeach()->setHere(true);
                
                landAction(getWorld()->getPeach());
            }
        }
        else traverseAction(getWorld()->getPeach());
    }
    //Check is Yoshi traverses a square
    if(getX()==getWorld()->getYoshi()->getX() && getY()==getWorld()->getYoshi()->getY()){
        //Check if Yoshi lands on a square
        if(getWorld()->getYoshi()->hasLanded()){
            //Checks if Yoshi is new
            if(!getWorld()->getYoshi()->isHere()){
                //Marks Yoshi as having activated the square already
                getWorld()->getYoshi()->setHere(true);
                
                landAction(getWorld()->getYoshi());
            }
        }
        else traverseAction(getWorld()->getYoshi());
    }
}

int Boo::nSquares(){
    return randInt(1,3);
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

void Coin::traverseAction(Player* plyr){}

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
    //Change walk direction and notify player that they landed on directional square
    plyr->setWalkDir(forcingDirection);
    plyr->setOnDirSQ(true);
}

void Directional::traverseAction(Player* plyr){landAction(plyr);}


//**************
//||BANK CLASS||
//**************
Bank::Bank(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Bank::landAction(Player* plyr){
    //Add coins to players balance
    plyr->addCoins(getWorld()->getBal());
    //Resets central bank
    getWorld()->setBal(0);
}

void Bank::traverseAction(Player* plyr){
    int playerBal = plyr->getCoins();
    if(playerBal<5){
        //Sets players coins to 0
        plyr->addCoins(-playerBal);
        //Adds coins to central bank
        getWorld()->setBal(getWorld()->getBal()+playerBal);
        //Play sound for depositing to bank
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    else{
        //Deducts 5 coins from player
        plyr->addCoins(-5);
        //Adds 5 coins to central bank
        getWorld()->setBal(getWorld()->getBal()+5);
        //Play sound for withdrawing from bank
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
}


//***************
//||EVENT CLASS||
//***************
Event::Event(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Event::landAction(Player* plyr){
    int randEvent = randInt(1,3);
    switch(randEvent){
        //Teleport player to random square
        case 1:
        {
            //Teleport player to random valid square
            plyr->teleport();
            
            //Play teleport sound
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            plyr->setTP(true);
            break;
        }//Case 1
        //Swap players
        case 2:
        {
            //Swap (x,y) coordinates, number of ticks left in roll, player walk direciton, player sprite direction, and player roll/walk state with other player
            Player* p1 = getWorld()->getPeach();
            Player* p2 = getWorld()->getYoshi();
            
            //Store p1 values
            int tempX = p1->getX();
            int tempY = p1->getY();
            int tempT = p1->getTicks();
            int tempDir = p1->getWalkDir();
            int tempRoll = p1->getRoll();
            int tempState = p1->getState();
            
            //Swap p1 values w/ p2 values
            p1->moveTo(p2->getX(),p2->getY());
            p1->setTicks(p2->getTicks());
            p1->setWalkDir(p2->getWalkDir());
            p1->setRoll(p2->getRoll());
            p1->setState(p2->getState());
            
            //Swap p2 values w/ p1 values
            p2->moveTo(tempX,tempY);
            p2->setTicks(tempT);
            p2->setWalkDir(tempDir);
            p2->setRoll(tempRoll);
            p2->setState(tempState);
            
            //Makes sure swapped player doesn't reactivate event square
            if(plyr==p1){
                p1->setHere(false);
                p2->setHere(true);
            }
            else{
                p1->setHere(true);
                p2->setHere(false);
            }
            
            //Play teleport sound
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            break;
        }//Case 2
        //Give player vortex
        case 3:
        {
            //If player doesn't have a vortex already, give them a vortex and play sound
            if(!plyr->hasVortex()){
                plyr->toggleVortex();
                getWorld()->playSound(SOUND_GIVE_VORTEX);
            }
            break;
        }//Case 3
    }
}

void Event::traverseAction(Player* plyr){}


//******************
//||DROPPING CLASS||
//******************
Dropping::Dropping(StudentWorld* whereAmI, int imageID, int startX, int startY):Square(whereAmI,imageID, startX, startY){}

void Dropping::landAction(Player* plyr){
    int randOption = randInt(1,2);
    switch(randOption){
        //Deduct 10 coins, or as many as possible if player has less than 10 coins, from player
        case 1:
        {
            int pBal = plyr->getCoins();
            pBal<10 ? plyr->addCoins(-pBal) : plyr->addCoins(-10);
            break;
        }
        //Deduct a star from player, if they have at least one star
        case 2:
        {
            if(plyr->getStars()>0) plyr->addStars(-1);
        }
    }
    //Play dropping square sound
    getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
}

void Dropping::traverseAction(Player* plyr){}

//************
//VORTEX CLASS
//************
Vortex::Vortex(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir):
Actor(whereAmI, imageID, startX, startY, right, 0, 1), walkDir(dir){}

void Vortex::doSomething(){
    if(!isAlive()) return;
    moveAtAngle(walkDir, 2); //Move 2 pixels in walk direction
    //Marks vortex as dead if it goes out of bounds
    if(getX()<0 || getX()>=VIEW_WIDTH || getY()<0 || getY()>=VIEW_HEIGHT){
        dead();
        return;
    }
    //Checks for impactable actor overlapping with vortex
    Actor* impactedActor = getWorld()->impactCheck(getX(), getY());
    //If such an actor is found
    if(impactedActor != nullptr){
        impactedActor->hit();
        //Set vortex to dead
        dead();
        //Play vortex hit sound
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}
