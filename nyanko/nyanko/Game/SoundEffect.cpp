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
		//���肵�����̌��ʉ�
		DecisionSE.Load("Sound/decision.wav");
		sb_DicisionSE[i].Create(DecisionSE);

		//�I���������̌��ʉ�
		ChoiceSE.Load("Sound/choice.wav");
		sb_ChoiceSE[i].Create(ChoiceSE);
	}

	//�L�̖���
	CatSE.Load("Sound/cat7.wav");
	sb_CatSE.Create(CatSE);


}

void SoundEffect::Update()
{

}

//���艹��炷�֐�
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

//�I������炷�֐�
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

//�L�̖�����炷�֐�
void SoundEffect::CatSEPlay()
{
	sb_CatSE.Play(false);
}

