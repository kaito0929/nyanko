#include "Sprite.h"

//コンストラクタ
Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = height = 0;
	rotate = 0.0f;
	alpha = 1.0f;
}
//デストラクタ
Sprite::~Sprite()
{
	//メモリの開放などの必要な死
}

//中心座標を設定(スクリーン座標)
void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}
//サイズの設定
void Sprite::SetSize(int Width, int Height)
{
	width = Width;
	height = Height;
}
//回転の設定
void Sprite::SetRotate(float Rotate)
{
	rotate = Rotate;
}

void  Sprite::SetAlpha(float a)
{
	if (a < 0.0f)
	{
		alpha = 0.0f;
	}
	else if (a>1.0f)
	{
		alpha = 1;
	}
	else
	{
		alpha = a;
	}

}