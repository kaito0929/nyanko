#define _CRT_SECURE_NO_WARNINGS
//fopen�ł̌x�����Ȃ���	//���̌x��������ƃR���p�C���ł��Ȃ��Ȃ�

#include "Wave.h"
#include <string>
using namespace std;

//�R���X�g���N�^
WaveFile::WaveFile()
{
	//waveFormat�̐擪�A�h���X����
	//WAVEFORMATEX�̃f�[�^�ʕ�����0�ɂ���
	//�܂�waveFormat
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	pWaveData = NULL;
	dataSize = 0;
}

//�f�X�g���N�^
WaveFile::~WaveFile()
{
	Release();
}

//�������
void WaveFile::Release()
{
	//�����o�ϐ������Z�b�g����
	if (pWaveData != NULL)//���[�h�ς݂�������
	{
		delete[] pWaveData;//new����Ƃ��ɔz��Ŋm�ۂ��邽��[]��t����

		//�ȉ��R���X�g���N�^�ƈꏏ
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

		pWaveData = NULL;
		dataSize = 0;
	}
}

bool WaveFile::Load(TCHAR* FilePath)
{
	//.Wav�`���̃t�@�C���͎O�̃`�����N�ɕ�����Ă���
	//�`�����N:�f�[�^����؂�����
	
	//RIFF�w�b�_�[�`�����N	//�t�@�C���̎�ނ�\���w�b�_�[
	//�t�H�[�}�b�g�`�����N	//�����f�[�^�̋L�^�̂����
	//�f�[�^�`�����N		//����(�g�`)�f�[�^

	FILE* fp;

	//�t�@�C���̃I�[�v��
	if (!(fp = _tfopen(FilePath, _T("rb"))))
	{
		//�t�@�C���I�[�v���Ɏ��s������
		return false;
	}

	//�t�@�C���̃I�[�v���ɐ���

	char chunkID[5]{};
	char type[5]{};
	unsigned int chunkSize = 0;
	
	//RIFF�w�b�_�[��ǂ�

	//RIFF�w�b�_�[�\��-----------------------------------
	//riff 4byte "RIFF"��4����
	//size 4byte ���t�@�C���T�C�Y-8(byte�P��)
	//type 4byte "WAVE"��4����(�����f�[�^�̏ꍇ "WAVE")
	//---------------------------------------------------

	//riff = "RIFF" type=="WAVE"�̏ꍇ�̂݁A���̌�̓ǂݍ��݂��s��

	//fread�֐� chunkID�̃A�h���X�� fp ����ǂ݂������f�[�^���R�s�[����
	//�f�[�^��char4�����ꌏ
	fread(chunkID, sizeof(char) * 4, 1, fp);//RIFF��4����

	//fp�͓ǂݏo�����s������
	//���̓ǂݏo���J�n�ʒu���ǂ݂��������������ւ����
	fread(&chunkSize, sizeof(unsigned int), 1, fp);//�t�@�C���ʂ�ǂ�
	fread(type, sizeof(char) * 4, 1, fp);//type��ǂ݂���

	//�ǂݍ��񂾕�������r���₷���悤�ɃX�g�����O�Ɋi�[
	string chunkID_st = chunkID;
	string type_st = type;

	//������WAVE�t�@�C�����ǂ����𒲂ׂ�
	if (!(chunkID_st == "RIFF"&&type_st == "WAVE"))
	{
		//��������O��Ă���̂Ŏ��s
		return false;
	}

	//���ꂼ��̃`�����N�̓ǂݏo����������
	bool fmtChunk = false;
	bool dataChunk = false;

	while (true)
	{
		//�ǂ̃`�����N��chunkID�@chunk�̗e�ʂ���f�[�^���X�^�[�g����
		fread(chunkID, sizeof(char) * 4, 1, fp);//chunkID
		fread(&chunkSize, sizeof(unsigned int), 1, fp);//�T�C�Y
		//�T�C�Y�̓`�����N�S�̂���-8(id��size�̗e��)���ꂽ����

		chunkID_st = chunkID;

		if (chunkID_st == "fmt ")//fmt�̌�ɃX�y�[�X
		{
			//�t�H�[�}�b�g�`�����N----------------------------------------
			//id			4byte	�`�����N���ʎq
			//size			4byte	id��size�������`�����N�T�C�Y
			//format		2byte	�g�`�f�[�^�̃t�H�[�}�b�g
			//channels		2byte	�`�����l����	(���m����or�X�e���I)
			//samplerate	4byte	�T���v�����O���g��
			//bytepersec	4byte	���σf�[�^����
			//blockalign	2byte	�f�[�^�̃u���b�N�T�C�Y
			//bitswidth		2byte	1�T���v���ӂ�̃r�b�g��
			//extended_size	2byte	�g���f�[�^�̃T�C�Y
			//extended		Nbyte	�g���f�[�^
			//------------------------------------------------------------

			if (chunkSize >= sizeof(WAVEFORMATEX))
			{
				//�`�����N�T�C�Y��WAVEFORMATEX���傫���ꍇ
				//�����炭�g���f�[�^������ꍇ

				//WAVEFORMATEX���ǂݍ���
				fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);
				
				//�`�����N�̎c��̈�̑傫��������
				int diff = chunkSize - sizeof(WAVEFORMATEX);
				//diff : difference ��

				//���̃`�����N�̐擪�܂ŃV�[�N
				fseek(fp, diff, SEEK_CUR);
			}
			else
			{
				//�`�����N�T�C�Y��WAVEFORMATEX�̑傫���ɖ����Ȃ��ꍇ
				//�`�����N�̏I���܂œǂ�
				//��肠�������镪�ǂ�

				fread(&waveFormat, chunkSize, 1, fp);
			}

			//�t�H�[�}�b�g�͌Ă�
			fmtChunk = true;


		}
		else if (chunkID_st=="data")
		{
			//�f�[�^�`�����N---------------------------------------------------------------
			//id				4byte	�`�����N���ʎq
			//size				4byte	id��size�������`�����N�T�C�Y(�g�`�f�[�^�̃f�[�^��)
			//waveformatData	Nbyte	�g�`�f�[�^
			//-----------------------------------------------------------------------------

			//�f�[�^�T�C�Y�m��
			dataSize = chunkSize;
			pWaveData = new byte[dataSize];

			//�f�[�^�ǂݍ���	//fread�̖߂�l�͐������ǂ߂�����	dataSize�����ǂ�
			unsigned long readCount = fread(pWaveData, sizeof(byte), dataSize, fp);
			if (!(readCount == dataSize))
			{
				//�S�ēǂ߂Ȃ�����
				fclose(fp);//�t�@�C�������

				Release();//�f�[�^�����Z�b�g����

				return false;
			}

			//�S�ēǂ߂�
			//�f�[�^�`�����N�͓ǂݍ��ݍς�
			dataChunk = true;


		}
		else
		{
			//fmt�ł�data�ł��Ȃ��`�����N�͓ǂݔ�΂�
			fseek(fp, chunkSize, SEEK_CUR);
		}

		if (fmtChunk&&dataChunk)//�t�H�[�}�b�g���f�[�^���ǂݍ��ݍς�
		{
			//while���甲����
			break;
		}
	}

	fclose(fp);
	return true;

}