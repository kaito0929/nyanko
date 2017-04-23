#include "SceneManager.h"


SceneManager::SceneManager() : mNextScene(STATE_NONE)//次のシーン管理変数
{
	mScene = (BaseScene*) new Title(this);
}

//初期化
void SceneManager::Initialize()
{
	mScene->Initialize();
}

//終了処理
void SceneManager::Finalize()
{
	mScene->Finalize();
}

//更新
void SceneManager::Update()
{
	if (mNextScene != STATE_NONE)//次のシーンがセットされていたら
	{
		mScene->Finalize();//現在のシーンの終了処理を実行
		delete mScene;
		switch (mNextScene)//シーンによって処理を分岐
		{
		case STATE_TITLE:
			mScene = (BaseScene*)new Title(this);	//タイトル画面のインスタンスを生成する
			break;
		case STATE_MAIN:
			mScene = (BaseScene*)new Floor1Update(this);
			break;
		case STATE_RESULT:
			mScene = (BaseScene*)new Result(this);
			break;
		}
		mNextScene = STATE_NONE;	//次のシーン情報をクリア
		mScene->Initialize();		//シーンを初期化
	}
	mScene->Update();			//シーンの更新
}

//描画
void SceneManager::Draw()
{
	mScene->Draw();//シーンの描画
}

//引数 NexySceneにシーンを変更する
void SceneManager::ChangeScene(STATE NextScene)
{
	mNextScene = NextScene;		//次のシーンをセットする
}