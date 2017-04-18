#pragma once

#define DIRECTSOUND_VERSION 0x800

#pragma comment(lib,"dsound.lib")//ダイレクトサウンド
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>//ダイレクトサウンド
#include <tchar.h>
#include <comdef.h>

class DirectSound
{
private:
	//シングルトンにする	インスタンスを割り当てるポインタ
	static DirectSound* pInstance;

	//サウンドデバイス
	IDirectSound8* pDirectSound8;

	void Release();

	//コンストラクタ //デストラクタ
	//シングルトンにするためにクラス外から実行できないようにする
	DirectSound();
	~DirectSound();

	void operator=(const DirectSound& obj){}	//代入演算
	DirectSound(const DirectSound& obj){};		//コピーコンストラクタ

public:
	//インスタンスの取得
	static DirectSound* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectSound();
		}
		return pInstance;
	}

	bool Create(HWND hWnd);//デバイス作成

	HRESULT CreateSoundBuffer(DSBUFFERDESC * pDesc, IDirectSoundBuffer** ppBuffer);


};