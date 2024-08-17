#include "Player.h"
#include "SoundEffect.h"
#include "Camera.h"

Player::Player()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
    AnimSetMap animSetMap;
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = true;
        animSetMap["Idle"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 1.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Walk"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 2.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Attack"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Damaged"] = animSetting;
    }
    m_AnimMesh2 = new AnimMesh("res\\model\\hoshiman\\hoshiman.x", b, rot, 0.5f, animSetMap);
    m_AnimMesh2->SetAnim("Idle");
    SoundEffect::get_ton()->load("res\\sound\\attack01.wav");

    m_spriteHP = new Sprite("res\\image\\hp_green_p.png");
    m_spriteHPBack = new Sprite("res\\image\\hp_black_p.png");
}

Player::~Player()
{
    SAFE_DELETE(m_spriteHP);
    SAFE_DELETE(m_spriteHPBack);
    SAFE_DELETE(m_AnimMesh2);
}

void Player::Update()
{
    if (m_bAttack)
    {
        ++m_attackTimeCounter;
    }
    if (m_attackTimeCounter >= 30)
    {
        m_attackTimeCounter = 0;
        m_bAttack = false;
    }
    if (m_bDamaged)
    {
        ++m_damagedTimeCounter;
    }
    if (m_damagedTimeCounter >= 30)
    {
        m_damagedTimeCounter = 0;
        m_bDamaged = false;
    }
}

void Player::Render()
{
    m_AnimMesh2->SetPos(m_pos);
    m_AnimMesh2->SetRotate(m_rotate);
    m_AnimMesh2->Render();

    POINT screenPos = Camera::GetScreenPos(m_pos);
    m_spriteHPBack->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 128, (FLOAT)screenPos.y, 0.f });
    m_spriteHP->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 128, (FLOAT)screenPos.y, 0.f },
        255,
        (m_HP*256/100));
}

void Player::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 Player::GetPos()
{
    return m_pos;
}

void Player::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}

D3DXVECTOR3 Player::GetRotate()
{
    return m_rotate;
}

bool Player::SetAttack()
{
    bool ret { false };
    if (m_bAttack == false && m_bDamaged == false)
    {
        SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
        m_AnimMesh2->SetAnim("Attack", 0.f);
        m_bAttack = true;
        ret = true;
    }
    return ret;
}

void Player::SetWalk()
{
    m_AnimMesh2->SetAnim("Walk");
}

void Player::SetDamaged()
{
    //if (!m_bDamaged)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh2->SetAnim("Damaged", 0.f);
        m_bDamaged = true;
    }
}

D3DXVECTOR3 Player::GetAttackPos()
{
    D3DXVECTOR3 pos { m_pos };
    D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
    norm.x = std::sin(m_rotate.y + D3DX_PI);
    norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));
    pos += norm * 2;
    return pos;
}

void Player::SetHP(const int hp)
{
    m_HP = hp;
}

int Player::GetHP()
{
    return m_HP;
}
