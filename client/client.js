const zmq = require("zeromq");
const args = require("args");
const ReqClient = require("./ReqClient.js");
const PubClient = require("./PubClient.js");

args.options([
	{name: "reqPort", description: "Port of the rep req server.", defaultValue: 4242},
	{name: "pubPort", description: "Port of the pub sub server.", defaultValue: 4243},
	{name: "id", description: "Identity of the client."}]);
const {reqPort, pubPort, id} = args.parse(process.argv);
const reqClient = new ReqClient(reqPort, id);
const pubClient = new pubClient(pubPort);

;(async () => {
	await reqClient.connect();
	console.log(reqClient.identity, "connected.");
	console.log(await reqClient.selfstats());
	// await reqClient.forward();
})()
.catch(err => {

});