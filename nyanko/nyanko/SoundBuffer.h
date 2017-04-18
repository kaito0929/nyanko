#pragma once

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>
#include <tchar.h>
#include <comdef.h>

//名前だけ宣言　引数に使う
class WaveFile;

class SoundBuffer
{
private:
	//セカンダリバッファ
	IDirectSoundBuffer8 *pSecondaryBuffer;

public:
	//コンストラクタ　デストラクタ
	SoundBuffer();
	~SoundBuffer();

	//作成
	bool Create(WaveFile& waveFile);

	//再生と停止
	void Play(bool Loop);
	void Stop();

	bool Playing();
	//解放
	void Release();
};