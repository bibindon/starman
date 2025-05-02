#include "Opening.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Common.h"
#include "SharedObj.h"
#include "BGM.h"
#include "SoundEffect.h"
#include "Camera.h"
#include <cassert>

using namespace NSStoryTelling;

namespace NSStoryTelling
{
class Sprite : public ISprite
{
public:

    Sprite(LPDIRECT3DDEVICE9 dev)
        : m_pD3DDevice(dev)
    {
    }

    void DrawImage(const int x, const int y, const int transparency) override
    {
        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width),
            static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
        m_D3DSprite->Draw(
            m_pD3DTexture,
            &rect,
            &center,
            &pos,
            D3DCOLOR_ARGB(transparency, 255, 255, 255));
        m_D3DSprite->End();

    }

    void Load(const std::string& filepath) override
    {
        LPD3DXSPRITE tempSprite { nullptr };
        if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
        {
            throw std::exception("Failed to create a sprite.");
        }

        if (FAILED(D3DXCreateTextureFromFile(
            m_pD3DDevice,
            filepath.c_str(),
            &m_pD3DTexture)))
        {
            throw std::exception("Failed to create a texture.");
        }

        D3DSURFACE_DESC desc { };
        if (FAILED(m_pD3DTexture->GetLevelDesc(0, &desc)))
        {
            throw std::exception("Failed to create a texture.");
        }
        m_width = desc.Width;
        m_height = desc.Height;
    }

    ISprite* Create()
    {
        return new Sprite(m_pD3DDevice);
    }

    ~Sprite()
    {
        SAFE_RELEASE(m_D3DSprite);
        SAFE_RELEASE(m_pD3DTexture);
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXSPRITE m_D3DSprite = NULL;
    LPDIRECT3DTEXTURE9 m_pD3DTexture = NULL;
    UINT m_width { 0 };
    UINT m_height { 0 };
};

class Font : public IFont
{
public:

    Font(LPDIRECT3DDEVICE9 pD3DDevice)
        : m_pD3DDevice(pD3DDevice)
    {
    }

    void Init(const bool bEnglish)
    {
        HRESULT hr = S_OK;
        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                "�l�r ����",
                                &m_pFont);
        }
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                DEFAULT_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                "Courier New",
                                &m_pFont);
        }
        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::string& msg, const int x, const int y)
    {
        RECT rect = { x, y, 0, 0 };
        m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
                          D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    ~Font()
    {
        m_pFont->Release();
        m_pFont = nullptr;
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;
};

class SoundEffect : public ISoundEffect
{
    virtual void PlayMove() override
    {
        ::SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav");
    }
    virtual void Init() override
    {
        ::SoundEffect::get_ton()->load("res\\sound\\cursor_move.wav");
    }
};
}

Opening::Opening()
{
    BGM::get_ton()->load("res\\sound\\opening.wav");

    {
        ISoundEffect* pSE = NEW NSStoryTelling::SoundEffect();
        pSE->Init();

        ISprite* sprTextBack = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprTextBack->Load("res\\image\\textBack.png");

        ISprite* sprFade = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprFade->Load("res\\image\\black.png");

        IFont* pFont = NEW NSStoryTelling::Font(SharedObj::GetD3DDevice());

        std::vector<Page> pageList;
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening01.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening02.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("����Ƃ���Ƀp���[�G�b�O���Ƃ����f�����������B");
            vs.push_back("�p���[�G�b�O���ɂ͊C�Ƃ������̑嗤�A�����Ė����̓����������B");
            vs.push_back("���̓��̒��Ƀv�����^�����Ƃ��������������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening03.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�v�����^�����́A����]�̖؂̉��ň�l�̒j�����Q�Ă����B�Q�Ă����j���A�z�V�}���͕l�ӂŖڂ��o�܂����B");
            vs.push_back("�z�V�}���͓��������̌`�����Ă����B���̌`�����Ă��邤���ɑS�g���F�ł܂��ɐ��Ƃ��������ł������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u���A���ʁE�E�E�v");
            vs.push_back("�����ꐺ�������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening04.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.clear();
            vs.push_back("�z�V�}���̓o�C�N�Ńc�[�����O���y���ނ��ߑD�œ��̃I�v�e�B�v�����Ɉړ����������B");
            vs.push_back("�������A�i���c�A�[���������ߑ䕗�ł����s���ꂽ�B");
            vs.push_back("�����āA������{���{���������D�͋T�􂩂�Z�����N����A�D���X���₪�ē]�����Ă��܂����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�D���]�������Ƃ��A�D�͍̂ӂ��A�z�V�}���͑D�̊��I�ɒ͂܂����B���̂��ƐQ���ɂR�O���ԉ߂������B");
            vs.push_back("�Y�����ĂR�O���Ԍ�A���̎p���߂��Ɍ��ꂽ�B");
            vs.push_back("�z�V�}���͖��䖲���ɂȂ�A���I���I�[���ɂ��ē��܂ő����A���ɂ��ǂ蒅���ƕl�ӂɓ|�ꂱ�񂾁B�����āA���̂܂܈ӎ����������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }

        m_storyTelling = NEW StoryTelling();
        m_storyTelling->Init(pFont, pSE, sprTextBack, sprFade, pageList, SharedObj::IsEnglish());
    }
}

Opening::~Opening()
{
}

void Opening::Update(eBattleState* eState)
{
    if (m_storyTelling != nullptr)
    {
        if (GamePad::IsDown(eGamePadButtonType::A))
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_SPACE))
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (Mouse::IsDownLeft())
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (m_firstPage == false && m_bPlay == false)
        {
            BGM::get_ton()->StopAll();
            BGM::get_ton()->play("res\\sound\\opening.wav", 80, true);
            m_bPlay = true;
        }

        bool bStoryFinish = m_storyTelling->Update();
        if (bStoryFinish)
        {
            m_storyTelling->Finalize();
            delete m_storyTelling;
            m_storyTelling = nullptr;

            m_bStopBeforeCameraFade = true;
            m_stopBeforeCameraFade = 0;
        }
    }
    else if (m_bStopBeforeCameraFade)
    {
        ++m_stopBeforeCameraFade;
        if (m_stopBeforeCameraFade >= WAIT_MAX)
        {
            m_bStopBeforeCameraFade = false;
            m_bCameraFadeOut = true;
            m_cameraFadeOutCnt = 0;
            Camera::SetCameraMode(eCameraMode::TITLE_TO_BATTLE);
        }
    }
    else
    {
        ++m_cameraFadeOutCnt;
        if (m_cameraFadeOutCnt >= Camera::MOVE_COUNT_MAX)
        {
            *eState = eBattleState::NORMAL;
        }
    }
}

void Opening::Render()
{
    if (m_storyTelling != nullptr)
    {
        m_storyTelling->Render();
    }
}
