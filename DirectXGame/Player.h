#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class MapChipField;

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

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void UpdateMovement();

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool top = false; //天井衝突
		bool landing = false;//着地
		bool worll = false;//壁接触
		Vector3 move;//移動量
	};

	//マップ衝突判定関数
	void collisionDetection(CollisionMapInfo& Info);
	void collisionDetectionUp(CollisionMapInfo& Info);
	void collisionDetectionDown(CollisionMapInfo& Info);


private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	//モデル
	KamataEngine::Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	//カメラ
	KamataEngine::Camera* camera_ = nullptr;

	

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

	
	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


	
};
