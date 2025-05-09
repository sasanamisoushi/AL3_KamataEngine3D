#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;
	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	camera_ = camera;
}

void Skydome::Update() {
	// 行列を定義バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	//3Dモデルの描画
	model_->Draw(worldTransform_, *camera_);
	Model::PostDraw();
}
