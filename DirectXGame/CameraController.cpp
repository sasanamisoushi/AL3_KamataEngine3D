#include "CameraController.h"
#include "Math.h"





void CameraController::Initialize(Camera* camera) {

	// カメラの初期化
	camera_ = camera;
}

void CameraController::Update() {
	
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//const Vector3& targetVelocity = target_->GetVelocity();
	
	//追従対象とオフセットと追従対象の速度からカメラの座標を計算
	targetPos_ = targetWorldTransform.translation_ + targetOffset_  * kVelocityBias;

	//座標補間によりゆったり追従
	camera_->translation_ = Lerp(camera_->translation_, targetPos_, kInterpolationRate);

	//追従対象が画面外に出ないように修正
	camera_->translation_.x = max(camera_->translation_.x, targetPos_.x + targetMargin.left);
	camera_->translation_.x = min(camera_->translation_.x, targetPos_.x + targetMargin.right);
	camera_->translation_.y = max(camera_->translation_.y, targetPos_.y + targetMargin.bottom);
	camera_->translation_.y = min(camera_->translation_.y, targetPos_.y + targetMargin.top);
	
	//移動範囲制限
	camera_->translation_.x = max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = max(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = min(camera_->translation_.y, movableArea_.top);

	//行列を更新する
	camera_->UpdateMatrix();

}

void CameraController::Reset() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
