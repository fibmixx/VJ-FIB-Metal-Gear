#define MINIAUDIO_IMPLEMENTATION
#include "AudioManager.h"
#include <iostream>

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cerr << "Error inicializando motor de audio.\n";
        return;
    }
}

void AudioManager::shutdown() {
    ma_engine_uninit(&engine);
}

void AudioManager::playSound(const std::string& filePath, float volume) {
    ma_sound* sound = new ma_sound;
    if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, sound) == MA_SUCCESS) {
        ma_sound_set_volume(sound, volume);
        ma_sound_start(sound);
    }
    else {
        std::cerr << "No se pudo cargar sonido: " << filePath << std::endl;
        delete sound;
    }
}

void AudioManager::playMusic(const std::string& filePath, bool loop) {
    if (musicPlaying)
        ma_sound_uninit(&currentMusic);

    if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &currentMusic) == MA_SUCCESS) {
        ma_sound_set_looping(&currentMusic, loop);
        ma_sound_set_volume(&currentMusic, 0.5f); //muy fuerte sino
        ma_sound_start(&currentMusic);
        musicPlaying = true;
    }
    else {
        std::cerr << "No se pudo cargar música: " << filePath << std::endl;
    }
}

void AudioManager::stopMusic() {
    if (musicPlaying) {
        ma_sound_uninit(&currentMusic);
        musicPlaying = false;
    }
}