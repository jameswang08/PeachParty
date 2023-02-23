#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor: public GraphObject{
  public:
    Actor();
    void virtual doSomething() = 0;
  private:
    int spriteDirection; //Sprite Direction
};

class Peach: public Actor{
  public:
    Peach();
  private:
    int walkDirection; //Walk Direction
    int pNum; //Player Number
    int state; //Walking or Waiting State
    int tTMove; //Ticks to Move
};

class Coin: public Actor{
  public:
    Coin();
    bool isActive(); //Checks if coin square is alive or dead
  private:
    bool aliveStatus;
    int numCoins; //Number of coins to give/take
};

#endif // ACTOR_H_
