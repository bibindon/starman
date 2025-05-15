#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

#include "BGM.h"
#include "Common.h"

#include <mmsystem.h>
#include <thread>

using std::vector;
using std::string;

BGM* BGM::single_ton_;

void BGM::Update()
{
    m_model.Update();


    // BGM
    {
        bool bChanged = false;
        stBgm _stBgmPrev;
        stBgm _stBgm = m_model.GetBGM(&bChanged, &_stBgmPrev);

        // 停止されたなら停止
        if (!_stBgm.m_filename.empty())
        {
			if (!_stBgm.m_bPlay)
			{
				stop(_stBgm.m_filename);
			}

			// 音量変更されたか
			if (_stBgm.m_bChangedVolume)
			{
				int volume = per_to_decibel(_stBgm.m_volume);
				dx8sound_buffers_.at(_stBgm.m_filename)->SetVolume(volume);
			}
        }

        // 別のBGMにかわったなら以前のBGMを停止
        if (bChanged)
        {
            if (!_stBgmPrev.m_filename.empty())
            {
				stop(_stBgmPrev.m_filename);
            }

            load(_stBgm.m_filename);
            play(_stBgm.m_filename, _stBgm.m_volume, true);
        }
    }

    // 環境音
    {
        auto envBgmMap = m_model.GetEnvBGM();

        for (auto& envBgm : envBgmMap)
        {
            if (envBgm.second.m_bChanged)
            {
                if (envBgm.second.m_bPlay)
                {
                    load(envBgm.second.m_filename);

                    // 効果音に対してフェードインをやるとBGMが鳴らなくなってしまう。
                    // 現状困っていないので放置
                    play(envBgm.second.m_filename, envBgm.second.m_volume, false);
                }
                else
                {
                    // 再生と停止を同時に行われると一度もplay関数が呼ばれずにstop関数が呼ばれることがある。
                    if (dx8sound_buffers_.find(envBgm.second.m_filename) != dx8sound_buffers_.end())
                    {
						stop(envBgm.second.m_filename);
                    }
                }
            }
        }
    }
}

BGM* BGM::Get()
{
    return single_ton_;
}

void BGM::Init(HWND hwnd)
{
    if (single_ton_ == nullptr)
    {
        single_ton_ = NEW BGM { hwnd };
    }
}

void BGM::Finalize()
{
    for (auto& pair : single_ton_->dx8sound_buffers_)
    {
        SAFE_RELEASE(pair.second);
    }

    SAFE_RELEASE(single_ton_->dx8sound_);
    SAFE_DELETE(single_ton_);
}

void BGM::Play(const std::string& filename, const int volume)
{
    m_model.SetBGM(filename, volume);
}

void BGM::Stop()
{
    m_model.StopBGM();
}

// Reference
// http://marupeke296.com/DSSMP_No2_GetSoundFromWave.html
bool BGM::load(const std::string& filename)
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
    if (!open_wave(filename, _waveformatex, &_wave_data, _wave_size))
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

void BGM::play(const string& filename, const int a_volume, const bool fadeIn)
{
    // Transform volume
    // 0 ~ 100 -> -10000 ~ 0
    int volume { 0 };
    volume = per_to_decibel(a_volume);
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
                                            int volume2 = per_to_decibel(a_volume * i / 30);
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
                                            int volume2 = per_to_decibel(a_volume * i / 30);
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

void BGM::stop(const string& filename)
{
    dx8sound_buffers_.at(filename)->Stop();
}

void BGM::PlayEnv(const std::string& filename, const int volume)
{
    m_model.SetEnvBGM(filename, volume);
}

void BGM::StopEnv(const std::string& filename)
{
    m_model.StopEnvBGM(filename);
}

void BGM::StopAll()
{
    m_model.StopAll();
}

void BGM::SetRandomMode(const bool mode)
{
    m_model.SetRandomMode(mode);
}

BGM::BGM(HWND hwnd)
{
    // Create sound device.
    LPDIRECTSOUND8 _dx8sound { nullptr };
    DirectSoundCreate8(nullptr, &_dx8sound, nullptr);
    SAFE_RELEASE(dx8sound_);
    dx8sound_ = _dx8sound;
    dx8sound_->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
}

bool BGM::open_wave(const std::string& filepath,
                    WAVEFORMATEX& waveformatex,
                    vector<char>* buff,
                    DWORD& wave_size)
{
    if (filepath.empty())
    {
        return false;
    }

    HMMIO _hmmio { nullptr };

    char* Name = NEW char[filepath.length() + 1];
    strcpy_s(Name, filepath.length() + 1, filepath.c_str());
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
int BGM::per_to_decibel(int percent)
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

void BGMModel::SetRandomMode(const bool mode)
{
    m_bRandomMode = mode;
}

// 1秒に1回呼ばれる想定
void BGMModel::Update()
{
    if (!m_bRandomMode)
    {
        return;
    }

    m_counter++;

    // 10分でBGM変更
    if (m_counter > 60 * 10)
    {
        m_counter = 0;
        m_stBgmPrev = m_stBgm;

        int rand_ = rand();
        if (rand_ % 2 == 0)
        {
            m_stBgm.m_filename = "res\\sound\\field.wav";
        }
        else if (rand_ % 2 == 1)
        {
            m_stBgm.m_filename = "res\\sound\\novel.wav";
        }

        if (m_stBgm.m_filename == m_stBgmPrev.m_filename)
        {
            m_bChanged = false;
        }
        else
        {
            m_bChanged = true;
        }
    }
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

void BGMModel::SetBGM(const std::string& bgmName, const int volume)
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

std::unordered_map<std::string, envBgm> BGMModel::GetEnvBGM()
{
    auto _copy = m_envBgmMap;

    for (auto& envBgm : m_envBgmMap)
    {
        envBgm.second.m_bChanged = false;
    }

    return _copy;
}

void BGMModel::SetEnvBGM(const std::string& bgmName, const int volume)
{
    if (!m_envBgmMap[bgmName].m_bPlay)
    {
        m_envBgmMap[bgmName].m_filename = bgmName;
        m_envBgmMap[bgmName].m_bChanged = true;
        m_envBgmMap[bgmName].m_bPlay = true;
        m_envBgmMap[bgmName].m_volume = volume;
    }
}

void BGMModel::StopEnvBGM(const std::string& bgmName)
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
