#include "Player.h"
#include <cassert>

using namespace KamataEngine;

void Player::Initilize(Model* model, Camera* camera) {
	//NULLポインタチェック
	assert(model);
	//モデル
	model_ = model;


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
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, *camera_); 
	Model::PostDraw();
}
