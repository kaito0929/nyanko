#include "mesh.h"

//コンストラクタ
Mesh::Mesh()
{
	pMesh = NULL;
}

//デストラクタ
Mesh::~Mesh()
{
	if (pMesh != NULL)
	{
		pMesh->Release();
		pMesh = NULL;
	}
}

void Mesh::Draw(D3DXMATRIXA16& mat_transform,	//3D座標　行列
				D3DXMATRIXA16& mat_scale,		//拡大率　行列
				D3DXMATRIXA16& mat_rotate)		//回転	  行列
{
	//引数の三つの行列を掛け合わせて
	//3D座標　拡大率　回転を全て含む行列を作る

	//最終的な行列を入れる変数
	D3DXMATRIXA16 matWorld;

	//計算用の一時的な行列
	D3DXMATRIXA16 matTmp;

	//単位行列に初期化
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTmp);

	//行列の掛け算
	//*注　行列の掛け算は前後逆にすると結果が変わるので
	//　　 順番は 拡大*回転*座標の順番を守る

	//matTmpに　拡大*回転　を入れる
	D3DXMatrixMultiply(&matTmp, &mat_scale, &mat_rotate);
	//matWorldに tmp*座標を入れる
	D3DXMatrixMultiply(&matWorld, &matTmp, &mat_transform);

	//Direct3Dにメッシュの情報と行列を渡して表示してもらう
	Direct3D::GetInstance()->DrawMesh(*this,matWorld);

}

void Mesh::Load(TCHAR* path)
{
	Direct3D::GetInstance()->LoadMesh(*this, path);
}