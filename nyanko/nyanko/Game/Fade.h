#pragma once

#include "../mesh.h"
#include "../Direct3D.h"
#include "../Texture.h"
#include "../Sprite.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

#include "GameState.h"

//�t�F�[�h�A�E�g�p�̐��l
#define FADE_OUT_CHANGENUM 1 
//�t�F�[�h�C���p�̐��l
#define FADE_IN_CHANGENUM -1 
#define FADE_SPEED 0.01f


class Fade
{
private:

	//�t�F�[�h�C���p�̃e�N�X�`��
	Texture FadeTex;
	Sprite FadeSprite;

	//�^�[���؂�ւ��̃J�b�g�C���p�̃e�N�X�`��(1P)
	Texture CutIn_FirstPlayerTex;
	Sprite CutIn_FirstPlayerSprite;

	//�^�[���؂�ւ��̃J�b�g�C���p�̃e�N�X�`��(2P)
	Texture CutIn_SecondPlayerTex;
	Sprite CutIn_SecondPlayerSprite;


public:
	
	//�R���X�g���N�^
	Fade();
	//�f�X�g���N�^
	~Fade();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();

	bool FadeIn();
	bool FadeOut();

	//�J�b�g�C���̃e�N�X�`���̃t�F�[�h�C���t�F�[�h�A�E�g���s���֐�
	void CutInFade(int playerturn,int *fade);



};