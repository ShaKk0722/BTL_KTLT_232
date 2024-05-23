/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
// class Configuration;
class Map;
class Character;
class Sherlock;
class Watson;
class Criminal;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

// class ArrayMovingObject;
// class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK = 0,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
protected:
    ElementType type;

public:
    MapElement(){}; // default constructor
    MapElement(ElementType in_type);
    virtual ~MapElement(){};
    ElementType getType() const;
    virtual int getReqExp() const = 0;
}; // abstract class
class Path : public MapElement
{
public:
    Path();
    int getReqExp() const;
};
class Wall : public MapElement
{
public:
    Wall();
    int getReqExp() const;
};
class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    FakeWall();
    FakeWall(int in_req_exp);
    int getReqExp() const override;
};
class Position
{
private:
    int r, c;

public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    bool operator!=(const Position &other) const;
    void operator=(const Position &other);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position other) const;
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    int getNumRows() const;
    int getNumCols() const;
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    virtual int getEXP() const;
    MovingObject(); // default constructor
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    Position getCurrentPosition() const;
    int getIndex() const;
    string getName() const;
    void set_position(Position &pos);

    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const;
    virtual Robot *init_robot(int index);
    virtual BaseItem *getItem() const
    {
        return nullptr;
    }
}; // abstract class
class Character : public MovingObject
{
protected:
    int hp;
    int exp;

public:
    Character(); // default constructor
    Character(int index, const Position &pos, Map *map, int hp, int exp, string name);
    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
};
class Sherlock : public Character
{
private:
    // TODO
    string moving_rule;
    SherlockBag *bag;
    int start = -1;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const override;
    void move() override;
    Position getNextPosition() override;
    SherlockBag *getBag();
    bool meetRobot(MovingObject *robot);
    bool meetWatson(Watson *watson);
    void tradeWatson(Watson *watson);
    bool beatC(MovingObject *robotC);
    void beatW(MovingObject *robotW);
    void beatS(MovingObject *robotS);
    void beatSW(MovingObject *robotSW);
    bool beatRobot(MovingObject *robot);
    ~Sherlock()
    {
    }
};
class Watson : public Character
{
private:
    // TODO
    string moving_rule;
    WatsonBag *bag;
    int start = -1;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const override;
    void move() override;
    Position getNextPosition() override;
    WatsonBag *getBag();
    bool meetRobot(MovingObject *robot);
    void tradeSherlock(Sherlock *sherlock);
    void beatC(MovingObject *robotC);
    void beatW(MovingObject *robotW);
    void beatS(MovingObject *robotS);
    void beatSW(MovingObject *robotSW);
    void beatRobot(MovingObject *robot);
    ~Watson()
    {
    }
};
class Criminal : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    Position prev_pos;
    int step_count;
    bool first;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    string str() const override;
    int get_distance(int x1, int y1, int x2, int y2);
    void move() override;
    Position getNextPosition() override;
    Position getPrevPosition() const;
    int getStepCount() const;
    Map *get_map();
    Robot *init_robot(int index);
    ~Criminal()
    {
    }
};
class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int capacity;
    int count;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    int size() const;
    string str() const;
    MovingObject *get(int index) const;
    bool remove(MovingObject *mv_obj);
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    // TODO
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    void loadConfig(const string &str);
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

class BaseItem
{
private:
    ItemType type;

public:
    BaseItem(ItemType type);
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    int getItemType() const;
    virtual string str() = 0;
    virtual string getChallenge()
    {
        return "";
    }
    ~BaseItem(){};
};
class MagicBook : public BaseItem
{
public:
    MagicBook();
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() override;
};
class EnergyDrink : public BaseItem
{
public:
    EnergyDrink();
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() override;
};
class FirstAid : public BaseItem
{
public:
    FirstAid();
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() override;
};
class ExcemptionCard : public BaseItem
{
public:
    ExcemptionCard();
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() override;
};
class PassingCard : public BaseItem
{
private:
    string chal;

public:
    PassingCard(string challenge) : chal(challenge), BaseItem(PASSING_CARD){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() override;
    string getChallenge();
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
protected:
    RobotType robottype;
    BaseItem *item;

public:
    Robot();
    Robot(RobotType robotType, int index, const Position &pos, Map *map, string name);
    bool addtoArrayMovingObject(ArrayMovingObject &arr_moving);
    int calculateDistance(Position other1, Position other2) const;
    BaseItem *getItem() const;
    RobotType getRobotType() const;
    int getDistance(Character *obj) const;
    ~Robot()
    {
    }
};
class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    RobotC();
    RobotC(int index, const Position &pos, Map *map, Criminal *criminal);
    void move();
    string str() const;
    Position getNextPosition();
    ~RobotC()
    {
    }
};
class RobotS : public Robot
{
private:
    Sherlock *sherlock;
    Criminal *criminal;

public:
    RobotS();
    RobotS(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    void move();
    string str() const;
    Position getNextPosition();
    ~RobotS()
    {
    }
};
class RobotW : public Robot
{
private:
    Watson *watson;
    Criminal *criminal;

public:
    RobotW();
    RobotW(int index, const Position &pos, Map *map, Criminal *criminal, Watson *watson);
    void move();
    string str() const;
    Position getNextPosition();
    ~RobotW()
    {
    }
};
class RobotSW : public Robot
{
private:
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

public:
    RobotSW();
    RobotSW(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    void move();
    string str() const;
    Position getNextPosition();
    ~RobotSW()
    {
    }
};

struct Node
{
    BaseItem *item;
    Node *next;

    Node(BaseItem *item) : item(item), next(nullptr){};
    Node(BaseItem *item, Node *next) : item(item), next(next){};
    ~Node()
    {
    }
};

class BaseBag
{
protected:
    Character *obj;

public:
    BaseBag();
    BaseBag(Character *obj);
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();                  // return the item as described above , if not found, return NULL
    virtual BaseItem *get(ItemType itemType); // return the item as described above , if not found , return NULL
    virtual string str() const;
    ~BaseBag()
    {
    }
};

class SherlockBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 13;
    int count;

public:
    SherlockBag();
    SherlockBag(Sherlock *sherlock);
    Node *getHead();
    bool insert(BaseItem *item);
    BaseItem *get(ItemType itemType);
    BaseItem *get();

    string str() const;
    ~SherlockBag()
    {
    }
};

class WatsonBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 15;
    int count;

public:
    WatsonBag();
    WatsonBag(Watson *watson);
    Node *getHead();
    bool insert(BaseItem *item);
    BaseItem *get();
    BaseItem *get(ItemType itemType);
    string str() const;
    ~WatsonBag()
    {
    }
};
class StudyPinkProgram
{
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;
    bool isWin = false;

public:
    StudyPinkProgram(const string &config_file_path)
    {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
        arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
        this->arr_mv_objs->add(criminal);
        this->arr_mv_objs->add(sherlock);
        this->arr_mv_objs->add(watson);
    }

    bool isStop() const
    {
        if (this->sherlock->getHP() == 0 || this->watson->getHP() == 0 || isWin == true)
            return true;
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
            return true;
        if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
            return true;
        return false;
    }

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()) || isWin == true)
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    ~StudyPinkProgram()
    {
        delete config;
        delete sherlock;
        delete watson;
        delete criminal;
        delete map;
        delete arr_mv_objs;
    }
    void run(bool verbose)
    {
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
            for (int i = 0; i < arr_mv_objs->size(); i++)
            {
                arr_mv_objs->get(i)->move();
                if (arr_mv_objs->get(i)->getName() == "Sherlock")
                {
                    if (isStop())
                    {
                        break;
                    }
                    if (sherlock->meetWatson(watson))
                    {
                        sherlock->tradeWatson(watson);
                        watson->tradeSherlock(sherlock);
                    }
                    for (int j = 3; j < arr_mv_objs->size(); j++)
                    {

                        if (sherlock->meetRobot(arr_mv_objs->get(j)))
                        {
                            isWin = (sherlock->beatRobot(arr_mv_objs->get(j)));
                            if (isWin)
                            {
                                Position pos = criminal->getCurrentPosition();
                                sherlock->set_position(pos);
                                break;
                            }
                        }
                    }
                    if (isWin)
                    {
                        break;
                    }
                }
                else if (arr_mv_objs->get(i)->getName() == "Watson")
                {
                    if (isStop())
                    {
                        break;
                    }
                    if (sherlock->meetWatson(watson))
                    {
                        sherlock->tradeWatson(watson);
                        watson->tradeSherlock(sherlock);
                    }
                    for (int i = 3; i < arr_mv_objs->size(); i++)
                    {
                        if (watson->meetRobot(arr_mv_objs->get(i)))
                        {
                            watson->beatRobot(arr_mv_objs->get(i));
                        }
                    }
                }
                else if (arr_mv_objs->get(i)->getName() == "Criminal")
                {
                    if (isStop())
                    {
                        break;
                    }
                    int index = arr_mv_objs->size();
                    Robot *temp = arr_mv_objs->get(i)->init_robot(index);
                    if (temp != nullptr)
                    {
                        if (arr_mv_objs->add(temp) == false)
                        {
                            delete temp;
                        }
                    }
                }
                else
                {
                    if (sherlock->meetRobot(arr_mv_objs->get(i)))
                    {
                        isWin = (sherlock->beatRobot(arr_mv_objs->get(i)));
                        if (isWin)
                        {
                            Position pos = criminal->getCurrentPosition();
                            sherlock->set_position(pos);
                            break;
                        }
                    }
                    if (watson->meetRobot(arr_mv_objs->get(i)))
                    {
                        watson->beatRobot(arr_mv_objs->get(i));
                    }
                }
                
            }
            if (verbose)
            {
                printStep(istep);
            }
            if (isStop())
            {
                break;
            }
        }
        printResult();
    }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
