#include <iostream>
#include "HeaderOnlyCsv.hpp"
#include <algorithm>

//---------------------------------------------------------------------------------
// prolitan.xを読み込み、木や石の配置データと敵の配置データをcsvで作成する。
//
// - 頂点データには何度も同じ点が出現する。
//   点は3つ以上のポリゴンで使われているので３回以上登場することになる。
//   例えば、正四面体はひとつの頂点が３つの三角形から共有されている。
//   これらの重複を排除する
//
// - 頂点にランダムで木や石、敵を割り当てる。同一の頂点に二つ以上割り当てられないようにする
//
// - 木や石、敵が割り当てられる確率を設定できるようにする
//
// - すべての頂点にどれかを割り当てる必要はない。
//   100％だったら全部の頂点に何かしらは割り当てる、
//   50％だったら半分の頂点に何かしらを割り当てる、という確立を設定できるようにする
//
// - 特定の位置ではこれが配置されてほしい、というのがあるのでそれをくみ取れるようにする。
//   例えば、バナナがたくさん取れるエリア、や特定のモンスターが現れるエリアなど。
// 
// - 100m x 100mで格子状に分割し、同じ種類のモンスターや同じ種類のオブジェクトが
//   まとまって配置されるようにする
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// 座標構造体
//---------------------------------------------------------------------------------
struct stPos;

//---------------------------------------------------------------------------------
// 分割関数
//---------------------------------------------------------------------------------
std::vector<std::string> split(const std::string& input, char delimiter);

//---------------------------------------------------------------------------------
// 読み込み関数
//---------------------------------------------------------------------------------
std::vector<stPos> ReadX(const std::string& xPath);

//---------------------------------------------------------------------------------
// 割り当て関数
//---------------------------------------------------------------------------------
void AssignContent(const std::vector<stPos>& posList,
                   std::vector<stPos>* mapObjList,
                   std::vector<stPos>* enemyList);

// 確率 (0% ~ 100%)

const int MapObj1 = 1;
const int MapObj2 = 1;
const int MapObj3 = 1;
const int MapObj4 = 1;
const int MapObj6 = 1;
const int MapObj7 = 1;
const int MapObj8 = 1;
const int MapObj9 = 1;
const int MapObj10 = 1;
const int MapObj11 = 1;
const int MapObj12 = 1;
const int Enemy1 = 1;
const int Enemy2 = 1;
const int Enemy3 = 1;
const int Enemy4 = 1;
const int Enemy5 = 1;
const int Enemy6 = 1;

//---------------------------------------------------------------------------------
// main関数
//---------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    auto posList = ReadX(argv[1]);

    std::vector<stPos> mapObjList;
    std::vector<stPos> enemyList;

    AssignContent(posList, &mapObjList, &enemyList);

    return 0;
}

struct stPos
{
    float x;
    float y;
    float z;

    // 比較演算子（ソート用）
    bool operator<(const stPos& other) const
    {
        if (x != other.x)
        {
            return x < other.x;
        }

        if (y != other.y)
        {
            return y < other.y;
        }

        return z < other.z;
    }

    // 同値判定（重複判定用）
    bool operator==(const stPos& other) const
    {
        if ((other.x - 0.1f) < x && x < (other.x + 0.1f))
        {
            if ((other.y - 0.1f) < y && y < (other.y + 0.1f))
            {
                if ((other.z - 0.1f) < z && z < (other.z + 0.1f))
                {
                    return true;
                }
            }
        }

        return false;
    }
};

std::vector<std::string> split(const std::string& input, char delimiter)
{
    std::vector<std::string> result;
    std::string current;

    for (char ch : input)
    {
        if (ch == delimiter)
        {
            result.push_back(current);
            current.clear();
        }
        else
        {
            current += ch;
        }
    }

    // 最後の要素も追加
    result.push_back(current);

    return result;
}

std::vector<stPos> ReadX(const std::string& xPath)
{
    std::ifstream ifs(xPath);
    if (!ifs.is_open())
    {
        throw std::exception("Failed to open x file.");
    }

    int startIndex = 0;
    std::vector<std::string> vertexLine;

    {
        std::string line;
        bool vertexStart = false;
        while (std::getline(ifs, line))
        {
            // "    Mesh { // Grid mesh"の次の次の行からが頂点データ。
            if (line == "    Mesh { // Grid mesh")
            {
                std::getline(ifs, line);
                std::getline(ifs, line);
                startIndex++;
                startIndex++;
                vertexStart = true;
                continue;
            }

            // 「,,」が登場したら頂点データ終了
            if (vertexStart && line.find(";;") != std::string::npos)
            {
                vertexLine.push_back(line);
                break;
            }

            if (vertexStart)
            {
                vertexLine.push_back(line);
            }
            else
            {
                startIndex++;
            }
        }
    }

    //----------------------------------------------------------
    // 頂点データは以下のようになっている
    // 
    // 72.274902;1602.057251;-0.613953;,
    // 72.274902;1602.057251;-0.613953;,
    // 141.719360;1602.057251;-0.613953;,
    // 141.719360;1671.501709;-0.613953;,
    // 141.719360;1671.501709;-0.613953;,
    //
    // 末尾の文字を削除してからセミコロンで分割すればよい。
    //----------------------------------------------------------

    std::vector<stPos> posList;

    for (size_t i = 0; i < vertexLine.size(); ++i)
    {
        std::string line = vertexLine.at(i);
        line.pop_back();
        auto vs = split(line, ';');
        stPos _stPos;
        _stPos.x = std::stof(vs.at(0));
        _stPos.y = std::stof(vs.at(2));
        _stPos.z = std::stof(vs.at(1));
        posList.push_back(_stPos);
    }

    // 重複排除
    std::sort(posList.begin(), posList.end());
    posList.erase(std::unique(posList.begin(), posList.end()), posList.end());

    return posList;
}

void AssignContent(const std::vector<stPos>& posList,
                   std::vector<stPos>* mapObjList,
                   std::vector<stPos>* enemyList)
{


}

