

const uint16_t GEN_VILLAGERS = 2048;
const uint16_t GEN_ZOMBIES = 128;


class Entity {
  public:
    double pos_X = 0, pos_Y = 0; // Entity position
    uint16_t targ_X = 0, targ_Y = 0; //Target position
    float rotation = 0; // As degrees
    float health = 255, max_health = 255, speed = .1, attack = 1;
    const uint8_t idle = 10, run = 20;
    uint8_t type; // 0 Villager, 1 Zombie
    uint16_t index_in_array;

    Entity (uint16_t, uint8_t, double, double);
    Entity (); //For empty initialisation

    void think ();
    void moveTowards (uint16_t, uint16_t);
    void move ();
};

Entity::Entity (uint16_t index_in_array, uint8_t type, double pos_X, double pos_Y)
{
    this->index_in_array = index_in_array;
    this->type = type;
    this->pos_X = this->targ_X = pos_X;
    this->pos_Y = this->targ_X = pos_Y;
}

Entity::Entity () { }


void Entity::think ()
{
  //Loiter
    moveTowards(pos_X + ri(-2, 2), pos_Y + ri(-2, 2));
}

void Entity::moveTowards (uint16_t x, uint16_t y)
{
    targ_X = x;
    targ_Y = y;
}

void Entity::move ()
{
    float angle = vecToAng(targ_X - pos_X, targ_Y - pos_Y);
    float dir_X, dir_Y;
    angToVec(angle, dir_X, dir_Y);
    pos_X += speed * dir_X;
    pos_Y += speed * dir_Y;
}

std::vector<Entity*> entity = std::vector<Entity*>();
