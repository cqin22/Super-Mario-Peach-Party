#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iostream> // defines the overloads of the << operator
#include <iomanip>  // defines the manipulator setw

using namespace std;



GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}

int StudentWorld::init()
{
    getBoardNumber();
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found){
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
    }
    
    m_bank = 0;
    
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT; j++){
            switch(bd.getContentsOf(i, j)){
                case Board::player:{
                    Peach* peach = new Peach(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_peachPointer = peach;
                    m_actors.insert(m_actors.end(), peach);
                    
                    Yoshi* yoshi = new Yoshi(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_yoshiPointer = yoshi;
                    m_actors.insert(m_actors.end(), yoshi);
                    
                    CoinSquare* coin = new CoinSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, true);
                    m_actors.insert(m_actors.end(), coin);
                    break;
                }
                case Board::bowser:{
                    CoinSquare* coin = new CoinSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, true);
                    m_actors.insert(m_actors.end(), coin);
                    
                    Bowser* bowser = new Bowser(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_actors.insert(m_actors.end(), bowser);
                    break;
                }
                case Board::boo:{
                    CoinSquare* coin = new CoinSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, true);
                    m_actors.insert(m_actors.end(), coin);
                    
                    Boo* boo = new Boo(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_actors.insert(m_actors.end(),  boo);
                    break;
                }
                case Board::blue_coin_square:{
                    CoinSquare* coin = new CoinSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, true);
                    m_actors.insert(m_actors.end(), coin);
                    break;
                }
                case Board::red_coin_square:{
                    CoinSquare* coin = new CoinSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, false);
                    m_actors.insert(m_actors.end(), coin);
                    break;
                }
                case Board::star_square:{
                    StarSquare* star = new StarSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_actors.insert(m_actors.end(), star);
                    break;
                }
                case Board::down_dir_square:{
                    DirectionalSquare* dir = new DirectionalSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, GODOWN);
                    m_actors.insert(m_actors.end(), dir);
                    break;
                }
                case Board::up_dir_square:{
                    DirectionalSquare* dir = new DirectionalSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, GOUP);
                    m_actors.insert(m_actors.end(), dir);
                    break;
                }
                case Board::left_dir_square:{
                    DirectionalSquare* dir = new DirectionalSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, GOLEFT);
                    m_actors.insert(m_actors.end(), dir);

                    break;
                }
                case Board::right_dir_square:{
                    DirectionalSquare* dir4 = new DirectionalSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j, GORIGHT);
                    m_actors.insert(m_actors.end(), dir4);
                    break;
                }
                case Board::bank_square:{
                    BankSquare* b = new BankSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_actors.insert(m_actors.end(), b);
                    break;
                }
                case Board::event_square:{
                    EventSquare* event = new EventSquare(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                    m_actors.insert(m_actors.end(), event);
                    break;
                }
                default:
                    break;
            }

            

            }
        }
    
    
    startCountdownTimer(99);
    
    return GWSTATUS_CONTINUE_GAME; //must return
    
    

}

int StudentWorld::move()
{ //commandsd everything
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    ostringstream oss;  // oss is a name of our choosing.
    oss << "P1 Roll: " + to_string((m_peachPointer->getTicksToMove())/RAND) + " Stars: " + to_string(m_peachPointer->getStars()) +  " $$: " + to_string(m_peachPointer->getCoins()) + " | " + to_string(timeRemaining()) + " | Bank: " + to_string(m_bank) + " | " + "P2 Roll: " + to_string(m_yoshiPointer->getTicksToMove()/RAND) + " Stars: " + to_string(m_yoshiPointer->getStars()) +  " $$: " + to_string(m_yoshiPointer->getCoins());
    
    setGameStatText(oss.str());
    
    //to_string(m_peachPointer->getCoins()) + "    b:" + to_string(m_bank) + "   s:" + to_string(m_peachPointer->getStars())
          
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    for(; it != m_actors.end(); it++){
        if(!(*it)->isActive()){
            delete *it;
            it = m_actors.erase(it);
            it--;
        }
        
        if((*it)->isActive())(*it)->doSomething();
    }
    
    if (timeRemaining() <= 0){
        playSound(SOUND_GAME_FINISHED);
        bool peachWon = false;
        if(getPeach()->getStars() > getYoshi()->getStars()) peachWon = true;
        else if(getPeach()->getStars() == getYoshi()->getStars()){
            if(getPeach()->getCoins() > getYoshi()->getCoins()) peachWon = true;
        }
        
        if(peachWon){
            setFinalScore(getPeach()->getStars(), getPeach()->getCoins());
            return GWSTATUS_PEACH_WON;}
        else{
            setFinalScore(getYoshi()->getStars(), getYoshi()->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        
    }
        
        return GWSTATUS_CONTINUE_GAME;

}

Board StudentWorld::getBoard(){
    return bd;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

Peach* StudentWorld::getPeach(){
    return m_peachPointer;
}

Yoshi* StudentWorld::getYoshi(){
    return m_yoshiPointer;
}

int StudentWorld::getBank(){
    int temp = m_bank;
    m_bank = 0;
    return temp;
}

void StudentWorld::addBank(int add){
    m_bank += add;
}

void StudentWorld::cleanUp()
{
    auto it = m_actors.begin();
    
    for(; it != m_actors.end();){
        delete (*it);    //not wokring
        it = m_actors.erase(it);
    }

}

void StudentWorld::deposit(int x, int y){
    for(auto it = m_actors.begin(); it != m_actors.end(); it++){
        Actor *a = (*it);
        if(a->isSquare() && a->getY() == y && a->getX() == x){
            a->setActive(false);
            break;
        }
    }
    DroppingSquare* dropping = new DroppingSquare(this, x, y);
    m_actors.insert(m_actors.end(), dropping);
    playSound(SOUND_DROPPING_SQUARE_CREATED);
}

