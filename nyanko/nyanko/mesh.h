#pragma once

#include "Direct3D.h"


class Mesh
{
	friend class Direct3D;

private:
	//���b�V���f�[�^������|�C���^
	LPD3DXMESH pMesh;
	//���[�h�������b�V���ɓK�p����Ă���}�e���A���̐�
	DWORD numMaterials;

	//���b�V���ɓK�p���ꂽ�}�e���A��//�z��Ń������m�ۂ��܂�
	D3DMATERIAL9 * pMaterials;
	//�}�e���A���ɑΉ�����e�N�X�`��//�z��Ń������m�ۂ��܂�
	LPDIRECT3DTEXTURE9* ppTextures;

public:
	Mesh();
	~Mesh();

	//�\���֐�
	void Draw(	D3DXMATRIXA16& mat_transform,	//3D���W�@�s��
				D3DXMATRIXA16& mat_scale,		//�g�嗦�@�s��
				D3DXMATRIXA16& mat_rotate);		//��]	  �s��

	//���[�h�֐�
	void Load(TCHAR* path);

};