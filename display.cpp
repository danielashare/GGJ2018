#include <SFML/Graphics.hpp> //For SFML graphics
#include "map.cpp"

const uint8_t TILE_SCALE = 32;
const uint8_t TILE_W = 64, TILE_H = 32;
const uint16_t WINDOW_W = 1024, WINDOW_H = 512;

const uint mm_size = WINDOW_W / 8; //Size of minimap on the screen
const uint mm_diag_width = sqrt(pow(mm_size, 2) + pow(mm_size, 2)); //Width of minimap rotated 45deg
const uint mm_len = MAP_A * 4;
sf::Uint8* mm = new sf::Uint8[mm_len]; //Pixel data of the minimap
sf::Texture mm_tex;
sf::RectangleShape minimap (sf::Vector2f(MAP_W, MAP_H));

void getGroundTex (uint8_t m, uint8_t &biome_code, uint16_t &tex_X, uint16_t &tex_Y)
{
    biome_code = m & 0x03;
    tex_X = biome_code * TILE_W;
    tex_Y = 0;
}

void doDISPLAY (uint32_t game_time, sf::RenderWindow &window, sf::Image groundTexImg, sf::Sprite &groundTile, bool is_lazy = false)
{
    window.clear(sf::Color(255, 255, 255));

  //Draw screen
    //Calculate map crop (as map coords)
    int16_t tiles_X, tiles_Y, camera_X1, camera_Y1, camera_X2, camera_Y2, camera_W, camera_H;
    tiles_X = (WINDOW_W / TILE_SCALE);
    tiles_Y = (WINDOW_H / TILE_SCALE);
    camera_X1 = uint16_t(protag_X) - tiles_X/2;
    camera_Y1 = uint16_t(protag_Y) - tiles_Y ;
    camera_X2 = camera_X1 + tiles_X + 2;
    camera_Y2 = camera_Y1 + tiles_Y*2 + 2;
    camera_W = camera_X2 - camera_X1;
    camera_H = camera_Y2 - camera_Y1;

  //Draw biome and sprites
    float p_X_d = decimal(protag_X);
    float p_Y_d = decimal(protag_Y);
    float p_X_D = 1 - p_X_d;
    float p_Y_D = 1 - p_Y_d;
    float protag_offset_X = ( (p_Y_D * TILE_H) + (p_X_D * (TILE_W/2)) );     //
    float protag_offset_Y = ( (p_Y_D * (TILE_H/2)) + (p_X_d * (TILE_H/2)) ); // Calculate protag decimal offset
    double draw_X = protag_offset_X, draw_Y = WINDOW_H - (TILE_H * (tiles_Y / 2)) + protag_offset_Y;
    draw_X -= TILE_W * ((tiles_X/4) + 1);
    draw_Y -= TILE_H;
    double start_draw_X = draw_X, start_draw_Y = draw_Y;
    for (int16_t y = camera_Y1; y < camera_Y2; ++y) {
        for (int16_t x = camera_X1; x < camera_X2; ++x) {
          //Get texture
            uint16_t *mapPtr = &map[x][y];
            uint8_t biome_code;
            uint16_t tex_X, tex_Y;
            getGroundTex(*mapPtr, biome_code, tex_X, tex_Y);
          //Position
            groundTile.setTextureRect(sf::IntRect(tex_X, tex_Y, TILE_W, TILE_H));
            groundTile.setPosition(sf::Vector2f(draw_X, draw_Y));
          //Modulation normalise
            groundTile.setColor(sf::Color::White);
          //Modulate if water
            if (biome_code == 3) {
                uint8_t r = 255;
                uint8_t g = 255;
                uint8_t b = 200 + fabs( sin(((x * y / 20) + ((float)game_time / 100))) ) * 55;
                groundTile.setColor(sf::Color(r, g, b));
            }
          //Modulate if protag pos
            if (x == (int16_t)protag_X && y == (int16_t)protag_Y) {
                groundTile.setColor(sf::Color(255, 0, 255));
            }
          //Draw
            window.draw(groundTile);
          //Move half right and half up
            draw_X += TILE_W / 2;
            draw_Y -= TILE_H / 2;
        }
      //Move half right and half down
        start_draw_X += (TILE_W / 2);
        start_draw_Y += (TILE_H / 2);
        draw_X = start_draw_X;
        draw_Y = start_draw_Y;
    }
  //Draw entities



  //Lazy actions (per second)
    if (is_lazy) {
        //Render minimap

        uint32_t p = 0; //Pixel pointer
        for (uint16_t y = 0; y < MAP_H; ++y) {
            for (uint16_t x = 0; x < MAP_W; ++x) {
                if (x >= protag_X - 5 && x <= protag_X + 5 || y >= protag_Y - 5 && y <= protag_Y + 5) {
                    mm[p] = 255;
                    mm[p + 1] = mm[p + 2] = 0;
                    mm[p + 3] = 255;
                } else {
                    uint16_t tex_X, tex_Y;
                    uint16_t *mapPtr = &map[x][y];
                    uint8_t biome_code = *mapPtr & 0x03;
                    sf::Color c;
                    switch (biome_code) {
                        case 0: c = sf::Color::Black; break; //Stone
                        case 1: c = sf::Color::Green; break; //Grass
                        case 2: c = sf::Color::Yellow; break; //Sand
                        case 3: c = sf::Color::Blue; break; //Water
                    }
                    mm[p]     = c.r;
                    mm[p + 1] = c.g;
                    mm[p + 2] = c.b;
                    mm[p + 3] = 255;
                }
                p += 4;
            }
        }
    }
    //Draw minimap
    mm_tex.update(mm);
    minimap.setTexture(&mm_tex);
    window.draw(minimap);



    window.display();
}
