#include <SFML/Graphics.hpp> //For SFML graphics
#include "map.cpp"

const uint8_t TILE_SCALE = 32;
const uint8_t TILE_W = 64, TILE_H = 32;
const uint8_t SPRITE_W = 64, SPRITE_H = 64;
const uint16_t WINDOW_W = 1024, WINDOW_H = 512;

const uint16_t mm_size = WINDOW_W / 8; //Size of minimap on the screen
const uint16_t mm_diag_width = sqrt(pow(mm_size, 2) + pow(mm_size, 2)); //Width of minimap rotated 45deg
const uint32_t mm_len = MAP_A * 4;
const uint8_t mm_crosshair = 2; //Half width of crosshair, in tiles
sf::Uint8* mm = new sf::Uint8[mm_len]; //Pixel data of the minimap
sf::Texture mm_tex;
sf::RectangleShape minimap (sf::Vector2f(MAP_W, MAP_H));

const uint16_t daynight_cycle = 1000;
float daynight_colour (uint32_t game_time, uint16_t mapPtr) {
    float lux = getLux(mapPtr);
    lux = ((fabs(sin(float(game_time + daynight_cycle) / daynight_cycle)) * .9) + .1) + (lux / 5);
    return (lux < 1 ? lux : 1);
}

void getBiomeTex (uint8_t m, uint8_t &biome_code, uint16_t &tex_X, uint16_t &tex_Y)
{
    biome_code = getBiome(m);
    tex_X = biome_code * TILE_W;
    tex_Y = 0;
}
void getSpriteTex (uint8_t sprite_code, uint16_t mapPtr, uint16_t &tex_X, uint16_t &tex_Y)
{
    tex_X = getFrame(mapPtr) * SPRITE_W;
    tex_Y = (sprite_code - 1) * SPRITE_H;
}




void drawBiome (uint32_t game_time, sf::RenderWindow &window, sf::Sprite &biomeTile, uint16_t x, uint16_t y, double draw_X, double draw_Y)
{
    uint16_t *mapPtr = &map[x][y];
  //Prepare biome for draw
    //Get biome texture
    uint8_t biome_code;
    uint16_t tex_X, tex_Y;
    getBiomeTex(*mapPtr, biome_code, tex_X, tex_Y);
    //Set biome Position
    biomeTile.setTextureRect(sf::IntRect(tex_X, tex_Y, TILE_W, TILE_H));
    biomeTile.setPosition(sf::Vector2f(draw_X, draw_Y));
    //Modulate biome
    uint8_t r = 255, g = 255, b = 255;
    switch (biome_code) {
        case 0: //Stone
        {
            uint8_t c = pi((x*y), 0, 20);
            r = 235 + c;
            g = 235 + c;
            b = 235 + c;
        }
            break;
        case 1: //Grass
            r = 210 + fabs( sin(x * y / 20) ) * 40;
            g = 225 + fabs( sin(x * y / 33) ) * 30;
            b = 255;
            break;
        case 3: //Water
            r = 255;
            g = 255;
            b = 200 + fabs( sin(((x * y / 20) + ((float)game_time / 100))) ) * 55;
            break;
    }
    //Modulate for day/night
    r *= daynight_colour(game_time, *mapPtr);
    g *= daynight_colour(game_time, *mapPtr);
    b *= daynight_colour(game_time, *mapPtr);
    biomeTile.setColor(sf::Color(r, g, b));
    //Modulate if protag pos
    if (x == (int16_t)protag_X && y == (int16_t)protag_Y) {
        biomeTile.setColor(sf::Color(255, 0, 255));
    }
    //Draw biome
    window.draw(biomeTile);
}

void drawSprite (uint32_t game_time, sf::RenderWindow &window, sf::Sprite &spriteTile, uint16_t x, uint16_t y, double draw_X, double draw_Y)
{
    uint16_t *mapPtr = &map[x][y];
    uint8_t sprite_code = getSprite(*mapPtr);
    if (sprite_code) {
      //Prepare sprite for draw
        uint16_t tex_X, tex_Y;
        getSpriteTex(sprite_code, *mapPtr, tex_X, tex_Y);
        //Set sprite Position
        spriteTile.setTextureRect(sf::IntRect(tex_X, tex_Y, SPRITE_W, SPRITE_H));
        spriteTile.setPosition(sf::Vector2f(draw_X, draw_Y - SPRITE_H/2));
        //Modulation
        uint8_t r = 255, g = 255, b = 255;
        //If foliage, modulate with a distinctive colour
        if (isFoliage(sprite_code)) {
            uint8_t l = pi(x * y + 3, 0, 50);
            r = pi(x * y    , 200, 255) - l;
            g = pi(x * y + 1, 200, 255) - l;
            b = pi(x * y + 2, 200, 255) - l;
        }
        //Modulate for day/night
        r *= daynight_colour(game_time, *mapPtr);
        g *= daynight_colour(game_time, *mapPtr);
        b *= daynight_colour(game_time, *mapPtr);
        spriteTile.setColor(sf::Color(r, g, b));
        //Draw sprite
        window.draw(spriteTile);
    }
}

void doISOMETRIC (uint32_t game_time, sf::RenderWindow &window, void (*drawer)(uint32_t, sf::RenderWindow&, sf::Sprite&, uint16_t, uint16_t, double, double), sf::Sprite tile)
{
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
  //Prepare isometric loop
    float p_X_d = decimal(protag_X);
    float p_Y_d = decimal(protag_Y);
    float p_X_D = 1 - p_X_d;
    float p_Y_D = 1 - p_Y_d;
    float protag_offset_X = ( (p_Y_D * TILE_H) + (p_X_D * (TILE_W/2)) );     //
    float protag_offset_Y = ( (p_Y_D * (TILE_H/2)) + (p_X_d * (TILE_H/2)) ); // Calculate protag decimal offset
    double draw_X = protag_offset_X, draw_Y = -(TILE_H * (tiles_Y / 2)) + protag_offset_Y;
    draw_X += TILE_W * (tiles_X/4);
    draw_Y -= TILE_H*2;
    double start_draw_X = draw_X, start_draw_Y = draw_Y;
  //Start isometric loop
    for (int16_t y = camera_Y1; y < camera_Y2; ++y) {
      for (int16_t x = camera_X2; x >= camera_X1; --x) {
        //Preapare and call upon the argument drawer function
          (*drawer)(game_time, window, tile, x, y, draw_X, draw_Y);
        //Move half left and half down
          draw_X -= TILE_W / 2;
          draw_Y += TILE_H / 2;
      }
    //Move half right and half down
      start_draw_X += (TILE_W / 2);
      start_draw_Y += (TILE_H / 2);
      draw_X = start_draw_X;
      draw_Y = start_draw_Y;
    }
}

void doDISPLAY (uint32_t game_time, sf::RenderWindow &window, sf::Sprite &biomeTile, sf::Sprite &spriteTile, sf::Sprite &entityTile, bool is_lazy = false)
{
    window.clear(sf::Color(255, 255, 255));

    doISOMETRIC(game_time, window, drawBiome, biomeTile);
    doISOMETRIC(game_time, window, drawSprite, spriteTile);

  //Draw entities

  //Lazy actions
    if (is_lazy) {
        uint32_t p = 0; //Pixel pointer for minimap
        for (uint16_t y = 0; y < MAP_H; ++y) {
            for (uint16_t x = 0; x < MAP_W; ++x) {
                if (getAnimated(map[x][y])) {
                    uint8_t frame = getFrame(map[x][y]);
                    setFrame(x, y, (frame < 6 ? frame + 1 : 0));
                }
              //Render minimap
                if (x >= protag_X - mm_crosshair && x <= protag_X + mm_crosshair || y >= protag_Y - mm_crosshair && y <= protag_Y + mm_crosshair) {
                    mm[p] = 255;
                    mm[p + 1] = mm[p + 2] = 0;
                    mm[p + 3] = 255;
                } else {
                    uint16_t tex_X, tex_Y;
                    uint16_t *mapPtr = &map[x][y];
                    uint8_t biome_code = getBiome(*mapPtr);
                    uint8_t sprite_code = getSprite(*mapPtr);
                    sf::Color c;
                    if (sprite_code) {
                        switch (sprite_code) {
                            case 1: c = sf::Color(128, 84, 0); break; //Crate
                            case 2: c = sf::Color(0, 0, 0); break; //Brick
                            case 3: c = sf::Color::Yellow; break; //Campfire
                            default:
                                if (isFoliage(sprite_code)) {
                                    c = sf::Color(0, 64, 0); //Foliage
                                }
                                break;
                        }
                    } else {
                        switch (biome_code) {
                            case 0: c = sf::Color(100, 64, 0); break; //Stone
                            case 1: c = sf::Color(0, 128, 0); break; //Grass
                            case 2: c = sf::Color::Yellow; break; //Sand
                            case 3: c = sf::Color::Blue; break; //Water
                        }
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
