#include "GameState.h"
#include "Character.h"
#include "../directInput.h"
#include "../Xinput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../mesh.h"
#include "../Direct3D.h"

int h = 3;

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
	MikenekoTex.Load("Material/mikeneko.png");
	//座標の設定
	MikenekoSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	MikenekoSprite.SetSize(100, 100);

	//キャラの体力は3
	HitPoint = 3;
}
//描画関数
void Mikeneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
}
//実際に動かす関数
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	UnitFade();
}

//ユニットがやられた時にテクスチャをフェードアウトさせる
void Mikeneko::UnitFade()
{
	if (AliveFlag == false)
	{
		MikenekoSprite.SetAlpha(MikenekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
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
			//h - 2;
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
	KuronekoTex.Load("Material/kuroneko.png");
	//座標の設定
	KuronekoSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	KuronekoSprite.SetSize(100, 100);

	//キャラの体力は3
	HitPoint = 3;
	
}
//描画関数
void Kuroneko::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
}
//実際に動かす関数
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

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
		KuronekoSprite.SetAlpha(KuronekoSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
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
	TyatoraTex.Load("Material/tyatora.png");
	//座標の設定
	TyatoraSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	TyatoraSprite.SetSize(100, 100);

	//キャラの体力は3
	HitPoint = 3;
}
//描画関数
void Tyatora::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);
	Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
}
//実際に動かす関数
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);

	if (HitPoint <= 0)
	{
		AliveFlag = false;
	}

	UnitFade();
}

void Tyatora::UnitBattle(CatState cat, int *hp, bool attackFlag)
{
	if (attackFlag == true && MoveFlag == true)
	{
		//cat1が茶トラの場合に処理する戦闘結果
		if (cat == KURO)
		{
			*hp - 2;
		}
		else if (cat == TYATORA)
		{
			*hp - 1;
		}
	}
}

void Tyatora::UnitFade()
{
	if (AliveFlag == false)
	{
		TyatoraSprite.SetAlpha(TyatoraSprite.GetAlpha() + (0.1f*FADE_OUT_CHANGENUM));
	}
}