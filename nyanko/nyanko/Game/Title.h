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

	//�^�C�g���̃e�N�X�`��
	Texture TitleTex;
	Sprite TitleSprite;

	//�G���^�[�L�[�������Ă��炤�w���̃e�N�X�`��
	Texture EnterTex;
	Sprite EnterSprite;


public:

	Title();
	~Title();

	//������
	void Initialize();
	//���ۂ̓���
	void Update();
	//�`��
	void Draw();

};