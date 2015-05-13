class Ship;
class Wall;
class Zombie;
class Weapon;
class EnvironmentWeapon;
class GoalObject;

enum Types { ShipType, WallType, ZombieType, WeaponType, EnvironmentWeaponType, GoalType, NONE };
struct Type{
private:
	void *ref;
public:
	Types type;

	Type(){
		type = NONE;
		ref = 0;
	}
	Type(Ship *s){
		type = ShipType;
		ref = s;
	}
	Type(Wall *w){
		type = WallType;
		ref = w;
	}
	Type(Zombie *z){
		type = ZombieType;
		ref = z;
	}
	Type(Weapon *we){
		type = WeaponType;
		ref = we;
	}
	Type(EnvironmentWeapon *ew){
		type = EnvironmentWeaponType;
		ref = ew;
	}
	Type(GoalObject *go){
		type = GoalType;
		ref = go;
	}

	Wall *getWall(){
		return (Wall *)ref;
	}

	Ship *getShip(){
		return (Ship *)ref;
	}

	Zombie *getZombie(){
		return (Zombie *)ref;
	}

	Weapon *getWeapon(){
		return (Weapon *)ref;
	}
	EnvironmentWeapon *getEnvironmentWeapon(){
		return (EnvironmentWeapon *)ref;
	}
	GoalObject *getGoalObject(){
		return (GoalObject *)ref;
	}
};
