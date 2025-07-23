#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class MapChipField;

class Enemy;

class Player {
public:

	//左右
	enum class LRDiretion {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上
		kNumCorner
	};

	//振るまい
	enum class Behavior {
		kUnknown = -1,
		kRoot,   //通常状態
		kAttack, //攻撃中
	};

	//攻撃フェーズ
	enum class AttackPhase { 
		kUnknown = -1,  //無効な状態
		kAnticipation,  //予備動作
		kAction,        //前進動作
		kRecovery,      //余韻動作
	};

	//初期化
	void Initilize(Model* model,Model* modelAttack, Camera* camera,  const Vector3& position);

	//更新
	void Update();

	//描画
	void Draw();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(const Enemy* enemy);

	// デスフラグのgetter
	bool isDead() const { return isDead_; }

	//通常行動更新
	void BehaviorRootUpdate();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//振るまい
	Behavior behavior_ = Behavior::kRoot;

	//次の振る舞いのリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;

	//通常行動初期化
	void BehaviorRootInitialize();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	// 現在の攻撃フェーズ
	AttackPhase attackPhase_ = AttackPhase::kUnknown;

	bool IsAttack() const { return behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kAction; }

private:
	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	//カメラ
	Camera* camera_ = nullptr;

	// 速度
	Vector3 velocity_ = {};
	
	static inline const float kAcceleration = 0.1f;
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

	// 接地状態フラグ
	bool onGround_ = true;

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 1.0f;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 9.8f;

	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.3f;

	

	
	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.04f;

	void UpdateMovement();


	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool top = false;     // 天井衝突
		bool landing = false; // 着地
		bool hitwall = false;   // 壁接触
		Vector3 move;         // 移動量
	};

	// マップ衝突判定関数
	void collisionDetection(CollisionMapInfo& Info);
	void collisionDetectionUp(CollisionMapInfo& Info);
	void collisionDetectionDown(CollisionMapInfo& Info);
	void collisionDetectionLeft(CollisionMapInfo& Info);
	void collisionDetectionRight(CollisionMapInfo& Info);


	
	
	//指定した角の座標計算
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 接地状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁に接触している場合
	void UpdateOnWall(const CollisionMapInfo& info);

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.0f;

	//微小な数値
	static inline const float kGroundSearchHeight = 0.06f;

	//着地時の減速減衰率
	static inline const float kAttenuationWall = 0.2f;

	//デスフラグ
	bool isDead_ = false;

	//予備動作の時間
	static inline const uint32_t kAnticipationTime = 8;

	//前進動作の時間
	static inline const uint32_t kActionTime = 5;

	//余韻動作の時間
	static inline const uint32_t kRecoveryTime = 10;
	
	Model* modelAttack_ = nullptr;
	KamataEngine::WorldTransform worldTransformAttack_;

};
