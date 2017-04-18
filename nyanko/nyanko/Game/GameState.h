#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "Character.h"

#define CHACKNUM 4
#define MAPSIZE 9

//右方向と上方向への攻撃する位置の上限
#define MINUS_ATTACK_NUM 85
//左方向と下方向への攻撃する位置の上限
#define PLUS_ATTACK_NUM 120

//フェードアウト用の数値
#define FADE_OUT_CHANGENUM -1 

struct Cat
{
	//ユニットの座標
	int x;
	int y;
	//攻撃しているように見せるためのユニットの表示場所の変数
	int AttackPosX;
	int AttackPosY;
};

//基本的なプレイヤーの操作
enum PlayerState
{
	CHOICE,			//どのユニットを操作するか決める
	MOVE,			//選択したユニットを移動
	COMMAND,		//攻撃、待機、戻るの三つの選択肢を決める
	ATTACKCHOICE,	//攻撃対象を選択
};

//ユニットを移動した後に表示されるコマンドの選択
enum Command
{
	ATTACK,			//攻撃コマンド
	STANDBY,		//待機コマンド
	BACK,			//戻るコマンド
};

//攻撃する際の動く方向
enum AttackMotion
{
	UP,				//上方向
	DOWN,			//下方向
	RIGHT,			//右方向
	LEFT,			//左方向
	RETURN,			//元の位置へ
	INITIALSTATE,	//方向も決まっていない初期の状態
};

class GameState
{
private:

	//三毛猫のテクスチャ
	Texture MikenekoTex;
	Sprite MikenekoSprite;

	//茶トラのテクスチャ
	Texture TyatoraTex;
	Sprite TyatoraSprite;

	//黒猫のテクスチャ
	Texture KuronekoTex;
	Sprite KuronekoSprite;

	//白猫のテクスチャ
	Texture ShironekoTex;
	Sprite ShironekoSprite;

	//移動できる範囲のマス
	Texture MoveSquaresTex;
	Sprite MoveSquaresSprite;

	//攻撃可能を示すマス
	Texture AttackSquaresTex;
	Sprite AttackSquaresSprite;

	//選択したりするカーソルのテクスチャ
	Texture CursorTex;
	Sprite CursorSprite;

	//行動を決めるコマンド（攻撃）
	Texture AttackCommandTex;
	Sprite AttackCommandSprite;

	//行動を決めるコマンド（待機）
	Texture StandbyCommandTex;
	Sprite StandbyCommandSprite;

	//行動を決めるコマンド（戻る）
	Texture BackCommandTex;
	Sprite BackCommandSprite;

	//コマンドを選択する矢印
	Texture TheArrowTex;
	Sprite TheArrowSprite;

	//三毛猫の座標
	Cat mike;
	//茶トラの座標
	Cat tyatora;
	//黒猫の座標
	Cat kuro;
	//白猫の座標
	Cat shiro;

	//プレイヤーの行動手順
	PlayerState playerstate;

	//攻撃と待機コマンドの選択
	Command command;

	//攻撃した際の動き
	AttackMotion attack;

	//マップデータ:9*9
	int Map[MAPSIZE][MAPSIZE];

	//移動範囲を調べる際の方向変換の為の変数
	int ChackDirectionX[CHACKNUM];
	int ChackDirectionY[CHACKNUM];

	//移動範囲を調べるための変数
	int SearchX;
	int SearchY;

	//カーソルの表示位置の変数
	int CursorX;
	int CursorY;

	//移動位置を決める際のフラグ
	bool MoveChoiceFlag;

	//残り移動回数
	//ユニットは三回移動することが出来る
	int MoveCount;

	//攻撃コマンドを表示するかのフラグ
	bool AttackCommandDisplay;

	//攻撃できるユニットが四方にいるか調べる変数
	int AttackSearchX;
	int AttackSearchY;

	//移動する前の変数を代入する
	int BeforeMapNumX;
	int BeforeMapNumY;

	//生存フラグ
	bool AliveFlag;	

	//攻撃するユニットを決めたかのフラグ
	bool AttackUnitChoiceFlag;

	//攻撃完了のフラグ
	bool AttackCompleteFlag;

	bool flag[3];

public:
	//コンストラクタ
	GameState();			
	//デストラクタ
	~GameState();			

	//初期化
	void Initialize();	
	//実際の動き
	void Update();			
	//描画
	void Draw();			

	//移動できる位置を見つける関数
	void Search();

	//カーソルを移動させる
	void MoveCursor();

	//コマンドを選択する矢印を動かす
	void CommandChoice();

	//攻撃しているように動かす関数
	void AttackMotion(int x,int y,int *attackPosX,int *attackPosY,bool *flag);

	//ユニットをフェードアウトさせる関数
	void UnitFade();

	//動かすユニットを決定
	void UnitChoice(int x, int y,bool *flag);

	//ユニットを実際に移動させる関数
	void UnitMove(int *x, int *y,bool flag);
};