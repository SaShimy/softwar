const zmq = require("zeromq");
const args = require("args");
const ReqClient = require("./ReqClient.js");
// const PubClient = require("./PubClient.js");
const GAME = {
	status: 0,
	mapSize: 10,
	players: [
		{x: 0, y:0},
	],
	energyCells: [
		{x:3, y:2},
	],
	ap: 0
};

const isPlayer = (x, y) => {
	for(const player of GAME.players) {
		if (player.x === x && player.y === y) {
			return true;
		}
	}
	return false
}
const isEnergy = (x, y) => {
	for(const cell of GAME.energyCells) {
		if (cell.x === x && cell.y === y) {
			return true;
		}
	}
	return false;
}

args.options([
	{name: "reqPort", description: "Port of the rep req server.", defaultValue: 4242},
	{name: "pubPort", description: "Port of the pub sub server.", defaultValue: 4243},
	{name: "id", description: "Identity of the client."}]);
const {reqPort, pubPort, id} = args.parse(process.argv);
const reqClient = new ReqClient(reqPort, id);
const updateScreen = () => {
	console.log("test")
	process.stdout.write("\033[2J");
	let map = "";
	for (let i = 0; i < GAME.mapSize; i++) {
		let line = "";
		for (let j = 0; j < GAME.mapSize; j++) {
			if (isPlayer(i, j)) {
				line = line + "P ";
			} else if (isEnergy(i, j)) {
				line = line + "E ";
			} else {
				line = line + "# ";
			}
		}
		map += line + "\n";
	}
	console.log(map);
};
const pubClient = zmq.socket("sub");
pubClient.on("message", (channel, message) => {
	const notif = JSON.parse(message.toString().match(/\{.*\}/)[0]);
	if (GAME.status >= 1) {
		updateScreen();
		GAME.ap = 1;
	}
	if (notif.notification_type === 0) {
		GAME.mapSize = notif.data.map_size;
		GAME.status = notif.data.game_status;
		GAME.players = notif.data.players;
		GAME.energyCells = notif.data.energy_cells;
	}

});
pubClient.connect("tcp://127.0.0.1:"+pubPort);
pubClient.subscribe("softwar");

const waitRefill = async () => {
	let refill = false;
	while(!refill) {
		await reqClient._wait(10);
		if (GAME.ap === 1) {
			refill = true;
		}
	}
}

;(async () => {
	await waitRefill();
    await reqClient.connect();
    let action_gather = 0;
    let action_attack = 0;
    let action_leftfwd = 0;
    let action_rightfwd = 0;
    while (1) {
	if (action_gather == 1) {
	    const energy = await reqClient.selfstats();
	    if(energy < 85) {
		await reqClient.gather;
	    }
	    action_gather = 0;
	}
	const tab = JSON.parse(await reqClient.watch());
	for (let i = 0; i < tab.length && tab[i] == "empty"; i++);
	if(tab[i] == "energy") {
	    if(i == 0) {
		await reqClient.forward();
		action_gather = 1;
	    } else if (i == 1) {
		await reqClient.jump();
		await reqClient.leftfwd();
		action_gather = 1;
	    } else if (i == 2) {
		await reqClient.jump();
		action_gather = 1;
	    } else if (i == 3) {
		await reqClient.jump();
		await reqClient.rightfwd();
		action_gather = 1;	
	    }
	} else if(tab[i] != "empty"){
	    if(action_attack >= 0) {
		try {
		    await reqClient.attack();
		} catch(e) {
		    
		}
		action_attack = -2;
	    }
	} else {
	    try {
		await reqClient.forward();
	    } catch(e) {
		    await reqClient.left();
	    }
	}
	action_attack++;
	
	
    }
	/*await reqClient.connect();
	await reqClient.forward();
	await reqClient.attack();
	await reqClient.inspect("#0x02");
	console.log(reqClient.identity, "connected.");
	console.log(await reqClient.selfstats());
	await reqClient._wait(3000);
	console.log(await reqClient.selfstats());*/
	// await reqClient.forward();
})()
.catch(err => {
	console.log("Error:", err);
	process.exit();
});
