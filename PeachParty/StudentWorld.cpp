#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
StudentWorld::~StudentWorld(){
    //Prevents double delete
    if(actors.size()>0){
        cleanUp();
    }
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), bankAcc(0)
{
}

int StudentWorld::init()
{
    //Load board
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if(result ==  Board::load_fail_file_not_found || result == Board::load_fail_bad_format) return GWSTATUS_BOARD_ERROR; //Returns GWSTATUS_BOARD_ERROR is board is improperly formatted or missing
    
    //Dynammically Allocate Actors
    for(int r=0;r<BOARD_WIDTH;r++){
        for(int c=0;c<BOARD_HEIGHT;c++){
            switch(bd.getContentsOf(c, r)){
                //Empty tile
                case(Board::empty):
                    break;
                //Player tile
                case(Board::player):
                    peachPointer = new Player(this, IID_PEACH, c*SPRITE_WIDTH,r*SPRITE_HEIGHT);
                    yoshiPointer = new Player(this, IID_YOSHI, c*SPRITE_WIDTH,r*SPRITE_HEIGHT);
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,3));
                    break;
                //Blue coin tile
                case(Board::blue_coin_square):
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,3));
                    break;
                //Red coin tile
                case(Board::red_coin_square):
                    actors.push_back(new Coin(this, IID_RED_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,-3));
                    break;
                //Star square tile
                case(Board::star_square):
                    actors.push_back(new Star(this,IID_STAR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                //Up directional square tile
                case(Board::up_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,Actor::up));
                    break;
                //Right direcitonal square tile
                case(Board::right_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,Actor::right));
                    break;
                //Down directional square tile
                case(Board::down_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,Actor::down));
                    break;
                //Left directional square tile
                case(Board::left_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,Actor::left));
                    break;
                //Bank square tile
                case(Board::bank_square):
                    actors.push_back(new Bank(this,IID_BANK_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                //Event square tile
                case(Board::event_square):
                    actors.push_back(new Event(this,IID_EVENT_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                //Boswer tile
                case(Board::bowser):
                    actors.push_back(new Bowser(this,IID_BOWSER, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,3));
                    break;
                //Boo tile
                case(Board::boo):
                    actors.push_back(new Boo(this,IID_BOO, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT,3));
                    break;
            }
        }//for
    }//for
    
	startCountdownTimer(99);  //Begin game timer
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    //Ask actors to do something
    //Check is things die later..
    peachPointer->doSomething();
    for(int i=0;i<actors.size();i++){
        actors[i]->doSomething();
    }
    
    //Update game stats
    string p1Stats = "P1 Roll: " + to_string(peachPointer->getRoll()) + " Stars: " + to_string(peachPointer->getStars()) + " $$: " + to_string(peachPointer->getCoins());
    peachPointer->hasVortex() ? p1Stats+=" VOR | " : p1Stats+=" | ";
    
    string p2Stats = "P2 Roll: " + to_string(yoshiPointer->getRoll()) + " Stars: " + to_string(yoshiPointer->getStars()) + " $$: " + to_string(yoshiPointer->getCoins());
    if(yoshiPointer->hasVortex()) p2Stats+=" VOR";
    
    setGameStatText(p1Stats + "Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getBal()) + " | " + p2Stats );
    
    if (timeRemaining() <= 0){
        playSound(SOUND_GAME_FINISHED); //Play sound when game ends
        
        //Placeholder for evaluating which player won
        return GWSTATUS_PEACH_WON;
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //Delete players
    delete peachPointer;
    delete yoshiPointer;
    while(actors.size()>0){
        Actor* toBeDeleted = actors[0];
        actors.erase(actors.begin());
        delete toBeDeleted;
    }
}

//Checks if a tile is empty
bool StudentWorld::isEmpty(int x, int y){
    if(bd.getContentsOf(x, y) == Board::empty) return true;
    return false;
}

Player* StudentWorld::getPeach(){
    return peachPointer;
}

Player* StudentWorld::getYoshi(){
    return yoshiPointer;
}

int StudentWorld::getBal() const{
    return bankAcc;
}

void StudentWorld::setBal(int amt){
    bankAcc = amt;
}
