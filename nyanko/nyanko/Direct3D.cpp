#include "Direct3D.h"

#include "Texture.h"
#include "Sprite.h"
#include "mesh.h"
#include "Singleton.h"

#include <d3d9.h>//direct3d
#include <d3dx9.h>//direct3d

Direct3D* Direct3D::pInstance = NULL;
RENDER_STATE Direct3D::currentState;

//�R���X�g���N�^
Direct3D::Direct3D()
{
	//Direct3D�f�o�C�X�����p�I�u�W�F�N�g
	pD3D9 = NULL;

	//Direct3D�f�o�C�X
	//�e�N�X�`���̃��[�h��|���S���̕\���ɕK�v
	pDevice3D = NULL;

	//�f�o�C�X���쐬�ς݂��ǂ���
	isDeviceCreated =false;
}

Direct3D::~Direct3D()
{
	//�f�o�C�X�̊J������
	Release();
}

bool Direct3D::Create(HWND hWnd)
{
	//�o�b�N�o�b�t�@�̑傫�������肷��
	//�傫���̓N���C�A���g�̈�ɍ��킹��
	RECT rect;
	GetClientRect(hWnd, &rect);			//�N���C�A���g�̈�̏��擾
	int Width = rect.right - rect.left;		//��
	int Height = rect.bottom - rect.top;	//����

	//Direct3D9�I�u�W�F�N�g�̍쐬
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	//�f�B�X�v���C���擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//�X���b�v�`�F�C���̐ݒ�
	//�X���b�v�`�F�C���A�o�b�N�o�b�t�@���t�����g�o�b�t�@�ɏ���������
	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth	= Width;	//�o�b�N�o�b�t�@��
	D3DParam.BackBufferHeight	= Height;	//�o�b�N�o�b�t�@����
	D3DParam.BackBufferFormat	= Display.Format;	//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	D3DParam.BackBufferCount = 1;					//�o�b�N�o�b�t�@��
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;	//�}���`�T���v�����O�̐ݒ�@�Ȃ�
	D3DParam.MultiSampleQuality = 0;				//�}���`�T���v�����O�̃N�I���e�B
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�X���b�v�`�F�C���̕��@�@Direct3D�C��
	D3DParam.hDeviceWindow = hWnd;					//�E�B���h�E�̃n���h��
	D3DParam.Windowed = TRUE;						//�E�B���h�E���[�h
	D3DParam.EnableAutoDepthStencil = TRUE;			//�[�x�X�e���V���o�b�t�@
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8; //�[�x�X�e���V���̃t�H�[�}�b�g
	D3DParam.Flags = 0;								//0�Ȃ̂ŉ���������
	D3DParam.FullScreen_RefreshRateInHz = 0;		//�X�N���[���̃��t���b�V�����[�g�@
													//�E�B���h�E���[�h���ƕK��0
	
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//�A�_�v�^���t���b�V�����[�g��present���������s���郌�[�g�̊֌W

	//�������̐ݒ�Ńf�o�C�X�쐬�����݂�
	//HAL���[�h�ō쐬
	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)))
	{
		//��ڂ̐ݒ�Ŏ��s������
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//��ڂ̐ݒ肩��HARDWARE->MIXED
			&D3DParam, &pDevice3D)))
		{
			//��ڂ̐ݒ�Ŏ��s������
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,//��ڂ̐ݒ肩�� HAL->REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)))
			{
				//�O�߂̐ݒ�Ŏ��s������
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//�O�ڂ̐ݒ肩��HARDWARE->SOFTWARE
					&D3DParam, &pDevice3D)))
				{
					//�S���Ɏ��s //�J������
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;
					return false;
				}
			}
		}	
	}
	return true;//�ǂꂩ�ɂ͐������Ă���
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (!(isDeviceCreated))//�܂�����Ă��Ȃ��Ȃ���
	{
		isDeviceCreated = Create(hWnd);//�f�o�C�X�̍쐬�֐�
	}

	return isDeviceCreated;//�쐻���o���Ă���Ȃ�true���Ԃ�
}

void Direct3D::Release()
{
	if (isDeviceCreated)
	{
		//�f�o�C�X���쐬�ς݂Ȃ�J������
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;

		//�f�o�C�X�̍쐬�t���O��off�ɂ���
		isDeviceCreated = false;
	}
}

HRESULT Direct3D::BeginScene()//�`��J�n�̍��}
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
HRESULT Direct3D::ClearScreen()//�o�b�N�o�b�t�@�̃N���A
{
	if (isDeviceCreated)
	{
		DWORD ClearColor = 0xFFFFFFFF;//�w�i�N���A�F //�ԁ@0xAARRGGBB

		//�w�i�N���A
		return pDevice3D->Clear(0, NULL, 
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //�N���A�^�[�Q�b�g�̃o�b�t�@
			ClearColor, 1.0f, 0);
		
	}
	else
	{
		return S_FALSE;
	}
}
HRESULT Direct3D::EndScene()//�`��I���̍��}
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

HRESULT Direct3D::Present()//�o�b�N�o�b�t�@���t�����g�ɔ��f
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
	//�f�o�C�X������Ă��邩�ǂ������m�F
	if (isDeviceCreated)
	{
		//�t�@�C���ǂݍ���	�t�@�C������e�N�X�`���̃f�[�^���쐬����
		//Texture�N���X�� pTexture�ɃA�N�Z�X�ł���̂�
		//Texture����Direct3D�N���X���t�����h�Ɏw�肵�Ă��邩��
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice3D, FilePath, &tex.pTexture)))
		{
			//pTexture�̎w����ɓǂݍ��񂾉摜�f�[�^���ۑ������`�ɂȂ�
			//���[�h����
			return true;
		}
		
	}
	
	//���[�h���s
	return false;

}

//static
void Direct3D::DrawSprite(Sprite&sprite, Texture& texture, bool isTurn)
{
	//�C���X�^���X�̎擾
	Direct3D *pD3D = Direct3D::GetInstance();

	//�f�o�C�X�������ƕ`��ł��Ȃ��̂ŋA��
	if (pD3D->isDeviceCreated == false){ return; }

	//TRIANGLESTRIP(�g���C�A���O���X�g���b�v)�Œ��_��`�悷��
	SpriteVertex vertex[4]//�l�p�`�Ȃ̂Œ��_�l��
	//�g���C�A���O���X�g���b�v���g�p����̂�
	//vertex�Ɋi�[���鏇�Ԃ͉E��@�E���@����@�����@�̏��Ԃɂ���
	{
		//
		//�E��
		{ (float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
			(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//�E��
		{ (float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
		//����
		{ -(float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//����
		{ -(float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f, 0x00ffffff,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
	};

	for (int i = 0; i < 4; i++)
	{
		//�O�p���̉��@�藝���g�p���Ē��_����]
		//4�̒��_�̒��S�����_�ɂȂ��Ă���̂ł��̕��@�ŉ�]�\
		float x = vertex[i].x*cosf(sprite.rotate) - vertex[i].y*sinf(sprite.rotate);
		float y = vertex[i].x*sinf(sprite.rotate) + vertex[i].y*cosf(sprite.rotate);

		//���s�ړ�
		vertex[i].x = x + sprite.pos.x;
		vertex[i].y = y + sprite.pos.y;

		vertex[i].ColorDefuse += static_cast<int>(((float)0x000000ff)*sprite.alpha) << (8 * 3);
	}
	

	//�e�N�X�`���̃Z�b�g
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

	//���_�\���̐錾���Z�b�g
	pD3D->pDevice3D->SetFVF(Sprite::SPRITE_FVF);

	//�|���S���̕`��@������
	//�g���C�A���O���X�g���b�v�ŕ`��
	//2�̓v���~�e�B�u�̐� �O�p�`�̐�
	//���_���̐擪�A�h���X
	//��̒��_�\���̗̂e��
	pD3D->pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(SpriteVertex));
	
}



//���b�V���̃��[�h
void Direct3D::LoadMesh(Mesh& mesh, TCHAR* path)
{
	if (mesh.pMesh == NULL)
	{
		//X�t�@�C���̃p�X���擾
		CHAR dir[_MAX_DIR];
		_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);
		//dir��path����t�@�C���������폜�������̂�����

		//���ۂɎg�p����f�[�^�ɃR�s�[����܂�
		//�ꎞ�I�Ƀ}�e���A������ۑ�����o�b�t�@�[
		LPD3DXBUFFER pBufferMaterial;

		//���b�V���̃��[�h
		if (D3DXLoadMeshFromX
			(path, D3DXMESH_SYSTEMMEM, pDevice3D,
			NULL, &pBufferMaterial,
			NULL, &mesh.numMaterials,
			&mesh.pMesh) != D3D_OK)
		{
			//���[�h�Ɏ��s
			return;
		}

		//�}�e���A���̏���
		if (mesh.numMaterials > 0)
		{
			//�}�e���A���̌����������̈�̊m��
			mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
			mesh.ppTextures = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

			//�o�b�t�@�̐擪�A�h���X���擾
			//�|�C���^�^�ϊ�
			D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();

			for (int i = 0; i < mesh.numMaterials; i++)
			{
				//���ꂼ��̃}�e���A�����o�b�t�@����R�s�[
				mesh.pMaterials[i] = d3dxMaterials[i].MatD3D;
				mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

				mesh.ppTextures[i] = NULL;

				//�}�e���A���Ƀe�N�X�`�����ݒ肳��Ă�����
				//�e�N�X�`���̃t�@�C���������o���ă��[�h
				if (d3dxMaterials[i].pTextureFilename != NULL)
				{
					//�e�N�X�`���̃t�@�C���p�X���쐬
					CHAR texturepath[1024];
					ZeroMemory(texturepath, sizeof(texturepath));

					//������A��
					//�� + dir�̓��e =  dir�̓��e
					lstrcat(texturepath, dir);
					//texturepath�̓��e(dir�̓��e) + �t�@�C���l�[�� = dir �t�@�C���l�[�� (�t�@�C���p�X)
					lstrcat(texturepath, d3dxMaterials[i].pTextureFilename);

					if (D3DXCreateTextureFromFile
						(pDevice3D, texturepath, &mesh.ppTextures[i])
						!= D3D_OK)
					{
						//�e�N�X�`���̓ǂݍ��݂Ɏ��s

						//�t�@�C���p�X�̍쐬�Ɏ��s�܂���
						//�t�@�C���p�X�̏ꏊ�Ƀt�@�C�����Ȃ�
						//�t�@�C���̔j���̂ǂꂩ
						mesh.ppTextures[i] = NULL;
					}

				}
			}

		}
		//�}�e���A����ǂݍ��񂾃o�b�t�@�̊J��
		pBufferMaterial->Release();
	}
}

//���b�V���̕\��  //��Œǉ��Ń}�e���A���������ɂ��邩��
void Direct3D::DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat)
{
	if (mesh.pMesh != NULL)
	{
		
		//SetTransform�Ƃ����֐������
		//direct3D�f�o�C�X�ɍs��̏���n��
		//�������͍s��̎��
		//D3DTS_WORLD�̏ꍇ�̓��b�V���̕\������g�����X�t�H�[��
		pDevice3D->SetTransform(D3DTS_WORLD, &worldMat);

		//���_�V�F�[�_
		pDevice3D->SetVertexShader(NULL);
		
		//���_�t�H�[�}�b�g
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());
		

		if (mesh.numMaterials > 0)
		{
			for (unsigned int i = 0; i < mesh.numMaterials; i++)
			{
				//�}�e���A���ƃe�N�X�`����
				//�f�o�C�X�ɓn��
				pDevice3D->SetMaterial(&mesh.pMaterials[i]);
				pDevice3D->SetTexture(0, mesh.ppTextures[i]);

				//i�Ԃ̃}�e���A�����K������Ă���|���S����\������
				mesh.pMesh->DrawSubset(i);

				//�}�e���A�����̌������[�v����̂�
				//�|���S���͑S�ĕ\�������
			}
		}
		else
		{
			//�}�e���A�������������ꍇ(�܂����肦�Ȃ���)
			//�K���ɍ�����}�e���A����\��

			D3DMATERIAL9 mtrl;
			ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			pDevice3D->SetMaterial(&mtrl);

			//�\���֐�
			mesh.pMesh->DrawSubset(0);
		}
	}
}

void Direct3D::SetViewMatrix(D3DXMATRIXA16 &mat)
{
	//�r���[�s��(�J�������)���f�o�C�X�ɓn��
	pDevice3D->SetTransform(D3DTS_VIEW, &mat);
}

void Direct3D::SetupProjectionMatrix()
{
	//�ˉe�s��
	//�����炭�J�����̎���p�̐ݒ�
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH	//LH:����n���W <-> RH:�E��n���W
		(&matProj,
		3.0f / 4.0f,  //y�������ւ̎���@���W�A��
		1.0f,		  //�A�X�y�N�g��
		1.0f,		  //�߂��r���[���ʂ�z�l
		100.0f);	  //�����r���[���ʂ�z�l


	//�v���W�F�N�V�����̍s����f�o�C�X�ɐݒ�
	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);

}

//�`�惂�[�h�̐ݒ�
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
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//���u�����h�̖�����
			}
			break;

			case RENDER_ALPHATEST:
			{
				//���e�X�g�ɂ�铧���̈�̐؂蔲��
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//���e�X�g�̗L����
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					// �A���t�@�Q�ƒl
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//���e�X�g���i�

				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			//���u�����h�̖�����
			}
			break;

			case RENDER_ALPHABLEND:
			{
				//���u�����h�̐ݒ�
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			//���e�X�g�̗L����
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			break;

			case RENDER_HALFADD:
			{
				//�����Z
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//���e�X�g�̖�����
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//���u�����f�B���O�̗L����
				d3d->pDevice3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@���s��Ȃ�

				//pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			//z�e�X�g���s��Ȃ�
				//pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//���C�e�B���O���s��Ȃ�

				d3d->pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		//�u�����f�B���O�I�v�V�������Z
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//SRC�̐ݒ�
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//DEST�̐ݒ�

			}
			break;

			case RENDER_ADD:
			{
				//�S���Z����
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//���u�����f�B���O�̗L����
				d3d->pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//�u�����f�B���O�I�v�V�������Z
				d3d->pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);	//SRC�̐ݒ�
				d3d->pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//DEST�̐ݒ�
				d3d->pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//���e�X�g�̖�����

			}
			break;
			case RENDER_MESH:
			{
				//�J�����O
				pInstance->pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				//���C�e�B���O
				pInstance->pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);

				//Z�o�b�t�@
				pInstance->pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);

				//�A���r�G���g
				pInstance->pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);


				//�{�������ɏ����ׂ��łȂ���
				//���C�f�B���O�����ł�

				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1.0f;//�F
				light.Diffuse.g = 1.0f;
				light.Diffuse.b = 1.0f;
				light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);//����
				light.Range = 1000.0f;//����

				//0���C�g�̔ԍ�
				pInstance->pDevice3D->SetLight(0, &light);
				pInstance->pDevice3D->LightEnable(0, TRUE);//0�Ԃ̃��C�g��L����
			}
				break;
			
			}
		}
	}
	else
	{
		MessageBox(NULL, "�C���X�^���X���쐬����Ă��Ȃ��̂Ŏ��s�ł��܂���", TEXT("Direct3D Render"), MB_OK);
	}
}

void Direct3D::DrawLine(LINE_VERTEX* pVertex, int count)
{
	//���ݐݒ肳��Ă���s������Z�b�g
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

