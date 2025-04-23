#include "Player.h"

using namespace KamataEngine;

void Player::Initilize(Model* model, uint32_t textureHandle, Camera* camera) {
	//NULLポインタチェック
	assert(model);

	//モデル
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数の内容をメンバ変数に記録
	camera_ = camera;
}

void Player::Update() {
//行列を定義バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_, textureHandle_); }
