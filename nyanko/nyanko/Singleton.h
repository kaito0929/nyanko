//Singletonのテンプレートクラス
//クラスインスタンスが二つ以上つくられないことを保証するクラス
//コンストラクタやコピーコンストラクタ、代入演算をprivateに隠すことで実現
//インスタンスの作成と取得は専用の静的関数から行う

#pragma once

using namespace std;
#include <vector>
#include <iostream>
#include <string>
#include <mutex>

#include <assert.h>

//シングルトンのインスタンスが作成されるために
//解放関数の関数ポインタを登録する
//プログラムの終了時にまとめて開放関数を呼び出すクラス
class SingletonFinalizer
{
public:

	//関数ポインタの型定義
	typedef void(*FinalizerFunc)();

private:
	//解放関数のポインタを保存するベクトル
	static vector<FinalizerFunc> finalizers;

public:

	//ベクタに解放関数を登録
	static void AddFinalizer(FinalizerFunc func);
	//纏めて解放関数を呼ぶ
	static void Finalize();
};

//クラスTがシングルトンになるクラス
//派生をする際には次のような宣言で行う
//class Test :  public Singleton<Test>

template <class T>
class Singleton
{
private:
	//プライベートなメンバ変数

	static T* pInstance;		//そのクラス唯一のインスタンスへのポインタ
							//Create関数で動的に割り当てを行う

	static bool created;	//作成済みかどうか

protected:
	//プロテクテッドなメンバ変数

public:
	//パブリックなメンバ変数


private:
	//スタティックなメンバ関数はインラインにしないと
	//リンクエラーが出るっぽい(未確認)

	//プライベートなメンバ関数
	static std::once_flag initFlag;

	//クラス外から代入を呼べなくする
	Singleton& operator=(const Singleton& s){}//代入演算
	//コピーコンストラクタも代入同様
	Singleton(const Singleton& s){}

	//作成の関数　スタティック
	static void Create()
	{
		if (pInstance == nullptr)
		{
			//インスタンスの作成
			pInstance = new T;

			//ファイナライザークラスに
			//解放関数を登録して
			//プログラム終了直前に自動的に
			//開放してもらえるようにする
			SingletonFinalizer::AddFinalizer(Release);

			created = true;
		}
	};

	//解放の関数
	static void Release()
	{
		if (pInstance != nullptr)
		{
			//インスタンスの開放
			delete pInstance;
			pInstance = nullptr;
			created = false;
		}
	};

protected:
	//プロテクテッドなメンバ関数

	//コンストラクタや代入演算子はクラス外から
	//アクセスできてはいけない
	//静的メンバ関数を介して new をするしかインスタンスを作る方法を無くす

	Singleton(){}
	virtual ~Singleton()
	{
		string s;
		s = typeid(*this).name();

		cout << "Singleton開放 クラス名 ： " << s << endl;
	}
	static bool IsInstatnceCreated(){ return (pInstance != nullptr); }
public:
	//パブリックなメンバ関数

	//インスタンス取得関数
	static T& GetInstance()
	{
		//初めてGetInstanceを実行するときに
		//インスタンスを作成します
		if (!created)
		{
			Create();
		}

		return *pInstance;
	};
	
};

//スタティックなメンバ変数を初期化
template<class T> T*   Singleton<T>::pInstance = nullptr;
template<class T> bool Singleton<T>::created  = false;
