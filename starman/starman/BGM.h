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
    std::wstring m_filename;
    bool m_bPlay = false;
    int m_volume = 0;

    // 音量が変わったか
    bool m_bChangedVolume = false;
};

struct envBgm
{
    std::wstring m_filename;
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
    void SetBGM(const std::wstring& bgmType, const int volume);

    // BGMを停止
    void StopBGM();

    // 環境音は一度に複数再生できる。
    // なので停止しなければ環境音は延々と増えていく
    // 環境音は10分おきに切り替わったりしない。
    std::unordered_map<std::wstring, envBgm> GetEnvBGM();
    void SetEnvBGM(const std::wstring& bgmName, const int volume);
    void StopEnvBGM(const std::wstring& bgmName);

    void StopAll();

private:

    int m_counter = 0;
    stBgm m_stBgm;
    bool m_bChanged = false;
    stBgm m_stBgmPrev;

    std::unordered_map<std::wstring, envBgm> m_envBgmMap;
    bool m_bRandomMode = false;

    // BGMの選曲
    // 上のルールほど優先度が高い
    //
    // 死亡していたら死亡時のBGM
    // タイトルだったらタイトルのBGM
    // オープニングだったらオープニングのBGM
    // エンディングだったらエンディングのBGM
    // 戦闘だったら戦闘のBGM
    // 瀕死だったら瀕死のBGM
    // 体力が少ないなら体力が少ないBGM
    // 夜だったら夜のBGM
    // 航海中だったら航海中のBGM
    // 特定の地域なら特定のBGM
    // それ以外ならランダム再生
    //
    // 基本的にBGMModel内で判定をする。外からじゃないと設定できない時もある。
    // そのときはそのときだ。

    bool m_bDead = false;
    bool m_bTitle = false;
    bool m_bOpening = false;
    bool m_bEnding = false;
    bool m_bBattle = false;
    bool m_bDying = false;
    bool m_bWeak = false;
    bool m_bNight = false;
    bool m_bVoyage = false;
    bool m_bHaikyo = false; // 廃墟の近くにいる
    bool m_bToudai = false; // 灯台の近くにいる
    bool m_bKaiganDoukutsu = false; // 海岸洞窟の近くにいる
    bool m_bJinja = false; // 神社の近くにいる
    bool m_bKokeniwa = false; // 苔庭の近くにいる
    bool m_bMinatoAto = false; // 港跡の近くにいる
    bool m_bDoukutsu = false; // 洞窟の中にいる

    std::string m_strTitle = "res\\sound\\title.wav";
    std::string m_strOpening = "res\\sound\\opening.wav";

    std::string m_strEnding = "res\\sound\\ending.wav";
    std::string m_strBattle1 = "res\\sound\\battle1.wav";
    std::string m_strBattle2 = "res\\sound\\battle2.wav";
    std::string m_strDying = "res\\sound\\dying.wav";
    std::string m_strWeak = "res\\sound\\weak.wav";
    std::string m_strNight = "res\\sound\\night.wav";
    std::string m_strVoyage = "res\\sound\\voyage.wav";
    std::string m_strHaikyo = "res\\sound\\haikyo.wav";
    std::string m_strToudai = "res\\sound\\toudai.wav";
    std::string m_strKaiganDoukutsu = "res\\sound\\kaiganDoukutsu.wav";
    std::string m_strJinja = "res\\sound\\jinja.wav";
    std::string m_strKokeniwa = "res\\sound\\kokeniwa.wav";
    std::string m_strMinatoAto = "res\\sound\\minatoato.wav";
    std::string m_strDoukutsu = "res\\sound\\doukutsu.wav";

    std::string m_strField1 = "res\\sound\\field1.wav";
    std::string m_strField2 = "res\\sound\\field2.wav";
    std::string m_strField3 = "res\\sound\\field3.wav";

};

class BGM
{
public:
    static BGM* Get();
    static void Init(HWND hwnd);
    static void Finalize(); // For memory leak check.

    void Update();

    // BGM再生中にPlayを実行したら再生中のBGMは止まる。Stopを実行する必要はない。
    void Play(const std::wstring& filename, const int volume);
    void Stop();

    // 環境音再生中にPlayEnvを実行しても再生中の環境音は止まらない。
    // 止めたい場合はStopEnvを実行する。
    void PlayEnv(const std::wstring& filename, const int volume = 100);
    void StopEnv(const std::wstring& filename);

    void StopAll();

    // 10分に1度BGMを変更するモード
    void SetRandomMode(const bool mode);

private:

    bool load(const std::wstring& filename);
    void play(const std::wstring& filename, const int a_volume = 100, const bool fadeIn = false);
    void stop(const std::wstring& filename);

    static BGM* single_ton_;
    // hide ctor
    BGM(HWND hwnd);
    BGM(const BGM&) { }
    void operator=(const BGM&) { }

    bool open_wave(
        const std::wstring& filepath,
        WAVEFORMATEX& waveFormatEx,
        std::vector<char>* ppData,
        DWORD& dataSize);

    int per_to_decibel(const int percent);

    LPDIRECTSOUND8 dx8sound_ { nullptr };
    std::unordered_map<std::wstring, LPDIRECTSOUNDBUFFER8> dx8sound_buffers_ { };

    std::thread* m_th1 = nullptr;
    std::thread* m_th2 = nullptr;
    bool m_cancel1 = false;
    bool m_cancel2 = false;

    BGMModel m_model;
};

