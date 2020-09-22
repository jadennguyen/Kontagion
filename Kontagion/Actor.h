#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//#include "StudentWorld.h"
class StudentWorld;


////////////////////////////////////////////
//BASE CLASS: ACTOR//
/////////////////////////////////////////
class Actor: public GraphObject
{
  public:
    Actor(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable);
    
    virtual ~Actor();
    
    virtual void doSomething() {return;}
    bool isAlive();
    void setDead();
    bool isDamageable();
    virtual bool hasHP();
    virtual bool blockObjects();
    virtual bool isEdible();
    StudentWorld* getStudentWorld() const;
    virtual bool preventsLevelCompleting();
    
    
  private:
    bool m_alive;
    StudentWorld* m_studentWorld;
    bool m_isDamageable;
};

class Dirt: public Actor
{
  public:
    Dirt(int startX, int startY, StudentWorld* world);
    ~Dirt();
    virtual bool blockObjects();
    
};



class Pit: public Actor
{
  public:
    Pit(int startX, int startY, StudentWorld* world);
    ~Pit();
    virtual void doSomething();
    bool isEmpty();
    virtual bool preventsLevelCompleting();
    
  private:
    int m_regSalmonella;
    int m_aggSalmonella;
    int m_eColi;
};



//////////////////////////////////////////////////////////////////
//AGENTS: SOCRATES, BACTERIUM//
///////////////////////////////////////////////////////////////
class Agent: public Actor
{
  public:
    Agent(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world);
    ~Agent();
    int getHealth() const;
    void takeDamage(int damage);
    virtual bool hasHP();
    void setHealth(int number);
    virtual void playHurtSound();
    virtual void playDeadSound();
    
  private:
    int m_health;
};


class Socrates: public Agent
{
  public:
    Socrates(StudentWorld* world);
    ~Socrates();
    virtual void doSomething();
    int getSprayCharges() const;
    int getFlameCharges() const;
    void increaseFlameCharges();
    
  private:
    int m_positionalAngle;
    int m_sprayCharges;
    int m_flameCharges;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//BACTERIUM: ECOLI, SALMONELLA IMPLEMENTATIONS//
////////////////////////////////////////////////////////////////////////////////////////////////////
class Bacterium: public Agent
{
  public:
    Bacterium(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world);
    ~Bacterium();
    int getMovementPlanDistance() const;
    void decreaseMovementPlanDistance();
    void resetMovementPlanDistance();
    int getFood() const;
    void increaseFood();
    void resetFood();
    virtual bool preventsLevelCompleting();
    
 private:
    int m_movementPlanDistance;
    int m_food;
};

class Ecoli: public Bacterium
{
  public:
    Ecoli(int startX, int startY, StudentWorld* world);
    ~Ecoli();
    virtual void doSomething();
    virtual void playHurtSound();
    virtual void playDeadSound();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SALMONELLA: REGULAR SALMONELLA, AGGRESIVE SALMONELLA IMPLEMENTATIONS//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Salmonella: public Bacterium
{
  public:
    Salmonella(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world);
    ~Salmonella();
    virtual void playHurtSound();
    virtual void playDeadSound();
};

class RegularSalmonella: public Salmonella
{
  public:
    RegularSalmonella(int startX, int startY, StudentWorld* world);
    ~RegularSalmonella();
    virtual void doSomething();
};

class AggressiveSalmonella: public Salmonella
{
  public:
    AggressiveSalmonella(int startX, int startY, StudentWorld* world);
    ~AggressiveSalmonella();
    virtual void doSomething();
};


////////////////////////////////////////////////////////////
//PROJECTILE: FLAME, SPRAY//
////////////////////////////////////////////////////////
class Projectile: public Actor
{
  public:
    Projectile(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable);
    ~Projectile();
    int getTravelDistance() const;
    void increaseTravelDistance(int distance);
    
private:
    int m_travelDistance;
};

class Spray: public Projectile
{
  public:
    Spray(int startX, int startY, Direction startDirection,StudentWorld* world);
    ~Spray();
    virtual void doSomething();
    
};


class Flame: public Projectile
{
  public:
    Flame(int startX, int startY, Direction startDirection,StudentWorld* world);
    ~Flame();
    virtual void doSomething();
};

//////////////////////////////////////////////////////////////////////////////
//ACTIVATING OBJECTS: FOOD, GOODIE//
//////////////////////////////////////////////////////////////////////////
class ActivatingObject: public Actor
{
  public:
    ActivatingObject(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable);
    ~ActivatingObject();
};

class Food: public ActivatingObject
{
  public:
    Food(int startX, int startY, StudentWorld* world);
    ~Food();
    virtual bool isEdible();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GOODIE: RESTORE HEALTH, FLAMETHROWER, EXTRA LIFE, FUNGUS//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Goodie: public ActivatingObject
{
  public:
    Goodie(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, int lifeTime);
    ~Goodie();
    virtual void doSomething();
    virtual void performAbilities(Socrates* socrates) = 0;
    void decreaseLifeTime();
    int getLifeTime() const;
  private:
    int m_lifeTime;
};


class RestoreHealthGoodie: public Goodie
{
  public:
    RestoreHealthGoodie(int startX, int startY, StudentWorld* world, int lifeTime);
    ~RestoreHealthGoodie();
    virtual void performAbilities(Socrates* socrates);
};


class FlamethrowerGoodie: public Goodie
{
  public:
    FlamethrowerGoodie(int startX, int startY, StudentWorld* world, int lifeTime);
    ~FlamethrowerGoodie();
    //virtual void doSomething();
    virtual void performAbilities(Socrates* socrates);
};


class ExtraLifeGoodie: public Goodie
{
  public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld* world, int lifeTime);
    ~ExtraLifeGoodie();
    virtual void performAbilities(Socrates* socrates);
};

class Fungus: public Goodie
{
  public:
    Fungus(int startX, int startY, StudentWorld* world, int lifeTime);
    ~Fungus();
    virtual void performAbilities(Socrates* socrates);
};

#endif // ACTOR_H_
