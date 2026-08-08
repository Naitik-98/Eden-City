#pragma once

#include <string>

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    bool init();
    void cleanup();

    void playAmbient(const std::string& filepath);
    void stopAmbient();

private:
    AudioManager() = default;
    ~AudioManager() = default;

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    void* m_engine = nullptr; // Opaque pointer to hide miniaudio types in header
    void* m_sound = nullptr;
    bool m_soundInitialized = false;
};
