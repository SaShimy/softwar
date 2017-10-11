const zmq = require("zeromq");
const inquirer = require('inquirer');

const client = zmq.socket("req");
client.identity = "test";

client.connect("tcp://127.0.0.1:4242");
console.log("Connected to server.")

client.on("message", message => {
	console.log(message.toString());
});

client.send("test1");
client.send("test2");
// inquirer.prompt([{type: "input", message: "Say hello.", name: "message"}])
// .then(answers => {
// 	console.log(answers)
// })
// .catch(err => {
// 	console.log(err)
// })