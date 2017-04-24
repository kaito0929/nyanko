#pragma once
#include "GameState.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

//フェードアウト用の数値
#define CAT_FADE_OUT_CHANGENUM -1 

//猫を区別するためのステータス
enum CatState
{
	MIKE,		//三毛猫
	KURO,		//黒猫
	TYATORA,	//茶トラ
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

	//キャラクターの体力
	int HitPoint;

	///攻撃対象として選ばれたかのフラグ
	bool AttackChoiceFlag;

	//猫のステータス
	CatState cat;

	//ヒットポイントのハートのテクスチャ
	Texture HitPointTex;
	Sprite HitPointSprite[3];
	//ヒットポイントのハートの位置
	float HitPointPos[3];

	//ユニットが移動可能かのフラグ
	bool MovePossidleFlag;

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
	Texture MikenekoTex[2];
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

	//ユニット同士の戦闘の処理を行う関数
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//ユニットをフェードアウトさせる関数
	void UnitFade();
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
	Texture KuronekoTex[2];
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

	//ユニット同士の戦闘の処理を行う関数
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//ユニットをフェードアウトさせる関数
	void UnitFade();
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
	Texture TyatoraTex[2];
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

	//ユニット同士の戦闘の処理を行う関数
	void UnitBattle(CatState Cat, int *hp, bool attackFlag);

	//ユニットをフェードアウトさせる関数
	void UnitFade();
};
