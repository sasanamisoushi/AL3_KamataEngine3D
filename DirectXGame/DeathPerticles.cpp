#include "DeathPerticles.h"
#include "WorldtransfomUpdate.h"

void DeathPerticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	//モデル
	model_ = model;
	//カメラ
	camera_ = camera;

	// ワールド変換の初期化
	for (KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathPerticles::Update() {
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity=
	}


	// 02_11の12ページ
	for (auto& worldTransform : worldTransforms_) {
		// ワールド行列更新（アフィン変換～DirectXに転送）
		worldTransformUpdate(worldTransform);
	}
}

void DeathPerticles::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	// 02_11の13ページ
	for (auto& worldTransform : worldTransforms_) {
		// 02_11_33枚目で&objectColor_を追加
		model_->Draw(worldTransform, *camera_);
	}
	Model::PostDraw();
}
