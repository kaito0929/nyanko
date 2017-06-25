#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ISceneChanger.h"
#include "../Scene/BaseScene.h"
#include "Character.h"

#include "Sound.h"
#include "SoundEffect.h"
#include "Fade.h"

#define CHACKNUM 4
#define MAPSIZE 9

//右方向と上方向への攻撃する位置の上限
#define MINUS_ATTACK_NUM 85
//左方向と下方向への攻撃する位置の上限
#define PLUS_ATTACK_NUM 120


//マップ内の移動可能範囲の最大と最小値
#define MAP_SIZE_MIN 1
#define MAP_SIZE_MAX 7

//マップの端っこの最大と最小値
#define MAPEND_MIN 0
#define MAPEND_MAX 8

//マップの端っこの数値
#define MAPEND_NUM -2
//マップの標準の数値
#define MAPSTANDARD_NUM -1
//攻撃対象がいるマップの数値
#define MAP_ATTACK_OK -4

extern bool FirstPlayerLosingFlag;
extern bool SecondPlayerLosingFlag;


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

//プレイヤーのターンを切り替えるための変数
enum PlayerTurn
{
	FirstPlayer_Turn,
	SecondPlayer_Turn,
};

//ターン切り替えのカットインのための状態
enum FADE
{
	FADE_IN,
	FADE_OUT,
};

class GameState : public BaseScene
{
private:

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

	//背景のテクスチャ
	Texture BackGroundTex;
	Sprite BackGroundSprite;

	//相性の相関図のテクスチャ
	Texture CompatibilityTex;
	Sprite CompatibilitySprite;

	//プレイヤーの行動手順
	PlayerState playerstate;

	//攻撃と待機コマンドの選択
	Command command;

	//攻撃した際の動き
	AttackMotion attack;

	//プレイヤーのターンを切り替えるための変数
	PlayerTurn playerTurn;

	//カットインのフェードインフェードアウト
	int hoge;

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

	//残り移動範囲
	//ユニットは三マス移動することが出来る
	int MoveCount;

	//攻撃コマンドを表示するかのフラグ
	bool AttackCommandDisplay;

	//攻撃できるユニットが四方にいるか調べる変数
	int AttackSearchX;
	int AttackSearchY;

	//移動する前の変数を代入する
	int BeforeMapNumX;
	int BeforeMapNumY;


	//攻撃するユニットを決めたかのフラグ
	bool AttackUnitChoiceFlag;

	//攻撃完了のフラグ
	bool AttackCompleteFlag;

	//プレイヤーの行動回数
	int PlayerActionNum;

	bool TurnChangeFlag;

	Mikeneko mike_1P[2];
	Kuroneko kuro_1P[2];
	Tyatora tyatora_1P[2];

	Mikeneko mike_2P[2];
	Kuroneko kuro_2P[2];
	Tyatora tyatora_2P[2];


	Sound sound;
	SoundEffect se;

public:
	//コンストラクタ
	GameState::GameState(ISceneChanger* changer);
	//デストラクタ
	~GameState();			

	//初期化
	void Initialize() override;
	//実際の動き
	void Update() override;
	//描画
	void Draw() override;

	//移動できる位置を見つける関数
	void Search();

	//カーソルを移動させる
	void MoveCursor();

	//コマンドを選択する矢印を動かす
	void CommandChoice();

	//攻撃しているように動かす関数
	void AttackMotion(int x,int y,int *attackPosX,int *attackPosY,bool *moveFlag,bool *actionFlag);

	//動かすユニットを決定
	void UnitChoice(int x, int y,bool *flag,bool moveFlag);

	//ユニットを実際に移動させる関数
	void UnitMove(int *x, int *y,bool flag);

	//マップの数値を変えるための関数
	void MapChange(int myX, int myY, bool myFlag, int enemyX, int enemyY, bool enemyFlag);

	//移動可能かのフラグを切り替える関数
	void UnitMovePossidleChange(bool aliveFlag, bool *moveFlag);

	//1Pの行動関数
	void FirstPlayer_Update();

	//2Pの行動関数
	void SecondPlayer_Update();

	//マップの数値をリセットする関数
	void MapReset();

	//コマンドを操作する関数
	void CommandPlay(int *x,int *y,bool flag,bool* moveFlag);

	//ユニット同士の戦闘の処理を行う関数
	void UnitBattle();

	//攻撃対象として選ばれたかのフラグを操作する関数
	void AttackFlag(int x, int y, bool*flag);

	//移動先に攻撃可能か調べる関数
	void CheckAttack();

	void TurnChange();
};