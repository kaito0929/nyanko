#include "Direct3D.h"

#include "Texture.h"
#include "Sprite.h"
#include "mesh.h"
#include "Singleton.h"

#include <d3d9.h>//direct3d
#include <d3dx9.h>//direct3d

Direct3D* Direct3D::pInstance = NULL;
RENDER_STATE Direct3D::currentState;

//コンストラクタ
Direct3D::Direct3D()
{
	//Direct3Dデバイス生成用オブジェクト
	pD3D9 = NULL;

	//Direct3Dデバイス
	//テクスチャのロードやポリゴンの表示に必要
	pDevice3D = NULL;

	//デバイスが作成済みかどうか
	isDeviceCreated =false;
}

Direct3D::~Direct3D()
{
	//デバイスの開放処理
	Release();
}

bool Direct3D::Create(HWND hWnd)
{
	//バックバッファの大きさを決定する
	//大きさはクライアント領域に合わせる
	RECT rect;
	GetClientRect(hWnd, &rect);			//クライアント領域の情報取得
	int Width = rect.right - rect.left;		//幅
	int Height = rect.bottom - rect.top;	//高さ

	//Direct3D9オブジェクトの作成
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	//ディスプレイ情報取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//スワップチェインの設定
	//スワップチェイン、バックバッファをフロントバッファに書き換える
	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth	= Width;	//バックバッファ幅
	D3DParam.BackBufferHeight	= Height;	//バックバッファ高さ
	D3DParam.BackBufferFormat	= Display.Format;	//バックバッファのフォーマット
	D3DParam.BackBufferCount = 1;					//バックバッファ数
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;	//マルチサンプリングの設定　なし
	D3DParam.MultiSampleQuality = 0;				//マルチサンプリングのクオリティ
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;	//スワップチェインの方法　Direct3D任せ
	D3DParam.hDeviceWindow = hWnd;					//ウィンドウのハンドル
	D3DParam.Windowed = TRUE;						//ウィンドウモード
	D3DParam.EnableAutoDepthStencil = TRUE;			//深度ステンシルバッファ
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8; //深度ステンシルのフォーマット
	D3DParam.Flags = 0;								//0なので何も無い筈
	D3DParam.FullScreen_RefreshRateInHz = 0;		//スクリーンのリフレッシュレート　
													//ウィンドウモードだと必ず0
	
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//アダプタリフレッシュレートとpresent処理を実行するレートの関係

	//いくつかの設定でデバイス作成を試みる
	//HALモードで作成
	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)))
	{
		//一つ目の設定で失敗したら
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//一つ目の設定からHARDWARE->MIXED
			&D3DParam, &pDevice3D)))
		{
			//二つ目の設定で失敗したら
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,//一つ目の設定から HAL->REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)))
			{
				//三つめの設定で失敗したら
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//三つ目の設定からHARDWARE->SOFTWARE
					&D3DParam, &pDevice3D)))
				{
					//全部に失敗 //開放処理
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;
					return false;
				}
			}
		}	
	}
	return true;//どれかには成功している
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (!(isDeviceCreated))//まだ作っていないなら作る
	{
		isDeviceCreated = Create(hWnd);//デバイスの作成関数
	}

	return isDeviceCreated;//作製が出来ているならtrueが返る
}

void Direct3D::Release()
{
	if (isDeviceCreated)
	{
		//デバイスが作成済みなら開放する
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;

		//デバイスの作成フラグをoffにする
		isDeviceCreated = false;
	}
}

HRESULT Direct3D::BeginScene()//描画開始の合図
{
	if (isDeviceCreated)
	{
		return pDevice3D->BeginScene();
	}
	else
	{
		return S_FALSE;
	}
}
HRESULT Direct3D::ClearScreen()//バックバッファのクリア
{
	if (isDeviceCreated)
	{
		DWORD ClearColor = 0xFFFFFFFF;//背景クリア色 //赤　0xAARRGGBB

		//背景クリア
		return pDevice3D->Clear(0, NULL, 
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //クリアターゲットのバッファ
			ClearColor, 1.0f, 0);
		
	}
	else
	{
		return S_FALSE;
	}
}
HRESULT Direct3D::EndScene()//描画終了の合図
{
	if (isDeviceCreated)
	{
		return pDevice3D->EndScene();

	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::Present()//バックバッファをフロントに反映
{	
	if (isDeviceCreated)
	{
		return pDevice3D->Present(NULL, NULL, NULL, NULL);
	}
	else
	{
		return S_FALSE;
	}	
}


bool Direct3D::LoadTexture(Texture& tex, TCHAR*FilePath)
{
	//デバイスが作られているかどうかを確認
	if (isDeviceCreated)
	{
		//ファイル読み込み	ファイルからテクスチャのデータを作成する
		//Textureクラスの pTextureにアクセスできるのは
		//Texture側でDirect3Dクラスをフレンドに指定しているから
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice3D, FilePath, &tex.pTexture)))
		{
			//pTextureの指す先に読み込んだ画像データが保存される形になる
			//ロード成功
			return true;
		}
		
	}
	
	//ロード失敗
	return false;

}

//static
void Direct3D::DrawSprite(Sprite&sprite, Texture& texture, bool isTurn)
{
	//インスタンスの取得
	Direct3D *pD3D = Direct3D::GetInstance();

	//デバイスが無いと描画できないので帰る
	if (pD3D->isDeviceCreated == false){ return; }

	//TRIANGLESTRIP(トライアングルストリップ)で頂点を描画する
	SpriteVertex vertex[4]//四角形なので頂点四つ
	//トライアングルストリップを使用するので
	//vertexに格納する順番は右上　右下　左上　左下　の順番にする
	{
		//
		//右上
		{ (float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
			(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//右下
		{ (float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
		//左上
		{ -(float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//左下
		{ -(float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
	};

	for (int i = 0; i < 4; i++)
	{
		//三角日の加法定理を使用して頂点を回転
		//4つの頂点の中心が原点になっているのでこの方法で回転可能
		float x = vertex[i].x*cosf(sprite.rotate) - vertex[i].y*sinf(sprite.rotate);
		float y = vertex[i].x*sinf(sprite.rotate) + vertex[i].y*cosf(sprite.rotate);

		//平行移動
		vertex[i].x = x + sprite.pos.x;
		vertex[i].y = y + sprite.pos.y;

		vertex[i].ColorDefuse += static_cast<int>(((float)0x000000ff)*sprite.alpha) << (8 * 3);
	}
	

	//テクスチャのセット
	pD3D->pDevice3D->SetTexture(0, texture.pTexture);

	if (currentState == RENDER_STATE::RENDER_ALPHABLEND)
	{
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pD3D->pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}

	//頂点構造体宣言をセット
	pD3D->pDevice3D->SetFVF(Sprite::SPRITE_FVF);

	//ポリゴンの描画　引数は
	//トライアングルストリップで描画
	//2はプリミティブの数 三角形の数
	//頂点情報の先頭アドレス
	//一つの頂点構造体の容量
	pD3D->pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(SpriteVertex));
	
}



//メッシュのロード
void Direct3D::LoadMesh(Mesh& mesh, TCHAR* path)
{
	if (mesh.pMesh == NULL)
	{
		//Xファイルのパスを取得
		CHAR dir[_MAX_DIR];
		_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);
		//dirにpathからファイル名だけ削除したものが入る

		//実際に使用するデータにコピーするまで
		//一時的にマテリアル情報を保存するバッファー
		LPD3DXBUFFER pBufferMaterial;

		//メッシュのロード
		if (D3DXLoadMeshFromX
			(path, D3DXMESH_SYSTEMMEM, pDevice3D,
			NULL, &pBufferMaterial,
			NULL, &mesh.numMaterials,
			&mesh.pMesh) != D3D_OK)
		{
			//ロードに失敗
			return;
		}

		//マテリアルの準備
		if (mesh.numMaterials > 0)
		{
			//マテリアルの個数分メモリ領域の確保
			mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
			mesh.ppTextures = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

			//バッファの先頭アドレスを取得
			//ポインタ型変換
			D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();

			for (int i = 0; i < mesh.numMaterials; i++)
			{
				//それぞれのマテリアルをバッファからコピー
				mesh.pMaterials[i] = d3dxMaterials[i].MatD3D;
				mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

				mesh.ppTextures[i] = NULL;

				//マテリアルにテクスチャが設定されていたら
				//テクスチャのファイル名を取り出してロード
				if (d3dxMaterials[i].pTextureFilename != NULL)
				{
					//テクスチャのファイルパスを作成
					CHAR texturepath[1024];
					ZeroMemory(texturepath, sizeof(texturepath));

					//文字列連結
					//空 + dirの内容 =  dirの内容
					lstrcat(texturepath, dir);
					//texturepathの内容(dirの内容) + ファイルネーム = dir ファイルネーム (ファイルパス)
					lstrcat(texturepath, d3dxMaterials[i].pTextureFilename);

					if (D3DXCreateTextureFromFile
						(pDevice3D, texturepath, &mesh.ppTextures[i])
						!= D3D_OK)
					{
						//テクスチャの読み込みに失敗

						//ファイルパスの作成に失敗または
						//ファイルパスの場所にファイルがない
						//ファイルの破損のどれか
						mesh.ppTextures[i] = NULL;
					}

				}
			}

		}
		//マテリアルを読み込んだバッファの開放
		pBufferMaterial->Release();
	}
}

//メッシュの表示  //後で追加でマテリアルを引数にするかも
void Direct3D::DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat)
{
	if (mesh.pMesh != NULL)
	{
		
		//SetTransformという関数を介して
		//direct3Dデバイスに行列の情報を渡す
		//第一引数は行列の種類
		//D3DTS_WORLDの場合はメッシュの表示するトランスフォーム
		pDevice3D->SetTransform(D3DTS_WORLD, &worldMat);

		//頂点シェーダ
		pDevice3D->SetVertexShader(NULL);
		
		//頂点フォーマット
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());
		

		if (mesh.numMaterials > 0)
		{
			for (unsigned int i = 0; i < mesh.numMaterials; i++)
			{
				//マテリアルとテクスチャを
				//デバイスに渡す
				pDevice3D->SetMaterial(&mesh.pMaterials[i]);
				pDevice3D->SetTexture(0, mesh.ppTextures[i]);

				//i番のマテリアルが適応されているポリゴンを表示する
				mesh.pMesh->DrawSubset(i);

				//マテリアル分の個数分ループするので
				//ポリゴンは全て表示される
			}
		}
		else
		{
			//マテリアルが無かった場合(まずありえないが)
			//適当に作ったマテリアルを表示

			D3DMATERIAL9 mtrl;
			ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			pDevice3D->SetMaterial(&mtrl);

			//表示関数
			mesh.pMesh->DrawSubset(0);
		}
	}
}

void Direct3D::SetViewMatrix(D3DXMATRIXA16 &mat)
{
	//ビュー行列(カメラ情報)をデバイスに渡す
	pDevice3D->SetTransform(D3DTS_VIEW, &mat);
}

void Direct3D::SetupProjectionMatrix()
{
	//射影行列
	//おそらくカメラの視野角の設定
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH	//LH:左手系座標 <-> RH:右手系座標
		(&matProj,
		3.0f / 4.0f,  //y軸方向への視野　ラジアン
		1.0f,		  //アスペクト比
		1.0f,		  //近いビュー平面のz値
		100.0f);	  //遠いビュー平面のz値


	//プロジェクションの行列をデバイスに設定
	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);

}

//描画モードの設定
void Direct3D::SetRenderState(RENDER_STATE renderState)
{
	Direct3D *d3d = Direct3D::GetInstance();
	if (d3d)
	{
		if (d3d->IsDeviceCreated())
		{
			currentState = renderState;

			switch (renderState)
			{
			case RENDER_DEFAULT:
			{
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//αブレンドの無効化
			}
			break;

			case RENDER_ALPHATEST:
			{
				//αテストによる透明領域の切り抜き
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//αテストの有効化
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					// アルファ参照値
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//αテスト合格基準

				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			//αブレンドの無効化
			}
			break;

			case RENDER_ALPHABLEND:
			{
				//αブレンドの設定
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//αテストの有効化
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			break;

			case RENDER_HALFADD:
			{
				//半加算
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//αテストの無効化
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//αブレンディングの有効化
				d3d->pDevice3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファを行わない

				//pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			//zテストを行わない
				//pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//ライティングを行わない

				d3d->pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		//ブレンディングオプション加算
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//SRCの設定
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//DESTの設定

			}
			break;

			case RENDER_ADD:
			{
				//全加算合成
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//αブレンディングの有効化
				d3d->pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//ブレンディングオプション加算
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);	//SRCの設定
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//DESTの設定
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//αテストの無効化

			}
			break;
			case RENDER_MESH:
			{
				//カリング
				pInstance->pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				//ライティング
				pInstance->pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);

				//Zバッファ
				pInstance->pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);

				//アンビエント
				pInstance->pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);


				//本来ここに書くべきでないが
				//ライディングをついでに

				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1.0f;//色
				light.Diffuse.g = 1.0f;
				light.Diffuse.b = 1.0f;
				light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);//向き
				light.Range = 1000.0f;//距離

				//0ライトの番号
				pInstance->pDevice3D->SetLight(0, &light);
				pInstance->pDevice3D->LightEnable(0, TRUE);//0番のライトを有効に
			}
				break;
			
			}
		}
	}
	else
	{
		MessageBox(NULL, "インスタンスが作成されていないので実行できません", TEXT("Direct3D Render"), MB_OK);
	}
}

void Direct3D::DrawLine(LINE_VERTEX* pVertex, int count)
{
	//現在設定されている行列をリセット
	D3DXMATRIXA16 mat_transform, 
				  mat_scale,
				  mat_rotate,
				  mat_world,
				  mat_tmp;

	D3DXMatrixTranslation(&mat_transform, 0, 0, 0);
	D3DXMatrixScaling(&mat_scale, 1, 1, 1);
	D3DXMatrixRotationY(&mat_rotate, 0);

	D3DXMatrixMultiply(&mat_tmp, &mat_scale, &mat_rotate);
	D3DXMatrixMultiply(&mat_world, &mat_tmp, &mat_transform);

	pDevice3D->SetTransform(D3DTS_WORLD, &mat_world);

	pDevice3D->SetFVF(D3DFVF_LINE_VERTEX);
	pDevice3D->DrawPrimitiveUP(D3DPT_LINELIST, count, pVertex, sizeof(LINE_VERTEX));


}

