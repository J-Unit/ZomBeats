#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include "Types.h"
#include <cocos2D/external/Box2D/Dynamics/b2Body.h>
#include <cocos2D/external/Box2D/Dynamics/b2Fixture.h>
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