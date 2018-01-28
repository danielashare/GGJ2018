

const uint16_t GEN_VILLAGERS = 2048;
const uint16_t GEN_ZOMBIES = 128;
const float ANI_INTERVAL = 2;
const uint8_t ATTACK_DISTANCE = 8;


class Entity {
  public:
    uint8_t type; // 0 Villager, 1 Zombie
    uint16_t index_in_array;

    double pos_X = 0, pos_Y = 0; // Entity position
    uint16_t targ_X = 0, targ_Y = 0; //Target position
    float rot = 0; // As degrees
    uint8_t frame = 0;
    bool had_moved = true;

    uint8_t attack_timeout = 0;

    float health_score = 255, speed = .02, power_score = 1;

    Entity (uint16_t, uint8_t, double, double);
    Entity (); //For empty initialisation

    void think ();
    void moveTowards (uint16_t, uint16_t);
    bool tryDir (float, float);
    void move ();
    void animate ();

  private:
      void attack (Entity*);
      Entity* target;
      float animate_clock = 0;
};

std::vector<Entity*> entity = std::vector<Entity*>();

Entity::Entity (uint16_t index_in_array, uint8_t type, double pos_X, double pos_Y)
{
    this->index_in_array = index_in_array;
    this->type = type;
    this->pos_X = this->targ_X = pos_X;
    this->pos_Y = this->targ_X = pos_Y;
    this->animate_clock = ri(0, ANI_INTERVAL);
}

Entity::Entity () { }

void Entity::attack (Entity* who)
{
    target = who;
    speed *= 4;
    attack_timeout = 2;
}

void Entity::think ()
{
    switch (type) {
        case 0: //Villager
          //Loiter
            if (rb(.02)) { moveTowards(pos_X + ri(-3, 3), pos_Y + ri(-3, 3)); }
            break;
        case 1: //Zombie
            if (attack_timeout) {
                --attack_timeout;
            } else {
                speed = .02;
              //Loiter
                if (rb(.03)) { moveTowards(pos_X + ri(-3, 3), pos_Y + ri(-4, 4)); }
              //Find a Villager to attack
                for (uint16_t e = 0; e < entity.size(); ++e) {
                    if (eD_approx(pos_X, pos_Y, entity[e]->pos_X, entity[e]->pos_Y) < ATTACK_DISTANCE) {
                        attack(entity[e]);
                        break;
                    }
                }
            }
            break;
    }
}

void Entity::moveTowards (uint16_t x, uint16_t y)
{
    targ_X = x;
    targ_Y = y;
}

bool Entity::tryDir (float dir_X, float dir_Y)
{
    float dist = eD_approx(pos_X, pos_Y, pos_X + dir_X, pos_Y + dir_Y);
    double d_X = dir_X * dist * speed;
    double d_Y = dir_Y * dist * speed;
    double new_X = pos_X + d_X, new_Y = pos_Y + d_Y;
    if (!isSolid(getSprite(new_X, new_Y)) && getBiome(new_X, new_Y) != B_WATER) {
        pos_X = new_X;
        pos_Y = new_Y;
        return true;
    } else {
      //Try pushing outwards
        pushCrate(new_X, new_Y, d_X, d_Y);
        return false;
    }
}

void Entity::move ()
{
    if (attack_timeout) {
        targ_X = target->pos_X;
        targ_Y = target->pos_Y;
    }
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
