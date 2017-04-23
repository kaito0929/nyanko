#pragma once

#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneManager : public ISceneChanger, Task
{
private:
	BaseScene* mScene;		//シーンの管理変数
	STATE mNextScene;		//次のシーン管理変数

public:
	SceneManager();
	void Initialize() override;	//初期化
	void Finalize() override;	//終了処理
	void Update() override;		//更新
	void Draw() override;		//描画

	//引数　NextSceneにシーンを変更する
	void ChangeScene(STATE NextScene) override;
};