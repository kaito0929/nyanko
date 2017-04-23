#include "Title.h"
#include "../directInput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"


Title::Title()
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
	TitleSprite.SetSize(1200, 1000);

	//エンターを押すように指示するテクスチャ
	EnterTex.Load("Material/enter.png");
	EnterSprite.SetPos(600, 700);
	EnterSprite.SetSize(600, 400);
}


void Title::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHATEST);
	Direct3D::DrawSprite(TitleSprite, TitleTex);

	Direct3D::DrawSprite(EnterSprite, EnterTex);
}

void Title::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

}


