const zmq = require("zeromq");
const inquirer = require("inquirer");
const { promisify } = require("util");

const PORT = "4242";
let responses = [];

// const client = zmq.socket("req");
// client.identity = "#0x01";

// client.connect("tcp://127.0.0.1:4242");
// console.log("Connected to server.")

// client.on("message", message => {
// 	console.log("Received:", message.toString());
// });

const responseHandler = reply => {
	const callback = responses.shift();
	callback.apply(this, arguments);
}
const client = zmq.socket("req");
client.connect(`tcp://127.0.0.1:${PORT}`);
client.on("message", responseHandler);

const mySend = (message, callback) => {
	responses.push(callback);
	client.send(message);
}
client.identity = "#0x01";
mySend("identify|null", response => {
	console.log("test");
	console.log(response);
});

// 	let connected = false;
// 	while(!connected) {
// 		client.identity = "#0x" + (Math.random() * (99 - 1) + 1);
// 		console.log(status, response);
// 		if (status === "ok") {
// 			connected = true;
// 		}
// 	}
// };