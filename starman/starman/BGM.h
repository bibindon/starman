#pragma once

#include <dsound.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

//------------------------------------------------
// BGMと環境音はアルゴリズムが異なるので分けて考える必要がある。
// ・BGMが一度に二種類再生されることはない。
// 　・そのため、BGMをセットしたら以前再生していたBGMは停止してよい。（停止しないといけない）
// ・環境音は一度に二種類以上再生できる。
// ・BGMは時々曲を変更したいが環境音にその必要はない。
// ・BGMと環境音が同時になることは問題ない。
// ・スクリプトの都合上、enumを定義せずに「ファイル名」で管理する必要がある
// ・BGMの音量だけが変更されたときは最初から再生するのではなく音量の変更だけ行われるようにする
//------------------------------------------------

struct stBgm
{
    std::string m_filename;
    bool m_bPlay = false;
    int m_volume = 0;

    // 音量が変わったか
    bool m_bChangedVolume = false;
};

struct envBgm
{
    std::string m_filename;
    bool m_bPlay = false;

    // 再生・停止の切り替えが行われたか
    bool m_bChanged = false;

    int m_volume = 0;

    // 音量が変わったか
    bool m_bChangedVolume = false;
};

// BGMのロジック部分
class BGMModel
{
public:
    BGMModel();

    // 10分に1度BGMを変更するモード
    void SetRandomMode(const bool mode);

    // 1秒に一度呼ばれる想定
    // 10分経過したらランダムで切り替え。
    void Update();

    // 再生中のBGMを取得
    // bChanged: 最近BGMが変更されたか。
    stBgm GetBGM(bool* bChanged, stBgm* bgmPrev);

    // BGMを設定
    // イカダに乗ったら専用のBGMを流したいので、外からセットするのは可能にする。
    // しかし、10分経ったらイカダに乗っていてもBGMは変わる。
    void SetBGM(const std::string& bgmType, const int volume);

    // BGMを停止
    void StopBGM();

    // 環境音は一度に複数再生できる。
    // なので停止しなければ環境音は延々と増えていく
    // 環境音は10分おきに切り替わったりしない。
    std::unordered_map<std::string, envBgm> GetEnvBGM();
    void SetEnvBGM(const std::string& bgmName, const int volume);
    void StopEnvBGM(const std::string& bgmName);

    void StopAll();

private:

    int m_counter = 0;
    stBgm m_stBgm;
    bool m_bChanged = false;
    stBgm m_stBgmPrev;

    std::unordered_map<std::string, envBgm> m_envBgmMap;
    bool m_bRandomMode = false;
};

class BGM
{
public:
    static BGM* Get();
    static void Init(HWND hwnd);
    static void Finalize(); // For memory leak check.

    void Update();

    // BGM再生中にPlayを実行したら再生中のBGMは止まる。Stopを実行する必要はない。
    void Play(const std::string& filename, const int volume);
    void Stop();

    // 環境音再生中にPlayEnvを実行しても再生中の環境音は止まらない。
    // 止めたい場合はStopEnvを実行する。
    void PlayEnv(const std::string& filename, const int volume = 100);
    void StopEnv(const std::string& filename);

    void StopAll();

    // 10分に1度BGMを変更するモード
    void SetRandomMode(const bool mode);

private:

    bool load(const std::string& filename);
    void play(const std::string& filename, const int a_volume = 100, const bool fadeIn = false);
    void stop(const std::string& filename);

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

    std::thread* m_th1 = nullptr;
    std::thread* m_th2 = nullptr;
    bool m_cancel1 = false;
    bool m_cancel2 = false;

    BGMModel m_model;
};

