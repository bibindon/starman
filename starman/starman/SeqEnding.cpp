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
                                "ＭＳ 明朝",
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
            vs.push_back("数時間後、ホシマンは目を覚まし砂浜から内陸の方に歩いていった。");
            vs.push_back("しばらくして道路を見つけた。");
            vs.push_back("道路を歩き続けるとガソリンスタンドがあった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("そこでホシマンは今いる場所がジーフォーク共和国であることを確認した。");
            vs.push_back("北西に進んでいたつもりだったが南に相当ずれていた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("その日からホシマンは法律を無視して救助を最優先することにした。");
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
            vs.push_back("３か月後、８月３１日。");
            vs.push_back("ホシマンは深夜の海岸にいた。");
            vs.push_back("ホシマンはラデオウ連邦に密入国していた。政府に頼るわけにはいかなかった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("プロリタン島は国家的な犯罪の舞台となっているため、政府が救助に協力するか分からなかった。");
            vs.push_back("それどころかホシマンが暗殺されてしまう可能性もあるとホシマンは考えた。");
            vs.push_back("島には危険人物が二人いるため急いで救出に向かう必要もあった。自分で向かうしかなかった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("モーターボートに乗るための免許は二日でとることができ、誰でも受かるようだった。");
            vs.push_back("しかし陸から９ｋｍしか運転できなかった。");
            vs.push_back("ラデオウ連邦からプロリタン島までは１４０ｋｍあり、個人が合法的に行くことはほぼ不可能だった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("小型船舶の免許を取得するにもホシマンは密入国者だった。");
            vs.push_back("ホシマンは借金をして中古のモーターボートをネットで購入した。");
            vs.push_back("そして自家用車にボート取付用の器具を取り付け、モーターボートを取り付けた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("さらに海上でも現在地を確認できるGPS受信機を購入した。");
            vs.push_back("日に日に無人島での生活はホシマンにとって過去の記憶となっていき、");
            vs.push_back("記憶の中のダイケイマンとサンカクマンの声、形が曖昧となっていた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("今も二人はプロリタン島にいる。命がけの逃亡生活をしているかもしれない。");
            vs.push_back("どちらかは悪魔と契約しているかもしれない。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("決意が薄れ、このまま二人の救出をやめて普通の生活をし始めてしまうのではないか、");
            vs.push_back("そのことがホシマンは恐怖だった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("そして、８月３１日まで息を潜め、８月３１日に海岸までやってきた。");
            vs.push_back("最悪、二人を救出した後なら、警察に捕まっても良い。警察に捕まれば助かるからだ。");
            vs.push_back("ホシマンは郵便ポストに封筒を数枚いれた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("封筒のあて先は存在しない住所であり、送り主が警察やマスコミとなっていた。");
            vs.push_back("存在しない住所が書かれた郵便物は送り主のもとへ帰ってくる。");
            vs.push_back("そして、送り主のもとへ帰ってくるのは１～２週間かかる。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ホシマン単独による救助が失敗し、死亡した場合、プロリタン島に生存者がいるということを誰も知らなくなってしまう。");
            vs.push_back("かと言って、警察に頼れば救助自体ができなくなる可能性があった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ホシマンが救助に失敗した場合、警察に郵便物が届き、");
            vs.push_back("ホシマンが死亡した場合でもプロリタン島に生存者いるという情報が残せる、という作戦だった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("郵便物には、生存者がいるということだけが書いてあり、差出人や生存者の情報は載せていなかった。");
            vs.push_back("もし救出に成功すればそのままで問題なかった。");
            vs.push_back("ホシマンは意を決してプロリタン島に向かって出発した。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("何もない真っ暗な海をモーターボートで進んだ。モーターボートの最高速度は時速２０ｋｍ程度。");
            vs.push_back("時々、GPS受信機で現在地を確認した。");
            vs.push_back("イカダで航海したときとは打って変わって危なげなく目的地まで到達できそうだった。");
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
            vs.push_back("翌朝、ホシマンはダイレクトネクス海岸洞窟の辺りに到着した。");
            vs.push_back("ボートで海岸洞窟に近づくと人の姿が見えた。");
            vs.push_back("サンカクマンだった。サンカクマンはこちらに気づき手を振った。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("サンカクマンはそのあと、一度、奥に引っ込み、そのあと、ダイケイマンが杖を突きながら出てきた。");
            vs.push_back("ホシマンは海岸洞窟に到着すると、ダイケイマンたちに手を振ってから、水上ボートをロープで岩にくくりつけた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「それが流れて行っちゃったら大変だよ」");
            vs.push_back("ダイケイマンは笑いながら言った。");
            vs.push_back("「ははは」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「冗談よしてくれ」");
            vs.push_back("ホシマンは海から上がり砂浜に到着した。ダイケイマンとサンカクマンも砂浜にやってきた。");
            vs.push_back("「よかった・・・ほんとうによかったよ・・・」");
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
            vs.push_back("「待ってたぜー！」");
            vs.push_back("「歩けるようになったんだな」");
            vs.push_back("「おー、杖があればね」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「・・・。");
            vs.push_back("　あいつらは来なかったか？」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「来たよ。両方とも餓死寸前の状態でな。ガインの方は夜中に泥棒に来た。");
            vs.push_back("  普通に『食べ物をくれ』って言ってくれればいいのに。");
            vs.push_back("  こっちの姿に気づいたら逃げて行った。そのあとは会ってない。」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「ネスの方は、まぁ・・・いつもの感じで脅迫してきたから追い返した。二人がどうなってるかはわからない」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「わかった。すまないが警察が来るかもしれないから急いで帰ろう。一般人がここに来るのは違法なんだ」");
            vs.push_back("「お、　おーけー」");
            vs.push_back("「シカクマンの墓、見ていくか？」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「・・・。");
            vs.push_back("　あ、あぁ・・・」");
            vs.push_back("海岸洞窟を通り過ぎ、内陸の方に行くと、以前と変わらない姿でシカクマンの墓があった。");
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
            vs.push_back("ホシマンは墓の前で正座し、手を合わせて目をつぶった。");
            vs.push_back("「「「・・・」」」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「じゃあ・・・、行こう」");
            vs.push_back("ホシマンは立ち上がり、３人でモーターボートのところに向かった。");
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
            vs.push_back("モーターボートの中にはタンクが８個あった。");
            vs.push_back("「船は、実は燃費がすごく悪いんだ。１リットルで２Ｋｍくらいしか進めない。");
            vs.push_back("  １６０リットル持ってきた。ガソリン入れるから待っててくれ」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("モーターボート内のガソリンの残量は１割以下、すでにガソリン携行缶は一つが空になっていた。");
            vs.push_back("ホシマンは２０リットルガソリン携行缶を３つ使いガソリンを満タンにした。");
            vs.push_back("「帰りは、乗員が３人で海流が進行方向と逆向きになる。それを加味すると８０リットルくらいで足りるはずだ」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「わかった」");
            vs.push_back("「乗ってくれ。１０時間くらいかかる。食料と水はボートの中に用意してある」");
            vs.push_back("３人はモーターボートに乗った。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「久しぶりのお菓子だ」");
            vs.push_back("「小さくて軽くてカロリーの高いものにしたから、お菓子ばかりだ。好きなだけ食べてくれ」");
            vs.push_back("ガソリンの匂いが充満する。鍵を回すとエンジンの力強い音がした。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("（たのむ・・・このまま無事帰らせてくれ・・・）");
            vs.push_back("縄を外し、ホシマンたちは出発した。");
            vs.push_back("ホシマンはGPS装置を確認しながら進んだ。");
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

            vs.push_back("「うぉ～すげぇ～」");
            vs.push_back("「１年前に船に乗った時を思い出すなぁ～」");
            vs.push_back("「・・・」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ダイケイマンとサンカクマンは潮風を堪能していた。後ろを見るとすでに遠くなっている島がみえた。");
            vs.push_back("サンカクマンは海岸に角の生えた人が立っていることに気づいた。サンカクマンは手を振った。");
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
            vs.push_back("９時間後、３人はラデオウ連邦の本土に到着した。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("３人はホシマンの家で一泊することにした。");
            vs.push_back("ホシマンの家に到着したあとサンカクマンは母方の祖父母の家に電話した。");
            vs.push_back("迎えに来てもらうことになった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("そこでサンカクマンの両親が１年前の海難事故によって亡くなっていることを知った。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("翌日、ホシマンの家に老夫婦が車でやってきた。サンカクマンは老夫婦と共に帰っていった。");
            vs.push_back("それから、ホシマンはダイケイマンを連れて病院に行った。");
            vs.push_back("そこで去年の海難事故の生存者が帰還している、ということが明るみになった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ホシマンたちは警察の事情聴取を受けることになった。");
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
            vs.push_back("１年後、サンカクマンの家の近くの喫茶店に３人が集まった。");
            vs.push_back("あれからSNSで連絡を取り合っていたが久しぶりに会うことにしたのであった。");
            vs.push_back("１年前、ラデオウ連邦に帰還したとき、ホシマンは警察に逮捕された。そして事情聴取を受けた後、釈放となった。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("オプティプル島へ向かう船が沈没した本当の原因は、");
            vs.push_back("船の修繕費として政府から送られていた助成金を公務員が懐に入れていたことだった。");
            vs.push_back("船は修理されずに修理されたことになっていた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ラデオウ連邦ではジャーナリストが政府によって暗殺されることが日常だったため");
            vs.push_back("ホシマンたちの帰還は小さく報じられた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("また、高度に文明が発展した情報化社会において、");
            vs.push_back("『無人島からの生還』というニュースは毎秒追加され続けるネットニュースによって、すぐに埋もれた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ホシマンは借金を返済するため、倉庫で働くようになっていた。");
            vs.push_back("ダイケイマンは複数回に及ぶ骨の整形手術を受け、ついに歩けるようになっていた。");
            vs.push_back("サンカクマンは母方の祖父母の家に住むことになり小学６年生となっていた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("喫茶店に集まった３人はメニューを見ていた。");
            vs.push_back("「これを注文するしかね～な～」");
            vs.push_back("ダイケイマンはメニューの写真を見せた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("サンカクマンとホシマンが顔を近づけると、写真のメニューはパエリアだった。");
            vs.push_back("「結局、あの黒い貝は本当にムール貝だったらしい」");
            vs.push_back("ホシマンが言った。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「味比べしてみよう。パエリアのさ、ご飯の中に貝殻とかエビの殻が入ってるの、ちょっと嫌だったんだよね」");
            vs.push_back("「今は？」");
            vs.push_back("「うーん・・・」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「無人島生活で泥まみれのものをあんなに食べてたのに・・・」");
            vs.push_back("「そうなんだけどなぁ～」");
            vs.push_back("やがてパエリア、ナタデココ入りのデザート、フルーツが運ばれてきた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「『なんでこいつら果物ばっかり注文するんだろう』って思われてそう」");
            vs.push_back("ダイケイマンが言った。");
            vs.push_back("３人はムール貝を食べた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「同じだ～・・・、まーでも、無人島で食べたやつには叶わないね～」");
            vs.push_back("サンカクマンは得意げに言った。");
            vs.push_back("ホシマンはあの日のことを思い出していた。");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「徹夜で森を歩いて、村役場から海岸洞窟に行ったことがあった。");
            vs.push_back("　あの日は、夜に海岸洞窟について、それで焚き火を４人で囲んで、");
            vs.push_back("　この貝を食べながらカードゲームをした。懐かしいな」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「・・・。");
            vs.push_back("　次集まるときは３人で砂浜に行くか？」");
            vs.push_back("「行きたい！」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("「・・・」");
            vs.push_back("３人はお互いの顔を見た。");
            vs.push_back("「決まりだな」");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ホシマンは言った。");
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
