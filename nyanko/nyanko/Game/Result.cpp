#include "../directInput.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "Result.h"
#include "GameState.h"

Result::Result(ISceneChanger* changer) : BaseScene(changer)
{

}

Result::~Result()
{

}

void Result::Initialize()
{
	//1P�����������ꍇ�ɕ\��������
	FirstPlayerVictoryTex.Load("Material/result1.png");
	FirstPlayerVictorySprite.SetPos(600, 500);
	FirstPlayerVictorySprite.SetSize(1200, 1100);

	//2P�����������ꍇ�ɕ\��������
	SecondPlayerVictoryTex.Load("Material/result2.png");
	SecondPlayerVictorySprite.SetPos(600, 500);
	SecondPlayerVictorySprite.SetSize(1200, 1100);

	//�G���^�[�������悤�Ɏw������e�N�X�`��
	EnterTex.Load("Material/enter.png");
	EnterSprite.SetPos(600, 700);
	EnterSprite.SetSize(600, 400);

	//�t�F�[�h�A�E�g�p�̃e�N�X�`��
	FadeTex.Load("Material/fade_b.png");
	FadeSprite.SetPos(600, 500);
	FadeSprite.SetSize(1200, 1100);
	FadeSprite.SetAlpha(0);

	sound.Initialize();
	se.Initialize();
}

void Result::Draw()
{
	Direct3D::SetRenderState(RENDER_ALPHABLEND);

	if (SecondPlayerLosingFlag == true)
	{
		Direct3D::DrawSprite(FirstPlayerVictorySprite, FirstPlayerVictoryTex);
	}
	if (FirstPlayerLosingFlag == true)
	{
		Direct3D::DrawSprite(SecondPlayerVictorySprite, SecondPlayerVictoryTex);
	}

	if (EnterFlashingNum == 0)
	{
		//�G���^�[�L�[�������悤�Ɏw��
		Direct3D::DrawSprite(EnterSprite, EnterTex);
	}

	//�t�F�[�h�A�E�g�p�̉摜��`��
	Direct3D::DrawSprite(FadeSprite, FadeTex);

}

void Result::Update()
{
	DirectInput* pDi = DirectInput::GetInstance();

	sound.ResultSoundPlay();

	//==���G���^�[�L�[�������悤�Ɏw�����o���e�N�X�`����_�ł����遡====================================
	//�J�E���g���v���X
	EnterDrawCount++;
	//�\����\����switch�Ő؂�ւ���
	switch (EnterFlashingNum)
	{
	case 0://�e�N�X�`���̕\��
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 1;
		}
		break;
	case 1://�e�N�X�`���̔�\��
		if (EnterDrawCount % 30 == 0)
		{
			EnterFlashingNum = 0;
		}
		break;
	}
	//==================================================================================================

	//�G���^�[�L�[���������Ȃ�t���O��true�ɂ��ăt�F�[�h�A�E�g�J�n
	if (pDi->KeyJustPressed(DIK_RETURN))
	{
		if (FadeFlag == false)
		{
			se.DecisionSEPlay();
		}
		FadeFlag = true;
	}
	//FadeFlag��true�Ȃ���s����悤��
	if (FadeFlag == true)
	{
		FadeSprite.SetAlpha(FadeSprite.GetAlpha() + (0.01f*FADE_OUT_CHANGENUM));
	}

	//���S�ɉ�ʂ��Â��Ȃ����Ȃ�V�[�������C���Q�[���ɕύX
	if (FadeSprite.GetAlpha() == 1)
	{
		sound.ResultSoundStop();
		mSceneChanger->ChangeScene(STATE_TITLE);
	}
}