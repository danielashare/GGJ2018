#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/tutorials/2.4
#include <iostream> //Terminal

#include "display.cpp"


const uint16_t RELOAD_TIME = 10;


int main ()
{
  //Create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Infection");
  //Display rendering loading screen


  //Declare asset thingies
    sf::Text txt_HUD;
    sf::Font fnt_arial;
    sf::Image biomeTexImg;
    sf::Texture biomeTexture;
    sf::Sprite biomeTile;
    sf::Image spriteTexImg;
    sf::Texture spriteTexture;
    sf::Sprite spriteTile;
    sf::Image entityTexImg;
    sf::Texture entityTexture;
    sf::Sprite entityTile;
    sf::Image villagerTexImg;
    sf::Texture villagerTexture;
    sf::Sprite villagerTile;
    sf::Image zombieTexImg;
    sf::Texture zombieTexture;
    sf::Sprite zombieTile;
    sf::CircleShape projectileTile (3);
  //Load textures/fonts
    if (!fnt_arial.loadFromFile("Assets/arial.ttf"))
    {
      std::cout << "Couldn't load Assets/arial.ttf" << std::endl;
    }
    if (!biomeTexImg.loadFromFile("Assets/biomes.png")) {
        std::cout << "Couldn't load Assets/biomes.png" << std::endl;
    }
    if (!spriteTexImg.loadFromFile("Assets/sprites.png")) {
        std::cout << "Couldn't load Assets/sprites.png" << std::endl;
    }
    if (!villagerTexImg.loadFromFile("Assets/villager.png")) {
        std::cout << "Couldn't load Assets/villager.png" << std::endl;
    }
    if (!zombieTexImg.loadFromFile("Assets/zombie.png")) {
        std::cout << "Couldn't load Assets/zombie.png" << std::endl;
    }
    txt_float.setFont(fnt_arial);
    txt_float.setCharacterSize(12);
    txt_HUD.setFont(fnt_arial);
    txt_HUD.setCharacterSize(20);
    txt_HUD.setPosition(sf::Vector2f(4, WINDOW_H - 24));
    txt_float.setFillColor(sf::Color::White);
    biomeTexture.loadFromImage(biomeTexImg);
    biomeTile.setTexture(biomeTexture);
    biomeTexture.setSmooth(false);
    spriteTexture.loadFromImage(spriteTexImg);
    spriteTile.setTexture(spriteTexture);
    spriteTexture.setSmooth(false);
    villagerTexture.loadFromImage(villagerTexImg);
    villagerTile.setTexture(villagerTexture);
    villagerTexture.setSmooth(false);
    zombieTexture.loadFromImage(zombieTexImg);
    zombieTile.setTexture(zombieTexture);
    zombieTexture.setSmooth(false);
    projectileTile.setFillColor(sf::Color::Red);
  //Minimap
    mm_tex.create(MAP_W, MAP_H);
    minimap.scale(float(mm_size) / float(MAP_W), float(mm_size) / float(MAP_H));
    minimap.setOutlineThickness(1);
    minimap.setOutlineColor(sf::Color(0, 0, 0));
    minimap.setPosition(sf::Vector2f(0, mm_diag_width / 2));
    minimap.setRotation(-45);

  //Generate map
    genMap();

  //Dummy Entity
    entity.push_back(new Entity());
  //Spawn Villagers
    for (uint16_t v = 0; v < GEN_VILLAGERS; ++v) {
        uint16_t x, y;
        do {
            x = ri(0, MAP_W);
            y = ri(0, MAP_H);
        } while (getBiome(x, y) != B_STONE || getSprite(x, y));
        entity.push_back(new Entity(0, x, y));
    }
    Entity* prot = entity[1];
  //Spawn Zombies
    for (uint16_t z = 0; z < GEN_ZOMBIES; ++z) {
        uint16_t x, y;
        do {
            x = ri(0, MAP_W);
            y = ri(0, MAP_H);
        } while (getBiome(x, y) == B_STONE || getBiome(x, y) == B_WATER || getSprite(x, y));
        entity.push_back(new Entity(1, x, y));
    }
  //Move player to suitable location (stone)
    do {
        prot->pos_X = ri(0, MAP_W);
        prot->pos_Y = ri(0, MAP_H);
    } while (getBiome(prot->pos_X, prot->pos_Y) != B_STONE || getSprite(prot->pos_X, prot->pos_Y));

    uint64_t prev_shot = 0;
  //Start game-loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
              window.close();
        }

      //Check mouse
        sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
		mouse_pos.x = (float(WINDOW_W) / float(window.getSize().x)) * float(mouse_pos.x);  //
		mouse_pos.y = (float(WINDOW_H) / float(window.getSize().y)) * float(mouse_pos.y); // Adjust for scaled window
        prot->rot = normaliseAng(vecToAng((mouse_pos.x - float(WINDOW_W / 2)) / 2, mouse_pos.y - float(WINDOW_H / 2)) + 45);
      //Check keyboard and mouse
        float dir_X = 0, dir_Y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //Move protag foward (NW)
            angToVec(prot->rot, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //Move protag right (NE)
            angToVec(prot->rot + 90, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //Move protag backward (SE)
            angToVec(prot->rot + 180, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //Move protag left (SW)
            angToVec(prot->rot + 270, dir_X, dir_Y);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && prev_shot + RELOAD_TIME < game_time) {
            prot->shootDir();
            prev_shot = game_time;
        }
      //Speed based on mouse distance from the center
        if (mouse_pos.x > WINDOW_W*.75) { mouse_pos.x = WINDOW_W*.75; }
        if (mouse_pos.x < WINDOW_W*.25) { mouse_pos.x = WINDOW_W*.25; }
        float dist = eD_approx(WINDOW_W/2, WINDOW_H/2, mouse_pos.x, mouse_pos.y) / (WINDOW_W/2);
        dir_X *= dist;
        dir_Y *= dist;
      //Set protag data
        if (fabs(dir_X) + fabs(dir_Y) > .1) {
            prot->had_moved = true;
            prot->speed = dist / 8;
        }
        double new_X = prot->pos_X + dir_X/6;
        double new_Y = prot->pos_Y + dir_Y/6;
        if (prot->tryDir(dir_X, dir_Y)) {
            prot->pos_X = new_X;
            prot->pos_Y = new_Y;
        }
        prot->rot = prot->rot;

      //DISPLAY
        doDISPLAY(prot, game_time, window, biomeTile, spriteTile, villagerTile, zombieTile, projectileTile, txt_float, txt_HUD, !(game_time % 50));

      //Entity & Projectile stuff
        bool is_nighttime = sky_darkness < .4;
        uint16_t ents = 0, humans = 0, zombies = 0;
        for (uint16_t e = 2; e < entity.size(); ++e) {
            Entity* ent = entity[e];
            ent->animate();
            if (ent->is_dead) { continue; }
            if (rb(0.01)) { ent->think(is_nighttime); }
            ent->move();

            ++ents;
            if (ent->type == E_VILLAGER) { ++humans; }
             else if (ent->type == E_ZOMBIE) { ++zombies; }
        }
        for (uint16_t p = 0; p < projectile.size(); ++p) {
            Projectile* proj = projectile[p];
            proj->opacity -= .01;
            if (proj->opacity <= 0) {
                projectile.erase(projectile.begin() + p);
                --p;
                continue;
            }
            if (!proj->had_hit) { proj->move(); }
        }
        prot->animate();


      //HUD
        txt_HUD.setString("Human: "+ std::to_string(humans) +", zombie: "+ std::to_string(zombies) +"; "+ std::to_string(uint8_t(float(zombies)/ents*100)) +"% infected");
        sf::sleep(sf::milliseconds(10));
        ++game_time;
    }

    return 0; //End the program
}
