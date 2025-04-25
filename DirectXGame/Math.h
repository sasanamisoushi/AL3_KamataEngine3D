#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Math {

	//平行移動
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	//拡大縮小
	Matrix4x4 MkeScaleMatrix(const Vector3& scale);

	//X軸回転行列
	Matrix4x4 MakeRoteXMatrix(float radian);

	// Y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);
};
