#include "../directInput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "Result.h"
#include "GameState.h"

Result::Result(ISceneChanger* changer) : BaseScene(changer)
{

}

Result::~Result()
{

}

void Result::Initialize()
{
	//1Pが勝利した場合に表示する画面
	FirstPlayerVictoryTex.Load("Material/result1.png");
	FirstPlayerVictorySprite.SetPos(600, 500);
	FirstPlayerVictorySprite.SetSize(1200, 1100);

	//2Pが勝利した場合に表示する画面
	SecondPlayerVictoryTex.Load("Material/result2.png");
	SecondPlayerVictorySprite.SetPos(600, 500);
	SecondPlayerVictorySprite.SetSize(1200, 1100);

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

void Result::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	if (SecondPlayerLosingFlag == true)
	{
		Direct3D::DrawSprite(FirstPlayerVictorySprite, FirstPlayerVictoryTex);
	}
	if (FirstPlayerLosingFlag == true)
	{
		Direct3D::DrawSprite(SecondPlayerVictorySprite, SecondPlayerVictoryTex);
	}

	if (EnterFlashingNum == 0)
	{
		//エンターキーを押すように指示
		Direct3D::DrawSprite(EnterSprite, EnterTex);
	}

	//フェードアウト用の画像を描画
	Direct3D::DrawSprite(FadeSprite, FadeTex);

}

void Result::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	sound.ResultSoundPlay();

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
		sound.ResultSoundStop();
		mSceneChanger->ChangeScene(STATE_TITLE);
	}
}