#include "BoundingBox.h"

#define FORWARD 0
#define RIGHT	1
#define UP		2

//F forward <--->B back
//R right   <--->L left
//U up      <--->D down

#define VERTEX_FRU 0
#define VERTEX_FLU 1
#define VERTEX_BRU 2
#define VERTEX_BLU 3
#define VERTEX_FRD 4
#define VERTEX_FLD 5
#define VERTEX_BRD 6
#define VERTEX_BLD 7

OrientedBoundingBox::OrientedBoundingBox()
{
	position.x = position.y = position.z = 0;

	for (int i = 0; i < 3; i++)
	{
		//�����ƕ�������S��0�ŏ�����
		length[i] = 0;
		NormDirection[i].x = 0;
		NormDirection[i].y = 0;
		NormDirection[i].z = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		vertexPos[i].x = 0;
		vertexPos[i].y = 0;
		vertexPos[i].z = 0;
	}
}

OrientedBoundingBox::~OrientedBoundingBox()
{

}

void OrientedBoundingBox::UpdateInfo(D3DXVECTOR3 newPos, D3DXVECTOR3 newFor, D3DXVECTOR3 newRight, D3DXVECTOR3 newUp)
{
	//�W���� ������1��
	//�P�ʃx�N�g�����Č����Ă��̂͑����ԈႢ
	//�W�����x�N�g����������
	D3DXVec3Normalize(&NormDirection[FORWARD],&newFor);
	D3DXVec3Normalize(&NormDirection[RIGHT], &newRight);
	D3DXVec3Normalize(&NormDirection[UP], &newUp);

	position = newPos;

	//���_�̍��W���X�V���܂�
	vertexPos[VERTEX_FRU] //�O	�E	��̓_
		= position + 
		NormDirection[FORWARD] * length[FORWARD] * 0.5 +
		NormDirection[RIGHT] * length[RIGHT] * 0.5 +
		NormDirection[UP] * length[UP] * 0.5;

	vertexPos[VERTEX_FLU]
		= position + 
		(NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	vertexPos[VERTEX_BRU]
		= position + 
		(NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	vertexPos[VERTEX_BLU] 
		= position + 
		(NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	//������

	vertexPos[VERTEX_FRD] 
		= position + 
		(NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);

	vertexPos[VERTEX_FLD]
		= position + 
		(NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);

	vertexPos[VERTEX_BRD] 
		= position + 
		(NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);

	vertexPos[VERTEX_BLD]
		= position + 
		(NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);
}

void OrientedBoundingBox::SetLength(float forward, float right, float up)
{
	length[FORWARD] = forward;
	length[RIGHT] = right;
	length[UP] = up;
}

void OrientedBoundingBox::DrawLine()
{
	//�ӂ�12�{�Ȃ̂Œ��_�̐���24
	LINE_VERTEX lineVertex[24];
	for (int i = 0; i < 24; i++)
	{
		lineVertex[i].pos.x = 0;
		lineVertex[i].pos.y = 0;
		lineVertex[i].pos.z = 0;
		lineVertex[i].color = 0xFF000000;
	}

	lineVertex[0].pos = vertexPos[VERTEX_FLU];
	lineVertex[1].pos = vertexPos[VERTEX_FRU];//�㐳�ʑ��̉���

	lineVertex[2].pos = vertexPos[VERTEX_FRU];
	lineVertex[3].pos = vertexPos[VERTEX_BRU];

	lineVertex[4].pos = vertexPos[VERTEX_BRU];
	lineVertex[5].pos = vertexPos[VERTEX_BLU];

	lineVertex[6].pos = vertexPos[VERTEX_BLU];
	lineVertex[7].pos = vertexPos[VERTEX_FLU];

	//���ʂ̕�

	lineVertex[8].pos = vertexPos[VERTEX_FLD];
	lineVertex[9].pos = vertexPos[VERTEX_FRD];

	lineVertex[10].pos = vertexPos[VERTEX_FRD];
	lineVertex[11].pos = vertexPos[VERTEX_BRD];

	lineVertex[12].pos = vertexPos[VERTEX_BRD];
	lineVertex[13].pos = vertexPos[VERTEX_BLD];

	lineVertex[14].pos = vertexPos[VERTEX_BLD];
	lineVertex[15].pos = vertexPos[VERTEX_FLD];

	//��

	lineVertex[16].pos = vertexPos[VERTEX_FLU];
	lineVertex[17].pos = vertexPos[VERTEX_FLD];

	lineVertex[18].pos = vertexPos[VERTEX_FRU];
	lineVertex[19].pos=  vertexPos[VERTEX_FRD];

	lineVertex[20].pos = vertexPos[VERTEX_BLU];
	lineVertex[21].pos = vertexPos[VERTEX_BLD];

	lineVertex[22].pos = vertexPos[VERTEX_BRU];
	lineVertex[23].pos = vertexPos[VERTEX_BRD];

	//���̕`��
	Direct3D::GetInstance()->DrawLine(lineVertex, 12);
}

bool OrientedBoundingBox::Collision(OrientedBoundingBox& obb1, OrientedBoundingBox& obb2)
{
	//�����x�N�g��(�W����)��
	//�����������Ċe�����x�N�g���̒��������߂�
	D3DXVECTOR3 AE[3];//box1
	D3DXVECTOR3 BE[3];//box2

	for (int i = 0; i < 3; i++)
	{
		AE[i] = obb1.NormDirection[i] * (obb1.length[i] / 2);
		BE[i] = obb2.NormDirection[i] * (obb2.length[i] / 2);
	}

	D3DXVECTOR3 Interval = obb1.position - obb2.position;

	FLOAT rA, rB, L;
	
	//�������𕪗����ɂ����ۂ̕����̃`�F�b�N
	//obb1�̕�����

	//�������@obb.1NormDirection[FORWARD]
	rA = obb1.length[FORWARD] / 2;
	//: obb1�̐��ʃx�N�g���ɕ��s�Ȏ���obb2�𓊉e�����ۂ̒��������߂�
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[FORWARD], &BE[0], &BE[1], &BE[2]);
	//�C���^�[�o���𓊉e�����ۂ̒��������߂�
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[FORWARD], &Interval));
	if (L > rA + rB)return false;//�������m�F�ł����@�Փ˂��Ă��Ȃ�

	//������ obb1.NormDirection[1] : obb1�̉E�����̃x�N�g���ɕ��s�Ȏ���obb2�𓊉e
	rA = obb1.length[1] / 2;
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[1], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[1], &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ obb1.NormDirection[2] : obb1�̏�����̃x�N�g���ɕ��s�Ȏ���obb2�𓊉e
	rA = obb1.length[2] / 2;
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[2], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[2], &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�


	//�������𕪗����� obb2

	//������ obb1.NormDirection[0] : obb2�̐��ʕ����̃x�N�g���ɕ��s�Ȏ���obb2�𓊉e
	rA = LengthSegmentOnSeparateAxis(&obb2.NormDirection[0], &AE[0], &AE[1], &AE[2]);
	rB = obb2.length[0] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[0], &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ obb1.NormDirection[0] : obb2�̉E�����̃x�N�g���ɕ��s�Ȏ���obb2�𓊉e
	rA = LengthSegmentOnSeparateAxis(&obb2.NormDirection[1], &AE[0], &AE[1], &AE[2]);
	rB = obb2.length[1] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[1], &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ obb1.NormDirection[0] : obb2�̏�����̃x�N�g���ɕ��s�Ȏ���obb2�𓊉e
	rA = LengthSegmentOnSeparateAxis(&obb2.NormDirection[1], &AE[0], &AE[1], &AE[2]);
	rB = obb2.length[2] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[2], &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�


	//�O�ς𕪗�����

	//�������̗��̂ɂ���
	//C00 : cross obb1.normdirection[0] obb2.normdirection[0]
	//C10 : cross obb1.normdirection[1] obb2.normdirection[0]

	D3DXVECTOR3 Cross;

	//������ c00
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[0]);

	//�O�ώ���obb1��obb2�𓊉e�����ۂ̒����𒲂ׂ�
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//cross�Ɏg�p����0�Ԃ͎g��Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//cross�Ɏg�p����0�Ԃ͎g��Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;

	//������ c01
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//cross�Ɏg�p�����ԍ� 0�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ c01
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//cross�Ɏg�p�����ԍ� 0�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//--

	//������ c10
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[0]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//cross�Ɏg�p�����ԍ� 0�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ c11
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ c12
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//--

	//������ c20
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[0]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//cross�Ɏg�p�����ԍ� 0�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ c21
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//cross�Ɏg�p�����ԍ� 1�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�

	//������ c22
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//cross�Ɏg�p�����ԍ� 2�͎g�p���Ȃ�
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//�Փ˂��Ă��Ȃ�


	//�ǂ̎��ł��������m�F�ł��Ȃ�����
	//�������ʂ����݂��Ȃ�

	return true;//�Փ˂��Ă���

}

float OrientedBoundingBox::LengthSegmentOnSeparateAxis
(D3DXVECTOR3* pSep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	//�O�̓��ς̐�Ύ��̍��v�œ��e�������������v�Z

	FLOAT r1 = fabs(D3DXVec3Dot(pSep, e1));
	FLOAT r2 = fabs(D3DXVec3Dot(pSep, e2));
	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(pSep, e3))) : 0;

	return r1 + r2 + r3;
}