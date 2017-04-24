#pragma once

#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

class SoundEffect
{
private:

	//決定した時の効果音
	WaveFile DecisionSE;
	SoundBuffer sb_DicisionSE[10];

	//選択した時の効果音
	WaveFile ChoiceSE;
	SoundBuffer sb_ChoiceSE[10];

	//攻撃した時の猫の鳴き声
	WaveFile CatSE;
	SoundBuffer sb_CatSE;

	//カットインが行われた時の効果音
	WaveFile CutInSE;
	SoundBuffer sb_CutInSE;

public:
	SoundEffect();
	~SoundEffect();

	void Initialize();
	void Update();

	//決定音を鳴らす関数
	void DecisionSEPlay();
	//選択音を鳴らす関数
	void ChoiceSEPlay();
	//猫の鳴き声を鳴らす関数
	void CatSEPlay();
	//カットインの効果音を鳴らす関数
	void CutInSEPlay();

};