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
    cleanUp();
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
                case(Board::player):
                    peachPointer = new Peach(c*SPRITE_WIDTH,r*SPRITE_HEIGHT);
                    actors.push_back(new Coin(c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
                    break;
                case(Board::blue_coin_square):
                    actors.push_back(new Coin(c*SPRITE_WIDTH,r*SPRITE_HEIGHT));
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
}
