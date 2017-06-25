#include "Fade.h"

Fade::Fade()
{

}

Fade::~Fade()
{

}

void Fade::Initialize()
{
	//�J�b�g�C���p�̃e�N�X�`���i1P�j
	CutIn_FirstPlayerTex.Load("Material/turn1.png");
	CutIn_FirstPlayerSprite.SetSize(1200, 600);
	CutIn_FirstPlayerSprite.SetPos(600, 500);
	CutIn_FirstPlayerSprite.SetAlpha(1);

	//�J�b�g�C���p�̃e�N�X�`���i2P�j
	CutIn_SecondPlayerTex.Load("Material/turn2.png");
	CutIn_SecondPlayerSprite.SetSize(1200, 600);
	CutIn_SecondPlayerSprite.SetPos(600, 500);
	CutIn_SecondPlayerSprite.SetAlpha(0);

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(1);

}

void Fade::Draw()
{
	//1P�̃J�b�g�C���`��
	Direct3D::DrawSprite(CutIn_FirstPlayerSprite, CutIn_FirstPlayerTex);
	//2P�̃J�b�g�C���`��
	Direct3D::DrawSprite(CutIn_SecondPlayerSprite, CutIn_SecondPlayerTex);
	//�t�F�[�h�A�E�g�p�̉摜��`��
	Direct3D::DrawSprite(FadeSprite, FadeTex);
}

void Fade::Update()
{

}

bool Fade::FadeIn()
{
	//�t�F�[�h�C�����s��
	FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (FADE_SPEED*FADE_IN_CHANGENUM));

	if (FadeSprite.GetAlpha() == 0)
	{
		return true;
	}

	return false;
}

bool Fade::FadeOut()
{
	//�t�F�[�h�A�E�g���s��
	FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (FADE_SPEED*FADE_OUT_CHANGENUM));

	if (FadeSprite.GetAlpha() == 1)
	{
		return true;
	}

	return false;
}

void Fade::CutInFade(int playerturn, int *fade)
{
	//�S�̓I�ȃt�F�[�h�C���t�F�[�h�A�E�g�p�̉摜�̃��l��0�Ȃ�Ώ��������悤��
	if (FadeIn()==true)
	{
		//fade�ɂ���ăt�F�[�h�C���A�t�F�[�h�A�E�g��؂�ւ���
		switch (*fade)
		{
		case FADE_IN://�t�F�[�h�C�����s��

			//playerTurn�ɂ���ăt�F�[�h�C�����s���摜��؂�ւ���
			if (playerturn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}
			else if (playerturn == SecondPlayer_Turn)
			{
				CutIn_SecondPlayerSprite.SetAlpha(CutIn_SecondPlayerSprite.GetAlpha() + (0.03f* FADE_IN_CHANGENUM));
			}

			break;
		case FADE_OUT://�t�F�[�h�A�E�g���s��

			//playerTurn�ɂ���ăt�F�[�h�C�����s���摜��؂�ւ���
			if (playerturn == FirstPlayer_Turn)
			{
				CutIn_FirstPlayerSprite.SetAlpha(CutIn_FirstPlayerSprite.GetAlpha() + (0.03f* FADE_OUT_CHANGENUM));
				//���l��1�ɂ܂ŉ��Z���ꂽ��PlayerTurn��FADE_IN�ɐ؂�ւ��ăt�F�[�h�C�����s���悤��
				if (CutIn_FirstPlayerSprite.GetAlpha() == 1)
				{
					*fade = FADE_IN;
				}
			}
			//�������̓��e����L�Ɠ����Ȃ̂ŃR�����g�͏�L���Q�Ƃ��Ă�������
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