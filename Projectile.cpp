class Projectile {
public:
  float vel_X, vel_Y, rot;
  double pos_X, pos_Y;
  bool had_Hit = false;

  void move();

};

void Projectile::move() {
  pos_X += vel_X;
  pos_Y += vel_Y;
  if(isSolid(getSprite(pos_X, pos_Y)))
  {
    had_Hit = true;
  }
}
