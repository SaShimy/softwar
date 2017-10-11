const zmq = require("zeromq");
const inquirer = require('inquirer');

const client = zmq.socket("req");
client.identity = "#0x01";

client.connect("tcp://127.0.0.1:4242");
console.log("Connected to server.")

client.on("message", message => {
	console.log("Received:", message.toString());
});

client.send("forward");
// inquirer.prompt([{type: "input", message: "Say hello.", name: "message"}])
// .then(answers => {
// 	console.log(answers)
// })
// .catch(err => {
// 	console.log(err)
// })