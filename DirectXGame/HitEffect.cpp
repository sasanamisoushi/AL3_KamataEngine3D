#include "HitEffect.h"
#include "WorldtransfomUpdate.h"

//静的メンバ変数の実体
KamataEngine::Model* HitEffect::model_ = nullptr;
KamataEngine::Camera* HitEffect::camera_ = nullptr;

void HitEffect::Initialize(const KamataEngine::Vector3& position) {

	// 円形エフェクト
	circleWorldtransform_.translation_ = position;

	
}

void HitEffect::Update() {
	//円形エフェクト
	worldTransformUpdate(circleWorldtransform_);
}

void HitEffect::Draw() { 
	// 円形エフェクト
	model_->Draw(circleWorldtransform_, *camera_); 
}

HitEffect* HitEffect::Create(const KamataEngine::Vector3& position) { 
	//インスタンス生成
	HitEffect* instanse = new HitEffect();
	//newの失敗を検出
	assert(instanse);
	//インスタンスの初期化
	instanse->Initialize(position);
	//初期化したインスタンス
	return instanse; 
}
