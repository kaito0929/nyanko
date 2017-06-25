#include "Fade.h"

Fade::Fade()
{

}

Fade::~Fade()
{

}

void Fade::Initialize()
{
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

}

void Fade::Draw()
{
	//1Pのカットイン描画
	Direct3D::DrawSprite(CutIn_FirstPlayerSprite, CutIn_FirstPlayerTex);
	//2Pのカットイン描画
	Direct3D::DrawSprite(CutIn_SecondPlayerSprite, CutIn_SecondPlayerTex);
	//フェードアウト用の画像を描画
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}

void Fade::Update()
{

}

bool Fade::FadeIn()
{
	//フェードインを行う
	FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (FADE_SPEED*FADE_IN_CHANGENUM));

	if (FadeSprite.GetAlpha() == 0)
	{
		return true;
	}

	return false;
}

bool Fade::FadeOut()
{
	//フェードアウトを行う
	FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (FADE_SPEED*FADE_OUT_CHANGENUM));

	if (FadeSprite.GetAlpha() == 1)
	{
		return true;
	}

	return false;
}

void Fade::CutInFade(int playerturn, int *fade)
{
	//全体的なフェードインフェードアウト用の画像のα値が0ならば処理されるように
	if (FadeIn()==true)
	{
		//fadeによってフェードイン、フェードアウトを切り替える
		switch (*fade)
		{
		case FADE_IN://フェードインを行う

			//playerTurnによってフェードインを行う画像を切り替える
			if (playerturn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}
			else if (playerturn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}

			break;
		case FADE_OUT://フェードアウトを行う

			//playerTurnによってフェードインを行う画像を切り替える
			if (playerturn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));
				//α値が1にまで加算されたらPlayerTurnをFADE_INに切り替えてフェードインを行うように
				if (CutIn_FirstPlayerSprite.GetAlpha() == 1)
				{
					*fade = FADE_IN;
				}
			}
			//こっちの内容も上記と同じなのでコメントは上記を参照してください
			else if (playerturn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));

				if (CutIn_SecondPlayerSprite.GetAlpha() == 1)
				{
					*fade = FADE_IN;
				}
			}
			break;
		}
	}
}