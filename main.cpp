#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/tutorials/2.4

int main ()
{
  //Create the window
    sf::RenderWindow window(sf::VideoMode(500, 500), "Bounce"); //The window will be 500x500 px, and have the title "Bounce"

  //Create a ball with a radius of 50
    sf::CircleShape ball(50);

  //Store the ball's direction
    int directionX = 5; //This will make the ball go to the right!
    int directionY = 4; //This will make the ball go down!
  //^ an 'int' is an integer - a whole number from -3bn to 3bn

  //Start a 'game loop'
    while (true) { //Loop FOREVER!

      //Clear the window with a colour (painting over anything there before!)
        window.clear(sf::Color(128, 128, 200));

      //Move the ball in the stored direction
        ball.move(directionX, directionY);

      //Check if the ball has bounced against the edges!
        //Get the ball's current position
        auto bPos = ball.getPosition(); //(Here, 'auto' automatically figures out what datatype getPosition() is)
        //Check if touching the left/right edges)
        if (bPos.x < 0 || bPos.x > 400) { directionX *= -1; } //If X is smaller than 0 OR bigger than 500, flip the direction!
        if (bPos.y < 0 || bPos.y > 400) { directionY *= -1; } //Same here x3
        //The reason it's 400 instead of 500 is because the ball is 100px wide. Its 'position' is the top-left corner of the ball

      //Draw the ball to the screen
        window.draw(ball);

      //Display all our changes to the window (the draws don't come up one-by-one, but all at once!)
        window.display();

      //Sleep the program for a very small time
        sf::sleep(sf::milliseconds(16));

    }

    return 0; //End the program
}
