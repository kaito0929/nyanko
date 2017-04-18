#include "SoundBuffer.h"
#include "Wave.h"
#include "DirectSound.h"

//�R���X�g���N�^
SoundBuffer::SoundBuffer()
{
	pSecondaryBuffer = NULL;
}

//�f�X�g���N�^
SoundBuffer::~SoundBuffer()
{
	Release();
}

//�������
void SoundBuffer::Release()
{
	//�Z�J���_���o�b�t�@�Ƀf�[�^������Ȃ�
	if (pSecondaryBuffer != NULL)
	{
		//����̑O�ɍĐ���~����
		Stop();

		//���
		pSecondaryBuffer->Release();
		//����ς݂̃|�C���^��NULL����
		pSecondaryBuffer = NULL;
	}
}

//�o�b�t�@�쐬
//�ǂݍ��ݍς݂�waveFile�N���X�̎Q�Ƃ�����
bool SoundBuffer::Create(WaveFile& waveFile)
{
	//�Z�J���_���o�b�t�@�쐬�p�ݒ�
	//desc : description
	DSBUFFERDESC desc = {};

	//�`�����l���̐��ŕ���
	//���m�����@1�`�����l��
	//�X�e���I�@2�`�����l��

	if (waveFile.waveFormat.nChannels==1)
	{
		//���m����
		desc.dwFlags = 
			DSBCAPS_CTRL3D |
			DSBCAPS_CTRLVOLUME |
			DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_CTRLPOSITIONNOTIFY |
			DSBCAPS_STATIC;

		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else /*if (waveFile.waveFormat.nChannels == 2)*///3�`�����l���ȏ�̃X�e���I������悤�Ȃ̂łȂ�
	{
		//�X�e���I
		desc.dwFlags =
			//DSBCAPS_CTRL3D |//���m�������甲��
			DSBCAPS_CTRLVOLUME |
			DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_CTRLPOSITIONNOTIFY |
			DSBCAPS_STATIC |
			DSBCAPS_CTRLPAN;//�X�e���I�͂��̍Ō�̍s������������

		desc.guid3DAlgorithm = GUID_NULL;
	}
	
	desc.dwSize = sizeof(DSBUFFERDESC);
	//�g�`�f�[�^�T�C�Y
	desc.dwBufferBytes = waveFile.dataSize;
	//wave�t�@�C���t�H�[�}�b�g�w��
	desc.lpwfxFormat = &waveFile.waveFormat;
	//�ݒ�I���

	//�f�o�C�X�擾
	DirectSound * pDirectSound = DirectSound::GetInstance();

	//�T�E���h�o�b�t�@
	//�����o�ϐ��̕���8�����Ă������A������͂��Ă��Ȃ�
	IDirectSoundBuffer* pBuffer = NULL;

	//�o�b�t�@�쐬
	if (FAILED(pDirectSound->CreateSoundBuffer(&desc, &pBuffer)))
	{
		return false;
		//�n���ꂽwave�f�[�^�ɖ�肪����
		//����������Ă��Ȃ���
		//���[�h���s
		//�̉\��
	}

	//�T�E���h�o�b�t�@����Z�J���_���o�b�t�@�쐬
	//IDirectSoundBuffer����IDirectSoundBuffer8�ɕϊ�����
	//�������ŕϊ���̌^���w�肵�āA�������ɕϊ������f�[�^������
	//�ϊ���̌^���w�肷�鐫����A�������̌^���s��
	//�Ȃ̂�void**�ɕϊ����ċ����Ƀf�[�^�������݂���
	pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pSecondaryBuffer);
	pBuffer->Release();//�ϊ�������ϊ����͗p�Ȃ�

	unsigned char *block1 = NULL;
	unsigned char *block2 = NULL;
	unsigned long blocksize1 = 0;
	unsigned long blocksize2 = 0;

	//�Z�J���_���o�b�t�@�����b�N���ăf�[�^����������
	pSecondaryBuffer->Lock
		(0, waveFile.dataSize,
		(void**)&block1, &blocksize1,
		(void**)&block2, &blocksize2,
		DSBLOCK_ENTIREBUFFER);

	//�Z�J���_���o�b�t�@�ɉ��Ƀf�[�^���R�s�[
	memcpy(block1, waveFile.pWaveData, waveFile.dataSize);

	//�Z�J���_���o�b�t�@�̃��b�N����
	pSecondaryBuffer->Unlock(block1, blocksize1, block2, 0);

	return true;

}

//�Đ��J�n
void SoundBuffer::Play(bool loop)
{
	if (pSecondaryBuffer != NULL)
	{
		DWORD LoopFlag(loop ? DSBPLAY_LOOPING : 0);
		pSecondaryBuffer->Play(0, 0, LoopFlag);
		
	}
}

//�Đ���~
void SoundBuffer::Stop()
{
	if (pSecondaryBuffer != NULL)
	{
		pSecondaryBuffer->Stop();
	}
}

bool SoundBuffer::Playing()
{
	DWORD d;
	LPDWORD lpd = &d;
	pSecondaryBuffer->GetStatus(lpd);

	if (DSBSTATUS_PLAYING&d != 0/* || DSBSTATUS_LOOPING&d != 0*/)
	{
		return true;
	}
	else
	{
		return false;
	}
}