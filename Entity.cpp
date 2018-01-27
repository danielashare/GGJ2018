class Entity {
public:
  double position_X, position_Y; // Entity position
  float rotation; // As degrees
  uint8_t health, max_health, speed, attack;
  const uint8_t idle = 10, run = 20;
  std::string eType; // Zombie, Villager, Player
  uint16_t index_in_array;

  Entity(uint16_t index_in_array, std::string type, double pos_X, double pos_Y)
  {
    this->index_in_array = index_in_array;
    this->eType = type;
    this->position_X = pos_X;
    this->position_Y = pos_Y;
  }
};

// entities
const uint16_t MAX_ENTITIES = 512;
Entity* entity[MAX_ENTITIES];
uint16_t number_of_entities = 0;
