// IMAGES
HERO_IMAGE = "../Resources/textures/ricky_pause.png"
ZOMBIE_IMAGE = "../Resources/textures/zombie_new.png"
WEAPON_SWORD_IMAGE = "../Resources/textures/sword.png"
WEAPON_SHOTGUN_IMAGE = "../Resources/textures/shotgun.png"
ENVI_LAWNMOWER_IMAGE = "../Resources/textures/mower.png"
WALL_PYRAMID_IMAGE = "../Resources/textures/wall.png"
WALL_FENCE_IMAGE = "../Resources/textures/fence.png"
WALL_HOUSE_IMAGE = "../Resources/textures/house.png"
WALL_ROCK_IMAGE = "../Resources/textures/rock.png"
WALL_TREE_IMAGE = "../Resources/textures/tree.png"
OBJECTIVE_RECORD_IMAGE = "../Resources/textures/record.png"
TILE_IMAGE = "../Resources/textures/tile_new.png"
ENVI_TRASHCAN_IMAGE = "../Resources/textures/trashcan_single.png"

//SONGS
SIMPLE_BEAT = {"bpm":128.0, "path":"songs/SimpleBeat.mp3", "offset":0.0}
OVERDRIVE = {"bpm":128.0, "path":"songs/01 OverDrive.mp3", "offset":0.0}
LOOKING_FOR = {"bpm":128.0, "path":"songs/01 Looking For.mp3", "offset":0.0}
CHILL_DEEP = {"bpm":120.0, "path":"songs/ChillDeepHouse.mp3", "offset":0.0}
IN_PIZZA = {"bpm":90.0, "path":"songs/InPizzaWeCrust.mp3", "offset":0.0}
BLANCO = {"bpm":90.0, "path":"songs/BLANCO.mp3", "offset":0.0}
PICKLE = {"bpm":140.0, "path":"songs/01 Pickles.mp3", "offset":0.0}
GRASS = {"bpm":120.0, "path":"songs/Galactic_Grass.mp3", "offset":0.0}
ROBOTS = {"bpm":110.0, "path":"songs/RobotsInTheRain.mp3", "offset":0.0}
TRAP = {"bpm":140.0, "path":"songs/Its_A_Trap.mp3", "offset":0.0}
RF1 = {"bpm":78.0, "path":"songs/rf1.mp3", "offset":0.0}
RF2 = {"bpm":130.0, "path":"songs/rf2.mp3", "offset":0.0}
RF3 = {"bpm":110.0, "path":"songs/rf3.mp3", "offset":0.0}
RF4 = {"bpm":100.0, "path":"songs/rf4.mp3", "offset":0.0}
RF5 = {"bpm":124.0, "path":"songs/rf5.mp3", "offset":0.0}
RF6 = {"bpm":92.0, "path":"songs/rf6.mp3", "offset":0.0}
RF7 = {"bpm":125.0, "path":"songs/rf7.mp3", "offset":0.0}
RF8 = {"bpm":125.0, "path":"songs/rf8.mp3", "offset":0.0}
RF9 = {"bpm":93.0, "path":"songs/rf9.mp3", "offset":0.0}
RF10 = {"bpm":138.0, "path":"songs/rf10.mp3", "offset":0.0}
/* rf1-10
	1- 78
	2- 130
	3- 110
	4- 100
	5- 124
	6- 92
	7- 125
	8- 125
	9- 93
	10- 138
	*/





TILE_SIZE = 256;
NODES_PER_TILE = 4;
WALL_SIZE = 64;
ZOMBIE_SIZE = 128;
HERO_HEIGHT = 128;
HERO_WIDTH = 85;
SWORD_WIDTH = 60;
SWORD_HEIGHT = 35;
objId = 0;
level = new function(){
	this.tip = ""
	this.bkgX = 1 * 10;
	this.bkgY = 1 * 10;
	this.nodesX = this.bkgX * NODES_PER_TILE;
	this.nodesY = this.bkgY * NODES_PER_TILE;
	this.nodeWidth = this.bkgX * TILE_SIZE / this.nodesX;
	this.nodeHeight = this.bkgY * TILE_SIZE / this.nodesY;
	//this.nWalls = 0;
	//this.nZombies = 0;
	//this.nWeapons = 0;
	this.zombieObjective = 0;
	this.objectives = {};
	this.walls = {};
	this.zombies = {};
	this.weapons = {};
	this.environment_weapons = {};
	this.startX = 0;
	this.startY = 0;
	this.track = SIMPLE_BEAT;
};
ia = document.getElementById("ItemAdd");
it = document.getElementById("ItemType");
ia.onchange = function(){
	switch(ia.options[ia.selectedIndex].value){
		case "start location":
			it.innerHTML = "<option value='none'>Default</option>";
		case "zombie":
			it.innerHTML = "<option value='cop'>Police</option>";
			break;
		case "weapon":
			it.innerHTML = "<option value='sword'>Sword</option> <option value='shotgun'>Shotgun</option>";
			break;
		case "enviweapon":
			it.innerHTML = "<option value='lawnmower'>Lawn Mower</option> <option value='trashcan'>Trash Can</option>";
			break;
		case "wall":
			it.innerHTML = "<option value='pyramid'>Pyramid</option> <option value='rock'>Rock</option> <option value='fence'>Fence</option> <option value='tree'>Tree</option> <option value='house'>House</option>";
			break;
		case "objective":
			it.innerHTML = "<option value='record'>Record</option>";
			break;
	}
	it.selectedIndex = 0;
};
ia.onchange();
function generateBackground(){
	document.getElementById('display').innerHTML = "";
	level.nodesX = level.bkgX * NODES_PER_TILE;
	level.nodesY = level.bkgY * NODES_PER_TILE;
	level.nodeWidth = level.bkgX * TILE_SIZE / level.nodesX;
	level.nodeHeight = level.bkgY * TILE_SIZE / level.nodesY;
	document.getElementById('display').style.width = level.bkgX * TILE_SIZE;
	document.getElementById('display').style.height = level.bkgY * TILE_SIZE;
	for (i=0;i<level.bkgX;i++){
		for (j=0;j<level.bkgY;j++){
			img = document.createElement("img");
			img.className = "bkg";
			img.src = TILE_IMAGE;
			img.style.left = i*TILE_SIZE;
			img.style.top = j*TILE_SIZE;
			document.getElementById('display').appendChild(img);
		}
	}
	document.getElementById("Track").value = level.track.path;
	changeTrack();
	/*if(document.getElementById("grid").checked)*/ drawGrid();
}

function importLevel(){
	f = document.getElementById('import').files[0];
	r = new FileReader();
	r.readAsText(f, 'UTF-8');
	r.onload = function(e){
		level = JSON.parse(e.target.result);
		generateBackground();
		if(!level.tip) level.tip = "";
		document.getElementById("tip").value = level.tip;
		objId = 0;
		for(i = 0; i<level.walls.length; i++) addImage(level.walls[i], "wall");
		for(i = 0; i<level.zombies.length; i++) addImage(level.zombies[i], "zombie");
		for(i = 0; i<level.weapons.length; i++) addImage(level.weapons[i], "weapon");
		for(i = 0; i<level.environment_weapons.length; i++) addImage(level.environment_weapons[i], "enviweapon");
		for(i = 0; i<level.objectives.length; i++) addImage(level.objectives[i], "objective");
		addImage({"x":level.startX, "y":level.startY}, "start location");
		document.getElementById("zombieObjective").value = level.zombieObjective;
	};
}
document.getElementById('import').onchange = importLevel;
/*window.requestFileSystem  = window.requestFileSystem || window.webkitRequestFileSystem;

window.requestFileSystem(window.PERSISTENT, 1024*1024, function(fs){
	fs.root.getFile('level.zbl', {create: true, exclusive: true}, function(fileEntry) {
		fileEntry.createWriter(function(fileWriter) {

      fileWriter.seek(fileWriter.length); // Start write position at EOF.

      // Create a new Blob and write it to log.txt.
      var blob = new Blob(['Hello World'], {type: 'text/plain'});
      fileWriter.write(blob);
  	});
  });
});*/
function exportLevel() {
	l = document.getElementById("link");
    if(l) document.getElementById("download").removeChild(l);
    link = document.createElement("a");
    link.innerHTML = "level.zbl";
    link.id = "link";
    link.download = "level.zbl";
    link.onclick = function(){
    	document.getElementById("download").removeChild(link);
    }
    exp = level;
    exp.walls = Object.keys(level.walls).map(function(k){return level.walls[k];});
    exp.zombies = Object.keys(level.zombies).map(function(k){return level.zombies[k];});
    exp.weapons = Object.keys(level.weapons).map(function(k){return level.weapons[k];});
    exp.environment_weapons = Object.keys(level.environment_weapons).map(function(k){return level.environment_weapons[k];});
    exp.objectives = Object.keys(level.objectives).map(function(k){return level.objectives[k];});
    link.href = 'data:text/csv;charset=utf-8,' + escape(JSON.stringify(exp));
    document.getElementById("download").appendChild(link);
}
document.getElementById('export').onclick = exportLevel;

function createObject(event){
	if(event.target.className == "obj" || event.target.className.indexOf("objective")>-1){
		delete level.walls[event.target.id];
		delete level.zombies[event.target.id];
		delete level.weapons[event.target.id];
		delete level.environment_weapons[event.target.id];
		delete level.objectives[event.target.id];
		document.getElementById("display").removeChild(event.target);
		return;
	}
	size_x = size_y = 0;
	switch(ia.options[ia.selectedIndex].value){
		case "wall": 
			switch(it.options[it.selectedIndex].value){
				case "pyramid": case "fence": case "rock": size_x = size_y = WALL_SIZE / 2; break;
				case "tree": size_x = size_y = WALL_SIZE; break;
				case "house": size_x = size_y = WALL_SIZE * 2; break;
			} break;
		case "zombie": size_x = size_y = ZOMBIE_SIZE / 2; break;
		case "start location": size_x = HERO_WIDTH / 2; size_y = HERO_HEIGHT/2; break;
		case "weapon": size_x = SWORD_WIDTH / 2; size_y = SWORD_HEIGHT / 2; break;
		case "enviweapon": size_x = SWORD_WIDTH / 2; size_y = SWORD_HEIGHT / 2; break;
		case "record": size_x = SWORD_WIDTH / 2; size_y = SWORD_HEIGHT / 2; break;
	}
	pos_x = event.pageX - document.getElementById("display").offsetLeft;
	pos_y = event.pageY - document.getElementById("display").offsetTop;
	nx = Math.floor(pos_x/level.nodeWidth);//*level.nodeWidth;
	ny = Math.floor(pos_y/level.nodeHeight);//*level.nodeHeight;
	ix = nx * level.nodeWidth;
	iy = ny * level.nodeHeight;
	img = document.createElement("img");
	img.className = "obj";
	img.style.left = ix;// + size_x;
	img.style.top = iy; //+ size_y;
	obj = {x: nx, y: ny};
	switch(ia.options[ia.selectedIndex].value){
		case "wall":
			obj.type = it.options[it.selectedIndex].value;
			switch(obj.type){
				case "pyramid": img.src = WALL_PYRAMID_IMAGE; break;
				case "tree"   : img.src = WALL_TREE_IMAGE; break;
				case "fence"  : img.src = WALL_FENCE_IMAGE; break;
				case "rock"   : img.src = WALL_ROCK_IMAGE; break;
				case "house"  : img.src = WALL_HOUSE_IMAGE; break;
			}
			level.walls[objId] = obj;
			break;
		case "zombie":
			level.zombies[objId] = obj;
			img.src = ZOMBIE_IMAGE;
			break;
		case "start location":
			start = document.getElementsByClassName("start")[0]
			if(start) document.getElementById("display").removeChild(start);
			img.className += " start";
			img.src = HERO_IMAGE;
			level.startX = nx;
			level.startY = ny;
			break;
		case "weapon":
			obj.type = it.options[it.selectedIndex].value;
			switch(obj.type){
				case "sword"  : img.src = WEAPON_SWORD_IMAGE; break;
				case "shotgun": img.src = WEAPON_SHOTGUN_IMAGE; break;
			}
			level.weapons[objId] = obj; 
			break;
		case "enviweapon":
			obj.type = it.options[it.selectedIndex].value;
			switch(obj.type){
				case "lawnmower": img.src = ENVI_LAWNMOWER_IMAGE; break;
				case "trashcan": img.src = ENVI_TRASHCAN_IMAGE; break;

			}
			level.environment_weapons[objId] = obj;
			break;
		case "objective":
			objv = document.getElementsByClassName("objective")[0]
			if(objv){
				delete level.objectives[objv.id];
				document.getElementById("display").removeChild(objv);
			}
			obj.type = it.options[it.selectedIndex].value;
			img.className += " objective";
			switch(obj.type){
				case "record": img.src = OBJECTIVE_RECORD_IMAGE; break;
			}
			level.objectives[objId] = obj;
			break;	
	}
	img.id = objId++;
	document.getElementById("display").appendChild(img);
}

function addImage(obj, type){
	ix = obj.x * level.nodeWidth;
	iy = obj.y * level.nodeHeight;
	img = document.createElement("img");
	img.className = "obj";
	img.style.left = ix;
	img.style.top = iy;
	switch(type){
		case "wall":
			switch(obj.type){
				case "pyramid": img.src = WALL_PYRAMID_IMAGE; break;
				case "tree"   : img.src = WALL_TREE_IMAGE; break;
				case "fence"  : img.src = WALL_FENCE_IMAGE; break;
				case "rock"   : img.src = WALL_ROCK_IMAGE; break;
				case "house"  : img.src = WALL_HOUSE_IMAGE; break;
			}
		break;
		case "zombie":
			img.src = ZOMBIE_IMAGE;
		break;
		case "start location":
			img.className += " start";
			img.src = HERO_IMAGE;
		break;
		case "weapon":
			switch(obj.type){
				case "sword"  : img.src = WEAPON_SWORD_IMAGE; break;
				case "shotgun": img.src = WEAPON_SHOTGUN_IMAGE; break;
			}
		break;
		case "enviweapon":
			switch(obj.type){
				case "lawnmower": img.src = ENVI_LAWNMOWER_IMAGE; break;
				case "trashcan" : img.src = ENVI_TRASHCAN_IMAGE; break;
			}
		break;
		case "objective":
			img.className += " objective";
			switch(obj.type){
				case "record": img.src = OBJECTIVE_RECORD_IMAGE; break;
			}
		break;
	}
	img.id = objId++;
	document.getElementById("display").appendChild(img);
}


//Audio
function changeTrack(){
	track = document.getElementById("Track").value;
	switch(track){
		case "songs/SimpleBeat.mp3":
			level.track = SIMPLE_BEAT;
			break;
		case "songs/01 OverDrive.mp3":
			level.track = OVERDRIVE;
			break;
		case "songs/01 Looking For.mp3":
			level.track = LOOKING_FOR;
			break;
		case "songs/ChillDeepHouse.mp3":
			level.track = CHILL_DEEP;
			break;
		case "songs/InPizzaWeCrust.mp3":
			level.track = IN_PIZZA;
			break;
		case "songs/BLANCO.mp3":
			level.track = BLANCO;
			break;
		case "songs/01 Pickles.mp3":
			level.track = PICKLE;
			break;
		case "songs/Galactic_Grass.mp3":
			level.track = GRASS;
			break;
		case "songs/RobotsInTheRain.mp3":
			level.track = ROBOTS;
			break;
		case "songs/Its_A_Trap.mp3":
			level.track = TRAP;
			break;
		case "songs/rf1.mp3":
			level.track = RF1;
			break;
		case "songs/rf2.mp3":
			level.track = RF2;
			break;
		case "songs/rf3.mp3":
			level.track = RF3;
			break;
		case "songs/rf4.mp3":
			level.track = RF4;
			break;
		case "songs/rf5.mp3":
			level.track = RF5;
			break;
		case "songs/rf6.mp3":
			level.track = RF6;
			break;
		case "songs/rf7.mp3":
			level.track = RF7;
			break;
		case "songs/rf8.mp3":
			level.track = RF8;
			break;
		case "songs/rf9.mp3":
			level.track = RF9;
			break;
		case "songs/rf10.mp3":
			level.track = RF10;
			break;
	}
	playing = !document.getElementById("audio").paused;
	document.getElementById("audiosrc").src = "../Resources/" + level.track.path;
	document.getElementById("audio").load();
	if(playing) document.getElementById("audio").play();
}

function drawGrid(){
	var c=document.getElementById("display");
	for(i=0;i<level.nodesY+1;i++){
		var l = document.createElement('div');
		l.className = "line horizontal";
		l.style.top = level.nodeHeight*i;
		l.style.left = 0;
		l.style.width = level.nodeWidth*level.nodesX;
		c.appendChild(l);
	}
	for(i=0;i<level.nodesX+1;i++){
		var l = document.createElement('div');
		l.className = "line vertical";
		l.style.left = level.nodeWidth*i;
		l.style.top = 0;
		l.style.height = level.nodeHeight*level.nodesY;
		c.appendChild(l);
	}
}

function flipGrid(){
	if(document.getElementById("grid").checked){
		drawGrid();
	}else{
		lines = document.getElementsByClassName("line");
		for(line in lines) if(line.parent) line.parent.removeChild(line);
	}
}

function changeZombieObjective(){
	level.zombieObjective = +document.getElementById("zombieObjective").value;
}

function updateTip(){
	level.tip = document.getElementById("tip").value.trim();
}

function clearTip(){
	document.getElementById("tip").value = "";
	updateTip();
}


////Main
generateBackground();
document.getElementById("update").onclick = function(){
	level.bkgX = 1* document.getElementById("inX").value;
	level.bkgY = 1* document.getElementById("inY").value;
	level.nodesX = level.bkgX * NODES_PER_TILE;
	level.nodesY = level.bkgY * NODES_PER_TILE;
	level.nodeWidth = level.bkgX * TILE_SIZE / level.nodesX;
	level.nodeHeight = level.bkgY * TILE_SIZE / level.nodesY;
	document.getElementById("display").style.width = level.bkgX * TILE_SIZE;
	document.getElementById("display").style.height = level.bkgY * TILE_SIZE;
	generateBackground();
};
document.getElementById("display").onclick = createObject;