#pragma once
#include "Direct3D.h"

class OrientedBoundingBox
{
	//OBB

protected:
	D3DXVECTOR3 position;//�{�b�N�X�̒��S
	//����(�P�ʃx�N�g��)	
	//����	�E	��
	D3DXVECTOR3 NormDirection[3];

	//�e���̒���
	//����	�E	��
	FLOAT length[3];

	//������	�p	���W
	D3DXVECTOR3 vertexPos[8];

public:

	OrientedBoundingBox();
	virtual ~OrientedBoundingBox();

	void UpdateInfo(D3DXVECTOR3 newPos, 
					D3DXVECTOR3 newFor, 
					D3DXVECTOR3 newRight, 
					D3DXVECTOR3 newUp);

	void SetLength(float forward, float right, float up);

	void DrawLine();

private:
	//�������ɒ����̂𓊉e�������̒���
	//pSep�@������(�P�ʃx�N�g��)
	//e1,e2,e3 
	//����{�b�N�X�̕����x�N�g��(����������)1,2,3
	static float LengthSegmentOnSeparateAxis
		(D3DXVECTOR3* pSep,
		 D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = NULL);

public:

	//obb1��obb2���ڐG���Ă����true��Ԃ�
	static bool Collision(OrientedBoundingBox& obb1, OrientedBoundingBox& obb2);
};