enum type {Zombie, Villager, Player};

// entities
const uint16_t MAX_ENTITIES = 512;
Entity* entity[MAX_ENTITIES];
uint8_t number_of_entities = 0;

class Entity {
  double position_X, position_Y; // Entity position
  float rotation; // As degrees
  uint8_t health, max_health, speed, attack;
  const uint8_t idle = 10, run = 20;
  type eType;

  Entity(type type, double pos_X, double pos_Y)
  {
    this->eType = type;
    this->position_X = pos_X;
    this->position_Y = pos_Y;
  }
};
