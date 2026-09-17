#ifndef _AudioManager_INCLUDE
#define _AudioManager_INCLUDE

#include "miniaudio.h"
#include <string>


class AudioManager {
public:
    static AudioManager& instance();  // Singleton

    void init();
    void shutdown();

    void playSound(const std::string& filePath, float volume = 1.0f);
    void playMusic(const std::string& filePath, bool loop = true);
    void stopMusic();

private:
    AudioManager() = default;
    ma_engine engine{};
    ma_sound currentMusic{};
    bool musicPlaying = false;

};

#endif