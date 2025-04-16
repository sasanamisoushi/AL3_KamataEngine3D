#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	
	//エンジンの初期化
	KamataEngine::Initialize();
	//ゲームシーンの初期化
	GameScene* gameScene = new GameScene();
	//メインループ
	while (true) {
		//エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		//DirectXCommonインスタンスの取得
		DirectXCommon* dxCommon = DirectXCommon::GetInstance();

		//描画の開始
		dxCommon->PreDraw();

		gameScene->Draw();

		//描画終了
		dxCommon->PostDraw();
	}

	//ゲームシーンの解放
	delete gameScene;
	//nullptrの代入
	gameScene = nullptr;

	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}
