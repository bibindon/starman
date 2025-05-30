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

enum class eBGMStatus
{
    NONE,
    START_REQUEST,
    STARTED,
    STOP_REQUEST,
    STOPPED,
};

struct stBGM
{
    std::string m_filename;
    eBGMStatus m_eBGMStatus = eBGMStatus::NONE;
    int m_volume = 0;

    // 音量が変わったか
    bool m_bChangedVolume = false;
};

struct envBGM
{
    std::string m_filename;
    eBGMStatus m_eBGMStatus = eBGMStatus::NONE;
    int m_volume = 0;

    // 音量が変わったか
    bool m_bChangedVolume = false;
};

// BGMのロジック部分
class BGMModel
{
public:
    BGMModel();

    // 1秒に一度呼ばれる想定
    void Update();

    bool GetChangeRequest(stBGM* stBGM1, stBGM* stBGM2);
    void SetChangeRequestComplete();

private:

    stBGM m_stBGM;
    stBGM m_stBGMPrev;

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
    // TODO クエストでこの会話中はこのBGMを流したい、というのがありそう。
    // m_bQuest123、のように個別に変数を用意してしまったほうが楽かもしれない。
    // 不細工ではあるが。

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

    std::string m_strDead = "res\\sound\\dead.wav";

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

    void InvestigateCurrentStatus();
    std::string SelectBGM();
};

// 環境音のモデル
class BGMEnvModel
{
public:
    BGMEnvModel();
    void Update();
    void InvestigateBGMEnv();

    std::vector<envBGM> GetChangeRequest();
    void SetChangeRequestComplete();

private:

    std::unordered_map<std::string, envBGM> m_envBgmMap;

    bool m_bTorch = false;
    bool m_bSea = false;
    bool m_bForest = false;
    bool m_bRain = false;

    std::string m_strTorch = "res\\sound\\torch.wav";
    std::string m_strSea = "res\\sound\\sea.wav";
    std::string m_strForest = "res\\sound\\forest.wav";
    std::string m_strRain = "res\\sound\\rain.wav";
};

class BGM
{
public:
    void Init(HWND hwnd);
    void Finalize(); // For memory leak check.

    bool Load(const std::string& filename);
    void Play(const std::string& filename, const int a_volume = 100, const bool fadeIn = false);
    void Stop(const std::string& filename);

private:

    bool OpenWave(
        const std::string& filepath,
        WAVEFORMATEX& waveFormatEx,
        std::vector<char>* ppData,
        DWORD& dataSize);

    int PerToDecimal(const int percent);

    LPDIRECTSOUND8 dx8sound_ { nullptr };
    std::unordered_map<std::string, LPDIRECTSOUNDBUFFER8> dx8sound_buffers_ { };

    std::thread* m_th1 = nullptr;
    std::thread* m_th2 = nullptr;
    bool m_cancel1 = false;
    bool m_cancel2 = false;
};

class BGMManager
{
public:
    static BGMManager* Get();
    void Init(HWND hWnd);
    void Finalize();
    void Update();

private:
    static BGMManager* m_obj;

    BGMManager();

    BGMModel m_BGMModel;
    BGMEnvModel m_BGMEnvModel;
    BGM m_BGM;
};

