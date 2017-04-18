#include "SoundBuffer.h"
#include "Wave.h"
#include "DirectSound.h"

//コンストラクタ
SoundBuffer::SoundBuffer()
{
	pSecondaryBuffer = NULL;
}

//デストラクタ
SoundBuffer::~SoundBuffer()
{
	Release();
}

//解放処理
void SoundBuffer::Release()
{
	//セカンダリバッファにデータがあるなら
	if (pSecondaryBuffer != NULL)
	{
		//解放の前に再生停止する
		Stop();

		//解放
		pSecondaryBuffer->Release();
		//解放済みのポインタにNULLを代入
		pSecondaryBuffer = NULL;
	}
}

//バッファ作成
//読み込み済みのwaveFileクラスの参照が引数
bool SoundBuffer::Create(WaveFile& waveFile)
{
	//セカンダリバッファ作成用設定
	//desc : description
	DSBUFFERDESC desc = {};

	//チャンネルの数で分岐
	//モノラル　1チャンネル
	//ステレオ　2チャンネル

	if (waveFile.waveFormat.nChannels==1)
	{
		//モノラル
		desc.dwFlags = 
			DSBCAPS_CTRL3D |
			DSBCAPS_CTRLVOLUME |
			DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_CTRLPOSITIONNOTIFY |
			DSBCAPS_STATIC;

		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else /*if (waveFile.waveFormat.nChannels == 2)*///3チャンネル以上のステレオもあるようなのでなし
	{
		//ステレオ
		desc.dwFlags =
			//DSBCAPS_CTRL3D |//モノラルから抜く
			DSBCAPS_CTRLVOLUME |
			DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_CTRLPOSITIONNOTIFY |
			DSBCAPS_STATIC |
			DSBCAPS_CTRLPAN;//ステレオはこの最後の行が増えただけ

		desc.guid3DAlgorithm = GUID_NULL;
	}
	
	desc.dwSize = sizeof(DSBUFFERDESC);
	//波形データサイズ
	desc.dwBufferBytes = waveFile.dataSize;
	//waveファイルフォーマット指定
	desc.lpwfxFormat = &waveFile.waveFormat;
	//設定終わり

	//デバイス取得
	DirectSound * pDirectSound = DirectSound::GetInstance();

	//サウンドバッファ
	//メンバ変数の方に8がついていたが、こちらはついていない
	IDirectSoundBuffer* pBuffer = NULL;

	//バッファ作成
	if (FAILED(pDirectSound->CreateSoundBuffer(&desc, &pBuffer)))
	{
		return false;
		//渡されたwaveデータに問題がある
		//初期化されていないか
		//ロード失敗
		//の可能性
	}

	//サウンドバッファからセカンダリバッファ作成
	//IDirectSoundBufferからIDirectSoundBuffer8に変換する
	//第一引数で変換後の型を指定して、第二引数に変換したデータを入れる
	//変換後の型を指定する性質上、第二引数の型が不定
	//なのでvoid**に変換して強引にデータ書き込みする
	pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pSecondaryBuffer);
	pBuffer->Release();//変換したら変換元は用なし

	unsigned char *block1 = NULL;
	unsigned char *block2 = NULL;
	unsigned long blocksize1 = 0;
	unsigned long blocksize2 = 0;

	//セカンダリバッファをロックしてデータを書き込み
	pSecondaryBuffer->Lock
		(0, waveFile.dataSize,
		(void**)&block1, &blocksize1,
		(void**)&block2, &blocksize2,
		DSBLOCK_ENTIREBUFFER);

	//セカンダリバッファに音にデータをコピー
	memcpy(block1, waveFile.pWaveData, waveFile.dataSize);

	//セカンダリバッファのロック解除
	pSecondaryBuffer->Unlock(block1, blocksize1, block2, 0);

	return true;

}

//再生開始
void SoundBuffer::Play(bool loop)
{
	if (pSecondaryBuffer != NULL)
	{
		DWORD LoopFlag(loop ? DSBPLAY_LOOPING : 0);
		pSecondaryBuffer->Play(0, 0, LoopFlag);
		
	}
}

//再生停止
void SoundBuffer::Stop()
{
	if (pSecondaryBuffer != NULL)
	{
		pSecondaryBuffer->Stop();
	}
}

bool SoundBuffer::Playing()
{
	DWORD d;
	LPDWORD lpd = &d;
	pSecondaryBuffer->GetStatus(lpd);

	if (DSBSTATUS_PLAYING&d != 0/* || DSBSTATUS_LOOPING&d != 0*/)
	{
		return true;
	}
	else
	{
		return false;
	}
}