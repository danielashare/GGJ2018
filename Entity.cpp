class Entity {
public:
  double position_X, position_Y; // Entity position
  float rotation; // As degrees
  uint8_t health, max_health, speed, attack;
  const uint8_t idle = 10, run = 20;
  std::string eType; // Zombie, Villager, Player
  uint16_t index_in_array;

  Entity(uint16_t index_in_array, std::string type)
  {
    this->index_in_array = index_in_array;
    this->eType = type;
    if(eType == "Villager")
    {
      placeEntity(0);
    }
    else if(eType == "Zombie")
    {
      placeEntity(1);
    }
    else
    {
      placeEntity(0);
    }
  }

  void placeEntity(uint8_t target_biome)
  {
    uint16_t possible_X, possible_Y;
    bool found = 0;
    int count = 0;
    while(!found && count < 100)
    {
      // Get random coordinate to test
      possible_X = rand() % MAP_W;
      possible_Y = rand() % MAP_H;
      std::cout << "Trying " << possible_X << ", " << possible_Y << "\n";
      if(getBiome(possible_X,possible_Y) == target_biome)
      {
        std::cout << "Match\n";
        this->position_X = (double)possible_X;
        this->position_Y = (double)possible_Y;
        found = !found;
        count = 0;
      }
      else
      {
        std::cout << "No Match" << getBiome(possible_X,possible_Y) << "\n";
        count++;
      }
      if (count == 100){
        std::cin >> count;
      }

    }
  }
};

// entities
const uint16_t MAX_ENTITIES = 512;
Entity* entity[MAX_ENTITIES];
uint16_t number_of_entities = 0;
