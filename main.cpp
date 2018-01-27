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
    biomeTexture.loadFromImage(biomeTexImg);
    biomeTile.setTexture(biomeTexture);
    biomeTexture.setSmooth(false);
    spriteTexture.loadFromImage(spriteTexImg);
    spriteTile.setTexture(spriteTexture);
    spriteTexture.setSmooth(false);
    villagerTexture.loadFromImage(spriteTexImg);
    villagerTile.setTexture(spriteTexture);
    villagerTexture.setSmooth(false);
    zombieTexture.loadFromImage(spriteTexImg);
    zombieTile.setTexture(spriteTexture);
    zombieTexture.setSmooth(false);
  //Minimap
    mm_tex.create(MAP_W, MAP_H);
    minimap.scale(float(mm_size) / float(MAP_W), float(mm_size) / float(MAP_H));
    minimap.setOutlineThickness(1);
    minimap.setOutlineColor(sf::Color(0, 0, 0));
    minimap.setPosition(sf::Vector2f(0, mm_diag_width / 2));
    minimap.setRotation(-45);

  //Generate entities
    entity[0] = new Entity(0, "Player", 0, 0);
    std::cout << "Made " << entity[0]->eType << " " << std::to_string(entity[0]->index_in_array) << "\n";
    for(int i = 1; i < 257; i++)
    {
      entity[i] = new Entity(i, "Villager", 0, 0);
      std::cout << "Made " << entity[i]->eType << " " << std::to_string(entity[i]->index_in_array) << "\n";
    }
    for(int i = 257; i < 513; i++)
    {
      entity[i] = new Entity(i, "Zombie", 0, 0);
      std::cout << "Made " << entity[i]->eType << " " << std::to_string(entity[i]->index_in_array) << "\n";
    }

  //Generate map
    genMap();


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
        normaliseAng(protag_rot);
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
        protag_X += dir_X;// / 10;
        protag_Y += dir_Y;// / 5;

        doDISPLAY(game_time, window, biomeTile, spriteTile, villagerTile, zombieTile, !(game_time % 50));

        sf::sleep(sf::milliseconds(10));
        ++game_time;
    }

    return 0; //End the program
}
