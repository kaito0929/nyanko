#include "Singleton.h"
#include <iostream>
#include <windows.h>
#include <string.h>

//�R�����g�A�E�g�̊֐��̓C�����C���ɕς��܂���

//template<class T>
//void Singleton<T>::Create()
//{
//	if (pInstance == nullptr)
//	{
//		//�C���X�^���X�̍쐬
//		pInstance = new T;
//
//		//�t�@�C�i���C�U�[�N���X��
//		//����֐���o�^����
//		//�v���O�����I�����O�Ɏ����I��
//		//�J�����Ă��炦��悤�ɂ���
//		SingletonFinalizer::AddFinalizer(Release);
//
//		created = true;
//	}
//}
//
//template<class T>
//void Singleton<T>::Release()
//{
//	if (pInstance != nullptr)
//	{
//		//�C���X�^���X�̊J��
//		delete pInstance;
//		pInstance = nullptr;
//		created = false;
//	}
//}
//
////�C���X�^���X�̎擾
//template <class T>
//T& Singleton<T>::GetInstance()
//{
//	//���߂�GetInstance�����s����Ƃ���
//	//�C���X�^���X���쐬���܂�
//	if (!created)
//	{
//		Create();
//	}
//
//	//�Q�ƂŕԂ��̂�*�ŃC���X�^���X�̎��̂��w��
//	return *pInstance;
//}
//
////�R���X�g���N�^
//template<class T>
//Singleton<T>::Singleton()
//{
//
//}

//SingletonFinalizer

//�Y�ꂪ�������X�^�e�B�b�N�ȃ����o�ϐ��̓N���X���Ő錾���������ł�
//���̂��Ȃ��̂Ŗ������̊O���V���{���ƃG���[���o�܂�
//�X�^�e�B�b�N�ȕϐ��͕K�����̂����܂��傤
vector<SingletonFinalizer::FinalizerFunc> SingletonFinalizer::finalizers;

void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	for (int i = 0; i<finalizers.size(); i++)
	{
		if (finalizers[i] == func)
		{
			//���d�o�^�͊ԈႢ�Ȃ̂Ŗ���
			return;
		}
	}

	//���d�o�^�ł͂Ȃ��̂Œǉ�
	finalizers.push_back(func);
}

//�v���O�����̏I�����O�Ɏ��s
void SingletonFinalizer::Finalize()
{
	//�Z�߂ĉ�����鏈��

	for (int i = 0; i<finalizers.size(); i++)
	{
		//�֐��|�C���^�̎��s
		finalizers[i]();
	}

	//��������I������̂ō폜
	finalizers.clear();
}