#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "GameState.h"

//フェードアウト用の数値
#define FADE_OUT_CHANGENUM 1 
//フェードイン用の数値
#define FADE_IN_CHANGENUM -1 
#define FADE_SPEED 0.01f


class Fade
{
private:

	//フェードイン用のテクスチャ
	Texture FadeTex;
	Sprite FadeSprite;

	//ターン切り替えのカットイン用のテクスチャ(1P)
	Texture CutIn_FirstPlayerTex;
	Sprite CutIn_FirstPlayerSprite;

	//ターン切り替えのカットイン用のテクスチャ(2P)
	Texture CutIn_SecondPlayerTex;
	Sprite CutIn_SecondPlayerSprite;


public:
	
	//コンストラクタ
	Fade();
	//デストラクタ
	~Fade();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();

	bool FadeIn();
	bool FadeOut();

	//カットインのテクスチャのフェードインフェードアウトを行う関数
	void CutInFade(int playerturn,int *fade);



};