#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include "Types.h"
#include <cocos2d/external/Box2D/Box2D.h>
//#include "Box2D.h"

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