#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject{
  public:
    Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    void virtual doSomething() = 0;
    StudentWorld* getWorld();
  private:
    int spriteDirection; //Sprite Direction
    StudentWorld* world;
};

class Player: public Actor{
  public:
    Player(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
    bool canMove(int direction); //Checks if peach can move in this direction
  private:
    //CONSTANTS
    const int WALKING = 1;
    const int WAITING = 2;
    const int P1 = 1;
    const int P2 = 2;
    
    int walkDir; //Walk Direction
    int pNum; //Player Number
    int state; //Walking or Waiting State
    int tTMove; //Ticks to Move
    int nCoins; //Number of coins
    int nStars; //Number of stars
    bool hasVortex; //Whether a player has a vortex projectile or not

};

class Coin: public Actor{
  public:
    Coin(StudentWorld* whereAmI, int imageID, int startX, int startY, int amt);
    void doSomething();
    bool isActive(); //Checks if coin square is alive or dead
  private:
    bool aliveStatus; //Whether the block is active or not
    int numCoins; //Number of coins to give/take
};

#endif // ACTOR_H_
