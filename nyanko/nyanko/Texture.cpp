#include "Texture.h"

//コンストラクタ	デストラクタ
Texture::Texture()
{
	pTexture = NULL;

	//分割を縦横それぞれ1で分割
	//(分割していない状態)
	divU = divV = 1;

	//表示する画像は分割の0番目
	//全く分割していないので0以外はに設定できない
	numU = numV = 0;
}

//デストラクタ
Texture::~Texture()
{
	Release();
}

//解放処理
void Texture::Release()
{
	if (pTexture != NULL)
	{
		pTexture->Release();

		divU = divV = 1;

		numU = numV = 0;
	}
}

//分割数の設定
void Texture::SetDivide(unsigned int DivU, unsigned int DivV)
{
	if (DivU==0&&DivV==0)
	{
		//分割数は必ず1以上
		//全く分割しない場合が1なので
		return;
	}

	divU = DivU;
	divV = DivV;
}

//分割した何番目を使用するか決める関数
void Texture::SetUVNum(unsigned int NumU, unsigned int NumV)
{
	//指定した分割数以上の値は設定できない
	if (NumU >= divU)return;
	if (NumV >= divV)return;

	numU = NumU;
	numV = NumV;
}

//画像のロード
bool Texture::Load(TCHAR*FilePath)//戻り値は成功でtrue
{
	Direct3D *pD3D = Direct3D::GetInstance();

	//Direct3Dクラスに画像ファイルの読み込み
	//読み込みに成功すると
	//pTextureの指す先にデータが保存される
	return pD3D->LoadTexture(*this, FilePath);
}
