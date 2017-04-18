#define _CRT_SECURE_NO_WARNINGS
//fopenでの警告をなくす	//この警告があるとコンパイルできなくなる

#include "Wave.h"
#include <string>
using namespace std;

//コンストラクタ
WaveFile::WaveFile()
{
	//waveFormatの先頭アドレスから
	//WAVEFORMATEXのデータ量分だけ0にする
	//つまりwaveFormat
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	pWaveData = NULL;
	dataSize = 0;
}

//デストラクタ
WaveFile::~WaveFile()
{
	Release();
}

//解放処理
void WaveFile::Release()
{
	//メンバ変数をリセットする
	if (pWaveData != NULL)//ロード済みだったら
	{
		delete[] pWaveData;//newするときに配列で確保するため[]を付ける

		//以下コンストラクタと一緒
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

		pWaveData = NULL;
		dataSize = 0;
	}
}

bool WaveFile::Load(TCHAR* FilePath)
{
	//.Wav形式のファイルは三つのチャンクに分かれている
	//チャンク:データを区切った塊
	
	//RIFFヘッダーチャンク	//ファイルの種類を表すヘッダー
	//フォーマットチャンク	//音声データの記録のされ方
	//データチャンク		//音声(波形)データ

	FILE* fp;

	//ファイルのオープン
	if (!(fp = _tfopen(FilePath, _T("rb"))))
	{
		//ファイルオープンに失敗したら
		return false;
	}

	//ファイルのオープンに成功

	char chunkID[5]{};
	char type[5]{};
	unsigned int chunkSize = 0;
	
	//RIFFヘッダーを読む

	//RIFFヘッダー構成-----------------------------------
	//riff 4byte "RIFF"の4文字
	//size 4byte 総ファイルサイズ-8(byte単位)
	//type 4byte "WAVE"の4文字(音声データの場合 "WAVE")
	//---------------------------------------------------

	//riff = "RIFF" type=="WAVE"の場合のみ、その後の読み込みを行う

	//fread関数 chunkIDのアドレスに fp から読みだしたデータをコピーする
	//データ量char4個分を一件
	fread(chunkID, sizeof(char) * 4, 1, fp);//RIFFの4文字

	//fpは読み出しを行った後
	//次の読み出し開始位置が読みだした分だけ後ろへずれる
	fread(&chunkSize, sizeof(unsigned int), 1, fp);//ファイル量を読む
	fread(type, sizeof(char) * 4, 1, fp);//typeを読みだす

	//読み込んだ文字列を比較しやすいようにストリングに格納
	string chunkID_st = chunkID;
	string type_st = type;

	//正しいWAVEファイル化どうかを調べる
	if (!(chunkID_st == "RIFF"&&type_st == "WAVE"))
	{
		//条件から外れているので失敗
		return false;
	}

	//それぞれのチャンクの読み出しをしたか
	bool fmtChunk = false;
	bool dataChunk = false;

	while (true)
	{
		//どのチャンクもchunkID　chunkの容量からデータがスタートする
		fread(chunkID, sizeof(char) * 4, 1, fp);//chunkID
		fread(&chunkSize, sizeof(unsigned int), 1, fp);//サイズ
		//サイズはチャンク全体から-8(idとsizeの容量)されたもの

		chunkID_st = chunkID;

		if (chunkID_st == "fmt ")//fmtの後にスペース
		{
			//フォーマットチャンク----------------------------------------
			//id			4byte	チャンク識別子
			//size			4byte	idとsizeを除くチャンクサイズ
			//format		2byte	波形データのフォーマット
			//channels		2byte	チャンネル数	(モノラルorステレオ)
			//samplerate	4byte	サンプリング周波数
			//bytepersec	4byte	平均データ割合
			//blockalign	2byte	データのブロックサイズ
			//bitswidth		2byte	1サンプル辺りのビット数
			//extended_size	2byte	拡張データのサイズ
			//extended		Nbyte	拡張データ
			//------------------------------------------------------------

			if (chunkSize >= sizeof(WAVEFORMATEX))
			{
				//チャンクサイズがWAVEFORMATEXより大きい場合
				//おそらく拡張データがある場合

				//WAVEFORMATEX分読み込み
				fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);
				
				//チャンクの残り領域の大きさを検索
				int diff = chunkSize - sizeof(WAVEFORMATEX);
				//diff : difference 差

				//次のチャンクの先頭までシーク
				fseek(fp, diff, SEEK_CUR);
			}
			else
			{
				//チャンクサイズがWAVEFORMATEXの大きさに満たない場合
				//チャンクの終わりまで読む
				//取りあえずある分読む

				fread(&waveFormat, chunkSize, 1, fp);
			}

			//フォーマットは呼んだ
			fmtChunk = true;


		}
		else if (chunkID_st=="data")
		{
			//データチャンク---------------------------------------------------------------
			//id				4byte	チャンク識別子
			//size				4byte	idとsizeを除くチャンクサイズ(波形データのデータ量)
			//waveformatData	Nbyte	波形データ
			//-----------------------------------------------------------------------------

			//データサイズ確保
			dataSize = chunkSize;
			pWaveData = new byte[dataSize];

			//データ読み込み	//freadの戻り値は正しく読めた件数	dataSize件分読む
			unsigned long readCount = fread(pWaveData, sizeof(byte), dataSize, fp);
			if (!(readCount == dataSize))
			{
				//全て読めなかった
				fclose(fp);//ファイルを閉じる

				Release();//データをリセットする

				return false;
			}

			//全て読めた
			//データチャンクは読み込み済み
			dataChunk = true;


		}
		else
		{
			//fmtでもdataでもないチャンクは読み飛ばし
			fseek(fp, chunkSize, SEEK_CUR);
		}

		if (fmtChunk&&dataChunk)//フォーマットもデータも読み込み済み
		{
			//whileから抜ける
			break;
		}
	}

	fclose(fp);
	return true;

}