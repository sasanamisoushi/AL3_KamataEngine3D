#pragma once
#include "KamataEngine.h"



class HitEffect {
public:

	void Initialize(const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	// インスタンス生成と初期化
	static HitEffect* Create(const KamataEngine::Vector3& position);

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	bool IsDead() const { return isDead_; }

private:
	//モデル(借りてくる用)
	static KamataEngine::Model* model_;
	//カメラ(借りてくる用)
	static KamataEngine::Camera* camera_;

	//円のワールドトランスフォーム
	KamataEngine::WorldTransform circleWorldtransform_;

	// デスフラグ
	bool isDead_ = false;
};