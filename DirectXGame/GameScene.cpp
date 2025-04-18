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
}

void GameScene::Update() {}

void GameScene::Draw() {}

GameScene::~GameScene() {
	//3Dモデルデータの解放
	delete model_;
}