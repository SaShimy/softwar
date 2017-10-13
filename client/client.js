const zmq = require("zeromq");
const inquirer = require("inquirer");
const requests = [];
const PORT = "4242";
const client = zmq.socket("req");
const responseHandler = response => {
	requests[requests.length-1].response = response.toString();
};
client.on("message", responseHandler);
const wait = time => {
	return new Promise(resolve => {
		setTimeout(resolve, time);
	});
};
const mySend = async message => {
	requests.push({ request: message });
	client.send(message);
	let i = 0;
	while(!requests[requests.length-1].response) {
		if (i > 100) {
			throw new Error("Timeout");
		}
		await wait(1);
		i++;
	}
	return requests[requests.length-1].response
};

const connect = async identity => {
	let connected = false;
	while(!connected) {
		client.identity = identity;
		client.connect(`tcp://127.0.0.1:${PORT}`);
		const [status, data]  = (await mySend("identify|"+identity)).split("|");
		if (status === "ok") {
			connected = true;
		}
	}
};

connect("#0x01").then(() => {
	console.log(client.identity);
});