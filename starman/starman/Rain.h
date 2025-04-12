#pragma once

#include <array>

#include <d3dx9.h>

// �J�̐��H��`�悷�邱�Ƃ͂ł��邪�A����Â����邱�Ƃ͂ł��Ȃ��B
// �O���Â����鏈�������͗�O�I��Map�N���X�ɂ���Ă��炤�B
class Rain
{
public:

    static Rain* Get();
    void Finalize();
    void Update();
    void Draw();
    bool IsRain();

    // �^�C�g����ʂ�\�����Ă���A�ȂǁA���炩�̗��R�ŉJ�̕`������Ȃ��ł��������Ƃ��ɌĂԁB
    // �J���~���Ă���E���Ȃ����ƂƂ͊֌W�Ȃ��B
    void SetShow(const bool bShow);

private:

    void Init();

    static Rain* m_obj;
    LPD3DXSPRITE m_sprite = NULL;
    LPDIRECT3DTEXTURE9 m_texture = NULL;

    static constexpr int RAINDROP = 6;
    std::array<D3DXVECTOR3, RAINDROP> m_pos;

    bool m_bShow = false;
    bool m_bPreviousRain = false;
};

