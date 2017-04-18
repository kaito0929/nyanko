#include "GameState.h"
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
	mike.x = 4;
	mike.y = 3;
	mike.AttackPosX = 100;
	mike.AttackPosY = 100;

	tyatora.x = 3;
	tyatora.y = 3;
	tyatora.AttackPosX = 100;
	tyatora.AttackPosY = 100;

	kuro.x = 5;
	kuro.y = 3;
	kuro.AttackPosX = 100;
	kuro.AttackPosY = 100;

	shiro.x = 2;
	shiro.y = 2;
	shiro.AttackPosX = 100;
	shiro.AttackPosY = 100;

	SearchX = 0;
	SearchX = 0;
	
	CursorX = 1;
	CursorY = 1;

	MoveCount = 2;

	flag[0] == false;
	flag[1] == false;
	flag[2] == false;

	//三毛猫のテクスチャの初期化
	MikenekoTex.Load("Material/mikeneko.png");
	MikenekoSprite.SetPos(mike.x * 200, mike.y * 150);
	MikenekoSprite.SetSize(100, 100);

	//茶トラのテクスチャの初期化
	TyatoraTex.Load("Material/tyatora.png");
	TyatoraSprite.SetPos(tyatora.x * 200, tyatora.y * 150);
	TyatoraSprite.SetSize(100, 100);

	//黒猫のテクスチャの初期化
	KuronekoTex.Load("Material/kuroneko.png");
	KuronekoSprite.SetPos(kuro.x * 200, kuro.y * 150);
	KuronekoSprite.SetSize(100, 100);

	//白猫のテクスチャの初期化
	ShironekoTex.Load("Material/sironeko.png");
	ShironekoSprite.SetPos(shiro.x * 200, shiro.y * 150);
	ShironekoSprite.SetSize(100, 100);

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
				Direct3D::DrawSprite(MoveSquaresSprite, MoveSquaresTex);
			}
			else if (Map[y][x] == -4)
			{
				Direct3D::DrawSprite(AttackSquaresSprite, AttackSquaresTex);
			}

			Direct3D::DrawSprite(CursorSprite, CursorTex);
		}
	}

	Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
	Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
	Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
	Direct3D::DrawSprite(ShironekoSprite, ShironekoTex);

	if (playerstate == COMMAND)
	{
		if (AttackCommandDisplay == true)
		{
			Direct3D::DrawSprite(AttackCommandSprite, AttackCommandTex);
		}
		Direct3D::DrawSprite(StandbyCommandSprite, StandbyCommandTex);
		Direct3D::DrawSprite(BackCommandSprite, BackCommandTex);
		Direct3D::DrawSprite(TheArrowSprite, TheArrowTex);
	}
}

void GameState::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	CursorSprite.SetPos((CursorX + 1) * 100, (CursorY + 1) * 100);
	MikenekoSprite.SetPos((mike.x + 1) * mike.AttackPosX, (mike.y + 1) * mike.AttackPosY);
	TyatoraSprite.SetPos((tyatora.x + 1) * tyatora.AttackPosX, (tyatora.y + 1) * tyatora.AttackPosY);
	KuronekoSprite.SetPos((kuro.x + 1) * kuro.AttackPosX, (kuro.y + 1) * kuro.AttackPosY);
	ShironekoSprite.SetPos((shiro.x + 1) * 100, (shiro.y + 1) * 100);
	
	//=■移動可能範囲の表示関数■================================

	Search();

	//===========================================================

	switch (playerstate)
	{
	case CHOICE://どの猫を動かすか決める

		//攻撃コマンドの表示フラグはfalseにしておく
		AttackCommandDisplay = false;
		//選択用のカーソルを動かす関数
		MoveCursor();

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

		for (int i = 0; i < 3; i++)
		{
			flag[i] = false;
		}

		if (AliveFlag == true)
		{
			//ユニット（猫）がいる場所は-3に
			Map[mike.y][mike.x] = -3;
			Map[shiro.y][shiro.x] = -3;
			Map[tyatora.y][tyatora.x] = -3;
			Map[kuro.y][kuro.x] = -3;
		}

		//ユニットを選択
		UnitChoice(mike.x, mike.y,&flag[0]);
		UnitChoice(tyatora.x, tyatora.y, &flag[1]);
		UnitChoice(kuro.x, kuro.y, &flag[2]);
		
		break;
	case MOVE://猫を移動させる

		//選択用のカーソルを移動させる関数
		MoveCursor();

		UnitMove(&mike.x, &mike.y,flag[0]);
		UnitMove(&tyatora.x, &tyatora.y,flag[1]);
		UnitMove(&kuro.x, &kuro.y, flag[2]);

		break;
	case COMMAND://待機、攻撃、戻るといった行動を決めるコマンド選択

		//移動したその場から四方を調べて攻撃可能なマスがあるかを調べる
		for (int i = 0; i < CHACKNUM; i++)
		{
			AttackSearchX = CursorX + ChackDirectionX[i];
			AttackSearchY = CursorY + ChackDirectionY[i];

			//上、下、右、左の何処かに攻撃可能マスがあれば
			//攻撃コマンドを表示させる
			if (Map[AttackSearchY][AttackSearchX] == -4)
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
				playerstate = CHOICE;
			}
			break;
		case BACK://元の場所へ戻る
			//矢印の位置を変更
			TheArrowSprite.SetPos(650, 650);
			if (pDi->KeyJustPressed(DIK_RETURN))
			{
				mike.x = BeforeMapNumX;
				mike.y = BeforeMapNumY;
				playerstate = CHOICE;
			}
			break;
		}

		break;

	case ATTACKCHOICE://攻撃する敵ユニットを決定
		
		if (AttackUnitChoiceFlag == false)
		{
			//攻撃対象を選択するカーソルを移動させる関数
			MoveCursor();
		}

		if (pDi->KeyJustPressed(DIK_RETURN))
		{
			if (Map[CursorY][CursorX] == -4)
			{
				//フラグをtrueにして移動開始
				AttackUnitChoiceFlag = true;
			}
		}

		AttackMotion(mike.x, mike.y, &mike.AttackPosX, &mike.AttackPosY, &flag[0]);
		AttackMotion(tyatora.x, tyatora.y, &tyatora.AttackPosX, &tyatora.AttackPosY, &flag[1]);
		AttackMotion(kuro.x, kuro.y, &kuro.AttackPosX, &kuro.AttackPosY, &flag[2]);
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
					//三毛猫の隣のマスを調べる
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
		ShironekoSprite.SetAlpha(ShironekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
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