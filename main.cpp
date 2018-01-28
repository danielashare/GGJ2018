#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/tutorials/2.4
#include <iostream> //Terminal

#include "protag.cpp"
#include "display.cpp"


int main ()
{
  //Create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Infection");
  //Display rendering loading screen


  //Declare asset thingies
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
    txt_HUD.setFont(fnt_arial);
    txt_HUD.setCharacterSize(12);
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
  //Minimap
    mm_tex.create(MAP_W, MAP_H);
    minimap.scale(float(mm_size) / float(MAP_W), float(mm_size) / float(MAP_H));
    minimap.setOutlineThickness(1);
    minimap.setOutlineColor(sf::Color(0, 0, 0));
    minimap.setPosition(sf::Vector2f(0, mm_diag_width / 2));
    minimap.setRotation(-45);

  //Generate map
    genMap();

  //Spawn Villagers
    for (uint16_t v = 0; v < GEN_VILLAGERS; ++v) {
        uint16_t x, y;
        do {
            x = ri(0, MAP_W);
            y = ri(0, MAP_H);
        } while (getBiome(x, y) != B_STONE || getSprite(x, y));
        entity.push_back(new Entity(v, 0, x, y));
    }
  //Spawn Zombies
    for (uint16_t z = 0; z < GEN_ZOMBIES; ++z) {
        uint16_t x, y;
        do {
            x = ri(0, MAP_W);
            y = ri(0, MAP_H);
        } while (getBiome(x, y) == B_STONE || getSprite(x, y));
        entity.push_back(new Entity(z, 1, x, y));
    }
  //Move player to suitable location (stone)
    do {
        protag_X = ri(0, MAP_W);
        protag_Y = ri(0, MAP_H);
    } while (getBiome(protag_X, protag_Y) != B_STONE || getSprite(protag_X, protag_Y));

  //Start game-loop
    uint32_t game_time = 0;
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
        protag_rot = vecToAng((mouse_pos.x - float(WINDOW_W / 2)) / 2, mouse_pos.y - float(WINDOW_H / 2)) + 45;
        protag_rot = normaliseAng(protag_rot);
      //Check keyboard
        float dir_X = 0, dir_Y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //Move protag foward (NW)
            angToVec(protag_rot, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //Move protag right (NE)
            angToVec(protag_rot + 90, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //Move protag backward (SE)
            angToVec(protag_rot + 180, dir_X, dir_Y);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //Move protag left (SW)
            angToVec(protag_rot + 270, dir_X, dir_Y);
        }
      //Speed based on mouse distance from the center
        float dist = eD_approx(WINDOW_W/2, WINDOW_H/2, mouse_pos.x, mouse_pos.y) / (WINDOW_W/2);
        dir_X *= dist;
        dir_Y *= dist;
      //Set protag data
        if (fabs(dir_X) + fabs(dir_Y) > .1) {
            entity[0]->had_moved = true;
            entity[0]->speed = dist / 8;
        }
        double new_X = protag_X + (dir_X / 10);
        double new_Y = protag_Y + (dir_Y / 5);
        if (entity[0]->tryDir(dir_X, dir_Y)) {
            entity[0]->pos_X = protag_X = new_X;
            entity[0]->pos_Y = protag_Y = new_Y;
        }
        entity[0]->rot = protag_rot;

        doDISPLAY(game_time, window, biomeTile, spriteTile, villagerTile, zombieTile, txt_HUD, !(game_time % 50));

      //Entity stuff
        for (uint16_t e = 1; e < entity.size(); ++e) {
            if (rb(0.01)) { entity[e]->think(); }
            entity[e]->move();
            entity[e]->animate();
        }
        entity[0]->animate();

        sf::sleep(sf::milliseconds(10));
        ++game_time;
    }

    return 0; //End the program
}
