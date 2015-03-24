#include "AIController.h"
#include "GameState.h"
#include "Zombie.h"
#include "Wall.h"
#include "LevelMap.h"

#define FLOCK_RADIUS 400.0f
#define MIN_DIST 115.0f
#define IMPULSE 2000.0f
#define COHESION 50.0f
#define ALIGNMENT 50.0f
#define SEPERATION 300.0f
#define ZOMBIENESS 50.0f
#define ATTRACTION 30.0f


AIController::AIController()
{
}


AIController::~AIController()
{
}

//Flocking algorithm implementation
void AIController::update(GameState *state){
	CTypedPtrDblList<Zombie> flock;
	CTypedPtrDblList<b2Body> tooClose;
	Zombie *curZ, *othZ;
	b2Vec2 dir;//, tmp;
	float len;
	for (CTypedPtrDblElement<Zombie> *cur = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(cur); cur = cur->Next()){
		curZ = cur->Data();
		dir.SetZero();
		//Calculate neighbor groups
		flock.RemoveAll();
		tooClose.RemoveAll();
		for (CTypedPtrDblElement<Zombie> *other = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(other); other = other->Next()){
			othZ = other->Data();
			if (other != cur){
				len = (curZ->body->GetPosition() - othZ->body->GetPosition()).Length();
				if (len <= FLOCK_RADIUS){
					if (len < MIN_DIST){
						tooClose.AddTail(othZ->body);
					}
					else{
						flock.AddTail(othZ);
					}
				}
			}
		}
		for (int i = 0; i < state->level->nWalls; i++){

			len = (curZ->body->GetPosition() - state->level->walls[i].body->GetPosition()).Length();
			if (len < MIN_DIST){
				tooClose.AddTail(state->level->walls[i].body);
			}
		}
		//COHESION
		curZ->cohesion.SetZero();
		if (flock.GetCount() > 0) {
			for (CTypedPtrDblElement<Zombie> *other = flock.GetHeadPtr(); !flock.IsSentinel(other); other = other->Next()){
				curZ->cohesion += other->Data()->body->GetPosition();
			}
			curZ->cohesion.Set(curZ->cohesion.x / flock.GetCount(), curZ->cohesion.y / flock.GetCount());
			curZ->cohesion -= curZ->body->GetPosition();
			curZ->cohesion.Normalize();
			curZ->cohesion *= COHESION;
			dir += curZ->cohesion;
		}
		//ALIGNMENT
		curZ->alignment.SetZero();
		for (CTypedPtrDblElement<Zombie> *other = flock.GetHeadPtr(); !flock.IsSentinel(other); other = other->Next()){
			if (!isZero(other->Data()->body->GetLinearVelocity())){
				curZ->alignment += 1 / other->Data()->body->GetLinearVelocity().Length() * other->Data()->body->GetLinearVelocity();
			}
		}
		curZ->alignment.Normalize();
		curZ->alignment *= ALIGNMENT;
		dir += curZ->alignment;
		//SEPERATION
		curZ->seperation.SetZero();
		if (tooClose.GetCount() > 0){
			for (CTypedPtrDblElement<b2Body> *other = tooClose.GetHeadPtr(); !tooClose.IsSentinel(other); other = other->Next()){
				curZ->seperation += other->Data()->GetPosition();
			}
			curZ->seperation.Set(curZ->seperation.x / tooClose.GetCount(), curZ->seperation.y / tooClose.GetCount());
			curZ->seperation = curZ->body->GetPosition() - curZ->seperation;
			curZ->seperation.Normalize();
			curZ->direction.Set(curZ->seperation.x, curZ->seperation.y);
			curZ->seperation *= SEPERATION;
			dir += curZ->seperation;

		}
		//ZOMBIENESS
		setVecGaussian(&curZ->zombiness);
		curZ->zombiness.Set(curZ->zombiness.x / 5, curZ->zombiness.y / 5);
		curZ->direction += curZ->zombiness;
		curZ->direction.Normalize();
		curZ->zombiness = ZOMBIENESS * curZ->direction;
		dir += curZ->zombiness;
		//ATTRACTION
		curZ->attraction = state->ship->body->GetPosition() - curZ->body->GetPosition();
		curZ->attraction.Normalize();
		curZ->attraction *= ATTRACTION * curZ->awareness;
		dir += curZ->attraction;
		//apply
		dir.Normalize();
		dir *= IMPULSE;
		curZ->aidir = dir;
		curZ->body->ApplyLinearImpulse(dir, curZ->body->GetPosition(), true);
	}
}
