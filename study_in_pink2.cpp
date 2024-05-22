#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
const Position Position::npos = Position(-1, -1);
//          Code bên dưới

// MapElement

// MapElement - Path

// MapElement - Wall

// MapElement - FakeWall

// Map

// Position

// MovingObject

// MovingObject - Sherlock

// MovingObject - Watson

// MovingObject - Criminal

// MovingObject - Robot

// Robot - RobotC

// Robot - RobotS

// Robot - RobotW

// Robot - RobotSW

// ArrayMovingObject

// BaseItem

// BaseItem - MagicBook

// BaseItem - EnergyDrink

// BaseItem - FirstAid

// BaseItem - ExcemptionCard

// BaseItem - PassingCard

// BaseBag

// BaseBag - SherlockBag


bool PassingCard::canUse(Character* obj, Robot* robot)
{
    return (obj->getName() == "Watson" && obj->getHP() % 2 != 0);
}
void PassingCard::use(Character* obj, Robot* robot)
{
    if (!canUse(obj, robot)) return;
    if (this->chal == "all")
        return;
    if (this->chal == robot->getName())
        return;
    if (obj->getEXP() >= 50)
        obj->setEXP(obj->getEXP() - 50);
    else
        obj->setEXP(0);
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////