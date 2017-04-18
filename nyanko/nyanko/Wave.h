#pragma once

#pragma comment(lib,"dsound.lib")//ダイレクトサウンド
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>//ダイレクトサウンド
#include <tchar.h>
#include <comdef.h>

//一つのwaveファイルのデータを保存する

class WaveFile
{
	//サウンドバッファークラスから自由にメンバにアクセスできるようにする
	friend class SoundBuffer;

private:
	WAVEFORMATEX waveFormat;//どういう記録のされ方をしているか

	byte* pWaveData;		//波形データ(音声データ) //一番重要 //newするからdelete必須
	int dataSize;		//波形データのデータ量 byte単位
public:
	//コンストラクタ
	WaveFile();

	//デストラクタ
	~WaveFile();

	//解放処理
	void Release();

	//ファイルのロード
	bool Load(TCHAR* FilePath);
};