#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

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
                    peachPointer = new Peach;
                    break;
            }
        }//for
    }//for
    
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
