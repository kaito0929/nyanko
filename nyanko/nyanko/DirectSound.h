#pragma once

#define DIRECTSOUND_VERSION 0x800

#pragma comment(lib,"dsound.lib")//�_�C���N�g�T�E���h
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>//�_�C���N�g�T�E���h
#include <tchar.h>
#include <comdef.h>

class DirectSound
{
private:
	//�V���O���g���ɂ���	�C���X�^���X�����蓖�Ă�|�C���^
	static DirectSound* pInstance;

	//�T�E���h�f�o�C�X
	IDirectSound8* pDirectSound8;

	void Release();

	//�R���X�g���N�^ //�f�X�g���N�^
	//�V���O���g���ɂ��邽�߂ɃN���X�O������s�ł��Ȃ��悤�ɂ���
	DirectSound();
	~DirectSound();

	void operator=(const DirectSound& obj){}	//������Z
	DirectSound(const DirectSound& obj){};		//�R�s�[�R���X�g���N�^

public:
	//�C���X�^���X�̎擾
	static DirectSound* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectSound();
		}
		return pInstance;
	}

	bool Create(HWND hWnd);//�f�o�C�X�쐬

	HRESULT CreateSoundBuffer(DSBUFFERDESC * pDesc, IDirectSoundBuffer** ppBuffer);


};