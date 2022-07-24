"use strict";

// ===========================================================================

let outputColor = toColour(72, 144, 48, 255);
let errorColour = toColour(72, 144, 48, 255);
let godMode = false;

// ===========================================================================

addEventHandler("OnMapLoaded", (event, mapName) => {
	//if(game.mapName != "FREERIDENOC") {
	//	game.changeMap("FREERIDENOC");
	//	return false;
	//}

	game.setTrafficEnabled(false);
	game.createPlayer("TommyCOATHAT.i3d", new Vec3(-1981.51, -4.66, 29.37), 0.0);
});

// ===========================================================================

addEventHandler("OnProcess", (event, deltaTime) => {
	if(godMode) {
		localPlayer.health = 200;
	}
});

// ===========================================================================

addEventHandler("OnPedEnteringVehicle", function(event, ped, vehicle, seat) {
    message(`You are entering a vehicle`);
});

// ===========================================================================

addEventHandler("OnPedEnteredVehicle", function(event, ped, vehicle, seat) {
    message(`You entered a vehicle`);
});

// ===========================================================================

addEventHandler("OnPedExitingVehicle", function(event, ped, vehicle, seat) {
    message(`You are exiting a vehicle`);
});

// ===========================================================================

addEventHandler("OnPedExitedVehicle", function(event, ped, vehicle, seat) {
    message(`You have exited a vehicle`);
});

// ===========================================================================

addCommandHandler("veh", (command, params, client) => {
	let model = getVehicleModelFromParams(params);

	if(!model) {
		message("That vehicle model is invalid!");
		return false;
	}

	let vehicle = game.createVehicle(`${model}.i3d`, getPosInFrontOfPos(localPlayer.position, localPlayer.rotation.z, 5), localPlayer.rotation.z);

	if(game.mapName == "FREERIDENOC") {
		vehicle.lights = true;
	} else {
		vehicle.lights = false;
	}

    message(`You spawned a ${vehicleNames[vehicleModels.indexOf(model)]}`, COLOUR_YELLOW);
});

// ===========================================================================

addCommandHandler("gun", (command, params) => {
	let weaponId = toNumber(params) || 10;

	localPlayer.giveWeapon(weaponId, 999, 999);
});

// ===========================================================================

addCommandHandler("god", (command, params) => {
	godMode = true;
});

// ===========================================================================

addCommandHandler("siren", (command, params) => {
	if(localPlayer.vehicle == null) {
		message("You need to be in a vehicle!");
		return false;
	}

	localPlayer.vehicle.siren = !localPlayer.vehicle.siren;
});

// ===========================================================================

addCommandHandler("engine", (command, params) => {
	if(localPlayer.vehicle == null) {
		message("You need to be in a vehicle!");
		return false;
	}

	localPlayer.vehicle.engine = !localPlayer.vehicle.engine;
});

// ===========================================================================

addCommandHandler("lights", (command, params) => {
	if(localPlayer.vehicle == null) {
		message("You need to be in a vehicle!");
		return false;
	}

	localPlayer.vehicle.lights = !localPlayer.vehicle.lights;
});

// ===========================================================================

addCommandHandler("locked", (command, params) => {
	if(localPlayer.vehicle == null) {
		message("You need to be in a vehicle!");
		return false;
	}

	localPlayer.vehicle.locked = !localPlayer.vehicle.locked;
});

// ===========================================================================

addCommandHandler("skin", (command, params) => {
	let model = getSkinModelFromParams(params);

	if(!model) {
		message("That ped model is invalid!");
		return false;
	}

	localPlayer.skin = `${model}.i3d`;
});

// ===========================================================================

function getVehicleModelFromParams(params) {
	for(let i in vehicleNames) {
		if(vehicleNames[i].toLowerCase().indexOf(params.toLowerCase()) != -1) {
			return vehicleModels[i];
		}
	}

	for(let i in vehicleModels) {
		if(vehicleModels[i].toLowerCase().indexOf(params.toLowerCase()) != -1) {
			return vehicleModels[i];
		}
	}

	return false;
}

// ===========================================================================

function getSkinModelFromParams(params) {
	//for(let i in skinNames) {
	//	if(skinNames[i].toLowerCase().indexOf(params.toLowerCase()) == -1) {
	//		return skinModels[i];
	//	}
	//}

	for(let i in skinModels) {
		if(skinModels[i].toLowerCase().indexOf(params.toLowerCase()) != -1) {
			return skinModels[i];
		}
	}

	return false;
}

// ===========================================================================

let vehicleModels = [
	"fordtTud00",
	"fordtTud01",
	"fordtTud02",
	"fordtTud03",
	"fordtTud04",
	"fordtto00",
	"fordtto01",
	"fordtto02",
	"fordtto03",
	"fordtto04",
	"fordtru00",
	"fordtru01",
	"fordtru02",
	"fordtru03",
	"fordtru04",
	"fordtpi00",
	"fordtpi01",
	"fordtpi02",
	"fordtpi03",
	"fordtpi04",
	"fordtFor00",
	"fordtFor01",
	"fordtFor02",
	"fordtFor03",
	"fordtFor04",
	"fordtco00",
	"fordtco01",
	"fordtco02",
	"fordtco03",
	"fordtco04",
	"forAtu00",
	"ForAtu01",
	"ForAtu02",
	"ForAtu03",
	"ForAro00",
	"ForAro01",
	"ForAro02",
	"ForAro03",
	"ForApic00",
	"ForApic01",
	"ForApic02",
	"ForApic03",
	"ForAfo00",
	"ForAfo01",
	"ForAfo02",
	"ForAfo03",
	"ForAde00",
	"ForAde01",
	"ForAde02",
	"ForAde03",
	"ForAcou00",
	"ForAcou01",
	"ForAcou02",
	"ForAcou03",
	"ForAtu00",
	"ForAtu01",
	"ForAtu02",
	"ForAtu03",
	"forVco00",
	"forVco01",
	"forVco02",
	"forVco03",
	"forVfor00",
	"forVfor01",
	"forVfor02",
	"forVfor03",
	"forVro00",
	"forVro01",
	"forVro02",
	"forVro03",
	"forVto00",
	"forVto01",
	"forVto02",
	"forVto03",
	"forVtud00",
	"forVtud01",
	"forVtud02",
	"forVtud03",
	"miller00",
	"speedster00",
	"speedster01",
	"speedster02",
	"alfa00",
	"alfa8C00",
	"alfa8C01",
	"merced500K00",
	"merced500K01",
	"bugatti00",
	"bugatti01",
	"pontFor00",
	"pontFor01",
	"pontTud00",
	"pontTud01",
	"blackha00",
	"blackha01",
	"black00",
	"taxi00",
	"hudcou00",
	"hudcou01",
	"hudcou02",
	"hudfor00",
	"hudfor01",
	"hudfor02",
	"hudtu00",
	"hudtu01",
	"hudtu02",
	"cad_ford00",
	"cad_phaeton00",
	"cad_road00",
	"hartmann00",
	"hearseCa00",
	"polCad00",
	"chemaFor00",
	"chemaFor01",
	"chemaFor02",
	"polimFor00",
	"chematud00",
	"chematud01",
	"chematud02",
	"polimTud00",
	"chev00",
	"chev01",
	"chev02",
	"poli00",
	"arrow00",
	"cordca00",
	"cordca01",
	"cordph00",
	"cordph01",
	"cordse00",
	"cordse01",
	"deuseJco00",
	"duesenberg00",
	"airflFor00",
	"airflFor01",
	"airfltud00",
	"airfltud01",
	"buiCou00",
	"buiCou01",
	"buiCou02",
	"buigang00",
	"buikFor00",
	"buikFor01",
	"buikFor02",
	"Ambulance00",
	"fire00",
	"hearseA00",
	"truckA00",
	"truckB00",
	"TruckBxx00",
	"truckBx00",
	"phantom00",
	"thunderbird00",
	"FordHOT00",
	"Blackdragon00",
	"cord_sedanH00",
	"Flamer00",
	"fordApick00",
	"fordApicktaxi00",
	"fordTH00",
	"FThot00",
	"hotrodp200",
	"hotrodp300",
	"hotrodp400",
	"hotrodp500",
	"chevroletm6H00",
	"TBirdold00",
	"fordAdelH00",
	"hotrodp600",
	"phantomtaxi00",
];

// ===========================================================================

let vehicleNames = [
	"Blue Bolt Ace Tudor", 					// 0
	"Dark Blue Bolt Ace Tudor",
	"Brown Bolt Ace Tudor",
	"Green Bolt Ace Tudor",
	"Red Bolt Ace Tudor",
	"Blue Bolt Ace Touring", 				// 5
	"Dark Blue Bolt Ace Touring",
	"Brown Bolt Ace Touring",
	"Green Bolt Ace Touring",
	"Red Bolt Ace Touring",
	"Blue Bolt Ace Runabout", 				// 10
	"Dark Blue Bolt Ace Runabout",
	"Brown Bolt Ace Runabout",
	"Green Bolt Ace Runabout",
	"Red Bolt Ace Runabout",
	"Blue Bolt Ace Pickup", 				// 15
	"Dark Blue Bolt Ace Pickup",
	"Brown Bolt Ace Pickup",
	"Green Bolt Ace Pickup",
	"Red Bolt Ace Pickup",
	"Blue Bolt Ace Fordor",					// 20
	"Dark Blue Bolt Ace Fordor",
	"Brown Bolt Ace Fordor",
	"Green Bolt Ace Fordor",
	"Red Bolt Ace Fordor",
	"Blue Bolt Ace Coupe",					// 25
	"Dark Blue Bolt Ace Coupe",
	"Brown Bolt Ace Coupe",
	"Green Bolt Ace Coupe",
	"Red Bolt Ace Coupe",
	"Brown Bolt Model B Tudor",				// 30
	"Red Bolt Model B Tudor",
	"Green Bolt Model B Tudor",
	"Dark Blue Bolt Model B Tudor",
	"Brown Bolt Model B Roadster",
	"Red Bolt Model B Roadster",			// 35
	"Green Bolt Model B Roadster",
	"Dark Blue Bolt Model B Roadster",
	"Brown Bolt Model B Pickup",
	"Red Bolt Model B Pickup",
	"Green Bolt Model B Pickup",			// 40
	"Dark Blue Bolt Model B Pickup",
	"Brown Bolt Model B Fordor",
	"Red Bolt Model B Fordor",
	"Green Bolt Model B Fordor",
	"Dark Blue Bolt Model B Fordor",		// 45
	"Brown Bolt Model B Delivery",
	"Red Bolt Model B Delivery",
	"Green Bolt Model B Delivery",
	"Dark Blue Bolt Model B Delivery",
	"Brown Bolt Model B Coupe",				// 50
	"Red Bolt Model B Coupe",
	"Green Bolt Model B Coupe",
	"Dark Blue Bolt Model B Coupe",
	"Brown Bolt Model B Tudor",
	"Red Bolt Model B Tudor",				// 55
	"Green Bolt Model B Tudor",
	"Dark Blue Bolt Model B Tudor",
	"Green Bolt V8 Coupe",
	"Red Bolt V8 Coupe",
	"Blue Bolt V8 Coupe",					// 60
	"Grey Bolt V8 Coupe",
	"Green Bolt V8 Forder",
	"Red Bolt V8 Forder",
	"Blue Bolt V8 Forder",
	"Grey Bolt V8 Forder",					// 65
	"Green Bolt V8 Roadster",
	"Red Bolt V8 Roadster",
	"Blue Bolt V8 Roadster",
	"Grey Bolt V8 Roadster",
	"Green Bolt V8 Touring",				// 70
	"Red Bolt V8 Touring",
	"Blue Bolt V8 Touring",
	"Grey Bolt V8 Touring",
	"Green Bolt V8 Tudor",
	"Red Bolt V8 Tudor",					// 75
	"Blue Bolt V8 Tudor",
	"Grey Bolt V8 Tudor",
	"Brubaker",
	"Silver Bruno Speedster 851",
	"Red Bruno Speedster 851",				// 80
	"Green Bruno Speedster 851",
	"Caesar 8C 2300 Racing",
	"Red Caesar 8C Mostro",
	"Black Caesar 8C Mostro",
	"White Celeste Marque 500",				// 85
	"Brown Celeste Marque 500",
	"Blue Corrozella C-Otto",
	"Green Corrozella C-Otto",
	"Blue Crusader Chromium Forder",
	"Violet Crusader Chromium Forder",		// 90
	"Green Crusader Chromium Forder",
	"Dark Blue Crusader Chromium Forder",
	"Blue Falconer",
	"Red Falconer",
	"Gangster Falconer",					// 95
	"Falconer Yellowcar",
	"Umber Guardian Terraplane Coupe",
	"Beige Guardian Terraplane Coupe",
	"Black Guardian Terraplane Coupe",
	"Umber Guardian Terraplane Fordor",		// 100
	"Beige Guardian Terraplane Fordor",
	"Black Guardian Terraplane Fordor",
	"Umber Guardian Terraplane Tudor",
	"Beige Guardian Terraplane Tudor",
	"Black Guardian Terraplane Tudor",		// 105
	"Lassister Fordor",
	"Lassister Phaeton",
	"Lassister Roadster",
	"Lassister Appolyon",
	"Lassister Charon",						// 110
	"Lassister Police",						// 111
	"Green Shubert Extra Six Forder",
	"White Shubert Extra Six Forder",
	"Blue Shubert Extra Six Forder",
	"Shubert Extra Six Forder Police",		// 115
	"Green Shubert Extra Six Tudor",
	"White Shubert Extra Six Tudor",
	"Blue Shubert Extra Six Tudor",
	"Shubert Extra Six Tudor Police",		// 119
	"Red Shubert Six",						// 120
	"White Shubert Six",
	"Black Shubert Six",
	"Shubert Six Police",					// 123
	"Silver Fletcher",
	"Orange Thor 810 Cabriolet",			// 125
	"Black Thor 810 Cabriolet",
	"Orange Thor 810 Phaeton",
	"Black Thor 810 Phaeton",
	"Orange Thor 810 Sedan",
	"Black Thor 810 Sedan",					// 130
	"Trautenberg Model J",
	"Trautenberg Racer 4WD",
	"Yellow Ulver Airstream Fordor",
	"Green Ulver Airstream Fordor",
	"Yellow Ulver Airstream Tudor",			// 135
	"Green Ulver Airstream Tudor",
	"Blue Wright Coupe",
	"Red Wright Coupe",
	"Green Wright Coupe",
	"Gangster Wright Coupe",				// 140
	"Blue Wright Fordor",
	"Red Wright Fordor",
	"Green Wright Fordor",
	"Bolt Ambulance",
	"Bolt Firetruck",						// 145
	"Bolt Hearse",
	"Bolt Hearse",
	"Bolt Truck Flatbed",
	"Bolt Truck Covered",
	"Bolt Truck (Atlantic Import)",
	"Bolt Truck",							// 150
	"phantom00",
	"thunderbird00",
	"FordHOT00",
	"Blackdragon00",
	"cord_sedanH00",
	"Flamer00",
	"fordApick00",
	"fordApicktaxi00",
	"fordTH00",
	"FThot00",
	"hotrodp200",
	"hotrodp300",
	"hotrodp400",
	"hotrodp500",
	"chevroletm6H00",
	"TBirdold00",
	"fordAdelH00",
	"hotrodp600",
	"phantomtaxi00",
];

// ===========================================================================

let skinModels = [
	"Tommy",
	"TommyBOXER",
	"TommyCOAT",
	"TommyCOATHAT",
	"TommyDELNIK",
	"TommyDELNIKHIGH",
	"TommyFREERIDER",
	"TommyGUN",
	"TommyHAT",
	"TommyHIGH",
	"TommyHIGHBLOOD",
	"TommyHighCOATHAT",
	"TommyHighHAT",
	"TommyNAHAC",
	"TommyOLD",
	"TommyOLDBLOOD",
	"TommyPYTEL",
	"TommyRACER",
	"TommyRACER2",
	"TommyRUKAV",
	"TommySAILOR",
	"TommyTAXIDRIVER",
	"TommyTAXIdriverHIGH",
	"AsisPZ1",
	"Barman01",
	"Bclerk01",
	"Bclerk02",
	"Bguard01",
	"Bguard01M",
	"Bguard02",
	"Bguard03",
	"Bguard03M",
	"Biff",
	"BigDig",
	"BnkO01",
	"BnkO02",
	"BnkO03",
	"BobAut01",
	"Bookmaker01",
	"Bookmaker02",
	"Boxer01",
	"Boxer02",
	"Boxer03",
	"Boxer04",
	"Carlo",
	"China1",
	"Chulig1",
	"Chulig1b",
	"David",
	"Delnik01",
	"Delnik02",
	"Delnik03",
	"Detektiv01",
	"Detektiv02",
	"Detektiv03",
	"Enemy01+",
	"Enemy01",
	"Enemy02+",
	"Enemy02",
	"Enemy03+",
	"Enemy03",
	"Enemy04",
	"Enemy04BLOOD",
	"Enemy04K",
	"Enemy05",
	"Enemy06+",
	"Enemy06",
	"Enemy07+",
	"Enemy07",
	"Enemy08+",
	"Enemy08",
	"Enemy08K",
	"Enemy09+",
	"Enemy09",
	"Enemy09K",
	"Enemy10+",
	"Enemy10",
	"Enemy10K",
	"Enemy11K",
	"Enemy12",
	"Enemy12K",
	"Enemy13C",
	"Enemy91",
	"Enemy92",
	"FMVENemy11K",
	"FREEgang01",
	"FREEgang02",
	"FrankHIGH",
	"Friend1",
	"Friend2",
	"Gangster01",
	"Gangster02",
	"Gangster03",
	"Gangster04",
	"Gangster05",
	"GodzMan1",
	"Guard01",
	"Guard02",
	"Hasic01",
	"HighCivil",
	"HighCivilBLOOD",
	"Homeless01",
	"Hoolig01",
	"Hoolig02",
	"Hoolig03",
	"Hoolig04",
	"Hoolig05",
	"Hoolig06",
	"I04Delnik01+",
	"I04Delnik01",
	"Joe",
	"Kasar",
	"Knez",
	"LifeG01",
	"Lucas",
	"Luigi",
	"Malticka1",
	"MorelloHIGH",
	"MorelloLOW",
	"NormanHIGH",
	"Organizator01",
	"Paulie",
	"PaulieCOATHAT",
	"PaulieCTHIGH",
	"PaulieCorpse",
	"PaulieHIGH",
	"Pepe",
	"PoliceMan01",
	"PoliceMan02",
	"Politik",
	"PortGuard01",
	"PortGuard02",
	"ProdZ1",
	"Prokur",
	"Radni01",
	"Radni02",
	"Ralph",
	"RalphHIGH",
	"ReditelB",
	"ReditelH",
	"RidicNakladaku",
	"SalMan01K",
	"SalMan02K",
	"SalMan03",
	"SalMan03K",
	"SalMan04",
	"SalMan05",
	"SalMan05K",
	"Salieri2",
	"SalieriHIGH",
	"SalieriHIGH2",
	"SalieriLOW",
	"Sam",
	"SamCOATHAT",
	"SamHIGH",
	"SamHIGHblood1",
	"SamHIGHblood2",
	"SamHIGHblood3",
	"SamHIGHblood4",
	"Samblood1",
	"Sergio",
	"SergioBLOOD",
	"SynRad1",
	"SynRad1BLOOD",
	"SynRad1DEAD",
	"Tony",
	"VincenzoHIGH",
	"VincenzoLOW",
	"Vrabec",
	"Vratny1",
	"Vypravci",
	"Vypravci2",
	"WillG1",
	"WillG2",
	"WillMan01",
	"WillMan02",
	"Zavod1",
	"Zavod2",
	"Zavod3",
	"ZavodFMV1",
	"ZavodFMV2",
	"civil02",
	"civil03",
	"civil04",
	"civil05",
	"civil06",
	"civil11",
	"civil11M",
	"civil12",
	"civil13",
	"civil14",
	"civil15",
	"civil16",
	"civil16M",
	"civil17",
	"civil18",
	"civil19",
	"civil19M",
	"civil21",
	"civil21N",
	"civil22",
	"civil31",
	"civil32",
	"civil33",
	"civil34",
	"civil35",
	"civil36",
	"civil36M",
	"civil37",
	"civil38",
	"civil39",
	"civil40",
	"civil41",
	"civil42",
	"civil42M",
	"civil43",
	"civil44",
	"civil51",
	"civil51M",
	"civil52",
	"civil53",
	"civil54",
	"civil54M",
	"civil55",
	"civil55M",
	"civil56",
	"civil56M",
	"civil57",
	"civil57M",
	"civil60",
	"civil61",
	"civil62",
	"civil63",
	"civil70",
	"civil70M",
	"civil71",
	"civil72",
	"frank",
	"ohorelec01",
	"pianist1",
	"pol01",
	"pol02",
	"pol03",
	"pol11",
	"pol12",
	"pol13",
	"polim62",
	"pumpar01",
	"recep",
	"sailor01",
	"sailor01M",
	"sailor02",
	"sailor02M",
	"sailor03",
	"waiter01",
	"waiter01M",
	"waiter02",
	"waiter02M",
	"waiter03",
	"Alice1",
	"Berta",
	"Bitch01",
	"Bitch02",
	"Bitch02Mask",
	"Bitch03M",
	"CarlZen1",
	"Czena01",
	"Czena02",
	"Czena03",
	"Czena04",
	"Czena05",
	"Czena06",
	"Czena07",
	"Czena07M",
	"Czena08",
	"Czena09",
	"Czena09M",
	"Czena10",
	"Czena10M",
	"Czena11",
	"Czena11M",
	"Czena12",
	"Czena13",
	"FMVCzena03",
	"FMVCzena04",
	"March1",
	"Michelle",
	"MichelleLOW",
	"Milenka1",
	"Sarah1",
	"Sarah1Obl",
	"Sarah2",
	"Sarah2HIGH",
	"Sarah2HIGHnaha",
	"Sarah2LOW",
	"Serv01",
	"ZenaRad01",
	"gab",
	"ShadowGirlHIGH",
	"ShadowMan",
	"ShadowManHIGH",
	"ShadowManLOW2",
	"UfoPanak",
	"invisible"
];

let weaponNames = {
	2: "Knuckle Duster",
	3: "Knife",
	4: "Baseball Bat",
	5: "Molotov Cocktail",
	6: "Colt Detective Special",
	7: "S&W Model 27 Magnum",
	8: "S&W Model 10 M&P",
	9: "Colt 1911",
	10: "Thompson 1928",
	11: "Pump-action Shotgun",
	12: "Sawed-off Shotgun",
	13: "US Rifle, M1903 Springfield",
	14: "Mosin-Nagant 1891/30",
	15: "Grenade",
	17: "Bucket",
	20: "Steel Bar",
	25: "Crowbar",
	28: "Wooden Plank",
	29: "Bottle",
	31: "Sword",
	32: "Dogs Head",
};

// ===========================================================================

function getPosInFrontOfPos(pos, angle, distance) {
    while(angle < 0.0)
        angle += 360.0;
    while(angle > 360.0)
        angle -= 360.0;

    angle = degToRad(angle);

    let x = (pos.x+((Math.cos(angle-(Math.PI/2)))*distance));
    let y = pos.y;
    let z = (pos.z+((Math.sin(angle+(Math.PI/2)))*distance))

    return new Vec3(x, y, z);
}

// ===========================================================================

function degToRad(deg) {
	return deg * Math.PI / 180;
}

// ===========================================================================

function radToDeg(rad) {
	return rad * 180 / Math.PI;
}

// ===========================================================================