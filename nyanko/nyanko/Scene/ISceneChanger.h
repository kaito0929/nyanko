#pragma once

typedef enum
{
	STATE_TITLE,		//タイトル画面
	STATE_MAIN,			//ゲーム画面
	STATE_RESULT,		//リザルト画面

	STATE_NONE			//無し
}STATE;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;
	virtual void ChangeScene(STATE NextScene) = 0;//指定シーンに変更する
};