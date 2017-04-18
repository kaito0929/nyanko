#include "DirectSound.h"

DirectSound* DirectSound::pInstance = NULL;

//コンストラクタ
DirectSound::DirectSound()
{
	pDirectSound8 = NULL;
}

//デストラクタ
DirectSound::~DirectSound()
{
	Release();
}

//解放
void DirectSound:: Release()
{
	if (pDirectSound8 != NULL)
	{
		pDirectSound8->Release();
		pDirectSound8 = NULL;
	}
}

//サウンドデバイスの作成と協調レベルの設定
bool DirectSound::Create(HWND hWnd)
{
	//デバイスの作成
	if (FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
	{
		//失敗
		Release();
		return false;
	}

	//協調レベルの設定
	//また音を鳴らすウィンドウの設定
	if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		//失敗
		Release();
		return false;
	}
	return true;
}

HRESULT DirectSound::CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer)
{
	if (pDirectSound8 == NULL)
	{
		//デバイスが作成されていないので
		//バッファの作成が出来ない
		return S_FALSE;
	}

	//サウンドバッファの作成
	//ppBufferの指す先のポインタに　サウンドバッファが記録されているアドレスが代入される
	return pDirectSound8->CreateSoundBuffer(pDesc, ppBuffer, NULL);
}