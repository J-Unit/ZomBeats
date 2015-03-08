enum b2TypesEnum { ShipType, WallType, ZombieType };
struct ModelType{
private:
	void *ref;
public:
	b2TypesEnum type;

	ModelType(b2TypesEnum t, void* r){
		type = t;
		ref = r;
	}
	/*Type(Ship *s){
		type = ShipType;
		ref = s;
	}
	Type(Wall *w){
		type = WallType;
		ref = w;
	}

	Wall *getWall(){
		return (Wall *)ref;
	}

	Ship *getShip(){
		return (Ship *)ref;
	}*/


};

