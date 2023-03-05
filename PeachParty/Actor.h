#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject{
  public:
    Actor(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    virtual bool isLiving(); //Only Players/Baddies are living
    bool isAlive() const; //Checks if actor is alive or dead
    void dead(); //Sets actor to dead
    virtual bool isImpactable() const; //Indicates whether actor is impactable or not
    virtual void hit(); //Func for what actor should do, if it gets hit by a vortex
  private:
    int spriteDirection; //Sprite Direction
    StudentWorld* world; //Pointer to world actors are in
    bool aliveStatus; //Indicates whether actor is alive or dead
};

//Base class for player and baddie
class Moves: public Actor{
  public:
    Moves(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething() = 0;
    bool isLiving(); //Only living things move
    int moveInRandDir(); //Returns new valid random direction

    
    //Helper funcs
    bool canMove(int direction); //Checks if player/baddie can move in this direction
    void corner(); //Checks if player can't move forward, and if so then choose appropriate direction
    bool fork(); //Checks for fork
    virtual void move(); //Moves player/baddie
    virtual void moveFunc() = 0; //Pure virtual func for when ticks to move = 0
    
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
    void moveFunc();

    //Getters
    int isWaiting() const; //Checks if player is in waiting state
    bool hasLanded() const; //Checks if a player has landed on square
    bool isHere() const; //Checks if a player is still on square
    int getStars() const; //Checks how many stars player has
    int getCoins() const; //Checks how many coins player has
    bool hasVortex() const; //Checks if player has vortex
    int getRoll() const; //Checks player's die roll

    //Setters
    void setHere(bool tf); //Sets here to tf
    void addStars(int amt); //Adds stars to players stars
    void addCoins(int amt); //Adds coins to players coins
    void toggleVortex(); //Sets vortex to opposite of what it was before
    void setRoll(int n); //Changes the players roll
    void setOnDirSQ(bool tf); //Setter for onDirSq
    void setTP(bool tf); //Setter for teleported

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
    bool onDirSq; //Flag for when play lands on directional square
    bool teleported; //Flag for when player is teleported
};

//Baddie base class for Boo and Bowser
class Baddie: public Moves{
  public:
    Baddie(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void doSomething();
    virtual void moveFunc() = 0; //Func for what baddie does when they are walking
    virtual void pausedAction(Player* plyr) = 0; //Func for what baddie does when they are paused
    virtual int nSquares() = 0; //Func to get random num of squares to move
    
    //Getters
    bool metPeach() const; //Getter for peach contact
    bool metYoshi() const; //Getter for yoshi contact
    bool isImpactable() const; //Baddies are impactable
    
    //Setters
    void setPeach(bool tf); //Setter for peach contact
    void setYoshi(bool tf); //Setter for yoshi contact
    void setPC(int amt); //Setter for pauseCounter
    
  private:
    static const int PAUSED = 0;
    static const int WALKING = 1;
    int travelDist; //Travel distance
    int pauseCounter; //Pause counter
    int sTMove; //Squares to move
    bool peach; //Checks if in contact w/ peach
    bool yoshi; //Checks if baddie in contact w/ yoshi
    
};

//Derived class for bowser
class Bowser: public Baddie{
  public:
    Bowser(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void pausedAction(Player* plyr);
    void moveFunc(); //Specialized func for bowser movement
    int nSquares(); //Return random number of squares to move
  private:
    static const int PAUSED = 0;
    static const int WALKING = 1;
};

//Derived class for boo
class Boo: public Baddie{
  public:
    Boo(StudentWorld* whereAmI, int imageID, int startX, int startY);
    void pausedAction(Player* plyr);
    void moveFunc(); //Specialized func for boo movement
    int nSquares(); //Return random number of squares to move
  private:
    static const int PAUSED = 0;
    static const int WALKING = 1;
};

//Square base class for all the squares
class Square: public Actor{
  public:
    Square(StudentWorld* whereAmI, int imageID, int startX, int startY);
    virtual void doSomething();
    virtual void landAction(Player* plyr) = 0;//Specialized square action for if player lands on square
    virtual void traverseAction(Player* plyr) = 0;//Specialized square action for if player traverses square
  private:
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

//Derived class for vortex
class Vortex: public Actor{
  public:
    Vortex(StudentWorld* whereAmI, int imageID, int startX, int startY, int dir);
    void doSomething();
  private:
    int walkDir;
};

#endif // ACTOR_H_
