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
: GameWorld(assetPath)
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
                case(Board::empty):
                    break;
                case(Board::player):
                    peachPointer = new Player(this, IID_PEACH, c*SPRITE_WIDTH,r*SPRITE_HEIGHT);
                    yoshiPointer = new Player(this, IID_YOSHI, c*SPRITE_WIDTH,r*SPRITE_HEIGHT);
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT, 3));
                    break;
                case(Board::blue_coin_square):
                    actors.push_back(new Coin(this, IID_BLUE_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT, 3));
                    break;
                case(Board::red_coin_square):
                    actors.push_back(new Coin(this, IID_RED_COIN_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT, -3));
                    break;
                case(Board::star_square):
                    actors.push_back(new Star(this,IID_STAR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                case(Board::up_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors[actors.size()-1]->setDirection(Actor::up);
                    break;
                case(Board::right_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors[actors.size()-1]->setDirection(Actor::right);
                    break;
                case(Board::down_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors[actors.size()-1]->setDirection(Actor::down);
                    break;
                case(Board::left_dir_square):
                    actors.push_back(new Directional(this,IID_DIR_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    actors[actors.size()-1]->setDirection(Actor::left);
                    break;
                case(Board::bank_square):
                    actors.push_back(new Bank(this,IID_BANK_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                case(Board::event_square):
                    actors.push_back(new Star(this,IID_EVENT_SQUARE, c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
            }
        }//for
    }//for
    
	startCountdownTimer(99);  //Begin game timer
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    if (timeRemaining() <= 0){
        playSound(SOUND_GAME_FINISHED); //Play sound when game ends
        
        //Placeholder for evaluating which player won
        return GWSTATUS_PEACH_WON;
    }
    
    //Ask actors to do something
    //Check is things die later..
    peachPointer->doSomething();
    for(int i=0;i<actors.size();i++){
        actors[i]->doSomething();
    }
    
    //Update game stats
    setGameStatText(to_string(timeRemaining()));
    
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
