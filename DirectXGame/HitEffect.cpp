#include "HitEffect.h"
#include "WorldtransfomUpdate.h"
#include <random>
#include <numbers>

//静的メンバ変数の実体
KamataEngine::Model* HitEffect::model_ = nullptr;
KamataEngine::Camera* HitEffect::camera_ = nullptr;

void HitEffect::Initialize(const KamataEngine::Vector3& position) {

	//乱数生成エンジン
	std::random_device seedGenerator;
	//メルセンヌ・ツイスターエンジン
	std::mt19937_64 randomEngine;
	//メルセンヌ・ツイスターエンジンの初期化
	randomEngine.seed(seedGenerator());

	//指定範囲の乱数生成器(浮動小数点数用）
	std::uniform_real_distribution<float> rotationDistribution(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

	
	

	//楕円エフェクト
	for (KamataEngine::WorldTransform& worldTransform : ellipseWorldTransforms_) {
		
		worldTransform.rotation_ = {0.0f, 0.0f, rotationDistribution(randomEngine)};
		worldTransform.translation_ = position;
		//worldTransform.translation_.z = -1.0f;
		worldTransform.Initialize();
		

	}

	// 円形エフェクト
	circleWorldtransform_.translation_ = position;
	circleWorldtransform_.Initialize();
	circleWorldtransform_.translation_.z = -1.0f;
	objectColor_.Initialize();
	
}

void HitEffect::Update() {

	if (IsDead()) {
		return; 
	}

	switch (state_) {
	case HitEffect::State::kSpread: {
		++counter_;
		float scale = 0.5f + static_cast<float>(counter_)/kSpreadTime*0.5f;
		const float slashScale = 2.0f;
		for (auto& slashWorldTransform : ellipseWorldTransforms_) {
			slashWorldTransform.scale_ = {0.1f, scale * slashScale, 1.0f};
		}

		const float circleScale = 1.0;

		circleWorldtransform_.scale_ = {scale * circleScale, scale * circleScale, 1.0f};

		if (counter_ >= kSpreadTime) {
			state_ = State::kFade;
			counter_ = 0; // カウンターをリセット
		}
		break;
	}
	case HitEffect::State::kFade: {
		++counter_;
		objectColor_.SetColor(KamataEngine::Vector4{1.0f, 1.0f, 1.0f, 1.0f - static_cast<float>(counter_) / kFadeTime});

		if (++counter_ >= kFadeTime) {
			state_ = State::kDead;
		}

		break;
	}
	default:
		break;
	}

	//円形エフェクト
	worldTransformUpdate(circleWorldtransform_);

	// 楕円エフェクト
	for (auto& slashWorldTransform : ellipseWorldTransforms_) {
		worldTransformUpdate(slashWorldTransform);
	}


}

void HitEffect::Draw() { 
	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();
	KamataEngine::Model::PreDraw(dxCommon->GetCommandList());
	
	if (IsDead()) {
		return; 
	}
	
	// 楕円エフェクト
	for (auto& slashWorldTransform : ellipseWorldTransforms_) {
		model_->Draw(slashWorldTransform, *camera_, &objectColor_); 
	}

	// 円形エフェクト
	model_->Draw(circleWorldtransform_, *camera_, &objectColor_); 
	KamataEngine::Model::PostDraw();
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
