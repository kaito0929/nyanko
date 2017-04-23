#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

class Title
{
private:

	//タイトルのテクスチャ
	Texture TitleTex;
	Sprite TitleSprite;

	//エンターキーを押してもらう指示のテクスチャ
	Texture EnterTex;
	Sprite EnterSprite;


public:

	Title();
	~Title();

	//初期化
	void Initialize();
	//実際の動き
	void Update();
	//描画
	void Draw();

};