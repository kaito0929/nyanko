#pragma once

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>
#include <tchar.h>
#include <comdef.h>

//���O�����錾�@�����Ɏg��
class WaveFile;

class SoundBuffer
{
private:
	//�Z�J���_���o�b�t�@
	IDirectSoundBuffer8 *pSecondaryBuffer;

public:
	//�R���X�g���N�^�@�f�X�g���N�^
	SoundBuffer();
	~SoundBuffer();

	//�쐬
	bool Create(WaveFile& waveFile);

	//�Đ��ƒ�~
	void Play(bool Loop);
	void Stop();

	bool Playing();
	//���
	void Release();
};