#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include "Box2D.h"
#include "Types.h"

class BoxObject{
public:
	b2Body *body;
	b2BodyDef bodyDef;
	b2FixtureDef fixture;
protected:
	Type type;

	BoxObject(){
		type = Type();
	}
};
#endif