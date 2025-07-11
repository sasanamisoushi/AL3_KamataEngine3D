#pragma once
#include "KamataEngine.h"
#include "Math.h"


using namespace KamataEngine;

class Player;

class Enemy {
public:

	// 振るまい
	enum class Behavior {
		kUnknown = -1,
		kWalk,   // 通常状態
		kDefeated, // 攻撃中
	};

	//初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw();


	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	//経過時間
	float walkTimer_ = 0.0f;

	// AABBを取得
	AABB GetAABB();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突応答
	void OnCollision(const Player* player);

	bool IsDead() const { return isDead_; }

	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

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

	//当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//デスフラグ
	bool isDead_ = false;

	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f;

	bool isCollisionDisabled_ = false;
};
