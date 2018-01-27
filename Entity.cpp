class Entity {
public:
  uint16_t prev_X = 0, prev_Y = 0; //Previous position on the entity_map
  double position_X = 0, position_Y = 0; // Entity position
  uint16_t target_X = 0, target_Y = 0; //Target position
  float rotation = 0; // As degrees
  uint8_t health = 255, max_health = 255, speed = 1, attack = 1;
  const uint8_t idle = 10, run = 20;
  uint8_t type; // 0 Villager, 1 Zombie
  uint16_t index_in_array;

  Entity (uint16_t, uint8_t, double, double);
  Entity (); //For empty initialisation

  void moveTowards (uint16_t, uint16_t);
  void move ();
};

Entity::Entity (uint16_t index_in_array, uint8_t type, double pos_X, double pos_Y)
{
    this->index_in_array = index_in_array;
    this->type = type;
    this->position_X = this->prev_X = this->target_X = pos_X;
    this->position_Y = this->prev_X = this->target_X = pos_Y;
}

Entity::Entity () { }

void Entity::moveTowards (uint16_t x, uint16_t y)
{
    target_X = x;
    target_Y = y;
}

void Entity::move ()
{
    float angle = vecToAng(position_X - target_X, position_Y - target_Y);
    float dir_X, dir_Y;
    angToVec(angle, dir_X, dir_Y);
    position_X += dir_X;
    position_Y += dir_Y;
}

// entities
const uint16_t MAX_ENTITIES = 512;
Entity* entity[MAX_ENTITIES];
uint16_t number_of_entities = 0;
