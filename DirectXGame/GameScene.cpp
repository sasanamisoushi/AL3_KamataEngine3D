#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	//ファイル名を指定してテクスチャハンドルを読み込む
	textureHandle_ = TextureManager::Load("playerk.png");
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//カメラの初期化
	camera_.Initialize();

	//自キャラの更新
	player_ = new Player();
	//自キャラの初期化
	player_->Initilize();
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();
}

void GameScene::Draw() {
	// 自キャラの描画
	player_->Draw();
}

GameScene::~GameScene() {
	//3Dモデルデータの解放
	delete model_;

	//自キャラの解放
	delete player_;
}