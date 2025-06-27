#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

Vector3& operator+=(Vector3& lhv, const Vector3& rhv);
Vector3& operator-=(Vector3& lhv, const Vector3& rhv);
Vector3& operator*=(Vector3& v, float s);
Vector3& operator/=(Vector3& v, float s);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v1, const float f);

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡大縮小
Matrix4x4 MkeScaleMatrix(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRoteXMatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 線形保管
float Lerp(float a, float b, float t);

// イーズインアウト
float EaseInOut(float a, float b, float t);

// 02_06のスライド24枚目のLerp関数
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

//AABB関数
struct AABB {
	Vector3 min;
	Vector3 max;
};


bool IsCollision(const AABB& aabb1, const AABB& aabb2);
