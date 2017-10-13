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
};

const PLAYER = {
	ap: 0,
	action_attack: 0,
	alive: true
}

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
	console.log(notif);
	if (notif.notification_type === 0) {
		GAME.mapSize = notif.map_size;
		GAME.status = notif.game_status;
		GAME.players = notif.players || [];
		GAME.energyCells = notif.energy_cells || [];
	}
	if (notif.notification_type === 3) {
		console.log("You are dead.");
		process.exit();
		if (notif.data.identity === reqClient.identity) {
			PLAYER.alive = false;
		} else {
			console.log(`Client ${notif.data.identity} died.`);
		}
	}
	if (notif.notification_type === 4) {
		console.log("You won!");
		process.exit();
	}
	if (GAME.status >= 1 && PLAYER.alive) {
		updateScreen();
	    PLAYER.ap = 1;
	    if(PLAYER.action_attack < 0) {
			PLAYER.action_atttack++;
	    }
	}
	if (!PLAYER.alive) {
		console.log("You are dead.");
		process.exit();
	}
});
pubClient.connect("tcp://127.0.0.1:"+pubPort);
pubClient.subscribe("softwar");

const waitRefill = async () => {
	let refill = false;
	while(!refill) {
		await reqClient._wait(10);
		if (PLAYER.ap === 1) {
			refill = true;
		}
	}
}

;(async () => {
    await reqClient.connect();
    await waitRefill();
    let action_gather = 0;
    let action_leftfwd = 0;
    let action_rightfwd = 0;
    while (1) {
		reqClient._wait(1);
	if (action_gather == 1) {
	    const energy = await reqClient.selfstats();
	    if(energy < 85) {
		if(PLAYER.ap < 1) {
		    await waitRefill();
		}
		await reqClient.gather;
	    }
	    action_gather = 0;
	}
	const tab = (await reqClient.watch()).replace("[", "").replace("]", "").trim().split(",");
	for (var i = 0; i < tab.length && tab[i] == "empty"; i++);
	if(tab[i] == "energy") {
	    if(i == 0) {
		if(PLAYER.ap < 0.5) {
		    await waitRefill();
		}
		try {
			await reqClient.forward();
		} catch (error) {}
		action_gather = 1;
		//await waitRefill();
	    } else if (i == 1) {
		try {		    
		    await reqClient.jump();
		    if(PLAYER.ap < 1) {
			await waitRefill();
		    }
		    await reqClient.leftfwd();
		    action_gather = 1;
		    //await waitRefill();
		} catch(e) {
		    if(PLAYER.ap < 1) {
			await waitRefill();
		    }
			try {
				await reqClient.forward();
			} catch (error) {}
			try {
				await reqClient.forward();
			} catch (error) {}
		    if(PLAYER.ap < 1) {
				try {
					await waitRefill();
			
				} catch (error) {
					
				}
			}
			try {
				await reqClient.leftfwd();
			} catch (error) {
			}
		    action_gather = 1;
		}
	    } else if (i == 2) {
		try {
		    await reqClient.jump();
		    action_gather = 1;
		} catch(e) {
		    if(PLAYER.ap < 1) {
				try {
					await waitRefill();
				} catch (error) {
					
				}
			}
			try {
				await reqClient.forward();
			} catch (error) {}
			try {
				await reqClient.forward();
			} catch (error) {}
		    action_gather = 1;
		}
		
	    } else if (i == 3) {
		try {
		    await reqClient.jump();
		    if(PLAYER.ap < 1) {
			await waitRefill();
		    }
		    await reqClient.rightfwd();
		    action_gather = 1;
		    //await waitRefill();
		} catch(e) {
		    if(PLAYER.ap < 1) {
				try {
					await waitRefill();
				} catch (error) {}
		    }
			try {
				await reqClient.forward();
			} catch (error) {}
			try {
				await reqClient.forward();
			} catch (error) {}
		    if(PLAYER.ap < 1) {
			await waitRefill();
			}
			try {
				await reqClient.rightfwd();
			} catch (error) {}
		    action_gather = 1;
		}
		
	    }
	} else if(tab[i] != "empty"){
	    if(PLAYER.action_attack >= 0) {
		try {
		    if(PLAYER.ap < 1) {
			await waitRefill();
		    }
		    await reqClient.attack();
		} catch(e) {
		}
		PLAYER.action_attack = -2;
	    }
	} else {
	    try {
		if(PLAYER.ap < 0.5) {
		    await waitRefill();
		}
		await reqClient.forward();
	    } catch(e) {
		    await reqClient.left();
	    }
	}
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
