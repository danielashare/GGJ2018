

const uint16_t GEN_VILLAGERS = 2048;
const uint16_t GEN_ZOMBIES = 128;
const float ANI_INTERVAL = 2;


class Entity {
  public:
    uint8_t type; // 0 Villager, 1 Zombie
    uint16_t index_in_array;

    double pos_X = 0, pos_Y = 0; // Entity position
    uint16_t targ_X = 0, targ_Y = 0; //Target position
    float rot = 0; // As degrees
    uint8_t frame = 0;
    bool had_moved = true;

    float health = 255, max_health = 255, speed = .02, attack = 1;

    Entity (uint16_t, uint8_t, double, double);
    Entity (); //For empty initialisation

    void think ();
    void moveTowards (uint16_t, uint16_t);
    void move ();
    void animate ();

  private:
      bool tryDir (float, float);
      float animate_clock = 0;
};

Entity::Entity (uint16_t index_in_array, uint8_t type, double pos_X, double pos_Y)
{
    this->index_in_array = index_in_array;
    this->type = type;
    this->pos_X = this->targ_X = pos_X;
    this->pos_Y = this->targ_X = pos_Y;
    this->animate_clock = ri(0, ANI_INTERVAL);
}

Entity::Entity () { }


void Entity::think ()
{
  //Loiter
    if (rb(.01) * type+1) { moveTowards(pos_X + ri(-3, 3), pos_Y + ri(-3, 3)); }
}

void Entity::moveTowards (uint16_t x, uint16_t y)
{
    targ_X = x;
    targ_Y = y;
}

bool Entity::tryDir (float dir_X, float dir_Y)
{
    float dist = eD_approx(pos_X, pos_Y, pos_X + dir_X, pos_Y + dir_Y);
    dir_X *= dist * speed;
    dir_Y *= dist * speed;
    double new_X = pos_X + dir_X, new_Y = pos_Y + dir_Y;
    if (!isSolid(getSprite(new_X, new_Y))) {
        pos_X = new_X;
        pos_Y = new_Y;
    }
}

void Entity::move ()
{
    if (abs(uint16_t(pos_X + .5) - targ_X) || abs(uint16_t(pos_Y + .5) - targ_Y)) { //Need to move?
        rot = vecToAng(targ_X - pos_X, targ_Y - pos_Y);
        float dir_X, dir_Y;
        angToVec(rot, dir_X, dir_Y);
        if (tryDir(dir_X, dir_Y)) {
            had_moved = true;
        } else {
            frame = 0;
            targ_X = pos_X;
            targ_Y = pos_Y;
        }
    } else {
        frame = 0;
    }
}

void Entity::animate ()
{
    animate_clock += speed * 4;
    if (animate_clock > ANI_INTERVAL) {
        animate_clock = 0;
        if (had_moved) { ++frame; }
        had_moved = false;
    }
}

std::vector<Entity*> entity = std::vector<Entity*>();
