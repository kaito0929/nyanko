#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"
#include <stdio.h>
#include <stdlib.h>	//rand()関数用
#include <time.h>	//time()関数用

//1Pの敗北フラグ
bool FirstPlayerLosingFlag = false;
//2Pの敗北フラグ
bool SecondPlayerLosingFlag = false;


GameState::GameState(ISceneChanger* changer) : BaseScene(changer)
{
	
}

GameState::~GameState()
{

}
//初期化関数
void GameState::Initialize()
{
	//1Pの猫の初期化
	mike_1P.Initialize();
	kuro_1P.Initialize();
	tyatora_1P.Initialize();

	//2Pの猫の初期化
	mike_2P.Initialize();
	kuro_2P.Initialize();
	tyatora_2P.Initialize();

	mike_1P.x = 1;
	mike_1P.y = 7;

	kuro_1P.x = 2;
	kuro_1P.y = 7;

	tyatora_1P.x = 3;
	tyatora_1P.y = 7;

	mike_2P.x = 1;
	mike_2P.y = 1;

	kuro_2P.x = 2;
	kuro_2P.y = 1;

	tyatora_2P.x = 3;
	tyatora_2P.y = 1;

	SearchX = 0;
	SearchX = 0;
	
	CursorX = 1;
	CursorY = 1;

	MoveCount = 2;

	//移動可能範囲のマスのテクスチャの初期化
	MoveSquaresTex.Load("Material/masu_b.png");
	MoveSquaresSprite.SetSize(100, 100);

	//攻撃可能場所のマスのテクスチャの初期化
	AttackSquaresTex.Load("Material/masu_r.png");
	AttackSquaresSprite.SetSize(100, 100);

	//カーソルのテクスチャの初期化
	CursorTex.Load("Material/carsor.png");
	CursorSprite.SetSize(100, 100);

	//攻撃コマンドのテクスチャの初期化
	AttackCommandTex.Load("Material/attack.png");
	AttackCommandSprite.SetSize(200, 100);
	AttackCommandSprite.SetPos(800, 450);

	//待機コマンドのテクスチャの初期化
	StandbyCommandTex.Load("Material/taiki.png");
	StandbyCommandSprite.SetSize(200, 100);
	StandbyCommandSprite.SetPos(800, 550);

	//戻るコマンドのテクスチャの初期化
	BackCommandTex.Load("Material/back.png");
	BackCommandSprite.SetSize(200, 100);
	BackCommandSprite.SetPos(800, 650);

	//コマンド選択時の矢印のテクスチャ
	TheArrowTex.Load("Material/yazirusi.png");
	TheArrowSprite.SetSize(100, 50);
	TheArrowSprite.SetPos(650, 450);

	//背景のテクスチャ
	BackGroundTex.Load("Material/tochi_akichi.png");
	BackGroundSprite.SetSize(1200, 1100);
	BackGroundSprite.SetPos(600, 500);

	//カットイン用のテクスチャ（1P）
	CutIn_FirstPlayerTex.Load("Material/turn1.png");
	CutIn_FirstPlayerSprite.SetSize(1200, 600);
	CutIn_FirstPlayerSprite.SetPos(600, 500);
	CutIn_FirstPlayerSprite.SetAlpha(1);

	//カットイン用のテクスチャ（2P）
	CutIn_SecondPlayerTex.Load("Material/turn2.png");
	CutIn_SecondPlayerSprite.SetSize(1200, 600);
	CutIn_SecondPlayerSprite.SetPos(600, 500);
	CutIn_SecondPlayerSprite.SetAlpha(0);

	//フェードアウト用のテクスチャ
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(1);

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;
	playerTurn = FirstPlayer_Turn;
	fade = FADE_IN;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;

	sound.Initialize();
	se.Initialize();

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (y == 0 || x == 0 || y == 8 || x == 8)
			{
				Map[y][x] = -2;
			}
			else
			{
				Map[y][x] = -1;
			}
		}
	}

	ChackDirectionX[0] = 1;		//右の確認
	ChackDirectionY[0] = 0;
	ChackDirectionX[1] = 0;		//下の確認
	ChackDirectionY[1] = 1;
	ChackDirectionX[2] = -1;	//左の確認
	ChackDirectionY[2] = 0;
	ChackDirectionX[3] = 0;		//上の確認
	ChackDirectionY[3] = -1;


	MoveChoiceFlag = false;

	BeforeMapNumX = 0;
	BeforeMapNumY = 0;

	PlayerActionNum = 3;

	AttackUnitChoiceFlag = false;
	AttackCompleteFlag = false;

	FirstPlayerLosingFlag = false;
	SecondPlayerLosingFlag = false;

}
//描画関数
void GameState::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//背景の描画
	Direct3D::DrawSprite(BackGroundSprite, BackGroundTex);

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			MoveSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);
			AttackSquaresSprite.SetPos((x + 1) * 100, (y + 1) * 100);

			if (Map[y][x]>-1)
			{
				//移動可能の青マスを描画
				Direct3D::DrawSprite(MoveSquaresSprite, MoveSquaresTex);
			}
			else if (Map[y][x] <= -4)
			{
				//赤マスを描画
				Direct3D::DrawSprite(AttackSquaresSprite, AttackSquaresTex);
			}
			//ユニットを選択したり移動したりするカーソルの描画
			Direct3D::DrawSprite(CursorSprite, CursorTex);
		}
	}

	//Direct3D::DrawSprite(ShironekoSprite, ShironekoTex);


	//1Pの三毛猫を描画
	mike_1P.Draw();
	//1Pの黒猫を描画
	kuro_1P.Draw();
	//1Pの茶トラを描画
	tyatora_1P.Draw();

	//2Pの三毛猫を描画
	mike_2P.Draw();
	//2Pの黒猫を描画
	kuro_2P.Draw();
	//2Pの茶トラを描画
	tyatora_2P.Draw();

	//1Pのカットイン描画
	Direct3D::DrawSprite(CutIn_FirstPlayerSprite, CutIn_FirstPlayerTex);
	//2Pのカットイン描画
	Direct3D::DrawSprite(CutIn_SecondPlayerSprite, CutIn_SecondPlayerTex);

	//行動コマンドの描画
	//行動手順がCOMMANDの時に処理を行う
	if (playerstate == COMMAND)
	{
		//四方に攻撃できる敵がいるならば攻撃コマンドを表示
		if (AttackCommandDisplay == true)
		{
			//攻撃コマンド
			Direct3D::DrawSprite(AttackCommandSprite, AttackCommandTex);
		}
		//待機コマンド
		Direct3D::DrawSprite(StandbyCommandSprite, StandbyCommandTex);
		//戻るコマンド
		Direct3D::DrawSprite(BackCommandSprite, BackCommandTex);
		//選択用の矢印
		Direct3D::DrawSprite(TheArrowSprite, TheArrowTex);
	}

	//フェードアウト用の画像を描画
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}
//メインの動作
void GameState::Update()
{
	sound.MainSoundPlay();

	if (FirstPlayerLosingFlag == false && SecondPlayerLosingFlag == false)
	{
		//フェードインを行う
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_IN_CHANGENUM));
	}
	else if (FirstPlayerLosingFlag == true || SecondPlayerLosingFlag == true)
	{
		//フェードアウトを行う
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_OUT_CHANGENUM));

		if (FadeSprite.GetAlpha() == 1)
		{
			sound.MainSoundStop();
			mSceneChanger->ChangeScene(STATE_RESULT);
		}
	}

	//カーソルの位置を変更できるようにここで処理
	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);


	//移動可能範囲の表示関数
	Search();

	//1Pのユニットの処理を行う
	mike_1P.Update();
	kuro_1P.Update();
	tyatora_1P.Update();

	//2Pのユニットの処理を行う
	mike_2P.Update();
	kuro_2P.Update();
	tyatora_2P.Update();

	//両プレイヤー共通の処理なので分ける必要はないのでここで処理
	if (playerstate == CHOICE)
	{
		//==■1P用の処理■=================================================================
		//移動中かどうかのフラグをfalseにしておく
		mike_1P.MoveFlag = false;
		kuro_1P.MoveFlag = false;
		tyatora_1P.MoveFlag = false;
		//攻撃対象として選ばれたかのフラグをfalseにしておく
		mike_1P.AttackChoiceFlag = false;
		kuro_1P.AttackChoiceFlag = false;
		tyatora_1P.AttackChoiceFlag = false;
		//=================================================================================

		//==■2P用の処理■=================================================================
		//移動中かどうかのフラグをfalseにしておく
		mike_2P.MoveFlag = false;
		kuro_2P.MoveFlag = false;
		tyatora_2P.MoveFlag = false;
		//攻撃対象として選ばれたかのフラグをfalseにしておく
		mike_2P.AttackChoiceFlag = false;
		kuro_2P.AttackChoiceFlag = false;
		tyatora_2P.AttackChoiceFlag = false;
		//=================================================================================
	}

	CutInFade();

	switch (playerTurn)
	{
	case FirstPlayer_Turn:
		//1Pの行動
		FirstPlayer_Update();
		break;
	case SecondPlayer_Turn:
		//2Pの行動
		SecondPlayer_Update();
		break;
	}

	//1Pが全滅した場合に負けフラグをtrueにする
	//その後でリザルト画面へ遷移
	if (mike_1P.AliveFlag == false && kuro_1P.AliveFlag == false && tyatora_1P.AliveFlag == false)
	{
		FirstPlayerLosingFlag = true;
	}
	//2Pが全滅した場合に負けフラグをtrueにする
	//その後でリザルト画面へ遷移
	else if (mike_2P.AliveFlag == false && kuro_2P.AliveFlag == false && tyatora_2P.AliveFlag == false)
	{
		SecondPlayerLosingFlag = true;
	}

}

//移動可能範囲の表示関数
void GameState::Search()
{

	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			//Mapの数値が1以上ならその場所から開始
			if (Map[y][x]>=1)
			{
				for (int i = 0; i < CHACKNUM; i++)
				{
					//猫の隣のマスを調べる
					SearchX = x + ChackDirectionX[i];
					SearchY = y + ChackDirectionY[i];

					//残り移動回数にはMapの数値を代入
					MoveCount = Map[y][x];

					//フラグがtrueならば繰り返す
					while (MoveChoiceFlag == true)
					{
						//Mapの数値が-1なら処理
						//それ以外ならばループから抜ける
						if (Map[SearchY][SearchX] == -1)
						{
							//移動回数をマイナス
							MoveCount--;

							//残り移動回数をMapへと代入
							Map[SearchY][SearchX] = MoveCount;

							//数値をそれぞれの方向へ加算
							SearchX += ChackDirectionX[i];
							SearchY += ChackDirectionY[i];

						}
						else if (Map[SearchY][SearchX] == -3)
						{
							Map[SearchY][SearchX] = -4;
							break;
						}
						else
						{
							break;
						}

						//移動回数が0になったらループから抜ける
						if (MoveCount == 0)
						{
							break;
						}
					}
				}
			}
		}
	}

}

//移動する猫や場所を選択するカーソルを操作する関数
void GameState::MoveCursor()
{
	DirectInput* pDi = DirectInput::GetInstance();

	//================================================================
	//カーソルの移動を制御する関数
	//================================================================

	//カーソルの上移動
	if (pDi->KeyJustPressed(DIK_UP))
	{
		if (CursorY != 1)
		{
			CursorY--;
			//移動した時に効果音を再生
			se.ChoiceSEPlay();
		}
	}

	//カーソルの下移動
	if (pDi->KeyJustPressed(DIK_DOWN))
	{
		if (CursorY != 7)
		{
			CursorY++;
			//移動した時に効果音を再生
			se.ChoiceSEPlay();
		}
	}

	//カーソルの左移動
	if (pDi->KeyJustPressed(DIK_LEFT))
	{
		if (CursorX != 1)
		{
			CursorX--;
			//移動した時に効果音を再生
			se.ChoiceSEPlay();
		}
	}

	//カーソルの右移動
	if (pDi->KeyJustPressed(DIK_RIGHT))
	{
		if (CursorX != 7)
		{
			CursorX++;
			//移動した時に効果音を再生
			se.ChoiceSEPlay();
		}
	}
}

//攻撃か待機か戻るかを選ぶ矢印を操作する関数
void GameState::CommandChoice()
{
	DirectInput* pDi = DirectInput::GetInstance();

	//コマンドを選択するカーソルを移動する

	//======================================================
	//　ATTACK:隣に敵ユニットがいるならば選択可能
	//　STANDBY:何もせずに待機
	//　BACK:行動する前に戻る
	//======================================================

	switch(command)
	{
	case ATTACK:

		//隣に敵ユニットがいない場合は待機コマンドになるように変更
		if (AttackCommandDisplay == false)
		{
			command = STANDBY;
		}

		//上キー、下キーを押して、戻るコマンドか待機コマンドを選択
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = BACK;
			se.ChoiceSEPlay();
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = STANDBY;
			se.ChoiceSEPlay();
		}
		break;
	case STANDBY:

		//上キー、下キーを押して、戻るコマンドか攻撃コマンドを選択
		//隣に敵ユニットがいない場合、攻撃コマンドに移動しないようにする
		if (pDi->KeyJustPressed(DIK_UP))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
				se.ChoiceSEPlay();
			}
			else
			{
				command = BACK;
				se.ChoiceSEPlay();
			}
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = BACK;
			se.ChoiceSEPlay();
		}
		break;
	case BACK:

		//上キー、下キーを押して、待機コマンドか攻撃コマンドを選択
		//隣に敵ユニットがいない場合、攻撃コマンドに移動しないようにする
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = STANDBY;
			se.ChoiceSEPlay();
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
				se.ChoiceSEPlay();
			}
			else
			{
				command = STANDBY;
				se.ChoiceSEPlay();
			}
		}
		break;
	}

}

//攻撃しているように動かす関数
void GameState::AttackMotion(int x, int y, int *attackPosX, int *attackPosY, bool *moveFlag, bool *actionFlag)
{
	if (*moveFlag == true)
	{
		//フラグがtrueならば攻撃の方向を決定する
		if (AttackUnitChoiceFlag == true)
		{
			se.CatSEPlay();
			//カーソルの位置とユニットの位置を比較して
			//移動する方向を変更する
			if (CursorY < y)
			{
				//カーソルの位置がユニットより下だった場合（この場合はy軸なので数値が少ない方が上）
				//ユニットの真上へ移動するように変更
				//以下同じようにする
				attack = UP;
			}
			else if (CursorY > y)
			{
				attack = DOWN;
			}
			else if (CursorX < x)
			{
				attack = LEFT;
			}
			else if (CursorX > x)
			{
				attack = RIGHT;
			}
		}

		//移動状態がRETURN以外ならばフラグをfalseにしておく
		if (attack != RETURN)
		{
			AttackUnitChoiceFlag = false;
		}


		//上で決定した方向にユニットの座標を変更する
		//移動上限に達した場合には元の位置へと戻るように
		//変数を加算したり減算したりする
		switch (attack)
		{
		case UP://上方向の敵に向かって動く
			if (*attackPosY >= MINUS_ATTACK_NUM)
			{
				*attackPosY -= 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case DOWN://下方向の敵に向かって動く
			if (*attackPosY <= PLUS_ATTACK_NUM)
			{
				*attackPosY += 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RIGHT://右方向の敵に向かって動く
			if (*attackPosX <= PLUS_ATTACK_NUM)
			{
				*attackPosX += 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case LEFT://左方向の敵に向かって動く
			if (*attackPosX >= MINUS_ATTACK_NUM)
			{
				*attackPosX -= 1;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RETURN://元の位置へ戻る

			//変数が100以下だったり100以上であるならば
			//元に位置に戻るように変数を加算か減算する
			if (*attackPosY < 100)
			{
				*attackPosY += 1;
			}
			else if (*attackPosY > 100)
			{
				*attackPosY -= 1;
			}
			else if (*attackPosX < 100)
			{
				*attackPosX += 1;
			}
			else if (*attackPosX > 100)
			{
				*attackPosX -= 1;
			}
			else
			{
				//1Pが2Pに攻撃した際の戦闘内容の処理
				mike_1P.UnitBattle(tyatora_2P.cat, &tyatora_2P.HitPoint, tyatora_2P.AttackChoiceFlag);
				mike_1P.UnitBattle(mike_2P.cat, &mike_2P.HitPoint, mike_2P.AttackChoiceFlag);
				//2Pが1Pに攻撃した際の戦闘内容の処理
				mike_2P.UnitBattle(tyatora_1P.cat, &tyatora_1P.HitPoint, tyatora_1P.AttackChoiceFlag);
				mike_2P.UnitBattle(mike_1P.cat, &mike_1P.HitPoint, mike_1P.AttackChoiceFlag);

				//1Pが2Pに攻撃した際の戦闘内容の処理
				kuro_1P.UnitBattle(mike_2P.cat, &mike_2P.HitPoint, mike_2P.AttackChoiceFlag);
				kuro_1P.UnitBattle(kuro_2P.cat, &kuro_2P.HitPoint, kuro_2P.AttackChoiceFlag);
				//2Pが1Pに攻撃した際の戦闘内容の処理
				kuro_2P.UnitBattle(mike_1P.cat, &mike_1P.HitPoint, mike_1P.AttackChoiceFlag);
				kuro_2P.UnitBattle(kuro_1P.cat, &kuro_1P.HitPoint, kuro_1P.AttackChoiceFlag);

				//1Pが2Pに攻撃した際の戦闘内容の処理
				tyatora_1P.UnitBattle(kuro_2P.cat, &kuro_2P.HitPoint, kuro_2P.AttackChoiceFlag);
				tyatora_1P.UnitBattle(tyatora_2P.cat, &tyatora_2P.HitPoint, tyatora_2P.AttackChoiceFlag);
				//2Pが1Pに攻撃した際の戦闘内容の処理
				tyatora_2P.UnitBattle(kuro_1P.cat, &kuro_1P.HitPoint, kuro_1P.AttackChoiceFlag);
				tyatora_2P.UnitBattle(tyatora_1P.cat, &tyatora_1P.HitPoint, tyatora_1P.AttackChoiceFlag);

				*actionFlag = false;

				attack = INITIALSTATE;
				playerstate = CHOICE;
				PlayerActionNum--;
			}
			break;
		}
	}
}

//ユニットを選択した時の処理
void GameState::UnitChoice(int x,int y,bool *flag,bool moveFlag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	//カーソルの位置と猫の位置が一致していて
	//エンターキーを押したら処理
	if (CursorX == x && CursorY == y&&moveFlag == true)
	{
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//移動可能場所を探すフラグをtrueにして
			MoveChoiceFlag = true;
			//playerstateをMoveに変更
			//これで移動可能に
			playerstate = MOVE;
			//選択した猫の位置を3に
			Map[CursorY][CursorX] = 3;

			//ユニットの元に位置の変数を代入
			BeforeMapNumX = x;
			BeforeMapNumY = y;

			*flag = true;
			se.DecisionSEPlay();
		}
	}
}

//ユニットを移動させる
void GameState::UnitMove(int *x, int *y,bool flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		//カーソルの位置が-1以上ならば処理
		if (Map[CursorY][CursorX] > -1 && flag == true)
		{
			se.DecisionSEPlay();
			//猫をカーソルの位置へ移動
			*x = CursorX;
			*y = CursorY;
			//移動可能マスを調べるフラグをfalseに
			MoveChoiceFlag = false;
			//コマンド選択へ遷移
			playerstate = COMMAND;
		}
	}
}

//マップの数値を変える関数
void GameState::MapChange(int x,int y,bool flag)
{
	if (flag == true)
	{
		Map[y][x] = -3;
	}
}

//自ターンの時にマップの数値を切り替える
void GameState::MyTurnMapChange(int x, int y, bool flag)
{
	if (flag == true)
	{
		Map[y][x] = -2;
	}
}

void GameState::UnitMovePossidleChange(bool aliveFlag, bool *moveFlag)
{
	if (aliveFlag == true)
	{
		*moveFlag = true;
	}
	else
	{
		*moveFlag = false;
	}
}

//1Pの行動関数
void GameState::FirstPlayer_Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	switch (playerstate)
	{
	case CHOICE://どの猫を動かすか決める

		//攻撃コマンドの表示フラグはfalseにしておく
		AttackCommandDisplay = false;
		//選択用のカーソルを動かす関数
		MoveCursor();
		//マップの数値を一度リセットしておく
		MapReset();
		
		//敵ユニットのいる場所が-3になるようにする
		//この関数は1Pのターンの時に実行されるのでここでの敵は2Pになる
		MapChange(mike_2P.x, mike_2P.y, mike_2P.AliveFlag);
		MapChange(kuro_2P.x, kuro_2P.y, kuro_2P.AliveFlag);
		MapChange(tyatora_2P.x, tyatora_2P.y, tyatora_2P.AliveFlag);

		//自分のユニットの位置は-2にしておいて味方と味方が重ならないようにする
		//-2ならばSearch関数のループが抜けるので
		MyTurnMapChange(mike_1P.x, mike_1P.y, mike_1P.AliveFlag);
		MyTurnMapChange(kuro_1P.x, kuro_1P.y, kuro_1P.AliveFlag);
		MyTurnMapChange(tyatora_1P.x, tyatora_1P.y, tyatora_1P.AliveFlag);

		//自分のユニットを選択
		//この関数はこれから移動するユニットのMoveFlagを操作する
		//既に移動済みであるならば選択できないようになっている
		UnitChoice(mike_1P.x, mike_1P.y, &mike_1P.MoveFlag, mike_1P.MovePossidleFlag);
		UnitChoice(kuro_1P.x, kuro_1P.y, &kuro_1P.MoveFlag, kuro_1P.MovePossidleFlag);
		UnitChoice(tyatora_1P.x, tyatora_1P.y, &tyatora_1P.MoveFlag, tyatora_1P.MovePossidleFlag);

		//最大行動回数が0になればターンが切り替わる
		//ユニットを移動し、待機か攻撃を行った際にPlayerActionNumはマイナスされる
		//カットインをフェードインさせるのはこの処理で
		if (PlayerActionNum == 0)
		{
			playerTurn = SecondPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//敵ユニット、2Pの移動可能かのフラグをtrueにして行動可能にする
			UnitMovePossidleChange(mike_2P.AliveFlag, &mike_2P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_2P.AliveFlag, &kuro_2P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_2P.AliveFlag, &tyatora_2P.MovePossidleFlag);
		}

		//全てのユニットが行動不可能ならばターンを切り替える
		if (mike_1P.MovePossidleFlag == false && kuro_1P.MovePossidleFlag == false && tyatora_1P.MovePossidleFlag == false)
		{
			playerTurn = SecondPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//敵ユニット、2Pの移動可能かのフラグをtrueにして行動可能にする
			UnitMovePossidleChange(mike_2P.AliveFlag, &mike_2P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_2P.AliveFlag, &kuro_2P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_2P.AliveFlag, &tyatora_2P.MovePossidleFlag);
		}
		

		break;
	case MOVE://猫を移動させる

		//選択用のカーソルを移動させる関数
		//この場合は移動先を決定するために移動
		MoveCursor();

		//選択したユニットを移動
		//移動ちゅかのフラグがtrueだった場合にのみ移動
		//カーソルの座標を選択されているユニットの座標に代入する
		UnitMove(&mike_1P.x, &mike_1P.y, mike_1P.MoveFlag);
		UnitMove(&kuro_1P.x, &kuro_1P.y, kuro_1P.MoveFlag);
		UnitMove(&tyatora_1P.x, &tyatora_1P.y, tyatora_1P.MoveFlag);

		break;
	case COMMAND://待機、攻撃、戻るといった行動を決めるコマンド選択

		//移動したその場から四方を調べて攻撃可能なマスがあるかを調べる
		for (int i = 0; i < CHACKNUM; i++)
		{
			//移動した先の四方を調べる
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//上、下、右、左の何処かに攻撃可能マスがあれば
			//攻撃コマンドを表示させる
			if (Map[AttackSearchY][AttackSearchX] == -5)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3 || Map[AttackSearchY][AttackSearchX] == -4)
			{
				//隣の数値が-3か-4ならばマップの数値を-5にして、攻撃可能とする
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//コマンドを選択する矢印の移動
		CommandChoice();

		//選択したコマンドの効果を処理する関数
		//移動したユニットの分だけ処理を行う
		CommandPlay(&mike_1P.x, &mike_1P.y, mike_1P.MoveFlag, &mike_1P.MovePossidleFlag);
		CommandPlay(&kuro_1P.x, &kuro_1P.y, kuro_1P.MoveFlag, &kuro_1P.MovePossidleFlag);
		CommandPlay(&tyatora_1P.x, &tyatora_1P.y, tyatora_1P.MoveFlag, &tyatora_1P.MovePossidleFlag);

		break;

	case ATTACKCHOICE://攻撃する敵ユニットを決定

		//攻撃中でなければカーソルを動かせるようにしておく
		if (AttackUnitChoiceFlag == false)
		{
			//攻撃対象を選択するカーソルを移動させる関数
			MoveCursor();
		}

		//エンターキーを押した時に
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//その場のマップの数値が-5であれば処理
			if (Map[CursorY][CursorX] == -5)
			{
				//フラグをtrueにして移動開始
				AttackUnitChoiceFlag = true;

				//攻撃対象として選ばれた2Pキャラのフラグをtrueにする
				//そうすれば四方にない攻撃可能マスを選んだ場合でも攻撃は行われない
				AttackFlag(mike_2P.x, mike_2P.y, CursorX, CursorY, &mike_2P.AttackChoiceFlag);
				AttackFlag(kuro_2P.x, kuro_2P.y, CursorX, CursorY, &kuro_2P.AttackChoiceFlag);
				AttackFlag(tyatora_2P.x, tyatora_2P.y, CursorX, CursorY, &tyatora_2P.AttackChoiceFlag);
			}
		}

		//ユニットを決められた方向へ攻撃している風に見せる
		//カーソルとユニットの位置を比較して方向を決定
		//そのモーションが終わったら戦闘の後処理を行ってHPの減算や行動終了にしたりする
		AttackMotion(mike_1P.x, mike_1P.y, &mike_1P.AttackPosX, &mike_1P.AttackPosY, &mike_1P.MoveFlag, &mike_1P.MovePossidleFlag);
		AttackMotion(kuro_1P.x, kuro_1P.y, &kuro_1P.AttackPosX, &kuro_1P.AttackPosY, &kuro_1P.MoveFlag, &kuro_1P.MovePossidleFlag);
		AttackMotion(tyatora_1P.x, tyatora_1P.y, &tyatora_1P.AttackPosX, &tyatora_1P.AttackPosY, &tyatora_1P.MoveFlag, &tyatora_1P.MovePossidleFlag);
		break;
	}
}

//2Pの行動関数
void GameState::SecondPlayer_Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	switch (playerstate)
	{
	case CHOICE://どの猫を動かすか決める

		//攻撃コマンドの表示フラグはfalseにしておく
		AttackCommandDisplay = false;
		//選択用のカーソルを動かす関数
		MoveCursor();
		//マップの数値を一度リセットしておく
		MapReset();

		//敵ユニットのいる場所が-3になるようにする
		//この関数は2Pのターンの時に実行されるのでここでの敵は1Pになる
		MapChange(mike_1P.x, mike_1P.y, mike_1P.AliveFlag);
		MapChange(kuro_1P.x, kuro_1P.y, kuro_1P.AliveFlag);
		MapChange(tyatora_1P.x, tyatora_1P.y, tyatora_1P.AliveFlag);

		//自分のユニットの位置は-2にしておいて味方と味方が重ならないようにする
		//-2ならばSearch関数のループが抜けるので
		MyTurnMapChange(mike_2P.x, mike_2P.y, mike_2P.AliveFlag);
		MyTurnMapChange(kuro_2P.x, kuro_2P.y, kuro_2P.AliveFlag);
		MyTurnMapChange(tyatora_2P.x, tyatora_2P.y, tyatora_2P.AliveFlag);

		//自分のユニットを選択
		//この関数はこれから移動するユニットのMoveFlagを操作する
		//既に移動済みであるならば選択できないようになっている
		UnitChoice(mike_2P.x, mike_2P.y, &mike_2P.MoveFlag, mike_2P.MovePossidleFlag);
		UnitChoice(kuro_2P.x, kuro_2P.y, &kuro_2P.MoveFlag, kuro_2P.MovePossidleFlag);
		UnitChoice(tyatora_2P.x, tyatora_2P.y, &tyatora_2P.MoveFlag, tyatora_2P.MovePossidleFlag);

		//最大行動回数が0になればターンが切り替わる
		//ユニットを移動し、待機か攻撃を行った際にPlayerActionNumはマイナスされる
		//カットインをフェードインさせるのはこの処理で
		if (PlayerActionNum == 0)
		{
			playerTurn = FirstPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//敵ユニット、1Pの移動可能かのフラグをtrueにして行動可能にする
			UnitMovePossidleChange(mike_1P.AliveFlag, &mike_1P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_1P.AliveFlag, &kuro_1P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_1P.AliveFlag, &tyatora_1P.MovePossidleFlag);
		}

		//全てのユニットが行動不可能ならばターンを切り替える
		if (mike_2P.MovePossidleFlag == false && kuro_2P.MovePossidleFlag == false && tyatora_2P.MovePossidleFlag == false)
		{
			playerTurn = FirstPlayer_Turn;
			fade = FADE_OUT;
			PlayerActionNum = 3;

			//敵ユニット、2Pの移動可能かのフラグをtrueにして行動可能にする
			UnitMovePossidleChange(mike_1P.AliveFlag, &mike_1P.MovePossidleFlag);
			UnitMovePossidleChange(kuro_1P.AliveFlag, &kuro_1P.MovePossidleFlag);
			UnitMovePossidleChange(tyatora_1P.AliveFlag, &tyatora_1P.MovePossidleFlag);
		}

		break;
	case MOVE://猫を移動させる

		//選択用のカーソルを移動させる関数
		//この場合は移動先を決定するために移動
		MoveCursor();

		//選択したユニットを移動
		//移動中のフラグがtrueだった場合にのみ移動
		//カーソルの座標を選択されているユニットの座標に代入する
		UnitMove(&mike_2P.x, &mike_2P.y, mike_2P.MoveFlag);
		UnitMove(&kuro_2P.x, &kuro_2P.y, kuro_2P.MoveFlag);
		UnitMove(&tyatora_2P.x, &tyatora_2P.y, tyatora_2P.MoveFlag);

		break;
	case COMMAND://待機、攻撃、戻るといった行動を決めるコマンド選択

		//移動したその場から四方を調べて攻撃可能なマスがあるかを調べる
		for (int i = 0; i < CHACKNUM; i++)
		{
			//移動した先の四方を調べる
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//上、下、右、左の何処かに攻撃可能マスがあれば
			//攻撃コマンドを表示させる
			if (Map[AttackSearchY][AttackSearchX] == -5)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3 || Map[AttackSearchY][AttackSearchX] == -4)
			{
				//隣の数値が-3か-4ならばマップの数値を-5にして、攻撃可能とする
				Map[AttackSearchY][AttackSearchX] = -5;
			}

		}

		//コマンドを選択する矢印の移動
		CommandChoice();

		//選択したコマンドの効果を処理する関数
		//移動したユニットの分だけ処理を行う
		CommandPlay(&mike_2P.x, &mike_2P.y, mike_2P.MoveFlag, &mike_2P.MovePossidleFlag);
		CommandPlay(&kuro_2P.x, &kuro_2P.y, kuro_2P.MoveFlag, &kuro_2P.MovePossidleFlag);
		CommandPlay(&tyatora_2P.x, &tyatora_2P.y, tyatora_2P.MoveFlag, &tyatora_2P.MovePossidleFlag);

		break;

	case ATTACKCHOICE://攻撃する敵ユニットを決定

		//攻撃中でなければカーソルを動かせるようにしておく
		if (AttackUnitChoiceFlag == false)
		{
			//攻撃対象を選択するカーソルを移動させる関数
			MoveCursor();
		}

		//エンターキーを押した時に
		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			//その場のマップの数値が-5であれば処理
			if (Map[CursorY][CursorX] == -5)
			{
				//フラグをtrueにして移動開始
				AttackUnitChoiceFlag = true;

				//攻撃対象として選ばれた2Pキャラのフラグをtrueにする
				//そうすれば四方にない攻撃可能マスを選んだ場合でも攻撃は行われない
				AttackFlag(mike_1P.x, mike_1P.y, CursorX, CursorY, &mike_1P.AttackChoiceFlag);
				AttackFlag(kuro_1P.x, kuro_1P.y, CursorX, CursorY, &kuro_1P.AttackChoiceFlag);
				AttackFlag(tyatora_1P.x, tyatora_1P.y, CursorX, CursorY, &tyatora_1P.AttackChoiceFlag);
			}
		}

		//ユニットを決められた方向へ攻撃している風に見せる
		//カーソルとユニットの位置を比較して方向を決定
		//そのモーションが終わったら戦闘の後処理を行ってHPの減算や行動終了にしたりする
		AttackMotion(mike_2P.x, mike_2P.y, &mike_2P.AttackPosX, &mike_2P.AttackPosY, &mike_2P.MoveFlag, &mike_2P.MovePossidleFlag);
		AttackMotion(kuro_2P.x, kuro_2P.y, &kuro_2P.AttackPosX, &kuro_2P.AttackPosY, &kuro_2P.MoveFlag, &kuro_2P.MovePossidleFlag);
		AttackMotion(tyatora_2P.x, tyatora_2P.y, &tyatora_2P.AttackPosX, &tyatora_2P.AttackPosY, &tyatora_2P.MoveFlag, &tyatora_2P.MovePossidleFlag);
		break;
	}
}

//マップの数値をリセットする関数
void GameState::MapReset()
{
	//マップをリセット
	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			//移動不可能な場所は-2にしておく
			//それ以外の場所は移動可能場所として-1に
			if (y == 0 || x == 0 || y == 8 || x == 8)
			{
				Map[y][x] = -2;
			}
			else
			{
				Map[y][x] = -1;
			}
		}
	}

}

//コマンドの内容を処理する関数
void GameState::CommandPlay(int *x, int *y, bool flag,bool *moveFlag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	if (flag == true)
	{
		//選択するコマンドによって変化
		switch (command)
		{
		case ATTACK://攻撃フェーズに遷移

			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 450);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				se.DecisionSEPlay();
				playerstate = ATTACKCHOICE;
			}
			break;
		case STANDBY://その場に待機する
			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 550);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				se.DecisionSEPlay();
				//プレイヤーの行動回数をマイナス
				PlayerActionNum--;
				//行動済みとする
				*moveFlag = false;
				//行動ユニットを決める処理に戻る
				playerstate = CHOICE;
			}
			break;
		case BACK://元の場所へ戻る
			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				se.DecisionSEPlay();
				*x = BeforeMapNumX;
				*y = BeforeMapNumY;
				playerstate = CHOICE;
				break;
			}
		}
	}
}

//攻撃対象として選ばれたかのフラグを操作する関数
void GameState::AttackFlag(int x, int y, int cursorX, int cursorY, bool*flag)
{
	//ユニットの座標とカーソルの座標が一致したならば
	if (x == cursorX&&y == cursorY)
	{
		//そのユニットのflagを操作する
		*flag = true;
	}
}

//カットインのフェードインフェードアウト
void GameState::CutInFade()
{
	//全体的なフェードインフェードアウトようの画像のα値が0ならば処理されるように
	if (FadeSprite.GetAlpha() == 0)
	{
		//fadeによってフェードイン、フェードアウトを切り替える
		switch (fade)
		{
		case FADE_IN://フェードインを行う

			//playerTurnによってフェードインを行う画像を切り替える
			if (playerTurn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}
			else if (playerTurn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}

			break;
		case FADE_OUT://フェードアウトを行う

			//playerTurnによってフェードインを行う画像を切り替える
			if (playerTurn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));
				//α値が1にまで加算されたらPlayerTurnをFADE_INに切り替えてフェードインを行うように
				if (CutIn_FirstPlayerSprite.GetAlpha() == 1)
				{
					//カットイン時の効果音再生
					se.CutInSEPlay();
					fade = FADE_IN;
				}
			}
			//こっちの内容も上記と同じなのでコメントは上記を参照してください
			else if (playerTurn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));

				if (CutIn_SecondPlayerSprite.GetAlpha() == 1)
				{
					se.CutInSEPlay();
					fade = FADE_IN;
				}
			}
			break;
		}
	}
}