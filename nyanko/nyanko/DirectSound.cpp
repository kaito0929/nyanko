#include "DirectSound.h"

DirectSound* DirectSound::pInstance = NULL;

//�R���X�g���N�^
DirectSound::DirectSound()
{
	pDirectSound8 = NULL;
}

//�f�X�g���N�^
DirectSound::~DirectSound()
{
	Release();
}

//���
void DirectSound:: Release()
{
	if (pDirectSound8 != NULL)
	{
		pDirectSound8->Release();
		pDirectSound8 = NULL;
	}
}

//�T�E���h�f�o�C�X�̍쐬�Ƌ������x���̐ݒ�
bool DirectSound::Create(HWND hWnd)
{
	//�f�o�C�X�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
	{
		//���s
		Release();
		return false;
	}

	//�������x���̐ݒ�
	//�܂�����炷�E�B���h�E�̐ݒ�
	if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		//���s
		Release();
		return false;
	}
	return true;
}

HRESULT DirectSound::CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer)
{
	if (pDirectSound8 == NULL)
	{
		//�f�o�C�X���쐬����Ă��Ȃ��̂�
		//�o�b�t�@�̍쐬���o���Ȃ�
		return S_FALSE;
	}

	//�T�E���h�o�b�t�@�̍쐬
	//ppBuffer�̎w����̃|�C���^�Ɂ@�T�E���h�o�b�t�@���L�^����Ă���A�h���X����������
	return pDirectSound8->CreateSoundBuffer(pDesc, ppBuffer, NULL);
}