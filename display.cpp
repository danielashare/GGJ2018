#include <SFML/Graphics.hpp> //For SFML graphics
#include "map.cpp"

const uint8_t TILE_SCALE = 32;
const uint8_t TILE_W = 64, TILE_H = 32;
const uint16_t window_W = 1024, window_H = 512;

void getTex (uint8_t m, uint16_t &tex_X, uint16_t &tex_Y)
{

}

void doDISPLAY (sf::RenderWindow &window, sf::Sprite &groundTile)
{
    window.clear(sf::Color(255, 255, 255));

  //Draw map ground
    //Calculate map crop
    int16_t tiles_X, tiles_Y, camera_X1, camera_Y1, camera_X2, camera_Y2;
    tiles_X = (window_W / TILE_SCALE);
    tiles_Y = (window_H / TILE_SCALE);
    camera_X1 = px - (tiles_X / 2);
    camera_Y1 = py - (tiles_Y / 2);
    camera_X2 = camera_X1 + tiles_X;
    camera_Y2 = camera_Y1 + tiles_Y;

    int16_t draw_X = 0, draw_Y = tiles_Y / 2;
    for (int16_t y = camera_Y1; y < camera_Y2; ++y) {
        for (int16_t x = camera_X1; x < camera_X2; ++x) {
          //Move half right and half up
            draw_X += TILE_W / 2;
            draw_Y -= TILE_H / 2;
          //Get texture
            uint8_t *mapPtr = &map[x][y];
            uint16_t tex_X, tex_Y;
            getTex(*mapPtr, tex_X, tex_Y);
        }
    }

    window.display();
}
