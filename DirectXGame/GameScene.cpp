#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("playerk.png");
	// スプライトインスタンスの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
}

void GameScene::Update() {}

void GameScene::Draw() {
	//directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();


	//スプライト描画後処理
	Sprite::PostDraw();
}

GameScene::~GameScene() { delete sprite_;}




