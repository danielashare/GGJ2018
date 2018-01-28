#include <SFML/Audio.hpp>

#define SOUNDCHANNELS 8
#define EARSHOT 16

//Create soundBuffer vector
std::vector<sf::SoundBuffer*> soundBuffer;
std::vector<sf::Sound*> soundChannel;

// function to play sound
uint8_t sCh = 0;
void playSound (uint8_t sound, float pitch, double orgX, double orgY, double protag_X, double protag_Y) {
  //Load buffer
    soundChannel[sCh]->setBuffer(*soundBuffer[sound]);
  //Calculate volume, based on distance
    float dist = eD_approx(orgX, orgY, protag_X, protag_Y);
    if (dist > EARSHOT) { return; }
    uint8_t vol = ((pow(EARSHOT, 2) - pow(dist, 2)) / pow(EARSHOT, 2)) * 100;
    soundChannel[sCh]->setVolume(vol);
  //Unique pitch
    soundChannel[sCh]->setPitch(pitch);
  //Play
    soundChannel[sCh]->play();
  //Change channel
    ++sCh;
    if (sCh == SOUNDCHANNELS) { sCh = 0; }
}
