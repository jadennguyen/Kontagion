#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


/////////////////////////////////////////////////////
//ACTOR IMPLEMENTATIONS//
//////////////////////////////////////////////////
Actor::Actor(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable)
    : GraphObject(imageID, startX, startY, startDirection, depth), m_alive(true), m_studentWorld(world), m_isDamageable(isDamageable)
{}

Actor::~Actor()
{}

StudentWorld* Actor::getStudentWorld() const
{
    return m_studentWorld;
}

bool Actor::isAlive()
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
}

bool Actor::isDamageable()
{
    return m_isDamageable;
}

bool Actor::hasHP()
{
    return false;
}

bool Actor::blockObjects()
{
    return false;
}

bool Actor::isEdible()
{
    return false;
}

bool Actor::preventsLevelCompleting()
{
    return false;
}


///////////////////////////////////////////////////
//DIRT IMPLEMENTATIONS//
///////////////////////////////////////////////
Dirt::Dirt(int startX, int startY, StudentWorld* world)
    : Actor(IID_DIRT, startX, startY, 0, 1, world, true)
{}

Dirt::~Dirt()
{}

bool Dirt::blockObjects()
{
    return true;
}


///////////////////////////////////////////////////
//PIT IMPLEMENTATIONS//
///////////////////////////////////////////////
Pit::Pit(int startX, int startY, StudentWorld* world)
    :Actor(IID_PIT, startX, startY, 0, 1, world, false), m_regSalmonella(5), m_aggSalmonella(3), m_eColi(2)
{}

Pit::~Pit()
{}

bool Pit::preventsLevelCompleting()
{
    return true;
}

bool Pit::isEmpty()  //Return true if all of the bacteria counts are 0
{
    return m_regSalmonella == 0 && m_aggSalmonella == 0 && m_eColi == 0;
}
void Pit::doSomething()
{
    if(isEmpty())   // Once a pit create all of it bacteria, it then disappears from the game
    {
        setDead();
    }else{
        int pitChance = randInt(1, 50);  //Produce the chance of creating certain bacterias
        if (pitChance == 1)
        {
            int bacteriaChance = randInt(1, 3);
            if(m_eColi > 0 && bacteriaChance == 1)
            {
                Ecoli* newEcoli = new Ecoli(getX(), getY(), getStudentWorld());
                getStudentWorld()->addActors(newEcoli);
                m_eColi--;
            }else if (m_regSalmonella > 0 && bacteriaChance == 2)
            {
                RegularSalmonella* newRegularSalmonella = new RegularSalmonella(getX(), getY(), getStudentWorld());
                getStudentWorld()->addActors(newRegularSalmonella);
                m_regSalmonella--;
            }else if (m_aggSalmonella > 0 && bacteriaChance == 3){
                AggressiveSalmonella* newAgressiveSalmonella = new AggressiveSalmonella(getX(), getY(), getStudentWorld());
                getStudentWorld()->addActors(newAgressiveSalmonella);
                m_aggSalmonella--;
            }
            getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//AGENTS:SOCRATES, BACTERIA IMPLEMENTATIONS//
//////////////////////////////////////////////////////////////////////////////////////////////
Agent::Agent(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world)
    :Actor(imageID, startX, startY, startDirection, depth, world, true), m_health(health)
{}

Agent::~Agent()
{}

int Agent::getHealth() const  //Allow access to the m_health variable
{
    return m_health;
}

void Agent::takeDamage(int damage)
{
    m_health -= damage;
}

bool Agent::hasHP()
{
    return true;
}

void Agent::setHealth(int number)
{
    m_health = number;
}

void Agent::playHurtSound() //Play the hurt sound depending on what agent it is
{}

void Agent::playDeadSound() //Play the dead sound depending on what agent it is
{}


////////////////////////////////////////////////////////////
//SOCRATES IMPLEMENTATIONS//
////////////////////////////////////////////////////////
Socrates::Socrates(StudentWorld* world)
    : Agent(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, 100, world), m_positionalAngle(180), m_sprayCharges(20), m_flameCharges(5)
{}

Socrates::~Socrates()
{}

int Socrates::getSprayCharges() const   //Allow access to the m_sprayCharges member variable
{
    return m_sprayCharges;
}

int Socrates::getFlameCharges() const   //AlloW access to the m_flameCharges member variable
{
    return m_flameCharges;
}
    
void Socrates::increaseFlameCharges()
{
    m_flameCharges += 5;
}


void Socrates::doSomething(){
    //Try to get user input (if any is available)
    //const double PI = 4 * atan(1);
    if (!isAlive())
    {
        return;
    }
    int value;
    if(getStudentWorld()->getKey(value))
    {
        double PI = 4 * atan(1);
        // User hit a key during this tick!
        switch (value) {
            case KEY_PRESS_LEFT:  //Move Socrates counterclockwise
                //Adjust Socrates's positional angle
                m_positionalAngle += 5; //Move Socrates 5 degrees counterclockwise
                
                //Adjust Socrates's (x,y) coordinates around the perimeter of the Petri dish based on new positional angle
                moveTo(128 + 128 * cos(m_positionalAngle * (PI/180)), 128 + 128 * sin(m_positionalAngle * (PI/180)));
                
                //Set the direction that Socrates is facing to his positional angle + 180 degrees (causing him to always face toward the center of the Petri dish)
                setDirection((m_positionalAngle + 180) % 360) ;
                break;
            case KEY_PRESS_RIGHT:  //Move Socrates clockwise
                //Adjust Socrates's positional angle
                m_positionalAngle -= 5; //Move Socrates 5 degrees clockwise
                
                //Adjust Socrates's (x,y) coordinates around the perimeter of the Petri dish based on new positional angle
                moveTo(128 + 128 * cos(m_positionalAngle * (PI/180)), 128 + 128 * sin(m_positionalAngle * (PI/180)));
                
                //Set the direction that Socrates is facing to his positional angle + 180 degrees (causing him to always face toward the center of the Petri dish)
                setDirection((m_positionalAngle + 180) % 360);
                break;
            case KEY_PRESS_SPACE:
                //Add spray object SPRITE_WIDTH pixels directly in front
                if(m_sprayCharges > 0)
                {
                    double X,Y;
                    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, X, Y);
                    Spray* newSpray = new Spray(X, Y, getDirection(), getStudentWorld());
                    getStudentWorld()->addActors(newSpray);
                    m_sprayCharges--;
                    getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_ENTER:   //Create the flames when the enter key is pressed
                if(m_flameCharges > 0)
                {
                    
                    
                    for (int i = 0; i < 16; i++)
                    {
                        double X,Y;
                        getPositionInThisDirection(getDirection() + i*22, SPRITE_WIDTH, X, Y);
                        Flame* newFlame = new Flame(X, Y, getDirection() + i*22, getStudentWorld());
                        //Flame* newFlame = new Flame(getX() + SPRITE_WIDTH*cos(i*22*(PI/180)), getY() + SPRITE_WIDTH*sin(i*22*(PI/180)), getDirection() + i*22, getStudentWorld());
                        getStudentWorld()->addActors(newFlame);
                    }
                    m_flameCharges--;
                    getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
        }
    }else{
        if (m_sprayCharges < 20)
        {
            m_sprayCharges++;
        }
    }
    
   
}


//////////////////////////////////////////////////////////////
//BACTERIUM IMPLEMENTATIONS//
//////////////////////////////////////////////////////////
Bacterium::Bacterium(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world)
         :Agent(imageID, startX, startY, startDirection, depth, health, world), m_movementPlanDistance(0), m_food(0)
{}

Bacterium::~Bacterium()
{}

int Bacterium::getMovementPlanDistance() const
{
    return m_movementPlanDistance;
}

int Bacterium::getFood() const
{
    return m_food;
}

void Bacterium::increaseFood()
{
    m_food++;
}

void Bacterium::resetFood()
{
    m_food = 0;
}

void Bacterium::decreaseMovementPlanDistance()
{
    m_movementPlanDistance--;
}

void Bacterium::resetMovementPlanDistance()
{
    m_movementPlanDistance = 10;
}

bool Bacterium::preventsLevelCompleting()
{
    return true;
}


/////////////////////////////////////////////////////
//ECOLI IMPLEMENTATIONS//
//////////////////////////////////////////////////
Ecoli::Ecoli(int startX, int startY, StudentWorld* world)
     :Bacterium(IID_ECOLI, startX, startY, 90, 0, 5, world)
{}

Ecoli::~Ecoli()
{}

void Ecoli::playHurtSound()
{
    getStudentWorld()->playSound(SOUND_ECOLI_HURT);
}

void Ecoli::playDeadSound()
{
    getStudentWorld()->playSound(SOUND_ECOLI_DIE);
}

void Ecoli::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    Socrates *socrates = getStudentWorld()->getOverlappingSocrates(this);
    
    if (socrates != nullptr)  //Check if it overlaps with Socrates
    {
        socrates->takeDamage(4);
        getStudentWorld()->playSound(SOUND_PLAYER_HURT);
        if (socrates->getHealth() <= 0)
        {
            socrates->setDead();
            getStudentWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }else if (getFood() == 3)  //Check to see if it's eaten a total of 3 food since it was last divided or was born
    {
        double newX = getX();
        double newY = getY();
        
        if(getX() < VIEW_WIDTH/2)
        {
            newX += SPRITE_WIDTH/2;
        }else if(getX() > VIEW_WIDTH/2)
        {
            newX -= SPRITE_WIDTH/2;
        }
        if(getY() < VIEW_HEIGHT/2)
        {
            newY += SPRITE_WIDTH/2;
        }else if(getY() > VIEW_HEIGHT/2)
        {
            newY -= SPRITE_WIDTH/2;
        }
        
        Ecoli *newEcoli = new Ecoli(newX, newY, getStudentWorld());
        getStudentWorld()->addActors(newEcoli);
        getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
        resetFood();  //Reset food count back to 0 so it must now eat 3 food objects before it can divide again
    }
    int angleToSocrates;
    if (getStudentWorld()->getAngleToNearbySocrates(this, 256, angleToSocrates))
    {
        
        for(int i = 0; i < 10; i++)
        {
            double dx,dy;
            getPositionInThisDirection(angleToSocrates, 2, dx, dy);
            if(getStudentWorld()->withinRange(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2, VIEW_RADIUS) && !getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
            {
                moveTo(dx, dy);
                return;
            }else
            {
                angleToSocrates = (angleToSocrates + 10) % 360;
            }
        }
    }
    
    Actor* edible;
    if (getStudentWorld()->overlapWithEdible(edible, this))  //Check to see if it overlaps with one or more food objects
    {
        increaseFood();
        edible->setDead();
    }
}


////////////////////////////////////////////////////////////////
//SALMONELLA IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////
Salmonella::Salmonella(int imageID, int startX, int startY, Direction startDirection, int depth, int health, StudentWorld* world)
          :Bacterium(imageID, startX, startY, startDirection, depth, health, world)
{}

Salmonella::~Salmonella()
{}

void Salmonella::playHurtSound()
{
    getStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Salmonella::playDeadSound()
{
    getStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
}


//////////////////////////////////////////////////////////////////////////////////
//REGULAR SALMONELLA IMPLEMENTATIONS//
//////////////////////////////////////////////////////////////////////////////
RegularSalmonella::RegularSalmonella(int startX, int startY, StudentWorld* world)
                 :Salmonella(IID_SALMONELLA, startX, startY, 90, 0, 4, world)
{}

RegularSalmonella::~RegularSalmonella()
{}

void RegularSalmonella::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    Socrates *socrates = getStudentWorld()->getOverlappingSocrates(this);
    
    if (socrates != nullptr)  //Check if it overlaps with Socrates
    {
        socrates->takeDamage(1);
        getStudentWorld()->playSound(SOUND_PLAYER_HURT);
        if (socrates->getHealth() <= 0)
        {
            socrates->setDead();
            getStudentWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }else if (getFood() == 3)  //Check to see if it's eaten a total of 3 food since it was last divided or was born
    {
        double newX = getX();
        double newY = getY();
        
        if(getX() < VIEW_WIDTH/2)
        {
            newX += SPRITE_WIDTH/2;
        }else if(getX() > VIEW_WIDTH/2)
        {
            newX -= SPRITE_WIDTH/2;
        }
        
        if(getY() < VIEW_HEIGHT/2)
        {
            newY += SPRITE_WIDTH/2;
        }else if(getY() > VIEW_HEIGHT/2)
        {
            newY -= SPRITE_WIDTH/2;
        }
        
        RegularSalmonella *newRegularSalmonella= new RegularSalmonella(newX, newY, getStudentWorld());
        getStudentWorld()->addActors(newRegularSalmonella);
        getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
        resetFood();  //Reset food count back to 0 so it must now eat 3 food objects before it can divide again
    }

    if (getMovementPlanDistance() > 0)
    {
        decreaseMovementPlanDistance();
        double dx, dy;
        getPositionInThisDirection(getDirection(), 3, dx, dy);
        if (getStudentWorld()->withinRange(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2, VIEW_RADIUS) && !getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
        {
            moveTo(dx, dy);
            
        }else{
            int randDirection = randInt(0, 359);
            setDirection(randDirection);
            resetMovementPlanDistance();
        }
        return;
    }else if(getStudentWorld()->anyNearbyEdible(this, 128))
    {
        int angleToNearestEdible;
        getStudentWorld()->getAngleToNearestNearbyEdible(this, 128, angleToNearestEdible);
        double dx, dy;
        getPositionInThisDirection(angleToNearestEdible, 3, dx, dy);
        if (getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
        {
            int randDirection = randInt(0, 359);
            setDirection(randDirection);
            resetMovementPlanDistance();
            return;
        }
        moveTo(dx, dy);
    }else if(!getStudentWorld()->anyNearbyEdible(this, 128))
    {
        int randDirection = randInt(0, 359);
        setDirection(randDirection);
        resetMovementPlanDistance();
        return;
    }
    
    Actor* edible;
    if (getStudentWorld()->overlapWithEdible(edible, this))  //Check to see if it overlaps with one or more food objects
    {
        increaseFood();
        edible->setDead();
    }
}


////////////////////////////////////////////////////////////////////////////////////////
//AGGRESSIVE SALMONELLA IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////////////////////////////
AggressiveSalmonella::AggressiveSalmonella(int startX, int startY, StudentWorld* world)
                    :Salmonella(IID_SALMONELLA, startX, startY, 90, 0, 10, world)
{}

AggressiveSalmonella::~AggressiveSalmonella()
{}

void AggressiveSalmonella::doSomething()
{
    //Step 1
    if(!isAlive())
    {
        return;
    }
    
    //Step 2
    int angleToSocrates;
    if (getStudentWorld()->getAngleToNearbySocrates(this, 72, angleToSocrates))
    {
        double dx, dy;
        getPositionInThisDirection(angleToSocrates, 3, dx, dy);
        if (getStudentWorld()->withinRange(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2, VIEW_RADIUS) && !getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
        {
            moveTo(dx, dy);
        }
        //Next it will perform steps 3-5 from below, and then immediately return. It must NOT perform steps 6 and beyond in this case.
        
        //Step 3
        Socrates *socrates = getStudentWorld()->getOverlappingSocrates(this);
        if (socrates != nullptr)  //Check if it overlaps with Socrates
        {
            socrates->takeDamage(2);
            getStudentWorld()->playSound(SOUND_PLAYER_HURT);
            if (socrates->getHealth() <= 0)
            {
                socrates->setDead();
                getStudentWorld()->playSound(SOUND_PLAYER_DIE);
            }
            return;
        }
        
        //Step 4
        if (getFood() == 3)  //Check to see if it's eaten a total of 3 food since it was last divided or was born
        {
            double newX = getX();
            double newY = getY();
            
            if(getX() < VIEW_WIDTH/2)
            {
                newX += SPRITE_WIDTH/2;
            }else if(getX() > VIEW_WIDTH/2)
            {
                newX -= SPRITE_WIDTH/2;
            }
            
            if(getY() < VIEW_HEIGHT/2)
            {
                newY += SPRITE_WIDTH/2;
            }else if(getY() > VIEW_HEIGHT/2)
            {
                newY -= SPRITE_WIDTH/2;
            }
            
            AggressiveSalmonella *newAggressiveSalmonella= new AggressiveSalmonella(newX, newY, getStudentWorld());
            getStudentWorld()->addActors(newAggressiveSalmonella);
            getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
            resetFood();  //Reset food count back to 0 so it must now eat 3 food objects before it can divide again
            return;
        }
        
        //Step 5
        Actor* edible;
        if (getStudentWorld()->overlapWithEdible(edible, this))  //Check to see if it overlaps with one or more food objects
        {
            increaseFood();
            edible->setDead();
        }
        return;
    }
    
    //Step 3
    Socrates *socrates = getStudentWorld()->getOverlappingSocrates(this);
    if (socrates != nullptr)  //Check if it overlaps with Socrates
    {
        socrates->takeDamage(2);
        getStudentWorld()->playSound(SOUND_PLAYER_HURT);
        if (socrates->getHealth() <= 0)
        {
            socrates->setDead();
            getStudentWorld()->playSound(SOUND_PLAYER_DIE);
        }
    }else if (getFood() == 3)  //Step 4: Check to see if it's eaten a total of 3 food since it was last divided or was born
    {
        double newX = getX();
        double newY = getY();
        
        if(getX() < VIEW_WIDTH/2)
        {
            newX += SPRITE_WIDTH/2;
        }else if(getX() > VIEW_WIDTH/2)
        {
            newX -= SPRITE_WIDTH/2;
        }
        
        if(getY() < VIEW_HEIGHT/2)
        {
            newY += SPRITE_WIDTH/2;
        }else if(getY() > VIEW_HEIGHT/2)
        {
            newY -= SPRITE_WIDTH/2;
        }
        
        AggressiveSalmonella *newAggressiveSalmonella= new AggressiveSalmonella(newX, newY, getStudentWorld());
        getStudentWorld()->addActors(newAggressiveSalmonella);
        getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
        resetFood();  //Reset food count back to 0 so it must now eat 3 food objects before it can divide again
    }
    
    //Step 5
    Actor* edible;
    if (getStudentWorld()->overlapWithEdible(edible, this))  //Check to see if it overlaps with one or more food objects
    {
        increaseFood();
        edible->setDead();
    }
    
    //Step 6
    
    if (getMovementPlanDistance() > 0)
    {
        decreaseMovementPlanDistance();
        double dx, dy;
        getPositionInThisDirection(getDirection(), 3, dx, dy);
        if (getStudentWorld()->withinRange(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2, VIEW_RADIUS) && !getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
        {
            moveTo(dx, dy);
            
        }else{
            int randDirection = randInt(0, 359);
            setDirection(randDirection);
            resetMovementPlanDistance();
        }
        return;
    }else if(getStudentWorld()->anyNearbyEdible(this, 128)) //Step 7
    {
        int angleToNearestEdible;
        getStudentWorld()->getAngleToNearestNearbyEdible(this, 128, angleToNearestEdible);
        double dx, dy;
        getPositionInThisDirection(angleToNearestEdible, 3, dx, dy);
        if (getStudentWorld()->isBacteriumMovementBlockedAt(dx, dy))
        {
            int randDirection = randInt(0, 359);
            setDirection(randDirection);
            resetMovementPlanDistance();
            return;
        }else{
            moveTo(dx, dy);
        }
    }else if(!getStudentWorld()->anyNearbyEdible(this, 128))
    {
        int randDirection = randInt(0, 359);
        setDirection(randDirection);
        resetMovementPlanDistance();
        return;
    }
}


////////////////////////////////////////////////////////////////
//PROJECTILE IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////
Projectile::Projectile(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable)
          :Actor(imageID, startX, startY, startDirection, depth, world, isDamageable), m_travelDistance(0)
{}

Projectile::~Projectile()
{}

int Projectile::getTravelDistance() const
{
    return m_travelDistance;
}

void Projectile::increaseTravelDistance(int distance)
{
    m_travelDistance += distance;
}


/////////////////////////////////////////////////////
//SPRAY IMPLEMENTATIONS//
//////////////////////////////////////////////////
Spray::Spray(int startX, int startY, Direction startDirection, StudentWorld* world)
     :Projectile(IID_SPRAY, startX, startY, startDirection, 1, world, false)
{}


Spray::~Spray()
{}

void Spray::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    
    //check to see if it overlaps with a damageable object
    Actor* actor;
    if (getStudentWorld()->collidedWithActor(getX(), getY(), actor))
    {
        //Tell single impacted object that it has been damaged with 2 hit points of damage
        if(actor->hasHP())
        {
            dynamic_cast<Agent*>(actor)->takeDamage(2);
            dynamic_cast<Agent*>(actor)->playHurtSound();
            if(dynamic_cast<Agent*>(actor)->getHealth() <= 0)
            {
                int foodDropChance = randInt(1, 2);
                if (foodDropChance == 1)
                {
                    Food* newFood = new Food(actor->getX(), actor->getY(), getStudentWorld());
                    getStudentWorld()->addActors(newFood);
                }
                actor->setDead();
                getStudentWorld()->increaseScore(100);
            }
            
        }else{
            actor->setDead();
        }
        //Immediately set its status to dead, so spray goes away immediately
        setDead();
        return;
    }
    moveAngle(getDirection(), SPRITE_WIDTH);
    increaseTravelDistance(SPRITE_WIDTH);
    if (getTravelDistance() == 112)
    {
        setDead();
    }
}


/////////////////////////////////////////////////////
//FLAME IMPLEMENTATIONS//
//////////////////////////////////////////////////
Flame::Flame(int startX, int startY, Direction startDirection,StudentWorld* world)
     :Projectile(IID_FLAME, startX, startY, startDirection, 1, world, false)
{}

Flame::~Flame()
{}

void Flame::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    
    //check to see if it overlaps with a damageable object
    Actor* actor;
    if (getStudentWorld()->collidedWithActor(getX(), getY(), actor))
    {
        //Tell single impacted object that it has been damaged with 5 hit points of damage
        if(actor->hasHP())
        {
            dynamic_cast<Agent*>(actor)->takeDamage(5);
            dynamic_cast<Agent*>(actor)->playHurtSound();
            if(dynamic_cast<Agent*>(actor)->getHealth() <= 0)
            {
                int foodDropChance = randInt(1, 2);
                if (foodDropChance == 1)
                {
                    Food* newFood = new Food(actor->getX(), actor->getY(), getStudentWorld());
                    getStudentWorld()->addActors(newFood);
                }
                actor->setDead();
                getStudentWorld()->increaseScore(100);
            }
            
        }else{
            actor->setDead();
        }
        //Immediately set its status to dead, so spray goes away immediately
        setDead();
        return;
    }
    moveAngle(getDirection(), SPRITE_WIDTH);
    increaseTravelDistance(SPRITE_WIDTH);
    if (getTravelDistance() == 32)
    {
        setDead();
    }
}


///////////////////////////////////////////////////////////////////////////////
//ACTIVATING OBJECT IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////////////////////
ActivatingObject::ActivatingObject(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, bool isDamageable)
                :Actor(imageID, startX, startY, startDirection, depth, world, isDamageable)
{}


ActivatingObject::~ActivatingObject()
{}


///////////////////////////////////////////////////
//FOOD IMPLEMENTATIONS//
///////////////////////////////////////////////
Food::Food(int startX, int startY, StudentWorld* world)
    : ActivatingObject(IID_FOOD, startX, startY, 90, 1, world, false)
{}

Food::~Food()
{}

bool Food::isEdible()
{
    return true;
}


///////////////////////////////////////////////////////
//GOODIE IMPLEMENTATIONS//
////////////////////////////////////////////////////
Goodie::Goodie(int imageID, int startX, int startY, Direction startDirection, int depth, StudentWorld* world, int lifeTime)
      :ActivatingObject(imageID, startX, startY, startDirection, depth, world, true), m_lifeTime(lifeTime)
{}

Goodie::~Goodie()
{}

int Goodie::getLifeTime() const
{
    return m_lifeTime;
}

void Goodie::decreaseLifeTime()
{
    m_lifeTime--;
}

void Goodie::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    Socrates* socrates = getStudentWorld()->getOverlappingSocrates(this);
    
    if(socrates != nullptr)
    {
        setDead();
        performAbilities(socrates);
        return;
    }
    
    if (getLifeTime() == 0)
    {
        setDead();
    }else{
        decreaseLifeTime();
    }
}


/////////////////////////////////////////////////////////////////////////
//RESTORE HEALTH IMPLEMENTATIONS//
//////////////////////////////////////////////////////////////////////
RestoreHealthGoodie::RestoreHealthGoodie(int startX, int startY, StudentWorld* world, int lifeTime)
                   :Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, 0, 1, world, lifeTime)
{}

RestoreHealthGoodie::~RestoreHealthGoodie()
{}

void RestoreHealthGoodie::performAbilities(Socrates* socrates)
{
    socrates->setHealth(100);
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->increaseScore(250);
}


////////////////////////////////////////////////////////////////////////////////////
//FLAMETHROWER GOODIE IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////////////////////////
FlamethrowerGoodie::FlamethrowerGoodie(int startX, int startY, StudentWorld* world, int lifeTime)
                  :Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, 0, 1, world, lifeTime)
{}

FlamethrowerGoodie::~FlamethrowerGoodie()
{}

void FlamethrowerGoodie::performAbilities(Socrates* socrates)
{
    socrates->increaseFlameCharges();
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->increaseScore(300);
}


////////////////////////////////////////////////////////////////
//EXTRA LIFE IMPLEMENTATIONS//
/////////////////////////////////////////////////////////////
ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY, StudentWorld* world, int lifeTime)
                  :Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, 0, 1, world, lifeTime)
{}

ExtraLifeGoodie::~ExtraLifeGoodie()
{}

void ExtraLifeGoodie::performAbilities(Socrates* socrates)
{
    socrates->getStudentWorld()->incLives();
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->increaseScore(500);
}


///////////////////////////////////////////////////////
//FUNGUS IMPLEMENTATIONS//
///////////////////////////////////////////////////
Fungus::Fungus(int startX, int startY, StudentWorld* world, int lifeTime)
      :Goodie(IID_FUNGUS, startX, startY, 0, 1, world, lifeTime)
{}

Fungus::~Fungus()
{}

void Fungus::performAbilities(Socrates *socrates)
{
    socrates->takeDamage(20);
    getStudentWorld()->playSound(SOUND_PLAYER_HURT);
    if (socrates->getHealth() <= 0)
    {
        socrates->setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_DIE);
    }
    getStudentWorld()->increaseScore(-50);
    
}
