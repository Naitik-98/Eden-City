#include "AudioManager.h"
#include <iostream>

#define MINIAUDIO_IMPLEMENTATION
#include "../lib/miniaudio/miniaudio.h"

bool AudioManager::init() {
    m_engine = new ma_engine(); // Zero-initialize
    ma_result result = ma_engine_init(NULL, static_cast<ma_engine*>(m_engine));
    if (result != MA_SUCCESS) {
        std::cerr << "[AudioManager] Failed to initialize audio engine.\n";
        delete static_cast<ma_engine*>(m_engine);
        m_engine = nullptr;
        return false;
    }
    
    m_sound = new ma_sound(); // Zero-initialize
    return true;
}

void AudioManager::cleanup() {
    if (m_sound) {
        if (m_soundInitialized) {
            ma_sound_uninit(static_cast<ma_sound*>(m_sound));
        }
        delete static_cast<ma_sound*>(m_sound);
        m_sound = nullptr;
    }
    if (m_engine) {
        ma_engine_uninit(static_cast<ma_engine*>(m_engine));
        delete static_cast<ma_engine*>(m_engine);
        m_engine = nullptr;
    }
}

void AudioManager::playAmbient(const std::string& filepath) {
    if (!m_engine || !m_sound) return;

    // Uninit if already playing
    if (m_soundInitialized) {
        ma_sound_uninit(static_cast<ma_sound*>(m_sound));
        m_soundInitialized = false;
    }

    ma_result result = ma_sound_init_from_file(static_cast<ma_engine*>(m_engine), filepath.c_str(), 0, NULL, NULL, static_cast<ma_sound*>(m_sound));
    if (result == MA_SUCCESS) {
        m_soundInitialized = true;
        ma_sound_set_looping(static_cast<ma_sound*>(m_sound), MA_TRUE);
        ma_sound_set_volume(static_cast<ma_sound*>(m_sound), 0.5f); // 50% volume
        ma_sound_start(static_cast<ma_sound*>(m_sound));
    } else {
        std::cerr << "[AudioManager] Failed to load sound: " << filepath << "\n";
    }
}

void AudioManager::stopAmbient() {
    if (m_sound) {
        ma_sound_stop(static_cast<ma_sound*>(m_sound));
    }
}
