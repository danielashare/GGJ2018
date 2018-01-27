class Entity {
  double position_X, position_Y; // Entity position
  float rotation; // As degrees
  uint8_t health, max_health, speed, attack;
  const uint8_t idle = 10, run = 20;
  enum type {Zombie, Villager, Player};

};
