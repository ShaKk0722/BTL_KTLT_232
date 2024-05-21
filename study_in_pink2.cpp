#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

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
SherlockBag::SherlockBag(Sherlock *sherlock)
{
    this->obj = sherlock;
}
Node *SherlockBag::getHead()
{
    return this->head;
}
bool SherlockBag::insert(BaseItem *item)
{
    // ...
}
BaseItem *SherlockBag::get()
{
    // ...
}

BaseItem *SherlockBag::get(ItemType itemType)
{
    // ...
}

string SherlockBag::str() const
{
    // ...
}

Node *SherlockBag::remove(ItemType itemType)
{
    // ...
}

// BaseBag - WatsonBag
SherlockBag::SherlockBag(Sherlock *sherlock)
{
    // ...
}

Node *SherlockBag::getHead()
{
    // ...
}

bool SherlockBag::insert(BaseItem *item)
{
    // ...
}

BaseItem *SherlockBag::get()
{
    // ...
}

BaseItem *SherlockBag::get(ItemType itemType)
{
    // ...
}

string SherlockBag::str() const
{
    // ...
}

Node *SherlockBag::remove(ItemType itemType)
{
    // ...
}
// Configuration

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////