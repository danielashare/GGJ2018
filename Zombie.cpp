#include "Entity.cpp"

class Zombie : public Entity {
  uint8_t index_in_array;

  Zombie(uint8_t index, type type, double pos_X, double pos_Y) : Entity (type, pos_X, pox_Y)
  {
    this.index_in_array = index;
  }

};
