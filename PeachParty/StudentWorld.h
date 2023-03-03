#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Board.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
  public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isEmpty(int x, int y); //Checks if next tile is empty
    Player* getPeach();
    Player* getYoshi();
    int getBal() const; //Returns number of coins in the bank
    void setBal(int amt); //Sets the number of coins in the bank
  private:
    std::vector<Actor*> actors; //Vector containing list of all actors, except for Peach and Yoshi
    Player* peachPointer; //Pointer to Peach Actor
    Player* yoshiPointer; //Pointer to Yoshi Pointer
    Board bd;
    int bankAcc;
};

#endif // STUDENTWORLD_H_
