#include "Texture.h"

//�R���X�g���N�^	�f�X�g���N�^
Texture::Texture()
{
	pTexture = NULL;

	//�������c�����ꂼ��1�ŕ���
	//(�������Ă��Ȃ����)
	divU = divV = 1;

	//�\������摜�͕�����0�Ԗ�
	//�S���������Ă��Ȃ��̂�0�ȊO�͂ɐݒ�ł��Ȃ�
	numU = numV = 0;
}

//�f�X�g���N�^
Texture::~Texture()
{
	Release();
}

//�������
void Texture::Release()
{
	if (pTexture != NULL)
	{
		pTexture->Release();

		divU = divV = 1;

		numU = numV = 0;
	}
}

//�������̐ݒ�
void Texture::SetDivide(unsigned int DivU, unsigned int DivV)
{
	if (DivU==0&&DivV==0)
	{
		//�������͕K��1�ȏ�
		//�S���������Ȃ��ꍇ��1�Ȃ̂�
		return;
	}

	divU = DivU;
	divV = DivV;
}

//�����������Ԗڂ��g�p���邩���߂�֐�
void Texture::SetUVNum(unsigned int NumU, unsigned int NumV)
{
	//�w�肵���������ȏ�̒l�͐ݒ�ł��Ȃ�
	if (NumU >= divU)return;
	if (NumV >= divV)return;

	numU = NumU;
	numV = NumV;
}

//�摜�̃��[�h
bool Texture::Load(TCHAR*FilePath)//�߂�l�͐�����true
{
	Direct3D *pD3D = Direct3D::GetInstance();

	//Direct3D�N���X�ɉ摜�t�@�C���̓ǂݍ���
	//�ǂݍ��݂ɐ��������
	//pTexture�̎w����Ƀf�[�^���ۑ������
	return pD3D->LoadTexture(*this, FilePath);
}
