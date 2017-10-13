const zmq = require("zeromq");
class PubClient {
	constructor(port) {
		this.port = port;
		this.client = zmq.socket("sub");
		this.client.on("message", (channel, message) => {
			console.log("From sub:", JSON.parse(message.toString().match(/\{.*\}/)[0]));
		})
		this.client.connect("tcp://127.0.0.1:"+port);
		this.client.subscribe("softwar");
	};
};

module.exports = PubClient;