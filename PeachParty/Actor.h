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
    bool hasLanded(); //Checks if a player has landed on square
    int getStars(); //Checks how many stars player has
    int getCoins(); //Checks how many coins player has
    bool vortex(); //Checks if player has vortex
    int getRoll(); //Checks player's die roll
  private:
    //CONSTANTS
    static const int WALKING = 1;
    static const int WAITING = 2;
    const int P1 = 1;
    const int P2 = 2;
    
    int dieRoll; //Player's roll
    int walkDir; //Walk Direction
    int state; //Walking or Waiting State
    int tTMove; //Ticks to Move
    int nCoins; //Number of coins
    int nStars; //Number of stars
    bool hasVortex; //Whether a player has a vortex projectile or not
    bool landed; //Flag for when the player lands on a square
};

//Baddie base class for Boo and Bowser
class Baddie: public Actor{
  public:
    Baddie(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething();
  private:
    static const int WALKING = 1;
    static const int PAUSED = 2;
    int walkDir; //Walk direction
    int state; //State, WALKING or PAUSED
    int travelDist; //Travel distance
    int pauseCounter; //Pause counter
};

//Derived class for bowser
class Bowser: public Baddie{
  public:
    Bowser(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for boo
class Boo: public Baddie{
  public:
    Boo(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Square base class for all the squares
class Square: public Actor{
  public:
    Square(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething();
    bool isActive(); //Checks if coin square is alive or dead
  private:
    bool aliveStatus; //Whether the block is active or not
};

//Derived class for coin square
class Coin: public Square{
  public:
    Coin(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething();
    int getCoins();
    void setCoins(int amt);
  private:
    int nCoins; //Number of coins to give/take
};

//Derived class for Blue Coin Square
class Blue: public Coin{
  public:
    Blue(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for Red Coin Square
class Red: public Coin{
  public:
    Red(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for Red Coin Square

//Derived class for star square
class Star: public Square{
  public:
    Star(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for directional square
class Directional: public Square{
  public:
    Directional(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for bank square
class Bank: public Square{
  public:
    Bank(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
    int balance;
};

//Derived class for event square
class Event: public Square{
  public:
    Event(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};

//Derived class for dropping square
class Dropping: public Square{
  public:
    Dropping(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
};




#endif // ACTOR_H_
