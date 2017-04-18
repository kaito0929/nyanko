//Singleton�̃e���v���[�g�N���X
//�N���X�C���X�^���X����ȏ�����Ȃ����Ƃ�ۏ؂���N���X
//�R���X�g���N�^��R�s�[�R���X�g���N�^�A������Z��private�ɉB�����ƂŎ���
//�C���X�^���X�̍쐬�Ǝ擾�͐�p�̐ÓI�֐�����s��

#pragma once

using namespace std;
#include <vector>
#include <iostream>
#include <string>
#include <mutex>

#include <assert.h>

//�V���O���g���̃C���X�^���X���쐬����邽�߂�
//����֐��̊֐��|�C���^��o�^����
//�v���O�����̏I�����ɂ܂Ƃ߂ĊJ���֐����Ăяo���N���X
class SingletonFinalizer
{
public:

	//�֐��|�C���^�̌^��`
	typedef void(*FinalizerFunc)();

private:
	//����֐��̃|�C���^��ۑ�����x�N�g��
	static vector<FinalizerFunc> finalizers;

public:

	//�x�N�^�ɉ���֐���o�^
	static void AddFinalizer(FinalizerFunc func);
	//�Z�߂ĉ���֐����Ă�
	static void Finalize();
};

//�N���XT���V���O���g���ɂȂ�N���X
//�h��������ۂɂ͎��̂悤�Ȑ錾�ōs��
//class Test :  public Singleton<Test>

template <class T>
class Singleton
{
private:
	//�v���C�x�[�g�ȃ����o�ϐ�

	static T* pInstance;		//���̃N���X�B��̃C���X�^���X�ւ̃|�C���^
							//Create�֐��œ��I�Ɋ��蓖�Ă��s��

	static bool created;	//�쐬�ς݂��ǂ���

protected:
	//�v���e�N�e�b�h�ȃ����o�ϐ�

public:
	//�p�u���b�N�ȃ����o�ϐ�


private:
	//�X�^�e�B�b�N�ȃ����o�֐��̓C�����C���ɂ��Ȃ���
	//�����N�G���[���o����ۂ�(���m�F)

	//�v���C�x�[�g�ȃ����o�֐�
	static std::once_flag initFlag;

	//�N���X�O���������ĂׂȂ�����
	Singleton& operator=(const Singleton& s){}//������Z
	//�R�s�[�R���X�g���N�^��������l
	Singleton(const Singleton& s){}

	//�쐬�̊֐��@�X�^�e�B�b�N
	static void Create()
	{
		if (pInstance == nullptr)
		{
			//�C���X�^���X�̍쐬
			pInstance = new T;

			//�t�@�C�i���C�U�[�N���X��
			//����֐���o�^����
			//�v���O�����I�����O�Ɏ����I��
			//�J�����Ă��炦��悤�ɂ���
			SingletonFinalizer::AddFinalizer(Release);

			created = true;
		}
	};

	//����̊֐�
	static void Release()
	{
		if (pInstance != nullptr)
		{
			//�C���X�^���X�̊J��
			delete pInstance;
			pInstance = nullptr;
			created = false;
		}
	};

protected:
	//�v���e�N�e�b�h�ȃ����o�֐�

	//�R���X�g���N�^�������Z�q�̓N���X�O����
	//�A�N�Z�X�ł��Ă͂����Ȃ�
	//�ÓI�����o�֐������ new �����邵���C���X�^���X�������@�𖳂���

	Singleton(){}
	virtual ~Singleton()
	{
		string s;
		s = typeid(*this).name();

		cout << "Singleton�J�� �N���X�� �F " << s << endl;
	}
	static bool IsInstatnceCreated(){ return (pInstance != nullptr); }
public:
	//�p�u���b�N�ȃ����o�֐�

	//�C���X�^���X�擾�֐�
	static T& GetInstance()
	{
		//���߂�GetInstance�����s����Ƃ���
		//�C���X�^���X���쐬���܂�
		if (!created)
		{
			Create();
		}

		return *pInstance;
	};
	
};

//�X�^�e�B�b�N�ȃ����o�ϐ���������
template<class T> T*   Singleton<T>::pInstance = nullptr;
template<class T> bool Singleton<T>::created  = false;
