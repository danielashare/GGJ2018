#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/tutorials/2.4
#include <iostream> //Terminal

#include "protag.cpp"
#include "display.cpp"


int main ()
{
  //Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jammy Dodger");

  //Declare texture thingies
    sf::Image groundTexImg;
    sf::Texture groundTexture;
    sf::Sprite groundTile;
  //Load textures
    if (!groundTexImg.loadFromFile("Assets/ground.png")) {
      std::cout << "Couldn't load Assets/ground.png" << std::endl;
    }
    groundTexture.loadFromImage(groundTexImg);
    groundTile.setTexture(groundTexture);
    groundTexture.setSmooth(true);


  //Start game-loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
              window.close();
        }

        doDISPLAY(window, groundTile);

        sf::sleep(sf::milliseconds(10));
    }

    return 0; //End the program
}
