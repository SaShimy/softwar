const zmq = require("zeromq");
const requests = [];
const PORT = "4242";

const client = zmq.socket("req");
client.identity = "#0x01";

client.connect("tcp://127.0.0.1:4242");
client.send("identify|"+"#0x01")
console.log("Client1 connected to server.");