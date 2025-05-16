#include "Player.h"
#include <cassert>
#include <numbers>

using namespace KamataEngine;

void Player::Initilize(Model* model, Camera* camera, const Vector3& position) {
	//NULLポインタチェック
	assert(model);
	//モデル
	model_ = model;


	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_=position;
	//引数の内容をメンバ変数に記録
	camera_ = camera;

	//初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	//行列を定義バッファに転送
	worldTransform_.TransferMatrix();

	//移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT)|| 
		Input::GetInstance()->PushKey(DIK_LEFT)) {
	
		//左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		
			acceleration.x += kAcceleration;
		}
	}

	//移動
	worldTransform_.translation_ += velocity_;

	//行列更新

}

void Player::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, *camera_); 
	Model::PostDraw();
}
