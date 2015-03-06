#include "AIController.h"
#include "GameState.h"
#include "Zombie.h"

#define FLOCK_RADIUS 100.0f
#define MIN_DIST 10.0f
#define IMPULSE 1000.0f
#define COHESION 1.0f
#define ALIGNMENT 1.0f
#define SEPERATION 1.0f
#define ZOMBIENESS 1.0f
#define ATTRACTION 1.0f


AIController::AIController()
{
}


AIController::~AIController()
{
}

//Flocking algorithm implementation
void AIController::update(GameState *state){
	CTypedPtrDblList<Zombie> flock, tooClose;
	CTypedPtrDblElement<Zombie> *cur = state->zombies.GetHeadPtr(), *other;
	Zombie *curZ, *othZ;
	b2Vec2 dir, tmp;
	float len;
	while (!state->zombies.IsSentinel(cur)){
		curZ = cur->Data();
		dir.SetZero();
		//Calculate neighbor groups
		other = state->zombies.GetHeadPtr();
		while (!state->zombies.IsSentinel(other)){
			othZ = other->Data();
			if (other != cur){
				len = (curZ->body->GetPosition() - othZ->body->GetPosition()).Length();
				if (len <= FLOCK_RADIUS){
					if (len < MIN_DIST){
						tooClose.AddTail(othZ);
					}
					else{
						flock.AddTail(othZ);
					}
				}
			}
			other = other->Next();
		}
		//COHESION
		tmp.SetZero();
		other = flock.GetHeadPtr();
		while (!flock.IsSentinel(other)){
			tmp += other->Data()->body->GetPosition();
			other = other->Next();
		}
		tmp.Set(tmp.x / flock.GetCount(), tmp.y / flock.GetCount());
		tmp -= curZ->body->GetPosition();
		dir += COHESION * tmp;
		//ALIGNMENT
		tmp.SetZero();
		other = flock.GetHeadPtr();
		while (!flock.IsSentinel(other)){
			tmp += 1 / other->Data()->body->GetLinearVelocity().Length() * other->Data()->body->GetLinearVelocity();
			other = other->Next();
		}
		tmp.Normalize();
		dir += ALIGNMENT * tmp;
		//SEPERATION
		tmp.SetZero();
		other = tooClose.GetHeadPtr();
		while (!tooClose.IsSentinel(other)){
			tmp += other->Data()->body->GetPosition();
			other = other->Next();
		}
		tmp.Set(tmp.x / tooClose.GetCount(), tmp.y / tooClose.GetCount());
		tmp = curZ->body->GetPosition() - tmp;
		dir += SEPERATION * tmp;
		//ZOMBIENESS
		float x = (rand() - RAND_MAX / 2) / (RAND_MAX / 2);
		float y = (rand() - RAND_MAX / 2) / (RAND_MAX / 2);
		tmp = b2Vec2(x, y);
		dir += ZOMBIENESS * tmp;
		//ATTRACTION
		tmp = state->ship->body->GetPosition() - curZ->body->GetPosition();
		tmp.Normalize();
		dir += ATTRACTION * curZ->awareness * tmp;
		//apply
		dir.Normalize();
		dir *= IMPULSE;
		curZ->body->ApplyLinearImpulse(dir, curZ->body->GetPosition(), true);
		cur = cur->Next();
	}
}
