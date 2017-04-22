#pragma once
#include "GameState.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

enum Direction
{
	MOVE_UP,				//上方向
	MOVE_DOWN,			//下方向
	MOVE_RIGHT,			//右方向
	MOVE_LEFT,			//左方向
};

//================================================================
//クラス名  ：Characterクラス
//基底クラス：
//
//内容：このクラスをユニットの基底クラスとして派生させていく
//================================================================

class Character
{
protected:

public:
	//コンストラクタ
	Character();
	//デストラクタ
	~Character();

	//キャラクターの座標
	int x;
	int y;

	//キャラクターの生存フラグ
	bool AliveFlag;

	//攻撃しているように見せるためのユニットの表示場所の変数
	int AttackPosX;
	int AttackPosY;

	//移動中かのフラグ
	bool MoveFlag;
};


//================================================================
//クラス名  ：Mikenekoクラス
//基底クラス：Characterクラス
//
//内容：Characterから派生させたクラス。三毛猫のクラス
//================================================================
class Mikeneko :public Character
{
private:
	//三毛猫のテクスチャ
	Texture MikenekoTex;
	Sprite MikenekoSprite;

public:
	//コンストラクタ
	Mikeneko();
	//デストラクタ
	~Mikeneko();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();
};

//================================================================
//クラス名  ：Kuronekoクラス
//基底クラス：Characterクラス
//
//内容：Characterから派生させたクラス。黒猫のクラス
//================================================================
class Kuroneko :public Character
{
private:
	//黒猫のテクスチャ
	Texture KuronekoTex;
	Sprite KuronekoSprite;

public:
	//コンストラクタ
	Kuroneko();
	//デストラクタ
	~Kuroneko();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();
};

//================================================================
//クラス名  ：Tyatoraクラス
//基底クラス：Characterクラス
//
//内容：Characterから派生させたクラス。茶トラのクラス
//================================================================
class Tyatora :public Character
{
private:
	//茶トラのテクスチャ
	Texture TyatoraTex;
	Sprite TyatoraSprite;

public:
	//コンストラクタ
	Tyatora();
	//デストラクタ
	~Tyatora();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();
};
