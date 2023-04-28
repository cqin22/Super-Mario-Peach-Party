#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//const int RIGHT = 0;
//const int UP = 90;
//const int LEFT = 180;
//const int DOWN = 270;

static const int GODOWN = 270;
static const int GOUP = 90;
static const int GOLEFT = 180;
static const int GORIGHT = 0;
static const int RAND = 8;


class StudentWorld;

/* Base Classes */
class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection = right, int depth = 0, int size = 1.0);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    bool atAFork();
    void goRandom(int & walking_direction);
    bool isValid(int dir, int walking);
    void updateDirection(int & m_walking_direction);
    bool isSquare();
    void setSq(bool sq);
    bool isActive();
    void setActive(bool a);


private:
    bool m_sq;
    bool m_active;
    StudentWorld* m_world;
};


class Player: public Actor // or some subclass of Actor
{
public:
    Player(StudentWorld* world, bool isPeach, int startX, int startY);
    void swapPeach(Player* other);
    int getCoins();
    int getStars();
    int getTick();

    virtual void doSomething();
    void addCoins();
    void takeCoins();
    void tradeCoins();
    bool isWaiting();
    void tick();
    void setCoinsZero();
    void setWalkingDirection(int direction);
    void addSpecificCoins(int amount);
    int deductCoins();
    int getTicksToMove();
    void teleport();
    void halveCoins();
    
private:
    int m_stars;
    int m_coins;
    bool m_waitingToRoll;
    bool m_isPeach;
    int m_ticks_to_move;
    int m_playerNum;
    int m_walking_direction;
    int m_tickCount;
    int m_vortex;
};

class ActivateOnPlayer : public Actor {
public:
  ActivateOnPlayer(StudentWorld *world, int imageID, int startX, int startY,
                   int dir, double size, int depth);
    bool isPeachLandedOn();
    bool isYoshiLandedOn();
    bool isPeachPassed();
    bool isYoshiPassed();
private:
    bool isActivated;
};


/* Squares */
class CoinSquare: public ActivateOnPlayer // or some subclass of Actor
{
public:
    CoinSquare(StudentWorld* world, int startX, int startY, bool good);
    virtual void doSomething();
private:
    bool m_isNewPeach;
    bool m_isNewYoshi;
    bool m_alive;
    bool m_grant;

};

class StarSquare: public ActivateOnPlayer{
public:
    StarSquare(StudentWorld* world, int x, int y);
    virtual void doSomething();
private:
};

class DirectionalSquare: public ActivateOnPlayer{
public:
    DirectionalSquare(StudentWorld* world, int x, int y, int direction);
    virtual void doSomething();
private:
    int m_direction;
};

class BankSquare: public ActivateOnPlayer{
public:
    BankSquare(StudentWorld* world, int x, int y);
    virtual void doSomething();
};

class EventSquare: public ActivateOnPlayer{
public:
    EventSquare(StudentWorld* world, int x, int y);
    virtual void doSomething();
};

class DroppingSquare: public ActivateOnPlayer{
public:
    DroppingSquare(StudentWorld* world, int x, int y);
    virtual void doSomething();
};

/* Players */
class Peach: public Player{
public:
    Peach(StudentWorld* world, int startX, int startY);
private:
};

class Yoshi: public Player{
public:
    Yoshi(StudentWorld* world, int startX, int startY);
};

class Enemy : public ActivateOnPlayer {
public:
    Enemy(StudentWorld *sw, int imageID, int startX, int startY);
    virtual void doSomething();
    void moveRandomly();
    
//  bool is_a_square() const;
//  bool can_be_hit_by_vortex() const;
//  void hit_by_vortex();  // called when enemy is hit by a vortex projectile (called by vortex projectile)
    
private:
    int m_pauseCounter;
    bool m_isWalking;
    int m_ticks_to_move;
    int m_walking_direction;
    bool m_isBowser;
};

class Bowser : public Enemy {
public:
    Bowser(StudentWorld *sw, int startX, int startY);
private:
};

class Boo : public Enemy {
public:
    Boo(StudentWorld *sw, int startX, int startY);
private:
};

#endif // ACTOR_H_
