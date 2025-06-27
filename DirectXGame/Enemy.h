#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Enemy {
public:
	//初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw();


	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	//経過時間
	float walkTimer_ = 0.0f;

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	//歩行の速さ
	static inline const float kWalkSpeed = 0.01f;

	//速度
	Vector3 velocity_ = {};

	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;

	//最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 30.0f;

	//アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;
};
