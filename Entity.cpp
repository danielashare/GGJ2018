class Entity {
public:
  bool inited = false;
  uint16_t prev_X = 0, prev_Y = 0; //Previous position on the entity_map
  double position_X = 0, position_Y = 0; // Entity position
  uint16_t target_X = 0, target_Y = 0; //Target position
  float rotation = 0; // As degrees
  uint8_t health = 255, max_health = 255, speed = 1, attack = 1;
  const uint8_t idle = 10, run = 20;
  uint8_t type; // 0 Villager, 1 Zombie
  uint16_t index_in_array;

  void placeEntity(uint8_t target_biome)
  {
    uint16_t possible_X, possible_Y;
    bool found = 0;
    while(!found)
    {
      // Get random coordinate to test
      possible_X = ri(0, MAP_W - 1);
      possible_Y = ri(0, MAP_H - 1);
      std::cout << "Trying " << possible_X << ", " << possible_Y << "\n";
      if(getBiome(possible_X,possible_Y) == target_biome)
      {
        std::cout << "Match\n";
        this->position_X = (double)possible_X;
        this->position_Y = (double)possible_Y;
        found = !found;
      }

    }
  }

  Entity (uint16_t, uint8_t);
  Entity (); //For empty initialisation

  void moveTowards (uint16_t, uint16_t);
  void move ();
};

void updateEntityMap (Entity* e);

Entity::Entity (uint16_t index_in_array, uint8_t type)
{
    this->inited = true;
    this->index_in_array = index_in_array;
    this->type = type;
    if(type == 0)
    {
      placeEntity(0);
    }
    else if(type == 1)
    {
      placeEntity(1);
    }
    updateEntityMap(this);
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
    updateEntityMap(this);
}

// entities
const uint16_t MAX_ENTITIES = 512;
Entity* entity[MAX_ENTITIES];
uint16_t number_of_entities = 0;
Entity* entity_map[MAP_W][MAP_H];


void updateEntityMap (Entity* e)
{
std::cout << std::to_string(uint16_t(e->position_X)) << ", " << std::to_string(uint16_t(e->position_Y)) << std::endl;
    entity_map[e->prev_X][e->prev_Y] = new Entity();
    entity_map[uint16_t(e->position_X)][uint16_t(e->position_Y)] = e;
}

Entity* getEntity (uint16_t x, uint16_t y)
{
    return entity_map[x][y];
}
