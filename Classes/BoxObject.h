#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include "Types.h"
#include "Box2d/Box2d.h"
//#include "Box2D.h"

class BoxObject{
public:
	b2Body *body;
	b2BodyDef bodyDef;
	b2FixtureDef fixture;
	float width, height;
protected:
	Type type;

	BoxObject(){
		type = Type();
	}
};
#endif
