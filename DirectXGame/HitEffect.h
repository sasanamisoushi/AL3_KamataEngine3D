#pragma once
#include "KamataEngine.h"



class HitEffect {
public:

	enum class State {
		kSpread, // 拡大中
		kFade,   // フェードアウト中
		kDead    // 死亡
	};

	void Initialize(const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	// インスタンス生成と初期化
	static HitEffect* Create(const KamataEngine::Vector3& position);

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	bool IsDead() const { return state_ == State::kDead; }

private:

	// 拡大アニメーションの時間
	static inline const uint32_t kSpreadTime = 10;

	// フェードアウトアニメーションの時間
	static inline const uint32_t kFadeTime = 20;

	// エフェクトの寿命
	static inline const uint32_t kLifetime = kSpreadTime + kFadeTime;

	//モデル(借りてくる用)
	static KamataEngine::Model* model_;
	//カメラ(借りてくる用)
	static KamataEngine::Camera* camera_;

	//円のワールドトランスフォーム
	KamataEngine::WorldTransform circleWorldtransform_;

	// デスフラグ
	bool isDead_ = false;

	KamataEngine::ObjectColor objectColor_;

	// 楕円エフェクトの数
	static const inline uint32_t kellipseEffectNum = 2;

	//楕円形のワールドトランスフォーム
	std::array<KamataEngine::WorldTransform, kellipseEffectNum> ellipseWorldTransforms_;

	State state_ = State::kSpread;

	// カウンター
	uint32_t counter_ = 0;
};