#include "mesh.h"

//�R���X�g���N�^
Mesh::Mesh()
{
	pMesh = NULL;
}

//�f�X�g���N�^
Mesh::~Mesh()
{
	if (pMesh != NULL)
	{
		pMesh->Release();
		pMesh = NULL;
	}
}

void Mesh::Draw(D3DXMATRIXA16& mat_transform,	//3D���W�@�s��
				D3DXMATRIXA16& mat_scale,		//�g�嗦�@�s��
				D3DXMATRIXA16& mat_rotate)		//��]	  �s��
{
	//�����̎O�̍s����|�����킹��
	//3D���W�@�g�嗦�@��]��S�Ċ܂ލs������

	//�ŏI�I�ȍs�������ϐ�
	D3DXMATRIXA16 matWorld;

	//�v�Z�p�̈ꎞ�I�ȍs��
	D3DXMATRIXA16 matTmp;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTmp);

	//�s��̊|���Z
	//*���@�s��̊|���Z�͑O��t�ɂ���ƌ��ʂ��ς��̂�
	//�@�@ ���Ԃ� �g��*��]*���W�̏��Ԃ����

	//matTmp�Ɂ@�g��*��]�@������
	D3DXMatrixMultiply(&matTmp, &mat_scale, &mat_rotate);
	//matWorld�� tmp*���W������
	D3DXMatrixMultiply(&matWorld, &matTmp, &mat_transform);

	//Direct3D�Ƀ��b�V���̏��ƍs���n���ĕ\�����Ă��炤
	Direct3D::GetInstance()->DrawMesh(*this,matWorld);

}

void Mesh::Load(TCHAR* path)
{
	Direct3D::GetInstance()->LoadMesh(*this, path);
}