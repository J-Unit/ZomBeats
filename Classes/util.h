#ifndef UTIL_H
#define UTIL_H

#include "Box2D/Box2D.h"
#include <random>

#define CALIBRATION_LEVEL 0

static void setVecRandom(b2Vec2 *v){
	float x = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	float y = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	v->Set(x, y);
	v->Normalize();
}

static b2Vec2 randomUnitVector(){
	float x = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	float y = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	b2Vec2 ret(x, y);
	ret.Normalize();
	return ret;
}

static std::default_random_engine genr;
static std::normal_distribution<double> norm(0, 1);
static void setVecGaussian(b2Vec2 *v){
	float x = norm(genr);
	float y = norm(genr);
	v->Set(x, y);
}

static bool rectanglesOverlap(float b1x1, float b1y1, float b1x2, float b1y2, float b2x1, float b2y1, float b2x2, float b2y2){
	return !((b1x1 > b2x2 || b2x1 > b1x2) || (b2y2 < b1y1 || b1y2 < b2y1));
}

static bool isZero(b2Vec2 v){
	return v.x == 0.0f && v.y == 0.0f;
}
/*
static Vec2 cVec2(b2Vec2 v){
	return Vec2(v.x, v.y);
}

static b2Vec2 cb2Vec2(Vec2 v){
	return b2Vec2(v.x, v.y);
}*/

#endif
