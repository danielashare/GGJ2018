class Projectile {
public:
  float vel_X, vel_Y;
  double pos_X, pos_Y;
  bool had_Hit = false;
  Entity* shooter;

  void move();
  Projectile();

};

std::vector<Projectile*> projectiles = std::vector<Projectile*>();

Projectile::Projectile(double pos_X, double pos_Y, float rot, Entity* shooter) {
  this->pos_X = pos_X;
  this->pos_Y = pos_Y;
  this->shooter = shooter
  angToVec(rot, vel_X, vel_Y);
}

void Projectile::move() {
  pos_X += vel_X;
  pos_Y += vel_Y;
  if(isSolid(getSprite(pos_X, pos_Y)))
  {
    had_Hit = true;
  }
  else {
    Entity* here = entity[getMapEntity(pos_X, pos_Y)];
    if (uint16_t(here->pos_X) == uint16_t(pos_X) && uint16_t(here->pos_Y) == uint16_t(pos_Y)) {
      had_Hit = true;
      here->harm();
      shooter->reward();
    }
  }
}
