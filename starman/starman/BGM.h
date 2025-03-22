#pragma once

#include <dsound.h>
#include <string>
#include <vector>
#include <unordered_map>

class BGM
{
public:
    static BGM* get_ton();
    static void initialize(HWND hwnd);
    static void finalize(); // For memory leak check.

    bool load(const std::string& filename);
    void play(const std::string& filename, const int a_volume = 100);
    void stop(const std::string& filename);
    void StopAll();

private:
    static BGM* single_ton_;
    // hide ctor
    BGM(HWND hwnd);
    BGM(const BGM&) { }
    void operator=(const BGM&) { }

    bool open_wave(
        const std::string& filepath,
        WAVEFORMATEX& waveFormatEx,
        std::vector<char>* ppData,
        DWORD& dataSize);

    int per_to_decibel(const int percent);

    LPDIRECTSOUND8 dx8sound_ { nullptr };
    std::unordered_map<std::string, LPDIRECTSOUNDBUFFER8> dx8sound_buffers_ { };
};

