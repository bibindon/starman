#pragma once

#include <dsound.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

//------------------------------------------------
// BGM�Ɗ����̓A���S���Y�����قȂ�̂ŕ����čl����K�v������B
// �EBGM����x�ɓ��ލĐ�����邱�Ƃ͂Ȃ��B
// �@�E���̂��߁ABGM���Z�b�g������ȑO�Đ����Ă���BGM�͒�~���Ă悢�B�i��~���Ȃ��Ƃ����Ȃ��j
// �E�����͈�x�ɓ��ވȏ�Đ��ł���B
// �EBGM�͎��X�Ȃ�ύX�������������ɂ��̕K�v�͂Ȃ��B
// �EBGM�Ɗ����������ɂȂ邱�Ƃ͖��Ȃ��B
// �E�X�N���v�g�̓s����Aenum���`�����Ɂu�t�@�C�����v�ŊǗ�����K�v������
// �EBGM�̉��ʂ������ύX���ꂽ�Ƃ��͍ŏ�����Đ�����̂ł͂Ȃ����ʂ̕ύX�����s����悤�ɂ���
//------------------------------------------------

struct stBgm
{
    std::string m_filename;
    bool m_bPlay = false;
    int m_volume = 0;

    // ���ʂ��ς������
    bool m_bChangedVolume = false;
};

struct envBgm
{
    std::string m_filename;
    bool m_bPlay = false;

    // �Đ��E��~�̐؂�ւ����s��ꂽ��
    bool m_bChanged = false;

    int m_volume = 0;

    // ���ʂ��ς������
    bool m_bChangedVolume = false;
};

// BGM�̃��W�b�N����
class BGMModel
{
public:
    BGMModel();

    // 10����1�xBGM��ύX���郂�[�h
    void SetRandomMode(const bool mode);

    // 1�b�Ɉ�x�Ă΂��z��
    // 10���o�߂����烉���_���Ő؂�ւ��B
    void Update();

    // �Đ�����BGM���擾
    // bChanged: �ŋ�BGM���ύX���ꂽ���B
    stBgm GetBGM(bool* bChanged, stBgm* bgmPrev);

    // BGM��ݒ�
    // �C�J�_�ɏ�������p��BGM�𗬂������̂ŁA�O����Z�b�g����͉̂\�ɂ���B
    // �������A10���o������C�J�_�ɏ���Ă��Ă�BGM�͕ς��B
    void SetBGM(const std::string& bgmType, const int volume);

    // BGM���~
    void StopBGM();

    // �����͈�x�ɕ����Đ��ł���B
    // �Ȃ̂Œ�~���Ȃ���Ί����͉��X�Ƒ����Ă���
    // ������10�������ɐ؂�ւ�����肵�Ȃ��B
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

    // BGM�Đ�����Play�����s������Đ�����BGM�͎~�܂�BStop�����s����K�v�͂Ȃ��B
    void Play(const std::string& filename, const int volume);
    void Stop();

    // �����Đ�����PlayEnv�����s���Ă��Đ����̊����͎~�܂�Ȃ��B
    // �~�߂����ꍇ��StopEnv�����s����B
    void PlayEnv(const std::string& filename, const int volume = 100);
    void StopEnv(const std::string& filename);

    void StopAll();

    // 10����1�xBGM��ύX���郂�[�h
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

