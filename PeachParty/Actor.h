#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject{
  public:
    Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    
  private:
    int spriteDirection; //Sprite Direction
    StudentWorld* world;
};

class Player: public Actor{
  public:
    Player(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
    bool canMove(int direction); //Checks if peach can move in this direction

    //Getters
    bool hasLanded() const; //Checks if a player has landed on square
    bool isHere() const; //Checks if a player is still on square
    int getStars() const; //Checks how many stars player has
    int getCoins() const; //Checks how many coins player has
    bool hasVortex() const; //Checks if player has vortex
    int getRoll() const; //Checks player's die roll
    int getTicks() const; //Gets number of ticks player has left to move
    int getWalkDir() const; //Gets player's walk direction
    int getState() const; //Gets plyer's state

    //Setters
    void setHere(bool tf); //Sets here to true;
    void addCoins(int amt); //Adds coins to players coins
    void addStars(int amt); //Adds stars to players stars
    void setWalkDir(int dir); //Sets players move dir and adjusts sprite direction as well
    void toggleVortex(); //Sets vortex to opposite of what it was before
    void setTicks(int amt); //Sets the number of ticks player has left to move
    void setRoll(int n); //Changes the players roll
    void setState(int newState); //Sets player's state
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
    bool vortex; //Whether a player has a vortex projectile or not
    bool landed; //Flag for when the player lands on a square
    bool here; //Flag for if a player is staying on a square
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
    bool isActive()  const; //Checks if coin square is alive or dead
    virtual void landAction(Player* plyr) = 0;//Specialized square action for if player lands on square
    virtual void traverseAction(Player* plyr) = 0;//Specialized square action for if player traverses square
  private:
    bool aliveStatus; //Whether the block is active or not
};

//Derived class for coin square
class Coin: public Square{
  public:
    Coin(StudentWorld* whereAmI, int imageID, int startX, int startY, int amt);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
    int coinModifier; //Number of coins to give/take
};

//Derived class for Red Coin Square

//Derived class for star square
class Star: public Square{
  public:
    Star(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
};

//Derived class for directional square
class Directional: public Square{
  public:
    Directional(StudentWorld* whereAmI, int imageID, int startX, int startY, int fDir);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
    int forcingDirection;
};

//Derived class for bank square
class Bank: public Square{
  public:
    Bank(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
};

//Derived class for event square
class Event: public Square{
  public:
    Event(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
};

//Derived class for dropping square
class Dropping: public Square{
  public:
    Dropping(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void landAction(Player* plyr);
    void traverseAction(Player* plyr);
  private:
};

#endif // ACTOR_H_
