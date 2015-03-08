class Ship;
class Wall;
class Zombie;
class Weapon;

enum Types { ShipType, WallType, ZombieType, WeaponType, NONE };
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


};
