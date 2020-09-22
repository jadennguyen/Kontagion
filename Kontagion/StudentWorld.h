#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include "Actor.h"
#include <vector>

//class Actor;
//class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool withinRange(double x1, double y1, double x2, double y2, double r);
    bool closeToActors(double x, double y);
    bool collidedWithActor(double x, double y, Actor*& actor);
    bool overlapWithEdible(Actor*& edible, Actor* bacteria);
    bool isBacteriumMovementBlockedAt(double x, double y);
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;
    void getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle);
    bool anyNearbyEdible(Actor* a, int dist);
    void addActors(Actor* actor);
    void removeDeadActors();
    bool isLevelCompleted();
    
    Socrates* getOverlappingSocrates(Actor* a) const;
    std::string updateGameScore();
private:
    Socrates* m_player;
    std::vector<Actor*> m_actors;
    int m_numOfDirt;
    int m_numOfFood;
    int m_numOfPit;
    int m_maxLifeTime;
};

#endif // STUDENTWORLD_H_
