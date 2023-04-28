#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor :: Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth, int size) : GraphObject(imageID, startX, startY, startDirection, depth, size){
    m_active = true;
    m_world = world;

} // how to do deal with the fact that constructor must have a definiton even if i never use it
bool Actor::isSquare() {
    return m_sq;
}

bool Actor::isActive(){
    return m_active;
}
void Actor::setActive(bool a){
    m_active = a;
}

void Actor::setSq(bool sq){
    m_sq = sq;
}

StudentWorld* Actor :: getWorld(){
    return m_world;
}

bool Actor::atAFork(){
    int count = 0;
    if(getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH + 1, this->getY()/SPRITE_HEIGHT)  != Board::empty) count++;
    if (getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH - 1, this->getY()/SPRITE_HEIGHT)  != Board::empty) count++;
    if (getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT + 1)  != Board::empty) count++;
    if (getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT - 1)  != Board::empty) count++;
    return count > 2;
}


void Actor::goRandom(int & walking_direction){
    vector<int> vect;

    if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH +1) != Board::empty) vect.push_back(GOUP);
    if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH -1) != Board::empty) vect.push_back(GODOWN);
    if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH-1, getY()/SPRITE_WIDTH) != Board::empty) vect.push_back(GOLEFT);
    if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_WIDTH) != Board::empty) vect.push_back(GORIGHT);
    int ran = randInt(1, vect.size());

    walking_direction = vect[ran-1];
}

bool Actor::isValid(int dir, int walking){
    switch(dir){
        case ACTION_UP:{
            dir = 90;
            break;
        }
        case ACTION_DOWN:{
            dir = 270;
            break;
        }
        case ACTION_LEFT:{
            dir = 180;
            break;
        }
        case ACTION_RIGHT:{
            dir = 0;
            break;
        }
    }
    if(walking == GODOWN) walking = GOUP;
    else if(walking == GOUP) walking = GODOWN;
    else if(walking == GOLEFT) walking = GORIGHT;
    else if(walking == GORIGHT) walking = GOLEFT;


    if(dir == walking) {
        return false;
    }
    switch(dir){
        case up:{
            if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH +1) == Board::empty)
                return false;
            break;
        }
        case down:{
            if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH -1) == Board::empty) return false;
            break;
        }
        case left:{
            if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH-1, getY()/SPRITE_WIDTH) == Board::empty) return false;
            break;
        }
        case right:{
            if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH+1, getY()/SPRITE_WIDTH) == Board::empty) return false;
            break;
        }
        default:
            return false;
        }
    
    return true;
}

void Player::teleport(){
    
    int rand1 = randInt(1, 16);
    int rand2 = randInt(1, 16);
    while(getWorld()->getBoard().getContentsOf(rand1, rand2)  == Board::empty) {
        rand1 = randInt(1, 16);
        rand2 = randInt(1, 16);
    }
    moveTo(rand1*SPRITE_WIDTH, rand2*SPRITE_HEIGHT);

    m_walking_direction = -1;

}



void Actor::updateDirection(int & m_walking_direction){
    switch(m_walking_direction){
        case right: {
            if(getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH + 1, this->getY()/SPRITE_HEIGHT)  == Board::empty){
                if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH + 1) == Board::empty) {
                    setDirection(right);
                    m_walking_direction = down;
                }
                else {
                    setDirection(right);
                    m_walking_direction = up;
                }
            }
            break;
        }
        case left: {
            if(getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH - 1, this->getY()/SPRITE_HEIGHT)  == Board::empty){
                if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH + 1) == Board::empty) {
                    setDirection(right);
                    m_walking_direction = down;
                }
                else {
                    setDirection(right);
                    m_walking_direction = up;
                }
            }
            break;
        }
        case down: {
            if(getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT - 1)  == Board::empty){
                if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_WIDTH) == Board::empty) {
                    setDirection(left);
                    m_walking_direction = left;
                }
                else {
                    setDirection(right);
                    m_walking_direction = right;
                }
            }
            break;
        }
        case up: {
            if(getWorld()->getBoard().getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT + 1)  == Board::empty){
                if(getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_WIDTH) == Board::empty) {
                    setDirection(left);
                    m_walking_direction = left;
                }
                else {
                    setDirection(right);
                    m_walking_direction = right;
                }
            }
            break;
        }
    }
}

void Player::swapPeach(Player* other){ //must always have peach as parameter
    int tempX = getX();
    int tempY = getY();
    int tempTick = m_ticks_to_move;
    int tempWalk = m_walking_direction;
    int tempSprite = getDirection();
    bool tempIsWalking = m_waitingToRoll;
    
    moveTo(other->getX(), other->getY());
    m_ticks_to_move = other->getTicksToMove();
    m_walking_direction = other->m_walking_direction;
    setDirection(other->getDirection());
    m_waitingToRoll = other->m_waitingToRoll;
    
    other->moveTo(tempX, tempY);
    other->m_ticks_to_move  = tempTick;
    other-> m_walking_direction = tempWalk;
    other->setDirection(tempSprite);
    other->m_waitingToRoll = tempIsWalking;

}

Player :: Player(StudentWorld* world, bool isPeach, int startX, int startY) : //why ask for imageID here?
Actor(world, isPeach ? IID_PEACH : IID_YOSHI, startX, startY), m_playerNum(isPeach ? 1 : 2)
{
    setSq(false);
    m_stars = 0;
    m_coins = 0;
    m_isPeach = isPeach;
    m_ticks_to_move = 0;
    m_waitingToRoll = true;
    m_walking_direction = right;
    m_tickCount = 0;
}

int Player :: getStars(){
    return m_stars;
}
void Player :: setCoinsZero(){
    m_coins = 0;
}

void Player :: addSpecificCoins(int amount){
    m_coins += amount;
}

void Player :: addCoins(){
    m_coins += 3;
}

int Player::getTicksToMove(){
    return m_ticks_to_move;
}

void Player :: takeCoins(){
    m_coins -= 3;
}

void Player :: tradeCoins(){
    m_coins -= 20;
    m_stars ++;
}

void Player:: halveCoins(){
    m_coins /= 2;
}

int Player :: deductCoins(){
    if(m_coins < 5){
        int temp = m_coins;
        m_coins = 0;
        return temp;
    }
    
    else m_coins -= 5;
        return 5;
}

void Player::setWalkingDirection(int direction){
    m_walking_direction = direction;
}


void Player:: tick(){
    m_tickCount++;
}

int Player:: getTick() {return m_tickCount;}

int Player :: getCoins(){
    return m_coins;
}

bool Player :: isWaiting(){
    return m_waitingToRoll;
}



void Player::doSomething()
{
    bool dont = false;
    if(m_waitingToRoll)
    {
        if(m_walking_direction == -1) goRandom(m_walking_direction);
        
        switch (getWorld()->getAction(m_playerNum))
        {
            case ACTION_ROLL:{
                int die_roll = randInt(1, 10);
                m_ticks_to_move = die_roll * 8;
                m_waitingToRoll = false;
                break;
            }
            default:
                return;
        }
    }
    else{ // if its walking
        int temp = m_walking_direction;
        int want = -1;
        if(atAFork() && getX()%SPRITE_WIDTH == 0 & getY() % SPRITE_WIDTH == 0 && getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH) != Board::up_dir_square &&
           getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH) != Board::down_dir_square &&
           getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH) != Board::left_dir_square &&
           getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_WIDTH) != Board::right_dir_square)
        {
            if(m_isPeach)
                want = getWorld()->getAction(1);
            else
                want = getWorld()->getAction(2);
            
            if(want != ACTION_UP && want != ACTION_DOWN && want != ACTION_LEFT && want != ACTION_RIGHT) return;
            if(isValid(want, temp)){
                if(want == ACTION_LEFT){
                    setDirection(left);
                }
                else setDirection(right);
                
                switch(want){
                    case ACTION_UP:
                        m_walking_direction = 90;
                        break;
                    case ACTION_DOWN:
                        m_walking_direction = 270;
                        break;
                    case ACTION_LEFT:
                        m_walking_direction = 180;
                        break;
                    case ACTION_RIGHT:
                        m_walking_direction = 0;
                        break;
                }
            }
            else dont = true;
        }


        
        
        if(getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_WIDTH == 0){
            updateDirection(m_walking_direction);
        }
        if(!dont){
            switch(m_walking_direction){
                case up: {
                    this->moveTo(getX(), getY()+2);
                    break;
                }
                case down:{
                    this->moveTo(getX(), getY()-2);
                    break;
                }
                case left:{
                    this->moveTo(getX()-2, getY());
                    break;
                }
                case right:{
                    this->moveTo(getX()+2, getY());
                    break;
                }
            }
            m_ticks_to_move--;
            if(m_ticks_to_move == 0) m_waitingToRoll = true;
            
            if(!m_waitingToRoll) m_tickCount = 0;
        }
    }
}


ActivateOnPlayer :: ActivateOnPlayer(StudentWorld *world, int imageID, int startX, int startY, int dir, double size, int depth) : Actor(world, imageID, startX, startY, dir, size, depth)
{
    setSq(true);
    isActivated = false;
}

bool ActivateOnPlayer :: isPeachLandedOn() {
    bool landedPeach = getWorld()->getPeach()->getY() == GraphObject::getY() && getWorld()->getPeach()->getX() == GraphObject::getX() && getWorld()->getPeach()->isWaiting();
    if(landedPeach) return true;
    else return false;
}

bool ActivateOnPlayer:: isYoshiLandedOn(){
    bool landedYoshi = getWorld()->getYoshi()->getY() == GraphObject::getY() && getWorld()->getYoshi()->getX() == GraphObject::getX() && getWorld()->getYoshi()->isWaiting();
//    cout <<getWorld()->getYoshi()->getY()/SPRITE_WIDTH << endl;
//    cout <<getY()/SPRITE_WIDTH;
    
    if(landedYoshi) return true;
    else return false;
}

bool ActivateOnPlayer::isYoshiPassed(){
    bool passedYoshi = getWorld()->getYoshi()->getY() == GraphObject::getY() && getWorld()->getYoshi()->getX() == GraphObject::getX() && !getWorld()->getYoshi()->isWaiting();
    return passedYoshi;
}

bool ActivateOnPlayer::isPeachPassed(){
    bool passedPeach = getWorld()->getPeach()->getY() == GraphObject::getY() && getWorld()->getPeach()->getX() == GraphObject::getX() && !getWorld()->getPeach()->isWaiting();
    
    return passedPeach;
}

/* CoinSquare Implementation */

CoinSquare :: CoinSquare(StudentWorld* world, int startX, int startY, bool good) : ActivateOnPlayer (world, good ? IID_BLUE_COIN_SQUARE : IID_RED_COIN_SQUARE, startX, startY, right, 1, 1) { // depth?
    m_isNewPeach = true;
    m_isNewYoshi = true;
    m_grant = good;
    m_alive = true;
};

void CoinSquare :: doSomething() {
    if(!isActive()) return;
    if(!m_alive)
        return;

    if(getWorld()->getYoshi()->getTick() == 0 && m_grant && isYoshiLandedOn()){
        getWorld()->getYoshi()->addCoins();
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    else if(getWorld()->getYoshi()->getTick() == 0 && !m_grant && isYoshiLandedOn()){
        if(getWorld()->getYoshi()->getCoins() < 3) getWorld()->getYoshi()->setCoinsZero();
        else getWorld()->getYoshi()->takeCoins();
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
    
    
    if(getWorld()->getPeach()->getTick() == 0 && m_grant && isPeachLandedOn()){
        getWorld()->getPeach()->addCoins();
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    else if(getWorld()->getPeach()->getTick() == 0 && !m_grant && isPeachLandedOn()){
        if(getWorld()->getPeach()->getCoins() < 3) getWorld()->getPeach()->setCoinsZero();
        else getWorld()->getPeach()->takeCoins();
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
    
    if(isPeachLandedOn())
        getWorld()->getPeach()->tick();
    
    if(isYoshiLandedOn())
        getWorld()->getYoshi()->tick();
};



/* StarSquare Implementation */

StarSquare ::StarSquare(StudentWorld* world, int x, int y): ActivateOnPlayer(world, IID_STAR_SQUARE, x, y, 0, 1, 1)
{
    
}

void StarSquare :: doSomething(){
    if(!isActive()) return;
    bool passedPeach = isPeachPassed() || isPeachLandedOn();
    
    if(passedPeach){
        if(getWorld()->getPeach()->getCoins() < 20) return;
        else {
            getWorld()->getPeach()->tradeCoins();
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
    
    bool passedYoshi = isYoshiPassed() || isYoshiLandedOn();
    
    if(passedYoshi){
        if(getWorld()->getYoshi()->getCoins() < 20) return;
        else {
            getWorld()->getYoshi()->tradeCoins();
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
}

/* Peach Implementation*/
Peach :: Peach(StudentWorld* world, int x, int y) : Player(world, true, x, y){
}

Yoshi :: Yoshi(StudentWorld* world, int startX, int startY): Player(world, false, startX, startY){}


/* Directional Square Implementation */

DirectionalSquare::DirectionalSquare(StudentWorld* world, int x, int y, int direction): ActivateOnPlayer(world, IID_DIR_SQUARE, x, y, direction, 1, 1){
    m_direction = direction;
}

void DirectionalSquare::doSomething(){
    if(!isActive()) return;
    bool passedPeach = isPeachPassed();
    bool passedYoshi = isYoshiPassed();
    if(passedPeach) {
        getWorld()->getPeach()->setWalkingDirection(m_direction);
        if(m_direction == left)
        getWorld()->getPeach()->setDirection(left);
        else getWorld()->getPeach()->setDirection(right);

    }
    if(passedYoshi){
        getWorld()->getYoshi()->setWalkingDirection(m_direction);
        if(m_direction == left)
        getWorld()->getYoshi()->setDirection(left);
        else getWorld()->getPeach()->setDirection(right);
    }
}

BankSquare::BankSquare(StudentWorld* world, int x, int y):ActivateOnPlayer(world, IID_BANK_SQUARE, x, y, 0, 1, 1){
    
}

void BankSquare::doSomething(){    if(!isActive()) return;
    if(getWorld()->getPeach()->getTick() == 0 && isPeachLandedOn()){
        int coin = getWorld()->getBank();
        getWorld()->getPeach()->addSpecificCoins(coin);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        
    }
    else if(isPeachPassed()){
        int add= getWorld()->getPeach()->deductCoins();
        getWorld()->addBank(add);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    
    
    if(getWorld()->getYoshi()->getTick() == 0 && isYoshiLandedOn()){
        int coin = getWorld()->getBank();
        getWorld()->getYoshi()->addSpecificCoins(coin);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
    else if(isYoshiPassed()){
        int add = getWorld()->getYoshi()->deductCoins();
        getWorld()->addBank(add);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    
    if(isPeachLandedOn())
        getWorld()->getPeach()->tick();
    
    if(isYoshiLandedOn())
        getWorld()->getYoshi()->tick();
}

EventSquare::EventSquare(StudentWorld* world, int x, int y): ActivateOnPlayer(world, IID_EVENT_SQUARE, x, y, 0, 1, 1){
    
}

void EventSquare::doSomething(){
    if(!isActive()) return;
    if((isYoshiLandedOn() && getWorld()->getYoshi()->getTick() == 0) || (isPeachLandedOn() && getWorld()->getPeach()->getTick() == 0)){
        int rand = randInt(1, 2);
        switch(rand){
            case 1:{
                getWorld()->getYoshi() -> swapPeach(getWorld()->getPeach());
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            case 2:{
                if(isYoshiLandedOn()) getWorld()->getYoshi()->teleport();
                if(isPeachLandedOn()) getWorld()->getPeach()->teleport();
                break;
            }
        }
    
    }
    if(isPeachLandedOn())
        getWorld()->getPeach()->tick();
    
    if(isYoshiLandedOn())
        getWorld()->getYoshi()->tick();
}



/* Baddies stuff */



Enemy :: Enemy(StudentWorld *sw, int imageID, int startX, int startY): ActivateOnPlayer(sw, imageID, startX, startY, GORIGHT, 1, 0){
    if(imageID == IID_BOWSER) m_isBowser = true;
    else m_isBowser = false;
    m_isWalking = false;
    m_pauseCounter = 180;
    m_ticks_to_move = 0;
    m_walking_direction = GORIGHT;
}

void Enemy::doSomething(){
    Peach* peach = getWorld()->getPeach();
    Yoshi* yoshi = getWorld()->getYoshi();
    if(!m_isWalking){
        if(peach->getX() == getX() && peach->getY() == getY() && peach->isWaiting() && peach->getTick()==0){
            int rand = randInt(1, 2);
            if(rand == 1){
                peach->halveCoins();
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        
        if(yoshi->getX() == getX() && yoshi->getY() == getY() && yoshi->isWaiting() && yoshi->getTick()==0){
            int rand = randInt(1, 2);
            if(rand == 1){
                peach->halveCoins();
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        
        m_pauseCounter--;
        if(m_pauseCounter == 0){
            int squares_to_move = randInt(1, 10);
            m_ticks_to_move = squares_to_move * 8;
            goRandom(m_walking_direction);
            m_isWalking = true;
        }
        
        if(isPeachLandedOn())
            getWorld()->getPeach()->tick();

        if(isYoshiLandedOn())
            getWorld()->getYoshi()->tick();
    }
    if(m_isWalking){
        if(atAFork() && getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_WIDTH == 0){
            goRandom(m_walking_direction);
            if(m_walking_direction == GOLEFT) setDirection(GOLEFT);
            else setDirection(GORIGHT);
        }
        if(getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_WIDTH == 0){
            updateDirection(m_walking_direction);
            if(m_walking_direction == GOLEFT) setDirection(GOLEFT);
            else setDirection(GORIGHT);
        }
        switch(m_walking_direction){
            case up: {
                this->moveTo(getX(), getY()+2);
                break;
            }
            case down:{
                this->moveTo(getX(), getY()-2);
                break;
            }
            case left:{
                this->moveTo(getX()-2, getY());
                break;
            }
            case right:{
                this->moveTo(getX()+2, getY());
                break;
            }
        }
        m_ticks_to_move--;
        if(m_ticks_to_move == 0){
            m_isWalking = false;
            m_pauseCounter = 180;
            if(m_isBowser){
                int ran = randInt(1, 4);
                if(ran){
                    getWorld()->deposit(getX(), getY());
                    getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                }
            }
        }
    }



}


Bowser::Bowser(StudentWorld *sw, int startX, int startY):Enemy(sw, IID_BOWSER, startX, startY){
    setSq(false);
}

Boo::Boo(StudentWorld *sw, int startX, int startY):Enemy(sw, IID_BOO, startX, startY){
    setSq(false);

}


DroppingSquare::DroppingSquare(StudentWorld* world, int x, int y):ActivateOnPlayer(world, IID_DROPPING_SQUARE, x, y, 0, 1, 1)
{
}

void DroppingSquare::doSomething(){
    
}
                                 


                                    
