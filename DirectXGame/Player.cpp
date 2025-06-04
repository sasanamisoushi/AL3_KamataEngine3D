#define NOMINMAX
#include "Player.h"
#include "WorldtransfomUpdate.h"
#include <cassert>
#include <numbers>
#include <algorithm>
#include "MapChipField.h"


using namespace KamataEngine;

void Player::Initilize(Model* model, Camera* camera, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// モデル
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	UpdateMovement();

	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	//マップ衝突チェック
	collisionDetection(collisionMapInfo);
	
	
	// 移動
	worldTransform_.translation_ += velocity_;

	// 着地フラグ
	bool landing = false;

	// 地面とのあたり判定
	// 下降中
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 着地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空気状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		// タイマーを進める
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列更新
	worldTransformUpdate(worldTransform_);
}

void Player::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, *camera_);
	Model::PostDraw();
}

void Player::UpdateMovement() {
	// 移動入力
	// 接地状態
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				// 右向き切り替え
				if (lrDirection_ != LRDiretion::kRight) {
					lrDirection_ = LRDiretion::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 0.5f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				// 左向き切り替え
				if (lrDirection_ != LRDiretion::kLeft) {
					lrDirection_ = LRDiretion::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 0.5f;
				}
			}
			// 加速/原則
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		// ジャンプ入力
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

		// 空中
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	

}

void Player::collisionDetection(CollisionMapInfo& Info) { 
	collisionDetectionUp(Info); 
	collisionDetectionDown(Info);
	collisionDetectionLeft(Info);
	collisionDetectionRight(Info);
}

void Player::collisionDetectionUp(CollisionMapInfo& Info) { 

	//上昇あり？
	if (Info.move.y <= 0) {
		return;
	}


	//移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
	
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真上のあたり判定
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kblock) {
		hit = true;
	}

	//右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kblock) {
		hit = true;
	}

	//ブロックにヒット？
	if (hit) {
	
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Info.move + Vector3(0, +kHeight / 2.0f, 0));

		//めり込み先ブロックの移動範囲
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		Info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

		//天井に当たったことを記録する
		Info.top = true;
	}

}

void Player::collisionDetectionDown(CollisionMapInfo &Info) { Info; }

void Player::collisionDetectionLeft(CollisionMapInfo& Info) { Info; }

void Player::collisionDetectionRight(CollisionMapInfo& Info) { Info; }

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}