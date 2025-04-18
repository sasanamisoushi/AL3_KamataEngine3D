#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	
	//エンジンの初期化
	KamataEngine::Initialize();
	//ゲームシーンのインスタンスの生成
	GameScene* gameScene = new GameScene();
	//ゲームシーンの初期化
	gameScene->Initialize();
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
		
	//メインループ
	while (true) {
		//エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		
		//ImGui受付開始
		imguiManager->Begin();

		//ゲームシーンの更新
		gameScene->Update();

		//ImGui受付終了
		imguiManager->End();

		//描画の開始
		dxCommon->PreDraw();

		gameScene->Draw();

		//軸表示の描画
		AxisIndicator::GetInstance()->Draw();


		//ImGui描画
		imguiManager->Draw();

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
