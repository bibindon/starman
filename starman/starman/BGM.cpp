#pragma comment ( lib, "dxguid.lib")
#pragma comment ( lib, "dsound.lib")
#pragma comment ( lib, "winmm.lib")

#include "BGM.h"
#include "Common.h"

#include <mmsystem.h>
#include <thread>
#include "SharedObj.h"
#include "MainWindow.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"

using std::vector;
using std::wstring;

BGMManager* BGMManager::m_obj = nullptr;

void BGM::Init(HWND hwnd)
{
    // Create sound device.
    LPDIRECTSOUND8 _dx8sound { nullptr };
    DirectSoundCreate8(nullptr, &_dx8sound, nullptr);
    SAFE_RELEASE(dx8sound_);
    dx8sound_ = _dx8sound;
    dx8sound_->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
}

void BGM::Finalize()
{
    for (auto& pair : dx8sound_buffers_)
    {
        SAFE_RELEASE(pair.second);
    }

    SAFE_RELEASE(dx8sound_);
}

// Reference
// http://marupeke296.com/DSSMP_No2_GetSoundFromWave.html
bool BGM::Load(const std::wstring& filename)
{
    // Already loaded.
    if (dx8sound_buffers_.find(filename) != dx8sound_buffers_.end())
    {
        return true;
    }

    // Open wave file.
    WAVEFORMATEX _waveformatex { };
    vector<char> _wave_data;
    DWORD _wave_size { 0 };
    if (!OpenWave(filename, _waveformatex, &_wave_data, _wave_size))
    {
        return false;
    }

    DSBUFFERDESC DSBufferDesc { };
    DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
    DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    DSBufferDesc.dwBufferBytes = _wave_size;
    DSBufferDesc.dwReserved = 0;
    DSBufferDesc.lpwfxFormat = &_waveformatex;
    DSBufferDesc.guid3DAlgorithm = GUID_NULL;

    // Create secondary buffer.
    LPDIRECTSOUNDBUFFER _temp_buffer { nullptr };
    LPDIRECTSOUNDBUFFER8 _temp_buffer8 { nullptr };
    dx8sound_->CreateSoundBuffer(&DSBufferDesc, &_temp_buffer, nullptr);
    _temp_buffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_temp_buffer8);
    _temp_buffer->Release();
    if (_temp_buffer8 == 0)
    {
        return false;
    }
    LPDIRECTSOUNDBUFFER8 temp_shared_buff { _temp_buffer8 };
    dx8sound_buffers_[filename] = temp_shared_buff;

    // Write wave data to secondary buffer.
    LPVOID secondary_buffer { nullptr };
    DWORD _length { 0 };
    if (DS_OK == dx8sound_buffers_.at(filename)->Lock(
        0, 0, &secondary_buffer, &_length, nullptr, nullptr, DSBLOCK_ENTIREBUFFER))
    {
        memcpy(secondary_buffer, &_wave_data.at(0), _length);
        dx8sound_buffers_.at(filename)->Unlock(secondary_buffer, _length, nullptr, 0);
    }

    return true;
}

void BGM::Play(const wstring& filename, const int a_volume, const bool fadeIn)
{
    // Transform volume
    // 0 ~ 100 -> -10000 ~ 0
    int volume { 0 };
    volume = PerToDecimal(a_volume);
    dx8sound_buffers_.at(filename)->SetVolume(volume);
    dx8sound_buffers_.at(filename)->SetCurrentPosition(0);
    dx8sound_buffers_.at(filename)->Play(0, 0, DSBPLAY_LOOPING);

    if (fadeIn)
    {
        LPDIRECTSOUNDBUFFER8 soundBuffer = dx8sound_buffers_.at(filename);

        if (m_th1 == nullptr)
        {
            m_cancel1 = false;
            m_cancel2 = true;
            m_th2 = nullptr;
            m_th1 = new std::thread([=]
                                    {
                                        // 30回に分けて音量を0.1秒ごとに上げる
                                        for (int i = 0; i < 30; ++i)
                                        {
                                            int volume2 = PerToDecimal(a_volume * i / 30);
                                            soundBuffer->SetVolume(volume2);
                                            Sleep(100);
                                            if (this->m_cancel1)
                                            {
                                                break;
                                            }
                                        }
                                    });
        }
        else if (m_th2 == nullptr)
        {
            m_cancel1 = true;
            m_cancel2 = false;
            m_th1 = nullptr;
            m_th2 = new std::thread([=]
                                    {
                                        // 100回に分けて音量を0.1秒ごとに上げる
                                        for (int i = 0; i < 30; ++i)
                                        {
                                            int volume2 = PerToDecimal(a_volume * i / 30);
                                            soundBuffer->SetVolume(volume2);
                                            Sleep(100);
                                            if (this->m_cancel2)
                                            {
                                                break;
                                            }
                                        }
                                    });
        }
    }
}

void BGM::Stop(const wstring& filename)
{
    dx8sound_buffers_.at(filename)->Stop();
}

bool BGM::OpenWave(const std::wstring& filepath,
                    WAVEFORMATEX& waveformatex,
                    vector<char>* buff,
                    DWORD& wave_size)
{
    if (filepath.empty())
    {
        return false;
    }

    HMMIO _hmmio { nullptr };

    wchar_t* Name = NEW wchar_t[filepath.length() + 1];
    wcscpy_s(Name, filepath.length() + 1, filepath.c_str());
    _hmmio = mmioOpen(Name, nullptr, MMIO_READ);

    delete[] Name;
    if (_hmmio == nullptr)
    {
        return false;
    }

    // Search RIFF chunk.
    MMRESULT mmresult { 0 };
    MMCKINFO riff_chunk { };
    riff_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    mmresult = mmioDescend(_hmmio, &riff_chunk, NULL, MMIO_FINDRIFF);
    if (mmresult != MMSYSERR_NOERROR)
    {
        mmioClose(_hmmio, 0);
        return false;
    }

    // Search format chunk
    MMCKINFO format_chunk { };
    format_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmresult = mmioDescend(_hmmio, &format_chunk, &riff_chunk, MMIO_FINDCHUNK);
    if (mmresult != MMSYSERR_NOERROR)
    {
        mmioClose(_hmmio, 0);
        return false;
    }
    DWORD fmsize { format_chunk.cksize };
    DWORD size { (DWORD)mmioRead(_hmmio, (HPSTR)&waveformatex, fmsize) };
    if (size != fmsize)
    {
        mmioClose(_hmmio, 0);
        return false;
    }

    mmioAscend(_hmmio, &format_chunk, 0);

    // Search data chunk.
    MMCKINFO data_chunk { };
    data_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmresult = mmioDescend(_hmmio, &data_chunk, &riff_chunk, MMIO_FINDCHUNK);
    if (mmresult != MMSYSERR_NOERROR)
    {
        mmioClose(_hmmio, 0);
        return false;
    }
    buff->resize(data_chunk.cksize);
    size = mmioRead(_hmmio, (HPSTR)buff->data(), data_chunk.cksize);
    if (size != data_chunk.cksize)
    {
        return false;
    }
    wave_size = size;

    mmioClose(_hmmio, 0);

    return true;
}

// convert percent to decibel.
// https://katze.hatenablog.jp/entry/2013/07/01/000343
int BGM::PerToDecimal(int percent)
{
    int decibel { 0 };

    // 0 ` -600
    if (percent >= 50)
    {
        decibel = -600 + 600 * (percent - 50) / 50;
    }
    // -600 ` -1200
    else if (percent >= 25)
    {
        decibel = -1200 + 600 * (percent - 25) / 25;
    }
    // -1200 ` -1800
    else if (percent >= 12)
    {
        decibel = -1800 + 600 * (percent - 12) / 12;
    }
    // -1800 ` -2400
    else if (percent >= 6)
    {
        decibel = -2400 + 600 * (percent - 6) / 6;
    }
    // -2400 ` -3000
    else if (percent >= 3)
    {
        decibel = -3000 + 600 * (percent - 3) / 3;
    }
    // -3000`-3600
    else if (percent >= 1)
    {
        decibel = -3600 + 600 * (percent - 1);
    }
    else
    {
        decibel = DSBVOLUME_MIN;
    }
    return decibel;
}

BGMModel::BGMModel()
{
    srand((unsigned int)NULL);
}

// 1秒に1回呼ばれる想定
void BGMModel::Update()
{
    InvestigateCurrentStatus();

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
    std::string newBGM =  SelectBGM();

    if (newBGM.empty())
    {
        return;
    }
    else
    {
        m_stBgmPrev = m_stBgm;
        m_stBgm.m_filename = _T("res\\sound\\field1.wav");
        m_bChanged = true;
    }

    // ランダム選曲はいったん廃止
//    if (!m_bRandomMode)
//    {
//        return;
//    }
//
//    m_counter++;
//
//    // 10分でBGM変更
//    if (m_counter > 60 * 10)
//    {
//        m_counter = 0;
//        m_stBgmPrev = m_stBgm;
//
//        int rand_ = rand();
//        if (rand_ % 2 == 0)
//        {
//            m_stBgm.m_filename = _T("res\\sound\\field3.wav");
//        }
//        else if (rand_ % 2 == 1)
//        {
//            m_stBgm.m_filename = _T("res\\sound\\field1.wav");
//        }
//
//        if (m_stBgm.m_filename == m_stBgmPrev.m_filename)
//        {
//            m_bChanged = false;
//        }
//        else
//        {
//            m_bChanged = true;
//        }
//    }
}

stBgm BGMModel::GetBGM(bool* bChanged, stBgm* bgmPrev)
{
    if (m_bChanged)
    {
        *bChanged = true;
        m_bChanged = false;
        *bgmPrev = m_stBgmPrev;
    }
    else
    {
        *bChanged = false;
    }

    return m_stBgm;
}

void BGMModel::SetBGM(const std::wstring& bgmName, const int volume)
{
    if (bgmName == m_stBgm.m_filename)
    {
        m_stBgm.m_volume = volume;

        m_stBgm.m_bChangedVolume = true;
    }
    else
    {
        m_counter = 0;
        m_stBgmPrev = m_stBgm;
        m_stBgm.m_filename = bgmName;
        m_stBgm.m_volume = volume;
        m_stBgm.m_bPlay = true;

        m_bChanged = true;
    }
}

void BGMModel::StopBGM()
{
    if (m_stBgm.m_bPlay)
    {
        m_stBgm.m_bPlay = false;
        m_bChanged = true;
    }
}

std::unordered_map<std::wstring, envBgm> BGMModel::GetEnvBGM()
{
    auto _copy = m_envBgmMap;

    for (auto& envBgm : m_envBgmMap)
    {
        envBgm.second.m_bChanged = false;
    }

    return _copy;
}

void BGMModel::SetEnvBGM(const std::wstring& bgmName, const int volume)
{
    if (!m_envBgmMap[bgmName].m_bPlay)
    {
        m_envBgmMap[bgmName].m_filename = bgmName;
        m_envBgmMap[bgmName].m_bChanged = true;
        m_envBgmMap[bgmName].m_bPlay = true;
        m_envBgmMap[bgmName].m_volume = volume;
    }
}

void BGMModel::StopEnvBGM(const std::wstring& bgmName)
{
    if (m_envBgmMap[bgmName].m_bPlay)
    {
        m_envBgmMap[bgmName].m_bChanged = true;
        m_envBgmMap[bgmName].m_bPlay = false;
    }
}

void BGMModel::StopAll()
{
    StopBGM();

    for (auto& envBgm : m_envBgmMap)
    {
        StopEnvBGM(envBgm.first);
    }
}

void BGMModel::InvestigateCurrentStatus()
{
    //------------------------------------------
    // 死亡
    //------------------------------------------
    if (Common::Status()->GetDead())
    {
        m_bDead = true;
    }
    else
    {
        m_bDead = false;
    }

    auto seq = MainWindow::GetBattleSequence();
    if (seq != nullptr)
    {
        //------------------------------------------
        // タイトル
        //------------------------------------------
        if (seq->GetState() == eBattleState::TITLE)
        {
            m_bTitle = true;
        }
        else
        {
            m_bTitle = false;
        }

        //------------------------------------------
        // オープニング
        //------------------------------------------
        if (seq->GetState() == eBattleState::OPENING)
        {
            m_bOpening = true;
        }
        else
        {
            m_bOpening = false;
        }
    }

    //------------------------------------------
    // エンディング
    //------------------------------------------
    // TODO bEnding

    //------------------------------------------
    // 戦闘
    //------------------------------------------
    // TODO bBattle
    // 攻撃したらバトル開始
    // 30秒攻撃しなかったらバトル終了

    //------------------------------------------
    // 瀕死
    //------------------------------------------
    if (Common::Status()->GetMuscleCurrent() < 10.f ||
        Common::Status()->GetWaterCurrent() < 92.f ||
        (Common::Status()->GetCarboCurrent() < 10.f && Common::Status()->GetLipidCurrent() < 10.f))
    {
        m_bDying = true;
    }
    else
    {
        m_bDying = false;
    }

    //------------------------------------------
    // 体力少な目
    //------------------------------------------
    if (Common::Status()->GetMuscleCurrent() < 50.f ||
        Common::Status()->GetWaterCurrent() < 95.f ||
        (Common::Status()->GetCarboCurrent() < 50.f && Common::Status()->GetLipidCurrent() < 10.f))
    {
        m_bWeak = true;
    }
    else
    {
        m_bWeak = false;
    }

    //------------------------------------------
    // 夜
    //------------------------------------------
    auto datetime = NSStarmanLib::PowereggDateTime::GetObj();

    if (6 < datetime->GetHour() && datetime->GetHour() < 18)
    {
        m_bNight = false;
    }
    else
    {
        m_bNight = true;
    }

    //------------------------------------------
    // 航海
    //------------------------------------------
    auto voyage = VoyageManager::Get();
    if (voyage->GetRaftMode())
    {
        m_bVoyage = true;
    }
    else
    {
        m_bVoyage = false;
    }

    //------------------------------------------
    // 廃墟の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos{ 10.6f, 491.5f, -564.f };
        auto nearHaikyo = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearHaikyo)
        {
            m_bHaikyo = true;
        }
        else
        {
            m_bHaikyo = false;
        }
    }

    //------------------------------------------
    // 灯台の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos { -1321.f, 632.f, -1529.f };
        auto nearTarget = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearTarget)
        {
            m_bToudai = true;
        }
        else
        {
            m_bToudai = false;
        }
    }

    //------------------------------------------
    // 海岸洞窟の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos { 1092.0f, 40.8f, 504.1f };
        auto nearTarget = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearTarget)
        {
            m_bKaiganDoukutsu = true;
        }
        else
        {
            m_bKaiganDoukutsu = false;
        }
    }

    //------------------------------------------
    // 神社の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos { 15.8f, 492.8f, -1221.2f };
        auto nearTarget = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearTarget)
        {
            m_bJinja = true;
        }
        else
        {
            m_bJinja = false;
        }
    }

    //------------------------------------------
    // 苔庭の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos { 629.f, 773.9f, -1861.8f };
        auto nearTarget = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearTarget)
        {
            m_bJinja = true;
        }
        else
        {
            m_bJinja = false;
        }
    }

    //------------------------------------------
    // 港跡の近くにいる
    //------------------------------------------
    {
        auto ppos = SharedObj::GetPlayer()->GetPos();

        D3DXVECTOR3 targetPos { 1814.3f, 28.6f, -899.2f };
        auto nearTarget = Common::HitByBoundingBox(ppos, targetPos, 100.f);
        if (nearTarget)
        {
            m_bJinja = true;
        }
        else
        {
            m_bJinja = false;
        }
    }

    //------------------------------------------
    // 洞窟の近くにいる
    //------------------------------------------

    // TODO
}

// 変更ナシなら空文字
std::string BGMModel::SelectBGM()
{
    std::string newBGM;

    // 複数のフラグがONになることがあることに注意

    if (m_bDead)
    {
        newBGM = m_strDead;
    }
    else if (m_bTitle)
    {
        newBGM = m_strTitle;
    }
    else if (m_bOpening)
    {
        newBGM = m_strOpening;
    }
    else if (m_bEnding)
    {
        newBGM = m_strEnding;
    }
    else if (m_bEnding)
    {
        newBGM = m_strEnding;
    }
    else if (m_bBattle)
    {
        // 戦闘曲じゃないところから戦闘曲になったら
        // 戦闘曲１か戦闘曲２のどちらかを選曲する
        if (m_currentBGM == m_strBattle1 || m_currentBGM == m_strBattle2)
        {
            // Do nothing
        }
        else
        {
            auto rand_ = SharedObj::GetRandom() % 2;
            if (rand_ == 0)
            {
                newBGM = m_strBattle1;
            }
            else
            {
                newBGM = m_strBattle2;
            }
        }
    }
    else if (m_bDying)
    {
        newBGM = m_strDying;
    }
    else if (m_bWeak)
    {
        newBGM = m_strWeak;
    }
    else if (m_bNight)
    {
        newBGM = m_strNight;
    }
    else if (m_bVoyage)
    {
        newBGM = m_strVoyage;
    }
    else if (m_bHaikyo)
    {
        newBGM = m_strHaikyo;
    }
    else if (m_bToudai)
    {
        newBGM = m_strToudai;
    }
    else if (m_bKaiganDoukutsu)
    {
        newBGM = m_strKaiganDoukutsu;
    }
    else if (m_bJinja)
    {
        newBGM = m_strJinja;
    }
    else if (m_bKokeniwa)
    {
        newBGM = m_strKokeniwa;
    }
    else if (m_bMinatoAto)
    {
        newBGM = m_strMinatoAto;
    }
    else if (m_bDoukutsu)
    {
        newBGM = m_strDoukutsu;
    }
    // どれでもないなら通常曲をランダムで選曲
    else
    {
        // 通常曲じゃないところから通常曲になったら
        // 通常曲の中からどれかを選曲する
        if (m_currentBGM == m_strField1 ||
            m_currentBGM == m_strField2 ||
            m_currentBGM == m_strField3)
        {
            // Do nothing
        }
        else
        {
            auto rand_ = SharedObj::GetRandom() % 3;
            if (rand_ == 0)
            {
                newBGM = m_strField1;
            }
            else if (rand_ == 1)
            {
                newBGM = m_strField2;
            }
            else
            {
                newBGM = m_strField3;
            }
        }
    }

    if (m_currentBGM == newBGM)
    {
        return std::string();
    }

    return newBGM;
}

void BGMEnvModel::Update()
{

}

std::vector<std::string> BGMEnvModel::SelectBGM()
{
    // プレイヤーの標高が低かったら海の環境音
    // プレイヤーの標高が高かったら森の環境音
    // 松明持ってたら松明の環境音
    // 雨が降っていたら雨の環境音
    return std::vector<std::string>();

}

BGMManager* BGMManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = NEW BGMManager();
    }

    return m_obj;
}

BGMManager::BGMManager()
{
}

void BGMManager::Init(HWND hWnd)
{
    m_BGM.Init(hWnd);
}

void BGMManager::Finalize()
{
    m_BGM.Finalize();
}

void BGMManager::Update()
{
    m_BGMModel.Update();
    m_BGMEnvModel.Update();
}



