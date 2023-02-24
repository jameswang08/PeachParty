#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor: public GraphObject{
  public:
    Actor(int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    void virtual doSomething() = 0;
  private:
    int spriteDirection; //Sprite Direction
};

class Peach: public Actor{
  public:
    Peach(int startX, int startY);
    void doSomething();
  private:
    int walkDirection; //Walk Direction
    int pNum; //Player Number
    int state; //Walking or Waiting State
    int tTMove; //Ticks to Move
};

class Coin: public Actor{
  public:
    Coin(int startX, int startY);
    void doSomething();
    bool isActive(); //Checks if coin square is alive or dead
  private:
    bool aliveStatus;
    int numCoins; //Number of coins to give/take
};

#endif // ACTOR_H_
