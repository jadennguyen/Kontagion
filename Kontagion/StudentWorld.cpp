#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <algorithm>
#include <cmath>
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_maxLifeTime = max(randInt(0, 300 - 10*getLevel() - 1), 50);
    m_player = new Socrates(this);
    m_numOfDirt = max(180 - 20*getLevel(), 20);
    m_numOfFood = min(5*getLevel(), 25);
    m_numOfPit = getLevel();

    /*
    const double PI = 4 * atan(1);
    for (int i = 0; i < m_numOfDirt; i++)
    {
        int randRadius = randInt(0, 120);
        int randDirection = randInt(0, 360);
        double randX = randRadius * cos(randDirection * (PI/180));
        double randY = randRadius * sin(randDirection * (PI/180));
        m_actors.push_back(new Dirt(128 + randX, 128 + randY, this));
    }
    */
    
    //Create Dirt Piles

    for (int i = 0; i < m_numOfDirt; i++)
    {
        int x = randInt(0, VIEW_WIDTH);
        int y = randInt(0, VIEW_HEIGHT);
        while(!withinRange(x, y, VIEW_RADIUS, VIEW_RADIUS, 120))
        {
            x = randInt(0, VIEW_WIDTH);
            y = randInt(0, VIEW_HEIGHT);
        }
        m_actors.push_back(new Dirt(x,y,this));
    }
    
    //Create Food
    for(int i = 0; i < m_numOfFood; i++)
    {
        int randX = randInt(0, VIEW_WIDTH);
        int randY = randInt(0, VIEW_HEIGHT);
        
        while(!withinRange(randX, randY, VIEW_RADIUS, VIEW_RADIUS, 120) || closeToActors(randX, randY))
        {
            randX = randInt(0, VIEW_WIDTH);
            randY = randInt(0, VIEW_HEIGHT);
        }
        m_actors.push_back(new Food(randX, randY, this));
    }
    
    //Create Pit
    for (int i = 0; i < m_numOfPit; i++)
    {
        int randX = randInt(0, VIEW_WIDTH);
        int randY = randInt(0, VIEW_HEIGHT);
        
        while(!withinRange(randX, randY, VIEW_RADIUS, VIEW_RADIUS, 120) || closeToActors(randX, randY))
        {
            randX = randInt(0, VIEW_WIDTH);
            randY = randInt(0, VIEW_HEIGHT);
        }
        
        m_actors.push_back(new Pit(randX, randY, this));
    }
    
    //player->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    double PI = 4 * atan(1);
    
    
    //Create Fungus
    int ChanceFungus = max(510 - getLevel()*10,200);
    int randFungus = randInt(0, ChanceFungus - 1);
    if (randFungus == 0) {
        int randDirection = randInt(0, 360);
        double randX = VIEW_RADIUS * cos(randDirection * (PI/180));
        double randY = VIEW_RADIUS * sin(randDirection * (PI/180));
        m_actors.push_back(new Fungus(128 + randX, 128 + randY, this, m_maxLifeTime));
    }
    
    //Create Goodie
    int ChanceGoodie = max(510 - getLevel()*10, 250);
    int randGoodie = randInt(0, ChanceGoodie - 1);
    if (randGoodie == 0)
    {
        int goodieChoice = randInt(1, 10);
        int randDirection = randInt(0, 360);
        double randX = VIEW_RADIUS * cos(randDirection * (PI/180));
        double randY = VIEW_RADIUS * sin(randDirection * (PI/180));
        
        if(goodieChoice == 1)
        {
            m_actors.push_back(new ExtraLifeGoodie(128 + randX, 128 + randY, this, m_maxLifeTime));
        }else if(goodieChoice == 2 || goodieChoice == 3 || goodieChoice == 4)
        {
            m_actors.push_back(new FlamethrowerGoodie(128 + randX, 128 + randY, this, m_maxLifeTime));
            
        }else{
            m_actors.push_back(new RestoreHealthGoodie(128 + randX, 128 + randY, this, m_maxLifeTime));
        }
    }
    
    if (isLevelCompleted())
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if(m_player->isAlive()){
        m_player->doSomething();
    }
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        m_actors[i]->doSomething();
    }
    if (!m_player->isAlive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    string gameScore = updateGameScore();
    setGameStatText(gameScore);
    removeDeadActors();
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        delete *it;
        it = m_actors.erase(it);
    }
    delete m_player;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


//////////////////////////////////////////
//HELPER FUNCTIONS//
///////////////////////////////////////
bool StudentWorld::isLevelCompleted()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if(m_actors[i]->preventsLevelCompleting())
        {
            return false;
        }
    }
    return true;
}

bool StudentWorld::withinRange(double x1, double y1, double x2, double y2, double r)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double distance = sqrt(pow(dx, 2) + pow(dy, 2));
    if (distance <= r)
    {
        return true;
    }
    return false;
}


bool StudentWorld::closeToActors(double x, double y)
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        //Run through the vector of Actors and check if their Euclidean distance is within SPRITE_WIDTH of each other
        if (withinRange(x, y, (*it)->getX(), (*it)->getY(), SPRITE_WIDTH))
        {
            return true;
        }
        it++;
    }
    return false;
}

void StudentWorld::addActors(Actor* actor)
{
    m_actors.push_back(actor);
}

bool StudentWorld::collidedWithActor(double x, double y, Actor*& actor)
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->isDamageable() && withinRange(x, y, (*it)->getX(), (*it)->getY(), SPRITE_WIDTH))
        {
            actor = *it;
            return true;
        }
        it++;
    }
    return false;
}


void StudentWorld::removeDeadActors()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if (!(*it)->isAlive())
        {
            delete *it;
            it = m_actors.erase(it);
        }else{
            it++;
        }
    }
}

Socrates* StudentWorld::getOverlappingSocrates(Actor *a) const
{
    if (sqrt(pow(m_player->getX() - a->getX(),2) + pow(m_player->getY() - a->getY(), 2)) <= SPRITE_WIDTH)
    {
        return m_player;
    }
    return nullptr;
}

bool StudentWorld::overlapWithEdible(Actor *&edible, Actor *bacteria)
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->isEdible() && withinRange(bacteria->getX(), bacteria->getY(), (*it)->getX(), (*it)->getY(), SPRITE_WIDTH))
        {
            edible = *it;
            return true;
        }
        it++;
    }
    return false;
}


bool StudentWorld::getAngleToNearbySocrates(Actor *a, int dist, int &angle) const
{
    double PI = 4*atan(1);
    double dx = m_player->getX() - a->getX();
    double dy = m_player->getY() - a->getY();
    if (sqrt(pow(dx,2) + pow(dy, 2)) <= dist)
    {
        angle = atan2(dy,dx) * (180/PI);
        return true;
    }
    return false;
}

bool StudentWorld::isBacteriumMovementBlockedAt(double x, double y)
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->blockObjects() && withinRange(x, y, (*it)->getX(), (*it)->getY(), SPRITE_WIDTH/2))
        {
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::anyNearbyEdible(Actor *a, int dist)
{
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->isEdible() && withinRange(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), dist))
        {
            return true;
        }
        it++;
    }
    return false;
}

void StudentWorld::getAngleToNearestNearbyEdible(Actor *a, int dist, int &angle)
{
    vector<Actor*> nearestEdible;
    vector<Actor*>::iterator it = m_actors.begin();
    
    while (it != m_actors.end())
    {
        if ((*it)->isEdible() && withinRange(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), dist))
        {
            nearestEdible.push_back(*it);
        }
        it++;
    }
    
    vector<Actor*>::iterator q = nearestEdible.begin();
    
    for (vector<Actor*>::iterator p = nearestEdible.begin(); p != nearestEdible.end(); p++)
    {
    
        if (sqrt(pow((*p)->getX() - a->getX(), 2) + pow((*p)->getY() - a->getY(), 2)) < sqrt(pow((*q)->getX() - a->getX(), 2) + pow((*q)->getY() - a->getY(), 2)))
        {
            q = p;
        }
    }
    
    double PI = 4*atan(1);
    double dx = (*q)->getX() - a->getX();
    double dy = (*q)->getY() - a->getY();
    angle = atan2(dy, dx) * (180/PI);
}


string StudentWorld::updateGameScore()
{
    ostringstream oss;
    oss.fill('0');
    
    int numOfLives = getLives();
    int numOfLevels = getLevel();
    int score = getScore();
    int socratesHealth = m_player->getHealth();
    int numOfSprayCharges = m_player->getSprayCharges();
    int numOfFlameCharges = m_player->getFlameCharges();
    
    oss << "Score: " << setw(6) << score << setw(2) << "  " << "Level: " << setw(1) << numOfLevels << "  " << "Lives: " << setw(1) << numOfLives << "  " << "Health: " << setw(2) << socratesHealth << "  " << "Sprays: " << setw(2) << numOfSprayCharges << "  " << "Flames: " << setw(1) << numOfFlameCharges << endl;
    
    return oss.str();
}
