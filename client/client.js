const zmq = require("zeromq");
const inquirer = require("inquirer");

const PORT = "4242";

// const client = zmq.socket("req");
// client.identity = "#0x01";

// client.connect("tcp://127.0.0.1:4242");
// console.log("Connected to server.")

// client.on("message", message => {
// 	console.log("Received:", message.toString());
// });

const connect = () => {
	const client = zmq.socket("req");
	client.connect(`tcp://127.0.0.1:${PORT}`);
	let connected = false;
	while(!connected) {
		client.identity = "#0x" + (Math.random() * (99 - 1) + 1);
		const [status, response] = client.send("forward|null")._outgoing.lastBatch.content.toString().replace(",0", "").split("|");
		console.log(status, response);
		if (status === "ok") {
			connected = true;
		}
	}
};

connect();