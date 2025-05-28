#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;


class Player {
public:

	//初期化
	void Initilize(Model* model, Camera* camera, const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw();

	enum class LRDiretion {
		kRight,
		kLeft,
	};
	
	// 接地状態フラグ
	bool onGround_ = true;

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	//モデル
	KamataEngine::Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	//カメラ
	KamataEngine::Camera* camera_ = nullptr;

	//計算
	Math math_;

	// 速度
	Vector3 velocity_ = {};
	
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.3f;
	
	//左右向き
	LRDiretion lrDirection_ = LRDiretion::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間<秒>
	static inline const float kTimeTurn = 0.4f;

	

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.78f;

	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.2f;

	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 3.0f;

};
