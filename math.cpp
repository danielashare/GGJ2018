#ifndef _MATH_INCLUDED
#define _MATH_INCLUDED


#include <random> //For random generation

//===================================
//For optimised/approx code
//===================================
union
{
    int tmp;
    float f;
} u;
float sqrt_approx (float z)
{
    u.f = z;
    u.tmp -= 1 << 23; /* Subtract 2^m. */
    u.tmp >>= 1; /* Divide by 2. */
    u.tmp += 1 << 29; /* Add ((b + 1) / 2) * 2^m. */
    return u.f;
}

inline float eD_approx (float x1, float y1, float x2, float y2) //Calculate the distance between two points, as the crow flies
{
    return sqrt_approx(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

uint pi (uint32_t seed, int32_t min, int32_t max) //'Predictable' patterned random uint
{
    return ((((214013 * seed + 2531011) >> 16) & 0x7FFF) % (max - min)) + min;
}

float pf (uint seed) //'Predictable' patterned random float (doesn't work)
{
    seed = (214013 * seed + 2531011);
    return (float)0xFFFF / (float)((seed >> 16) & 0x7FFF);
}
//===================================

//===================================
//GENERATORS
//===================================
std::mt19937 gen(time(NULL));
bool rb (float dist = 0.5f) //Random boolean
{
    std::bernoulli_distribution b_dist(dist); //Random boolean https://msdn.microsoft.com/en-us/library/bb982365.aspx
    return b_dist(gen);
}

double rf (double min, double max)
{
    std::uniform_real_distribution<double> rd_dist(min, max); //http://www.cplusplus.com/reference/random/uniform_real_distribution/uniform_real_distribution/ & /operator()/
    return rd_dist(gen);
}

int ri (int min, int max)
{
    std::uniform_int_distribution<int> id_dist(min, max); //http://www.cplusplus.com/reference/random/uniform_int_distribution/ & /operator()/
    return id_dist(gen);
}

double rf_nd (double average, double spread)
{
    std::normal_distribution<double> nd_dist(average, spread);
    return nd_dist(gen);
}
//===================================


//===================================
//Angles
//===================================
float normaliseAng (float ang)
{
    if (ang >= 360) { ang -= 360; }
     else if (ang < 0) { ang += 360; }
    return ang;
}

float vecToAng (float dir_X, float dir_Y) //Takes a vector, and gives the angle where {0, -1} is 0' (North), {1, 0} is 90' (East)
{
    float ang = 90 + atan2(dir_Y, dir_X) * (180 / M_PI); //Converted from radians with r * 180/pi
    return normaliseAng(ang);
}

void angToVec (float rot, float &dir_X, float &dir_Y) //Takes an angle, and gives the vector where 0' (North) is {0, -1}, 90' (East) is {1, 0}
{
    float rad_rot = rot * (M_PI / 180); //Convert to radians with deg * (pi/180)
    dir_X = sin(rad_rot);
    dir_Y = -cos(rad_rot);
}

void targToVec (double ourX, double ourY, double tarX, double tarY, double &dirX, double &dirY)
{
  //Find the difference in coordinates
    double diffX = tarX - ourX;
    double diffY = tarY - ourY;
    //Finding the biggest direction, absolute
    bool xIsBiggest = fabs(diffX) > fabs(diffY);
    //The biggest direction becomes 1 (in the direction)
    double yDir = (diffY > 0 ? 1 : -1);
    double xDir = (diffX > 0 ? 1 : -1);
    if (xIsBiggest)
      { dirX = xDir; }
     else
      { dirY = yDir; }
    //The smallest direction becomes a fraction of 1 (in the direction, introduced from * diff)
    if (xIsBiggest)
      { dirY = (1.0 / fabs(diffX)) * diffY; }
     else
      { dirX = (1.0 / fabs(diffY)) * diffX; }
}
//===================================

void random_coord (uint16_t W, uint16_t H, uint16_t &x, uint16_t &y)
{
    x = ri(0, W - 1);
    y = ri(0, H - 1);
}

float decimal (float num)
{
    return num - (long)num;
}


#endif
