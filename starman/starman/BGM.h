#pragma once

#include <dsound.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

class BGM
{
public:
    static BGM* get_ton();
    static void initialize(HWND hwnd);
    static void finalize(); // For memory leak check.

    bool load(const std::string& filename);
    void play(const std::string& filename, const int a_volume = 100, const bool fadeIn = false);
    void stop(const std::string& filename);
    void StopAll();

    bool IsPlay(const std::string& filename);

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
    std::unordered_map<std::string, bool> m_isPlayMap;

    std::thread* m_th1 = nullptr;
    std::thread* m_th2 = nullptr;
    bool m_cancel1 = false;
    bool m_cancel2 = false;
};

