#pragma once

#include "../Direct3D.h"
#include "../Wave.h"
#include "../DirectSound.h"
#include "../SoundBuffer.h"

class SoundEffect
{
private:

	//Œˆ’è‚µ‚½‚ÌŒø‰Ê‰¹
	WaveFile DecisionSE;
	SoundBuffer sb_DicisionSE[10];

	//‘I‘ğ‚µ‚½‚ÌŒø‰Ê‰¹
	WaveFile ChoiceSE;
	SoundBuffer sb_ChoiceSE[10];

	//UŒ‚‚µ‚½‚Ì”L‚Ì–Â‚«º
	WaveFile CatSE;
	SoundBuffer sb_CatSE;


public:
	SoundEffect();
	~SoundEffect();

	void Initialize();
	void Update();

	//Œˆ’è‰¹‚ğ–Â‚ç‚·ŠÖ”
	void DecisionSEPlay();
	//‘I‘ğ‰¹‚ğ–Â‚ç‚·ŠÖ”
	void ChoiceSEPlay();
	//”L‚Ì–Â‚«º‚ğ–Â‚ç‚·ŠÖ”
	void CatSEPlay();

};