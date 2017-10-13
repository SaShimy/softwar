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
	]
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
// const reqClient = new ReqClient(reqPort, id);
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
	updateScreen();
	// if (notif.notification_type === 0) {
	// 	GAME.mapSize = notif.data.map_size;
	// 	GAME.status = notif.data.game_status;
	// 	GAME.players = notif.data.players;
	// 	GAME.energyCells = notif.data.energy_cells;
	// }

});
pubClient.connect("tcp://127.0.0.1:"+pubPort);
pubClient.subscribe("softwar");

;(async () => {
	await reqClient.connect();
	await reqClient.forward();
	await reqClient.attack();
	await reqClient.inspect("#0x02");
	console.log(reqClient.identity, "connected.");
	console.log(await reqClient.selfstats());
	await reqClient._wait(3000);
	console.log(await reqClient.selfstats());
	// await reqClient.forward();
})()
.catch(err => {

});