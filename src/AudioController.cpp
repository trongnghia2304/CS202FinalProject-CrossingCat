#include "AudioController.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <iostream>
#include <stdexcept>

AudioController *AudioController::_instance = nullptr;

AudioController::AudioController() {
    isMuted = false;
    loadSoundFromFile(SoundEffect::Hit, "./SmallHit.wav");
    loadMusicFromFile(Music::Game, "./MainTheme.wav");
}

AudioController &AudioController::instance() {
    if (_instance == nullptr) {
        _instance = new AudioController;
    }

    return *_instance;
}

void AudioController::playSound(SoundEffect::ID id) {
    if (isMuted)
        return;

    sound.setBuffer(getSoundBuffer(id));
    sound.play();
}

sf::SoundBuffer& AudioController::getSoundBuffer(SoundEffect::ID id) {
    auto found = soundBar.find(id);
    return *found->second;
}

void AudioController::playMusic(Music::ID id) {
    auto filename = musicPlaylist.find(id)->second;
    
    if (!music.openFromFile(filename)) {
        music.play();
    } else {
        std::cerr << "Loading music from \"" + filename + "\" failed.\n";
    }
}

void AudioController::pauseMusic() {
    music.pause();
}

void AudioController::resumeMusic() {
    music.play();
}

void AudioController::setMuted(bool mute) {
    mute = isMuted;
    
    updateSettings();
}

void AudioController::setMusicVolume(float vol) {
    musicVolume = vol;

    updateSettings();
}

void AudioController::updateSettings() {
    if (isMuted) {
        music.setVolume(0);
    } else {
        music.setVolume(musicVolume);
    }
}

void AudioController::loadMusicFromFile(Music::ID id, const std::string &path) {
    musicPlaylist.insert({id, path});
}

void AudioController::loadSoundFromFile(SoundEffect::ID id, const std::string &path) {
    std::unique_ptr<sf::SoundBuffer> pBuffer(new sf::SoundBuffer);
    if (pBuffer->loadFromFile(path)) {
        soundBar.insert({id, std::move(pBuffer)});
    } else {
        std::cerr << "Loading sound effect from \"" << path << "\" failed.\n";
    }
}
