#pragma once

#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

class SoundEffect
{
private:

	//���肵�����̌��ʉ�
	WaveFile DecisionSE;
	SoundBuffer sb_DicisionSE[10];

	//�I���������̌��ʉ�
	WaveFile ChoiceSE;
	SoundBuffer sb_ChoiceSE[10];

	//�U���������̔L�̖���
	WaveFile CatSE;
	SoundBuffer sb_CatSE;

	//�J�b�g�C�����s��ꂽ���̌��ʉ�
	WaveFile CutInSE;
	SoundBuffer sb_CutInSE;

public:
	SoundEffect();
	~SoundEffect();

	void Initialize();
	void Update();

	//���艹��炷�֐�
	void DecisionSEPlay();
	//�I������炷�֐�
	void ChoiceSEPlay();
	//�L�̖�����炷�֐�
	void CatSEPlay();
	//�J�b�g�C���̌��ʉ���炷�֐�
	void CutInSEPlay();

};