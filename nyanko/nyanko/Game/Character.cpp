#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"


//=====================================================
//■Characterクラス■
//=====================================================
//コンストラクタ
Character::Character()
{

}
//デストラクタ
Character::~Character()
{

}

//=====================================================
//■Mikenekoクラス■
//=====================================================
//コンストラクタ
Mikeneko::Mikeneko()
{

}
//デストラクタ
Mikeneko::~Mikeneko()
{

}
//初期化関数
void Mikeneko::Initialize()
{

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;

	cat = MIKE;

	//三毛猫のテクスチャの初期化
	MikenekoTex[0].Load("Material/mikeneko.png");
	MikenekoTex[1].Load("Material/mikeneko2.png");
	//座標の設定
	MikenekoSprite.SetPos(x * 200.0f, y * 150.0f);
	//大きさの設定
	MikenekoSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//キャラの体力は3
	HitPoint = 3;

	MovePossidleFlag = true;
}
//描画関数
void Mikeneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);


	//移動した後だったり相手のターンならばユニットを白黒に
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//実際に動かす関数
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//ヒットポイントのハートの位置を決定
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//ヒットポイントが0以下になればユニットは消滅
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	//ユニットがフェードアウトする関数
	UnitFade();
}

//ユニットがやられた時にテクスチャをフェードアウトさせる
void Mikeneko::UnitFade()
{
	if (AliveFlag == false)
	{
		MikenekoSprite.SetAlpha(MikenekoSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
	}
}

//攻撃する相手によって処理を変える関数
void Mikeneko::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1が三毛猫の場合に処理する戦闘結果
		if (cat == TYATORA)
		{
			*hp -= 2;
		}
		else if (cat == MIKE)
		{
			*hp -= 1;
		}
	}
}

//=====================================================
//■Kuronekoクラス■
//=====================================================
//コンストラクタ
Kuroneko::Kuroneko()
{

}
//デストラクタ
Kuroneko::~Kuroneko()
{

}
//初期化関数
void Kuroneko::Initialize()
{

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;


	cat = KURO;

	//黒猫のテクスチャの初期化
	KuronekoTex[0].Load("Material/kuroneko.png");
	KuronekoTex[1].Load("Material/kuroneko2.png");
	//座標の設定
	KuronekoSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	KuronekoSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//キャラの体力は3
	HitPoint = 3;

	MovePossidleFlag = true;
	
}
//描画関数
void Kuroneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//移動した後だったり相手のターンならばユニットを白黒に
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//実際に動かす関数
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//ヒットポイントのハートの位置を決定
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//ヒットポイントが0以下になればユニットは消滅
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}
	//ユニットがフェードアウトする関数S
	UnitFade();
}

void Kuroneko::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1が黒猫の場合に処理する戦闘結果
		if (cat == MIKE)
		{
			*hp -= 2;
		}
		else if (cat == KURO)
		{
			*hp -= 1;
		}
	}
}

void Kuroneko::UnitFade()
{
	if (AliveFlag == false)
	{
		KuronekoSprite.SetAlpha(KuronekoSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
	}
}

//=====================================================
//■Tyatoraクラス■
//=====================================================
//コンストラクタ
Tyatora::Tyatora()
{

}
//デストラクタ
Tyatora::~Tyatora()
{

}
//初期化関数
void Tyatora::Initialize()
{

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;
	MoveFlag = false;
	AttackChoiceFlag = false;

	cat = TYATORA;

	//茶トラのテクスチャの初期化
	TyatoraTex[0].Load("Material/tyatora.png");
	TyatoraTex[1].Load("Material/tyatora2.png");
	//座標の設定
	TyatoraSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	TyatoraSprite.SetSize(100, 100);

	HitPointTex.Load("Material/heart_gloss.png");
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetSize(30, 30);
		HitPointSprite[i].SetPos(x, y);
	}

	HitPointPos[0] = 0.7f;
	HitPointPos[1] = 1.0f;
	HitPointPos[2] = 1.3f;

	//キャラの体力は3
	HitPoint = 3;

	MovePossidleFlag = true;
}
//描画関数
void Tyatora::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//移動した後だったり相手のターンならばユニットを白黒に
	if (MovePossidleFlag == true)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex[0]);
	}
	else if (MovePossidleFlag == false)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex[1]);
	}

	for (int i = 0; i < HitPoint; i++)
	{
		Direct3D::DrawSprite(HitPointSprite[i], HitPointTex);
	}
}
//実際に動かす関数
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	//ヒットポイントのハートの位置を決定
	for (int i = 0; i < 3; i++)
	{
		HitPointSprite[i].SetPos((x + HitPointPos[i]) * AttackPosX, (y + 1.5f) * AttackPosY);
	}

	//ヒットポイントが0以下になればユニットは消滅
	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}
	//ユニットがフェードアウトする関数S
	UnitFade();
}

void Tyatora::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1が黒猫の場合に処理する戦闘結果
		if (cat == KURO)
		{
			*hp -= 2;
		}
		else if (cat == TYATORA)
		{
			*hp -= 1;
		}
	}
}

void Tyatora::UnitFade()
{
	if (AliveFlag == false)
	{
		TyatoraSprite.SetAlpha(TyatoraSprite.GetAlpha() + (0.1f*CAT_FADE_OUT_CHANGENUM));
	}
}