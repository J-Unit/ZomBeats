#ifndef UTIL_H
#define UTIL_H

#include <cocos2D/external/Box2D/Box2D.h>
#include <random>


static void setVecRandom(b2Vec2 *v){
	float x = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	float y = float(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
	v->Set(x, y);
	v->Normalize();
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

#endif
