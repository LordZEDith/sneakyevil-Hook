/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "Vector.h"
#include "SDK.h"
#include <stdint.h>

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f
#define rad(a) a * 0.01745329251

void AngleVectors(const Vector &angles, Vector *forward);
void VectorTransform(const Vector in1, float in2[3][4], Vector &out);
void SinCos(float a, float* s, float*c);
//void SinCos1(const float rad, float &sin, float &cos);
void VectorAngles(Vector forward, Vector &angles);
void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
void Normalize(Vector &vIn, Vector &vOut);
void CalcAngle(Vector src, Vector dst, Vector &angles);
bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
bool PointVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID, Vector Point);
void CalcAngleYawOnly(Vector src, Vector dst, Vector &angles);
void NormalizeVector(Vector& vec);

inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}