#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class GameObject;
class Player;




class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board getBoard();
    virtual ~StudentWorld();
    Peach* getPeach();
    Yoshi* getYoshi();
    int getBank();
    void addBank(int add);
    void deposit(int x, int y);


private:
    std::list<Actor*> m_actors;
    Peach* m_peachPointer;
    Yoshi* m_yoshiPointer;
    Board bd;
    int m_bank;
};

#endif // STUDENTWORLD_H_
