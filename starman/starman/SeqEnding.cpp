#include "SeqEnding.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Common.h"
#include "SharedObj.h"
#include "BGM.h"
#include "SoundEffect.h"
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
        if (m_D3DSprite != nullptr)
        {
            m_D3DSprite->Release();
        }

        if (m_pD3DTexture != nullptr)
        {
            m_pD3DTexture->Release();
        }
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

SeqEnding::SeqEnding()
{
    BGM::get_ton()->load("res\\sound\\ending.wav");
//    BGM::get_ton()->play(10);

    {
        ISoundEffect* pSE = NEW NSStoryTelling::SoundEffect();

        ISprite* sprTextBack = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprTextBack->Load("res\\image\\textBack.png");

        ISprite* sprFade = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprFade->Load("res\\image\\black.png");

        IFont* pFont = NEW NSStoryTelling::Font(SharedObj::GetD3DDevice());

        // TODO
        std::vector<Page> pageList;
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending01.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�����Ԍ�A�z�V�}���͖ڂ��o�܂����l��������̕��ɕ����Ă������B");
            vs.push_back("���΂炭���ē��H���������B");
            vs.push_back("���H�����������ƃK�\�����X�^���h���������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����Ńz�V�}���͍�����ꏊ���W�[�t�H�[�N���a���ł��邱�Ƃ��m�F�����B");
            vs.push_back("�k���ɐi��ł������肾��������ɑ�������Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���̓�����z�V�}���͖@���𖳎����ċ~�����ŗD�悷�邱�Ƃɂ����B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending02.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�R������A�W���R�P���B");
            vs.push_back("�z�V�}���͐[��̊C�݂ɂ����B");
            vs.push_back("�z�V�}���̓��f�I�E�A�M�ɖ��������Ă����B���{�ɗ���킯�ɂ͂����Ȃ������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�v�����^�����͍��ƓI�Ȕƍ߂̕���ƂȂ��Ă��邽�߁A���{���~���ɋ��͂��邩������Ȃ������B");
            vs.push_back("����ǂ��납�z�V�}�����ÎE����Ă��܂��\��������ƃz�V�}���͍l�����B");
            vs.push_back("���ɂ͊댯�l������l���邽�ߋ}���ŋ~�o�Ɍ������K�v���������B�����Ō����������Ȃ������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���[�^�[�{�[�g�ɏ�邽�߂̖Ƌ��͓���łƂ邱�Ƃ��ł��A�N�ł��󂩂�悤�������B");
            vs.push_back("������������X���������^�]�ł��Ȃ������B");
            vs.push_back("���f�I�E�A�M����v�����^�����܂ł͂P�S�O��������A�l�����@�I�ɍs�����Ƃ͂قڕs�\�������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���^�D���̖Ƌ����擾����ɂ��z�V�}���͖������҂������B");
            vs.push_back("�z�V�}���͎؋������Ē��Ẫ��[�^�[�{�[�g���l�b�g�ōw�������B");
            vs.push_back("�����Ď��Ɨp�ԂɃ{�[�g��t�p�̊������t���A���[�^�[�{�[�g�����t�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("����ɊC��ł����ݒn���m�F�ł���GPS��M�@���w�������B");
            vs.push_back("���ɓ��ɖ��l���ł̐����̓z�V�}���ɂƂ��ĉߋ��̋L���ƂȂ��Ă����A");
            vs.push_back("�L���̒��̃_�C�P�C�}���ƃT���J�N�}���̐��A�`���B���ƂȂ��Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("������l�̓v�����^�����ɂ���B�������̓��S���������Ă��邩������Ȃ��B");
            vs.push_back("�ǂ��炩�͈����ƌ_�񂵂Ă��邩������Ȃ��B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���ӂ�����A���̂܂ܓ�l�̋~�o����߂ĕ��ʂ̐��������n�߂Ă��܂��̂ł͂Ȃ����A");
            vs.push_back("���̂��Ƃ��z�V�}���͋��|�������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����āA�W���R�P���܂ő�����߁A�W���R�P���ɊC�݂܂ł���Ă����B");
            vs.push_back("�ň��A��l���~�o������Ȃ�A�x�@�ɕ߂܂��Ă��ǂ��B�x�@�ɕ߂܂�Ώ����邩�炾�B");
            vs.push_back("�z�V�}���͗X�փ|�X�g�ɕ����𐔖����ꂽ�B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����̂��Đ�͑��݂��Ȃ��Z���ł���A����傪�x�@��}�X�R�~�ƂȂ��Ă����B");
            vs.push_back("���݂��Ȃ��Z���������ꂽ�X�֕��͑����̂��Ƃ֋A���Ă���B");
            vs.push_back("�����āA�����̂��Ƃ֋A���Ă���̂͂P�`�Q�T�Ԃ�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�z�V�}���P�Ƃɂ��~�������s���A���S�����ꍇ�A�v�����^�����ɐ����҂�����Ƃ������Ƃ�N���m��Ȃ��Ȃ��Ă��܂��B");
            vs.push_back("���ƌ����āA�x�@�ɗ���΋~�����̂��ł��Ȃ��Ȃ�\�����������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�z�V�}�����~���Ɏ��s�����ꍇ�A�x�@�ɗX�֕����͂��A");
            vs.push_back("�z�V�}�������S�����ꍇ�ł��v�����^�����ɐ����҂���Ƃ�����񂪎c����A�Ƃ�����킾�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�X�֕��ɂ́A�����҂�����Ƃ������Ƃ����������Ă���A���o�l�␶���҂̏��͍ڂ��Ă��Ȃ������B");
            vs.push_back("�����~�o�ɐ�������΂��̂܂܂Ŗ��Ȃ������B");
            vs.push_back("�z�V�}���͈ӂ������ăv�����^�����Ɍ������ďo�������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����Ȃ��^���ÂȊC�����[�^�[�{�[�g�Ői�񂾁B���[�^�[�{�[�g�̍ō����x�͎����Q�O�������x�B");
            vs.push_back("���X�AGPS��M�@�Ō��ݒn���m�F�����B");
            vs.push_back("�C�J�_�ōq�C�����Ƃ��Ƃ͑ł��ĕς���Ċ�Ȃ��Ȃ��ړI�n�܂œ��B�ł������������B");
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending03.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.clear();
            vs.push_back("�����A�z�V�}���̓_�C���N�g�l�N�X�C�ݓ��A�̕ӂ�ɓ��������B");
            vs.push_back("�{�[�g�ŊC�ݓ��A�ɋ߂Â��Ɛl�̎p���������B");
            vs.push_back("�T���J�N�}���������B�T���J�N�}���͂�����ɋC�Â����U�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�T���J�N�}���͂��̂��ƁA��x�A���Ɉ������݁A���̂��ƁA�_�C�P�C�}�������˂��Ȃ���o�Ă����B");
            vs.push_back("�z�V�}���͊C�ݓ��A�ɓ�������ƁA�_�C�P�C�}�������Ɏ��U���Ă���A����{�[�g�����[�v�Ŋ�ɂ���������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u���ꂪ����čs������������ς���v");
            vs.push_back("�_�C�P�C�}���͏΂��Ȃ��猾�����B");
            vs.push_back("�u�͂͂́v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u��k�悵�Ă���v");
            vs.push_back("�z�V�}���͊C����オ�荻�l�ɓ��������B�_�C�P�C�}���ƃT���J�N�}�������l�ɂ���Ă����B");
            vs.push_back("�u�悩�����E�E�E�ق�Ƃ��ɂ悩������E�E�E�v");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending04.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.clear();
            vs.push_back("�u�҂��Ă����[�I�v");
            vs.push_back("�u������悤�ɂȂ����񂾂ȁv");
            vs.push_back("�u���[�A�񂪂���΂ˁv");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�E�E�E�B");
            vs.push_back("�@������͗��Ȃ��������H�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u������B�����Ƃ��쎀���O�̏�ԂłȁB�K�C���̕��͖钆�ɓD�_�ɗ����B");
            vs.push_back("  ���ʂɁw�H�ו�������x���Č����Ă����΂����̂ɁB");
            vs.push_back("  �������̎p�ɋC�Â����瓦���čs�����B���̂��Ƃ͉���ĂȂ��B�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�l�X�̕��́A�܂��E�E�E�����̊����ŋ������Ă�������ǂ��Ԃ����B��l���ǂ��Ȃ��Ă邩�͂킩��Ȃ��v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�킩�����B���܂Ȃ����x�@�����邩������Ȃ�����}���ŋA�낤�B��ʐl�������ɗ���͈̂�@�Ȃ񂾁v");
            vs.push_back("�u���A�@���[���[�v");
            vs.push_back("�u�V�J�N�}���̕�A���Ă������H�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�E�E�E�B");
            vs.push_back("�@���A�����E�E�E�v");
            vs.push_back("�C�ݓ��A��ʂ�߂��A�����̕��ɍs���ƁA�ȑO�ƕς��Ȃ��p�ŃV�J�N�}���̕悪�������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending05.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�z�V�}���͕�̑O�Ő������A������킹�Ėڂ��Ԃ����B");
            vs.push_back("�u�u�u�E�E�E�v�v�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u���Ⴀ�E�E�E�A�s�����v");
            vs.push_back("�z�V�}���͗����オ��A�R�l�Ń��[�^�[�{�[�g�̂Ƃ���Ɍ��������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending06.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.clear();
            vs.push_back("���[�^�[�{�[�g�̒��ɂ̓^���N���W�������B");
            vs.push_back("�u�D�́A���͔R������������񂾁B�P���b�g���łQ�j�����炢�����i�߂Ȃ��B");
            vs.push_back("  �P�U�O���b�g�������Ă����B�K�\��������邩��҂��ĂĂ���v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���[�^�[�{�[�g���̃K�\�����̎c�ʂ͂P���ȉ��A���łɃK�\�����g�s�ʂ͈����ɂȂ��Ă����B");
            vs.push_back("�z�V�}���͂Q�O���b�g���K�\�����g�s�ʂ��R�g���K�\�����𖞃^���ɂ����B");
            vs.push_back("�u�A��́A������R�l�ŊC�����i�s�����Ƌt�����ɂȂ�B�������������ƂW�O���b�g�����炢�ő����͂����v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�킩�����v");
            vs.push_back("�u����Ă���B�P�O���Ԃ��炢������B�H���Ɛ��̓{�[�g�̒��ɗp�ӂ��Ă���v");
            vs.push_back("�R�l�̓��[�^�[�{�[�g�ɏ�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�v���Ԃ�̂��َq���v");
            vs.push_back("�u�������Čy���ăJ�����[�̍������̂ɂ�������A���َq�΂��肾�B�D���Ȃ����H�ׂĂ���v");
            vs.push_back("�K�\�����̓������[������B�����񂷂ƃG���W���̗͋������������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�i���̂ށE�E�E���̂܂ܖ����A�点�Ă���E�E�E�j");
            vs.push_back("����O���A�z�V�}�������͏o�������B");
            vs.push_back("�z�V�}����GPS���u���m�F���Ȃ���i�񂾁B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending07.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;

            vs.push_back("�u�����`�������`�v");
            vs.push_back("�u�P�N�O�ɑD�ɏ���������v���o���Ȃ��`�v");
            vs.push_back("�u�E�E�E�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�_�C�P�C�}���ƃT���J�N�}���͒��������\���Ă����B��������Ƃ��łɉ����Ȃ��Ă��铇���݂����B");
            vs.push_back("�T���J�N�}���͊C�݂Ɋp�̐������l�������Ă��邱�ƂɋC�Â����B�T���J�N�}���͎��U�����B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending08.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�X���Ԍ�A�R�l�̓��f�I�E�A�M�̖{�y�ɓ��������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�R�l�̓z�V�}���̉Ƃňꔑ���邱�Ƃɂ����B");
            vs.push_back("�z�V�}���̉Ƃɓ����������ƃT���J�N�}���͕���̑c����̉Ƃɓd�b�����B");
            vs.push_back("�}���ɗ��Ă��炤���ƂɂȂ����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����ŃT���J�N�}���̗��e���P�N�O�̊C��̂ɂ���ĖS���Ȃ��Ă��邱�Ƃ�m�����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�����A�z�V�}���̉ƂɘV�v�w���Ԃł���Ă����B�T���J�N�}���͘V�v�w�Ƌ��ɋA���Ă������B");
            vs.push_back("���ꂩ��A�z�V�}���̓_�C�P�C�}����A��ĕa�@�ɍs�����B");
            vs.push_back("�����ŋ��N�̊C��̂̐����҂��A�҂��Ă���A�Ƃ������Ƃ�����݂ɂȂ����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�z�V�}�������͌x�@�̎������󂯂邱�ƂɂȂ����B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending09.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;
            vs.push_back("�P�N��A�T���J�N�}���̉Ƃ̋߂��̋i���X�ɂR�l���W�܂����B");
            vs.push_back("���ꂩ��SNS�ŘA������荇���Ă������v���Ԃ�ɉ���Ƃɂ����̂ł������B");
            vs.push_back("�P�N�O�A���f�I�E�A�M�ɋA�҂����Ƃ��A�z�V�}���͌x�@�ɑߕ߂��ꂽ�B�����Ď������󂯂���A�ߕ��ƂȂ����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�I�v�e�B�v�����֌������D�����v�����{���̌����́A");
            vs.push_back("�D�̏C�U��Ƃ��Đ��{���瑗���Ă����������������������ɓ���Ă������Ƃ������B");
            vs.push_back("�D�͏C�����ꂸ�ɏC�����ꂽ���ƂɂȂ��Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("���f�I�E�A�M�ł̓W���[�i���X�g�����{�ɂ���ĈÎE����邱�Ƃ����킾��������");
            vs.push_back("�z�V�}�������̋A�҂͏������񂶂�ꂽ�B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�܂��A���x�ɕ��������W������񉻎Љ�ɂ����āA");
            vs.push_back("�w���l������̐��ҁx�Ƃ����j���[�X�͖��b�ǉ����ꑱ����l�b�g�j���[�X�ɂ���āA�����ɖ����ꂽ�B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�z�V�}���͎؋���ԍς��邽�߁A�q�ɂœ����悤�ɂȂ��Ă����B");
            vs.push_back("�_�C�P�C�}���͕�����ɋy�ԍ��̐��`��p���󂯁A���ɕ�����悤�ɂȂ��Ă����B");
            vs.push_back("�T���J�N�}���͕���̑c����̉ƂɏZ�ނ��ƂɂȂ菬�w�U�N���ƂȂ��Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�i���X�ɏW�܂����R�l�̓��j���[�����Ă����B");
            vs.push_back("�u����𒍕����邵���ˁ`�ȁ`�v");
            vs.push_back("�_�C�P�C�}���̓��j���[�̎ʐ^���������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�T���J�N�}���ƃz�V�}��������߂Â���ƁA�ʐ^�̃��j���[�̓p�G���A�������B");
            vs.push_back("�u���ǁA���̍����L�͖{���Ƀ��[���L�������炵���v");
            vs.push_back("�z�V�}�����������B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u����ׂ��Ă݂悤�B�p�G���A�̂��A���т̒��ɊL�k�Ƃ��G�r�̊k�������Ă�́A������ƌ��������񂾂�ˁv");
            vs.push_back("�u���́H�v");
            vs.push_back("�u���[��E�E�E�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u���l�������œD�܂݂�̂��̂�����ȂɐH�ׂĂ��̂ɁE�E�E�v");
            vs.push_back("�u�����Ȃ񂾂��ǂȂ��`�v");
            vs.push_back("�₪�ăp�G���A�A�i�^�f�R�R����̃f�U�[�g�A�t���[�c���^�΂�Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�w�Ȃ�ł�����ʕ��΂����蒍������񂾂낤�x���Ďv���Ă����v");
            vs.push_back("�_�C�P�C�}�����������B");
            vs.push_back("�R�l�̓��[���L��H�ׂ��B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�������`�E�E�E�A�܁[�ł��A���l���ŐH�ׂ���ɂ͊���Ȃ��ˁ`�v");
            vs.push_back("�T���J�N�}���͓��ӂ��Ɍ������B");
            vs.push_back("�z�V�}���͂��̓��̂��Ƃ��v���o���Ă����B");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�O��ŐX������āA�����ꂩ��C�ݓ��A�ɍs�������Ƃ��������B");
            vs.push_back("�@���̓��́A��ɊC�ݓ��A�ɂ��āA����ŕ����΂��S�l�ň͂�ŁA");
            vs.push_back("�@���̊L��H�ׂȂ���J�[�h�Q�[���������B���������ȁv");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�E�E�E�B");
            vs.push_back("�@���W�܂�Ƃ��͂R�l�ō��l�ɍs�����H�v");
            vs.push_back("�u�s�������I�v");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�u�E�E�E�v");
            vs.push_back("�R�l�͂��݂��̊�������B");
            vs.push_back("�u���܂肾�ȁv");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�z�V�}���͌������B");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }
        {
            Page page;
            ISprite* sprite = NEW NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\ending10.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vvs;
            std::vector<std::string> vs;

            vs.push_back("");
            vvs.push_back(vs);
            page.SetTextList(vvs);
            pageList.push_back(page);
        }

        m_storyTelling = NEW StoryTelling();
        m_storyTelling->Init(pFont, pSE, sprTextBack, sprFade, pageList, SharedObj::IsEnglish());
    }
}

SeqEnding::~SeqEnding()
{
}

void SeqEnding::Update(eSequence* sequence)
{
    if (m_storyTelling != nullptr)
    {
        if (GamePad::IsDown(eGamePadButtonType::A))
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
        {
            m_storyTelling->Next();
            m_firstPage = false;
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_SPACE))
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
            //BGM::get_ton()->play();
            m_bPlay = true;
        }

        bFinish = m_storyTelling->Update();
        if (bFinish)
        {
            m_storyTelling->Finalize();
            delete m_storyTelling;
            m_storyTelling = nullptr;
            *sequence = eSequence::BATTLE;
        }
    }
}

void SeqEnding::Render()
{
    if (m_storyTelling != nullptr)
    {
        m_storyTelling->Render();
    }
}
