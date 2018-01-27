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
    sf::Image groundTexImg;
    sf::Texture groundTexture;
    sf::Sprite groundTile;
    sf::Image entityTexImg;
    sf::Texture entityTexture;
    sf::Sprite entityTile;
  //Load textures/fonts
    if (!fnt_arial.loadFromFile("Assets/arial.ttf"))
    {
      std::cout << "Couldn't load Assets/arial.ttf" << std::endl;
    }
    if (!groundTexImg.loadFromFile("Assets/biomes.png")) {
        std::cout << "Couldn't load Assets/biomes.png" << std::endl;
    }
    if (!entityTexImg.loadFromFile("Assets/entities.png")) {
        std::cout << "Couldn't load Assets/entities.png" << std::endl;
    }
    groundTexture.loadFromImage(groundTexImg);
    groundTile.setTexture(groundTexture);
    groundTexture.setSmooth(false);
    entityTexture.loadFromImage(entityTexImg);
    entityTile.setTexture(entityTexture);
    entityTexture.setSmooth(true);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //Move protag forwards (NW)
            float dir_X, dir_Y;
            angToVec(protag_rot, dir_X, dir_Y);
            protag_X += dir_X / 10;
            protag_Y += dir_Y / 10;
        }

        doDISPLAY(game_time, window, groundTile);

        sf::sleep(sf::milliseconds(10));
        ++game_time;
    }

    return 0; //End the program
}
