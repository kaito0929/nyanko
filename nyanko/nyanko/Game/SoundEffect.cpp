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

	//決定した時の効果音
	DecisionSE.Load("Sound/decision.wav");
	sb_DicisionSE[0].Create(DecisionSE);
	sb_DicisionSE[1].Create(DecisionSE);
	sb_DicisionSE[2].Create(DecisionSE);
	sb_DicisionSE[3].Create(DecisionSE);
	sb_DicisionSE[4].Create(DecisionSE);
	sb_DicisionSE[5].Create(DecisionSE);
	sb_DicisionSE[6].Create(DecisionSE);
	sb_DicisionSE[7].Create(DecisionSE);
	sb_DicisionSE[8].Create(DecisionSE);
	sb_DicisionSE[9].Create(DecisionSE);

	//選択した時の効果音
	ChoiceSE.Load("Sound/choice.wav");
	sb_ChoiceSE[0].Create(ChoiceSE);
	sb_ChoiceSE[1].Create(ChoiceSE);
	sb_ChoiceSE[2].Create(ChoiceSE);
	sb_ChoiceSE[3].Create(ChoiceSE);
	sb_ChoiceSE[4].Create(ChoiceSE);
	sb_ChoiceSE[5].Create(ChoiceSE);
	sb_ChoiceSE[6].Create(ChoiceSE);
	sb_ChoiceSE[7].Create(ChoiceSE);
	sb_ChoiceSE[8].Create(ChoiceSE);
	sb_ChoiceSE[9].Create(ChoiceSE);

	//猫の鳴き声
	CatSE.Load("Sound/cat7.wav");
	sb_CatSE.Create(CatSE);

	//カットインの効果音
	CutInSE.Load("Sound/cutin.wav");
	sb_CutInSE.Create(CutInSE);

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

//カットインの効果音を鳴らす関数
void SoundEffect::CutInSEPlay()
{
	sb_CutInSE.Play(false);
}