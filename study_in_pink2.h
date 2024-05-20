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

class Criminal;
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
    MAGIC_BOOK,
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
    virtual ~MapElement();
    virtual ElementType getType() const;
    virtual int getReqExp() const = 0;
}; // abstract class

class Path : public MapElement
{
public:
    Path();
    //~Path();
    ElementType getType() const
    {
        return PATH;
    }
    int getReqExp() const
    {
        return -1;
    }
};

class Wall : public MapElement
{
public:
    Wall();
    //~Wall();
    ElementType getType() const
    {
        return WALL;
    }
    int getReqExp() const
    {
        return -1;
    }
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    FakeWall(){}; // default constructor
    FakeWall(int req_exp);
    //~FakeWall();
    ElementType getType() const
    {
        return FAKE_WALL;
    }
    int getReqExp() const override
    {
        return this->req_exp;
    }
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    int getNumRows() const
    {
        return this->num_rows;
    }
    int getNumCols() const
    {
        return this->num_cols;
    }
    // bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position
{
private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string &str_pos);

    bool operator!=(const Position &other) const
    {
        return r != other.r || c != other.c;
    }

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(){}; // default constructor
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const
    {
        return this->pos;
    }
    virtual void move() = 0;
    virtual string str() const = 0;
    int getIndex() const
    {
        return index;
    }
    string getName() const
    {
        return name;
    }
}; // abstract class

class Character : public MovingObject
{
protected:
    int hp;
    int exp;

public:
    Character(){}; // default constructor
    Character(int index, const Position &pos, Map *map, string name) : MovingObject(index, pos, map, name)
    {
        this->hp = 0;
        this->exp = 0;
    };
    int getHP() const
    {
        return hp;
    }
    int getEXP() const
    {
        return exp;
    }
    void setHP(int hp)
    {
        this->hp = hp;
    }
    void setEXP(int exp)
    {
        this->exp = exp;
    }
};
class Sherlock : public Character
{
private:
    // TODO
    string moving_rule;
    SherlockBag *bag;
    // int next_char_ofrule;
    // static const int start = 0;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    string str() const override;
    void move() override;
    Position getNextPosition() override;
    SherlockBag *getBag()
    {
        return this->bag;
    }
    bool meetC(RobotC *robotC);
    bool meetS(RobotS *robotS);
    bool meetW(RobotW *robotW);
    bool meetSW(RobotSW *robotSW);
    bool meetWatson(Watson *watson);
    bool meetCrim(Criminal *criminal);
};

class Watson : public Character
{
private:
    // TODO
    string moving_rule;
    WatsonBag *bag;
    // int next_char_ofrule;
    // static const int start = 0;
public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    string str() const override;
    void move() override;
    Position getNextPosition() override;
    WatsonBag *getBag()
    {
        return this->bag;
    }
    bool meetC(RobotC *robotC);
    bool meetS(RobotS *robotS);
    bool meetW(RobotW *robotW);
    bool meetSW(RobotSW *robotSW);
    bool meetSherlock(Sherlock *sherlock);
    bool meetCrim(Criminal *criminal);
};

class Criminal : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    Position prev_pos;
    // static int step_count;
    int step_count;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    // getNextPosition
    // move
    // str
    // ...
    string str() const override;
    void move() override;
    Position getNextPosition() override;
    Position getPrevPosition() const
    {
        return this->prev_pos;
    }
    int getStepCount() const
    {
        return this->step_count; // check mod 3
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
    MovingObject *get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
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

    void loadConfig(const string &str)
    {
        int find_equal = str.find('=');
        string name = str.substr(0, find_equal);
        string value = str.substr(find_equal + 1);
        if (name == "MAP_NUM_ROWS")
        {
            map_num_rows = stoi(value);
        }
        else if (name == "MAP_NUM_COLS")
        {
            map_num_cols = stoi(value);
        }
        else if (name == "MAX_NUM_MOVING_OBJECTS")
        {
            max_num_moving_objects = stoi(value);
        }
        else if (name == "ARRAY_WALLS")
        {
            // Array(value, arr_walls, num_walls);
        }
        else if (name == "ARRAY_FAKE_WALLS")
        {
            // Array(value, arr_fake_walls, num_fake_walls);
        }
        else if (name == "SHERLOCK_MOVING_RULE")
        {
            sherlock_moving_rule = value;
        }
        else if (name == "SHERLOCK_INIT_POS")
        {
            sherlock_init_pos = Position(value);
        }
        else if (name == "SHERLOCK_INIT_HP")
        {
            sherlock_init_hp = stoi(value);
        }
        else if (name == "SHERLOCK_INIT_EXP")
        {
            sherlock_init_exp = stoi(value);
        }
        else if (name == "WATSON_MOVING_RULE")
        {
            watson_moving_rule = value;
        }
        else if (name == "WATSON_INIT_POS")
        {
            watson_init_pos = Position(value);
        }
        else if (name == "WATSON_INIT_HP")
        {
            watson_init_hp = stoi(value);
        }
        else if (name == "WATSON_INIT_EXP")
        {
            watson_init_exp = stoi(value);
        }
        else if (name == "CRIMINAL_INIT_POS")
        {
            criminal_init_pos = Position(value);
        }
        else if (name == "NUM_STEPS")
        {
            num_steps = stoi(value);
        }
        else
        {
            cout << "Invalid name: " << name << endl;
        }
    }

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
protected:
    RobotType robottype;
    BaseItem *item;

public:
    Robot(){};
    Robot(RobotType robotType, BaseItem item, int index, const Position &pos, Map *map, string name);

    bool addtoArrayMovingObject(ArrayMovingObject &arr_moving)
    {
        if (!arr_moving.isFull())
        {
            arr_moving.add(this);
            return true;
        }
        return false;
    }
    int calculateDistance(Position other1, Position other2)
    {
        return abs(other1.getCol() - other2.getCol()) + abs(other1.getRow() - other2.getRow());
    }
    virtual RobotType getRobotType() const
    {
        return this->robottype;
    }
    BaseItem *getItem() const
    {
        return this->item;
    }
};

class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    RobotC(){};
    RobotC(int index, const Position &pos, Map *map, Criminal *criminal);
    void move();
    string str();
    Position getNextPosition();
    RobotType getRobotType() const
    {
        return C;
    }
};
class RobotS : public Robot
{
private:
    Sherlock *sherlock;
    Criminal *criminal;

public:
    RobotS(){};
    RobotS(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    void move();
    string str();
    Position getNextPosition();
    RobotType getRobotType() const
    {
        return S;
    }
};
class RobotW : public Robot
{
private:
    Watson *watson;
    Criminal *criminal;

public:
    RobotW(){};
    RobotW(int index, const Position &pos, Map *map, Criminal *criminal, Watson *watson);
    void move();
    string str();
    Position getNextPosition();
    RobotType getRobotType() const
    {
        return W;
    }
};
class RobotSW : public Robot
{
private:
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

public:
    RobotSW(){};
    RobotSW(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    void move();
    string str();
    Position getNextPosition();
    RobotType getRobotType() const
    {
        return SW;
    }
};

class BaseItem
{
private:
    ItemType type;

public:
    BaseItem(ItemType type) : type(type){};
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
};
class MagicBook : public BaseItem
{
public:
    MagicBook() : BaseItem(MAGIC_BOOK){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
};

class EnergyDrink : public BaseItem
{
public:
    EnergyDrink() : BaseItem(ENERGY_DRINK){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
};

class FirstAid : public BaseItem
{
public:
    FirstAid() : BaseItem(FIRST_AID){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
};

class ExcemptionCard : public BaseItem
{
public:
    ExcemptionCard() : BaseItem(EXCEMPTION_CARD){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
};

class PassingCard : public BaseItem
{
private:
    string chal;

public:
    PassingCard(string challenge) : chal(challenge), BaseItem(PASSING_CARD){};
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
};

class BaseBag
{
private:
    Character *obj;

public:
    struct Node
    {
        BaseItem *item;
        Node *next;

        Node(BaseItem *item) : item(item), next(nullptr){};
        Node(BaseItem *item, Node *next) : item(item), next(next){};
    };

    BaseBag(){};
    BaseBag(Character *obj)
    {
        this->obj = obj;
    }
    virtual bool insert(BaseItem *item);      // return true if insert successfully
    virtual BaseItem *get();                  // return the item as described above , if not found, return NULL
    virtual BaseItem *get(ItemType itemType); // return the item as described above , if not found , return NULL
    virtual string str() const;
};

class SherlockBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 13;
    int count;

public:
    SherlockBag(){};
    SherlockBag(Sherlock *sherlock);
    Node *getHead()
    {
        return this->head;
    }
    bool insert(BaseItem *item) override;
    BaseItem *get() override;
    BaseItem *get(ItemType itemType) override;
    string str() const override;
    Node *remove(ItemType itemType);
};

class WatsonBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 15;
    int count;

public:
    WatsonBag(){};
    WatsonBag(Watson *watson);
    Node *getHead()
    {
        return head;
    }
    bool insert(BaseItem *item) override;
    BaseItem *get() override;
    BaseItem *get(ItemType itemType) override;
    string str() const override;
    int deleteItem(ItemType itemType);
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

public:
    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
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

    void run(bool verbose)
    {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
                arr_mv_objs->get(i)->move();
                if (isStop())
                {
                    printStep(istep);
                    break;
                }
                if (verbose)
                {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
