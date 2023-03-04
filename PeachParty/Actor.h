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

//Base class for player and baddie
class Moves: public Actor{
  public:
    Moves(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething() = 0;
    
    //Helper funcs
    bool canMove(int direction); //Checks if player/baddie can move in this direction
    
    //Getters
    int getWalkDir() const; //Gets player/baddie walk direction
    int getState() const; //Gets state
    int getTicks() const; //Gets number of ticks player/baddie has left to move
    
    //Setters
    void setWalkDir(int newDir); //Sets walk direction
    void setState(int newState); //Sets player/baddie state
    void setTicks(int amt); //Sets the number of ticks player has left to move
    
  private:
    static const int PAUSED_OR_WAITING = 0; //Default state for player/baddie
    
    //Vars
    int walkDir; //Walk Direction
    int state; //Walking or Waiting State
    int tTMove; //Ticks to Move
};

class Player: public Moves{
  public:
    Player(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();

    //Getters
    int isWaiting() const; //Checks if player is in waiting state
    bool hasLanded() const; //Checks if a player has landed on square
    bool isHere() const; //Checks if a player is still on square
    int getStars() const; //Checks how many stars player has
    int getCoins() const; //Checks how many coins player has
    bool hasVortex() const; //Checks if player has vortex
    int getRoll() const; //Checks player's die roll

    //Setters
    void setHere(bool tf); //Sets here to true;
    void addStars(int amt); //Adds stars to players stars
    void addCoins(int amt); //Adds coins to players coins
    void toggleVortex(); //Sets vortex to opposite of what it was before
    void setRoll(int n); //Changes the players roll

  private:
    //CONSTANTS
    static const int WAITING = 0;
    static const int WALKING = 1;
    const int P1 = 1;
    const int P2 = 2;
    
    int dieRoll; //Player's roll
    int nCoins; //Number of coins
    int nStars; //Number of stars
    bool vortex; //Whether a player has a vortex projectile or not
    bool landed; //Flag for when the player lands on a square
    bool here; //Flag for if a player is staying on a square
};

//Baddie base class for Boo and Bowser
class Baddie: public Moves{
  public:
    Baddie(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
  private:
    static const int PAUSED = 0;
    static const int WALKING = 1;
    int travelDist; //Travel distance
    int pauseCounter; //Pause counter
    int sTMove; //Squares to move
};

//Derived class for bowser
class Bowser: public Baddie{
  public:
    Bowser(StudentWorld* whereAmI, int imageID, int startX, int startY);
  private:
};

//Derived class for boo
class Boo: public Baddie{
  public:
    Boo(StudentWorld* whereAmI, int imageID, int startX, int startY);
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
