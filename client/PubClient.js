const zmq = require("zeromq");
class PubClient {
	constructor(port) {
		this.port = port;
		this.client = zmq.socket("sub");
		this.client.on("message", message => {
			console.log("From sub:", message);
		})
		this.client.connect("tcp://127.0.0.1:"+port);
		this.client.subscribe("softwar");
	}
};