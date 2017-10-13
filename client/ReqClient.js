const zmq = require("zeromq");
class ReqClient {
	constructor (port, identity) {
		if (!identity) {
			console.error("Identity not set.");
			process.exit(1);
		}
		this.identity = identity;
		this.port = port;
		this.client = zmq.socket("req");
		this.requests = []
		this.client.on("message", response => {
			this.requests[this.requests.length-1].response = response.toString();
		});
		this.client.identity = identity;
		this.client.connect(`tcp://127.0.0.1:${port}`);
	};

	_wait(time) {
		return new Promise(resolve => {
			setTimeout(resolve, time);
		});
	};

	send(message) {
		return (async message => {
			this.requests.push({ request: message });
			this.client.send(message);
			let i = 0;
			while(!this.requests[this.requests.length-1].response) {
				if (i > 100) {
					throw new Error("Timeout");
				}
				await this._wait(1);
				i++;
			}
			return this.requests[this.requests.length-1].response;
		})(message);
	};

	connect() {
		return (async () => {
			const [status, data] = await this.send("identify|"+this.identity);
			if (status === "ko") {
				throw new Error("Could not connect with this identity.");
			}
		})();
	}

	forward() {
		return (async () => {
			const [status, data] = await this.send("forward|null");
			if (status === "ko") {
				throw new Error("Could not go forward.");
			}
		})();
	};
	backward() {
		return (async () => {
			const [status, data] = await this.send("backward|null");
			if (status === "ko") {
				throw new Error("Could not go backward.");
			}
		})();
	};
	leftfwd() {
		return (async () => {
			const [status, data] = await this.send("leftfwd|null");
			if (status === "ko") {
				throw new Error("Could not go leftfwd.");
			}
		})();
	};
	rightfwd() {
		return (async () => {
			const [status, data] = await this.send("rightfwd|null");
			if (status === "ko") {
				throw new Error("Could not go rightfwd.");
			}
		})();
	};
	looking() {
		return (async () => {
			const [status, data] = await this.send("looking|null");
			if (status === "ko") {
				throw new Error("Could not look.");
			}
			return data;
		})();
	};
	gather() {
		return (async () => {
			const [status, data] = await this.send("gather|null");
			if (status === "ko") {
				throw new Error("Could not gather.");
			}
			return data;
		})();
	};
	watch() {
		return (async () => {
			const [status, data] = await this.send("watch|null");
			if (status === "ko") {
				throw new Error("Could not watch.");
			}
			return data;
		})();
	};
	attack() {
		return (async () => {
			const [status, data] = await this.send("attack|null");
			if (status === "ko") {
				throw new Error("Could not attack.");
			}
		})();
	};
	selfid() {
		return (async () => {
			const [status, data] = await this.send("selfid|null");
			if (status === "ko") {
				throw new Error("Could not selfid.");
			}
			return data;
		})();
	};
	selfstats() {
		return (async () => {
			const [status, data] = await this.send("selfstats|null");
			if (status === "ko") {
				throw new Error("Could not selfstats.");
			}
			return data;
		})();
	};
	inspect(process) {
		return (async (process) => {
			const [status, data] = await this.send("inspect|"+process);
			if (status === "ko") {
				throw new Error(`Could not inspect ${process}`);
			}
			return data;
		})();
	};
	next() {
		return (async (process) => {
			const [status, data] = await this.send("next|null");
			if (status === "ko") {
				throw new Error("Could not next.");
			}
		})();
	};
	jump() {
		return (async (process) => {
			const [status, data] = await this.send("jump|null");
			if (status === "ko") {
				throw new Error("Could not jump.");
			}
		})();
	};
};

module.exports = ReqClient;