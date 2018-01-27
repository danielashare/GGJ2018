#include <string.h> //For memset()

#include "math.cpp"


const uint16_t MAP_W = 4096, MAP_H = 4096;
uint16_t map[MAP_W][MAP_H]; //00000000-00000000 - 0000 frame, 0000 sprite, 00 biome

//Consonants
const uint8_t ISLANDS = 1;

void setBiome (uint16_t x, uint16_t y, uint8_t b)
{
    map[x][y] = b;
}


void genMap ()
{
  //Set map to water
    memset(map, 3, sizeof map);
  //Generate islands
  //To do this, first create a list of island positions, then loop through the map and check if they are within variable radius of their centre
    uint16_t island_positions[ISLANDS][2]; //[i, [x, y]]
    for (uint8_t i = 0; i < ISLANDS; ++i) {
        uint16_t x, y;
        random_coord(MAP_W, MAP_H, x, y);
        island_positions[i][0] = x;
        island_positions[i][1] = y;
    }

    for (uint8_t i = 0; i < ISLANDS; ++i) {
        uint32_t radius = ri(256, 512);
        for (int16_t y = 0; y < MAP_H; ++y) {
            for (int16_t x = 0; x < MAP_W; ++x) {
                if (eD_approx(x, y, island_positions[i][0], island_positions[i][1]) < radius) {
                    setBiome(x, y, 0);
                }
            }
        }
    }
}
