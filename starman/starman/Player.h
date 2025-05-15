#pragma once

#include <d3d9.h>
#include "AnimMesh.h"
#include "Sprite.h"
#include "Map.h"

enum class eDir
{
    LEFT,
    RIGHT,
    FRONT,
    BACK,
};

class Player
{
public:
    Player();
    ~Player();
    void Render();
    void Render2D();
    void Update(Map* stage1);
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos() const;
    void SetMove(const D3DXVECTOR3& move);
    D3DXVECTOR3 GetMove() const;
    void SetRotate(const D3DXVECTOR3& rotate);
    D3DXVECTOR3 GetRotate() const;
    bool SetAttack();
    bool SetAttackArrow();
    bool SetAttackAtlatl();
    void SetWalk();
    void SetIdle();
    void SetDamaged();

    void SetDead();
    bool GetDead() const;

    void SetSleep(const bool arg);

    D3DXVECTOR3 GetAttackPos() const;
    void SetJump();

    void SetSit();
    void SetLieDown();

    // ゲームパッドの場合でも3方向にしかステップさせない
    void SetStep(const eDir dir);
    void SetExamine();

    // イカダに乗る
    void RideRaft();

    // 洞窟の中にいるか。
    bool IsInCave();

    bool IsUnderWater();

private:
    AnimMesh* m_AnimMesh2 = nullptr;
    std::unordered_map<std::string, Mesh*> m_weaponMesh;
    std::unordered_map<NSStarmanLib::eBagPos, Mesh*> m_bagMesh;

    // なぜloadingPosという名前なのか謎
    D3DXVECTOR3 m_loadingPos = D3DXVECTOR3(0.f, 0.f, 0.f);
    D3DXVECTOR3 m_rotate = D3DXVECTOR3(0.f, D3DX_PI, 0.f);
    D3DXVECTOR3 m_dir = D3DXVECTOR3(0.f, 0.f, 1.f);

    // 移動ベクトル
    D3DXVECTOR3 m_move = D3DXVECTOR3(0.f, 0.f, 0.f);

    bool m_bAttack = false;
    int m_attackTimeCounter = 0;

    bool m_bDamaged = false;
    int m_damagedTimeCounter = 0;

    bool m_bJump = false;
    int m_jumpTimeCounter = 0;
    float m_jumpVelocity = 0.f;
    const float JUMP_INITIAL_VELOCITY = 0.25f;

    // 張り付きカウンター
    // どこでも0.1秒?張り付き状態を維持したらジャンプ可能になる
    int m_attachCount = 0;

    bool m_bJumpEnable = true;

    // 水中にいるか
    bool m_bUnderwater = false;

    //---------------------------------------------------
    // 投げる
    //---------------------------------------------------

    // 投げる
    void Throw();

    // 投げている
    bool m_bThrow = false;
    int m_throwTimeCounter = 0;

    // 投げている物
    int m_throwItemId = 0;

    //---------------------------------------------------
    // 魔法
    //---------------------------------------------------

    // 魔法実行
    void SetMagic();

    // 魔法実行中
    bool m_bMagic = false;
    int m_magicTimeCounter = 0;

    // 装備魔法を切り替えたら頭上に文字を表示し、フェードアウトさせる
    int m_switchMagicCounter = 0;
    bool m_bSwitchMagic = false;

    LPD3DXFONT m_D3DFont = nullptr;

    bool m_bInCave = false;

    // ステップ中
    bool m_bStep = false;
    int m_stepCounter = 0;
};
