#include <cassert>

#include "PopUp2.h"
#include "SharedObj.h"

PopUp2* PopUp2::m_singleTonObj = nullptr;

PopUp2* PopUp2::Get()
{
    return m_singleTonObj;
}

void PopUp2::Finalize()
{
    SAFE_DELETE(m_singleTonObj);
}

void PopUp2::SetText(const std::string& arg)
{
    m_textQ.push(arg);
    m_isShow = true;
}

void PopUp2::SetText(const UINT resID)
{
    SetText(Common::LoadString_(resID));
}

PopUp2::~PopUp2()
{
    delete m_font;
}

void PopUp2::Init(IPopUpFont* font)
{
    m_singleTonObj = NEW PopUp2();

    m_singleTonObj->m_font = font;
}

bool PopUp2::IsShow()
{
    return m_isShow;
}

void PopUp2::Cancel()
{
    m_isShow = false;

    // ‹ó‚É‚·‚é
    std::queue<std::string> work;
    std::swap(m_textQ, work);

    m_counter = 0;
    m_transparent = 0;
}

void PopUp2::Update()
{
    if (m_textQ.size() == 0)
    {
        return;
    }

    m_counter++;

    // 0 ~ 29
    if (0 <= m_counter && m_counter < FADE_IN)
    {
        m_transparent = m_counter * 255 / 30;
    }
    // 30 ~ 89
    else if ((FADE_IN <= m_counter) && (m_counter < (FADE_IN + SHOW)))
    {
        m_transparent = 255;
    }
    // 90 ~ 119
    else if (((FADE_IN + SHOW) <= m_counter) && (m_counter < (FADE_IN + SHOW + FADE_OUT)))
    {
        int work = 0;
        work = m_counter - (FADE_IN + SHOW);
        m_transparent = 255 - (work * 255 / 30);
    }
    // 120 ~
    else if ((FADE_IN + SHOW + FADE_OUT) <= m_counter)
    {
        m_counter = 0;
        m_transparent = 0;
        m_textQ.pop();
    }
}

void PopUp2::Render()
{
    if (m_textQ.size() >= 1)
    {
        m_font->Draw(m_textQ.front(), m_transparent);
    }
}

PopUpFont::PopUpFont(LPDIRECT3DDEVICE9 device, const bool bEnglish)
{
    LPDIRECT3DDEVICE9 D3DDevice = SharedObj::GetD3DDevice();
    HRESULT hresult = S_OK;

    if (!bEnglish)
    {
        hresult = D3DXCreateFont(D3DDevice,
                                 26,
                                 0,
                                 FW_NORMAL,
                                 1,
                                 false,
                                 SHIFTJIS_CHARSET,
                                 OUT_TT_ONLY_PRECIS,
                                 ANTIALIASED_QUALITY,
                                 FF_DONTCARE,
                                 "‚l‚r –¾’©",
                                 &m_D3DFont);
    }
    else
    {
        hresult = D3DXCreateFont(D3DDevice,
                                 26,
                                 0,
                                 FW_NORMAL,
                                 1,
                                 false,
                                 DEFAULT_CHARSET,
                                 OUT_TT_ONLY_PRECIS,
                                 CLEARTYPE_NATURAL_QUALITY,
                                 FF_DONTCARE,
                                 "Calibri",
                                 &m_D3DFont);
    }

    assert(hresult == S_OK);
}

void PopUpFont::Draw(const std::string& text, const int transparent)
{
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	RECT rect;
	rect.top = 700;
	rect.bottom = 700 + 100;
	rect.left = 0;
	rect.right = 1600;

	m_D3DFont->DrawText(NULL,
                        text.c_str(),
                        -1,
                        &rect,
                        DT_CENTER | DT_VCENTER,
                        D3DCOLOR_ARGB(transparent, 255, 255, 255));
}

PopUpFont::~PopUpFont()
{
    SAFE_RELEASE(m_D3DFont);
}

