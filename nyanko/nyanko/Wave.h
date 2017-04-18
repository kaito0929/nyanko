#pragma once

#pragma comment(lib,"dsound.lib")//�_�C���N�g�T�E���h
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>//�_�C���N�g�T�E���h
#include <tchar.h>
#include <comdef.h>

//���wave�t�@�C���̃f�[�^��ۑ�����

class WaveFile
{
	//�T�E���h�o�b�t�@�[�N���X���玩�R�Ƀ����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class SoundBuffer;

private:
	WAVEFORMATEX waveFormat;//�ǂ������L�^�̂���������Ă��邩

	byte* pWaveData;		//�g�`�f�[�^(�����f�[�^) //��ԏd�v //new���邩��delete�K�{
	int dataSize;		//�g�`�f�[�^�̃f�[�^�� byte�P��
public:
	//�R���X�g���N�^
	WaveFile();

	//�f�X�g���N�^
	~WaveFile();

	//�������
	void Release();

	//�t�@�C���̃��[�h
	bool Load(TCHAR* FilePath);
};