const zmq = require("zeromq");
const args = require("args");
const ReqClient = require("./ReqClient.js");

args.options([
	{name: "reqPort", description: "Port of the rep req server.", defaultValue: 4242},
	{name: "pubPort", description: "Port of the pub sub server.", defaultValue: 4243},
	{name: "id", description: "Identity of the client."}]);
const {reqPort, pubPort, id} = args.parse(process.argv);
const reqClient = new ReqClient(reqPort, id);

;(async () => {
	await reqClient.connect();
	console.log(reqClient.identity, "connected.");
	await reqClient.forward();
})();