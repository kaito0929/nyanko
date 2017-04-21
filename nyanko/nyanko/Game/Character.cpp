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

//マップの数値を変える関数
void Character::MapChange(int *map[9][9],int x,int y,bool flag)
{
	if (flag == true)
	{
		*map[y][x] = -3;
	}
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
	x = 4;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//三毛猫のテクスチャの初期化
	MikenekoTex.Load("Material/mikeneko.png");
	//座標の設定
	MikenekoSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	MikenekoSprite.SetSize(100, 100);
}
//描画関数
void Mikeneko::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(MikenekoSprite, MikenekoTex);
	}
}
//実際に動かす関数
void Mikeneko::Update()
{
	MikenekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
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
	x = 5;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//黒猫のテクスチャの初期化
	KuronekoTex.Load("Material/kuroneko.png");
	//座標の設定
	KuronekoSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	KuronekoSprite.SetSize(100, 100);
}
//描画関数
void Kuroneko::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(KuronekoSprite, KuronekoTex);
	}
}
//実際に動かす関数
void Kuroneko::Update()
{
	KuronekoSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
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
	x = 3;
	y = 3;

	AttackPosX = 100;
	AttackPosY = 100;

	AliveFlag = true;

	//茶トラのテクスチャの初期化
	TyatoraTex.Load("Material/tyatora.png");
	//座標の設定
	TyatoraSprite.SetPos(x * 200, y * 150);
	//大きさの設定
	TyatoraSprite.SetSize(100, 100);
}
//描画関数
void Tyatora::Draw()
{
	if (AliveFlag == true)
	{
		Direct3D::DrawSprite(TyatoraSprite, TyatoraTex);
	}
}
//実際に動かす関数
void Tyatora::Update()
{
	TyatoraSprite.SetPos((x + 1) * AttackPosX, (y + 1) * AttackPosY);
}