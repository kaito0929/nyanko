#include "../directInput.h"
#include "GameState.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"
#include "SoundEffect.h"

SoundEffect::SoundEffect()
{

}

SoundEffect::~SoundEffect()
{
	
}

void SoundEffect::Initialize()
{

	for (int i = 0; i < 10; i++)
	{
		//Œˆ’è‚µ‚½Žž‚ÌŒø‰Ê‰¹
		DecisionSE.Load("Sound/decision.wav");
		sb_DicisionSE[i].Create(DecisionSE);

		//‘I‘ð‚µ‚½Žž‚ÌŒø‰Ê‰¹
		ChoiceSE.Load("Sound/choice.wav");
		sb_ChoiceSE[i].Create(ChoiceSE);
	}

	//”L‚Ì–Â‚«º
	CatSE.Load("Sound/cat7.wav");
	sb_CatSE.Create(CatSE);


}

void SoundEffect::Update()
{

}

//Œˆ’è‰¹‚ð–Â‚ç‚·ŠÖ”
void SoundEffect::DecisionSEPlay()
{
	for (int i = 0; i < 10; i++)
	{
		if (sb_DicisionSE[i].Playing() == false)
		{
			sb_DicisionSE[i].Play(false);
			break;
		}
	}
}

//‘I‘ð‰¹‚ð–Â‚ç‚·ŠÖ”
void SoundEffect::ChoiceSEPlay()
{
	for (int i = 0; i < 10; i++)
	{
		if (sb_ChoiceSE[i].Playing() == false)
		{
			sb_ChoiceSE[i].Play(false);
			break;
		}
	}
}

//”L‚Ì–Â‚«º‚ð–Â‚ç‚·ŠÖ”
void SoundEffect::CatSEPlay()
{
	sb_CatSE.Play(false);
}

