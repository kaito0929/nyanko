#include "Title.h"
#include "../directInput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"



Title::Title(ISceneChanger* changer) : BaseScene(changer)
{

}

Title::~Title()
{

}

void Title::Initialize()
{
	//タイトルロゴ
	TitleTex.Load("Material/title.png");
	TitleSprite.SetPos(600, 500);
	TitleSprite.SetSize(1200, 1100);

	//エンターを押すように指示するテクスチャ
	EnterTex.Load("Material/enter.png");
	EnterSprite.SetPos(600, 700);
	EnterSprite.SetSize(600, 400);

	//フェードアウト用のテクスチャ
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(0);

	sound.Initialize();
	se.Initialize();
}


void Title::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	//タイトル画面の描画
	Direct3D::DrawSprite(TitleSprite, TitleTex);

	if (EnterFlashingNum == 0)
	{
		//エンターキーを押すように指示
		Direct3D::DrawSprite(EnterSprite, EnterTex);
	}

	//フェードアウト用の画像を描画
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}

void Title::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	sound.TitleSoundPlay();

	//==■エンターキーを押すように指示を出すテクスチャを点滅させる■====================================
	//カウントをプラス
	EnterDrawCount++;
	//表示非表示をswitchで切り替える
	switch (EnterFlashingNum)
	{
	case 0://テクスチャの表示
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 1;
		}
		break;
	case 1://テクスチャの非表示
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 0;
		}
		break;
	}
	//==================================================================================================


	//エンターキーを押したならフラグをtrueにしてフェードアウト開始
	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		if (FadeFlag == false)
		{
			se.DecisionSEPlay();
		}
		FadeFlag = true;
	}
	//FadeFlagがtrueなら実行するように
	if (FadeFlag == true)
	{
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_OUT_CHANGENUM));
	}

	//完全に画面が暗くなったならシーンをメインゲームに変更
	if (FadeSprite.GetAlpha() == 1)
	{
		sound.TitleSoundStop();
		mSceneChanger->ChangeScene(STATE_MAIN);
	}
}


