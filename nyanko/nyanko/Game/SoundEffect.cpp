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
		//決定した時の効果音
		DecisionSE.Load("Sound/decision.wav");
		sb_DicisionSE[i].Create(DecisionSE);

		//選択した時の効果音
		ChoiceSE.Load("Sound/choice.wav");
		sb_ChoiceSE[i].Create(ChoiceSE);
	}

	//猫の鳴き声
	CatSE.Load("Sound/cat7.wav");
	sb_CatSE.Create(CatSE);


}

void SoundEffect::Update()
{

}

//決定音を鳴らす関数
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

//選択音を鳴らす関数
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

//猫の鳴き声を鳴らす関数
void SoundEffect::CatSEPlay()
{
	sb_CatSE.Play(false);
}

