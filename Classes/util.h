#ifndef UTIL_H
#define UTIL_H

#include "Box2D.h"
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

#endif
