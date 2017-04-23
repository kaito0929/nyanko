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

int ax = 100;
int ay = 100;

GameState::GameState()
{
	
}

GameState::~GameState()
{

}

void GameState::Initialize()
{
	
	for (int i = 0; i < 2; i++)
	{
		mike[i].Initialize();
		kuro[i].Initialize();
		tyatora[i].Initialize();
	}

	mike[0].x = 1;
	mike[0].y = 7;

	kuro[0].x = 2;
	kuro[0].y = 7;

	tyatora[0].x = 3;
	tyatora[0].y = 7;

	mike[1].x = 1;
	mike[1].y = 1;

	kuro[1].x = 2;
	kuro[1].y = 1;

	tyatora[1].x = 3;
	tyatora[1].y = 1;

	/*shiro.x = 2;
	shiro.y = 2;
	shiro.AttackPosX = 100;
	shiro.AttackPosY = 100;*/

	SearchX = 0;
	SearchX = 0;
	
	CursorX = 1;
	CursorY = 1;

	MoveCount = 2;


	//白猫のテクスチャの初期化
	//ShironekoTex.Load("Material/sironeko.png");
	//ShironekoSprite.SetPos(shiro.x * 200, shiro.y * 150);
	//ShironekoSprite.SetSize(100, 100);

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

	playerstate = CHOICE;
	command = ATTACK;
	attack = INITIALSTATE;
	playerTurn = FirstPlayer_Turn;

	AttackCommandDisplay = false;
	AttackSearchX = 0;
	AttackSearchY = 0;

	AliveFlag = true;

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

}

void GameState::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

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
			else if (Map[y][x] == -6)
			{
				//赤マスを描画
				Direct3D::DrawSprite(AttackSquaresSprite, AttackSquaresTex);
			}
			//ユニットを選択したり移動したりするカーソルの描画
			Direct3D::DrawSprite(CursorSprite, CursorTex);
		}
	}

	//Direct3D::DrawSprite(ShironekoSprite, ShironekoTex);

	for (int i = 0; i < 2; i++)
	{
		//三毛猫を描画
		mike[i].Draw();
		//黒猫を描画
		kuro[i].Draw();
		//茶トラを描画
		tyatora[i].Draw();
	}

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
}

void GameState::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	for (int i = 0; i < 2; i++)
	{
		mike[i].Update();
		kuro[i].Update();
		tyatora[i].Update();
	}

	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);


	//移動可能範囲の表示関数
	Search();
	
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
	

	UnitFade();
	
	
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
						else if (Map[SearchY][SearchX] <= -3)
						{
							Map[SearchY][SearchX] = -6;
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

	//=■カーソルの移動■===================================================================
	if (pDi->KeyJustPressed(DIK_UP))
	{
		if (CursorY != 1)
		{
			CursorY--;
		}
	}

	if (pDi->KeyJustPressed(DIK_DOWN))
	{
		if (CursorY != 7)
		{
			CursorY++;
		}
	}

	if (pDi->KeyJustPressed(DIK_LEFT))
	{
		if (CursorX != 1)
		{
			CursorX--;
		}
	}

	if (pDi->KeyJustPressed(DIK_RIGHT))
	{
		if (CursorX != 7)
		{
			CursorX++;
		}
	}
}

//攻撃か待機かを選ぶ矢印を操作する関数
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
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = STANDBY;
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
			}
			else
			{
				command = BACK;
			}
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			command = BACK;
		}
		break;
	case BACK:

		//上キー、下キーを押して、待機コマンドか攻撃コマンドを選択
		//隣に敵ユニットがいない場合、攻撃コマンドに移動しないようにする
		if (pDi->KeyJustPressed(DIK_UP))
		{
			command = STANDBY;
		}
		if (pDi->KeyJustPressed(DIK_DOWN))
		{
			if (AttackCommandDisplay == true)
			{
				command = ATTACK;
			}
			else
			{
				command = STANDBY;
			}
		}
		break;
	}

}

//攻撃しているように動かす関数
void GameState::AttackMotion(int x, int y, int *attackPosX, int *attackPosY, bool *flag)
{
	if (*flag == true)
	{
		//フラグがtrueならば攻撃の方向を決定する
		if (AttackUnitChoiceFlag == true)
		{
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
				ay--;
				*attackPosY = ay;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case DOWN://下方向の敵に向かって動く
			if (*attackPosY <= PLUS_ATTACK_NUM)
			{
				ay++;
				*attackPosY = ay;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case RIGHT://右方向の敵に向かって動く
			if (*attackPosX <= PLUS_ATTACK_NUM)
			{
				ax++;
				*attackPosX = ax;
			}
			else
			{
				attack = RETURN;
			}
			break;
		case LEFT://左方向の敵に向かって動く
			if (*attackPosX >= MINUS_ATTACK_NUM)
			{
				ax--;
				*attackPosX = ax;
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
				ay++;
				*attackPosY = ay;
			}
			else if (*attackPosY > 100)
			{
				ay--;
				*attackPosY = ay;
			}
			else if (*attackPosX < 100)
			{
				ax++;
				*attackPosX = ax;
			}
			else if (*attackPosX > 100)
			{
				ax--;
				*attackPosX = ax;
			}
			else
			{
				AliveFlag = false;
				attack = INITIALSTATE;
				playerstate = CHOICE;
				*flag = false;
				PlayerActionNum--;
			}

			break;
		}
	}

	

}

//ユニットがやられた時にテクスチャをフェードアウトさせる
void GameState::UnitFade()
{
	if (AliveFlag == false)
	{
		//ShironekoSprite.SetAlpha(ShironekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
}

//ユニットを選択した時の処理
void GameState::UnitChoice(int x,int y,bool *flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	//カーソルの位置と猫の位置が一致していて
	//エンターキーを押したら処理
	if (CursorX == x && CursorY == y)
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
		}
	}
}

//ユニットを移動させる
void GameState::UnitMove(int *x, int *y,bool flag)
{
	DirectInput* pDi = DirectInput::GetInstance();

	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		//カーソルの位置が-1以上3未満ならば処理
		if (Map[CursorY][CursorX] > -1 && Map[CursorY][CursorX] != 3 && flag == true)
		{
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
void GameState::MapChange()
{
	for (int i = 0; i < 2; i++)
	{
		if (mike[i].AliveFlag == true)
		{
			Map[mike[i].y][mike[i].x] = -3;
		}
		if (kuro[i].AliveFlag == true)
		{
			Map[kuro[i].y][kuro[i].x] = -4;
		}
		if (tyatora[i].AliveFlag == true)
		{
			Map[tyatora[i].y][tyatora[i].x] = -5;
		}
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

		mike[0].MoveFlag = false;
		kuro[0].MoveFlag = false;
		tyatora[0].MoveFlag = false;
		
		//ユニット（猫）がいる場所は-3に
		MapChange();

		//ユニットを選択
		UnitChoice(mike[0].x, mike[0].y, &mike[0].MoveFlag);
		UnitChoice(kuro[0].x, kuro[0].y, &kuro[0].MoveFlag);
		UnitChoice(tyatora[0].x, tyatora[0].y, &tyatora[0].MoveFlag);

		if (PlayerActionNum == 0)
		{
			playerTurn = SecondPlayer_Turn;
		}

		break;
	case MOVE://猫を移動させる

		//選択用のカーソルを移動させる関数
		MoveCursor();

		//選択したユニットを移動
		UnitMove(&mike[0].x, &mike[0].y, mike[0].MoveFlag);
		UnitMove(&kuro[0].x, &kuro[0].y, kuro[0].MoveFlag);
		UnitMove(&tyatora[0].x, &tyatora[0].y, tyatora[0].MoveFlag);

		break;
	case COMMAND://待機、攻撃、戻るといった行動を決めるコマンド選択

		//移動したその場から四方を調べて攻撃可能なマスがあるかを調べる
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//上、下、右、左の何処かに攻撃可能マスがあれば
			//攻撃コマンドを表示させる
			if (Map[AttackSearchY][AttackSearchX] == -6)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3)
			{
				Map[AttackSearchY][AttackSearchX] = -4;
			}

		}

		//コマンドを選択する矢印の移動
		CommandChoice();

		//選択したコマンドの効果を処理する関数
		CommandPlay(&mike[0].x, &mike[0].y,mike[0].MoveFlag);
		CommandPlay(&kuro[0].x, &kuro[0].y, kuro[0].MoveFlag);
		CommandPlay(&tyatora[0].x, &tyatora[0].y, tyatora[0].MoveFlag);

		break;

	case ATTACKCHOICE://攻撃する敵ユニットを決定

		if (AttackUnitChoiceFlag == false)
		{
			//攻撃対象を選択するカーソルを移動させる関数
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -6)
			{
				//フラグをtrueにして移動開始
				AttackUnitChoiceFlag = true;
			}
		}

		//ユニットを決められた方向へ攻撃
		AttackMotion(mike[0].x, mike[0].y, &mike[0].AttackPosX, &mike[0].AttackPosY, &mike[0].MoveFlag);
		AttackMotion(kuro[0].x, kuro[0].y, &kuro[0].AttackPosX, &kuro[0].AttackPosY, &kuro[0].MoveFlag);
		AttackMotion(tyatora[0].x, tyatora[0].y, &tyatora[0].AttackPosX, &tyatora[0].AttackPosY, &tyatora[0].MoveFlag);
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
		
		//ユニット（猫）がいる場所は-3に
		MapChange();

		//ユニットを選択
		UnitChoice(mike[1].x, mike[1].y, &mike[1].MoveFlag);
		UnitChoice(kuro[1].x, kuro[1].y, &kuro[1].MoveFlag);
		UnitChoice(tyatora[1].x, tyatora[1].y, &tyatora[1].MoveFlag);

		break;
	case MOVE://猫を移動させる

		//選択用のカーソルを移動させる関数
		MoveCursor();

		UnitMove(&mike[1].x, &mike[1].y, mike[1].MoveFlag);
		UnitMove(&kuro[1].x, &kuro[1].y, kuro[1].MoveFlag);
		UnitMove(&tyatora[1].x, &tyatora[1].y, tyatora[1].MoveFlag);

		break;
	case COMMAND://待機、攻撃、戻るといった行動を決めるコマンド選択

		//移動したその場から四方を調べて攻撃可能なマスがあるかを調べる
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//上、下、右、左の何処かに攻撃可能マスがあれば
			//攻撃コマンドを表示させる
			if (Map[AttackSearchY][AttackSearchX] == -6)
			{
				AttackCommandDisplay = true;
			}
			else if (Map[AttackSearchY][AttackSearchX] == -3)
			{
				Map[AttackSearchY][AttackSearchX] = -4;
			}

		}

		//コマンドを選択する矢印の移動
		CommandChoice();

		//選択したコマンドの効果を処理する関数
		CommandPlay(&mike[1].x, &mike[1].y, mike[1].MoveFlag);
		CommandPlay(&kuro[1].x, &kuro[1].y, kuro[1].MoveFlag);
		CommandPlay(&tyatora[1].x, &tyatora[1].y, tyatora[1].MoveFlag);

		break;

	case ATTACKCHOICE://攻撃する敵ユニットを決定

		if (AttackUnitChoiceFlag == false)
		{
			//攻撃対象を選択するカーソルを移動させる関数
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -6)
			{
				//フラグをtrueにして移動開始
				AttackUnitChoiceFlag = true;
			}
		}

		AttackMotion(mike[1].x, mike[1].y, &mike[1].AttackPosX, &mike[1].AttackPosY, &mike[1].MoveFlag);
		AttackMotion(kuro[1].x, kuro[1].y, &kuro[1].AttackPosX, &kuro[1].AttackPosY, &kuro[1].MoveFlag);
		AttackMotion(tyatora[1].x, tyatora[1].y, &tyatora[1].AttackPosX, &tyatora[1].AttackPosY, &tyatora[1].MoveFlag);
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
void GameState::CommandPlay(int *x, int *y, bool flag)
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
				playerstate = ATTACKCHOICE;
			}
			break;
		case STANDBY://その場に待機する
			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 550);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				//プレイヤーの行動回数をマイナス
				PlayerActionNum--;
				//行動ユニットを決める処理に戻る
				playerstate = CHOICE;
			}
			break;
		case BACK://元の場所へ戻る
			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				*x = BeforeMapNumX;
				*y = BeforeMapNumY;
				playerstate = CHOICE;
				break;
			}
		}
	}
}