#include "Singleton.h"
#include <iostream>
#include <windows.h>
#include <string.h>

//コメントアウトの関数はインラインに変えました

//template<class T>
//void Singleton<T>::Create()
//{
//	if (pInstance == nullptr)
//	{
//		//インスタンスの作成
//		pInstance = new T;
//
//		//ファイナライザークラスに
//		//解放関数を登録して
//		//プログラム終了直前に自動的に
//		//開放してもらえるようにする
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
//		//インスタンスの開放
//		delete pInstance;
//		pInstance = nullptr;
//		created = false;
//	}
//}
//
////インスタンスの取得
//template <class T>
//T& Singleton<T>::GetInstance()
//{
//	//初めてGetInstanceを実行するときに
//	//インスタンスを作成します
//	if (!created)
//	{
//		Create();
//	}
//
//	//参照で返すので*でインスタンスの実体を指定
//	return *pInstance;
//}
//
////コンストラクタ
//template<class T>
//Singleton<T>::Singleton()
//{
//
//}

//SingletonFinalizer

//忘れがちだがスタティックなメンバ変数はクラス内で宣言しただけでは
//実体がないので未解決の外部シンボルとエラーが出ます
//スタティックな変数は必ず実体を作りましょう
vector<SingletonFinalizer::FinalizerFunc> SingletonFinalizer::finalizers;

void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	for (int i = 0; i<finalizers.size(); i++)
	{
		if (finalizers[i] == func)
		{
			//多重登録は間違いなので無効
			return;
		}
	}

	//多重登録ではないので追加
	finalizers.push_back(func);
}

//プログラムの終了直前に実行
void SingletonFinalizer::Finalize()
{
	//纏めて解放する処理

	for (int i = 0; i<finalizers.size(); i++)
	{
		//関数ポインタの実行
		finalizers[i]();
	}

	//解放処理終わったので削除
	finalizers.clear();
}