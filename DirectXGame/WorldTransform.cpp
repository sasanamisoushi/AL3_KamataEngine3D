#include "WorldTransform.h"
#include "Math.h"



void worldTransform(KamataEngine::WorldTransform& worldTransform) {
	//スケール、回転、平均移動を合成して行列を計算する
	worldTransform.matWorld_ = math_.MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	//定数バッファへの書き込み
	worldTransform.TransferMatrix();

}
