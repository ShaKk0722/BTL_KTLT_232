#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
const Position Position::npos = Position(-1, -1);
//          Code bên dưới

// MapElement
MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
}
ElementType MapElement::getType() const
{
    return this->type;
}

// MapElement - Path
Path::Path()
{
    this->type = PATH;
}
int Path::getReqExp() const
{
    return -1;
}

// MapElement - Wall
Wall::Wall()
{
    this->type = WALL;
}
int Wall::getReqExp() const
{
    return -1;
}

// MapElement - FakeWall
FakeWall::FakeWall()
{
    this->type = FAKE_WALL;
    req_exp = 0;
}
FakeWall::FakeWall(int in_req_exp)
{
    this->type = FAKE_WALL;
    req_exp = in_req_exp;
}
int FakeWall::getReqExp() const
{
    return this->req_exp;
}

// Position
Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}
Position::Position(const string &str_pos)
{
    int comma = str_pos.find(',');
    int end = str_pos.find(')');
    string r_value = str_pos.substr(1, comma - 1);
    string c_value = str_pos.substr(comma + 1, end - comma - 1);
    this->r = stoi(r_value);
    this->c = stoi(c_value);
}

bool Position::operator!=(const Position &other) const
{
    return r != other.r || c != other.c;
}
void Position::operator=(const Position &other)
{
    this->r = other.r;
    this->c = other.c;
}
int Position::getRow() const
{
    return this->r;
}
int Position::getCol() const
{
    return this->c;
}
void Position::setRow(int r)
{
    this->r = r;
}
void Position::setCol(int c)
{
    this->c = c;
}
string Position::str() const
{
    string ans = "(" + to_string(r) + "," + to_string(c) + ")";
    return ans;
}
bool Position::isEqual(int in_r, int in_c) const
{
    return (this->r == in_r && this->c == in_c);
}
bool Position::isEqual(Position other) const
{
    return (this->r == other.r && this->c == other.c);
}

// MovingObject
int MovingObject::getEXP() const
{
    return 0;
}
MovingObject::MovingObject(){}; // default constructor
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}
Position MovingObject::getCurrentPosition() const
{
    return this->pos;
}
int MovingObject::getIndex() const
{
    return index;
}
string MovingObject::getName() const
{
    return name;
}
void MovingObject::set_position(Position &pos)
{
    this->pos.setRow(pos.getRow());
    this->pos.setCol(pos.getCol());
}
MovingObject::~MovingObject()
{
}
string MovingObject::str() const
{
    return "empty";
}
Robot *MovingObject::init_robot(int index)
{
    return nullptr;
}

// Map
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
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
Map::~Map()
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
int Map::getNumRows() const
{
    return this->num_rows;
}
int Map::getNumCols() const
{
    return this->num_cols;
}
bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
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

// character
Character::Character(){}; // default constructor
Character::Character(int index, const Position &pos, Map *map, int hp, int exp, string name) : MovingObject(index, pos, map, name)
{
    setHP(hp);
    setEXP(exp);
}
int Character::getHP() const
{
    return hp;
}
int Character::getEXP() const
{
    return exp;
}
void Character::setHP(int hp)
{
    this->hp = (hp > 500) ? 500 : hp;
}
void Character::setEXP(int exp)
{

    this->exp = (exp > 900) ? 900 : exp;
}

// MovingObject - Sherlock
Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Sherlock")
{
    this->moving_rule = moving_rule;
    this->bag = new SherlockBag(this);
}
string Sherlock::str() const
{
    string ans = "Sherlock[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
    return ans;
}
void Sherlock::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
Position Sherlock::getNextPosition()
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
SherlockBag *Sherlock::getBag()
{
    return this->bag;
}
bool Sherlock::meetRobot(MovingObject* robot)
{
    return (this->getCurrentPosition().isEqual(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()));
}
bool Sherlock::meetWatson(Watson* watson)
{
    int r = watson->getCurrentPosition().getRow();
    int c = watson->getCurrentPosition().getCol();
    return (this->getCurrentPosition().isEqual(r, c));
}
void Sherlock::tradeWatson(Watson* watson)
{
    while (true)
    {
        BaseItem* item = bag->get(PASSING_CARD);
        if (item == nullptr) return;
        if (!watson->getBag()->insert(item))
        {
            this->bag->insert(item);
            return;
        }
    }
}
bool Sherlock::beatC(MovingObject* robotC)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 1)
    {
        item = this->bag->get(EXCEMPTION_CARD);
    }
    delete item;
    if (this->getEXP() > 500)
    {
        return true;
    }
    return false;
}
void Sherlock::beatW(MovingObject* robotW)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 1)
    {
        item = this->bag->get(EXCEMPTION_CARD);
    }
    delete item;
    this->bag->insert(robotW->getItem());
    return;
}
void Sherlock::beatS(MovingObject* robotS)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 1)
    {
        item = this->bag->get(EXCEMPTION_CARD);
    }
    if (this->getEXP() > 400)
    {
        delete item;
        this->bag->insert(robotS->getItem());
        return;
    }
    if (item != nullptr)
    {
        delete item;
        return;
    }
    this->setEXP(this->getEXP() * 90 / 100);
}
void Sherlock::beatSW(MovingObject* robotSW)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 1)
    {
        item = this->bag->get(EXCEMPTION_CARD);
    }
    if (this->getEXP() > 300 && this->getHP() > 335)
    {
        delete item;
        this->bag->insert(robotSW->getItem());
        return;
    }
    if (item != nullptr)
    {
        delete item;
        return;
    }
    this->setEXP(this->getEXP() * 85 / 100);
    this->setHP(this->getHP() * 85 / 100);
}
bool Sherlock::beatRobot(MovingObject* robot)
{
    if (robot->getName() == "RobotC")
    {
        if (this->beatC(robot))
        {
            return true;
        }
    }
    else if (robot->getName() == "RobotW")
    {
        this->beatW(robot);
    }
    else if (robot->getName() == "RobotS")
    {
        this->beatS(robot);
    }
    else if (robot->getName() == "RobotSW")
    {
        this->beatSW(robot);
    }
    BaseItem * item = this->getBag()->get();
    if (item != nullptr) item->use(this, nullptr);
    return false;
}








// MovingObject - Watson
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Watson")
{
    this->bag = new WatsonBag(this);
    this->moving_rule = moving_rule;
}
string Watson::str() const
{
    string ans = "Watson[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
    return ans;
}
void Watson::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
Position Watson::getNextPosition()
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
WatsonBag *Watson::getBag()
{
    return this->bag;
}
bool Watson::meetRobot(MovingObject* robot)
{
    return (this->getCurrentPosition().isEqual(robot->getCurrentPosition().getRow(), robot->getCurrentPosition().getCol()));
}
void Watson::tradeSherlock(Sherlock* sherlock)
{
    while (true)
    {
        BaseItem* item = bag->get(EXCEMPTION_CARD);
        if (item == nullptr) return;
        if (!sherlock->getBag()->insert(item))
        {
            this->bag->insert(item);
            return;
        }
    }
}
void Watson::beatC(MovingObject* robotC)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 0)
    {
        item = this->bag->get(PASSING_CARD);
    }
    if (item != nullptr)
    {
        item->use(this, (Robot*)robotC);
        delete item;
    }
    this->bag->insert(robotC->getItem());

}
void Watson::beatW(MovingObject* robotW)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 0)
    {
        item = this->bag->get(PASSING_CARD);
    }
    if (item != nullptr)
    {
        item->use(this, (Robot*)robotW);
        this->bag->insert(robotW->getItem());
        delete item;
        return;
    }
    if (this->getHP() > 350)
    {
        this->bag->insert(robotW->getItem());
        return;
    }
    this->setHP(this->getHP() * 95 / 100);
}
void Watson::beatS(MovingObject* robotS)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 0)
    {
        item = this->bag->get(PASSING_CARD);
    }
    if (item != nullptr)
    {
        item->use(this, (Robot*)robotS);
        this->bag->insert(robotS->getItem());
        delete item;
        return;
    }

}
void Watson::beatSW(MovingObject* robotSW)
{
    BaseItem* item = nullptr;
    if (this->getHP() % 2 == 0)
    {
        item = this->bag->get(PASSING_CARD);
    }
    if (item != nullptr)
    {
        item->use(this, (Robot*)robotSW);
        this->bag->insert(robotSW->getItem());
        delete item;
        return;
    }
    if (this->getEXP() > 600 && this->getHP() > 165)
    {
        this->bag->insert(robotSW->getItem());
        return;
    }
    this->setEXP(this->getEXP() * 85 / 100);
    this->setHP(this->getHP() * 85 / 100);
}
void Watson::beatRobot(MovingObject* robot)
{
    if (robot->getName() == "RobotC")
    {
        this->beatC(robot);
    }
    else if (robot->getName() == "RobotW")
    {
        this->beatW(robot);
    }
    else if (robot->getName() == "RobotS")
    {
        this->beatS(robot);
    }
    else if (robot->getName() == "RobotSW")
    {
        this->beatSW(robot);
    }
    BaseItem* item = this->getBag()->get();
    if(item !=nullptr) item->use(this, nullptr);
}






// MovingObject - Criminal
Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, 0, 0, "Criminal")
{
    this->step_count = 0;
    this->sherlock = sherlock;
    this->watson = watson;
    first = false;
}
string Criminal::str() const
{
    string ans = "Criminal[index=" + to_string(this->index) + ";pos=" + this->pos.str() + "]";
    return ans;
}
int Criminal::get_distance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}
void Criminal::move()
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
Position Criminal::getNextPosition()
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
    int ans = -1000000;
    char ch = 'u';
    if (map->isValid(pri_up, this))
    {
        int dis_sher = get_distance(pri_up.getRow(), pri_up.getCol(), sher_cur.getRow(), sher_cur.getCol());
        int dis_wat = get_distance(pri_up.getRow(), pri_up.getCol(), wat_cur.getRow(), wat_cur.getCol());
        u = dis_sher + dis_wat;
        if (u > ans)
        {
            ans = u;
            ch = 'u';
        }
    }
    if (map->isValid(pri_left, this))
    {
        int dis_sher = get_distance(pri_left.getRow(), pri_left.getCol(), sher_cur.getRow(), sher_cur.getCol());
        int dis_wat = get_distance(pri_left.getRow(), pri_left.getCol(), wat_cur.getRow(), wat_cur.getCol());
        l = dis_sher + dis_wat;
        if (l > ans)
        {
            ans = l;
            ch = 'l';
        }
    }
    if (map->isValid(pri_down, this))
    {
        int dis_sher = get_distance(pri_down.getRow(), pri_down.getCol(), sher_cur.getRow(), sher_cur.getCol());
        int dis_wat = get_distance(pri_down.getRow(), pri_down.getCol(), wat_cur.getRow(), wat_cur.getCol());
        d = dis_sher + dis_wat;
        if (d > ans)
        {
            ans = d;
            ch = 'd';
        }
    }
    if (map->isValid(pri_right, this))
    {
        int dis_sher = get_distance(pri_right.getRow(), pri_right.getCol(), sher_cur.getRow(), sher_cur.getCol());
        int dis_wat = get_distance(pri_right.getRow(), pri_right.getCol(), wat_cur.getRow(), wat_cur.getCol());
        r = dis_sher + dis_wat;
        if (r > ans)
        {
            ans = r;
            ch = 'r';
        }
    }
    if (ans == -1000000)
        return pri_cur.npos;
    if (ch == 'u')
        return pri_up;
    else if (ch == 'l')
        return pri_left;
    else if (ch == 'd')
        return pri_down;
    else
        return pri_right;
}
Position Criminal::getPrevPosition() const
{
    return this->prev_pos;
}
int Criminal::getStepCount() const
{
    return this->step_count; // check mod 3
}
Map *Criminal::get_map()
{
    return this->map;
}
Robot *Criminal::init_robot(int index)
{
    if (step_count != 3)
        return nullptr;
    if (first == false)
    {
        first = true;
        step_count = 0;
        return new RobotC(index, this->prev_pos, map, this);
    }
    Position sher = sherlock->getCurrentPosition();
    Position wat = watson->getCurrentPosition();
    int dis_sher = get_distance(sher.getRow(), sher.getCol(), this->prev_pos.getRow(), this->prev_pos.getCol());
    int dis_wat = get_distance(wat.getRow(), wat.getCol(), this->prev_pos.getRow(), this->prev_pos.getCol());
    step_count = 0;
    if (dis_sher < dis_wat)
        return new RobotS(index, this->prev_pos, map, this, this->sherlock);
    else if (dis_wat < dis_sher)
        return new RobotW(index, this->prev_pos, map, this, this->watson);
    return new RobotSW(index, this->prev_pos, map, this, this->sherlock, this->watson);
}

// MovingObject
ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[capacity];
    for (int i = 0; i < capacity; i++)
    {
        arr_mv_objs[i] = NULL;
    }
}
ArrayMovingObject::~ArrayMovingObject()
{
    delete[] arr_mv_objs;
}
bool ArrayMovingObject::isFull() const
{
    return (count == capacity);
}
bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (this->isFull())
        return false;
    this->arr_mv_objs[count] = mv_obj;
    this->count++;
    return true;
}
int ArrayMovingObject::size() const
{
    return this->count;
}
string ArrayMovingObject::str() const
{
    string ans = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity);
    for (int i = 0; i < count; i++)
    {
        if (this->arr_mv_objs[i] != nullptr)
            ans = ans + ";" + this->arr_mv_objs[i]->str();
    }
    ans = ans + "]";
    return ans;
}
MovingObject *ArrayMovingObject::get(int index) const
{
    return this->arr_mv_objs[index];
}

// config
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
void Configuration::loadConfig(const string &str)
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
Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    string line;
    while (getline(file, line))
    {
        loadConfig(line);
    }
    file.close();
}
Configuration::~Configuration()
{
    // delete[] arr_fake_walls;
    // delete[] arr_walls;
}
string Configuration::str() const
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

// BaseItem
BaseItem::BaseItem(ItemType type) : type(type){};
int BaseItem::getItemType() const
{
    return type;
}
BaseItem::~BaseItem() {}

// BaseItem - MagicBook
MagicBook::MagicBook() : BaseItem(MAGIC_BOOK){};
bool MagicBook::canUse(Character *obj, Robot *robot)
{
    return obj->getEXP() <= 350;
}
void MagicBook::use(Character *obj, Robot *robot)
{  
    int newHP = round(obj->getEXP() * 125.0 / 100);
    obj->setEXP(newHP);
}
string MagicBook::str()
{
    return "MagicBook";
}

// BaseItem - EnergyDrink
EnergyDrink::EnergyDrink() : BaseItem(ENERGY_DRINK){};
bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    return obj->getHP() <= 100;
}
void EnergyDrink::use(Character *obj, Robot *robot)
{
    int newHP = round(obj->getHP() * 120 / 100);
    obj->setHP(newHP);
}
string EnergyDrink::str()
{
    return "EnergyDrink";
}

// BaseItem - FirstAid
FirstAid::FirstAid() : BaseItem(FIRST_AID){};
bool FirstAid::canUse(Character *obj, Robot *robot)
{
    return (obj->getEXP() <= 350 || obj->getHP() <= 100);
}
void FirstAid::use(Character *obj, Robot *robot)
{
    int newHP = round(obj->getHP() * 150 / 100);
    obj->setHP(newHP);
}
string FirstAid::str()
{
    return "FirstAid";
}

// BaseItem - ExcemptionCard
ExcemptionCard::ExcemptionCard() : BaseItem(EXCEMPTION_CARD){};
bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    return (obj->getName() == "Sherlock" && obj->getHP() % 2 != 0);
}
void ExcemptionCard::use(Character *obj, Robot *robot)
{
}
string ExcemptionCard::str()
{
    return "ExcemptionCard";
}

// BaseItem - PassingCard
bool PassingCard::canUse(Character *obj, Robot *robot)
{
    return (obj->getName() == "Watson" && obj->getHP() % 2 != 0);
}
void PassingCard::use(Character *obj, Robot *robot)
{
    if (this->chal == "all")
        return;
    if (this->chal == robot->getName())
        return;
    if (obj->getEXP() >= 50)
        obj->setEXP(obj->getEXP() - 50);
    else
        obj->setEXP(0);
}
string PassingCard::str()
{
    return "PassingCard";
}

// Robot
Robot::Robot(){};
Robot::Robot(RobotType robotType, int index, const Position &pos, Map *map, string name) : MovingObject(index, pos, map, name)
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
bool Robot::addtoArrayMovingObject(ArrayMovingObject &arr_moving)
{
    if (!arr_moving.isFull())
    {
        arr_moving.add(this);
        return true;
    }
    return false;
}
int Robot::calculateDistance(Position other1, Position other2) const
{
    return abs(other1.getCol() - other2.getCol()) + abs(other1.getRow() - other2.getRow());
}
BaseItem *Robot::getItem() const
{
    return this->item;
}
RobotType Robot::getRobotType() const
{
    return this->robottype;
}
int Robot::getDistance(Character* obj) const
{
    return calculateDistance(this->getCurrentPosition(), obj->getCurrentPosition());
}




// Robot - RobotC
RobotC::RobotC(){};
RobotC::RobotC(int index, const Position &pos, Map *map, Criminal *criminal) : Robot(C, index, pos, map, "RobotC")
{
    this->criminal = criminal;
}
void RobotC::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
string RobotC::str() const
{
    string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=C" + ";dist=]";
    return ans;
}
Position RobotC::getNextPosition()
{
    Position next_cur = criminal->getPrevPosition();
    return (criminal->get_map()->isValid(next_cur, this) ? next_cur : next_cur.npos);
}

// Robot - RobotS
RobotS::RobotS(){};
RobotS::RobotS(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock) : Robot(S, index, pos, map, "RobotS")
{
    this->criminal = criminal;
    this->sherlock = sherlock;
}
void RobotS::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
string RobotS::str() const
{
    string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=S" + ";dist=";
    int distance = this->calculateDistance(this->getCurrentPosition(), sherlock->getCurrentPosition());
    ans = ans + to_string(distance) + "]";
    return ans;
}
Position RobotS::getNextPosition()
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
    int ans = 1000000;
    char ch = 'u';
    if (map->isValid(pri_up, this))
    {
        u = calculateDistance(pri_up, sher_cur);
        if (u < ans)
        {
            ans = u;
            ch = 'u';
        }
    }
    if (map->isValid(pri_right, this))
    {
        r = calculateDistance(pri_right, sher_cur);
        if (r < ans)
        {
            ans = r;
            ch = 'r';
        }
    }
    if (map->isValid(pri_down, this))
    {
        d = calculateDistance(pri_down, sher_cur);
        if (d < ans)
        {
            ans = d;
            ch = 'd';
        }
    }
    if (map->isValid(pri_left, this))
    {
        l = calculateDistance(pri_left, sher_cur);
        if (l < ans)
        {
            ans = l;
            ch = 'l';
        }
    }

    if (ans == 1000000)
        return pri_cur.npos;
    if (ch == 'u')
        return pri_up;
    else if (ch == 'r')
        return pri_right;
    else if (ch == 'd')
        return pri_down;
    else
        return pri_left;
}

// Robot - RobotW
RobotW::RobotW(){};
RobotW::RobotW(int index, const Position &pos, Map *map, Criminal *criminal, Watson *watson) : Robot(W, index, pos, map, "RobotW")
{
    this->criminal = criminal;
    this->watson = watson;
}
void RobotW::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
string RobotW::str() const
{
    string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=W" + ";dist=";
    int distance = this->calculateDistance(this->getCurrentPosition(), watson->getCurrentPosition());
    ans = ans + to_string(distance) + "]";
    return ans;
}
Position RobotW::getNextPosition()
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
    int ans = 1000000;
    char ch = 'u';
    if (map->isValid(pri_up, this))
    {
        u = calculateDistance(pri_up, wat_cur);
        if (u < ans)
        {
            ans = u;
            ch = 'u';
        }
    }
    if (map->isValid(pri_right, this))
    {
        r = calculateDistance(pri_right, wat_cur);
        if (r < ans)
        {
            ans = r;
            ch = 'r';
        }
    }
    if (map->isValid(pri_down, this))
    {
        d = calculateDistance(pri_down, wat_cur);
        if (d < ans)
        {
            ans = d;
            ch = 'd';
        }
    }
    if (map->isValid(pri_left, this))
    {
        l = calculateDistance(pri_left, wat_cur);
        if (l < ans)
        {
            ans = l;
            ch = 'l';
        }
    }

    if (ans == 1000000)
        return pri_cur.npos;
    if (ch == 'u')
        return pri_up;
    else if (ch == 'r')
        return pri_right;
    else if (ch == 'd')
        return pri_down;
    else
        return pri_left;
}


// Robot - RobotSW
RobotSW::RobotSW(){};
RobotSW::RobotSW(int index, const Position &pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson) : Robot(SW, index, pos, map, "RobotSW")
{
    this->criminal = criminal;
    this->sherlock = sherlock;
    this->watson = watson;
}
void RobotSW::move()
{
    Position next = getNextPosition();
    if (next != next.npos)
    {
        this->set_position(next);
    }
}
string RobotSW::str() const
{
    string ans = "Robot[pos=" + this->getCurrentPosition().str() + ";type=SW" + ";dist=";
    int sher = this->calculateDistance(this->getCurrentPosition(), sherlock->getCurrentPosition());
    int wat = this->calculateDistance(this->getCurrentPosition(), watson->getCurrentPosition());
    int distance = sher + wat;
    ans = ans + to_string(distance) + "]";
    return ans;
}
Position RobotSW::getNextPosition()
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
    int ans = 1000000;
    string ch = "u";

    if (map->isValid(pri_up_up, this))
    {
        if (map->isValid(check_up, this))
        {
            int dis_sher = calculateDistance(pri_up_up, sher_cur);
            int dis_wat = calculateDistance(pri_up_up, wat_cur);
            u = dis_wat + dis_sher;
            if (u < ans)
            {
                ans = u;
                ch = "u";
            }
        }
    }
    if (map->isValid(pri_up_right, this))
    {
        if (map->isValid(check_up, this) || map->isValid(check_right, this))
        {
            int dis_sher = calculateDistance(pri_up_right, sher_cur);
            int dis_wat = calculateDistance(pri_up_right, wat_cur);
            u_r = dis_wat + dis_sher;
            if (u_r < ans)
            {
                ans = u_r;
                ch = "u_r";
            }
        }
    }
    if (map->isValid(pri_right_right, this))
    {
        if (map->isValid(check_right, this))
        {
            int dis_sher = calculateDistance(pri_right_right, sher_cur);
            int dis_wat = calculateDistance(pri_right_right, wat_cur);
            r = dis_wat + dis_sher;
            if (r < ans)
            {
                ans = r;
                ch = "r";
            }
        }
    }
    if (map->isValid(pri_down_right, this))
    {
        if (map->isValid(check_down, this) || map->isValid(check_right, this))
        {
            int dis_sher = calculateDistance(pri_down_right, sher_cur);
            int dis_wat = calculateDistance(pri_down_right, wat_cur);
            d_r = dis_wat + dis_sher;
            if (d_r < ans)
            {
                ans = d_r;
                ch = "d_r";
            }
        }
    }
    if (map->isValid(pri_down_down, this))
    {
        if (map->isValid(check_down, this))
        {
            int dis_sher = calculateDistance(pri_down_down, sher_cur);
            int dis_wat = calculateDistance(pri_down_down, wat_cur);
            d = dis_wat + dis_sher;
            if (d < ans)
            {
                ans = d;
                ch = "d";
            }
        }
    }
    if (map->isValid(pri_down_left, this))
    {
        if (map->isValid(check_down, this) || map->isValid(check_left, this))
        {
            int dis_sher = calculateDistance(pri_down_left, sher_cur);
            int dis_wat = calculateDistance(pri_down_left, wat_cur);
            d_l = dis_wat + dis_sher;
            if (d_l < ans)
            {
                ans = d_l;
                ch = "d_l";
            }
        }
    }
    if (map->isValid(pri_left_left, this))
    {
        if (map->isValid(check_left, this))
        {
            int dis_sher = calculateDistance(pri_left_left, sher_cur);
            int dis_wat = calculateDistance(pri_left_left, wat_cur);
            l = dis_wat + dis_sher;
            if (l < ans)
            {
                ans = l;
                ch = "l";
            }
        }
    }
    if (map->isValid(pri_up_left, this))
    {
        if (map->isValid(check_up, this) || map->isValid(check_left, this))
        {
            int dis_sher = calculateDistance(pri_up_left, sher_cur);
            int dis_wat = calculateDistance(pri_up_left, wat_cur);
            u_l = dis_wat + dis_sher;
            if (u_l < ans)
            {
                ans = u_l;
                ch = "u_l";
            }
        }
    }
    if (ans == 1000000)
        return pri_cur.npos;
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







// BaseBag
BaseBag::BaseBag(){};
BaseBag::BaseBag(Character *obj)
{
    this->obj = obj;
}
bool BaseBag::insert(BaseItem *item)
{
    return true;
}
BaseItem *BaseBag::get()
{
    return nullptr;
}
BaseItem *BaseBag::get(ItemType itemType)
{
    return nullptr;
}
string BaseBag::str() const
{
    return "";
}








// BaseBag - SherlockBag
SherlockBag::SherlockBag(){};
SherlockBag::SherlockBag(Sherlock *sherlock)
{
    this->count = 0;
    this->head = NULL;
    this->obj = sherlock;
}
Node *SherlockBag::getHead()
{
    return this->head;
}
bool SherlockBag::insert(BaseItem *item)
{
    if (count >= capacity)
        return false;
    Node *temp = new Node(item, head);
    head = temp;
    count++;
    return true;
}
BaseItem* SherlockBag::get()
{
    Node* current = head;
    while (current != nullptr && (current->item->getItemType() == 0 || current->item->getItemType() == 1 || current->item->getItemType() == 2)) 
    {
        if (current->item->canUse(obj, nullptr))
        {
            BaseItem* temp = current->item;
            current->item = head->item;
            head->item = nullptr;
            Node* tempNode = head;
            head = head->next;
            delete tempNode;
            count--;
            return temp;
        }
        current = current->next;
    }
    return nullptr;
}
BaseItem *SherlockBag::get(ItemType itemType)
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
string SherlockBag::str() const
{
    string ans = "Bag[count=" + to_string(this->count) + ";";

    for (Node *temp = head; temp != NULL; temp = temp->next)
    {
        ans += temp->item->str() + ",";
        temp = temp->next;
    }
    return ans + "]";
}

// BaseBag - WatsonBag

WatsonBag::WatsonBag(){};
WatsonBag::WatsonBag(Watson *watson)
{
    this->count = 0;
    this->head = NULL;
    this->obj = watson;
}
Node *WatsonBag::getHead()
{
   
    return this->head;
}
bool WatsonBag::insert(BaseItem *item)
{
    if (count >= capacity)
        return false;
    Node *temp = new Node(item, head);
    head = temp;
    count++;
    return true;
}
BaseItem *WatsonBag::get()
{
    Node* current = head;
    while (current != nullptr && (current->item->getItemType() == 0 || current->item->getItemType() == 1 || current->item->getItemType() == 2))
    {
        if (current->item->canUse(obj, nullptr))
        {
            BaseItem* temp = current->item;
            current->item = head->item;
            head->item = nullptr;
            Node* tempNode = head;
            head = head->next;
            delete tempNode;
            count--;
            return temp;
        }
        current = current->next;
    }
    return nullptr;
}
BaseItem *WatsonBag::get(ItemType itemType)
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
string WatsonBag::str() const
{
    string ans = "Bag[count=" + to_string(this->count) + ";";

    for (Node *temp = head; temp != NULL; temp = temp->next)
    {
        ans += temp->item->str() + ",";
        temp = temp->next;
    }
    return ans + "]";
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////