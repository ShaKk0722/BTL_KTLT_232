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
    MapElement(ElementType in_type)
    {
        this->type = in_type;
    }
    virtual ~MapElement(){};
    ElementType getType() const
    {
        return this->type;
    }
    virtual int getReqExp() const = 0;
}; // abstract class
class Path : public MapElement
{
public:
    Path()
    {
        this->type = PATH;
    }
    //~Path();
    int getReqExp() const
    {
        return -1;
    }
};
class Wall : public MapElement
{
public:
    Wall()
    {
        this->type = WALL;
    }
    //~Wall();
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
    FakeWall()
    {
        this->type = FAKE_WALL;
        req_exp = 0;
    }
    FakeWall(int in_req_exp)
    {
        this->type = FAKE_WALL;
        req_exp = in_req_exp;
    }
    //~FakeWall();
    int getReqExp() const override
    {
        return this->req_exp;
    }
};
class Position
{
private:
    int r, c;

public:
    static const Position npos;
    Position(int r = 0, int c = 0)
    {
        this->r = r;
        this->c = c;
    }
    // str_pos = (12,342)
    Position(const string &str_pos)
    {
        int comma = str_pos.find(',');
        int end = str_pos.find(')');
        string r_value = str_pos.substr(1, comma - 1);
        string c_value = str_pos.substr(comma + 1, end - comma - 1);
        this->r = stoi(r_value);
        this->c = stoi(c_value);
    }

    bool operator!=(const Position &other) const
    {
        return r != other.r || c != other.c;
    }
    void operator=(const Position &other)
    {
        this->r = other.r;
        this->c = other.c;
    }
    int getRow() const
    {
        return this->r;
    }
    int getCol() const
    {
        return this->c;
    }
    void setRow(int r)
    {
        this->r = r;
    }
    void setCol(int c)
    {
        this->c = c;
    }
    string str() const
    {
        string ans = "(" + to_string(r) + "," + to_string(c) + ")";
        return ans;
    }

    bool isEqual(int in_r, int in_c) const
    {
        return (this->r == in_r && this->c == in_c);
    }
};
class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    virtual int getEXP() const
    {
        return 0;
    }
    MovingObject(){}; // default constructor
    MovingObject(int index, const Position pos, Map *map, const string &name = "")
    {
        this->index = index;
        this->pos = pos;
        this->map = map;
        this->name = name;
    }

    Position getCurrentPosition() const
    {
        return this->pos;
    }
    int getIndex() const
    {
        return index;
    }
    string getName() const
    {
        return name;
    }
    void set_position(Position &pos)
    {
        this->pos.setRow(pos.getRow());
        this->pos.setCol(pos.getCol());
    }

    virtual ~MovingObject()
    {
    }
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const
    {
        return "empty";
    }

}; // abstract class
class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    {
        this->num_rows = num_rows;
        this->num_cols = num_cols;

        map = new MapElement **[num_rows];
        for (int i = 0; i < num_rows; i++)
        {
            map[i] = new MapElement *[num_cols];
            for (int j = 0; j < num_cols; j++)
            {
                map[i][j] = NULL;
            }
        }

        for (int i = 0; i < num_walls; i++)
        {
            Position pos = array_walls[i];
            map[pos.getRow()][pos.getCol()] = new Wall;
        }
        for (int i = 0; i < num_fake_walls; i++)
        {
            Position pos = array_fake_walls[i];
            int in_req_exp = (pos.getRow() * 257 + pos.getCol() * 139 + 89) % 900 + 1;
            map[pos.getRow()][pos.getCol()] = new FakeWall(in_req_exp);
        }
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                if (map[i][j] == NULL)
                {
                    map[i][j] = new Path;
                }
            }
        }
    }
    ~Map()
    {
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                delete map[i][j];
            }
            delete[] map[i];
        }
        delete[] map;
    }
    int getNumRows() const
    {
        return this->num_rows;
    }
    int getNumCols() const
    {
        return this->num_cols;
    }
    bool isValid(const Position &pos, MovingObject *mv_obj) const
    {
        if (pos.getRow() <= -1 || pos.getRow() >= this->num_rows)
            return false;
        if (pos.getCol() <= -1 || pos.getCol() >= this->num_cols)
            return false;
        if (map[pos.getRow()][pos.getCol()]->getType() == PATH)
            return true;
        if (map[pos.getRow()][pos.getCol()]->getType() == WALL)
            return false;
        if (mv_obj->getName() == "RobotC" || mv_obj->getName() == "RobotS" || mv_obj->getName() == "RobotW" || mv_obj->getName() == "RobotSW")
            return true;
        if (mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal")
            return true;
        if (mv_obj->getEXP() >= map[pos.getRow()][pos.getCol()]->getReqExp())
            return true;
        return false;
    }
};

class Character : public MovingObject
{
protected:
    int hp;
    int exp;

public:
    Character(){}; // default constructor
    Character(int index, const Position &pos, Map *map, int hp, int exp, string name) : MovingObject(index, pos, map, name)
    {
        setHP(hp);
        setEXP(exp);
    }
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
        this->hp = (hp > 500) ? 500 : hp;
    }
    void setEXP(int exp)
    {

        this->exp = (exp > 900) ? 900 : exp;
    }
};
class Sherlock : public Character
{
private:
    // TODO
    string moving_rule;
    SherlockBag *bag;
    int start = -1;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Sherlock")
    {
        this->moving_rule = moving_rule;
    }
    string str() const override
    {
        string ans = "Sherlock[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
        return ans;
    }
    void move() override
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    Position getNextPosition() override
    {
        int length = moving_rule.length();
        start = (start + 1) % length;
        if (moving_rule[start] == 'L')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setCol(next_cur.getCol() - 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else if (moving_rule[start] == 'R')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setCol(next_cur.getCol() + 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else if (moving_rule[start] == 'U')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setRow(next_cur.getRow() - 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else
        {
            Position next_cur = getCurrentPosition();
            next_cur.setRow(next_cur.getRow() + 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
    }
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
    int start = -1;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Watson")
    {
        this->moving_rule = moving_rule;
    }

    string str() const override
    {
        string ans = "Watson[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
        return ans;
    }
    void move() override
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    Position getNextPosition() override
    {
        int length = moving_rule.length();
        start = (start + 1) % length;
        if (moving_rule[start] == 'L')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setCol(next_cur.getCol() - 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else if (moving_rule[start] == 'R')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setCol(next_cur.getCol() + 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else if (moving_rule[start] == 'U')
        {
            Position next_cur = getCurrentPosition();
            next_cur.setRow(next_cur.getRow() - 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
        else
        {
            Position next_cur = getCurrentPosition();
            next_cur.setRow(next_cur.getRow() + 1);
            return (map->isValid(next_cur, this) ? next_cur : next_cur.npos);
        }
    }
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
    int step_count;
    

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, 0, 0, "Criminal")
    {
        this->sherlock = sherlock;
        this->watson = watson;
    }
    string str() const override
    {
        string ans = "Criminal[index=" + to_string(this->index) + ";pos=" + this->pos.str() + "]";
        return ans;
    }
    int get_distance(int x1, int y1, int x2, int y2)
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }
    void move() override
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            prev_pos.setRow(this->getCurrentPosition().getRow());
            prev_pos.setCol(this->getCurrentPosition().getCol());
            this->set_position(next);
            step_count++;
        }
    }
    Position getNextPosition() override
    {
        Position pri_cur = this->getCurrentPosition();

        Position pri_up = pri_cur;
        pri_up.setRow(pri_cur.getRow() - 1);

        Position pri_down = pri_cur;
        pri_down.setRow(pri_cur.getRow() + 1);

        Position pri_left = pri_cur;
        pri_left.setCol(pri_cur.getCol() - 1);

        Position pri_right = pri_cur;
        pri_right.setCol(pri_cur.getCol() + 1);

        Position sher_cur = sherlock->getCurrentPosition();
        Position wat_cur = watson->getCurrentPosition();
        int l = 0, r = 0, u = 0, d = 0;
        if (map->isValid(pri_up, this))
        {
            int dis_sher = get_distance(pri_up.getRow(), pri_up.getCol(), sher_cur.getRow(), sher_cur.getCol());
            int dis_wat = get_distance(pri_up.getRow(), pri_up.getCol(), wat_cur.getRow(), wat_cur.getCol());
            u = dis_sher + dis_wat;
        }
        if (map->isValid(pri_down, this))
        {
            int dis_sher = get_distance(pri_down.getRow(), pri_down.getCol(), sher_cur.getRow(), sher_cur.getCol());
            int dis_wat = get_distance(pri_down.getRow(), pri_down.getCol(), wat_cur.getRow(), wat_cur.getCol());
            d = dis_sher + dis_wat;
        }
        if (map->isValid(pri_left, this))
        {
            int dis_sher = get_distance(pri_left.getRow(), pri_left.getCol(), sher_cur.getRow(), sher_cur.getCol());
            int dis_wat = get_distance(pri_left.getRow(), pri_left.getCol(), wat_cur.getRow(), wat_cur.getCol());
            l = dis_sher + dis_wat;
        }
        if (map->isValid(pri_right, this))
        {
            int dis_sher = get_distance(pri_right.getRow(), pri_right.getCol(), sher_cur.getRow(), sher_cur.getCol());
            int dis_wat = get_distance(pri_right.getRow(), pri_right.getCol(), wat_cur.getRow(), wat_cur.getCol());
            r = dis_sher + dis_wat;
        }
        if (u == 0 && d == 0 && l == 0 && r == 0)
            return pri_cur.npos;
        int ans = u;
        char ch = 'u';
        if (ans < l)
        {
            ans = l;
            ch = 'l';
        }
        if (ans < d)
        {
            ans = d;
            ch = 'd';
        }
        if (ans < r)
        {
            ans = r;
            ch = 'r';
        }
        if (ch == 'u')
            return pri_up;
        else if (ch == 'l')
            return pri_left;
        else if (ch == 'd')
            return pri_down;
        else
            return pri_right;
    }
    Position getPrevPosition() const
    {
        return this->prev_pos;
    }
    int getStepCount() const
    {
        return this->step_count; // check mod 3
    }
    Map *get_map()
    {
        return this->map;
    }
};
class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int capacity;
    int count;

public:
    ArrayMovingObject(int capacity)
    {
        this->capacity = capacity;
        this->count = 0;
        this->arr_mv_objs = new MovingObject *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            arr_mv_objs[i] = NULL;
        }
    }
    ~ArrayMovingObject()
    {
        delete[] arr_mv_objs;
    }
    bool isFull() const
    {
        return (count == capacity);
    }
    bool add(MovingObject *mv_obj)
    {
        if (this->isFull())
            return false;
        this->arr_mv_objs[count] = mv_obj;
        this->count++;
        return true;
    }
    MovingObject *get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const
    {
        string ans = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity);
        for (int i = 0; i < count; i++)
        {
            ans = ans + ";" + this->arr_mv_objs[i]->str();
        }
        ans = ans + "]";
        return ans;
    }
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
    void Array(string value, Position *&arr, int &num)
    {
        if (arr != nullptr)
        {
            arr = nullptr;
        }
        num = 0;
        for (char ch : value)
        {
            if (ch == '(')
                num++;
        }
        arr = new Position[num];
        int i = 0;
        int start = value.find('(');
        while (start != -1)
        {
            int mid = value.find(',');
            int end = value.find(')');
            int r = stoi(value.substr(start + 1, mid - start - 1));
            int c = stoi(value.substr(mid + 1, end - mid - 1));
            arr[i] = Position(r, c);
            i++;
            value = value.substr(end + 1);
            start = value.find('(');
        }
    }
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
            Array(value, arr_walls, num_walls);
        }
        else if (name == "ARRAY_FAKE_WALLS")
        {
            Array(value, arr_fake_walls, num_fake_walls);
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
    Configuration(const string &filepath)
    {
        ifstream file(filepath);
        string line;
        while (getline(file, line))
        {
            loadConfig(line);
        }
        file.close();
    }
    ~Configuration()
    {
        // delete[] arr_fake_walls;
        // delete[] arr_walls;
    }
    string str() const
    {
        string ans = "Configuration[\n";
        ans = ans + "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
        ans = ans + "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
        ans = ans + "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
        ans = ans + "NUM_WALLS=" + to_string(num_walls) + "\n";
        ans = ans + "ARRAY_WALLS=[";
        if (num_walls > 0)
            ans = ans + arr_walls->str();
        for (int i = 1; i < num_walls; i++)
        {
            ans = ans + ";" + arr_walls[i].str();
        }
        ans = ans + "]\n";
        ans = ans + "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
        ans = ans + "ARRAY_FAKE_WALLS=[";
        if (num_fake_walls > 0)
            ans = ans + arr_fake_walls->str();
        for (int i = 1; i < num_fake_walls; i++)
        {
            ans = ans + ";" + arr_fake_walls[i].str();
        }
        ans = ans + "]\n";
        ans = ans + "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
        ans = ans + "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
        ans = ans + "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
        ans = ans + "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
        ans = ans + "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
        ans = ans + "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
        ans = ans + "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
        ans = ans + "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
        ans = ans + "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
        ans = ans + "NUM_STEPS=" + to_string(num_steps) + "\n";
        ans = ans + "]";
        return ans;
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
    int getItemType() const
    {
        return type;
    }
    virtual string str() = 0;
};
class MagicBook : public BaseItem
{
public:
    MagicBook() : BaseItem(MAGIC_BOOK){};
    bool canUse(Character *obj, Robot *robot) override
    {
        return obj->getEXP() <= 350;
    }
    void use(Character *obj, Robot *robot) override
    {
        if (canUse(obj, robot))
        {
            obj->setEXP(obj->getEXP() * 125 / 100);
        }
    }
    string str() override
    {
        return "MagicBook";
    }
};
class EnergyDrink : public BaseItem
{
public:
    EnergyDrink() : BaseItem(ENERGY_DRINK){};
    bool canUse(Character *obj, Robot *robot) override
    {
        return obj->getHP() <= 100;
    }
    void use(Character *obj, Robot *robot) override
    {
        if (canUse(obj, robot))
        {
            obj->setHP(obj->getHP() * 120 / 100);
        }
    }
    string str() override
    {
        return "EnergyDrink";
    }
};
class FirstAid : public BaseItem
{
public:
    FirstAid() : BaseItem(FIRST_AID){};
    bool canUse(Character *obj, Robot *robot) override
    {
        return (obj->getEXP() <= 350 || obj->getHP() <= 100);
    }
    void use(Character *obj, Robot *robot) override
    {
        if (canUse(obj, robot))
        {
            obj->setHP(obj->getHP() * 150 / 100);
        }
    }
    string str() override
    {
        return "FirstAid";
    }
};
class ExcemptionCard : public BaseItem
{
public:
    ExcemptionCard() : BaseItem(EXCEMPTION_CARD){};
    bool canUse(Character *obj, Robot *robot) override
    {
        return (obj->getName() == "Sherlock" && obj->getHP() % 2 != 0);
    }
    void use(Character *obj, Robot *robot) override
    {
    }
    string str() override
    {
        return "ExcemptionCard";
    }
};
class PassingCard : public BaseItem
{
private:
    string chal;

public:
    PassingCard(string challenge) : chal(challenge), BaseItem(PASSING_CARD){};
    bool canUse(Character* obj, Robot* robot) override;
    void use(Character* obj, Robot* robot) override;
    string str() override
    {
        return "PassingCard";
    }
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
protected:
    RobotType robottype;
    BaseItem *item;

public:
    Robot(){};
    Robot(RobotType robotType, int index, const Position &pos, Map *map, string name) : MovingObject(index, pos, map, name)
    {
        this->robottype = robotType;
        int p = pos.getRow() * pos.getCol();
        int new_p = 0;
        while (p >= 10)
        {
            int temp = p;
            while (temp > 0)
            {
                new_p += temp % 10;
                temp /= 10;
            }
            p = new_p;
            new_p = 0;
        }
        if (p == 0 || p == 1)
            this->item = new MagicBook;
        else if (p == 2 || p == 3)
            this->item = new EnergyDrink;
        else if (p == 4 || p == 5)
            this->item = new FirstAid;
        else if (p == 6 || p == 7)
            this->item = new ExcemptionCard;
        else
        {
            int t = (pos.getRow() * 11 + pos.getCol()) % 4;
            if (t == 0)
                this->item = new PassingCard("RobotS");
            else if (t == 1)
                this->item = new PassingCard("RobotC");
            else if (t == 2)
                this->item = new PassingCard("RobotSW");
            else
                this->item = new PassingCard("all");
        }
    }

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
    BaseItem *getItem() const
    {
        return this->item;
    }
    RobotType getRobotType() const
    {
        return this->robottype;
    }
};

class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    RobotC(){};
    RobotC(int index, const Position &pos, Map *map, Criminal *criminal) : Robot(C, index, pos, map, "RobotC")
    {
        this->criminal = criminal;
    }
    void move()
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    string str()
    {
        string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=C" + ";dist=]";
        return ans;
    }
    Position getNextPosition()
    {
        Position next_cur = criminal->getPrevPosition();
        return (criminal->get_map()->isValid(next_cur, this) ? next_cur : next_cur.npos);
    }
};
class RobotS : public Robot
{
private:
    Sherlock *sherlock;
    Criminal *criminal;

public:
    RobotS(){};
    RobotS(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock) : Robot(S, index, pos, map, "RobotS")
    {
        this->criminal = criminal;
        this->sherlock = sherlock;
    }
    void move()
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    string str()
    {
        string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=S" + ";dist=";
        int distance = this->calculateDistance(this->getCurrentPosition(), sherlock->getCurrentPosition());
        ans = ans + to_string(distance) + "]";
        return ans;
    }
    Position getNextPosition()
    {
        Position pri_cur = this->getCurrentPosition();

        Position pri_up = pri_cur;
        pri_up.setRow(pri_cur.getRow() - 1);

        Position pri_down = pri_cur;
        pri_down.setRow(pri_cur.getRow() + 1);

        Position pri_left = pri_cur;
        pri_left.setCol(pri_cur.getCol() - 1);

        Position pri_right = pri_cur;
        pri_right.setCol(pri_cur.getCol() + 1);

        Position sher_cur = sherlock->getCurrentPosition();
        int l = 0, r = 0, u = 0, d = 0;
        if (map->isValid(pri_up, this))
        {
            int dis_sher = calculateDistance(pri_up, sher_cur);
            u = dis_sher;
        }
        if (map->isValid(pri_down, this))
        {
            int dis_sher = calculateDistance(pri_down, sher_cur);
            d = dis_sher;
        }
        if (map->isValid(pri_left, this))
        {
            int dis_sher = calculateDistance(pri_left, sher_cur);
            l = dis_sher;
        }
        if (map->isValid(pri_right, this))
        {
            int dis_sher = calculateDistance(pri_right, sher_cur);
            r = dis_sher;
        }
        if (u == 0 && d == 0 && l == 0 && r == 0)
            return pri_cur.npos;
        int ans = u;
        char ch = 'u';
        if (ans < r)
        {
            ans = r;
            ch = 'r';
        }
        if (ans < d)
        {
            ans = d;
            ch = 'd';
        }
        if (ans < l)
        {
            ans = l;
            ch = 'l';
        }
        if (ch == 'u')
            return pri_up;
        else if (ch == 'r')
            return pri_right;
        else if (ch == 'd')
            return pri_down;
        else
            return pri_left;
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
    void move()
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    string str()
    {
        string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=W" + ";dist=";
        int distance = this->calculateDistance(this->getCurrentPosition(), watson->getCurrentPosition());
        ans = ans + to_string(distance) + "]";
        return ans;
    }
    Position getNextPosition()
    {
        Position pri_cur = this->getCurrentPosition();

        Position pri_up = pri_cur;
        pri_up.setRow(pri_cur.getRow() - 1);

        Position pri_down = pri_cur;
        pri_down.setRow(pri_cur.getRow() + 1);

        Position pri_left = pri_cur;
        pri_left.setCol(pri_cur.getCol() - 1);

        Position pri_right = pri_cur;
        pri_right.setCol(pri_cur.getCol() + 1);

        Position wat_cur = watson->getCurrentPosition();
        int l = 0, r = 0, u = 0, d = 0;
        if (map->isValid(pri_up, this))
        {
            int dis_wat = calculateDistance(pri_up, wat_cur);
            u = dis_wat;
        }
        if (map->isValid(pri_down, this))
        {
            int dis_wat = calculateDistance(pri_down, wat_cur);
            d = dis_wat;
        }
        if (map->isValid(pri_left, this))
        {
            int dis_wat = calculateDistance(pri_left, wat_cur);
            l = dis_wat;
        }
        if (map->isValid(pri_right, this))
        {
            int dis_wat = calculateDistance(pri_right, wat_cur);
            r = dis_wat;
        }
        if (u == 0 && d == 0 && l == 0 && r == 0)
            return pri_cur.npos;
        int ans = u;
        char ch = 'u';
        if (ans < r)
        {
            ans = r;
            ch = 'r';
        }
        if (ans < d)
        {
            ans = d;
            ch = 'd';
        }
        if (ans < l)
        {
            ans = l;
            ch = 'l';
        }
        if (ch == 'u')
            return pri_up;
        else if (ch == 'r')
            return pri_right;
        else if (ch == 'd')
            return pri_down;
        else
            return pri_left;
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
    void move()
    {
        Position next = getNextPosition();
        if (next != next.npos)
        {
            this->set_position(next);
        }
    }
    string str()
    {
        string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=SW" + ";dist=";
        int sher = this->calculateDistance(this->getCurrentPosition(), sherlock->getCurrentPosition());
        int wat = this->calculateDistance(this->getCurrentPosition(), watson->getCurrentPosition());
        int distance = sher + wat;
        ans = ans + to_string(distance) + "]";
        return ans;
    }
    Position getNextPosition()
    {
        Position pri_cur = this->getCurrentPosition();

        Position pri_up_up = pri_cur;
        pri_up_up.setRow(pri_cur.getRow() - 1);
        Position check_up = pri_up_up;
        pri_up_up.setRow(pri_cur.getRow() - 1);

        Position pri_down_down = pri_cur;
        pri_down_down.setRow(pri_cur.getRow() + 1);
        Position check_down = pri_down_down;
        pri_down_down.setRow(pri_cur.getRow() + 1);

        Position pri_left_left = pri_cur;
        pri_left_left.setCol(pri_cur.getCol() - 1);
        Position check_left = pri_left_left;
        pri_left_left.setCol(pri_cur.getCol() - 1);

        Position pri_right_right = pri_cur;
        pri_right_right.setCol(pri_cur.getCol() + 1);
        Position check_right = pri_right_right;
        pri_right_right.setCol(pri_cur.getCol() + 1);

        Position pri_up_right = pri_cur;
        pri_up_right.setRow(pri_cur.getRow() - 1);
        pri_up_right.setCol(pri_cur.getCol() + 1);

        Position pri_down_right = pri_cur;
        pri_down_right.setRow(pri_cur.getRow() + 1);
        pri_down_right.setCol(pri_cur.getCol() + 1);

        Position pri_down_left = pri_cur;
        pri_down_left.setRow(pri_cur.getRow() + 1);
        pri_down_left.setCol(pri_cur.getCol() - 1);

        Position pri_up_left = pri_cur;
        pri_up_left.setRow(pri_cur.getRow() - 1);
        pri_up_left.setCol(pri_cur.getCol() - 1);

        Position sher_cur = sherlock->getCurrentPosition();
        Position wat_cur = watson->getCurrentPosition();
        int l = 0, r = 0, u = 0, d = 0;
        int u_r = 0, d_r = 0, d_l = 0, u_l = 0;

        if (map->isValid(pri_up_up, this))
        {
            if (map->isValid(check_up, this))
            {
                int dis_sher = calculateDistance(pri_up_up, sher_cur);
                int dis_wat = calculateDistance(pri_up_up, wat_cur);
                u = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_down_down, this))
        {
            if (map->isValid(check_down, this))
            {
                int dis_sher = calculateDistance(pri_down_down, sher_cur);
                int dis_wat = calculateDistance(pri_down_down, wat_cur);
                d = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_left_left, this))
        {
            if (map->isValid(check_left, this))
            {
                int dis_sher = calculateDistance(pri_left_left, sher_cur);
                int dis_wat = calculateDistance(pri_left_left, wat_cur);
                l = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_right_right, this))
        {
            if (map->isValid(check_right, this))
            {
                int dis_sher = calculateDistance(pri_right_right, sher_cur);
                int dis_wat = calculateDistance(pri_right_right, wat_cur);
                r = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_up_right, this))
        {
            if (map->isValid(check_up, this) || map->isValid(check_right, this))
            {
                int dis_sher = calculateDistance(pri_up_right, sher_cur);
                int dis_wat = calculateDistance(pri_up_right, wat_cur);
                u_r = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_down_right, this))
        {
            if (map->isValid(check_down, this) || map->isValid(check_right, this))
            {
                int dis_sher = calculateDistance(pri_down_right, sher_cur);
                int dis_wat = calculateDistance(pri_down_right, wat_cur);
                d_r = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_down_left, this))
        {
            if (map->isValid(check_down, this) || map->isValid(check_left, this))
            {
                int dis_sher = calculateDistance(pri_down_left, sher_cur);
                int dis_wat = calculateDistance(pri_down_left, wat_cur);
                d_l = dis_wat + dis_sher;
            }
        }
        if (map->isValid(pri_up_left, this))
        {
            if (map->isValid(check_up, this) || map->isValid(check_left, this))
            {
                int dis_sher = calculateDistance(pri_up_left, sher_cur);
                int dis_wat = calculateDistance(pri_up_left, wat_cur);
                u_l = dis_wat + dis_sher;
            }
        }
        if (u == 0 && d == 0 && l == 0 && r == 0 && u_r == 0 && d_r == 0 && d_l == 0 && u_l == 0)
        {
            return pri_cur.npos;
        }
        int ans = u;
        string ch = "u";
        if (ans < u_r)
        {
            ans = u_r;
            ch = "u_r";
        }
        if (ans < r)
        {
            ans = r;
            ch = "r";
        }
        if (ans < d_r)
        {
            ans = d_r;
            ch = "d_r";
        }
        if (ans < d)
        {
            ans = d;
            ch = "d";
        }
        if (ans < d_l)
        {
            ans = d_l;
            ch = "d_l";
        }
        if (ans < l)
        {
            ans = l;
            ch = "l";
        }
        if (ans < u_l)
        {
            ans = u_l;
            ch = "u_l";
        }
        if (ch == "u")
            return pri_up_up;
        else if (ch == "u_r")
            return pri_up_right;
        else if (ch == "r")
            return pri_right_right;
        else if (ch == "d_r")
            return pri_down_right;
        else if (ch == "d")
            return pri_down_down;
        else if (ch == "d_l")
            return pri_down_left;
        else if (ch == "l")
            return pri_left_left;
        else
            return pri_up_left;
    }
};

struct Node
{
    BaseItem *item;
    Node *next;

    Node(BaseItem *item) : item(item), next(nullptr){};
    Node(BaseItem *item, Node *next) : item(item), next(next){};
};

class BaseBag
{
protected:
    Character *obj;

public:
    BaseBag(){};
    BaseBag(Character *obj)
    {
        this->obj = obj;
    }
    virtual bool insert(BaseItem *item)
    {
        return true;
    }
    virtual BaseItem *get()
    {
        return nullptr;
    } // return the item as described above , if not found, return NULL
    virtual BaseItem *get(ItemType itemType)
    {
        return nullptr;
    } // return the item as described above , if not found , return NULL
    virtual string str() const
    {
        return "";
    }
};

class SherlockBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 13;
    int count;

public:
    SherlockBag(){};
    SherlockBag(Sherlock *sherlock)
    {
        this->count = 0;
        this->head = NULL;
        this->obj = sherlock;
    }
    Node *getHead()
    {
        return this->head;
    }
    bool insert(BaseItem *item)
    {
        if (count >= capacity)
            return false;
        Node *temp = new Node(item, head);
        head = temp;
        count++;
        return true;
    }
    BaseItem *get();
    BaseItem *get(ItemType itemType)
    {
        if (head == nullptr)
        {
            return nullptr;
        }
        Node *current = head;
        Node *prev = nullptr;
        while (current != nullptr)
        {
            if (current->item->getItemType() == itemType)
            {
                if (prev == nullptr)
                {
                    head = head->next;
                    BaseItem *temp = current->item;
                    head->item = nullptr;
                    delete current;
                    this->count--;
                    return temp;
                }
                else
                {
                    BaseItem *temp = current->item;
                    current->item = head->item;
                    head->item = nullptr;
                    Node *tempNode = head;
                    head = head->next;
                    delete tempNode;
                    count--;
                    return temp;
                }
            }
            prev = current;
            current = current->next;
        }
        return nullptr;
    }
    string str() const
    {
        string ans = "Bag[count=" + to_string(this->count) + ";";

        for (Node *temp = head; temp != NULL; temp = temp->next)
        {
            ans += temp->item->str() + ",";
            temp = temp->next;
        }
        return ans + "]";
    }
};

class WatsonBag : public BaseBag
{
private:
    Node *head;
    int const capacity = 15;
    int count;

public:
    WatsonBag(){};
    WatsonBag(Watson *watson)
    {
        this->count = 0;
        this->head = NULL;
        this->obj = watson;
    }
    Node *getHead()
    {
        return head;
    }
    bool insert(BaseItem *item)
    {
        if (count >= capacity)
            return false;
        Node *temp = new Node(item, head);
        head = temp;
        count++;
        return true;
    }
    BaseItem *get();
    BaseItem *get(ItemType itemType)
    {
        if (head == nullptr)
        {
            return nullptr;
        }
        Node *current = head;
        Node *prev = nullptr;
        while (current != nullptr)
        {
            if (current->item->getItemType() == itemType)
            {
                if (prev == nullptr)
                {
                    head = head->next;
                    BaseItem *temp = current->item;
                    head->item = nullptr;
                    delete current;
                    this->count--;
                    return temp;
                }
                else
                {
                    BaseItem *temp = current->item;
                    current->item = head->item;
                    head->item = nullptr;
                    Node *tempNode = head;
                    head = head->next;
                    delete tempNode;
                    count--;
                    return temp;
                }
            }
            prev = current;
            current = current->next;
        }
        return nullptr;
    }
    string str() const
    {
        string ans = "Bag[count=" + to_string(this->count) + ";";

        for (Node *temp = head; temp != NULL; temp = temp->next)
        {
            ans += temp->item->str() + ",";
            temp = temp->next;
        }
        return ans + "]";
    }
    // int deleteItem(ItemType itemType);
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
    StudyPinkProgram(const string &config_file_path)
    {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
        this->arr_mv_objs->add(criminal);
        this->arr_mv_objs->add(sherlock);
        this->arr_mv_objs->add(watson);
    }

    bool isStop() const
    {
        if (this->sherlock->getHP() == 0 || this->watson->getHP() == 0)
            return true;
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
            return true;
        if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
            return true;
        return false;
    }

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
            if (isStop())
            {
                break;
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
